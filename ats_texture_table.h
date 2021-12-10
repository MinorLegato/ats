#pragma once

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

typedef struct TextureID     TextureID;
typedef struct TextureEntry  TextureEntry;
typedef struct TextureTable  TextureTable;

extern R2i          tt_get_rect(TextureTable* table, TextureID id);
extern TextureID    tt_get_id(TextureTable* table, const char* name);
extern R2i          tt_get(TextureTable* table, const char* name);
extern void         tt_load_from_dir(TextureTable* table, const char* dir_path, MemoryArena* ma);

struct TextureID
{
    u16 index;
};

struct TextureEntry
{
    b32     in_use;
    u32     hash;

    R2i     rect;

    char    name[64];
};

struct TextureTable
{
    Image           image;
    TextureEntry    array[TEXTURE_TABLE_SIZE];
};

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

extern R2i tt_get_rect(TextureTable* table, TextureID id)
{
    return table->array[id.index].rect;
}

extern TextureID tt_get_id(TextureTable* table, const char* name)
{
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (table->array[index].in_use) {
        if ((table->array[index].hash == hash) && (strcmp(table->array[index].name, name) == 0)) {
            TextureID id = { index };
            return id;
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    TextureID id = ATS_INIT_ZERO;
    return id;
}

extern R2i tt_get(TextureTable* table, const char* name)
{
    return tt_get_rect(table, tt_get_id(table, name));
}

static void _tt_add_entry(TextureTable* table, const char* name, R2i rect)
{
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

static void cstr_copy_no_file_extension(char* out, char* str)
{
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;
    memcpy(out, str, i);
    out[i] = '\0';
}

static void cstr_concat(char* out, const char* a, const char* b)
{
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

typedef struct ImageData
{
    Image     image;
    char        name[256];
} ImageData;

static int cmp_image_data(const void* va, const void* vb)
{
    ImageData* a = (ImageData*)va;
    ImageData* b = (ImageData*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(R2i rect, Image image)
{
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height; 
}

static ImageData* tt__load_png_files_in_directory(u32* out_image_count, const char* dir_path, MemoryArena* ma)
{
    u32             image_count = 0;
    ImageData*   image_array = NULL;

    // get all .png files in directory:
    Defer (image_array = (ImageData*)ma_begin(ma), ma_end(ma, image_count * sizeof (ImageData))) {
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

    *out_image_count = image_count;
    return image_array;
}

extern void tt_load_from_dir(TextureTable* table, const char* dir_path, MemoryArena* ma)
{
    *table = ctor(TextureTable, { 4096, 4096, ma_array(ma, u32, 4096 * 4096) });
    table->array[0].in_use = true;

    for (u32 i = 0; i < 2048 * 2048; ++i) {
        table->image.pixels[i] = 0xff000000;
    }

    Defer (ma_save(ma), ma_restore(ma)) {
        u32         image_count     = 0;
        ImageData*  image_array     = tt__load_png_files_in_directory(&image_count, dir_path, ma);
        u32         rect_count      = 0;
        R2i*        rect_array      = NULL;

        Defer (rect_array = (R2i*)ma_begin(ma), ma_end(ma, 0)) {
            rect_array[rect_count++] = r2i(
                v2i(0, 0),
                v2i(table->image.width, table->image.height));

            for (u32 i = 0; i < image_count; ++i) {
                ImageData* data = &image_array[i];

                u32 j = 0;
                for (j = 0; j < rect_count; ++j) {
                    if (rect_contains_image(rect_array[j], data->image)) {
                        break;
                    }
                }

                R2i rect        = rect_array[j];
                rect_array[j]   = rect_array[--rect_count];

                V2i size        = v2i(data->image.width + 2, data->image.height + 2);
                V2i offset      = rect.min;

                _tt_add_entry(table, data->name, r2i(v2i(offset.x + 1, offset.y + 1),
                                                     v2i(offset.x + size.x - 1, offset.y + size.y - 1)));

                for (i32 y = 0; y < data->image.height; ++y) {
                    for (i32 x = 0; x < data->image.width; ++x) {
                        image_set_pixel(&table->image, x + offset.x + 1, y + offset.y + 1, image_get_pixel(&data->image, x, y));
                    }
                }
                
                {
                    R2i a = { { rect.min.x,           rect.min.y + size.y },  { rect.min.x + size.x, rect.max.y } };
                    R2i b = { { rect.min.x + size.x,  rect.min.y },           rect.max };

                    if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = a; }
                    if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = b; }
                }

                free(data->image.pixels);
            }
        }
    }
}

#endif

