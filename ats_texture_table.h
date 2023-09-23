#pragma once

#define TEXTURE_TABLE_SIZE (1024)

typedef struct {
  unsigned short index;
} tex_id;

typedef struct {
  unsigned short min_x;
  unsigned short min_y;
  unsigned short max_x;
  unsigned short max_y;
} tex_rect;

void tex_begin(unsigned short width, unsigned short height);
void tex_end(void);

void tex_add_image(const char* name, const unsigned* pixels, unsigned short width, unsigned short height);
void tex_load_dir(const char* dir_path);

const unsigned* tex_get_pixels(void);
unsigned short  tex_get_width(void);
unsigned short  tex_get_height(void);
tex_id          tex_get_id(const char* name);
tex_rect        tex_get_rect(tex_id id);
tex_rect        tex_get(const char* name);

