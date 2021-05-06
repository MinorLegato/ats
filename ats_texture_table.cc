#pragma once

#include <algorithm>

// =================================== STRING ARENA THING ==================================== //

static struct {
    u32     cap;
    u32     idx;
    char*   buf;

    u32     save_count;
    u32     save_array[8];
} str_arena;

inline void reset_str_arena(void)
{
    str_arena.idx = 0;
}

inline void push_str_arena(void) {
    str_arena.save_array[str_arena.save_count++] = str_arena.idx;
}

inline void pop_str_arena(void) {
    str_arena.idx = str_arena.save_array[--str_arena.save_count];
}

inline void free_str_arena(void) {
    free(str_arena.buf);

    str_arena.cap = 0;
    str_arena.idx = 0;
    str_arena.buf = NULL;
}

inline u32 allocate_str(u32 size) {
    auto arena = &str_arena;

    u32 idx     = arena->idx;
    u32 new_idx = idx + size + 1;

    if (new_idx >= arena->cap) {
        arena->cap = arena->cap? max(arena->cap << 1, new_idx) : max(2048, new_idx);
    }

    inline const char* data(void) const {
        return &str_arena.buf[begin];
    }

    inline char* data(void) {
        return &str_arena.buf[begin];
    }

    inline u32 len(void) const {
        return end - begin;
    }
};

inline b32 str_equal(const char* a, const char* b, u32 len) {
    for (u32 i = 0; i < len; ++i) {
        if (a[i] != b[i]) { return false; }
    }

    return true;
}

inline bool operator==(Str a, Str b) {
    if (a.len() != b.len()) { return false; }

    return str_equal(a.data(), b.data(), a.len());
}

inline bool operator==(Str a, const char* b) {
    u32 len = strlen(b);
    if (a.len() != len) { return false; }

    return str_equal(a.data(), b, len);
}

inline bool operator==(const char* b, Str a) {
    return a == b;
}

inline Str make_str(u32 len) {
    u32 idx = allocate_str(len);
    return { idx, idx + len };
}

inline Str make_str(const char* str, u32 len) {
    u32 idx = allocate_str(str, len);

    return { idx, idx + len };
}

inline Str make_str(const char* str) {
    u32 len = strlen(str);
    
    return make_str(str, len);
}

inline Str append(Str a, Str b) {
    u32 len = a.len() + b.len();

    Str c = make_str(len);

    u32 j = 0;
    for (u32 i = 0; i < a.len(); ++i) c[j++] = a[i];
    for (u32 i = 0; i < b.len(); ++i) c[j++] = b[i];

    return c;
}

inline Str append(Str a, const char* b) {
    u32 b_len   = strlen(b);
    u32 n_len   = a.len() + b_len;
    Str c       = make_str(n_len);

    u32 j = 0;
    for (u32 i = 0; i < a.len(); ++i) c[j++] = a[i];
    for (u32 i = 0; i < b_len; ++i)   c[j++] = b[i];

    return c;
}

inline Str append(const char* b, Str a) {
    u32 b_len   = strlen(b);
    u32 n_len   = a.len() + b_len;
    Str c       = make_str(n_len);

    u32 j = 0;
    for (u32 i = 0; i < b_len; ++i)   c[j++] = b[i];
    for (u32 i = 0; i < a.len(); ++i) c[j++] = a[i];

    return c;
}

inline u32 index(Str str, char c) {
    for (u32 i = 0; i < str.len(); ++i) {
        if (str[i] == c) { return i; }
    }

    return 0;
}

inline Str substr(Str str, u32 begin, u32 end) {
    assert(end >= begin);

    return make_str(str.data() + begin, end - begin);
}

inline b32 empty(Str str) {
    return str.begin >= str.end;
}

inline Str split_in_place(Str* str, char c) {
    u32 ln = 0;

    while (ln < str->len() && (*str)[ln] != c) ln++;
    (*str)[ln++] = '\0';

    Str result = { str->begin, str->begin + ln - 1 };

    while ((*str)[ln] == c) {
        ln++;
    }

    str->begin += ln;

    return result;
}

inline Str split_in_place(Str* str, const char* tokens) {
    b32 token_buffer[256] = {};

    while ((*tokens) != '\0') {
        token_buffer[*(tokens++)] = true;
    }

    token_buffer['\0'] = true;

    u32 ln = 0;
    while (!token_buffer[(*str)[ln]]) ln++;
    (*str)[ln++] = '\0';

    Str result = { str->begin, str->begin + ln - 1 };

    while (token_buffer[(*str)[ln]]) {
        ln++;
    }

    str->begin += ln;

    return result;
}

inline Str copy(Str str) {
    Str c = make_str(str.len());
    memcpy(c.data(), str.data(), str.len());
    return c;
}

inline Str to_lower(Str str) {
    Str c = copy(str);

    for (u32 i = 0; i < c.len(); ++i) {
        if (c[i] >= 'A' && c[i] <= 'Z') { c[i] += ('a' - 'A'); }
    }

    return c;
}

inline Str to_upper(Str str) {
    Str c = copy(str);

    for (u32 i = 0; i < c.len(); ++i) {
        if (c[i] >= 'a' && c[i] <= 'z') { c[i] -= ('a' - 'A'); }
    }

    return c;
}

inline Str file_read_str(const char* file_name) {
    FILE *fp = NULL;

    assert(fopen_s(&fp, file_name, "rb") == 0);

    size_t size = file_get_size(fp);

    Str str = make_str(size);
    assert(fread(str.data(), 1, size, fp) == size);

    fclose(fp);

    return str;
}

// ============================================= TEXTURE ======================================== //

#ifndef TEXTURE_TABLE_SIZE
#define TEXTURE_TABLE_SIZE (256)
#endif

struct Texture_Entry {
    b32     in_use;

    Rect    rect;

    u32     hash;
    char    name[128];
};

static u32 texture_table_packed_texture;
struct Texture_Entry texture_table[TEXTURE_TABLE_SIZE];

struct Texture_ID {
    u16 index;

    inline Rect get_rect(void) const {
        return texture_table[index].rect;
    }
};

static void push_texture_rect(const char* name, Rect rect) {
    u32 hash  = hash_cstr(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (texture_table[index].in_use) {
        if ((texture_table[index].hash == hash) && (strcmp(texture_table[index].name, name) == 0)) {
            assert(true);
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }

    Texture_Entry* entry = &texture_table[index];
    
    entry->in_use   = true;
    entry->rect     = rect;
    entry->hash     = hash;

    strcpy(entry->name, name);
}

static Texture_ID get_texture(const char* name) {
    u32 hash  = hash_cstr(name);
    u16 index = hash % TEXTURE_TABLE_SIZE;

    while (texture_table[index].in_use) {
        if ((texture_table[index].hash == hash) && (strcmp(texture_table[index].name, name) == 0)) {
            return { .index = index };
        }

        index = (index + 1) % TEXTURE_TABLE_SIZE;
    }
    
    assert(false);

    return { .index = 0 };
}

struct Image_Data {
    Image   image;
    Str     name;
};

struct Tex_Rect {
    v2i min;
    v2i max;
};

static u32 load_texture_directory(const char* dir_path) {
    push_str_arena();

    u32 file_count = 0;
    Str file_name[1024];

    Str dir_str = append(make_str(dir_path), "/");

    // get all .png files in directory:
    {
        WIN32_FIND_DATA find_data = {};
        HANDLE find_handle = FindFirstFile(append(dir_str, "*.png*").data(), &find_data);

        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                file_name[file_count++] = make_str(find_data.cFileName);
            } while(FindNextFile(find_handle, &find_data));

            FindClose(find_handle);
        }
    }

    static u32 pixels[1024 * 1024];

    // load all images:
    {
        i32         image_count = 0;
        Image_Data  image_array[1024];

        for (u32 i = 0; i < file_count; ++i) {
            Str fn = file_name[i];

            image_array[image_count++] = {
                .image  = image_load_from_file(append(dir_str, fn).data()),
                .name   = substr(fn, 0, index(fn, '.')),
            };
        }

        std::sort(image_array, image_array + image_count, [] (auto a, auto b) {
            return (a.image.width > b.image.width) || (a.image.height > b.image.height);
        });

        i32 rect_count  = 0;
        static Tex_Rect rect_array[1024];

        rect_array[rect_count++] = {
            .min = { 0, 0 },
            .max = { 1024, 1024 },
        };

        b32 split_vertical = true;

        for (i32 i = 0; i < image_count; ++i) {
            auto [image, name] = image_array[i];

            v2i size = { image.width, image.height };
            i32 rect_index = 0;

            for (i32 j = 0; j < rect_count; ++j) {
                Tex_Rect rect = rect_array[j];

                if (size.x < (rect.max.x - rect.min.x) && size.y < (rect.max.y - rect.min.y)) {
                    break;
                }
            }

            Tex_Rect rect = rect_array[rect_index];
            rect_array[rect_index] = rect_array[--rect_count];

            v2i offset = rect.min;

            push_texture_rect(name.data(), Rect {
                .min = { f32(offset.x + 0.1) / 1024.0f, f32(offset.y + 0.1) / 1024.0f },
                .max = { f32(offset.x + size.x - 0.1) / 1024.0f, f32(offset.y + size.y - 0.1) / 1024.0f },
            });

            for (i32 y = 0; y < image.height; ++y) for (i32 x = 0; x < image.width; ++x) {
                pixels[(y + offset.y) * 1024 + (x + offset.x)] = image.pixels[y * image.width + x];
            }
            
            {
                Tex_Rect a = {};
                Tex_Rect b = {};

                if (split_vertical) {
                    a = { .min = { rect.min.x,           rect.min.y + size.y },  .max = { rect.min.x + size.x, rect.max.y } };
                    b = { .min = { rect.min.x + size.x,  rect.min.y },           .max = rect.max };
                } else {
                    a = { .min = { rect.min.x + size.x,  rect.min.y },           .max = { rect.max.x, rect.min.y + size.y } };
                    b = { .min = { rect.min.x,           rect.min.y + size.y },  .max = rect.max };
                }

                if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = a; }
                if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { rect_array[rect_count++] = b; }

                //split_vertical = !split_vertical;
            }

            free(image.pixels);
        }
    }

    texture_table_packed_texture = texture_create(pixels, 1024, 1024, false).id;

    pop_str_arena();

    return texture_table_packed_texture;
};

