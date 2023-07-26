#pragma once

#include "ats_base.h"
#include "ats_image.h"

#define TEXTURE_TABLE_SIZE (1024)

typedef struct
{
   u16 index;
} tt_id;

typedef struct
{
   b32 in_use;
   u32 hash;
   r2i rect;
   char name[64];
} tt_entry;

typedef struct
{
   image img;
   tt_entry array[TEXTURE_TABLE_SIZE];
} texture_table;

extern void tt_begin(int width, int height);
extern void tt_end(void);
extern void tt_add_image(const char* name, image img);
extern void tt_load_from_dir(const char* dir_path);

extern texture_table* tt_get_texture_table(void);

extern tt_id tt_get_id(const char* name);
extern r2i tt_get_rect(tt_id id);
extern r2i tt_get(const char* name);
extern image tt_get_image(void);

