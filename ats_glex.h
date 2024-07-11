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

int main(void)
{
  platform_init("YOLO", 1600, 800, 4);
  r_init();

  // u32 game_pass = r_new_target();
  // u32 lighting_pass = r_new_target();

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
  layout (location = 1) in vec3 in_normal;
  layout (location = 2) in vec2 in_uv;
  layout (location = 3) in vec4 in_color;
  layout (location = 4) in float in_depth;

  out vec3 frag_pos;
  out vec3 frag_normal;
  out vec2 frag_uv;
  out vec4 frag_color;
  out float frag_depth;

  uniform mat4 mvp;

  void main()
  {
    frag_pos = in_pos;
    frag_normal = in_normal;
    frag_uv = in_uv;
    frag_color = in_color;
    frag_depth = in_depth;

    gl_Position = mvp * vec4(in_pos, 1);
  });

static const char* fragment_shader = GLSL(
  struct point_light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
  };

  vec3 calculate_point_light(point_light light, vec3 view_pos, vec3 frag_pos, vec3 frag_normal, vec4 color)
  {
    // ambient
    vec3 ambient = light.ambient * color.rgb;

    // diffuse 
    vec3 norm = normalize(frag_normal);
    vec3 light_dir = normalize(light.pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * color.rgb;

    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 2);
    vec3 specular = light.specular * spec * color.rgb;  

    // attenuation
    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
  }

  in vec3 frag_pos;
  in vec3 frag_normal;
  in vec2 frag_uv;
  in vec4 frag_color;
  in float frag_depth;

  out vec4 out_color;

  uniform sampler2D tex;
  uniform bool texture_enabled;

  uniform bool fog_enabled;
  uniform vec3 fog_color;

  uniform bool depth_enabled;

  uniform vec3 view_pos;

  uniform bool lighting_enabled;
  uniform int light_count;
  uniform point_light light_array[16];

  void main()
  {
    vec4 color = frag_color;

    if (texture_enabled) {
      vec2 tex_scale = 1.0 / textureSize(tex, 0);
      color = color * texture(tex, frag_uv * tex_scale);
    }

    if (color.a <= 0) discard;

    if (depth_enabled) {
      gl_FragDepth = frag_depth;
    } else {
      gl_FragDepth = gl_FragCoord.z;
    }

    if (lighting_enabled) {
      vec3 result;
      for (int i = 0; i < light_count; ++i) {
        result += calculate_point_light(light_array[i], view_pos, frag_pos, frag_normal, color);
      }
      color.rgb = result;
    }

    if (fog_enabled) {
      float distance = gl_FragCoord.z / gl_FragCoord.w;
      float d = 0.2 * distance;
      float f = 1.0 - clamp(exp2(-1.442695 * d * d), 0.0, 1.0);

      color.rgb = mix(color.rgb, fog_color, f);
    }

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

  void main()
  {
    frag_uv = uvs[gl_VertexID];
    gl_Position = vec4(verts[gl_VertexID], 0, 1);
  });

static const char* r_post_fx_none = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  void main()
  {
    out_color = texture(tex, frag_uv);
  });

static const char* r_post_fx_blur = GLSL(
  in  vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  const float offset = 1.0 / 1200.0;

  void main()
  {
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
    for (int i = 0; i < 9; i++) {
      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
    }
    vec4 col = vec4(0.0);
    for (int i = 0; i < 9; i++)
      col += sample_tex[i] * kernel[i];

    out_color = col;
  });

typedef struct {
  v3 pos;
  v3 normal;
  v2 uv;
  u32 color;
  f32 depth;
} r_vertex_data;

typedef struct {
  gl_shader shader;

  u32 framebuffer;
  u32 texture;
} r_target_data;

typedef struct {
  v3 pos;
  
  v3 ambient;
  v3 diffuse;
  v3 specular;

  f32 constant; 
  f32 linear;
  f32 quadratic;
} r_light;

static u32 r_light_count;
static r_light r_light_array[16];

static gl_buffer r_post_fx_buffer;
static gl_texture r_current_texture;
static usize r_target_count;
static r_target_data r_target_array[R_TARGET_MAX];
static gl_shader r_shader;
static gl_buffer r_buffer;
static u32 r_type;
static r_vertex_data r_current;
static u32 r_vertex_count;
static r_vertex_data r_vertex_array[R_VERTEX_MAX];

static v3 r_view_pos;
static v3 r_view_dir;

static void gl_set_matrix(m4 projection, m4 view)
{
  m4 mvp = m4_mul(projection, view);

  gl_use(&r_shader);
  gl_uniform_m4(gl_location(&r_shader, "mvp"), mvp);
}

static void gl_set_view(v3 pos, v3 dir)
{
  r_view_pos = pos;
  r_view_dir = v3_norm(dir);
  
  gl_uniform_v3(gl_location(&r_shader, "view_pos"), r_view_pos);
}

static void gl_init_ex(void)
{
  gl_init();

  gl_buffer_desc fx_buffer_desc = {0};
  r_post_fx_buffer = gl_buffer_create(&fx_buffer_desc);

  gl_shader_desc shader_desc = {0};

  shader_desc.vs = vertex_shader;
  shader_desc.fs = fragment_shader;

  r_shader = gl_shader_create(shader_desc);

  gl_buffer_desc buffer_desc = {0};

  buffer_desc.layout[0] = make(gl_layout) { 3, GL_FLOAT,         sizeof (r_vertex_data), offsetof(r_vertex_data, pos) };
  buffer_desc.layout[1] = make(gl_layout) { 3, GL_FLOAT,         sizeof (r_vertex_data), offsetof(r_vertex_data, normal) };
  buffer_desc.layout[2] = make(gl_layout) { 2, GL_FLOAT,         sizeof (r_vertex_data), offsetof(r_vertex_data, uv) };
  buffer_desc.layout[3] = make(gl_layout) { 4, GL_UNSIGNED_BYTE, sizeof (r_vertex_data), offsetof(r_vertex_data, color), 1 };
  buffer_desc.layout[4] = make(gl_layout) { 1, GL_FLOAT,         sizeof (r_vertex_data), offsetof(r_vertex_data, depth) };

  r_buffer = gl_buffer_create(&buffer_desc);

  gl_buffer_send(&r_buffer, r_vertex_array, sizeof (r_vertex_array));

  glEnable(GL_DEPTH_TEST);
}

static void gl_begin_frame(void)
{
  gl_use(&r_shader);
  gl_set_matrix(m4_identity(), m4_identity());

  r_view_pos = v3(0);
  r_light_count = 0;
}

static void gl_end_frame(void)
{
  // WOOP
}

static void gl_begin(u32 type)
{
  r_type = type;
  r_vertex_count = 0;
}

static void gl_uv(f32 x, f32 y)
{
  r_current.uv = v2(x, y);
}

static void gl_color(u32 color)
{
  r_current.color = color;
}

static void gl_normal(f32 x, f32 y, f32 z)
{
  r_current.normal = v3(x, y, z);
}

static void gl_depth(f32 depth)
{
  r_current.depth = depth;
}

static void gl_vertex(f32 x, f32 y, f32 z)
{
  r_current.pos = v3(x, y, z);
  r_vertex_array[r_vertex_count++] = r_current;
}

static void gl_end(void)
{
  gl_use(&r_shader);
  gl_buffer_bind(&r_buffer);
  gl_buffer_send(&r_buffer, r_vertex_array, r_vertex_count * sizeof (r_vertex_data));

  // add lights
  {
    i32 count = min(32, r_light_count);

    char buffer[256];
    for (i32 i = 0; i < count; ++i) {
      r_light light = r_light_array[i];

#define set_v3(var)  sprintf(buffer, "light_array[%d]." #var, i); gl_uniform_v3(gl_location(&r_shader, buffer), light.var);
#define set_f32(var) sprintf(buffer, "light_array[%d]." #var, i); gl_uniform_f32(gl_location(&r_shader, buffer), light.var);
      set_v3(pos);
      set_v3(ambient);
      set_v3(diffuse);
      set_v3(specular);
      set_f32(constant);
      set_f32(linear);
      set_f32(quadratic);
#undef set_v3
#undef set_f32
    }

    gl_uniform_i32(gl_location(&r_shader, "light_count"), count);
  }

  glDrawArrays(r_type, 0, r_vertex_count);
}

static void gl_set_texture(const gl_texture* texture)
{
  gl_use(&r_shader);
  gl_texture_bind(texture);

  r_current_texture = *texture;
}

enum {
  GLEX_NONE,
  GLEX_TEXTURE,
  GLEX_FOG,
  GLEX_LIGHTING,
  GLEX_DEPTH,
};

static void gl_enable(u32 tag)
{
  gl_use(&r_shader);

  switch (tag) {
    case GLEX_TEXTURE: {
      gl_uniform_i32(gl_location(&r_shader, "texture_enabled"), 1);
    } break;

    case GLEX_FOG: {
      gl_uniform_i32(gl_location(&r_shader, "fog_enabled"), 1);
    } break;

    case GLEX_LIGHTING: {
      gl_uniform_i32(gl_location(&r_shader, "lighting_enabled"), 1);
    } break;

    case GLEX_DEPTH: {
      gl_uniform_i32(gl_location(&r_shader, "depth_enabled"), 1);
    } break;
  }
}

static void gl_disable(u32 tag)
{
  gl_use(&r_shader);

  switch (tag) {
    case GLEX_TEXTURE: {
      gl_uniform_i32(gl_location(&r_shader, "texture_enabled"), 0);
    } break;

    case GLEX_FOG: {
      gl_uniform_i32(gl_location(&r_shader, "fog_enabled"), 0);
    } break;

    case GLEX_LIGHTING: {
      gl_uniform_i32(gl_location(&r_shader, "lighting_enabled"), 0);
    } break;

    case GLEX_DEPTH: {
      gl_uniform_i32(gl_location(&r_shader, "depth_enabled"), 0);
    } break;
  }
}

static void gl_fog_color(f32 r, f32 g, f32 b)
{
  gl_uniform_v3(gl_location(&r_shader, "fog_color"), v3(r, g, b));
}

static void gl_add_light(v3 pos, v3 ambient, v3 diffuse, v3 specular, f32 constant, f32 linear, f32 quadratic)
{
  if (r_light_count >= countof(r_light_array)) { return; }
  r_light_array[r_light_count++] = make(r_light) { pos, ambient, diffuse, specular, constant, linear, quadratic };
}

static void gl_billboard(tex_rect tr, v3 pos, v2 rad, u32 color, v3 right, v3 up)
{
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

  gl_color(color);
  gl_normal(-r_view_dir.x, -r_view_dir.y, -r_view_dir.z);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(ax, ay, az);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(bx, by, bz);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(cx, cy, cz);

  gl_uv(tr.max_x, tr.min_y); gl_vertex(cx, cy, cz);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(dx, dy, dz);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(ax, ay, az);
}

static void r_texture_box(tex_rect tr, r3 box, u32 color)
{
  gl_color(color);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.min.x, box.max.y, box.min.z);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.max.z);

  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.max.x, box.max.y, box.min.z);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);

  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
}

static void gl_texture_rect(tex_rect tr, r2 rect, f32 z, u32 color)
{
  gl_color(color);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(rect.min.x, rect.min.y, z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(rect.max.x, rect.min.y, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(rect.min.x, rect.max.y, z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(rect.min.x, rect.min.y, z);
}

static void r_texture_rect_flip(tex_rect tr, r2 rect, f32 z, u32 color, b32 flip_x, b32 flip_y)
{
  if (flip_x) { swap(f32, tr.min_x, tr.max_x); }
  if (flip_y) { swap(f32, tr.min_y, tr.max_y); }

  gl_color(color);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(rect.min.x, rect.min.y, z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(rect.max.x, rect.min.y, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(rect.min.x, rect.max.y, z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(rect.min.x, rect.min.y, z);
}

static void gl_rotated_texture(tex_rect tr, v2 pos, f32 z, v2 rad, f32 rot, u32 color, b32 flip_y)
{
  if (flip_y) { swap(f32, tr.min_y, tr.max_y); }

  m2 rot_matrix = m2_rotate(rot);

  v2 u = m2_mulv(rot_matrix, v2(0, 1));
  v2 r = m2_mulv(rot_matrix, v2(1, 0));

  f32 ax = pos.x - rad.x * r.x - rad.y * u.x;
  f32 ay = pos.y - rad.x * r.y - rad.y * u.y;

  f32 bx = pos.x + rad.x * r.x - rad.y * u.x;
  f32 by = pos.y + rad.x * r.y - rad.y * u.y;

  f32 cx = pos.x + rad.x * r.x + rad.y * u.x;
  f32 cy = pos.y + rad.x * r.y + rad.y * u.y;

  f32 dx = pos.x - rad.x * r.x + rad.y * u.x;
  f32 dy = pos.y - rad.x * r.y + rad.y * u.y;

  gl_color(color);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(ax, ay, z);
  gl_uv(tr.max_x, tr.max_y); gl_vertex(bx, by, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(cx, cy, z);
  gl_uv(tr.max_x, tr.min_y); gl_vertex(cx, cy, z);
  gl_uv(tr.min_x, tr.min_y); gl_vertex(dx, dy, z);
  gl_uv(tr.min_x, tr.max_y); gl_vertex(ax, ay, z);
}

static void gl_rotated(v2 pos, f32 z, v2 rad, f32 rot, u32 color)
{
  m2 rot_matrix = m2_rotate(rot);

  v2 u = m2_mulv(rot_matrix, v2(0, 1));
  v2 r = m2_mulv(rot_matrix, v2(1, 0));

  f32 ax = pos.x - rad.x * r.x - rad.y * u.x;
  f32 ay = pos.y - rad.x * r.y - rad.y * u.y;

  f32 bx = pos.x + rad.x * r.x - rad.y * u.x;
  f32 by = pos.y + rad.x * r.y - rad.y * u.y;

  f32 cx = pos.x + rad.x * r.x + rad.y * u.x;
  f32 cy = pos.y + rad.x * r.y + rad.y * u.y;

  f32 dx = pos.x - rad.x * r.x + rad.y * u.x;
  f32 dy = pos.y - rad.x * r.y + rad.y * u.y;

  gl_color(color);
  gl_vertex(ax, ay, z);
  gl_vertex(bx, by, z);
  gl_vertex(cx, cy, z);
  gl_vertex(cx, cy, z);
  gl_vertex(dx, dy, z);
  gl_vertex(ax, ay, z);
}

static void gl_line(v2 p0, v2 p1, f32 z, f32 rad, u32 color)
{
  v2 line         = v2_sub(p1, p0);
  f32 line_length = v2_len(line);
  v2 line_pos     = v2_add(p0, v2_scale(line, 0.5f));
  v2 axis         = { 0.0f, -1.0f };
  f32 rot         = v2_get_angle(axis, line);
  v2 pos          = { line_pos.x, line_pos.y };
  v2 scale        = { rad, 0.5f * line_length };

  gl_rotated(pos, z, scale, rot, color);
}

static void gl_box(r3 box, u32 color)
{
  gl_color(color);

  gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_vertex(box.min.x, box.max.y, box.min.z);

  gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_vertex(box.min.x, box.min.y, box.max.z);

  gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_vertex(box.min.x, box.max.y, box.max.z);

  gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.min.z);

  gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_vertex(box.min.x, box.min.y, box.min.z);

  gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_vertex(box.min.x, box.max.y, box.max.z);
}

static void gl_rect(r2 rect, f32 z, u32 color)
{
  gl_color(color);
  gl_vertex(rect.min.x, rect.min.y, z);
  gl_vertex(rect.max.x, rect.min.y, z);
  gl_vertex(rect.max.x, rect.max.y, z);
  gl_vertex(rect.max.x, rect.max.y, z);
  gl_vertex(rect.min.x, rect.max.y, z);
  gl_vertex(rect.min.x, rect.min.y, z);
}

static u32 gl_new_target(const char* fragment_shader)
{
  r_target_data* target = r_target_array + r_target_count++;
  gl_shader_desc shader_desc = {0};

  shader_desc.vs = r_post_fx_vertex_shader;
  shader_desc.fs = fragment_shader;

  target->shader = gl_shader_create(shader_desc);

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

static r_target_data* r_current_target = NULL;

static void gl_begin_pass(u32 target, f32 r, f32 g, f32 b, f32 a)
{
  r_current_target = r_target_array + target;

  glBindFramebuffer(GL_FRAMEBUFFER, r_current_target->framebuffer);

  glBindTexture(GL_TEXTURE_2D, r_current_target->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, platform.width, platform.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  gl_set_texture(&r_current_texture);
}

static void gl_end_pass(void)
{
  glDisable(GL_DEPTH_TEST);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  gl_use(&r_current_target->shader);
  gl_buffer_bind(&r_post_fx_buffer);

  glBindTexture(GL_TEXTURE_2D, r_current_target->texture);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  r_current_target = NULL;
  glEnable(GL_DEPTH_TEST);

  gl_set_texture(&r_current_texture);
}

#if 0
static const char* fs_blur = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  const float offset = 1.0 / 700.0;

  void main()
  {
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

  void main()
  {
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

