#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

static usize _file_get_size(FILE* fp) {
  fseek(fp, 0L, SEEK_END);
  usize size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

static FILE* file_open(const char* path, const char* mode) {
  FILE* file = fopen(path, mode);
  return file;
}

extern usize file_get_size(const char* path) {
  FILE* fp = fopen(path, "rb");
  if (!fp) return 0;
  usize size = _file_get_size(fp);
  fclose(fp);
  return size;
}

extern char* file_read_str(const char* file_name) {
  FILE* fp = NULL;
  char* buffer = NULL;
  if (fp = file_open(file_name, "rb")) {
    usize size = _file_get_size(fp);
    buffer = (char*)mem_alloc(size + 1);
    if (buffer) {
      buffer[size] = 0;
      if (fread(buffer, 1, size, fp) == 0) {
        buffer = NULL;
      }
    }
    fclose(fp);
  }
  return buffer;
}

extern b32 file_write_str(const char* file_name, const char* buffer) {
  FILE* fp = NULL;
  if (fp = file_open(file_name, "w")) {
    usize size = strlen(buffer);
    usize n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return false;
}

extern b32 file_append_str(const char* file_name, const char* buffer) {
  FILE* fp = NULL;
  if (fp = file_open(file_name, "a")) {
    usize size = strlen(buffer);
    usize n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return false;
}

extern b32 file_read_bin(const char* file_name, void* buffer, usize size) {
  FILE *fp = NULL;
  if (fp = file_open(file_name, "rb")) {
    fread(buffer, size, 1, fp);
    fclose(fp);
    return true;
  }
  return false;
} 

extern b32 file_write_bin(const char* file_name, const void* buffer, usize size) {
  FILE *fp = NULL;
  if (fp = file_open(file_name, "wb")) {
    fwrite(buffer, size, 1, fp);
    fclose(fp);
    return 1;
  }
  return false;
}

extern image file_load_image(const char* path) {
  image img = ATS_INIT;
  i32 channels = 0;
  img.pixels = (u32*)stbi_load(path, &img.width, &img.height, &channels, 4);
  assert(img.pixels);
  return img;
}

extern void file_free_image(image* img) {
  stbi_image_free(img->pixels);
  *img = make(image) ATS_INIT;
}

typedef struct file_iter {
  char current[MAX_PATH];

  b32 done;
  const char* path;

  HANDLE handle;
  WIN32_FIND_DATAA data;
} file_iter;

static bool file_iter_is_valid(const file_iter* it) {
  return !it->done;
}

static inline void file_cstr_concat(char* out, const char* a, const char* b) {
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static void file_iter_advance(file_iter* it) {
  it->done = !FindNextFileA(it->handle, &it->data);
  if (!it->done) {
    file_cstr_concat(it->current, it->path, it->data.cFileName);
  }
}

static file_iter file_iter_create(const char* path, const char* ext) {
  if (!path) path = "";
  if (!ext) ext = "*";

  file_iter it = ATS_INIT;

  it.path = path;

  char find_file_str[MAX_PATH] = ATS_INIT;
  file_cstr_concat(find_file_str, path, ext);

  it.handle = FindFirstFileA(find_file_str, &it.data);
  it.done = it.handle == INVALID_HANDLE_VALUE;

  if (!it.done) {
    file_cstr_concat(it.current, it.path, it.data.cFileName);
  }

  return it;
}

