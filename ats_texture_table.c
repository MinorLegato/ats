
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

typedef struct tt_image {
    b32 user_provided;

    image_t img;
    char name[256];
} tt_image;

static mem_allocator tt_allocator;

static texture_table tt_table;

static usize    tt_image_count;
static tt_image tt_image_array[1024];

extern texture_table*
tt_get_texture_table(void) {
    return &tt_table;
}

extern void
tt_add_image(const char* name, image_t img) {
    tt_image data = {0};

    data.user_provided = true;
    data.img = img;
    strcpy(data.name, name);

    tt_image_array[tt_image_count++] = data;
}

extern image_t
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
cstr_copy_without_extension(char* out, const char* str) {
    while (*str && *str != '.') {
        *(out++) = *(str++);
    }
    *out = '\0';
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
rect_contains_image(r2i rect, image_t image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;
    return image.width <= rect_width && image.height <= rect_height;
}

extern void
tt_load_from_dir(const char* dir_path) {
    for_iter(file_iter, it, file_iter_create(dir_path, "*.png")) {
        tt_image data = {0};
        data.img = file_load_image(it.current);
        cstr_copy_without_extension(data.name, it.data.cFileName);
        tt_image_array[tt_image_count++] = data;
    }
}

extern void
tt_begin(int width, int height, mem_allocator allocator) {
    tt_allocator = allocator;
    tt_image_count = 0;

    tt_table = (texture_table) {
        width,
        height, 
        (u32*)mem_zero(tt_allocator, width * height * sizeof (u32)),
    };

    tt_table.array[0].in_use = true;

    for (u32 i = 0; i < width * height; ++i) {
        tt_table.img.pixels[i] = 0xff000000;
    }
}

static usize tt_stack_top; 
static r2i   tt_stack_buf[4096];

static r2i
tt_get_fit(image_t img) {
    u32 j = 0;
    for (j = 0; j < tt_stack_top; ++j) {
        if (rect_contains_image(tt_stack_buf[j], img)) {
            break;
        }
    }
    r2i rect = tt_stack_buf[j];
    tt_stack_buf[j] = tt_stack_buf[--tt_stack_top];
    return rect;
}

extern void
tt_end(void) {
    tt_stack_top = 0;
    tt_stack_buf[tt_stack_top++] = (r2i) {
        .min = { 0, 0 },
        .max = { tt_table.img.width - 1, tt_table.img.height - 1 },
    };

    qsort(tt_image_array, tt_image_count, sizeof (tt_image), tt_cmp_image);

    for (usize i = 0; i < tt_image_count; ++i) {
        tt_image* data = &tt_image_array[i];

        r2i rect = tt_get_fit(data->img);
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

            if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { tt_stack_buf[tt_stack_top++] = a; }
            if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { tt_stack_buf[tt_stack_top++] = b; }
        }

        if (!data->user_provided) {
            file_free_image(&data->img);
        }
    }

    tt_image_count = 0;
    tt_stack_top = 0;
}

