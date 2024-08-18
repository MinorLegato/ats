#pragma once

#include "ats_base.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ext/stb_image_resize.h" 

#define TEXTURE_TABLE_SIZE (1024)

#define tex_id(...) (make(tex_id) { __VA_ARGS__ })
typedef struct tex_id tex_id;
struct tex_id
{
  u16 index;
};

#define tex_rect(...) (make(tex_rect) { __VA_ARGS__ })
typedef struct tex_rect tex_rect;
struct tex_rect
{
  u16 min_x;
  u16 min_y;
  u16 max_x;
  u16 max_y;
};

#ifndef TEXTURE_BOARDER_SIZE
#define TEXTURE_BOARDER_SIZE (2)
#endif

typedef struct tex_entry tex_entry;
struct tex_entry
{
  b32 in_use;
  u32 hash;

  tex_rect rect;
  char name[64];
};

typedef struct tex_table tex_table;
struct tex_table
{
  u16 width;
  u16 height;
  u32* pixels;

  tex_entry array[TEXTURE_TABLE_SIZE];
};

typedef struct tex_image tex_image;
struct tex_image
{
  b32 user_provided;

  u16 width;
  u16 height;
  const u32* pixels;

  char name[256];
};

static tex_table texture_table;
static usize tex_image_count;
static tex_image tex_image_array[2048];

static tex_table* tex_get_table(void)
{
  return &texture_table;
}

static u32* tex_get_pixels(void)
{
  return texture_table.pixels;
}

static u16 tex_get_width(void)
{
  return texture_table.width;
}

static u16 tex_get_height(void)
{
  return texture_table.height;
}

static tex_rect tex_get_rect(tex_id id)
{
  return texture_table.array[id.index].rect;
}

static tex_id tex_get_id(const char* name)
{
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use)
  {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0))
    {
      tex_id id = { index };
      return id;
    }
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  puts(name);
  assert(0);

  return tex_id(0);
}

static tex_rect tex_get(const char* name)
{
  return tex_get_rect(tex_get_id(name));
}

static void cstr_copy_without_extension(char* out, const char* str)
{
  while (*str && *str != '.')
  {
    *(out++) = *(str++);
  }
  *out = '\0';
}

static void cstr_concat(char* out, const char* a, const char* b)
{
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static int tex_cmp_image(const void* va, const void* vb)
{
  const tex_image* a = (const tex_image*)va;
  const tex_image* b = (const tex_image*)vb;
  return b->width - a->width;
}

static void tex_add_image(const char* name, void* pixels, u16 width, u16 height)
{
  tex_image image = {0};

  image.user_provided = 1;
  image.width = width;
  image.height = height;
  image.pixels = pixels;

  strcpy_s(image.name, countof(image.name), name);

  tex_image_array[tex_image_count++] = image;
}

static void tex_load_dir(const char* dir_path)
{
  const char* ext[] = { "*.png", "*.jpg" };
  for (i32 i = 0; i < countof(ext); ++i)
  {
    for_iter(file_iter, it, file_iter_create(dir_path, ext[i]))
    {
      tex_image image = {0};
      image.pixels = file_load_image(it.current, &image.width, &image.height);
      cstr_copy_without_extension(image.name, it.data.cFileName);
      tex_image_array[tex_image_count++] = image;
    }
  }
}

static void tex_load_dir_and_scale(const char* dir_path, u16 denominator)
{
  const char* ext[] = { "*.png", "*.jpg" };
  assert(denominator);
  for (i32 i = 0; i < countof(ext); ++i)
  {
    for_iter(file_iter, it, file_iter_create(dir_path, ext[i]))
    {
      u16 width = 0;
      u16 height = 0;
      u32* pixels = file_load_image(it.current, &width, &height);
      tex_image image = ZERO;
      image.user_provided = 1;
      image.width = width / denominator;
      image.height = height / denominator;
      image.pixels = mem_array(u32, image.width * image.height);
      stbir_resize_uint8(
        (unsigned char*)pixels, width, height, 0,
        (unsigned char*)image.pixels, image.width, image.height, 0,
        4);
      cstr_copy_without_extension(image.name, it.data.cFileName);
      tex_image_array[tex_image_count++] = image;
      free(pixels);
    }
  }
}

static void tex_begin(u16 width, u16 height)
{
  tex_image_count = 0;

  texture_table = make(tex_table)
  {
    width,
    height, 
    mem_array(u32, (usize)(width * height)),
  };

  texture_table.array[0].in_use = 1;
}

static usize tex_stack_top; 
static tex_rect tex_stack_buf[4096];

static b32 rect_contains_image(tex_rect rect, u16 width, u16 height)
{
  u16 rect_width  = rect.max_x - rect.min_x;
  u16 rect_height = rect.max_y - rect.min_y;

  return width <= rect_width && height <= rect_height;
}

static tex_rect tex_get_fit(u16 width, u16 height)
{
  u32 j = 0;
  for (j = 0; j < tex_stack_top; ++j)
  {
    if (rect_contains_image(tex_stack_buf[j], width, height))
    {
      break;
    }
  }

  tex_rect rect = tex_stack_buf[j];
  tex_stack_buf[j] = tex_stack_buf[--tex_stack_top];

  return rect;
}

static void _tex_add_entry(const char* name, tex_rect rect)
{
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  while (texture_table.array[index].in_use)
  {
    if ((texture_table.array[index].hash == hash) && (strcmp(texture_table.array[index].name, name) == 0))
    {
      assert(0);
    }
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  tex_entry* entry = &texture_table.array[index];

  entry->in_use = 1;
  entry->rect = rect;
  entry->hash = hash;

  strcpy_s(entry->name, 64, name);
}

static inline u32 _tex_get_image_pixel(const tex_image* image, u16 x, u16 y)
{
  return image->pixels[y * image->width + x];
}

static inline void _tex_set_pixel(u16 x, u16 y, u32 color)
{
  texture_table.pixels[y * texture_table.width + x] = color;
}

static inline u32 _tex_get_pixel(u16 x, u16 y)
{
  return texture_table.pixels[y * texture_table.width + x];
}

static void tex_end(void)
{
  tex_stack_top = 0;
  tex_stack_buf[tex_stack_top++] = make(tex_rect)
  {
    0,
    0,
    texture_table.width,
    texture_table.height,
  };

  qsort(tex_image_array, tex_image_count, sizeof (tex_image), tex_cmp_image);

  for (usize i = 0; i < tex_image_count; ++i)
  {
    tex_image* image = &tex_image_array[i];
    tex_rect rect = tex_get_fit(image->width + 2, image->height + 2);

    u16 offset_x = rect.min_x + TEXTURE_BOARDER_SIZE;
    u16 offset_y = rect.min_y + TEXTURE_BOARDER_SIZE;
    u16 size_x   = image->width + 2 * TEXTURE_BOARDER_SIZE;
    u16 size_y   = image->height + 2 * TEXTURE_BOARDER_SIZE;

    tex_rect tex =
    {
      offset_x,
      offset_y,
      (u16)(offset_x + image->width),
      (u16)(offset_y + image->height),
    };

    _tex_add_entry(image->name, tex);

    for (u16 y = 0; y < image->height; ++y)
    {
      for (u16 x = 0; x < image->width; ++x)
      {
        u16 tx = offset_x + x;
        u16 ty = offset_y + y;
        _tex_set_pixel(tx, ty, _tex_get_image_pixel(image, x, y));
      }
    }

    for (u16 y = (tex.min_y - TEXTURE_BOARDER_SIZE); y < (tex.max_y + TEXTURE_BOARDER_SIZE); ++y)
    {
      for (u16 x = (tex.min_x - TEXTURE_BOARDER_SIZE); x < (tex.max_x + TEXTURE_BOARDER_SIZE); ++x)
      {
        _tex_set_pixel(x, y, _tex_get_pixel(
            clamp(x, tex.min_x, tex.max_x - 1),
            clamp(y, tex.min_y, tex.max_y - 1)));
      }
    }

    tex_rect a =
    {
      (u16)(rect.min_x + size_x),
      rect.min_y,
      rect.max_x,
      rect.max_y,
    };

    tex_rect b =
    {
      rect.min_x,
      (u16)(rect.min_y + size_y),
      (u16)(rect.min_x + size_x),
      rect.max_y,
    };

    if (a.min_x + size_x <= rect.max_x && a.min_y + size_y <= rect.max_y)
    {
      tex_stack_buf[tex_stack_top++] = a;
    }

    if (b.min_x + size_x <= rect.max_x && b.min_y + size_y <= rect.max_y)
    {
      tex_stack_buf[tex_stack_top++] = b;
    }

    if (!image->user_provided)
    {
      file_free_image(image->pixels);
    }
  }

  tex_image_count = 0;
  tex_stack_top = 0;
}

