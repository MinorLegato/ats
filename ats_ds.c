#include "ats.h"

// ====================================== BIT STUFF =================================== //

ATS_API void bit_set(u32* array, u32 index)
{
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] |= (1 << bit);
}

ATS_API b32 bit_get(u32* array, u32 index)
{
  u32 idx = index >> 5;
  u32 bit = index & 31;
  return array[idx] & (1 << bit);
}

ATS_API void bit_clr(u32* array, u32 index)
{
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] &= ~(1 << bit);
}

// ========================================== S8 ====================================== //

ATS_API b32 str_iter_is_valid(str_iter* it)
{
  return it->current != 0;
}

ATS_API void str_iter_advance(str_iter* it)
{
  if (it->end) {
    *it->end = it->saved;
    it->current = it->end;
    it->end = 0;
  }
  while (*it->current && bit_get(it->del_table, it->current[0]) && !bit_get(it->sep_table, it->current[0])) {
    it->current++;
  }
  if (!(*it->current)) {
    it->current = 0;
    return;
  }
  it->end = it->current + 1;
  if (!bit_get(it->sep_table, it->current[0])) {
    while (*it->end && !bit_get(it->del_table, it->end[0]) && !bit_get(it->sep_table, it->end[0])) {
      it->end++;
    }
  }
  it->saved = *it->end;
  *it->end = '\0';
}

ATS_API str_iter str_iter_create(char* content, const char* delimiters, const char* separators)
{
  str_iter it = {0};
  it.current = content;

  if (!delimiters) delimiters = "";
  if (!separators) separators = "";

  for (u32 i = 0; delimiters[i]; ++i) {
    bit_set(it.del_table, delimiters[i]);
  }

  for (u32 i = 0; separators[i]; ++i) {
    bit_set(it.sep_table, separators[i]);
  }

  str_iter_advance(&it);
  return it;
}

// =========================================== RAY ITER 2D ========================================== //

ATS_API ray_iter ray_iter_create(v2 pos, v2 dir)
{
  ray_iter it = {0};

  it.pos = pos;
  it.dir = dir;

  //which box of the map we're in
  it.map_x = (i32)(pos.x);
  it.map_y = (i32)(pos.y);

  //length of ray from current position to next x or y-side
  it.side_dist_x = 0;
  it.side_dist_y = 0;

  //length of ray from one x or y-side to next x or y-side
  it.delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabs(1.0f / dir.x);
  it.delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabs(1.0f / dir.y);

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

ATS_API b32 ray_iter_is_valid(ray_iter* it)
{
  return 1;
}

ATS_API void ray_iter_advance(ray_iter* it)
{
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

ATS_API v2 ray_iter_get_position(ray_iter* it)
{
  f32 perp_wall_dist = 0;
  if (it->side == 0) perp_wall_dist = (it->side_dist_x - it->delta_dist_x);
  else               perp_wall_dist = (it->side_dist_y - it->delta_dist_y);
  return v2_add(it->pos, v2_scale(it->dir, perp_wall_dist));
}

ATS_API v2 ray_iter_get_normal(ray_iter* it)
{
  if (it->side == 0) return v2(-1.0f * sign(it->dir.x), 0);
  else               return v2(0, -1.0f * sign(it->dir.y));
  return v2(0);
}

// =========================================== RAY ITER 3D ========================================== //

ATS_API ray3_iter ray3_iter_create(v3 pos, v3 dir)
{
  ray3_iter it = {0};

  it.pos = pos;
  it.dir = dir;

  //which box of the map we're in
  it.map_x = (i32)pos.x;
  it.map_y = (i32)pos.y;
  it.map_z = (i32)pos.z;

  //length of ray from one x or y-side to next x or y-side
  it.delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabs(1.0f / dir.x);
  it.delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabs(1.0f / dir.y);
  it.delta_dist_z = (dir.z == 0.0f) ? 1e30 : fabs(1.0f / dir.z);

  it.side = 0; //was a NS, EW or a UD wall hit?

  //calculate step and initial side dist
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

  if (dir.z < 0) {
    it.step_z = -1;
    it.side_dist_z = (pos.z - it.map_z) * it.delta_dist_z;
  } else {
    it.step_z = 1;
    it.side_dist_z = (it.map_z + 1.0 - pos.z) * it.delta_dist_z;
  }

  return it;
}

ATS_API b32 ray3_iter_is_valid(ray3_iter* it)
{
  return 1;
}

ATS_API void ray3_iter_advance(ray3_iter* it)
{
  // jump to next map square, either in x, y, or in z direction.
  if ((it->side_dist_x < it->side_dist_y) && (it->side_dist_x < it->side_dist_z)) {
    it->side_dist_x += it->delta_dist_x;
    it->map_x += it->step_x;
    it->side = 0;
  } else if ((it->side_dist_y < it->side_dist_z) && (it->side_dist_y < it->side_dist_z)) {
    it->side_dist_y += it->delta_dist_y;
    it->map_y += it->step_y;
    it->side = 1;
  } else {
    it->side_dist_z += it->delta_dist_z;
    it->map_z += it->step_z;
    it->side = 2;
  }
}

ATS_API v3 ray3_iter_get_position(ray3_iter* it)
{
  f32 perp_wall_dist = 0;
  if      (it->side == 0) perp_wall_dist = (it->side_dist_x - it->delta_dist_x);
  else if (it->side == 1) perp_wall_dist = (it->side_dist_y - it->delta_dist_y);
  else                    perp_wall_dist = (it->side_dist_z - it->delta_dist_z);
  return v3_add(it->pos, v3_scale(it->dir, perp_wall_dist));
}

ATS_API v3 ray3_iter_get_normal(ray3_iter* it)
{
  if      (it->side == 0) return v3(-1.0f * sign(it->dir.x), 0, 0);
  else if (it->side == 1) return v3(0, -1.0f * sign(it->dir.y), 0);
  else                    return v3(0, 0, -1.0f * sign(it->dir.z));
  return v3(0);
}

// ========================================= PRIORITY QUEUE ====================================== //

ATS_API path_queue path_queue_create(usize capacity)
{
  path_queue queue = {0};
  queue.len = 0;
  queue.buf = mem_array(path_node, capacity);
  return queue;
}

ATS_API b32 path_queue_empty(path_queue* queue)
{
  return queue->len == 0;
}

ATS_API void path_queue_clear(path_queue* queue)
{
  queue->len = 0;
}

ATS_API void path_queue_push(path_queue* queue, path_node node)
{
  u32 i = queue->len + 1;
  u32 j = i / 2;
  while (i > 1 && queue->buf[j].w > node.w) {
    queue->buf[i] = queue->buf[j];
    i = j;
    j = j / 2;
  }
  queue->buf[i] = node;
  queue->len++;
}

ATS_API path_node path_queue_pop(path_queue* queue)
{
  path_node node = queue->buf[1];
  queue->buf[1] = queue->buf[queue->len];
  queue->len--;
  u32 i = 1;
  while (i != queue->len + 1) {
    u32 k = queue->len + 1;
    u32 j = 2 * i;
    if (j <= queue->len && queue->buf[j].w < queue->buf[k].w) {
      k = j;
    }
    if (j + 1 <= queue->len && queue->buf[j + 1].w < queue->buf[k].w) {
      k = j + 1;
    }
    queue->buf[i] = queue->buf[k];
    i = k;
  }
  return node;
}

// =================================================== SPATIAL MAP =================================================== //

ATS_API void sm_clear(spatial_map* map)
{
  for (u32 i = 0; i < SPATIAL_TABLE_MAX; ++i) {
    map->table[i] = 0;
  }
}

ATS_API u32 sm_index(spatial_map* map, i32 x, i32 y)
{
  u32 hash = hash2i(x, y);
  return hash & SPATIAL_TABLE_MOD;
}

ATS_API void sm_add(spatial_map* map, void* e, r2 e_rect)
{
  r2i rect = {
    { (i32)e_rect.min.x, (i32)e_rect.min.y },
    { (i32)e_rect.max.x, (i32)e_rect.max.y },
  };
  for_r2(rect, x, y)
  {
    u32 index = sm_index(map, x, y);
    sm_node* node = mem_type(sm_node);
    node->e = e;
    node->rect = e_rect;
    node->next = map->table[index];
    map->table[index] = node;
  }
}

ATS_API sm_node* sm_in_range(spatial_map* map, v2 pos, v2 rad, void* ignore)
{
  sm_node* result = 0;
  r2 rect = {
    { pos.x - rad.x, pos.y - rad.y },
    { pos.x + rad.x, pos.y + rad.y },
  };
  r2i irect = {
    { (i32)(pos.x - rad.x), (i32)(pos.y - rad.y) },
    { (i32)(pos.x + rad.x), (i32)(pos.y + rad.y) },
  };

  for_r2(irect, x, y) {
    u32 index = sm_index(map, x, y);
    for (sm_node* it = map->table[index]; it; it = it->next) {
      b32 unique = 1;
      if ((it->e == ignore) || !r2_intersect(rect, it->rect)) continue;
      for (sm_node* r = result; r; r = r->next) {
        if (r->e == it->e) {
          unique = 0;
          break;
        }
      }
      if (unique) {
        sm_node* n = mem_type(sm_node);
        *n = *it;
        n->next = result;
        result = n;
      }
    }
  }
  return result;
}

ATS_API void* sm_get_closest(spatial_map* map, v2 pos, f32 range, void* ignore, b32 (*condition_proc)(void*))
{
  void* result = NULL;
  f32 distance = range;
  for (sm_node* it = sm_in_range(map, pos, v2(range, range), ignore); it; it = it->next) {
    if (condition_proc && !condition_proc(it->e)) continue;
    v2 e_pos = {
      0.5f * (it->rect.min.x + it->rect.max.x),
      0.5f * (it->rect.min.y + it->rect.max.y),
    };
    f32 new_distance = v2_dist(e_pos, pos);
    if (new_distance <= distance) {
      result = it->e;
      distance = new_distance;
    }
  }
  return result;
}

ATS_API void* sm_at_position(spatial_map* map, v2 pos)
{
  u32 index = sm_index(map, pos.x, pos.y);
  for (sm_node* it = map->table[index]; it; it = it->next) {
    if (r2_contains(it->rect, pos)) {
      return it->e;
    }
  }
  return 0;
}

