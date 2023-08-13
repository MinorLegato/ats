#pragma once

#include "ats_base.h"

#define TEXTURE_TABLE_SIZE (1024)

typedef struct
{
   u16 index;
} tex_id;

typedef struct
{
   u16 min_x;
   u16 min_y;
   u16 max_x;
   u16 max_y;
} tex_rect;

extern void tex_begin(u16 width, u16 height);
extern void tex_end(void);

extern void tex_add_image(const char* name, const u32* pixels, u16 width, u16 height);
extern void tex_load_dir(const char* dir_path);

extern const u32* tex_get_pixels(void);
extern u16        tex_get_width(void);
extern u16        tex_get_height(void);
extern tex_id     tex_get_id(const char* name);
extern tex_rect   tex_get_rect(tex_id id);
extern tex_rect   tex_get(const char* name);

