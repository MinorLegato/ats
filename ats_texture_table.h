#pragma once

#include "ats_base.h"

#define TEXTURE_TABLE_SIZE (1024)

typedef struct {
  u16 index;
} tex_id;

#define tex_rect(...) ((tex_rect) { __VA_ARGS__ })
typedef struct {
  u16 min_x;
  u16 min_y;
  u16 max_x;
  u16 max_y;
} tex_rect;

static void tex_begin(u16 width, u16 height);
static void tex_end(void);
static void tex_add_image(const char* name, u32* pixels, u16 width, u16 height);
static void tex_load_dir(const char* dir_path);
static u32* tex_get_pixels(void);
static u16 tex_get_width(void);
static u16 tex_get_height(void);
static tex_id tex_get_id(const char* name);
static tex_rect tex_get_rect(tex_id id);
static tex_rect tex_get(const char* name);

