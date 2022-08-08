#define WIN32_LEAN_AND_MEAN
#undef APIENTRY
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

static usize
file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    usize size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

static FILE*
file_open(const char* path, const char* mode) {
    FILE* file = fopen(path, mode);
    return file;
}

extern char*
file_read_str(const char* file_name, mem_allocator allocator) {
    FILE* fp = NULL;
    char* buffer = NULL;
    if (fp = file_open(file_name, "rb")) {
        usize size = file_get_size(fp);
        buffer = (char*)mem_alloc(allocator, size + 1);
        if (buffer) {
            buffer[size] = 0;
            if (fread(buffer, 1, size, fp) == 0) {
                mem_free(allocator, buffer);
                buffer = NULL;
            }
        }
        fclose(fp);
    }
    return buffer;
}

extern b32
file_write_str(const char* file_name, const char* buffer) {
    FILE* fp = NULL;
    if (fp = file_open(file_name, "w")) {
        usize size = strlen(buffer);
        usize n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }
    return false;
}

extern b32
file_append_str(const char* file_name, const char* buffer) {
    FILE* fp = NULL;
    if (fp = file_open(file_name, "a")) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }
    return false;
}

extern b32
file_read_bin(const char* file_name, void* buffer, usize size) {
    FILE *fp = NULL;
    if (fp = file_open(file_name, "rb")) {
        fread(buffer, size, 1, fp);
        fclose(fp);
        return true;
    }
    return false;
} 

extern b32
file_write_bin(const char* file_name, const void* buffer, usize size) {
    FILE *fp = NULL;
    if (fp = file_open(file_name, "wb")) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return false;
}

extern image_t
file_load_image(const char* path) {
    image_t image = {0};
    i32 channels = 0;
    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);

    return image;
}

extern void
file_free_image(image_t* img) {
    stbi_image_free(img->pixels);
    *img = (image_t) {0};
}



typedef struct file_iter {
    char current[MAX_PATH];

    b32 done;
    const char* path;

    HANDLE handle;
    WIN32_FIND_DATA data;
} file_iter;

static bool
file_iter_is_valid(const file_iter* it) {
    return !it->done;
}

static inline void
file_cstr_concat(char* out, const char* a, const char* b) {
    while (*a) *out++ = *a++;
    while (*b) *out++ = *b++;
    *(out) = '\0';
}

static void
file_iter_advance(file_iter* it) {
    it->done = !FindNextFile(it->handle, &it->data);

    if (!it->done) {
        file_cstr_concat(it->current, it->path, it->data.cFileName);
    }
}

static file_iter
file_iter_create(const char* path, const char* ext) {
    if (!path) path = "";
    if (!ext) ext = "*";

    file_iter it = {0};

    it.path = path;

    char find_file_str[MAX_PATH] = {0};
    file_cstr_concat(find_file_str, path, ext);

    it.handle = FindFirstFile(find_file_str, &it.data);
    it.done = it.handle == INVALID_HANDLE_VALUE;

    if (!it.done) {
        file_cstr_concat(it.current, it.path, it.data.cFileName);
    }

    return it;
}

