// =================================================== SPATIAL MAP =================================================== //
// assumes that 'T' has this function:
// r2 get_rect(T* e)

#define SPATIAL_MAX (4 * 4 * 4096)

struct spatial_cell {
    void* e;
    r2 rect;

    struct spatial_cell* next;
};

struct spatial_map {
    struct spatial_cell* table[4096];

    usize count;
    struct spatial_cell array[SPATIAL_MAX];
};

static void
sm_clear(struct spatial_map* map) {
    memset(map->table, 0, sizeof map->table);
    map->count = 0;
}

static u32
sm_index(const struct spatial_map* map, v2i pos) {
    u32 hash = hash_v2i(pos);
    return hash % ArrayCount(map->table);
}

static void
sm_add(struct spatial_map* map, void* e, r2 e_rect) {
    v2 rad = {
        e_rect.max.x - e_rect.min.x,
        e_rect.max.y - e_rect.min.y,
    };

    r2i rect = {
        e_rect.min.x, e_rect.min.y,
        e_rect.max.x, e_rect.max.y,
    };

    for_r2(rect, x, y) {
        u32 index = sm_index(map, V2i(x, y));
        struct spatial_cell* cell = map->array + map->count++;

        cell->e = e;
        cell->rect = e_rect;
        cell->next = map->table[index];

        map->table[index] = cell;
    }
}

struct sm_entry {
    void* e;
    r2 rect;
};

struct sm_result {
    usize count;
    struct sm_entry* array;
};

static struct sm_result
sm_in_range(struct spatial_map* map, v2 pos, v2 rad, const void* ignore) {
    static struct sm_entry spatial_array[SPATIAL_MAX];

    struct sm_result result = {0};
    result.array = spatial_array;

    r2 rect = {
        pos.x - rad.x, pos.y - rad.y,
        pos.x + rad.x, pos.y + rad.y,
    };

    r2i irect = {
        pos.x - rad.x, pos.y - rad.y,
        pos.x + rad.x, pos.y + rad.y,
    };

    for_r2(irect, x, y) {
        u32 index = sm_index(map, V2i(x, y));
        for (struct spatial_cell* it = map->table[index]; it; it = it->next) {
            b32 unique = true;

            if (it->e == ignore) continue;
            if (!intersect(rect, it->rect)) continue;

            for_range(i, 0, result.count) {
                if (result.array[i].e == it->e) {
                    unique = false;
                    break;
                }
            }
            if (unique) {
                result.array[result.count++] = (struct sm_entry) {
                    .e = it->e,
                    .rect = it->rect,
                };
            }
        }
    }

    return result;
}

struct sm_iter {
    struct sm_entry* current;

    u32 index;
    struct sm_result result;
};

static struct sm_iter
sm_get_iterator(struct spatial_map* map, v2 pos, v2 rad, const void* ignore) {
    struct sm_iter it = {0};
    
    it.result = sm_in_range(map, pos, rad, ignore);
    it.current = &it.result.array[0];

    return it;
}

static b32
sm_iter_is_valid(const struct sm_iter* it) {
    return it->index < it->result.count;
}

static void
sm_iter_advance(struct sm_iter* it) {
    it->index++;
    it->current = &it->result.array[it->index];
}

static void*
sm_get_closest(struct spatial_map* map, v2 pos, f32 range, const void* ignore) {
    void* result = NULL;
    
    f32 distance = 2.0f * range;
    for_iter(sm_iter, it, sm_get_iterator(map, pos, V2(range, range), ignore)) {
        struct sm_entry* e = it.current;

        v2 pos = {
            0.5 * e->rect.min.x + e->rect.max.x,
            0.5 * e->rect.min.y + e->rect.max.y,
        };

        f32 new_distance = v2_dist(pos, pos);

        if (new_distance < distance) {
            result = e;
            distance = new_distance;
        }
    }

    return result;
}

static void*
sm_at_position(struct spatial_map* map, v2 pos) {
    u32 index = sm_index(map, V2i(pos.x, pos.y));

    for (struct spatial_cell* it = map->table[index]; it; it = it->next) {
        if (contains(it->rect, pos)) {
            return it->e;
        }
    }
    return NULL;
}

// ============================================ RAYCAST 2D TILEMAP ========================================== //

#define RAY2D_MAX_SIZE  (8192)

struct ray2D_map {
    r2i bounds;
    u32 array[RAY2D_MAX_SIZE];
};

static void
ray2D_init(struct ray2D_map* map, r2i bounds) {
    i32 width  = bounds.max.x - bounds.min.x;
    i32 height = bounds.max.y - bounds.min.y;

    assert((width * height) < (32 * RAY2D_MAX_SIZE));
    memset(map, 0, sizeof (struct ray2D_map));

    map->bounds = bounds;
}

static inline u32
ray2D_index(const struct ray2D_map* map, i32 x, i32 y) {
    u32 width = map->bounds.max.x - map->bounds.min.x;
    return y * width + x;
}

static void
ray2D_set_traversable(struct ray2D_map* map, i32 x, i32 y) {
    x -= map->bounds.min.x;
    y -= map->bounds.min.x;

    bit_set(map->array, ray2D_index(map, x, y));
}

static b32
ray2D_is_traversable(const struct ray2D_map* map, i32 x, i32 y) {
    if (!r2i_contains(map->bounds, V2i(x, y))) return false;

    x -= map->bounds.min.x;
    y -= map->bounds.min.x;

    return bit_get(map->array, ray2D_index(map, x, y));
}

static v2
ray2D_cast_dir(const struct ray2D_map* map, v2 pos, v2 dir, f32 max_range) {
    if (!ray2D_is_traversable(map, pos.x, pos.y)) return pos;

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

        if (!ray2D_is_traversable(map, map_x, map_y) || (v2_dist(pos, V2(map_x, map_y)) > (max_range + 1.0f))) {
            hit = true;
        }
    } 

    if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
    else           perp_wall_dist = (side_dist_y - delta_dist_y);

    return v2_add(pos, v2_scale(dir, (perp_wall_dist > max_range? max_range : perp_wall_dist)));
}

static v2
ray2D_cast_angle(const struct ray2D_map* map, v2 from, f32 angle, f32 max_range) {
    m2 rot = m2_rotate(angle);
    v2 dir = m2_mulv(rot, V2(0, 1));

    return ray2D_cast_dir(map, from, dir, max_range);
}

