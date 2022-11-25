#pragma once

extern void gl_init(void);

extern void gl_set_simple_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_simple_light_directed(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_light_emitter(int index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic);
extern void gl_set_light_directed(int index, v3 pos, v3 color);
extern void gl_set_light_global_ambient(f32 r, f32 g, f32 b);

extern void gl_init_bitmap_font(void);
extern void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color);
extern void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...);

extern void gl_begin(u32 type);
extern void gl_end(void);
extern void gl_color(u32 color);
extern void gl_normal(f32 x, f32 y, f32 z);
extern void gl_uv(f32 x, f32 y);
extern void gl_vertex(f32 x, f32 y, f32 z);
extern void gl_set_matrix(m4 projection, m4 view);
extern void gl_billboard(r2i tex_rect, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up);
extern void gl_texture_box(r2i tex_rect, r3 box, u32 color);
extern void gl_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color);
extern void gl_texture_rect_flip(r2i tex_rect, r2 rect, f32 z, u32 color, bool flip_x, bool flip_y);
extern void gl_box(r3 box, u32 color);
extern void gl_rect(r2 rect, f32 z, u32 color);

typedef struct gl_texture {
  u32 id;
  i32 width;
  i32 height;
} gl_texture_t;

extern gl_texture_t gl_texture_create(void *pixels, int width, int height, int is_smooth);
extern gl_texture_t gl_texture_create_from_image(image_t image, int is_smooth);
extern gl_texture_t gl_texture_load_from_file(const char *texture_path, int is_smooth);

extern void gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth);

extern void gl_texture_bind(const gl_texture_t* texture);
extern void gl_texture_destroy(gl_texture_t* texture);

typedef struct gl_shader {
  u32 id;
} gl_shader_t;

typedef struct gl_shader_desc {
  const char* vs;
  const char* fs;
} gl_shader_desc_t;

extern gl_shader_t gl_shader_create(gl_shader_desc_t desc);
extern gl_shader_t gl_shader_load_from_file(const char *vs, const char *fs,  mem_arena_t* ma);

extern void gl_use(const gl_shader_t* shader);
extern u32 gl_location(const gl_shader_t* shader, const char* name);

extern void gl_uniform_i32(u32 location, int u);
extern void gl_uniform_f32(u32 location, f32 u);
extern void gl_uniform_v2(u32 location, v2 u);
extern void gl_uniform_v3(u32 location, v3 u);
extern void gl_uniform_v4(u32 location, v4 u);
extern void gl_uniform_m2(u32 location, m2 m);
extern void gl_uniform_m3(u32 location, m3 m);
extern void gl_uniform_m4(u32 location, m4 m);

extern v3 gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview);

typedef struct gl_buffer {
  u32 vao;
  u32 vbo;
} gl_buffer_t;

typedef struct gl_layout {
  u32 size;
  u32 type;
  u32 stride;
  u32 offset;
  b32 normalize;
} gl_layout_t;

typedef struct gl_buffer_desc {
  gl_layout_t layout[32];
} gl_buffer_desc_t;

extern gl_buffer_t gl_buffer_create(const gl_buffer_desc_t* desc);
extern void gl_buffer_bind(const gl_buffer_t* buffer);
extern void gl_buffer_send(const gl_buffer_t* array, const void* data, u32 size);

