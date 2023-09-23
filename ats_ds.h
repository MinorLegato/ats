#pragma once 

#include "ats_mem.h"

// ====================================== BIT STUFF =================================== //

static void
bit_set(unsigned* array, unsigned index) {
  unsigned idx = index >> 5;
  unsigned bit = index & 31;
  array[idx] |= (1 << bit);
}

static unsigned
bit_get(const unsigned* array, unsigned index) {
  unsigned idx = index >> 5;
  unsigned bit = index & 31;
  return array[idx] & (1 << bit);
}

static void
bit_clr(unsigned* array, unsigned index) {
  unsigned idx = index >> 5;
  unsigned bit = index & 31;
  array[idx] &= ~(1 << bit);
}

// ===================================== STRING STUFF ================================= //

typedef struct string {
  unsigned size;
  const char* data;
} string;

#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).size, (s).data

#define str(text) (string) { sizeof (text) - 1, text }
#define STR(text) { sizeof (text) - 1, text }

static string
string_create(const char* str) {
  string s = {0};
  s.size = strlen(str);
  s.data = str;
  return s;
}

static int
string_equal(string a, string b) {
  if (a.size != b.size) return 0;
  return memcmp(a.data, b.data, a.size) == 0;
}

static int
string_equal_cstr(string a, const char* b) {
  unsigned b_size = strlen(b);
  if (a.size != b_size) return 0;
  return memcmp(a.data, b, a.size) == 0;
}

static int
string_empty(string s) {
  return s.size == 0;
}

typedef struct {
  string current;

  const char* content;

  unsigned del_table[8];
  unsigned sep_table[8];
} split_iter;

static int
split_iter_is_valid(const split_iter* it) {
  return it->current.size > 0;
}

static void
split_iter_advance(split_iter* it) {
  while (*it->content && bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
    it->content++;
  }

  const char* begin = it->content;

  if (bit_get(it->sep_table, *it->content)) {
    it->content++;
  } else {
    while (*it->content && !bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
      it->content++;
    }
  }

  it->current = (string) {
    (unsigned)(it->content - begin),
    begin,
  };
}

static split_iter
split_iter_create(const char* cstr, const char* delimiters, const char* separators) {
  if (!delimiters) delimiters = "";
  if (!separators) separators = "";

  split_iter it = {0};

  it.content = cstr;

  for (unsigned i = 0; delimiters[i]; ++i) {
    bit_set(it.del_table, delimiters[i]);
  }

  for (unsigned i = 0; separators[i]; ++i) {
    bit_set(it.sep_table, separators[i]);
  }

  split_iter_advance(&it);
  return it;
}

// =================================================== SPATIAL MAP =================================================== //

#define SPATIAL_MAX (8 * 4096)

typedef struct sm_cell {
  void* e;
  r2 rect;
  struct sm_cell* next;
} sm_cell;

typedef struct spatial_map {
  sm_cell* table[4096];

  unsigned count;
  sm_cell array[SPATIAL_MAX];
} spatial_map;

static void
sm_clear(spatial_map* map) {
  memset(map->table, 0, sizeof map->table);
  map->count = 0;
}

static unsigned
sm_index(const spatial_map* map, v2i pos) {
  unsigned hash = hash_v2i(pos);
  return hash % 4096;
}

static void
sm_add(spatial_map* map, void* e, r2 e_rect) {
  r2i rect = {
    (int)e_rect.min.x, (int)e_rect.min.y,
    (int)e_rect.max.x, (int)e_rect.max.y,
  };

  for (int y = rect.min.y; y <= rect.max.y; ++y) {
    for (int x = rect.min.x; x <= rect.max.x; ++x) {
      unsigned index = sm_index(map, (v2i) { x, y });
      sm_cell* cell = map->array + map->count++;

      cell->e = e;
      cell->rect = e_rect;
      cell->next = map->table[index];

      map->table[index] = cell;
    }
  }
}

typedef struct sm_entry {
  void* e;
  r2 rect;
} sm_entry;

typedef struct sm_result {
  unsigned  count;
  sm_entry* array;
} sm_result;

static sm_result
sm_in_range(spatial_map* map, v2 pos, v2 rad, const void* ignore) {
  static sm_entry spatial_array[SPATIAL_MAX];

  sm_result result = {0};
  result.array = spatial_array;

  r2 rect = {
    pos.x - rad.x, pos.y - rad.y,
    pos.x + rad.x, pos.y + rad.y,
  };

  r2i irect = {
    (int)(pos.x - rad.x), (int)(pos.y - rad.y),
    (int)(pos.x + rad.x), (int)(pos.y + rad.y),
  };

  for (int y = irect.min.y; y <= irect.max.y; ++y) {
    for (int x = irect.min.x; x <= irect.max.x; ++x) {
      unsigned index = sm_index(map, (v2i) { x, y });
      for (sm_cell* it = map->table[index]; it; it = it->next) {
        int unique = 1;

        if (it->e == ignore) continue;
        if (!r2_intersect(rect, it->rect)) continue;

        for (int i = 0; i < result.count; ++i) {
          if (result.array[i].e == it->e) {
            unique = 0;
            break;
          }
        }
        if (unique) {
          result.array[result.count++] = (sm_entry) {
            it->e,
            it->rect,
          };
        }
      }
    }
  }

  return result;
}

typedef struct sm_iter {
  sm_entry* current;
  unsigned  index;
  sm_result result;
} sm_iter;

static sm_iter
sm_get_iterator(spatial_map* map, v2 pos, v2 rad, const void* ignore) {
  sm_iter it = {0};

  it.result = sm_in_range(map, pos, rad, ignore);
  it.current = &it.result.array[0];

  return it;
}

static int
sm_iter_is_valid(const sm_iter* it) {
  return it->index < it->result.count;
}

static void
sm_iter_advance(sm_iter* it) {
  it->index++;
  it->current = it->result.array + it->index;
}

static void*
sm_get_closest(spatial_map* map, v2 pos, float range, const void* ignore, int (*condition_proc)(void*)) {
  void* result = 0;
  float distance = range;

  for (sm_iter it = sm_get_iterator(map, pos, (v2) { range, range }, ignore);
       sm_iter_is_valid(&it);
       sm_iter_advance(&it)) {
    sm_entry* e = it.current;

    if (condition_proc && !condition_proc(e->e)) {
      continue;
    }

    v2 e_pos = {
      0.5f * (e->rect.min.x + e->rect.max.x),
      0.5f * (e->rect.min.y + e->rect.max.y),
    };

    float new_distance = v2_dist(e_pos, pos);

    if (new_distance <= distance) {
      result = e->e;
      distance = new_distance;
    }
  }

  return result;
}

static void*
sm_at_position(spatial_map* map, v2 pos) {
  unsigned index = sm_index(map, (v2i) { (int)pos.x, (int)pos.y });
  for (sm_cell* it = map->table[index]; it; it = it->next) {
    if (r2_contains(it->rect, pos)) {
      return it->e;
    }
  }
  return 0;
}

// ============================================ RAYCAST 2D TILEMAP ========================================== //

#define TRAVERSE_MAP_SIZE   (512)
#define TRAVERSE_MOD        (511)
#define TRAVERSE_ARRAY_SIZE (8192) // (512 * 512) / 32

typedef struct {
  unsigned array[TRAVERSE_ARRAY_SIZE];
} traverse_map;

static void
tm_clear(traverse_map* map) {
  memset(map, 0, sizeof (traverse_map));
}

static unsigned
tm_get_index(const traverse_map* map, unsigned x, unsigned y) {
  return (y & TRAVERSE_MOD) * TRAVERSE_MAP_SIZE + (x & TRAVERSE_MOD);
}

static void
tm_set_traversable(traverse_map* map, unsigned x, unsigned y) {
  bit_set(map->array, tm_get_index(map, x, y));
}

static int
tm_is_traversable(const traverse_map* map, unsigned x, unsigned y) {
  return bit_get(map->array, tm_get_index(map, x, y));
}

static v2
tm_cast_dir(const traverse_map* map, v2 pos, v2 dir, float max_range) {
#if 0
  //if (!tm_is_traversable(map, pos.x, pos.y)) return pos;
#endif
  int kill_on_wall_hit = tm_is_traversable(map, (int)pos.x, (int)pos.y);

  //which box of the map we're in
  int map_x = (int)(pos.x);
  int map_y = (int)(pos.y);

  //length of ray from current position to next x or y-side
  float side_dist_x = 0;
  float side_dist_y = 0;

  //length of ray from one x or y-side to next x or y-side
  float delta_dist_x = (dir.x == 0.0f) ? 1e30f : fabsf(1.0f / dir.x);
  float delta_dist_y = (dir.y == 0.0f) ? 1e30f : fabsf(1.0f / dir.y);

  float perp_wall_dist = 0;

  //what direction to step in x or y-direction (either +1 or -1)
  int step_x = 0;
  int step_y = 0;

  int hit  = 0; //was there a wall hit?
  int side = 0; //was a NS or a EW wall hit?

  //calculate step and initial sideDist
  if (dir.x < 0) {
    step_x = -1;
    side_dist_x = (pos.x - map_x) * delta_dist_x;
  } else {
    step_x = 1;
    side_dist_x = (map_x + 1.0f - pos.x) * delta_dist_x;
  }

  if (dir.y < 0) {
    step_y = -1;
    side_dist_y = (pos.y - map_y) * delta_dist_y;
  } else {
    step_y = 1;
    side_dist_y = (map_y + 1.0f - pos.y) * delta_dist_y;
  }

  while (!hit) {
    //jump to next map square, either in x-direction, or in y-direction
    if (side_dist_x < side_dist_y) {
      side_dist_x += delta_dist_x;
      map_x += step_x;
      side = 0;
    } else {
      side_dist_y += delta_dist_y;
      map_y += step_y;
      side = 1;
    }

    if (v2_dist(pos, (v2) { (float)map_x, (float)map_y }) > (max_range + 1.0f)) {
      hit = 1;
    }

    if (kill_on_wall_hit) {
      if (!tm_is_traversable(map, map_x, map_y)) {
        hit = 1;
      }
    }
  } 

  if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
  else           perp_wall_dist = (side_dist_y - delta_dist_y);

  return v2_add(pos, v2_scale(dir, (perp_wall_dist > max_range? max_range : perp_wall_dist)));
}

static v2
tm_cast_angle(const traverse_map* map, v2 from, float angle, float max_range) {
  m2 rot = m2_rotate(angle);
  v2 dir = m2_mulv(rot, (v2) { 0, 1 });

  return tm_cast_dir(map, from, dir, max_range);
}

// =========================================== RAY ITER ========================================== //

typedef struct {
  v2 pos;
  v2 dir;

  //which box of the map we're in
  int map_x;
  int map_y;

  //length of ray from current position to next x or y-side
  float side_dist_x;
  float side_dist_y;

  //length of ray from one x or y-side to next x or y-side
  float delta_dist_x;
  float delta_dist_y;

  //what direction to step in x or y-direction (either +1 or -1)
  int step_x;
  int step_y;

  int side; // was a NS or a EW wall hit?
} ray_iter;

static ray_iter
ray_iter_create(v2 pos, v2 dir) {
  ray_iter it = {0};

  it.pos = pos;
  it.dir = dir;

  //which box of the map we're in
  it.map_x = (int)(pos.x);
  it.map_y = (int)(pos.y);

  //length of ray from current position to next x or y-side
  it.side_dist_x = 0;
  it.side_dist_y = 0;

  //length of ray from one x or y-side to next x or y-side
  it.delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabsf(1.0f / dir.x);
  it.delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabsf(1.0f / dir.y);

  //what direction to step in x or y-direction (either +1 or -1)
  it.step_x = 0;
  it.step_y = 0;

  it.side = 0; //was a NS or a EW wall hit?

  //calculate step and initial sideDist
  if (dir.x < 0) {
    it.step_x = -1;
    it.side_dist_x = (pos.x - it.map_x) * it.delta_dist_x;
  } else {
    it.step_x = 1;
    it.side_dist_x = (it.map_x + 1.0 - pos.x) * it.delta_dist_x;
  }

  if (dir.y < 0) {
    it.step_y = -1;
    it.side_dist_y = (pos.y - it.map_y) * it.delta_dist_y;
  } else {
    it.step_y = 1;
    it.side_dist_y = (it.map_y + 1.0 - pos.y) * it.delta_dist_y;
  }

  return it;
}

static int
ray_iter_is_valid(const ray_iter* it) {
  return 1;
}

static void
ray_iter_advance(ray_iter* it) {
  //jump to next map square, either in x-direction, or in y-direction
  if (it->side_dist_x < it->side_dist_y) {
    it->side_dist_x += it->delta_dist_x;
    it->map_x += it->step_x;
    it->side = 0;
  } else {
    it->side_dist_y += it->delta_dist_y;
    it->map_y += it->step_y;
    it->side = 1;
  }
}

static v2
ray_iter_get_position(const ray_iter* it) {
  float perp_wall_dist = 0;

  if (it->side == 0) perp_wall_dist = (it->side_dist_x - it->delta_dist_x);
  else               perp_wall_dist = (it->side_dist_y - it->delta_dist_y);

  return v2_add(it->pos, v2_scale(it->dir, perp_wall_dist));
}

// ========================================= PRIORITY QUEUE ====================================== //

typedef struct {
  float weight;
  v2i   e;
} priority_queue_entry;

typedef struct {
  unsigned len;
  priority_queue_entry* array;
} priority_queue;

static priority_queue
priority_queue_create(unsigned capacity) {
  priority_queue queue = {0};

  queue.len   = 0;
  queue.array = mem_array(priority_queue_entry, capacity);

  return queue;
}

static int
priority_queue_empty(const priority_queue* queue) {
  return queue->len == 0;
}

static void
priority_queue_clear(priority_queue* queue) {
  queue->len = 0;
}

static void
priority_queue_push(priority_queue* queue, v2i e, float weight) {
  priority_queue_entry node = { weight, e };
  unsigned i = queue->len + 1;
  unsigned j = i / 2;
  while (i > 1 && queue->array[j].weight > node.weight) {
    queue->array[i] = queue->array[j];
    i = j;
    j = j / 2;
  }
  queue->array[i] = node;
  queue->len++;
}

static float
priority_queue_pop(v2i* out, priority_queue* queue) {
  priority_queue_entry data = queue->array[1];
  queue->array[1] = queue->array[queue->len];
  queue->len--;
  unsigned i = 1;
  while (i != queue->len + 1) {
    unsigned k = queue->len + 1;
    unsigned j = 2 * i;
    if (j <= queue->len && queue->array[j].weight < queue->array[k].weight) {
      k = j;
    }
    if (j + 1 <= queue->len && queue->array[j + 1].weight < queue->array[k].weight) {
      k = j + 1;
    }
    queue->array[i] = queue->array[k];
    i = k;
  }
  *out = data.e;
  return data.weight;
}

// ================================ FIXED CAPACITY HEAP ARRAY ==================================== //

typedef struct {
  unsigned cap;
  unsigned len;
} array_header;

#define array_create(type, capacity) (type*)array_create_internal(sizeof (type), (capacity))

#define array_hdr(a)        ((array_header*)(a) - 1)
#define array_len(a)        array_hdr(a)->len
#define array_cap(a)        array_hdr(a)->cap
#define array_add(a, ...)   ((a)[array_hdr(a)->len++] = __VA_ARGS__)
#define array_rem(a, index) ((a)[(index)] = (a)[--array_len(a)])
#define array_clear(a)      (array_len(a) = 0)
#define array_sort(a, cmp)  qsort((a), array_len(a), sizeof (a)[0], cmp)

static void*
array_create_internal(unsigned type_size, unsigned capacity) {
  array_header* hdr = (array_header*)mem_alloc(sizeof (array_header) + type_size * capacity);
  hdr->cap = capacity;
  hdr->len = 0;
  return hdr + 1;
}

