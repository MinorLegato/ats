#pragma once 

#include "ats_base.h"
#include "ats_mem.h"

// ====================================== BIT STUFF =================================== //

static void bit_set(u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] |= (1 << bit);
}

static b32 bit_get(const u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  return array[idx] & (1 << bit);
}

static void bit_clr(u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] &= ~(1 << bit);
}

// ===================================== STRING STUFF ================================= //

typedef struct string {
  usize size;
  const char* data;

#ifdef __cplusplus
  inline char operator[](usize index) const { return data[index]; }
#endif
} string;

#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).size, (s).data

#define str(text) make(string) { sizeof (text) - 1, text }
#define STR(text) { sizeof (text) - 1, text }

static string string_create(const char* str) {
  string s = {0};
  s.size = strlen(str);
  s.data = str;
  return s;
}

static b32 string_equal(string a, string b) {
  if (a.size != b.size) return false;
  return memcmp(a.data, b.data, a.size) == 0;
}

static b32 string_equal_cstr(string a, const char* b) {
  usize b_size = strlen(b);
  if (a.size != b_size) return false;
  return memcmp(a.data, b, a.size) == 0;
}

#ifdef __cplusplus

static b32 operator==(string a, string b)    { return string_equal(a, b); }
static b32 operator==(string a, const char* b) { return string_equal_cstr(a, b); }

static b32 operator!=(string a, string b)    { return !string_equal(a, b); }
static b32 operator!=(string a, const char* b) { return !string_equal_cstr(a, b); }

constexpr u32 match_hash(string str) {
  u32 hash = 5381;
  for (int i = 0; i < str.size; i++)
    hash = ((hash << 5) + hash) + str[i];
  return hash;
}

#endif

static b32 string_empty(string s) {
  return s.size == 0;
}

typedef struct split_iter {
  string current;

  const char* content;

  u32 del_table[8];
  u32 sep_table[8];
} split_iter;

static b32 split_iter_is_valid(const split_iter* it) {
  return it->current.size;
}

static void split_iter_advance(split_iter* it) {
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

  it->current = make(string) {
    (usize)(it->content - begin),
    begin,
  };
}

static split_iter split_iter_create(const char* cstr, const char* delimiters, const char* separators) {
  assert(delimiters);
  if (!separators) separators = "";

  split_iter it = {0};

  it.content = cstr;

  for (u32 i = 0; delimiters[i]; ++i) {
    bit_set(it.del_table, delimiters[i]);
  }

  for (u32 i = 0; separators[i]; ++i) {
    bit_set(it.sep_table, separators[i]);
  }

  split_iter_advance(&it);
  return it;
}

#ifdef __cplusplus

static split_iter split_iter_create(const char* cstr, const char* delimiters) {
  return split_iter_create(cstr, delimiters, NULL);
}

static inline b32 is_valid(const split_iter* it) {
  return split_iter_is_valid(it);
}

static inline void advance(split_iter* it) {
  return split_iter_advance(it);
}

#endif

// =================================================== SPATIAL MAP =================================================== //

#define SPATIAL_MAX (8 * 4096)

typedef struct sm_cell {
  void* e;
  r2 rect;
  struct sm_cell* next;
} sm_cell;

typedef struct spatial_map {
  sm_cell* table[4096];

  usize count;
  sm_cell array[SPATIAL_MAX];
} spatial_map;

static void sm_clear(spatial_map* map) {
  memset(map->table, 0, sizeof map->table);
  map->count = 0;
}

static u32 sm_index(const spatial_map* map, v2i pos) {
  u32 hash = hash_v2i(pos);
  return hash % countof(map->table);
}

static void sm_add(spatial_map* map, void* e, r2 e_rect) {
  r2i rect = {
    (i32)e_rect.min.x, (i32)e_rect.min.y,
    (i32)e_rect.max.x, (i32)e_rect.max.y,
  };

  for_r2(rect, x, y) {
    u32 index = sm_index(map, make(v2i) { x, y });
    sm_cell* cell = map->array + map->count++;

    cell->e = e;
    cell->rect = e_rect;
    cell->next = map->table[index];

    map->table[index] = cell;
  }
}

typedef struct sm_entry {
  void* e;
  r2 rect;
} sm_entry;

typedef struct sm_result {
  usize count;
  sm_entry* array;
} sm_result;

static sm_result sm_in_range(spatial_map* map, v2 pos, v2 rad, const void* ignore) {
  static sm_entry spatial_array[SPATIAL_MAX];

  sm_result result = ATS_INIT;
  result.array = spatial_array;

  r2 rect = {
    pos.x - rad.x, pos.y - rad.y,
    pos.x + rad.x, pos.y + rad.y,
  };

  r2i irect = {
    (i32)(pos.x - rad.x), (i32)(pos.y - rad.y),
    (i32)(pos.x + rad.x), (i32)(pos.y + rad.y),
  };

  for_r2(irect, x, y) {
    u32 index = sm_index(map, make(v2i) { x, y });
    for (sm_cell* it = map->table[index]; it; it = it->next) {
      b32 unique = true;

      if (it->e == ignore) continue;
      if (!r2_intersect(rect, it->rect)) continue;

      for_range(i, 0, (isize)result.count) {
        if (result.array[i].e == it->e) {
          unique = false;
          break;
        }
      }
      if (unique) {
        result.array[result.count++] = make(sm_entry) {
          it->e,
          it->rect,
        };
      }
    }
  }

  return result;
}

typedef struct sm_iter {
  sm_entry* current;
  u32 index;
  sm_result result;
} sm_iter;

static sm_iter sm_get_iterator(spatial_map* map, v2 pos, v2 rad, const void* ignore) {
  sm_iter it = ATS_INIT;

  it.result = sm_in_range(map, pos, rad, ignore);
  it.current = &it.result.array[0];

  return it;
}

static b32 sm_iter_is_valid(const sm_iter* it) {
  return it->index < it->result.count;
}

static void sm_iter_advance(sm_iter* it) {
  it->index++;
  it->current = it->result.array + it->index;
}

static void* sm_get_closest(spatial_map* map, v2 pos, f32 range, const void* ignore, b32 (*condition_proc)(void*)) {
  void* result = NULL;
  f32 distance = range;

  for_iter(sm_iter, it, sm_get_iterator(map, pos, make(v2) { range, range }, ignore)) {
    sm_entry* e = it.current;

    if (condition_proc && !condition_proc(e->e)) {
      continue;
    }

    v2 e_pos = {
      0.5f * (e->rect.min.x + e->rect.max.x),
      0.5f * (e->rect.min.y + e->rect.max.y),
    };

    f32 new_distance = v2_dist(e_pos, pos);

    if (new_distance <= distance) {
      result = e->e;
      distance = new_distance;
    }
  }

  return result;
}

static void* sm_at_position(spatial_map* map, v2 pos) {
  u32 index = sm_index(map, make(v2i) { (i32)pos.x, (i32)pos.y });

  for (sm_cell* it = map->table[index]; it; it = it->next) {
    if (r2_contains(it->rect, pos)) {
      return it->e;
    }
  }
  return NULL;
}

// ============================================ RAYCAST 2D TILEMAP ========================================== //

#define TRAVERSE_MAP_SIZE   (512)
#define TRAVERSE_MOD        (511)
#define TRAVERSE_ARRAY_SIZE (8192) // (512 * 512) / 32

typedef struct traverse_map {
  u32 array[TRAVERSE_ARRAY_SIZE];
} traverse_map;

static void tm_clear(traverse_map* map) {
  memset(map, 0, sizeof (traverse_map));
}

static inline u32 tm_get_index(const traverse_map* map, u32 x, u32 y) {
  return (y & TRAVERSE_MOD) * TRAVERSE_MAP_SIZE + (x & TRAVERSE_MOD);
}

static inline void tm_set_traversable(traverse_map* map, u32 x, u32 y) {
  bit_set(map->array, tm_get_index(map, x, y));
}

static inline b32 tm_is_traversable(const traverse_map* map, u32 x, u32 y) {
  return bit_get(map->array, tm_get_index(map, x, y));
}

static v2 tm_cast_dir(const traverse_map* map, v2 pos, v2 dir, f32 max_range) {
#if 0
  //if (!tm_is_traversable(map, pos.x, pos.y)) return pos;
#endif
  b32 kill_on_wall_hit = tm_is_traversable(map, pos.x, pos.y);
  
  //which box of the map we're in
  int map_x = (int)(pos.x);
  int map_y = (int)(pos.y);

  //length of ray from current position to next x or y-side
  f32 side_dist_x = 0;
  f32 side_dist_y = 0;

  //length of ray from one x or y-side to next x or y-side
  f32 delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabsf(1.0f / dir.x);
  f32 delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabsf(1.0f / dir.y);

  f32 perp_wall_dist = 0;

  //what direction to step in x or y-direction (either +1 or -1)
  int step_x = 0;
  int step_y = 0;

  b32 hit  = false; //was there a wall hit?
  int side = 0; //was a NS or a EW wall hit?

  //calculate step and initial sideDist
  if (dir.x < 0) {
    step_x = -1;
    side_dist_x = (pos.x - map_x) * delta_dist_x;
  } else {
    step_x = 1;
    side_dist_x = (map_x + 1.0 - pos.x) * delta_dist_x;
  }

  if (dir.y < 0) {
    step_y = -1;
    side_dist_y = (pos.y - map_y) * delta_dist_y;
  } else {
    step_y = 1;
    side_dist_y = (map_y + 1.0 - pos.y) * delta_dist_y;
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

    if (v2_dist(pos, make(v2) { (f32)map_x, (f32)map_y }) > (max_range + 1.0f)) {
      hit = true;
    }

    if (kill_on_wall_hit) {
      if (!tm_is_traversable(map, map_x, map_y)) {
        hit = true;
      }
    }
  } 

  if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
  else           perp_wall_dist = (side_dist_y - delta_dist_y);

  return v2_add(pos, v2_scale(dir, (perp_wall_dist > max_range? max_range : perp_wall_dist)));
}

static v2 tm_cast_angle(const traverse_map* map, v2 from, f32 angle, f32 max_range) {
  m2 rot = m2_rotate(angle);
  v2 dir = m2_mulv(rot, make(v2) { 0, 1 });

  return tm_cast_dir(map, from, dir, max_range);
}

// ========================================= PRIORITY QUEUE ====================================== //

typedef struct priority_queue_entry {
  f32 weight;
  v2i e;
} priority_queue_entry;

typedef struct priority_queue {
  u32 len;
  priority_queue_entry* array;
} priority_queue;

static priority_queue priority_queue_create(usize capacity) {
  priority_queue queue = ATS_INIT;

  queue.len    = 0;
  queue.array  = mem_array(priority_queue_entry, capacity);

  return queue;
}

static b32 priority_queue_empty(const priority_queue* queue) {
  return queue->len == 0;
}

static void priority_queue_clear(priority_queue* queue) {
  queue->len = 0;
}

static void priority_queue_push(priority_queue* queue, v2i e, f32 weight) {
  priority_queue_entry node = { weight, e };
  int i = queue->len + 1;
  int j = i / 2;
  while (i > 1 && queue->array[j].weight > node.weight) {
    queue->array[i] = queue->array[j];
    i = j;
    j = j / 2;
  }
  queue->array[i] = node;
  queue->len++;
}

static f32 priority_queue_pop(v2i* out, priority_queue* queue) {
  priority_queue_entry data = queue->array[1];
  queue->array[1] = queue->array[queue->len];
  queue->len--;
  int i = 1;
  while (i != queue->len + 1) {
    int k = queue->len + 1;
    int j = 2 * i;
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

typedef struct array_header {
  u32 cap;
  u32 len;
} array_header;

#define array_create(type, capacity) (type*)array_create_internal(sizeof (type), (capacity))

#define array_hdr(a)        ((array_header*)(a) - 1)
#define array_len(a)        array_hdr(a)->len
#define array_cap(a)        array_hdr(a)->cap
#define array_add(a, ...)   ((a)[array_hdr(a)->len++] = __VA_ARGS__)
#define array_rem(a, index) ((a)[(index)] = (a)[--array_len(a)])
#define array_clear(a)      (array_len(a) = 0)
#define array_sort(a, cmp)  qsort((a), array_len(a), sizeof (a)[0], cmp)

static void* array_create_internal(u32 type_size, u32 capacity) {
  array_header* hdr = (array_header*)mem_alloc(sizeof (array_header) + type_size * capacity);
  hdr->cap = capacity;
  hdr->len = 0;
  return hdr + 1;
}

#ifndef __cplusplus

#if 0
// ================================== FIXED CAPACITY HEAP ARRAY ======================================= //

struct array_header {
  u64 len;
  u64 cap;
  u8  buf[];
};

#define array(type, cap)  (type*)_array_create(sizeof (type), (cap))

#define array_hdr(a)        ((struct array_header*)(a) - 1)
#define array_len(a)        array_hdr(a)->len
#define array_cap(a)        array_hdr(a)->cap
#define array_add(a, ...)   ((a)[array_hdr(a)->len++] = __VA_ARGS__)
#define array_rem(a, index) ((a)[(index)] = (a)[--array_len(a)])
#define array_clear(a)      (array_len(a) = 0)
#define array_sort(a, cmp)  qsort((a), array_len(a), sizeof (a)[0], cmp)

static void* _array_create(u64 type_size, u64 cap) {
  struct array_header* header = mem_alloc(sizeof (struct array_header) + cap * type_size);
  header->cap = cap;
  return header->buf;
}

// ================================ HASH MAP ======================================= //

struct hash_entry {
  u64   hash;
  void* data;

  struct hash_entry* next;
};

struct hash_map {
  u64 cap;

  struct hash_entry** table;
  struct hash_entry* free_list;
};

static void hm_put(struct hash_map* map, u64 hash, void* data) {
  if (!map->table) {
    map->cap = 4096;
    map->table = mem_array(struct hash_entry*, map->cap);
  }

  u32 index = hash % map->cap;

  for (struct hash_entry* entry = map->table[index]; entry; entry = entry->next) {
    if (entry->hash == hash) {
      entry->data = data;
      return;
    }
  }

  struct hash_entry* entry = NULL;

  if (map->free_list) {
    entry           = map->free_list;
    map->free_list  = map->free_list->next;
  } else {
    entry = mem_type(struct hash_entry);
  }

  entry->hash = hash;
  entry->data = data;
  entry->next = map->table[index];

  map->table[index] = entry;
}

static void* hm_get(struct hash_map* map, u64 hash) {
  if (!map->cap) return NULL;

  u32 index = hash % map->cap;

  for (struct hash_entry* entry = map->table[index]; entry; entry = entry->next) {
    if (entry->hash == hash) {
      return entry->data;
    }
  }

  return NULL;
}

static void hm_del(struct hash_map* map, u64 hash) {
  if (!map->cap) return;

  u32 index = hash % map->cap;

  struct hash_entry* prev = NULL;
  struct hash_entry* entry = map->table[index];

  for (;entry; entry = entry->next) {
    if (entry->hash == hash) {
      if (prev) {
        prev->next = entry->next;
      } else {
        map->table[index] = entry->next;
      }
      break;
    }
    prev = entry;
  }

  entry->next = map->free_list;
  map->free_list = entry;
}
#endif

#endif
