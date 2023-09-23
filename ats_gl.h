#pragma once

void gl_init(void);

void gl_set_simple_light_emitter(int index, float bright, float x, float y, float z);
void gl_set_simple_light_directed(int index, float bright, float x, float y, float z);
void gl_set_light_emitter(int index, v3 p, v3 color, float constant, float linear, float quadratic);
void gl_set_light_directed(int index, v3 pos, v3 color);
void gl_set_light_global_ambient(float r, float g, float b);

void gl_init_bitmap_font(void);
void gl_string(const char *str, float x, float y, float z, float sx, float sy, unsigned color);
void gl_string_format(float x, float y, float z, float sx, float sy, unsigned color, const char* fmt, ...);

void gl_begin(unsigned type);
void gl_end(void);
void gl_color(unsigned color);
void gl_normal(float x, float y, float z);
void gl_uv(float x, float y);
void gl_vertex(float x, float y, float z);
void gl_set_matrix(m4 projection, m4 view);
void gl_billboard(tex_rect tex, v3 pos, v2 rad, v3 normal, unsigned color, v3 right, v3 up);
void gl_texture_box(tex_rect tex, r3 box, unsigned color);
void gl_texture_rect(tex_rect tex, r2 rect, float z, unsigned color);
void gl_texture_rect_flip(tex_rect tex, r2 rect, float z, unsigned color, int flip_x, int flip_y);
void gl_box(r3 box, unsigned color);
void gl_rect(r2 rect, float z, unsigned color);

typedef struct {
  unsigned id;

  unsigned short width;
  unsigned short height;
} gl_texture;

gl_texture gl_texture_create(const void *pixels, int width, int height, int is_smooth);
gl_texture gl_texture_load_from_file(const char *texture_path, int is_smooth);

void gl_texture_update(gl_texture* texture, const void *pixels, int width, int height, int is_smooth);

void gl_texture_bind(const gl_texture* texture);
void gl_texture_destroy(gl_texture* texture);

typedef struct {
  unsigned id;
} gl_shader;

typedef struct {
  const char* vs;
  const char* fs;
} gl_shader_desc;

gl_shader gl_shader_create(gl_shader_desc desc);
gl_shader gl_shader_load_from_file(const char *vs, const char *fs);

void gl_use(const gl_shader* shader);
unsigned gl_location(const gl_shader* shader, const char* name);

void gl_uniform_i32(unsigned location, int u);
void gl_uniform_float(unsigned location, float u);
void gl_uniform_v2(unsigned location, v2 u);
void gl_uniform_v3(unsigned location, v3 u);
void gl_uniform_v4(unsigned location, v4 u);
void gl_uniform_m2(unsigned location, m2 m);
void gl_uniform_m3(unsigned location, m3 m);
void gl_uniform_m4(unsigned location, m4 m);

v3 gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview);

typedef struct {
  unsigned vao;
  unsigned vbo;
} gl_buffer;

typedef struct {
  unsigned size;
  unsigned type;
  unsigned stride;
  unsigned offset;
  unsigned normalize;
} gl_layout;

typedef struct {
  gl_layout layout[32];
} gl_buffer_desc;

gl_buffer gl_buffer_create(const gl_buffer_desc* desc);
void gl_buffer_bind(const gl_buffer* buffer);
void gl_buffer_send(const gl_buffer* array, const void* data, unsigned size);

