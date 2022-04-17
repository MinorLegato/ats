#ifndef __ATS_TEXTURE_TABLE_H__
#define __ATS_TEXTURE_TABLE_H__

#define TEXTURE_TABLE_SIZE (1024)

#ifndef TEXTURE_BORDER
#define TEXTURE_BORDER 0
#endif

struct Texture_ID {
    u16 index;
};

struct Texture_Entry {
    b32 in_use;
    u32 hash;
    Rect2i rect;
    char name[64];
};

struct Texture_Table {
    Image image;
    Texture_Entry array[TEXTURE_TABLE_SIZE];
};

extern void         tt_begin(int width, int height);
extern void         tt_end(void);
extern void         tt_add_image(const char* name, Image image);

extern Texture_ID   tt_get_id(const char* name);
extern Rect2i       tt_get_rect(Texture_ID id);
extern Rect2i       tt_get(const char* name);
extern void         tt_load_from_Dir(const char* dir_path);

extern Image        tt_get_image(void);

#endif // __ATS_TEXTURE_TABLE_H__

#ifdef ATS_IMPL
// ====================================================================================== //
// ====================================== IMPL ========================================== //
// ====================================================================================== //

struct TT_Image {
    Image   image;
    char    name[256];
};

static Texture_Table tt_table;

static u32 tt_image_count;
static u32 tt_image_capacity;

static TT_Image* tt_image_array;

static TT_Image* tt_new_image(void) {
    if (tt_image_count >= tt_image_capacity) {
        tt_image_capacity   = !tt_image_capacity? 256 : (tt_image_capacity << 1);
        tt_image_array      = (TT_Image*)realloc(tt_image_array, tt_image_capacity * (sizeof *tt_image_array));
    }
    TT_Image* data = &tt_image_array[tt_image_count++];
    clear_memory(data, sizeof (data));
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

extern Rect2i tt_get_rect(Texture_ID id) {
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
    Texture_ID id = {};
    return id;
}

extern Rect2i tt_get(const char* name) {
    return tt_get_rect(tt_get_id(name));
}

static void _tt_add_entry(const char* name, Rect2i rect) {
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

static void cstr_copy_without_extension(char* out, char* str) {
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

static int tt_cmp_image(const void* va, const void* vb) {
    TT_Image* a = (TT_Image*)va;
    TT_Image* b = (TT_Image*)vb;
    int dw = b->image.width  - a->image.width;
    int dh = a->image.height - a->image.height;
    return b->image.width - a->image.width;
}

extern b32 rect_contains_image(Rect2i rect, Image image) {
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
        auto data = tt_new_image();
        char file_path[256];
        cstr_concat(file_path, dir_path, find_data.cFileName);
        data->image = image_load_from_file(file_path);
        cstr_copy_without_extension(data->name, find_data.cFileName);
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

struct TT_Rect_Stack {
    u32     top;
    u32     cap;
    Rect2i* buf;

    inline void push(Rect2i rect) {
        if (this->top >= this->cap) {
            this->cap = !this->cap? 256 : (this->cap << 1);
            this->buf = (Rect2i*)realloc(this->buf, this->cap * sizeof (Rect2i));
        }
        this->buf[this->top++] = rect;
    }

    inline Rect2i get_fit(Image image) {
        u32 j = 0;
        for (j = 0; j < this->top; ++j) {
            if (rect_contains_image(this->buf[j], image)) {
                break;
            }
        }
        Rect2i rect = this->buf[j];
        this->buf[j] = this->buf[--this->top];
        return rect;
    }
};


extern void tt_end(void) {
    TT_Rect_Stack stack = {};
    stack.push(r2i(v2i(0, 0), v2i(tt_table.image.width, tt_table.image.height)));
    qsort(tt_image_array, tt_image_count, sizeof (TT_Image), tt_cmp_image);
    for (u32 i = 0; i < tt_image_count; ++i) {
        auto data  = &tt_image_array[i];
        auto rect   = stack.get_fit(data->image);
        auto size   = v2i(data->image.width + 2, data->image.height + 2);
        auto offset = rect.min;
        _tt_add_entry(data->name, r2i(v2i(offset.x + 1, offset.y + 1), v2i(offset.x + size.x - 1, offset.y + size.y - 1)));
        for (i32 y = 0; y < data->image.height; ++y) {
            for (i32 x = 0; x < data->image.width; ++x) {
                set_pixel(&tt_table.image, x + offset.x + 1, y + offset.y + 1, get_pixel(&data->image, x, y));
            }
        }
        Rect2i a = { { rect.min.x, rect.min.y + size.y }, { rect.min.x + size.x, rect.max.y } };
        Rect2i b = { { rect.min.x + size.x, rect.min.y }, rect.max };
        if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { stack.push(a); }
        if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { stack.push(b); }
        free(data->image.pixels);
    }
    free(tt_image_array);
    tt_image_array      = NULL;
    tt_image_capacity   = 0;
    tt_image_count      = 0;
    free(stack.buf);
}

#endif

