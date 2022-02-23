#pragma once

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

struct texture_id {
    u16 index;
};

struct texture_entry {
    b32     in_use;
    u32     hash;

    irect2  rect;

    char    name[64];
};

struct texture_table {
    image_t         image;
    texture_entry   array[TEXTURE_TABLE_SIZE];
};

extern void             tt_begin(int width, int height);
extern void             tt_end(void);
extern void             tt_add_image(const char* name, image_t image);
extern void             tt_add_image()

extern texture_id       tt_get_id(const char* name);
extern irect2           tt_get_rect(texture_id id);
extern irect2           tt_get(const char* name);
extern void             tt_load_from_dir(const char* dir_path);

extern image_t          tt_get_image(void);


#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

typedef struct {
    image_t     image;
    char        name[256];
} tt_image;

static texture_table    tt_table;

static u32              tt_image_count;
static u32              tt_image_capacity;
static tt_image*        tt_image_array;

static tt_image* tt_new_image(void) {
    if (tt_image_count >= tt_image_capacity) {
        tt_image_capacity   = !tt_image_capacity? 256 : (tt_image_capacity << 1);
        tt_image_array      = (tt_image*)realloc(tt_image_array, tt_image_capacity * (sizeof *tt_image_array));
    }

    tt_image* data = &tt_image_array[tt_image_count++];
    memset(data, 0, sizeof (data));
    return data;
}

extern void tt_add_image(const char* name, image_t image) {
    tt_image* data = tt_new_image();

    strcpy(data->name, name);
    data->image = image;
}

extern image_t tt_get_image(void) {
    return tt_table.image;
}

extern irect2 tt_get_rect(texture_id id) {
    return tt_table.array[id.index].rect;
}

extern texture_id tt_get_id(const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0)) {
            texture_id id = { index };
            return id;
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    assert(false);

    texture_id id = {};
    return id;
}

extern irect2 tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

static void _tt_add_entry(const char* name, irect2 rect) {
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

static int cmp_tt_image(const void* va, const void* vb) {
    tt_image* a = (tt_image*)va;
    tt_image* b = (tt_image*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(irect2 rect, image_t image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height;
}

extern void tt_load_from_dir(const char* dir_path) {
    char find_file_str[256] = {};
    cstr_concat(find_file_str, dir_path, "*.png*");

    WIN32_FIND_DATA find_data   = {};
    HANDLE          find_handle = FindFirstFile(find_file_str, &find_data);

    assert(find_handle != INVALID_HANDLE_VALUE);

    do {
        tt_image* data = tt_new_image();

        char file_path[256];
        cstr_concat(file_path, dir_path, find_data.cFileName);
        data->image = image_load_from_file(file_path);

        cstr_copy_no_file_extension(data->name, find_data.cFileName);
    } while (FindNextFile(find_handle, &find_data));

    FindClose(find_handle);
}

extern void tt_begin(int width, int height) {
    tt_table = { width, height, (u32*)calloc(width * height, sizeof (u32)) };

    tt_table.array[0].in_use = true;

    for (u32 i = 0; i < width * height; ++i) {
        tt_table.image.pixels[i] = 0xff000000;
    }
}

struct tt_rect_stack {
    u32         top;
    u32         cap;
    irect2*     buf;
};

static void tt_push_rect(tt_rect_stack* stack, irect2 rect) {
    if (stack->top >= stack->cap) {
        stack->cap = !stack->cap? 256 : (stack->cap << 1);
        stack->buf = (irect2*)realloc(stack->buf, stack->cap * sizeof (irect2));
    }

    stack->buf[stack->top++] = rect;
}

static irect2 tt_get_fit(tt_rect_stack* stack, image_t image) {
    u32 j = 0;
    for (j = 0; j < stack->top; ++j) {
        if (rect_contains_image(stack->buf[j], image)) {
            break;
        }
    }

    irect2 rect     = stack->buf[j];
    stack->buf[j]   = stack->buf[--stack->top];

    return rect;
}

extern void tt_end(void) {
    tt_rect_stack stack = {};
    tt_push_rect(&stack, ir2(iv2(0, 0), iv2(tt_table.image.width, tt_table.image.height)));

    qsort(tt_image_array, tt_image_count, sizeof (tt_image), cmp_tt_image);

    for (u32 i = 0; i < tt_image_count; ++i) {
        tt_image*   data    = &tt_image_array[i];
        irect2      rect    = tt_get_fit(&stack, data->image);
        ivec2       size    = iv2(data->image.width + 2, data->image.height + 2);
        ivec2       offset  = rect.min;

        _tt_add_entry(data->name, ir2(iv2(offset.x + 1, offset.y + 1),
                                      iv2(offset.x + size.x - 1, offset.y + size.y - 1)));

        for (i32 y = 0; y < data->image.height; ++y) {
            for (i32 x = 0; x < data->image.width; ++x) {
                image_set_pixel(&tt_table.image, x + offset.x + 1, y + offset.y + 1, image_get_pixel(&data->image, x, y));
            }
        }

        {
            irect2 a = { { rect.min.x,           rect.min.y + size.y },  { rect.min.x + size.x, rect.max.y } };
            irect2 b = { { rect.min.x + size.x,  rect.min.y },           rect.max };

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

