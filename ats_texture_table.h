#ifndef __ATS_TEXTURE_TABLE_H__
#define __ATS_TEXTURE_TABLE_H__

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

struct texture_id {
    u16 index;
};

struct texture_entry {
    b32 in_use;
    u32 hash;
    r2i rect;
    char name[64];
};

struct texture_table {
    struct image image;
    struct texture_entry array[TEXTURE_TABLE_SIZE];
};

extern void tt_begin(int width, int height);
extern void tt_end(void);
extern void tt_add_image(const char* name, struct image image);
extern void tt_load_from_dir(const char* dir_path);

extern struct texture_id tt_get_id(const char* name);
extern r2i tt_get_rect(struct texture_id id);
extern r2i tt_get(const char* name);

extern struct image tt_get_image(void);

#endif // __ATS_TEXTURE_TABLE_H__

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

struct tt_image {
    struct image image;
    char name[256];
};

global struct texture_table tt_table;

global usize tt_image_count;
global usize tt_image_capacity;
global struct tt_image* tt_image_array;

function struct tt_image*
tt_new_image(void) {
    if (tt_image_count >= tt_image_capacity) {
        tt_image_capacity = !tt_image_capacity? 256 : (tt_image_capacity << 1);
        tt_image_array = realloc(tt_image_array, tt_image_capacity * (sizeof *tt_image_array));
    }

    struct tt_image* data = &tt_image_array[tt_image_count++];
    memset(data, 0, sizeof (data));
    return data;
}

extern void
tt_add_image(const char* name, struct image image) {
    struct tt_image* data = tt_new_image();
    strcpy(data->name, name);
    data->image = image;
}

extern struct image
tt_get_image(void) {
    return tt_table.image;
}

extern r2i
tt_get_rect(struct texture_id id) {
    return tt_table.array[id.index].rect;
}

extern struct texture_id
tt_get_id(const char* name) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;
    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0)) {
            struct texture_id id = { index };
            return id;
        }
        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    assert(false);
    return (struct texture_id) {0};
}

extern r2i
tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

function void
_tt_add_entry(const char* name, r2i rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    struct texture_entry* entry = &tt_table.array[index];

    entry->in_use = true;
    entry->rect = rect;
    entry->hash = hash;

    strcpy_s(entry->name, 64, name);
}

function void
cstr_copy_without_extension(char* out, char* str) {
    int i = strlen(str) - 1;
    while (i && str[i] != '.') i--;
    CopyMemory(out, str, i);
    out[i] = '\0';
}

function void
cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

function int
tt_cmp_image(const void* va, const void* vb) {
    struct tt_image* a = (struct tt_image*)va;
    struct tt_image* b = (struct tt_image*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32
rect_contains_image(r2i rect, struct image image) {
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
        struct tt_image* data = tt_new_image();
        char file_path[256];
        cstr_concat(file_path, dir_path, find_data.cFileName);
        data->image = image_load_from_file(file_path);
        cstr_copy_without_extension(data->name, find_data.cFileName);
    } while (FindNextFile(find_handle, &find_data));

    FindClose(find_handle);
}

extern void
tt_begin(int width, int height) {
    tt_table = (struct texture_table) {
        width,
        height, 
        (u32*)calloc(width * height, sizeof (u32))
    };

    tt_table.array[0].in_use = true;

    for (u32 i = 0; i < width * height; ++i) {
        tt_table.image.pixels[i] = 0xff000000;
    }
}

struct tt_rect_stack {
    u32 top;
    u32 cap;
    r2i* buf;
};

function void
tt_push_rect(struct tt_rect_stack* stack, r2i rect) {
    if (stack->top >= stack->cap) {
        stack->cap = !stack->cap? 256 : (stack->cap << 1);
        stack->buf = realloc(stack->buf, stack->cap * sizeof (r2i));
    }
    stack->buf[stack->top++] = rect;
}

function r2i
tt_get_fit(struct tt_rect_stack* stack, struct image image) {
    u32 j = 0;
    for (j = 0; j < stack->top; ++j) {
        if (rect_contains_image(stack->buf[j], image)) {
            break;
        }
    }
    r2i rect = stack->buf[j];
    stack->buf[j] = stack->buf[--stack->top];
    return rect;
}

extern void
tt_end(void) {
    struct tt_rect_stack stack = {0};
    tt_push_rect(&stack, r2i(v2i(0, 0), v2i(tt_table.image.width - 1, tt_table.image.height - 1)));

    qsort(tt_image_array, tt_image_count, sizeof (struct tt_image), tt_cmp_image);

    for (u32 i = 0; i < tt_image_count; ++i) {
        struct tt_image* data = &tt_image_array[i];
        r2i rect = tt_get_fit(&stack, data->image);
        v2i size = v2i(data->image.width + 2, data->image.height + 2);
        v2i offset = rect.min;

        _tt_add_entry(data->name, r2i(v2i(offset.x + 1, offset.y + 1),
                                      v2i(offset.x + size.x - 1, offset.y + size.y - 1)));

        for (i32 y = 0; y < data->image.height; ++y) {
            for (i32 x = 0; x < data->image.width; ++x) {
                u32 pixel = image_get(&data->image, x, y);
                image_set(&tt_table.image, x + offset.x + 1, y + offset.y + 1, pixel);
            }
        }

        {
            r2i a = {
                { rect.min.x, rect.min.y + size.y },
                { rect.min.x + size.x, rect.max.y }
            };

            r2i b = {
                { rect.min.x + size.x, rect.min.y },
                { rect.max.x, rect.max.y },
            };

            if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { tt_push_rect(&stack, a); }
            if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { tt_push_rect(&stack, b); }
        }

        free(data->image.pixels);
    }

    {
        free(tt_image_array);

        tt_image_array      = NULL;
        tt_image_capacity   = 0;
        tt_image_count      = 0;
    }

    free(stack.buf);
}

#endif

