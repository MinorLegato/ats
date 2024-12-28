#include "ats.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

static usize _file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    usize size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

static FILE* _file_open(const char* path, const char* mode) {
    FILE* file = 0;
    errno_t error = fopen_s(&file, path, mode);
    return error? 0 : file;
}

ATS_API usize file_get_size(const char* path) {
    FILE* fp = _file_open(path, "rb");
    if (!fp) return 0;
    usize size = _file_get_size(fp);
    fclose(fp);
    return size;
}

ATS_API s8 file_read_s8(const char* file_name) {
    u8* buffer = 0;
    usize size = 0;
    FILE* fp = _file_open(file_name, "rb");
    if (fp) {
        size = _file_get_size(fp);
        buffer = (u8*)mem_alloc(size + 1);
        if (buffer) {
            buffer[size] = 0;
            if (fread(buffer, 1, size, fp) == 0) {
                buffer = 0;
            }
        }
        fclose(fp);
    }
    return (s8) { buffer, (isize)size };
}

ATS_API b32 file_write_s8(const char* file_name, s8 buffer) {
    FILE* fp = _file_open(file_name, "w");
    if (fp) {
        usize n = fwrite(buffer.buf, 1, buffer.len, fp);
        fclose(fp);
        return n == buffer.len;
    }
    return 0;
}

ATS_API b32 file_append_str(const char* file_name, s8 buffer) {
    FILE* fp = _file_open(file_name, "a");
    if (fp) {
        usize n = fwrite(buffer.buf, 1, buffer.len, fp);
        fclose(fp);
        return n == buffer.len;
    }
    return 0;
}

ATS_API usize file_read_bin(const char* file_name, void* buffer, usize size) {
    FILE* fp = _file_open(file_name, "rb");
    if (fp) {
        usize file_size = fread(buffer, 1, size, fp);
        fclose(fp);
        return file_size;
    }
    return 0;
} 

ATS_API b32 file_write_bin(const char* file_name, const void* buffer, usize size) {
    FILE *fp = _file_open(file_name, "wb");
    if (fp) {
        fwrite(buffer, 1, size, fp);
        fclose(fp);
        return 1;
    }
    return 0;
}

ATS_API u32* file_load_image(const char* path, u16* width, u16* height) {
    u32* pixels = 0;
    int channels = 0;
    int w = 0;
    int h = 0;
    pixels = (u32*)stbi_load(path, &w, &h, &channels, 4);
    assert(pixels);
    *width  = w;
    *height = h;
    return pixels;
}

ATS_API void file_free_image(const u32* pixels) {
    stbi_image_free((void*)pixels);
}

static void _file_cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

struct _file_iter {
    char current[MAX_PATH];
    char path[MAX_PATH];

    b32 done;

    HANDLE handle;
    WIN32_FIND_DATAA data;
};

ATS_API int file_iter_is_valid(struct _file_iter* it) {
    return !it->done;
}

ATS_API void file_iter_advance(struct _file_iter* it) {
    it->done = !FindNextFileA(it->handle, &it->data);
    if (!it->done) {
        _file_cstr_concat(it->current, it->path, it->data.cFileName);
    }
}

ATS_API struct _file_iter file_iter_create(const char* path, const char* ext) {
    if (!path) path = "./";
    if (!ext)  ext  = "*";
    struct _file_iter it = {0};
    u32 i = 0;
    for (i = 0; path[i]; ++i) {
        it.path[i] = path[i];
    }
    if (it.path[i - 1] != '/') {
        it.path[i] = '/';
    }
    char find_file_str[MAX_PATH] = {0};
    _file_cstr_concat(find_file_str, it.path, ext);
    it.handle = FindFirstFileA(find_file_str, &it.data);
    it.done = it.handle == INVALID_HANDLE_VALUE;
    if (!it.done) {
        _file_cstr_concat(it.current, it.path, it.data.cFileName);
    }
    return it;
}

ATS_API b32 file_iter_at_directory(struct _file_iter* it) {
    const char* n = it->data.cFileName;
    return (n[0] != '.') && (it->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

struct _dir_iter {
    s8 path;
    s8 name;
    s8 extension;
    i32 idx;
    struct _file_iter stack[256];
};

ATS_API void _dir_update_file_info(struct _dir_iter* it) {
    char* s = it->stack[it->idx].current;
    it->path = s8("");
    it->name = s8("");
    it->extension = s8("");
    u32 i = 0;
    u32 e = 0;
    u32 n = 0;
    for (i = 0; s && s[i]; ++i) {
        switch (s[i]) {
            case '.':
            e = i + 1; break;
            case '\\':
            case '/':
            n = i + 1; break;
        }
    }
    if (i)     it->path      = (s8) { (u8*)(s),     (isize)(i)     };
    if (e < i) it->name      = (s8) { (u8*)(s + n), (isize)(i - n) };
    if (n < i) it->extension = (s8) { (u8*)(s + e), (isize)(i - e) };
}

ATS_API struct _dir_iter dir_iter_create(const char* path) {
    struct _dir_iter it = {0};
    it.stack[it.idx] = file_iter_create(path, 0);
    _dir_update_file_info(&it);
    return it;
}

ATS_API void dir_iter_advance(struct _dir_iter* it) {
    if (file_iter_at_directory(&it->stack[it->idx])) {
        it->stack[it->idx + 1] = file_iter_create(it->stack[it->idx].current, 0);
        file_iter_advance(&it->stack[it->idx]);
        it->idx++;
        _dir_update_file_info(it);
    } else {
        file_iter_advance(&it->stack[it->idx]);
        _dir_update_file_info(it);
    }
    while ((it->idx >= 0) && !file_iter_is_valid(&it->stack[it->idx])) {
        it->idx--;
    }
}

ATS_API b32 dir_iter_is_valid(struct _dir_iter* it) {
    return it->idx >= 0;
}

