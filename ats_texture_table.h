#pragma once

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

typedef struct Texture_ID       Texture_ID;
typedef struct Texture_Entry    Texture_Entry;
typedef struct Texture_Table    Texture_Table;

extern void             tt_begin(int width, int height);
extern void             tt_end(void);
extern void             tt_add_image(const char* name, Image image);

extern R2i              tt_get_rect(Texture_ID id);
extern Texture_ID       tt_get_id(const char* name);
extern R2i              tt_get(const char* name);
extern void             tt_load_from_dir(const char* dir_path);

extern Image            tt_get_image(void);

struct Texture_ID {
    u16 index;
};

struct Texture_Entry {
    b32     in_use;
    u32     hash;

    R2i     rect;

    char    name[64];
};

struct Texture_Table {
    Image           image;
    Texture_Entry   array[TEXTURE_TABLE_SIZE];
};

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

typedef struct {
    Image       image;
    char        name[256];
} TT_Image;

static Texture_Table    tt_table;

static u32              tt_image_count;
static u32              tt_image_capacity;
static TT_Image*        tt_image_array;

static TT_Image* tt_new_image(void) {
    if (tt_image_count >= tt_image_capacity) {
        tt_image_capacity   = !tt_image_capacity? 256 : (tt_image_capacity << 1);
        tt_image_array      = (TT_Image*)realloc(tt_image_array, tt_image_capacity * (sizeof *tt_image_array));
    }

    TT_Image* data = &tt_image_array[tt_image_count++];
    memset(data, 0, sizeof (data));
    return data;
}

extern void tt_add_image(const char* name, Image image) {
    TT_Image* data = tt_new_image();

    strcpy(data->name, name);
    data->image = image;
}

extern Image tt_get_image(void) {
    return tt_table.image;
}

extern R2i tt_get_rect(Texture_ID id) {
    return tt_table.array[id.index].rect;
}

extern Texture_ID tt_get_id(const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0)) {
            Texture_ID id = { index };
            return id;
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    Texture_ID id = ATS_INIT_ZERO;
    return id;
}

extern R2i tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

static void _tt_add_entry(const char* name, R2i rect) {
    u32 hash = hash_str(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (tt_table.array[index].in_use) {
        if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0))
            assert(true);

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    Texture_Entry* entry = &tt_table.array[index];
    
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
    TT_Image* a = (TT_Image*)va;
    TT_Image* b = (TT_Image*)vb;

    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;

    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(R2i rect, Image image) {
    i32 rect_width  = rect.max.x - rect.min.x;
    i32 rect_height = rect.max.y - rect.min.y;

    return image.width <= rect_width && image.height <= rect_height; 
}

extern void tt_load_from_dir(const char* dir_path) {
    char find_file_str[256] = ATS_INIT_ZERO;
    cstr_concat(find_file_str, dir_path, "*.png*");

    WIN32_FIND_DATA find_data = ATS_INIT_ZERO;
    HANDLE find_handle = FindFirstFile(find_file_str, &find_data);

    if (find_handle != INVALID_HANDLE_VALUE) {
        do {
            TT_Image* data = tt_new_image();

            char file_path[256];
            cstr_concat(file_path, dir_path, find_data.cFileName);
            data->image = image_load_from_file(file_path);

            cstr_copy_no_file_extension(data->name, find_data.cFileName);
        } while (FindNextFile(find_handle, &find_data));

        FindClose(find_handle);
    }
}

extern void tt_begin(int width, int height) {
    tt_table = ctor(Texture_Table, { width, height, (u32*)calloc(width * height, sizeof (u32)) });

    tt_table.array[0].in_use = true;

    for (u32 i = 0; i < width * height; ++i) {
        tt_table.image.pixels[i] = 0xff000000;
    }
}

typedef struct {
    u32     top;
    u32     cap;
    R2i*    buf;
} TT_Rect_Stack;

static void tt_push_rect(TT_Rect_Stack* stack, R2i rect) {
    if (stack->top >= stack->cap) {
        stack->cap = !stack->cap? 256 : (stack->cap << 1);
        stack->buf = (R2i*)realloc(stack->buf, stack->cap * sizeof (R2i));
    }

    stack->buf[stack->top++] = rect;
}

static R2i tt_get_fit(TT_Rect_Stack* stack, Image image) {
    u32 j = 0;
    for (j = 0; j < stack->top; ++j) {
        if (rect_contains_image(stack->buf[j], image)) {
            break;
        }
    }

    R2i rect        = stack->buf[j];
    stack->buf[j]   = stack->buf[--stack->top];

    return rect;
}

extern void tt_end(void) {
    TT_Rect_Stack stack = ATS_INIT_ZERO;
    tt_push_rect(&stack, r2i(v2i(0, 0), v2i(tt_table.image.width, tt_table.image.height)));

    qsort(tt_image_array, tt_image_count, sizeof (TT_Image), cmp_tt_image);

    for (u32 i = 0; i < tt_image_count; ++i) {
        TT_Image*   data    = &tt_image_array[i];
        R2i         rect    = tt_get_fit(&stack, data->image);
        V2i         size    = v2i(data->image.width + 2, data->image.height + 2);
        V2i         offset  = rect.min;

        _tt_add_entry(data->name, r2i(v2i(offset.x + 1, offset.y + 1),
                                      v2i(offset.x + size.x - 1, offset.y + size.y - 1)));

        for (i32 y = 0; y < data->image.height; ++y) {
            for (i32 x = 0; x < data->image.width; ++x) {
                image_set_pixel(&tt_table.image, x + offset.x + 1, y + offset.y + 1, image_get_pixel(&data->image, x, y));
            }
        }

        {
            R2i a = { { rect.min.x,           rect.min.y + size.y },  { rect.min.x + size.x, rect.max.y } };
            R2i b = { { rect.min.x + size.x,  rect.min.y },           rect.max };

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

