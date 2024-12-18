#pragma once 

// ====================================== BIT STUFF =================================== //

static void bit_set(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    array[idx] |= (1 << bit);
}

static b32 bit_get(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    return array[idx] & (1 << bit);
}

static void bit_clr(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    array[idx] &= ~(1 << bit);
}

// ========================================== S8 ====================================== //

typedef struct s8 s8;
struct s8 {
    u8* buf;
    isize len;

#ifdef __cplusplus
    s8() = default;
    s8(u8* b, isize l) : buf { b }, len { l } {};
    template <isize N> s8(const char (&s)[N]) : buf { (u8*)s }, len { N - 1 } {}
#endif
};

#define S8_FMT "%.*s"
#define S8_ARG(s) (i32)(s).len, (s).buf

#ifndef __cplusplus
#define s8(text) (s8) { (u8*)(text), sizeof (text) - 1 }
#define S8(text) { (u8*)(text), sizeof (text) - 1 }
#endif

static b32 s8_eq(s8 a, s8 b) {
    if (a.len != b.len) return 0;
    for (u32 i = 0; i < a.len; ++i) {
        if (a.buf[i] != b.buf[i]) {
            return 0;
        }
    }
    return 1;
}

static b32 s8_empty(s8 s) {
    return s.len == 0;
}

static s8 s8_append(s8 a, s8 b) {
    return s8(0);
}

#define S8_ITER_TABLE (256 >> 5)

typedef struct s8_iter s8_iter;
struct s8_iter {
    s8 current;
    s8 content;

    u32 idx;

    u32 del_table[S8_ITER_TABLE];
    u32 sep_table[S8_ITER_TABLE];
};

static b32 s8_iter_is_valid(s8_iter* it)
{
    return it->current.len > 0;
}

static void s8_iter_advance(s8_iter* it)
{
    while ((it->content.len > 0) && bit_get(it->del_table, it->content.buf[0]) && !bit_get(it->sep_table, it->content.buf[0])) {
        it->content.buf++;
        it->content.len--;
    }

    it->current = it->content;
    it->current.len = it->content.len != 0;
    it->content.buf += it->current.len;
    it->content.len -= it->current.len;

    if (!bit_get(it->sep_table, it->current.buf[0])) {
        while ((it->current.len > 0) && !bit_get(it->del_table, it->content.buf[0]) && !bit_get(it->sep_table, it->content.buf[0])) {
            it->content.buf++;
            it->content.len--;
            it->current.len++;
        }
    }
}

static s8_iter s8_iter_create(s8 content, s8 delimiters, s8 separators)
{
    s8_iter it = ZERO;
    it.content = content;
    for (u32 i = 0; i < delimiters.len; ++i) {
        bit_set(it.del_table, delimiters.buf[i]);
    }
    for (u32 i = 0; i < separators.len; ++i) {
        bit_set(it.sep_table, separators.buf[i]);
    }
    s8_iter_advance(&it);
    return it;
}

// =========================================== RAY ITER 2D ========================================== //

typedef struct ray_iter ray_iter;
struct ray_iter {
    v2 pos;
    v2 dir;

    //which box of the map we're in
    i32 map_x;
    i32 map_y;

    //length of ray from current position to next x or y-side
    f32 side_dist_x;
    f32 side_dist_y;

    //length of ray from one x or y-side to next x or y-side
    f32 delta_dist_x;
    f32 delta_dist_y;

    //what direction to step in x or y-direction (either +1 or -1)
    i32 step_x;
    i32 step_y;

    i32 side; // was a NS or a EW wall hit?
};

static ray_iter ray_iter_create(v2 pos, v2 dir)
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
    it.delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabsf(1.0f / dir.x);
    it.delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabsf(1.0f / dir.y);

    //what direction to step in x or y-direction (either +1 or -1)
    it.step_x = 0;
    it.step_y = 0;

    it.side = 0; //was a NS or a EW wall hit?

    //calculate step and initial sideDist
    if (dir.x < 0)
    {
        it.step_x = -1;
        it.side_dist_x = (pos.x - it.map_x) * it.delta_dist_x;
    }
    else
    {
        it.step_x = 1;
        it.side_dist_x = (it.map_x + 1.0 - pos.x) * it.delta_dist_x;
    }

    if (dir.y < 0)
    {
        it.step_y = -1;
        it.side_dist_y = (pos.y - it.map_y) * it.delta_dist_y;
    }
    else
    {
        it.step_y = 1;
        it.side_dist_y = (it.map_y + 1.0 - pos.y) * it.delta_dist_y;
    }

    return it;
}

static b32 ray_iter_is_valid(ray_iter* it)
{
    return 1;
}

static void ray_iter_advance(ray_iter* it)
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

static v2 ray_iter_get_position(ray_iter* it)
{
    f32 perp_wall_dist = 0;
    if (it->side == 0) perp_wall_dist = (it->side_dist_x - it->delta_dist_x);
    else               perp_wall_dist = (it->side_dist_y - it->delta_dist_y);
    return v2_add(it->pos, v2_scale(it->dir, perp_wall_dist));
}

static v2 ray_iter_get_normal(ray_iter* it)
{
    if (it->side == 0) return v2(-1.0f * sign(it->dir.x), 0);
    else               return v2(0, -1.0f * sign(it->dir.y));
    return v2(0);
}

// =========================================== RAY ITER 3D ========================================== //

typedef struct ray3_iter ray3_iter;
struct ray3_iter {
    v3 pos;
    v3 dir;

    //which box of the map we're in
    i32 map_x;
    i32 map_y;
    i32 map_z;

    //length of ray from current position to next x or y-side
    f32 side_dist_x;
    f32 side_dist_y;
    f32 side_dist_z;

    //length of ray from one x or y-side to next x or y-side
    f32 delta_dist_x;
    f32 delta_dist_y;
    f32 delta_dist_z;

    //what direction to step in x or y-direction (either +1 or -1)
    i32 step_x;
    i32 step_y;
    i32 step_z;

    i32 side; // was a NS or a EW wall hit?
};

static ray3_iter ray3_iter_create(v3 pos, v3 dir)
{
    ray3_iter it = {0};

    it.pos = pos;
    it.dir = dir;

    //which box of the map we're in
    it.map_x = (i32)pos.x;
    it.map_y = (i32)pos.y;
    it.map_z = (i32)pos.z;

    //length of ray from one x or y-side to next x or y-side
    it.delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabsf(1.0f / dir.x);
    it.delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabsf(1.0f / dir.y);
    it.delta_dist_z = (dir.z == 0.0f) ? 1e30 : fabsf(1.0f / dir.z);

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

static b32 ray3_iter_is_valid(ray3_iter* it)
{
    return 1;
}

static void ray3_iter_advance(ray3_iter* it)
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

static v3 ray3_iter_get_position(ray3_iter* it)
{
    f32 perp_wall_dist = 0;
    if      (it->side == 0) perp_wall_dist = (it->side_dist_x - it->delta_dist_x);
    else if (it->side == 1) perp_wall_dist = (it->side_dist_y - it->delta_dist_y);
    else                    perp_wall_dist = (it->side_dist_z - it->delta_dist_z);
    return v3_add(it->pos, v3_scale(it->dir, perp_wall_dist));
}

static v3 ray3_iter_get_normal(ray3_iter* it) {
    if      (it->side == 0) return v3(-1.0f * sign(it->dir.x), 0, 0);
    else if (it->side == 1) return v3(0, -1.0f * sign(it->dir.y), 0);
    else                    return v3(0, 0, -1.0f * sign(it->dir.z));
    return v3(0);
}

// ========================================= PRIORITY QUEUE ====================================== //

#define path_node(...) make(path_node) { __VA_ARGS__ }
typedef struct {
    f32 w;
    i32 x;
    i32 y;
    i32 z;
} path_node;

typedef struct {
    u32 len;
    path_node* buf;
} path_queue;

static path_queue path_queue_create(usize capacity)
{
    path_queue queue = {0};
    queue.len = 0;
    queue.buf = mem_array(path_node, capacity);
    return queue;
}

static b32 path_queue_empty(path_queue* queue)
{
    return queue->len == 0;
}

static void path_queue_clear(path_queue* queue)
{
    queue->len = 0;
}

static void path_queue_push(path_queue* queue, path_node node)
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

static path_node path_queue_pop(path_queue* queue)
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

#define SPATIAL_TABLE_MAX 4096
#define SPATIAL_TABLE_MOD 4095

typedef struct sm_node sm_node;
struct sm_node {
    sm_node* next;
    void* e;
    r2 rect;
};

typedef struct {
    sm_node* table[SPATIAL_TABLE_MAX];
} spatial_map;

static void sm_clear(spatial_map* map)
{
    for (u32 i = 0; i < SPATIAL_TABLE_MAX; ++i) {
        map->table[i] = 0;
    }
}

static u32 sm_index(spatial_map* map, i32 x, i32 y)
{
    u32 hash = hash2i(x, y);
    return hash & SPATIAL_TABLE_MOD;
}

static void sm_add(spatial_map* map, void* e, r2 e_rect)
{
    r2i rect = {
        { (i32)e_rect.min.x, (i32)e_rect.min.y },
        { (i32)e_rect.max.x, (i32)e_rect.max.y },
    };
    for_r2(rect, x, y) {
        u32 index = sm_index(map, x, y);
        sm_node* node = mem_type(sm_node);
        node->e = e;
        node->rect = e_rect;
        node->next = map->table[index];
        map->table[index] = node;
    }
}

typedef struct sm_entry sm_entry;
struct sm_entry {
    void* e;
    r2 rect;
};

typedef struct sm_result sm_result;
struct sm_result {
    usize count;
    sm_entry* array;
};

static sm_node* sm_in_range(spatial_map* map, v2 pos, v2 rad, void* ignore)
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
            if ((it->e == ignore) || !r2_intersect(rect, it->rect)) {
                continue;
            }
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

static void* sm_get_closest(spatial_map* map, v2 pos, f32 range, void* ignore, b32 (*condition_proc)(void*))
{
    void* result = NULL;
    f32 distance = range;
    for (sm_node* it = sm_in_range(map, pos, v2(range, range), ignore); it; it = it->next) {
        if (condition_proc && !condition_proc(it->e)) {
            continue;
        }
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

static void* sm_at_position(spatial_map* map, v2 pos)
{
    u32 index = sm_index(map, pos.x, pos.y);
    for (sm_node* it = map->table[index]; it; it = it->next) {
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
    u32 array[TRAVERSE_ARRAY_SIZE];
} traverse_map;

static void tm_clear(traverse_map* map)
{
    for (u32 i = 0; i < TRAVERSE_ARRAY_SIZE; ++i) {
        map->array[i] = 0;
    }
}

static u32 tm_get_index(traverse_map* map, u32 x, u32 y)
{
    return (y & TRAVERSE_MOD) * TRAVERSE_MAP_SIZE + (x & TRAVERSE_MOD);
}

static void tm_set_traversable(traverse_map* map, u32 x, u32 y)
{
    bit_set(map->array, tm_get_index(map, x, y));
}

static b32 tm_is_traversable(traverse_map* map, u32 x, u32 y)
{
    return bit_get(map->array, tm_get_index(map, x, y));
}

static v2 tm_cast_dir(traverse_map* map, v2 pos, v2 dir, f32 max_range)
{
    b32 kill_on_wall_hit = tm_is_traversable(map, (i32)pos.x, (i32)pos.y);

    //which box of the map we're in
    int map_x = (int)(pos.x);
    int map_y = (int)(pos.y);

    //length of ray from current position to next x or y-side
    f32 side_dist_x = 0;
    f32 side_dist_y = 0;

    //length of ray from one x or y-side to next x or y-side
    f32 delta_dist_x = (dir.x == 0.0f) ? 1e30f : fabsf(1.0f / dir.x);
    f32 delta_dist_y = (dir.y == 0.0f) ? 1e30f : fabsf(1.0f / dir.y);

    f32 perp_wall_dist = 0;

    //what direction to step in x or y-direction (either +1 or -1)
    int step_x = 0;
    int step_y = 0;

    b32 hit  = 0; //was there a wall hit?
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

        if (v2_dist(pos, v2((f32)map_x, (f32)map_y)) > (max_range + 1.0f)) {
            hit = 1;
        }

        if (kill_on_wall_hit) {
            if (!tm_is_traversable(map, map_x, map_y))
            {
                hit = 1;
            }
        }
    } 

    if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
    else           perp_wall_dist = (side_dist_y - delta_dist_y);

    return v2_add(pos, v2_scale(dir, (perp_wall_dist > max_range? max_range : perp_wall_dist)));
}

static v2 tm_cast_angle(traverse_map* map, v2 from, f32 angle, f32 max_range)
{
    m2 rot = m2_rotate(angle);
    v2 dir = m2_mulv(rot, v2(0, 1));

    return tm_cast_dir(map, from, dir, max_range);
}

