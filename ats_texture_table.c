#pragma once

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ext/stb_image_resize.h" 

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ext/stb_image_write.h" 

#ifndef TEXTURE_BORDER_SIZE
#define TEXTURE_BORDER_SIZE (2)
#endif

#define TEXTURE_TABLE_SIZE (4096)

typedef struct tex_node
{
  struct tex_node* next;
  tex_rect rect;
  tex_rect fitted;
  char name[64];
} tex_node;

typedef struct
{
  u16 width;
  u16 height;
  u32* pixels;

  tex_node* array[TEXTURE_TABLE_SIZE];
} tex_table;

typedef struct
{
  b32 user_provided;

  u16 width;
  u16 height;
  const u32* pixels;

  char name[256];
} tex_image;

static tex_table texture_table;
static usize tex_image_count;
static tex_image tex_image_array[2048];

ATS_API u32* tex_get_pixels(void)
{
  return texture_table.pixels;
}

ATS_API u16 tex_get_width(void)
{
  return texture_table.width;
}

ATS_API u16 tex_get_height(void)
{
  return texture_table.height;
}

ATS_API tex_rect tex_get(const char* name)
{
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;
  for (tex_node* node = texture_table.array[index]; node; node = node->next)
  {
    if (strcmp(node->name, name) == 0)
    {
      return node->rect;
    }
  }
  assert(0);
  return tex_rect(0);
}

static int tex_cmp_image(const void* va, const void* vb)
{
  const tex_image* a = (const tex_image*)va;
  const tex_image* b = (const tex_image*)vb;
  return b->width - a->width;
}

static void tex__str_copy(char* s, usize count, const char* d)
{
  while (count-- && *d)
    *(s++) = *(d++);
}

ATS_API void tex_add_image(const char* name, void* pixels, u16 width, u16 height)
{
  tex_image image = {0};

  image.user_provided = 1;
  image.width = width;
  image.height = height;
  image.pixels = pixels;

  tex__str_copy(image.name, countof(image.name), name);

  tex_image_array[tex_image_count++] = image;
}

ATS_API void tex_load_dir(const char* path)
{
  dir_iter(path)
  {
    const char* ext = dir_extension();
    if (strcmp(ext, "png") != 0 && strcmp(ext, "jpg") != 0) continue;

    tex_image image = {0};
    image.pixels = file_load_image(dir_path(), &image.width, &image.height);
    tex__str_copy(image.name, countof(image.name), dir_name());
    tex_image_array[tex_image_count++] = image;
  }
}

ATS_API void tex_load_and_scale_dir(const char* path, u16 denominator)
{
  assert(denominator);
  dir_iter(path)
  {
    const char* ext = dir_extension();
    if (strcmp(ext, "png") != 0 && strcmp(ext, "jpg") != 0) continue;

    u16 width = 0;
    u16 height = 0;
    u32* pixels = file_load_image(dir_path(), &width, &height);

    tex_image image = {0};

    image.user_provided = 1;
    image.width = width / denominator;
    image.height = height / denominator;
    image.pixels = mem_array(u32, image.width * image.height);

    stbir_resize_uint8(
      (unsigned char*)pixels, width, height, 0,
      (unsigned char*)image.pixels, image.width, image.height, 0,
      4);

    tex__str_copy(image.name, countof(image.name), dir_name());
    tex_image_array[tex_image_count++] = image;

    free(pixels);
  }
}

ATS_API void tex_begin(u16 width, u16 height)
{
  tex_image_count = 0;
  texture_table = (tex_table)
  {
    width,
    height, 
    mem_array(u32, (usize)(width * height)),
  };
}

static usize tex_stack_top; 
static tex_rect tex_stack_buf[4096];

static b32 _rect_contains_image(tex_rect rect, u16 width, u16 height)
{
  u16 rect_width  = rect.max_x - rect.min_x;
  u16 rect_height = rect.max_y - rect.min_y;

  return width <= rect_width && height <= rect_height;
}

static tex_rect _tex_get_fit(u16 width, u16 height)
{
  u32 j = 0;
  for (j = 0; j < tex_stack_top; ++j)
  {
    if (_rect_contains_image(tex_stack_buf[j], width, height))
    {
      break;
    }
  }

  tex_rect rect = tex_stack_buf[j];
  tex_stack_buf[j] = tex_stack_buf[--tex_stack_top];

  return rect;
}

static void _tex_add_entry(const char* name, tex_rect rect, tex_rect fitted)
{
  u32 hash  = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  tex_node* node = mem_type(tex_node);

  node->next    = texture_table.array[index];
  node->rect    = rect;
  node->fitted  = fitted;

  tex__str_copy(node->name, 64, name);

  texture_table.array[index] = node;
}

static u32 _tex_get_image_pixel(const tex_image* image, u16 x, u16 y)
{
  return image->pixels[y * image->width + x];
}

static void _tex_set_pixel(u16 x, u16 y, u32 color)
{
  texture_table.pixels[y * texture_table.width + x] = color;
}

static u32 _tex_get_pixel(u16 x, u16 y)
{
  return texture_table.pixels[y * texture_table.width + x];
}

ATS_API void tex_end(void)
{
  tex_stack_top = 0;
  tex_stack_buf[tex_stack_top++] = (tex_rect)
  {
    .min_x = 0,
    .min_y = 0,
    .max_x = texture_table.width,
    .max_y = texture_table.height,
  };

  qsort(tex_image_array, tex_image_count, sizeof (tex_image), tex_cmp_image);

  for (usize i = 0; i < tex_image_count; ++i)
  {
    tex_image* image = &tex_image_array[i];
    tex_rect rect = _tex_get_fit(image->width + 2, image->height + 2);

    u16 offset_x = rect.min_x + TEXTURE_BORDER_SIZE;
    u16 offset_y = rect.min_y + TEXTURE_BORDER_SIZE;
    u16 size_x   = image->width + 2 * TEXTURE_BORDER_SIZE;
    u16 size_y   = image->height + 2 * TEXTURE_BORDER_SIZE;

    tex_rect tex =
    {
      .min_x = offset_x,
      .min_y = offset_y,
      .max_x = (u16)(offset_x + image->width),
      .max_y = (u16)(offset_y + image->height),
    };

    tex_rect fitted = {
      .min_x = 0xffff,
      .min_y = 0xffff,
      .max_x = 0,
      .max_y = 0,
    };

    for (u16 y = 0; y < image->height; ++y)
    {
      for (u16 x = 0; x < image->width; ++x)
      {
        u16 tx = offset_x + x;
        u16 ty = offset_y + y;
        u32 pixel = _tex_get_image_pixel(image, x, y);
        _tex_set_pixel(tx, ty, pixel);
        if (pixel)
        {
          fitted.min_x = min(fitted.min_x, x);
          fitted.min_y = min(fitted.min_y, y);
          fitted.max_x = max(fitted.max_x, x);
          fitted.max_y = max(fitted.max_y, y);
        }
      }
    }

    fitted.min_x += offset_x;
    fitted.min_y += offset_y;
    fitted.max_x += offset_x;
    fitted.max_y += offset_y;

    _tex_add_entry(image->name, tex, fitted);

    for (u16 y = (tex.min_y - TEXTURE_BORDER_SIZE); y < (tex.max_y + TEXTURE_BORDER_SIZE); ++y)
    {
      for (u16 x = (tex.min_x - TEXTURE_BORDER_SIZE); x < (tex.max_x + TEXTURE_BORDER_SIZE); ++x)
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

ATS_API void tex_save(const char* name)
{
  char image_filename[256] = {0};
  char header_filename[256] = {0};

  sprintf(image_filename, "%s.png", name);
  sprintf(header_filename, "%s.h", name);

  stbi_write_png(image_filename, texture_table.width, texture_table.height, 4, texture_table.pixels, 0);

  char* file_content = mem_temp();
  char* it = file_content;

#define emit(...) it += sprintf(it, __VA_ARGS__)
  emit("// GENERATED FILE\n");
  emit("#pragma once\n\n");
  emit("typedef enum frame_tag\n{\n");
  for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
  {
    for (tex_node* node = texture_table.array[i]; node; node = node->next)
    {
      emit("  FT_%s,\n", node->name);
    }
  }
  emit("  FT_count,\n");
  emit("} frame_tag_t;\n\n");
  emit("typedef struct frame_info_t\n{\n");
  emit("  frame_tag_t next;\n");
  emit("  tex_rect rect;\n");
  emit("  tex_rect fitted;\n");
  emit("} frame_info_t;\n\n");
  emit("static frame_info_t frame_info_table[FT_count] = \n{\n");
  for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
  {
    for (tex_node* node = texture_table.array[i]; node; node = node->next)
    {
      tex_rect rect   = node->rect;
      tex_rect fitted = node->fitted;

      emit("  [FT_%s] = {\n", node->name),
      emit("    .next   = FT_%s,\n", node->name);
      emit("    .rect   = { %d, %d, %d, %d },\n", rect.min_x, rect.min_y, rect.max_x, rect.max_y);
      emit("    .fitted = { %d, %d, %d, %d },\n", fitted.min_x, fitted.min_y, fitted.max_x, fitted.max_y);
      emit("  },\n");
    }
  }
  emit("};\n\n");
#undef emit

  file_write_bin(header_filename, file_content, it - file_content);
}

