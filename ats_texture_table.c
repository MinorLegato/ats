#pragma once

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "ext/stb_image_resize.h" 

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ext/stb_image_write.h" 

#ifndef TEXTURE_BORDER_SIZE
#define TEXTURE_BORDER_SIZE (2)
#endif

#define TEXTURE_TABLE_LOG2  (12)
#define TEXTURE_TABLE_SIZE  (1 << TEXTURE_TABLE_LOG2)
#define TEXTURE_TABLE_MOD   (TEXTURE_TABLE_SIZE - 1)

struct tex_frame
{
  struct tex_frame* next;

  tex_rect rect;
  tex_rect fitted;

  char name[64];
};

struct tex_animation
{
  struct tex_animation* next;
  struct tex_frame* frame;

  char name[64];
};

struct tex_entity
{
  struct tex_entity* next;
  struct tex_animation* animation;

  char name[64];
};

typedef struct
{
  b32 user_provided;

  u16 width;
  u16 height;

  const u32* pixels;

  char entity[64];
  char animation[64];
  char frame[64];
} tex_image;

static struct
{
  u16 width;
  u16 height;
  u32* pixels;

  struct tex_entity* entity[TEXTURE_TABLE_SIZE];

  struct
  {
    u32 count;
    tex_image array[2048];
  } image;

  struct
  {
    u32 count; 
    tex_rect array[4096];
  } stack;
} tex;

static int tex_cmp_image(const void* va, const void* vb)
{
  const tex_image* a = (const tex_image*)va;
  const tex_image* b = (const tex_image*)vb;
  return b->width - a->width;
}

static void tex__str_copy(char* d, usize count, const char* s)
{
  while (count-- && *s)
    *(d++) = *(s++);
  *d = '\0';
}

static tex_image* tex__new_image(const char* name, void* pixels, u16 width, u16 height)
{
  tex_image* image = arr_new(&tex.image);

  image->width  = width;
  image->height = height;
  image->pixels = pixels;

  u32 count = 0;
  u32 index = 0;
  char array[8][64] = {0};

  while (*name && (*name != '.'))
  {
    array[count][index++] = *(name++);
    if (*name == '/' || *name == '\\')
    {
      name++;
      count++;
      index = 0;
    }
  }

  tex__str_copy(image->frame, countof(image->frame), array[count]);

  switch (count)
  {
    case 0:
    {
      tex__str_copy(image->entity, countof(image->frame), array[count]);
      tex__str_copy(image->animation, countof(image->frame), "idle");
    } break;
    case 1:
    {
      tex__str_copy(image->entity, countof(image->frame), array[0]);
      tex__str_copy(image->animation, countof(image->frame), "idle");
    } break;
    case 2:
    {
      tex__str_copy(image->entity, countof(image->frame), array[0]);
      tex__str_copy(image->animation, countof(image->frame), array[1]);
    } break;
    default: assert(0);
  }

  return image;
}

static b32 _rect_contains_image(tex_rect rect, u16 width, u16 height)
{
  u16 rect_width  = rect.max_x - rect.min_x;
  u16 rect_height = rect.max_y - rect.min_y;

  return width <= rect_width && height <= rect_height;
}

static tex_rect tex__get_fit(u16 width, u16 height)
{
  u32 j = 0;
  for (j = 0; j < tex.stack.count; ++j)
  {
    if (_rect_contains_image(tex.stack.array[j], width, height))
      break;
  }
  tex_rect rect = tex.stack.array[j];
  tex.stack.array[j] = tex.stack.array[--tex.stack.count];
  return rect;
}

static struct tex_entity* tex__get_entity(const char* name)
{
  u32 hash = hash_str(name);
  u32 index = hash & TEXTURE_TABLE_MOD;
  for (struct tex_entity* e = tex.entity[index]; e; e = e->next)
  {
    if (strcmp(e->name, name) == 0)
    {
      return e;
    }
  }
  struct tex_entity* e = mem_type(struct tex_entity);
  e->next = tex.entity[index];
  tex__str_copy(e->name, countof(e->name), name);
  tex.entity[index] = e;
  return e;
}

static struct tex_animation* tex__get_animation(struct tex_entity* e, const char* name)
{
  struct tex_animation* prev = 0;
  struct tex_animation* node = 0;
  for (node = e->animation; node; node = node->next)
  {
    int cmp = strcmp(node->name, name);
    if (cmp == 0) return node;
    if (cmp > 0)
      break;
    prev = node;
  }
  struct tex_animation* anim = mem_type(struct tex_animation);
  anim->next = node;
  tex__str_copy(anim->name, countof(anim->name), name);
  if (prev)
  {
    prev->next = anim;
  }
  else
  {
    e->animation = anim;
  }
  return anim;
}

static struct tex_frame* tex__get_frame(struct tex_animation* a, const char* name)
{
  struct tex_frame* prev = 0;
  struct tex_frame* node = 0;
  for (node = a->frame; node; node = node->next)
  {
    int cmp = strcmp(node->name, name);
    if (cmp == 0) return node;
    if (cmp > 0) {
      break;
    }
    prev = node;
  }
  struct tex_frame* frame = mem_type(struct tex_frame);
  frame->next = node;
  tex__str_copy(frame->name, countof(frame->name), name);
  if (prev)
  {
    prev->next = frame;
  }
  else
  {
    a->frame = frame;
  }
  return frame;
}

static void tex__add_frame(tex_image* image, tex_rect rect, tex_rect fitted)
{
  u32 hash = hash_str(image->frame);
  u16 index = hash & TEXTURE_TABLE_MOD;

  struct tex_entity* entity = tex__get_entity(image->entity);
  struct tex_animation* animation = tex__get_animation(entity, image->animation);
  struct tex_frame* frame = tex__get_frame(animation, image->frame);

  frame->rect = rect;
  frame->fitted = fitted;
  tex__str_copy(frame->name, 64, image->frame);
}

static u32 tex__get_image_pixel(const tex_image* image, u16 x, u16 y)
{
  return image->pixels[y * image->width + x];
}

static void tex__set_pixel(u16 x, u16 y, u32 color)
{
  tex.pixels[y * tex.width + x] = color;
}

static u32 tex__get_pixel(u16 x, u16 y)
{
  return tex.pixels[y * tex.width + x];
}

ATS_API u32* tex_get_pixels(void)
{
  return tex.pixels;
}

ATS_API u16 tex_get_width(void)
{
  return tex.width;
}

ATS_API u16 tex_get_height(void)
{
  return tex.height;
}

ATS_API tex_rect tex_get(const char* name)
{
#if 0
  tex_frame* frame = tex__get_frame(name);
  return frame->rect;
#endif
  return tex_rect(0);
}

ATS_API void tex_add_image(const char* name, void* pixels, u16 width, u16 height)
{
  tex_image* image = tex__new_image(name, pixels, width, height);
  image->user_provided = 1;
}

ATS_API void tex_load_dir(const char* texture_path)
{
  dir_iter(texture_path) {
    const char* ext = dir_extension();

    if (strcmp(ext, "png") != 0 && strcmp(ext, "jpg") != 0) continue;

    u16 width = 0;
    u16 height = 0;
    u32* pixels = file_load_image(dir_path(), &width, &height);

    char* path = dir_path() + strlen(texture_path);
    while (*path == '/' || *path == '\\') path++;

    tex__new_image(path, pixels, width, height);
  }
}

#if 0
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

    arr_push(&tex.image, image);

    file_free_image(pixels);
  }
}
#endif

ATS_API void tex_begin(u16 width, u16 height)
{
  memset(&tex, 0, sizeof (tex));

  tex.width  = width;
  tex.height = height;
  tex.pixels = mem_array(u32, (usize)(width * height));
}

ATS_API void tex_end(void)
{
  tex.stack.count = 0;
  tex.stack.array[tex.stack.count++] = (tex_rect) {
    .min_x = 0,
    .min_y = 0,
    .max_x = tex.width,
    .max_y = tex.height,
  };

  arr_sort(&tex.image, tex_cmp_image);

  for_arr(tex_image, image, &tex.image) {
    tex_rect rect = tex__get_fit(image->width + 2, image->height + 2);

    u16 offset_x = rect.min_x + TEXTURE_BORDER_SIZE;
    u16 offset_y = rect.min_y + TEXTURE_BORDER_SIZE;
    u16 size_x   = image->width + 2 * TEXTURE_BORDER_SIZE;
    u16 size_y   = image->height + 2 * TEXTURE_BORDER_SIZE;

    tex_rect full =
    {
      .min_x = offset_x,
      .min_y = offset_y,
      .max_x = (u16)(offset_x + image->width),
      .max_y = (u16)(offset_y + image->height),
    };

    tex_rect fitted =
    {
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
        u32 pixel = tex__get_image_pixel(image, x, y);
        tex__set_pixel(tx, ty, pixel);
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

    tex__add_frame(image, full, fitted);

    for (u16 y = (full.min_y - TEXTURE_BORDER_SIZE); y < (full.max_y + TEXTURE_BORDER_SIZE); ++y)
    {
      for (u16 x = (full.min_x - TEXTURE_BORDER_SIZE); x < (full.max_x + TEXTURE_BORDER_SIZE); ++x)
      {
        u32 pixel = tex__get_pixel(clamp(x, full.min_x, full.max_x - 1), clamp(y, full.min_y, full.max_y - 1));
        tex__set_pixel(x, y, pixel);
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
      tex.stack.array[tex.stack.count++] = a;

    if (b.min_x + size_x <= rect.max_x && b.min_y + size_y <= rect.max_y)
      tex.stack.array[tex.stack.count++] = b;

    if (!image->user_provided)
      file_free_image(image->pixels);
  }

  tex.image.count = 0;
  tex.stack.count = 0;
}

ATS_API void tex_save(const char* name)
{
  char image_filename[256] = {0};
  char header_filename[256] = {0};

  sprintf(image_filename, "%s.png", name);
  sprintf(header_filename, "%s.h", name);

  stbi_write_png(image_filename, tex.width, tex.height, 4, tex.pixels, 0);

  char* file_content = mem_temp();
  char* it = file_content;

#define emit(...) it += sprintf(it, __VA_ARGS__)
  emit("// GENERATED FILE\n");
  emit("#pragma once\n\n");

  // enum entity:
  {
    emit("typedef enum {\n");
    for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
    {
      for (struct tex_entity* node = tex.entity[i]; node; node = node->next)
        emit("  TEX_ENTITY_%s,\n", node->name);
    }
    emit("  TEX_ENTITY_count,\n");
    emit("} tex_entity;\n\n");
  }
  
  // enum animation:
  {
    u32 used_count = 0;
    char* used_array[TEXTURE_TABLE_SIZE];

    emit("typedef enum {\n");
    for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
    {
      for (struct tex_entity* entity = tex.entity[i]; entity; entity = entity->next)
      {
        for (struct tex_animation* animation = entity->animation; animation; animation = animation->next)
        {
          u32 emit_enum = 1;
          for (u32 j = 0; j < used_count; ++j)
          {
            if (strcmp(used_array[j], animation->name) == 0)
            {
              emit_enum = 0;
              break;
            }
          }
          if (emit_enum)
          {
            emit("  TEX_ANIMATION_%s,\n", animation->name);
            used_array[used_count++] = animation->name;
          }
        }
      }
    }
    emit("  TEX_ANIMATION_count,\n");
    emit("} tex_animation;\n\n");
  }

  // enum frame:
  {
    emit("typedef enum {\n");
    for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
    {
      for (struct tex_entity* entity = tex.entity[i]; entity; entity = entity->next)
      {
        for (struct tex_animation* animation = entity->animation; animation; animation = animation->next)
        {
          for (struct tex_frame* frame = animation->frame; frame; frame = frame->next)
          {
            emit("  TEX_FRAME_%s,\n", frame->name);
          }
        }
      }
    }
    emit("  TEX_FRAME_count,\n");
    emit("} tex_frame;\n\n");
  }

  // entity -> animation lookup table:
  {
    emit("static tex_frame tex_animation_table[TEX_ENTITY_count][TEX_ANIMATION_count] = {\n");
    for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
    {
      for (struct tex_entity* entity = tex.entity[i]; entity; entity = entity->next)
      {
        emit("  [TEX_ENTITY_%s] = {\n", entity->name);
        for (struct tex_animation* animation = entity->animation; animation; animation = animation->next)
        {
          struct tex_frame* frame = animation->frame;
          emit("    [TEX_ANIMATION_%s] = TEX_FRAME_%s,\n", animation->name, frame->name);
        }
        emit("  },\n");
      }
    }
    emit("};\n\n");
  }

  // frame lookup table:
  {
    emit("typedef struct tex_info {\n");
    emit("  tex_frame next;\n");
    emit("  tex_animation animation;\n");
    emit("  tex_rect rect;\n");
    emit("  tex_rect fitted;\n");
    emit("} tex_info;\n\n");
    emit("static tex_info tex_info_table[TEX_FRAME_count] = {\n");

    for (u32 i = 0; i < TEXTURE_TABLE_SIZE; ++i)
    {
      for (struct tex_entity* entity = tex.entity[i]; entity; entity = entity->next)
      {
        for (struct tex_animation* animation = entity->animation; animation; animation = animation->next)
        {
          for (struct tex_frame* frame = animation->frame; frame; frame = frame->next)
          {
            tex_rect rect = frame->rect;
            tex_rect fitted = frame->fitted;
            struct tex_frame* next = frame->next? frame->next : animation->frame;

            emit("  [TEX_FRAME_%s] = {\n", frame->name),
            emit("    .next       = TEX_FRAME_%s,\n", next->name);
            emit("    .animation  = TEX_ANIMATION_%s,\n", animation->name);
            emit("    .rect       = { %d, %d, %d, %d },\n", rect.min_x, rect.min_y, rect.max_x, rect.max_y);
            emit("    .fitted     = { %d, %d, %d, %d },\n", fitted.min_x, fitted.min_y, fitted.max_x, fitted.max_y);
            emit("  },\n");
          }
        }
      }
    }
    emit("};\n\n");
  }
#undef emit

  file_write_bin(header_filename, file_content, it - file_content);
}

