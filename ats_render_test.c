#pragma once

#if 0 // example

#define ATS_OGL33
#include "../ats/ats.h"
#include "../ats/ats_platform.h"

#include "../ats/ats_file.c"
#include "../ats/ats_memory.c"

#include "../ats/ats_platform.c"
#include "../ats/ats_gl33.c"

#include "render.c"

int
main(void) {
  platform_init("YOLO", 1600, 800, 4);
  r_init();

  //u32 game_pass = r_new_target();
  //u32 lighting_pass = r_new_target();

  while (!platform.close) {
    if (platform.keyboard.pressed[KEY_ESCAPE]) {
      platform.close = true;
    }

    m4 projection = m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32);
    m4 view = m4_look_at(v3(0, 0, 8), v3(0, 0, 0), v3(0, 1, 0));
    m4 mvp = m4_mul(projection, view);

    r_begin_frame();
    r_clear(0xff000000);
    r_set_matrix(mvp);

    r_begin(GL_TRIANGLES);
    r_color(0xffffff00); r_vertex( 0,  1, 0);
    r_color(0xffff00ff); r_vertex(-1, -1, 0);
    r_color(0xff00ffff); r_vertex( 1, -1, 0);
    r_end();

    r_end_frame();

    platform_update();
  }
}

#endif

#define R_VERTEX_MAX (1024 * 1024)
#define R_MATRIX_MAX (256)
#define R_TARGET_MAX (256)

static const char* vertex_shader = GLSL(
  layout (location = 0) in vec3 in_pos;
  layout (location = 1) in vec2 in_uv;
  layout (location = 2) in vec4 in_color;

  out vec2 frag_uv;
  out vec4 frag_color;

  uniform mat4 mvp;

  void main() {
    frag_uv = in_uv;
    frag_color = in_color;

    gl_Position = mvp * vec4(in_pos, 1);
  });

static const char* fragment_shader = GLSL(
  in vec2 frag_uv;
  in vec4 frag_color;

  out vec4 out_color;

  uniform sampler2D tex;
  uniform bool texture_enabled;

  void main() {
    vec4 color;
    vec2 tex_scale = 1.0 / textureSize(tex, 0);

    if (texture_enabled) {
      color = frag_color * texture(tex, frag_uv * tex_scale);
    } else {
      color = frag_color;
    }

    if (color.a <= 0) discard;
    out_color = color;
  });

static const char* r_post_fx_vertex_shader = GLSL(
  out vec2 frag_uv;

  const vec2 verts[6] = vec2[](
    vec2(-1, +1), vec2(-1, -1), vec2(+1, -1),
    vec2(-1, +1), vec2(+1, -1), vec2(+1, +1));

  const vec2 uvs[6] = vec2[](
    vec2(0, 1), vec2(0, 0), vec2(1, 0),
    vec2(0, 1), vec2(1, 0), vec2(1, 1));

  void main() {
    frag_uv = uvs[gl_VertexID];
    gl_Position = vec4(verts[gl_VertexID], 0, 1);
  });

static const char* r_post_fx_none = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  void main() {
    out_color = texture(tex, frag_uv);
  });

static const char* r_post_fx_blur = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  const float offset = 1.0 / 600.0;

  void main() {
    vec2 offsets[9] = vec2[](
      vec2(-offset,  offset),     // top-left
      vec2( 0.0f,    offset),     // top-center
      vec2( offset,  offset),     // top-right
      vec2(-offset,  0.0f),       // center-left
      vec2( 0.0f,    0.0f),       // center-center
      vec2( offset,  0.0f),       // center-right
      vec2(-offset, -offset),     // bottom-left
      vec2( 0.0f,   -offset),     // bottom-center
      vec2( offset, -offset));    // bottom-right    

    float kernel[9] = float[](
      1.0 / 16, 2.0 / 16, 1.0 / 16,
      2.0 / 16, 4.0 / 16, 2.0 / 16,
      1.0 / 16, 2.0 / 16, 1.0 / 16);

    vec4 sample_tex[9];
    for(int i = 0; i < 9; i++) {
      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
    }
    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
      col += sample_tex[i] * kernel[i];

    out_color = col;
  });

typedef struct r_vertex_t {
  v3 pos;
  v2 uv;

  u32 color;
} r_vertex_t;

typedef struct r_target_t {
  gl_shader_t shader;

  u32 framebuffer;
  u32 texture;
} r_target_t;

static gl_buffer_t r_post_fx_buffer;

static gl_texture_t r_current_texture;

static usize r_target_count;
static r_target_t r_target_array[R_TARGET_MAX];

static gl_shader_t r_shader;
static gl_buffer_t r_buffer;

static u32 r_type;
static u32 r_vertex_count;
static r_vertex_t r_current;
static r_vertex_t r_vertex_array[R_VERTEX_MAX];

static void
r_set_matrix(m4 mvp) {
  gl_use(&r_shader);
  gl_uniform_m4(gl_location(&r_shader, "mvp"), mvp);
}

static void
r_init(void) {
  gl_init();

  gl_buffer_desc_t fx_buffer_desc = ATS_INIT;
  r_post_fx_buffer = gl_buffer_create(&fx_buffer_desc);

  gl_shader_desc_t shader_desc = ATS_INIT;

  shader_desc.vs = vertex_shader;
  shader_desc.fs = fragment_shader;

  r_shader = gl_shader_create(&shader_desc);

  gl_buffer_desc_t buffer_desc = ATS_INIT;

  buffer_desc.layout[0] = Make(gl_layout_t) { 3, GL_FLOAT,         sizeof (r_vertex_t), offsetof (r_vertex_t, pos) };
  buffer_desc.layout[1] = Make(gl_layout_t) { 2, GL_FLOAT,         sizeof (r_vertex_t), offsetof (r_vertex_t, uv) };
  buffer_desc.layout[2] = Make(gl_layout_t) { 4, GL_UNSIGNED_BYTE, sizeof (r_vertex_t), offsetof (r_vertex_t, color), true };

  r_buffer = gl_buffer_create(&buffer_desc);

  gl_buffer_send(&r_buffer, r_vertex_array, sizeof (r_vertex_array));

  glEnable(GL_DEPTH_TEST);
}

static void
r_begin_frame(void) {
  gl_use(&r_shader);
  r_set_matrix(m4_identity());
}

static void
r_end_frame(void) {
  //
}

static void
r_begin(u32 type) {
  r_type = type;
  r_vertex_count = 0;
}

static void
r_uv(f32 x, f32 y) {
  r_current.uv = V2(x, y);
}

static void
r_color(u32 color) {
  r_current.color = color;
}

static void
r_vertex(f32 x, f32 y, f32 z) {
  r_current.pos = V3(x, y, z);
  r_vertex_array[r_vertex_count++] = r_current;
}

static void
r_end(void) {
  gl_use(&r_shader);
  gl_buffer_bind(&r_buffer);
  gl_buffer_send(&r_buffer, r_vertex_array, r_vertex_count * sizeof (r_vertex_t));

  glDrawArrays(r_type, 0, r_vertex_count);
}

static void
r_set_texture(const gl_texture_t* texture) {
  gl_use(&r_shader);
  gl_texture_bind(texture);

  r_current_texture = *texture;
}

static void
r_enable_textures(void) {
  gl_use(&r_shader);
  gl_uniform_i32(gl_location(&r_shader, "texture_enabled"), true);
}

static void
r_disable_textures(void) {
  gl_use(&r_shader);
  gl_uniform_i32(gl_location(&r_shader, "texture_enabled"), false);
}

static void
r_billboard(r2i tex_rect, v3 pos, v2 rad, u32 color, v3 right, v3 up) {
  f32 ax = pos.x - right.x * rad.x - up.x * rad.y;
  f32 ay = pos.y - right.y * rad.x - up.y * rad.y;
  f32 az = pos.z - right.z * rad.x - up.z * rad.y;

  f32 bx = pos.x + right.x * rad.x - up.x * rad.y;
  f32 by = pos.y + right.y * rad.x - up.y * rad.y;
  f32 bz = pos.z + right.z * rad.x - up.z * rad.y;

  f32 cx = pos.x + right.x * rad.x + up.x * rad.y;
  f32 cy = pos.y + right.y * rad.x + up.y * rad.y;
  f32 cz = pos.z + right.z * rad.x + up.z * rad.y;

  f32 dx = pos.x - right.x * rad.x + up.x * rad.y;
  f32 dy = pos.y - right.y * rad.x + up.y * rad.y;
  f32 dz = pos.z - right.z * rad.x + up.z * rad.y;

  r_color(color);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(ax, ay, az);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(bx, by, bz);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(cx, cy, cz);

  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(cx, cy, cz);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(dx, dy, dz);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(ax, ay, az);
}

static void
r_texture_box(r2i tex_rect, r3 box, u32 color) {
  r_color(color);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.max.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.min.x, box.max.y, box.min.z);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.max.x, box.min.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.min.x, box.max.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.max.z);

  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.min.x, box.max.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.min.x, box.max.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.min.x, box.min.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.min.x, box.max.y, box.max.z);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.max.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.max.x, box.min.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.max.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.max.x, box.max.y, box.min.z);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.max.x, box.min.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.min.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.min.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.min.x, box.min.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.min.y, box.min.z);

  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.max.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(box.max.x, box.max.y, box.min.z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(box.max.x, box.max.y, box.max.z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(box.min.x, box.max.y, box.min.z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(box.min.x, box.max.y, box.max.z);
}

static void
r_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color) {
  r_color(color);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(rect.min.x, rect.min.y, z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(rect.max.x, rect.min.y, z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(rect.max.x, rect.max.y, z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(rect.max.x, rect.max.y, z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(rect.min.x, rect.max.y, z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(rect.min.x, rect.min.y, z);
}

static void
r_texture_rect_flip(r2i tex_rect, r2 rect, f32 z, u32 color, bool flip_x, bool flip_y) {
  if (flip_x) { Swap(f32, tex_rect.min.x, tex_rect.max.x); }
  if (flip_y) { Swap(f32, tex_rect.min.y, tex_rect.max.y); }

  r_color(color);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(rect.min.x, rect.min.y, z);
  r_uv(tex_rect.max.x, tex_rect.max.y); r_vertex(rect.max.x, rect.min.y, z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(rect.max.x, rect.max.y, z);
  r_uv(tex_rect.max.x, tex_rect.min.y); r_vertex(rect.max.x, rect.max.y, z);
  r_uv(tex_rect.min.x, tex_rect.min.y); r_vertex(rect.min.x, rect.max.y, z);
  r_uv(tex_rect.min.x, tex_rect.max.y); r_vertex(rect.min.x, rect.min.y, z);
}

static void
r_box(r3 box, u32 color) {
  r_color(color);

  r_vertex(box.min.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.min.z);
  r_vertex(box.max.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.min.z);
  r_vertex(box.min.x, box.min.y, box.min.z);
  r_vertex(box.min.x, box.max.y, box.min.z);

  r_vertex(box.min.x, box.min.y, box.max.z);
  r_vertex(box.max.x, box.min.y, box.max.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.min.x, box.max.y, box.max.z);
  r_vertex(box.min.x, box.min.y, box.max.z);

  r_vertex(box.min.x, box.max.y, box.max.z);
  r_vertex(box.min.x, box.max.y, box.min.z);
  r_vertex(box.min.x, box.min.y, box.min.z);
  r_vertex(box.min.x, box.min.y, box.min.z);
  r_vertex(box.min.x, box.min.y, box.max.z);
  r_vertex(box.min.x, box.max.y, box.max.z);

  r_vertex(box.max.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.max.x, box.min.y, box.max.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.max.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.min.z);

  r_vertex(box.min.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.min.y, box.min.z);
  r_vertex(box.max.x, box.min.y, box.max.z);
  r_vertex(box.max.x, box.min.y, box.max.z);
  r_vertex(box.min.x, box.min.y, box.max.z);
  r_vertex(box.min.x, box.min.y, box.min.z);

  r_vertex(box.min.x, box.max.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.max.x, box.max.y, box.min.z);
  r_vertex(box.max.x, box.max.y, box.max.z);
  r_vertex(box.min.x, box.max.y, box.min.z);
  r_vertex(box.min.x, box.max.y, box.max.z);
}

static void
r_rect(r2 rect, f32 z, u32 color) {
  r_color(color);
  r_vertex(rect.min.x, rect.min.y, z);
  r_vertex(rect.max.x, rect.min.y, z);
  r_vertex(rect.max.x, rect.max.y, z);
  r_vertex(rect.max.x, rect.max.y, z);
  r_vertex(rect.min.x, rect.max.y, z);
  r_vertex(rect.min.x, rect.min.y, z);
}

static u32
r_new_target(const char* fragment_shader) {
  r_target_t* target = r_target_array + r_target_count++;

  gl_shader_desc_t shader_desc = ATS_INIT;

  shader_desc.vs = r_post_fx_vertex_shader;
  shader_desc.fs = fragment_shader;

  target->shader = gl_shader_create(&shader_desc);

  glGenFramebuffers(1, &target->framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);

  glGenTextures(1, &target->texture);
  glBindTexture(GL_TEXTURE_2D, target->texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->texture, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return r_target_count - 1;
}

static r_target_t* r_current_target = NULL;

static void
r_begin_pass(u32 target) {
  r_current_target = r_target_array + target;

  glBindFramebuffer(GL_FRAMEBUFFER, r_current_target->framebuffer);

  glBindTexture(GL_TEXTURE_2D, r_current_target->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, platform.width, platform.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  r_set_texture(&r_current_texture);
}

static void
r_end_pass(void) {
  glDisable(GL_DEPTH_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  gl_use(&r_current_target->shader);
  gl_buffer_bind(&r_post_fx_buffer);

  glBindTexture(GL_TEXTURE_2D, r_current_target->texture);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  r_current_target = NULL;
  glEnable(GL_DEPTH_TEST);

  r_set_texture(&r_current_texture);
}

#if 0

static const char* fs_blur = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  const float offset = 1.0 / 700.0;

  void main() {
    vec2 offsets[9] = vec2[](
      vec2(-offset,  offset),     // top-left
      vec2( 0.0f,    offset),     // top-center
      vec2( offset,  offset),     // top-right
      vec2(-offset,  0.0f),       // center-left
      vec2( 0.0f,    0.0f),       // center-center
      vec2( offset,  0.0f),       // center-right
      vec2(-offset, -offset),     // bottom-left
      vec2( 0.0f,   -offset),     // bottom-center
      vec2( offset, -offset));    // bottom-right    

    float kernel[9] = float[](
      1.0 / 16, 2.0 / 16, 1.0 / 16,
      2.0 / 16, 4.0 / 16, 2.0 / 16,
      1.0 / 16, 2.0 / 16, 1.0 / 16);

    vec4 sample_tex[9];
    for(int i = 0; i < 9; i++) {
      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
    }
    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
      col += sample_tex[i] * kernel[i];

    out_color = col;
  });


static const char* fs_edge = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  const float offset = 1.0 / 800.0;  

  void main() {
    vec2 offsets[9] = vec2[](
      vec2(-offset,  offset),     // top-left
      vec2( 0.0f,    offset),     // top-center
      vec2( offset,  offset),     // top-right
      vec2(-offset,  0.0f),       // center-left
      vec2( 0.0f,    0.0f),       // center-center
      vec2( offset,  0.0f),       // center-right
      vec2(-offset, -offset),     // bottom-left
      vec2( 0.0f,   -offset),     // bottom-center
      vec2( offset, -offset));    // bottom-right    

    float kernel[9] = float[](
      1,  1, 1,
      1, -8, 1,
      1,  1, 1
    );

    vec4 sample_tex[9];
    for(int i = 0; i < 9; i++) {
      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
    }
    vec4 col = vec4(0.0);
    for(int i = 0; i < 9; i++)
      col += sample_tex[i] * kernel[i];

    if (col.a > 0) {
      out_color = vec4(0, 0, 0, 0.7);
    } else {
      out_color = texture(tex, frag_uv.st);
    }
  });

#endif

