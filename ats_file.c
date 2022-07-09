
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

extern char*
file_read_str(const char* file_name, m_allocator allocator) {
    FILE* fp = NULL;
    char* buffer = NULL;
    if (fopen_s(&fp, file_name, "rb") == 0) {
        usize size = file_get_size(fp);
        buffer = (char*)m_alloc(allocator, size + 1);
        if (buffer) {
            buffer[size] = 0;
            if (fread(buffer, 1, size, fp) == 0) {
                m_free(allocator, buffer);
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
    if (fopen_s(&fp, file_name, "w") == 0) {
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
    if (fopen_s(&fp, file_name, "a") == 0) {
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
    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);
        return true;
    }
    return false;
} 

extern b32
file_write_bin(const char* file_name, const void* buffer, usize size) {
    FILE *fp = NULL;
    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return false;
}

extern image
file_load_image(const char* path) {
    image image = {0};
    i32 channels = 0;
    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);
    return image;
}

extern void
file_free_image(image* img) {
    stbi_image_free(img->pixels);
    *img = (image) {0};
}

