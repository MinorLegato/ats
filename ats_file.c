#include "ats.h"

#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

static usize _file_get_size(FILE* fp)
{
  fseek(fp, 0L, SEEK_END);
  usize size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return size;
}

#define _file_open stbi__fopen
#if 0
static FILE* _file_open(const char* path, const char* mode)
{
  FILE* file = 0;
  errno_t error = fopen_s(&file, path, mode);
  return error? 0 : file;
}
#endif

ATS_API usize file_get_size(const char* path)
{
  FILE* fp = _file_open(path, "rb");
  if (!fp) return 0;
  usize size = _file_get_size(fp);
  fclose(fp);
  return size;
}

ATS_API char* file_read(const char* file_name)
{
  char* buffer = 0;
  usize size = 0;
  FILE* fp = _file_open(file_name, "r");
  if (fp) {
    size = _file_get_size(fp);
    buffer = (char*)mem_alloc(size + 1);
    if (buffer) {
      buffer[size] = 0;
      if (fread(buffer, 1, size, fp) == 0) {
        buffer = 0;
      }
    }
    fclose(fp);
  }
  return buffer;
}

ATS_API b32 file_write(const char* file_name, const char* buffer)
{
  FILE* fp = _file_open(file_name, "w");
  usize size = strlen(buffer);
  if (fp) {
    usize n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return 0;
}

ATS_API b32 file_append(const char* file_name, const char* buffer)
{
  FILE* fp = _file_open(file_name, "a");
  usize size = strlen(buffer);
  if (fp) {
    usize n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return 0;
}

ATS_API usize file_read_bin(const char* file_name, void* buffer, usize size)
{
  FILE* fp = _file_open(file_name, "rb");
  if (fp) {
    usize file_size = fread(buffer, 1, size, fp);
    fclose(fp);
    return file_size;
  }
  return 0;
} 

ATS_API b32 file_write_bin(const char* file_name, const void* buffer, usize size)
{
  FILE *fp = _file_open(file_name, "wb");
  if (fp) {
    fwrite(buffer, 1, size, fp);
    fclose(fp);
    return 1;
  }
  return 0;
}

ATS_API u32* file_load_image(const char* path, u16* width, u16* height)
{
  u32* pixels = 0;
  int channels = 0;
  int w = 0;
  int h = 0;
  pixels = (u32*)stbi_load(path, &w, &h, &channels, 4);
  assert(pixels != 0);
  *width  = w;
  *height = h;
  return pixels;
}

ATS_API void file_free_image(const u32* pixels)
{
  stbi_image_free((void*)pixels);
}

static void _file_cstr_concat(char* out, const char* a, const char* b)
{
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

struct file__iter
{
  char current[MAX_PATH];
  char path[MAX_PATH];

  b32 done;

  HANDLE handle;
  WIN32_FIND_DATAA data;
};

ATS_API int file_iter_is_valid(struct file__iter* it)
{
  return !it->done;
}

ATS_API void file_iter_advance(struct file__iter* it)
{
  it->done = !FindNextFileA(it->handle, &it->data);
  if (!it->done) {
    _file_cstr_concat(it->current, it->path, it->data.cFileName);
  }
}

ATS_API struct file__iter file_iter_create(const char* path, const char* ext)
{
  if (!path) path = "./";
  if (!ext)  ext  = "*";
  struct file__iter it = {0};
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

ATS_API b32 file_iter_at_directory(struct file__iter* it)
{
  const char* n = it->data.cFileName;
  return (n[0] != '.') && (it->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

static struct {
  char path[MAX_PATH];
  char name[MAX_PATH];
  char extension[MAX_PATH];

  i32 idx;
  struct file__iter stack[256];
} dir__iter;

ATS_API i32 dir_depth(void)
{
  return dir__iter.idx;
}

static void dir__update_file_info(void)
{
  char* s = dir__iter.stack[dir__iter.idx].current;

  i32 i = 0;
  i32 j = 0;
  i32 e = MAX_PATH;
  i32 n = MAX_PATH;

  dir__iter.path[0] = '\0';
  dir__iter.name[0] = '\0';
  dir__iter.extension[0] = '\0';

  for (i = 0; s && s[i]; ++i) {
    dir__iter.path[i] = s[i];
    switch (s[i]) {
      case '\\': case '/': n = i; break;
      case '.':            e = i; break;
    }
  }

  dir__iter.path[i] = '\0';

  if (e < MAX_PATH) {
    for (j = 0; (e + j) < (i - 1); ++j) dir__iter.extension[j] = s[(e + 1) + j];
    dir__iter.extension[j] = '\0';
  } else {
    e = i;
  }

  if (n < e) {
    for (j = 0; (n + j) < (e - 1); ++j) dir__iter.name[j] = s[(n + 1) + j];
    dir__iter.name[j] = '\0';
  }
}

ATS_API void dir_open(const char* path)
{
  memset(&dir__iter, 0, sizeof dir__iter);
  dir__iter.stack[dir__iter.idx] = file_iter_create(path, 0);
  dir__update_file_info();
}

ATS_API void dir_advance(void)
{
  if (file_iter_at_directory(&dir__iter.stack[dir__iter.idx])) {
    dir__iter.stack[dir__iter.idx + 1] = file_iter_create(dir__iter.stack[dir__iter.idx].current, 0);
    file_iter_advance(&dir__iter.stack[dir__iter.idx]);
    dir__iter.idx++;
  } else {
    file_iter_advance(&dir__iter.stack[dir__iter.idx]);
  }
  while ((dir__iter.idx >= 0) && !file_iter_is_valid(&dir__iter.stack[dir__iter.idx])) {
    dir__iter.idx--;
  }
  dir__update_file_info();
}

ATS_API b32 dir_is_valid(void)
{
  return dir__iter.idx >= 0;
}

ATS_API char* dir_path(void)
{
  return dir__iter.path;
}

ATS_API char* dir_name(void)
{
  return dir__iter.name;
}

ATS_API char* dir_extension(void)
{
  return dir__iter.extension;
}

