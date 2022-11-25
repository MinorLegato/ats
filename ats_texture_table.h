#pragma once

#define TEXTURE_TABLE_SIZE (1024)

typedef struct tt_id {
  u16 index;
} tt_id_t;

typedef struct tt_entry {
  b32 in_use;
  u32 hash;
  r2i rect;
  char name[64];
} tt_entry_t;

typedef struct texture_table {
  image_t img;
  tt_entry_t array[TEXTURE_TABLE_SIZE];
} texture_table_t;

extern void tt_begin(int width, int height, mem_arena_t* ma);
extern void tt_end(void);
extern void tt_add_image(const char* name, image_t img);
extern void tt_load_from_dir(const char* dir_path);

extern texture_table_t* tt_get_texture_table(void);

extern tt_id_t tt_get_id(const char* name);
extern r2i tt_get_rect(tt_id_t id);
extern r2i tt_get(const char* name);
extern image_t tt_get_image(void);


