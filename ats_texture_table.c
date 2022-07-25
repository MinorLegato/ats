
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

typedef struct tt_image {
    b32 user_provided;

    image img;
    char name[256];
} tt_image;

static m_allocator tt_allocator;

static texture_table tt_table;

static tt_image* tt_image_array;

extern texture_table*
tt_get_texture_table(void) {
    return &tt_table;
}

extern void
tt_add_image(const char* name, image img) {
    tt_image data = {0};

    data.user_provided = true;
    data.img = img;
    strcpy(data.name, name);

    buf_add(tt_image_array, data);
}

extern image
tt_get_image(void) {
    return tt_table.img;
}

extern r2i
tt_get_rect(texture_id id) {
    return tt_table.array[id.index].rect;
}

extern texture_id
tt_get_id(const char* name) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;
    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0)) {
            texture_id id = { index };
            return id;
        }
        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    assert(false);
    return (texture_id) {0};
}

extern r2i
tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

static void
_tt_add_entry(const char* name, r2i rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    texture_entry* entry = &tt_table.array[index];

    entry->in_use = true;
    entry->rect = rect;
    entry->hash = hash;

    strcpy_s(entry->name, 64, name);
}

static void
cstr_copy_without_extension(char* out, char* str) {
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;
    CopyMemory(out, str, i);
    out[i] = '\0';
}

static void
cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

static int
tt_cmp_image(const void* va, const void* vb) {
    tt_image* a = (tt_image*)va;
    tt_image* b = (tt_image*)vb;

    int dw = b->img.width  - a->img.width;
    int dh = a->img.height - a->img.height;

    return b->img.width - a->img.width;
}

extern b32
rect_contains_image(r2i rect, image image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;
    return image.width <= rect_width && image.height <= rect_height;
}

extern void
tt_load_from_dir(const char* dir_path) {
    char find_file_str[256] = {0};
    cstr_concat(find_file_str, dir_path, "*.png*");

    WIN32_FIND_DATA find_data = {0};
    HANDLE find_handle = FindFirstFile(find_file_str, &find_data);
    assert(find_handle != INVALID_HANDLE_VALUE);

    do {
        tt_image data = {0};
        
        char file_path[256];
        cstr_concat(file_path, dir_path, find_data.cFileName);
        data.img = file_load_image(file_path);
        cstr_copy_without_extension(data.name, find_data.cFileName);

        buf_add(tt_image_array, data);
    } while (FindNextFile(find_handle, &find_data));

    FindClose(find_handle);
}

extern void
tt_begin(int width, int height, m_allocator allocator) {
    tt_allocator = allocator;
    tt_image_array = buf_create(tt_image, 256, allocator);

    tt_table = (texture_table) {
        width,
        height, 
        (u32*)m_zero(tt_allocator, width * height * sizeof (u32)),
    };

    tt_table.array[0].in_use = true;

    for (u32 i = 0; i < width * height; ++i) {
        tt_table.img.pixels[i] = 0xff000000;
    }
}

static r2i
tt_get_fit(r2i* stack, image img) {
    u32 j = 0;
    for (j = 0; j < buf_len(stack); ++j) {
        if (rect_contains_image(stack[j], img)) {
            break;
        }
    }
    r2i rect = stack[j];
    buf_rem(stack, j);
    return rect;
}

extern void
tt_end(void) {
    r2i* stack = buf_create(r2i, 256, tt_allocator);

    buf_add(stack, (r2i) {
        .min = { 0, 0 },
        .max = { tt_table.img.width - 1, tt_table.img.height - 1 },
    });

    buf_sort(tt_image_array, tt_cmp_image);

    for_buf(i, tt_image_array) {
        tt_image* data = &tt_image_array[i];
        r2i rect = tt_get_fit(stack, data->img);
        v2i size = { data->img.width + 2, data->img.height + 2 };
        v2i offset = rect.min;

        _tt_add_entry(data->name, (r2i) {
            .min = { offset.x + 1, offset.y + 1 },
            .max = { offset.x + size.x - 1, offset.y + size.y - 1 },
        });

        for (i32 y = 0; y < data->img.height; ++y) {
            for (i32 x = 0; x < data->img.width; ++x) {
                u32 pixel = image_get(&data->img, x, y);

                image_set(&tt_table.img, x + offset.x + 1, y + offset.y + 1, pixel);
            }
        }

        {
            r2i a = {
                .min = { rect.min.x, rect.min.y + size.y },
                .max = { rect.min.x + size.x, rect.max.y },
            };

            r2i b = {
                .min = { rect.min.x + size.x, rect.min.y },
                .max = { rect.max.x, rect.max.y },
            };

            if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { buf_add(stack, a); }
            if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { buf_add(stack, b); }
        }

        if (!data->user_provided) {
            file_free_image(&data->img);
        }
    }

    {
        buf_free(tt_image_array);
        tt_image_array = NULL;
    }

    buf_free(stack);
}

