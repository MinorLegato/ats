#pragma once

#define TEXTURE_TABLE_SIZE 512

typedef struct TextureID TextureID;
struct TextureID {
    u16 index;
};

typedef struct TextureEntry TextureEntry;
struct TextureEntry {
    b32 in_use;
    u32 hash;

    RectI32 rect;

    char name[64];
};

typedef struct TextureTable TextureTable;
struct TextureTable {
    Image image;
    TextureEntry array[TEXTURE_TABLE_SIZE];
};

internal RectI32 tt_get_rect(TextureTable* table, TextureID id) {
    return table->array[id.index].rect;
}

internal void tt_add_entry(TextureTable* table, const char* name, RectI32 rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table->array[index].in_use) {
        if ((table->array[index].hash == hash) && (strcmp(table->array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    TextureEntry* entry = &table->array[index];
    
    entry->in_use = true;
    entry->rect = rect;
    entry->hash = hash;

    strcpy_s(entry->name, 64, name);
}

internal TextureID tt_get_id(TextureTable* table, const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table->array[index].in_use) {
        if ((table->array[index].hash == hash) && (strcmp(table->array[index].name, name) == 0))
            return (TextureID) { .index = index };

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    return (TextureID) { .index = 0 };
}

internal void cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

internal void cstr_copy_no_file_extension(char* out, char* str) {
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;
    memcpy(out, str, i);
    out[i] = '\0';
}

typedef struct ImageData ImageData;
struct ImageData {
    Image image;
    char name[256];
};

internal int cmp_image_data(const void* va, const void* vb) {
    ImageData* a = (ImageData*)va;
    ImageData* b = (ImageData*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

internal b32 recti_contains_image(RectI32 rect, Image image) {
    i32 rect_width = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height; 
}

internal TextureTable tt_load_from_dir(const char* dir_path) {
    TextureTable table = {
        .image = {
            .width  = 1024,
            .height = 1024,
            .pixels = alloc_array(u32, 1024 * 1024),
        },

        .array = { [0] = { .in_use = true } }
    };

    defer(alloc_save(), alloc_restore()) {
        u32 image_count = 0;
        ImageData* image_array = NULL;

        // get all .png files in directory:
        defer(image_array = alloc_begin(), alloc_end(image_count * sizeof (ImageData))) {
            char find_file_str[256];

            cstr_concat(find_file_str, dir_path, "*.png*");

            WIN32_FIND_DATA find_data = {0};
            HANDLE find_handle = FindFirstFile(find_file_str, &find_data);

            if (find_handle != INVALID_HANDLE_VALUE) {
                do {
                    ImageData* data = &image_array[image_count++];

                    char file_path[256];
                    cstr_concat(file_path, dir_path, find_data.cFileName);
                    data->image = image_load_from_file(file_path);

                    cstr_copy_no_file_extension(data->name, find_data.cFileName);
                } while(FindNextFile(find_handle, &find_data));

                FindClose(find_handle);
            }
        }

        qsort(image_array, image_count, sizeof (ImageData), cmp_image_data);

        u32         rect_count   = 0;
        RectI32*    rect_array   = NULL;

        defer(rect_array = alloc_begin(), alloc_end(0)) {
            rect_array[rect_count++] = (RectI32) {
                .min    = { 0, 0 },
                .max    = { table.image.width, table.image.height },
            };

            for (u32 i = 0; i < image_count; ++i) {
                ImageData* data = &image_array[i];

                u32 j = 0;
                for (j = 0; j < rect_count; ++j)
                    if (recti_contains_image(rect_array[j], data->image))
                        break;

                RectI32 rect = rect_array[j];
                rect_array[j] = rect_array[--rect_count];

                v2i size = { data->image.width, data->image.height };
                v2i offset = rect.min;

                tt_add_entry(&table, data->name, (RectI32) {
                    .min = { offset.x, offset.y },
                    .max = { offset.x + size.x, offset.y + size.y },
                });

                for (i32 y = 0; y < data->image.height; ++y) {
                    for (i32 x = 0; x < data->image.width; ++x) {
                        image_set_pixel(&table.image, x + offset.x, y + offset.y, image_get_pixel(&data->image, x, y));
                    }
                }

                {
                    RectI32 a = { .min = { rect.min.x,           rect.min.y + size.y },  .max = { rect.min.x + size.x, rect.max.y } };
                    RectI32 b = { .min = { rect.min.x + size.x,  rect.min.y },           .max = rect.max };

                    if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = a; }
                    if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = b; }
                }

                free(data->image.pixels);
            }
        }
    }

    return table;
}

