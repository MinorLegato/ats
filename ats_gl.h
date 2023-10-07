#pragma once

#include "ats_base.h"

void gl_init(void);

void gl_set_simple_light_emitter(i32 index, f32 bright, f32 x, f32 y, f32 z);
void gl_set_simple_light_directed(i32 index, f32 bright, f32 x, f32 y, f32 z);
void gl_set_light_emitter(i32 index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic);
void gl_set_light_directed(i32 index, v3 pos, v3 color);
void gl_set_light_global_ambient(f32 r, f32 g, f32 b);

void gl_init_bitmap_font(void);
void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color);
void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...);

void gl_begin(u32 type);
void gl_end(void);
void gl_color(u32 color);
void gl_normal(f32 x, f32 y, f32 z);
void gl_uv(f32 x, f32 y);
void gl_vertex(f32 x, f32 y, f32 z);
void gl_set_matrix(m4 projection, m4 view);
void gl_billboard(struct tex_rect tex, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up);
void gl_texture_box(struct tex_rect tex, r3 box, u32 color);
void gl_texture_rect(struct tex_rect tex, r2 rect, f32 z, u32 color);
void gl_texture_rect_flip(struct tex_rect tex, r2 rect, f32 z, u32 color, b8 flip_x, b8 flip_y);
void gl_box(r3 box, u32 color);
void gl_rect(r2 rect, f32 z, u32 color);

struct gl_texture {
  u32 id;

  u16 width;
  u16 height;
};

struct gl_texture gl_texture_create(const void *pixels, u16 width, u16 height, b8 is_smooth);
struct gl_texture gl_texture_load_from_file(const char *texture_path, b8 is_smooth);

void gl_texture_update(struct gl_texture* texture, const void *pixels, u16 width, u16 height, b8 is_smooth);

void gl_texture_bind(const struct gl_texture* texture);
void gl_texture_destroy(struct gl_texture* texture);

struct gl_shader {
  u32 id;
};

struct gl_shader_desc {
  const char* vs;
  const char* fs;
};

struct gl_shader gl_shader_create(struct gl_shader_desc desc);
struct gl_shader gl_shader_load_from_file(const char *vs, const char *fs);

void gl_use(const struct gl_shader* shader);
u32 gl_location(const struct gl_shader* shader, const char* name);

void gl_uniform_i32(u32 location, i32 u);
void gl_uniform_f32(u32 location, f32 u);
void gl_uniform_v2(u32 location, v2 u);
void gl_uniform_v3(u32 location, v3 u);
void gl_uniform_v4(u32 location, v4 u);
void gl_uniform_m2(u32 location, m2 m);
void gl_uniform_m3(u32 location, m3 m);
void gl_uniform_m4(u32 location, m4 m);

v3 gl_get_world_position(i32 x, i32 y, m4 in_projection, m4 in_modelview);

struct gl_buffer {
  u32 vao;
  u32 vbo;
};

struct gl_layout {
  u32 size;
  u32 type;
  u32 stride;
  u32 offset;
  u32 normalize;
};

struct gl_buffer_desc {
  struct gl_layout layout[32];
};

struct gl_buffer gl_buffer_create(const struct gl_buffer_desc* desc);
void gl_buffer_bind(const struct gl_buffer* buffer);
void gl_buffer_send(const struct gl_buffer* array, const void* data, u32 size);

