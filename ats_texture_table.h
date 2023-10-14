#pragma once

#include "ats_base.h"

#define TEXTURE_TABLE_SIZE (1024)

typedef struct {
  u16 index;
} tex_id;

typedef struct {
  u16 min_x;
  u16 min_y;
  u16 max_x;
  u16 max_y;
} tex_rect;

void tex_begin(u16 width, u16 height);
void tex_end(void);
void tex_add_image(const char* name, u32* pixels, u16 width, u16 height);
void tex_load_dir(const char* dir_path);
u32* tex_get_pixels(void);
u16 tex_get_width(void);
u16 tex_get_height(void);
tex_id tex_get_id(const char* name);
tex_rect tex_get_rect(tex_id id);
tex_rect tex_get(const char* name);

