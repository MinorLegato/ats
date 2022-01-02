#pragma once

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

typedef struct texture_id       texture_id;
typedef struct texture_entry    texture_entry;
typedef struct texture_table    texture_table;

extern void             tt_begin(void);
extern void             tt_end(void);

extern r2i              tt_get_rect(texture_id id);
extern texture_id       tt_get_id(const char* name);
extern r2i              tt_get(const char* name);
extern void             tt_load_from_dir(const char* dir_path, memory_arena_t* ma);

extern image_t          tt_get_image(void);

struct texture_id {
    u16 index;
};

struct texture_entry {
    b32     in_use;
    u32     hash;

    r2i     rect;

    char    name[64];
};

struct texture_table {
    image_t         image;
    texture_entry   array[TEXTURE_TABLE_SIZE];
};

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

static texture_table table;

extern image_t tt_get_image(void) {
    return table.image;
}

extern r2i tt_get_rect(texture_id id) {
    return table.array[id.index].rect;
}

extern texture_id tt_get_id(const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table.array[index].in_use) {
        if ((table.array[index].hash == hash) && (strcmp(table.array[index].name, name) == 0)) {
            texture_id id = { index };
            return id;
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    texture_id id = ATS_INIT_ZERO;
    return id;
}

extern r2i tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

static void _tt_add_entry(const char* name, r2i rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table.array[index].in_use) {
        if ((table.array[index].hash == hash) && (strcmp(table.array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    texture_entry* entry = &table.array[index];
    
    entry->in_use = true;
    entry->rect = rect;
    entry->hash = hash;

    strcpy_s(entry->name, 64, name);
}

static void cstr_copy_no_file_extension(char* out, char* str) {
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;

    copy_memory(out, str, i);

    out[i] = '\0';
}

static void cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

typedef struct ImageData {
    image_t     image;
    char        name[256];
} image_data;

static int cmp_image_data(const void* va, const void* vb) {
    image_data* a = (image_data*)va;
    image_data* b = (image_data*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(r2i rect, image_t image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height; 
}

static image_data* tt__load_png_files_in_directory(u32* out_image_count, const char* dir_path, memory_arena_t* ma) {
    u32             image_count = 0;
    image_data*   image_array = NULL;

    // get all .png files in directory:
    Defer (image_array = (image_data*)ma_begin(ma), ma_end(ma, image_count * sizeof (image_data))) {
        char find_file_str[256];

        cstr_concat(find_file_str, dir_path, "*.png*");

        WIN32_FIND_DATA find_data = {0};
        HANDLE find_handle = FindFirstFile(find_file_str, &find_data);

        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                image_data* data = &image_array[image_count++];

                char file_path[256];
                cstr_concat(file_path, dir_path, find_data.cFileName);
                data->image = image_load_from_file(file_path);

                cstr_copy_no_file_extension(data->name, find_data.cFileName);
            } while(FindNextFile(find_handle, &find_data));

            FindClose(find_handle);
        }
    }

    qsort(image_array, image_count, sizeof (image_data), cmp_image_data);

    *out_image_count = image_count;
    return image_array;
}

extern void tt_load_from_dir(const char* dir_path, memory_arena_t* ma) {
    table = ctor(texture_table, { 4096, 4096, ma_array(ma, u32, 4096 * 4096) });
    table.array[0].in_use = true;

    for (u32 i = 0; i < 2048 * 2048; ++i) {
        table.image.pixels[i] = 0xff000000;
    }

    Defer (ma_save(ma), ma_restore(ma)) {
        u32             image_count     = 0;
        image_data*     image_array     = tt__load_png_files_in_directory(&image_count, dir_path, ma);
        u32             rect_count      = 0;
        r2i*            rect_array      = NULL;

        Defer (rect_array = (r2i*)ma_begin(ma), ma_end(ma, 0)) {
            rect_array[rect_count++] = r2i(
                v2i(0, 0),
                v2i(table.image.width, table.image.height));

            for (u32 i = 0; i < image_count; ++i) {
                image_data* data = &image_array[i];

                u32 j = 0;
                for (j = 0; j < rect_count; ++j) {
                    if (rect_contains_image(rect_array[j], data->image)) {
                        break;
                    }
                }

                r2i rect        = rect_array[j];
                rect_array[j]   = rect_array[--rect_count];

                v2i size        = v2i(data->image.width + 2, data->image.height + 2);
                v2i offset      = rect.min;

                _tt_add_entry(data->name, r2i(v2i(offset.x + 1, offset.y + 1),
                                                     v2i(offset.x + size.x - 1, offset.y + size.y - 1)));

                for (i32 y = 0; y < data->image.height; ++y) {
                    for (i32 x = 0; x < data->image.width; ++x) {
                        image_set_pixel(&table.image, x + offset.x + 1, y + offset.y + 1, image_get_pixel(&data->image, x, y));
                    }
                }
                
                {
                    r2i a = { { rect.min.x,           rect.min.y + size.y },  { rect.min.x + size.x, rect.max.y } };
                    r2i b = { { rect.min.x + size.x,  rect.min.y },           rect.max };

                    if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = a; }
                    if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = b; }
                }

                free(data->image.pixels);
            }
        }
    }
}

#endif

