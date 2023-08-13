#pragma once

#include "ats_base.h"
//#include "ats_image.h"

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

typedef struct
{
   b32      in_use;
   u32      hash;
   tex_rect rect;
   char     name[64];
} tex_entry;

typedef struct
{
   u16   width;
   u16   height;
   u32*  pixels;

   tex_entry array[TEXTURE_TABLE_SIZE];
} tex_table;

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

