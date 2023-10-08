#pragma once

#include "ats_base.h"

#define TEXTURE_TABLE_SIZE (1024)

struct tex_id
{
  u16 index;
};

struct tex_rect
{
  u16 min_x;
  u16 min_y;
  u16 max_x;
  u16 max_y;
};

void tex_begin(u16 width, u16 height);
void tex_end(void);
void tex_add_image(const char* name, const u32* pixels, u16 width, u16 height);
void tex_load_dir(const char* dir_path);
const u32* tex_get_pixels(void);
u16 tex_get_width(void);
u16 tex_get_height(void);
struct tex_id tex_get_id(const char* name);
struct tex_rect tex_get_rect(struct tex_id id);
struct tex_rect tex_get(const char* name);

