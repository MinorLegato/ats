#pragma once

// EXAMPLE:
//#define ATS_OGL33
//#include "../ats/ats.h"
//#include "../ats/ats_platform.h"
//
//#include "../ats/ats_file.c"
//#include "../ats/ats_memory.c"
//
//#include "../ats/ats_platform.c"
//#include "../ats/ats_gl33.c"
//
//#include "render.c"
//
//int main(void)
//{
//  platform_init("YOLO", 1600, 800, 4);
//  glex_init();
//
//  while (!platform.close)
//  {
//    if (platform.keyboard.pressed[KEY_ESCAPE])
//    {
//      platform.close = true;
//    }
//
//    m4 projection = m4_perspective(0.5 * PI, platform.aspect_ratio, 0.1, 32);
//    m4 view       = m4_look_at(v3(0, 0, 8), v3(0, 0, 0), v3(0, 1, 0));
//    m4 mvp        = m4_mul(projection, view);
//
//    glex_begin_frame();
//
//    glex_clear(0xff000000);
//    glex_set_matrix(mvp);
//
//    glex_begin(GL_TRIANGLES);
//    glex_color(0xffffff00); glex_vertex( 0,  1, 0);
//    glex_color(0xffff00ff); glex_vertex(-1, -1, 0);
//    glex_color(0xff00ffff); glex_vertex( 1, -1, 0);
//    glex_end();
//
//    glex_end_frame();
//
//    platform_update();
//  }
//}

#define GLEX_VERTEX_MAX (4 * 1024 * 1024)
#define GLEX_TARGET_MAX (256)

static const char* glex_vertex_shader = GLSL(
  layout (location = 0) in vec3 in_pos;
  layout (location = 1) in vec3 in_normal;
  layout (location = 2) in vec2 in_uv;
  layout (location = 3) in vec4 in_color;

  out vec3 frag_pos;
  out vec3 frag_normal;
  out vec2 frag_uv;
  out vec4 frag_color;

  uniform mat4 mvp;

  void main()
  {
    frag_pos    = in_pos;
    frag_normal = in_normal;
    frag_uv     = in_uv;
    frag_color  = in_color;
    gl_Position = mvp * vec4(in_pos, 1);
  });

static const char* glex_fragment_shader = GLSL(
  struct point_light
  {
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

  out vec4 out_color;

  uniform sampler2D tex;
  uniform bool texture_enabled;

  uniform bool fog_enabled;
  uniform vec3 fog_color;

  uniform vec3 view_pos;

  uniform bool lighting_enabled;
  uniform int light_count;
  uniform point_light light_array[16];

  void main()
  {
    vec4 color = frag_color;

    if (texture_enabled)
    {
      vec2 tex_scale = 1.0 / textureSize(tex, 0);
      color = color * texture(tex, frag_uv * tex_scale);
    }

    if (color.a <= 0) discard;

    if (lighting_enabled)
    {
      vec3 result;
      for (int i = 0; i < light_count; ++i)
      {
        result += calculate_point_light(light_array[i], view_pos, frag_pos, frag_normal, color);
      }
      color.rgb = result;
    }

    if (fog_enabled)
    {
      float distance = gl_FragCoord.z / gl_FragCoord.w;
      float d = 0.2 * distance;
      float f = 1.0 - clamp(exp2(-1.442695 * d * d), 0.0, 1.0);

      color.rgb = mix(color.rgb, fog_color, f);
    }

    out_color = color;
  });

static const char* glex_post_fx_vertex_shader = GLSL(
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

static const char* glex_post_fx_none = GLSL(
  in vec2 frag_uv;
  out vec4 out_color;

  uniform sampler2D tex;

  void main()
  {
    out_color = texture(tex, frag_uv);
  });

static const char* glex_post_fx_blur = GLSL(
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
    for (int i = 0; i < 9; i++)
    {
      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
    }

    vec4 col = vec4(0.0);
    for (int i = 0; i < 9; i++)
    {
      col += sample_tex[i] * kernel[i];
    }

    out_color = col;
  });

typedef struct glex__vertex glex__vertex;
struct glex__vertex
{
  v3 pos;
  v3 normal;
  v2 uv;
  u32 color;
};

typedef struct glex__light glex__light;
struct glex__light
{
  v3 pos;
  v3 ambient;
  v3 diffuse;
  v3 specular;
  f32 constant; 
  f32 linear;
  f32 quadratic;
};

static struct
{
  v3 view_pos;
  v3 view_dir;

  gl_shader shader;
  gl_buffer buffer;
  gl_buffer post_fx_buffer;

  gl_texture current_texture;

  u32 light_count;
  glex__light light_array[16];

  struct
  {
    gl_shader shader;
    u32 framebuffer;
    u32 texture;
    u32 renderbuffer;
    gl_shader default_shader;
  } target;

  u32 type;
  glex__vertex current;
  u32 vertex_count;
  glex__vertex vertex_array[GLEX_VERTEX_MAX];
} glex;

static void glex_set_matrix(m4 projection, m4 view)
{
  m4 mvp = m4_mul(projection, view);

  gl_use(&glex.shader);
  gl_uniform_m4(gl_location(&glex.shader, "mvp"), mvp);
}

static void glex_set_view(v3 pos, v3 dir)
{
  glex.view_pos = pos;
  glex.view_dir = v3_norm(dir);
  gl_uniform_v3(gl_location(&glex.shader, "view_pos"), glex.view_pos);
}

static void glex_set_texture(const gl_texture* texture)
{
  gl_use(&glex.shader);
  gl_texture_bind(texture);
  glex.current_texture = *texture;
}

static void glex_init(void)
{
  gl_init();
  gl_buffer_desc fx_buffer_desc = {0};
  glex.post_fx_buffer = gl_buffer_create(&fx_buffer_desc);

  gl_shader_desc shader_desc = {0};

  shader_desc.vs = glex_vertex_shader;
  shader_desc.fs = glex_fragment_shader;

  glex.shader = gl_shader_create(shader_desc);

  gl_buffer_desc buffer_desc = {0};

  buffer_desc.layout[0] = make(gl_layout) { 3, GL_FLOAT,         sizeof (glex__vertex), offsetof(glex__vertex, pos) };
  buffer_desc.layout[1] = make(gl_layout) { 3, GL_FLOAT,         sizeof (glex__vertex), offsetof(glex__vertex, normal) };
  buffer_desc.layout[2] = make(gl_layout) { 2, GL_FLOAT,         sizeof (glex__vertex), offsetof(glex__vertex, uv) };
  buffer_desc.layout[3] = make(gl_layout) { 4, GL_UNSIGNED_BYTE, sizeof (glex__vertex), offsetof(glex__vertex, color), 1 };

  glex.buffer = gl_buffer_create(&buffer_desc);

  gl_buffer_send(&glex.buffer, glex.vertex_array, sizeof (glex.vertex_array));

  glEnable(GL_DEPTH_TEST);

  {
    gl_shader_desc shader_desc = {0};

    shader_desc.vs = glex_post_fx_vertex_shader;
    shader_desc.fs = glex_post_fx_none;

    glex.target.default_shader = gl_shader_create(shader_desc);

    glGenFramebuffers(1, &glex.target.framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glex.target.framebuffer);
    // color buffer:
    glGenTextures(1, &glex.target.texture);
    glBindTexture(GL_TEXTURE_2D, glex.target.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glex.target.texture, 0);
    // render buffer:
    glGenRenderbuffers(1, &glex.target.renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, glex.target.renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 4096, 4096);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glex.target.renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

static void glex_end_pass(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  gl_use(&glex.target.shader);
  gl_buffer_bind(&glex.post_fx_buffer);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glex.target.texture);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glex_set_texture(&glex.current_texture);
}

static void glex_begin_pass(gl_shader shader, f32 r, f32 g, f32 b, f32 a)
{
  glex_end_pass();

  glex.target.shader = shader;
  glBindFramebuffer(GL_FRAMEBUFFER, glex.target.framebuffer);

  glBindTexture(GL_TEXTURE_2D, glex.target.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, platform.width, platform.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, glex.target.renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, platform.width, platform.height);

  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glex_set_texture(&glex.current_texture);
}

static void glex_begin_frame(void)
{
  gl_use(&glex.shader);
  glex_set_matrix(m4_identity(), m4_identity());
  glex.view_pos = v3(0);
  glex.light_count = 0;

  glex_begin_pass(glex.target.default_shader, 0, 0, 0, 1);
}

static void glex_end_frame(void)
{
  glex_end_pass();
}

static void glex_begin(u32 type)
{
  glex.type = type;
  glex.vertex_count = 0;
}

static void glex_uv(f32 x, f32 y)
{
  glex.current.uv = v2(x, y);
}

static void glex_color(u32 color)
{
  glex.current.color = color;
}

static void glex_normal(f32 x, f32 y, f32 z)
{
  glex.current.normal = v3(x, y, z);
}

static void glex_vertex(f32 x, f32 y, f32 z)
{
  glex.current.pos = v3(x, y, z);
  glex.vertex_array[glex.vertex_count++] = glex.current;
}

static void glex_end(void)
{
  gl_use(&glex.shader);
  gl_buffer_bind(&glex.buffer);
  gl_buffer_send(&glex.buffer, glex.vertex_array, glex.vertex_count * sizeof (glex__vertex));

  // add lights
  {
    i32 count = min(32, glex.light_count);

    char buffer[256];
    for (i32 i = 0; i < count; ++i)
    {
      glex__light light = glex.light_array[i];
#define set_v3(var)  sprintf(buffer, "light_array[%d]." #var, i); gl_uniform_v3(gl_location(&glex.shader, buffer), light.var);
#define set_f32(var) sprintf(buffer, "light_array[%d]." #var, i); gl_uniform_f32(gl_location(&glex.shader, buffer), light.var);
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

    gl_uniform_i32(gl_location(&glex.shader, "light_count"), count);
  }

  glDrawArrays(glex.type, 0, glex.vertex_count);
}

enum
{
  GLEX_NONE,
  GLEX_TEXTURE,
  GLEX_FOG,
  GLEX_LIGHTING,
};

static void glex_enable(u32 tag)
{
  gl_use(&glex.shader);
  switch (tag)
  {
    case GLEX_TEXTURE:
    {
      gl_uniform_i32(gl_location(&glex.shader, "texture_enabled"), 1);
    } break;
    case GLEX_FOG:
    {
      gl_uniform_i32(gl_location(&glex.shader, "fog_enabled"), 1);
    } break;
    case GLEX_LIGHTING:
    {
      gl_uniform_i32(gl_location(&glex.shader, "lighting_enabled"), 1);
    } break;
  }
}

static void glex_disable(u32 tag)
{
  gl_use(&glex.shader);
  switch (tag)
  {
    case GLEX_TEXTURE:
    {
      gl_uniform_i32(gl_location(&glex.shader, "texture_enabled"), 0);
    } break;
    case GLEX_FOG:
    {
      gl_uniform_i32(gl_location(&glex.shader, "fog_enabled"), 0);
    } break;
    case GLEX_LIGHTING:
    {
      gl_uniform_i32(gl_location(&glex.shader, "lighting_enabled"), 0);
    } break;
  }
}

static void glex_fog_color(f32 r, f32 g, f32 b)
{
  gl_uniform_v3(gl_location(&glex.shader, "fog_color"), v3(r, g, b));
}

static void glex_add_light(v3 pos, v3 ambient, v3 diffuse, v3 specular, f32 constant, f32 linear, f32 quadratic)
{
  if (glex.light_count >= countof(glex.light_array))
  {
    return;
  }
  glex.light_array[glex.light_count++] = make(glex__light) { pos, ambient, diffuse, specular, constant, linear, quadratic };
}

static void glex_billboard(tex_rect tr, v3 pos, v2 rad, u32 color, v3 right, v3 up)
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

  glex_color(color);
  glex_normal(-glex.view_dir.x, -glex.view_dir.y, -glex.view_dir.z);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(ax, ay, az);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(bx, by, bz);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(cx, cy, cz);

  glex_uv(tr.max_x, tr.min_y); glex_vertex(cx, cy, cz);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(dx, dy, dz);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(ax, ay, az);
}

static void glex_texture_box(tex_rect tr, r3 box, u32 color)
{
  glex_color(color);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.min.x, box.max.y, box.min.z);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.min.x, box.max.y, box.max.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.max.z);

  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.min.x, box.max.y, box.max.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.min.x, box.max.y, box.max.z);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.max.x, box.max.y, box.min.z);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.min.y, box.min.z);

  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(box.min.x, box.max.y, box.max.z);
}

static void glex_texture_rect(tex_rect tr, r2 rect, f32 z, u32 color)
{
  glex_color(color);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(rect.min.x, rect.min.y, z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(rect.max.x, rect.min.y, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(rect.max.x, rect.max.y, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(rect.max.x, rect.max.y, z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(rect.min.x, rect.max.y, z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(rect.min.x, rect.min.y, z);
}

static void glex_texture_rect_flip(tex_rect tr, r2 rect, f32 z, u32 color, b32 flip_x, b32 flip_y)
{
  if (flip_x) swap(f32, tr.min_x, tr.max_x);
  if (flip_y) swap(f32, tr.min_y, tr.max_y);

  glex_color(color);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(rect.min.x, rect.min.y, z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(rect.max.x, rect.min.y, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(rect.max.x, rect.max.y, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(rect.max.x, rect.max.y, z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(rect.min.x, rect.max.y, z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(rect.min.x, rect.min.y, z);
}

static void glex_rotated_texture(tex_rect tr, v2 pos, f32 z, v2 rad, f32 rot, u32 color, b32 flip_y)
{
  if (flip_y) swap(f32, tr.min_y, tr.max_y);

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

  glex_color(color);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(ax, ay, z);
  glex_uv(tr.max_x, tr.max_y); glex_vertex(bx, by, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(cx, cy, z);
  glex_uv(tr.max_x, tr.min_y); glex_vertex(cx, cy, z);
  glex_uv(tr.min_x, tr.min_y); glex_vertex(dx, dy, z);
  glex_uv(tr.min_x, tr.max_y); glex_vertex(ax, ay, z);
}

static void glex_rotated(v2 pos, f32 z, v2 rad, f32 rot, u32 color)
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

  glex_color(color);
  glex_vertex(ax, ay, z);
  glex_vertex(bx, by, z);
  glex_vertex(cx, cy, z);
  glex_vertex(cx, cy, z);
  glex_vertex(dx, dy, z);
  glex_vertex(ax, ay, z);
}

static void glex_line(v2 p0, v2 p1, f32 z, f32 rad, u32 color)
{
  v2 line         = v2_sub(p1, p0);
  f32 line_length = v2_len(line);
  v2 line_pos     = v2_add(p0, v2_scale(line, 0.5f));
  v2 axis         = { 0.0f, -1.0f };
  f32 rot         = v2_get_angle(axis, line);
  v2 pos          = { line_pos.x, line_pos.y };
  v2 scale        = { rad, 0.5f * line_length };

  glex_rotated(pos, z, scale, rot, color);
}

static void glex_box(r3 box, u32 color)
{
  glex_color(color);

  glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_vertex(box.min.x, box.max.y, box.min.z);

  glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.min.x, box.max.y, box.max.z);
  glex_vertex(box.min.x, box.min.y, box.max.z);

  glex_vertex(box.min.x, box.max.y, box.max.z);
  glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_vertex(box.min.x, box.max.y, box.max.z);

  glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.min.z);

  glex_vertex(box.min.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.min.y, box.min.z);
  glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_vertex(box.max.x, box.min.y, box.max.z);
  glex_vertex(box.min.x, box.min.y, box.max.z);
  glex_vertex(box.min.x, box.min.y, box.min.z);

  glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.max.x, box.max.y, box.min.z);
  glex_vertex(box.max.x, box.max.y, box.max.z);
  glex_vertex(box.min.x, box.max.y, box.min.z);
  glex_vertex(box.min.x, box.max.y, box.max.z);
}

static void glex_rect(r2 rect, f32 z, u32 color)
{
  glex_color(color);
  glex_vertex(rect.min.x, rect.min.y, z);
  glex_vertex(rect.max.x, rect.min.y, z);
  glex_vertex(rect.max.x, rect.max.y, z);
  glex_vertex(rect.max.x, rect.max.y, z);
  glex_vertex(rect.min.x, rect.max.y, z);
  glex_vertex(rect.min.x, rect.min.y, z);
}

static gl_shader glex_new_target(const char* fragment_shader)
{
  gl_shader_desc shader_desc = {0};

  shader_desc.vs = glex_post_fx_vertex_shader;
  shader_desc.fs = fragment_shader;

  return gl_shader_create(shader_desc);
}

//static const char* fs_blur = GLSL(
//  in vec2 frag_uv;
//  out vec4 out_color;
//
//  uniform sampler2D tex;
//
//  const float offset = 1.0 / 700.0;
//
//  void main()
//  {
//    vec2 offsets[9] = vec2[](
//      vec2(-offset,  offset),     // top-left
//      vec2( 0.0f,    offset),     // top-center
//      vec2( offset,  offset),     // top-right
//      vec2(-offset,  0.0f),       // center-left
//      vec2( 0.0f,    0.0f),       // center-center
//      vec2( offset,  0.0f),       // center-right
//      vec2(-offset, -offset),     // bottom-left
//      vec2( 0.0f,   -offset),     // bottom-center
//      vec2( offset, -offset));    // bottom-right    
//
//    float kernel[9] = float[](
//      1.0 / 16, 2.0 / 16, 1.0 / 16,
//      2.0 / 16, 4.0 / 16, 2.0 / 16,
//      1.0 / 16, 2.0 / 16, 1.0 / 16);
//
//    vec4 sample_tex[9];
//    for(int i = 0; i < 9; i++)
//    {
//      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
//    }
//    vec4 col = vec4(0.0);
//    for(int i = 0; i < 9; i++)
//      col += sample_tex[i] * kernel[i];
//
//    out_color = col;
//  });
//
//static const char* fs_edge = GLSL(
//  in vec2 frag_uv;
//  out vec4 out_color;
//
//  uniform sampler2D tex;
//  const float offset = 1.0 / 800.0;  
//
//  void main()
//  {
//    vec2 offsets[9] = vec2[](
//      vec2(-offset,  offset),     // top-left
//      vec2( 0.0f,    offset),     // top-center
//      vec2( offset,  offset),     // top-right
//      vec2(-offset,  0.0f),       // center-left
//      vec2( 0.0f,    0.0f),       // center-center
//      vec2( offset,  0.0f),       // center-right
//      vec2(-offset, -offset),     // bottom-left
//      vec2( 0.0f,   -offset),     // bottom-center
//      vec2( offset, -offset));    // bottom-right    
//
//    float kernel[9] = float[](
//      1,  1, 1,
//      1, -8, 1,
//      1,  1, 1
//    );
//
//    vec4 sample_tex[9];
//    for(int i = 0; i < 9; i++)
//    {
//      sample_tex[i] = texture(tex, frag_uv.st + offsets[i]);
//    }
//    vec4 col = vec4(0.0);
//    for(int i = 0; i < 9; i++)
//    {
//      col += sample_tex[i] * kernel[i];
//    }
//    if (col.a > 0)
//    {
//      out_color = vec4(0, 0, 0, 0.7);
//    }
//    else
//    {
//      out_color = texture(tex, frag_uv.st);
//    }
//  });

