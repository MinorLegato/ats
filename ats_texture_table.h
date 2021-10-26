#pragma once

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

typedef struct texture_id_t texture_id_t;
typedef struct texture_entry_t texture_entry_t;
typedef struct texture_table_t texture_table_t;

extern rect2_t          tt_get_rect(texture_table_t* table, texture_id_t id);
extern void             tt_add_entry(texture_table_t* table, const char* name, rect2_t rect);
extern texture_id_t     tt_get_id(texture_table_t* table, const char* name);
extern texture_table_t  tt_load_from_dir(const char* dir_path, memory_arena_t* ma);

struct texture_id_t {
    u16 index;
};

struct texture_entry_t {
    b32 in_use;
    u32 hash;

    rect2_t rect;

    char name[64];
};

struct texture_table_t {
    image_t image;
    texture_entry_t array[TEXTURE_TABLE_SIZE];
};

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

extern rect2_t tt_get_rect(texture_table_t* table, texture_id_t id) {
    return table->array[id.index].rect;
}

extern void tt_add_entry(texture_table_t* table, const char* name, rect2_t rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table->array[index].in_use) {
        if ((table->array[index].hash == hash) && (strcmp(table->array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    f32 sx = 1.0f / table->image.width;
    f32 sy = 1.0f / table->image.height;

    rect.min.x *= sx;
    rect.min.y *= sy;
    rect.max.x *= sx;
    rect.max.y *= sy;

    rect.min.x += TEXTURE_BORDER * sx;
    rect.min.y += TEXTURE_BORDER * sy;
    rect.max.x -= TEXTURE_BORDER * sx;
    rect.max.y -= TEXTURE_BORDER * sy;
    
    texture_entry_t* entry = &table->array[index];
    
    entry->in_use = true;
    entry->rect = rect;
    entry->hash = hash;

    strcpy_s(entry->name, 64, name);
}

extern texture_id_t tt_get_id(texture_table_t* table, const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table->array[index].in_use) {
        if ((table->array[index].hash == hash) && (strcmp(table->array[index].name, name) == 0)) {
            texture_id_t id = { index };
            return id;
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    texture_id_t id = {0};
    return id;
}

static void cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

static void cstr_copy_no_file_extension(char* out, char* str) {
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;
    memcpy(out, str, i);
    out[i] = '\0';
}

typedef struct image_data_t image_data_t;
struct image_data_t {
    image_t image;
    char name[256];
};

static int cmp_image_data(const void* va, const void* vb) {
    image_data_t* a = (image_data_t*)va;
    image_data_t* b = (image_data_t*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(rect2_t rect, image_t image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height; 
}

static image_data_t* tt__load_png_files_in_directory(u32* out_image_count, const char* dir_path, memory_arena_t* ma) {
    u32             image_count = 0;
    image_data_t*   image_array = NULL;

    // get all .png files in directory:
    defer(image_array = (image_data_t*)ma_begin(ma), ma_end(ma, image_count * sizeof (image_data_t))) {
        char find_file_str[256];

        cstr_concat(find_file_str, dir_path, "*.png*");

        WIN32_FIND_DATA find_data = {0};
        HANDLE find_handle = FindFirstFile(find_file_str, &find_data);

        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                image_data_t* data = &image_array[image_count++];

                char file_path[256];
                cstr_concat(file_path, dir_path, find_data.cFileName);
                data->image = image_load_from_file(file_path);

                cstr_copy_no_file_extension(data->name, find_data.cFileName);
            } while(FindNextFile(find_handle, &find_data));

            FindClose(find_handle);
        }
    }

    qsort(image_array, image_count, sizeof (image_data_t), cmp_image_data);

    *out_image_count = image_count;
    return image_array;
}

extern texture_table_t tt_load_from_dir(const char* dir_path, memory_arena_t* ma) {
    texture_table_t table = { { 2048, 2048, ma_array(ma, u32, 2048 * 2048) } };
    table.array[0].in_use = true;

    for (u32 i = 0; i < 2048 * 2048; ++i) {
        table.image.pixels[i] = 0xff000000;
    }

    defer(ma_save(ma), ma_restore(ma)) {
        u32 image_count = 0;
        image_data_t* image_array = tt__load_png_files_in_directory(&image_count, dir_path, ma);

        u32       rect_count   = 0;
        rect2_t*  rect_array   = NULL;

        defer(rect_array = (rect2_t*)ma_begin(ma), ma_end(ma, 0)) {
            rect_array[rect_count++] = rect2(
                v2(0, 0),
                v2(table.image.width, table.image.height));

            for (u32 i = 0; i < image_count; ++i) {
                image_data_t* data = &image_array[i];

                u32 j = 0;
                for (j = 0; j < rect_count; ++j) {
                    if (rect_contains_image(rect_array[j], data->image)) {
                        break;
                    }
                }

                rect2_t rect  = rect_array[j];
                rect_array[j] = rect_array[--rect_count];

                vec2_t size   = v2(data->image.width + 2, data->image.height + 2);
                vec2_t offset = rect.min;

                tt_add_entry(&table, data->name, rect2(
                        v2(offset.x + 1, offset.y + 1),
                        v2(offset.x + size.x - 1, offset.y + size.y - 1)));

                for (i32 y = 0; y < data->image.height; ++y) {
                    for (i32 x = 0; x < data->image.width; ++x) {
                        image_set_pixel(&table.image, x + offset.x + 1, y + offset.y + 1, image_get_pixel(&data->image, x, y));
                    }
                }
                
                {
                    rect2_t a = { { rect.min.x,           rect.min.y + size.y },  { rect.min.x + size.x, rect.max.y } };
                    rect2_t b = { { rect.min.x + size.x,  rect.min.y },           rect.max };

                    if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = a; }
                    if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = b; }
                }

                free(data->image.pixels);
            }
        }
    }

    return table;
}

#endif

