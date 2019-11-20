#pragma once

#include "ats_tool.h"

// ====================================================== GENERATE TILEMAP =================================================== //

typedef enum TMap_Type {
    TMAP_NONE,
    TMAP_FLOOR,
    TMAP_WALL,
    TMAP_DOOR,
    TMAP_GRASS,
    TMAP_TREE,
    TMAP_COUNT
} TMap_Type;

typedef struct Line {
    f32   a[2];
    f32   b[2];
} Line;

typedef struct Room {
    int     min[2];
    int     max[2];
} Room;

static int room_intersect(const Room* a, const Room* b) {
    if (a->max[0] < b->min[0]) return 0;
    if (a->min[0] > b->max[0]) return 0;
    if (a->max[1] < b->min[1]) return 0;
    if (a->min[1] > b->max[1]) return 0;
    //
    return 1;
}

static int room_intersect_with_border(const Room* a, const Room* b, int r) {
    if ((a->max[0] + r) < (b->min[0] - r)) return 0;
    if ((a->min[0] - r) > (b->max[0] + r)) return 0;
    if ((a->max[1] + r) < (b->min[1] - r)) return 0;
    if ((a->min[1] - r) > (b->max[1] + r)) return 0;
    //
    return 1;
}

static f32 room_get_width(const Room* r) {
    return r->max[0] - r->min[0];
}

static f32 room_get_height(const Room* r) {
    return r->max[1] - r->min[1];
}

static void room_get_center(f32* center, const Room* r) { 
    center[0] = 0.5f * (r->min[0] + r->max[0]);
    center[1] = 0.5f * (r->min[1] + r->max[1]);
}

static Room room_get_overlap(const Room* a, const Room* b) {
    Room r;

    r.min[0] = max(a->min[0], b->min[0]);
    r.max[0] = min(a->max[0], b->max[0]);
    r.min[1] = max(a->min[1], b->min[1]);
    r.max[1] = min(a->max[1], b->max[1]);

    return r;
}

static int room_on_map(const Room* r, int width, int height) {
    if (r->min[0] < 1 || r->max[0] >= width)  return 0;
    if (r->min[1] < 1 || r->max[1] >= height) return 0;
    //
    return 1;
}

static int room_on_section(const Room* r, int min_x, int min_y, int max_x, int max_y) {
    if (r->min[0] < min_x || r->max[0] >= max_x) return 0;
    if (r->min[1] < min_y || r->max[1] >= max_y) return 0;
    //
    return 1;
}

static int room_intersect_lines(const Room* room, const Line* line_array, int line_count) {
    f32 pos[2] = {
        0.5f * (room->min[0] + room->max[0]),
        0.5f * (room->min[1] + room->max[1]),
    };

    f32 rad = 0.5f * min(room->max[0] - room->min[0], room->max[1] - room->min[1]);

    for (int i = 0; i < line_count; ++i) {
        const Line* line = &line_array[i];

        if (f2_segment_is_intersecting_circle(line->a, line->b, pos, rad))
            return 1;
    }

    return 0;
}

static Room* tmap_generate_section(
        int*        tiles,
        //
        int         width,
        int         height,
        //
        int         section_min_x,
        int         section_min_y,
        int         section_max_x,
        int         section_max_y,
        //
        int         room_size_min,
        int         room_size_max,
        int         room_max_count,
        int*        room_count) {
    static Room* room_array = NULL;

    room_array = realloc(room_array, room_max_count * sizeof *room_array);
    
    memset(room_array, 0, room_max_count * sizeof (Room));
    memset(tiles, 0, width * height * sizeof *tiles);

    if (section_min_x < 0)      section_min_x = 0;
    if (section_min_y < 0)      section_min_y = 0;
    if (section_max_x > width)  section_max_x = width  - 1;
    if (section_max_y > height) section_max_y = height - 1;

    // generate rooms!
    for (int i = 0; i < room_max_count; ++i) {
        Room    room = {0};

        f32 pos[2] = { randi(0, width), randi(0, height) };

        room.min[0] = randi(section_min_x, section_max_x);
        room.min[1] = randi(section_min_y, section_max_y);

        room.max[0] = room.min[0] + randi(room_size_min, room_size_max);
        room.max[1] = room.min[1] + randi(room_size_min, room_size_max);

        room_array[i] = room;
    }

    int collision = 1;

    for (int i = 0; i < 100 && collision; ++i) {
        collision = 0;

        for (int i = 0; i < room_max_count; ++i) {
            Room* a = &room_array[i];

            for (int j = 0; j < room_max_count; ++j) {
                if (i == j) continue;

                Room* b = &room_array[j];

                if (room_intersect_with_border(a, b, 0.0f)) {
                    collision = 1;

                    int dx = (a->max[0] - a->min[0]) - (b->max[0] - b->min[0]);
                    int dy = (a->max[1] - a->min[1]) - (b->max[1] - b->min[1]);

                    a->min[0] += dx;
                    a->max[0] += dx;
                    a->min[1] += dy;
                    a->max[1] += dy;

                    b->min[0] -= dx;
                    b->max[0] -= dx;
                    b->min[1] -= dy;
                    b->max[1] -= dy;
                }
            }
        }
    }
    // remove room outside of bounds:
    {
        for (int i = 0; i < room_max_count; ++i)
            if (!room_on_section(&room_array[i], section_min_x, section_min_y, section_max_x, section_max_y))
                room_array[i--] = room_array[--room_max_count];
    }

    int   main_count     = 0;
    Room* main_array[5] = {0};
    // mark rooms:
    {
        for (int i = 0; i < 5; ++i) {
            Room* r = &room_array[i];
            main_array[main_count++] = r;
        }
    }

    Line line_array[128] = {0};
    int  line_count      = 0;

    // make hallways between all the main rooms:
    {
        for (int i = 0; i < main_count; ++i) {
            Room* a = main_array[i];

            for (int j = i + 1; j < main_count; ++j) {
                Room* b = main_array[j];

                Line* line0 = &line_array[line_count++];
                Line* line1 = &line_array[line_count++];

                f32 pos_a[2] = { 0.5f * (a->min[0] + a->max[0]), 0.5f * (a->min[1] + a->max[1]) };
                f32 pos_b[2] = { 0.5f * (b->min[0] + b->max[0]), 0.5f * (b->min[1] + b->max[1]) };

                f32 dx = __builtin_fabs(pos_a[0] - pos_b[0]);
                f32 dy = __builtin_fabs(pos_a[0] - pos_b[0]);

                if (dx > dy) {
                    line0->a[0] = pos_a[0];
                    line0->a[1] = pos_a[1];
                    line0->b[0] = pos_b[0];
                    line0->b[1] = pos_a[1];

                    line1->a[0] = pos_b[0];
                    line1->a[1] = pos_a[1];
                    line1->b[0] = pos_b[0];
                    line1->b[1] = pos_b[1];
                } else {
                    line0->a[0] = pos_a[0];
                    line0->a[1] = pos_a[1];
                    line0->b[0] = pos_a[0];
                    line0->b[1] = pos_b[1];

                    line1->a[0] = pos_a[0];
                    line1->a[1] = pos_b[1];
                    line1->b[0] = pos_b[0];
                    line1->b[1] = pos_b[1];
                }
            }
        }
    }
    // remove rooms that does not intersect the lines!
    {
        for (int i = 0; i < room_max_count; ++i) {
            Room* room = &room_array[i];

            if (!room_intersect_lines(room, line_array, line_count))
                room_array[i--] = room_array[--room_max_count];
        }
    }

    for (int i = 0; i < line_count; ++i) {
        Line* line = &line_array[i];

        int ix = line->a[0];
        int iy = line->a[1];
        int ex = line->b[0];
        int ey = line->b[1];

        while (ix != ex || iy != ey) {
            tiles[iy * width + ix] = TMAP_FLOOR;

            if (ix < ex) ix++;
            if (ix > ex) ix--;
            if (iy < ey) iy++;
            if (iy > ey) iy--;
        }
    }

    for (int i = 0; i < room_max_count; ++i) {
        Room* room = &room_array[i];

        int min_x = room->min[0];
        int min_y = room->min[1];
        int max_x = room->max[0];
        int max_y = room->max[1];

        for (int y = min_y; y < max_y; ++y)
            for (int x = min_x; x < max_x; ++x)
                tiles[y * width + x] = TMAP_FLOOR;
    }

    // add walls to tilemap:
    for (int y = section_min_y; y < section_max_y; ++y) {
        for (int x = section_min_x; x < section_max_x; ++x) {
            if (tiles[y * width + x] == TMAP_FLOOR) {
                int xsr = x - 1, xar = x + 1;
                int ysr = y - 1, yar = y + 1;

                if (tiles[ysr * width + xsr] == TMAP_NONE) tiles[ysr * width + xsr] = TMAP_WALL;
                if (tiles[ysr * width + xar] == TMAP_NONE) tiles[ysr * width + xar] = TMAP_WALL;
                if (tiles[yar * width + xar] == TMAP_NONE) tiles[yar * width + xar] = TMAP_WALL;
                if (tiles[yar * width + xsr] == TMAP_NONE) tiles[yar * width + xsr] = TMAP_WALL;
            }
        }
    }

    if (room_count) *room_count = room_max_count;

    return room_array;
}

static Room* tmap_generate(int* tiles, int width, int height, int room_size_min, int room_size_max, int room_max_count, int* room_count) {
    memset(tiles, 0, width * height * sizeof *tiles);

    return tmap_generate_section(
        tiles,
        width,
        height,
        0,
        0,
        width,
        height,
        room_size_min,
        room_size_max,
        room_max_count,
        room_count);
}

static void tmap_generate_wilds(int* tiles, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles[y * width + x] = TMAP_GRASS;
        }
    }
}

static void tmap_place_doors(int* tiles, int width, int height, const Room* room_array, int room_count) {
    for (int i = 0; i < room_count; ++i) {
        const Room* r = &room_array[i];

        int min_x = r->min[0];
        int min_y = r->min[1];
        int max_x = r->max[0] - 1;
        int max_y = r->max[1] - 1;

        for (int y = r->min[1]; y < r->max[1]; ++y) {
            if (tiles[y * width + min_x] != TMAP_WALL) tiles[y * width + min_x] = TMAP_DOOR;
            if (tiles[y * width + max_x] != TMAP_WALL) tiles[y * width + max_x] = TMAP_DOOR;
        }

        for (int x = r->min[0]; x < r->max[0]; ++x) {
            if (tiles[min_y * width + x] != TMAP_WALL) tiles[min_y * width + x] = TMAP_DOOR;
            if (tiles[max_y * width + x] != TMAP_WALL) tiles[max_y * width + x] = TMAP_DOOR;
        }
    }
}

// ========================================================== FOV ======================================================= //

// @NOTE: assumes that (x, y) and (ex, ey) is on the map!
static int tmap_fov_ray_test(const int* tiles, int width, int height, int x, int y, int ex, int ey) {
    f32 ix = x + 0.5f;
    f32 iy = y + 0.5f;

    f32 dx = ex + 0.5f - ix;
    f32 dy = ey + 0.5f - iy;

    // normalize dx, dy
    {
        f32 len = __builtin_sqrtf(dx * dx + dy * dy);

        dx /= len;
        dy /= len;
        dx *= 0.1f;
        dy *= 0.1f;
    }

    while (x != ex || y != ey) {
        x = ix;
        y = iy;

        if (tiles[y * width + x] == TMAP_WALL) {
            return 0;
        }

        ix += dx;
        iy += dy;
    }

    return 1;
}

// @NOTE: assumes the user provides a buffer big enough to hold the data,   fov size == tiles size!
static void tmap_get_fov(int* fov, int px, int py, int fov_rad, int region_size, const int* tiles, int width, int height) {
    int sx = px - region_size;
    int sy = py - region_size;
    int ex = px + region_size;
    int ey = py + region_size;
    
    if (sx < 0)      sx = 0;
    if (sy < 0)      sy = 0;
    if (ex > width)  ex = width  - 1;
    if (ey > height) ey = height - 1;
    //
    int rad_sq = fov_rad * fov_rad;
    //
    for (int y = sy; y < ey; ++y) {
        for (int x = sx; x < ex; ++x) {
            int tile_index = y * width + x;

            fov[tile_index] = 0.0f;

            int dx = x - px;
            int dy = y - py;

            if (dx * dx + dy * dy <= rad_sq && tmap_fov_ray_test(tiles, width, height, x, y, px, py))
                fov[tile_index] = 1.0f;
        }
    }
}

// ====================================================== LIGHT ========================================================= //

static void tmap_get_light(f32* light, int px, int py, int light_rad, const int* tiles, int width, int height) {
    int sx = px - light_rad;
    int sy = py - light_rad;
    int ex = px + light_rad;
    int ey = py + light_rad;
 
    if (sx < 0)      sx = 0;
    if (sy < 0)      sy = 0;
    if (ex > width)  ex = width  - 1;
    if (ey > height) ey = height - 1;

    int rad_sq = light_rad * light_rad;

    for (int y = sy; y < ey; ++y) {
        for (int x = sx; x < ex; ++x) {
            int tile_index = y * width + x;

            int dx = x - px;
            int dy = y - py;

            int len_sq = dx * dx + dy * dy;

            if (len_sq <= rad_sq && tmap_fov_ray_test(tiles, width, height, x, y, px, py)) {
                light[tile_index] += 1.0f - (f32)(len_sq) / (f32)(light_rad * light_rad);
                light[tile_index] = CLAMP(light[tile_index], 0.0f, 1.0f);
            }
        }
    }
}

static void tmap_get_light_color(
        f32* light, f32* color, int px, int py, int light_rad, const f32 light_color[4], const int* tiles, int width, int height) {
    int sx = px - light_rad;
    int sy = py - light_rad;
    int ex = px + light_rad;
    int ey = py + light_rad;
 
    if (sx < 0)      sx = 0;
    if (sy < 0)      sy = 0;
    if (ex > width)  ex = width  - 1;
    if (ey > height) ey = height - 1;

    int rad_sq = light_rad * light_rad;

    for (int y = sy; y < ey; ++y) {
        for (int x = sx; x < ex; ++x) {
            int tile_index = y * width + x;

            int dx = x - px;
            int dy = y - py;

            int len_sq = dx * dx + dy * dy;

            if (len_sq <= rad_sq && tmap_fov_ray_test(tiles, width, height, x, y, px, py)) {
                f32 light_str = 1.0f - (f32)(len_sq) / (f32)(light_rad * light_rad);

                light[tile_index] += light_str;
                light[tile_index] =  CLAMP(light[tile_index], 0.0f, 1.0f);

                f32* c = &color[y * 4 * width + 4 * x];

                c[0] += light_color[0] * light_str;
                c[1] += light_color[1] * light_str;
                c[2] += light_color[2] * light_str;
                c[3] = 1.0f;
            }
        }
    }
}

