#pragma once

#include "ats_base.h"

typedef struct gl_texture gl_texture;
struct gl_texture
{
  u32 id;
  u16 width;
  u16 height;
};

typedef struct gl_shader gl_shader;
struct gl_shader
{
  u32 id;
};

typedef struct gl_shader_desc gl_shader_desc;
struct gl_shader_desc
{
  const char* vs;
  const char* fs;
};

typedef struct gl_buffer gl_buffer;
struct gl_buffer
{
  u32 vao;
  u32 vbo;
};

typedef struct gl_layout gl_layout;
struct gl_layout
{
  u32 size;
  u32 type;
  u32 stride;
  u32 offset;
  u32 normalize;
};

typedef struct gl_buffer_desc gl_buffer_desc;
struct gl_buffer_desc
{
  gl_layout layout[32];
};

static gl_texture gl_texture_create(const void *pixels, u16 width, u16 height, b8 is_smooth)
{
  assert(pixels);

  gl_texture texture = {0};

  texture.width = width;
  texture.height = height;

  glGenTextures(1, &texture.id);

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
}

static gl_texture gl_texture_load_from_file(const char* texture_path, b8 is_smooth)
{
  u16 width  = 0;
  u16 height = 0;
  u32* pixels = file_load_image(texture_path, &width, &height);

  gl_texture texture = gl_texture_create(pixels, width, height, is_smooth);
  file_free_image(pixels);

  return texture;
}

static void gl_texture_destroy(gl_texture* texture)
{
  glDeleteTextures(1, &texture->id);
  memset(texture, 0, sizeof *texture);
}

static v3 gl_get_world_position(i32 x, i32 y, m4 in_projection, m4 in_modelview)
{
  int viewport[4]    = {0};
  f64 modelview[16]  = {0};
  f64 projection[16] = {0};

  GLfloat win_x, win_y, win_z;

  for (u32 i = 0; i < 16; ++i) projection[i] = in_projection.e[i];
  for (u32 i = 0; i < 16; ++i) modelview[i] = in_modelview.e[i];

  glGetIntegerv(GL_VIEWPORT, viewport);

  win_x = (f64)x;
  win_y = (f64)viewport[3] - (f64)y;

  glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

  f64 result[3];
  f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);

  return v3((f32)result[0], (f32)result[1], (f32)result[2]);
}

static v3 gl_get_screen_position(f32 x, f32 y, f32 z, m4 mvp)
{
  v4 clip_space = m4_mulv(mvp, v4(x, y, z, 1.0));
  v3 ndc = v3_scale(clip_space.xyz, (1.0 / clip_space.w));
  return v3(
    (0.5f * (ndc.x + 1.0f)) * platform.width,
    (0.5f * (1.0f - ndc.y)) * platform.height,
    ndc.z);
}

static void gl_init_bitmap_font(void);

static void gl_init(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
  glClearDepth(1.0f);

  glDepthFunc(GL_LESS);

  glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  gl_init_bitmap_font();
}

static u32 gl_shader_compile(const char* source, u32 type)
{
  char log[512] = {0};
  i32 success = 0;
  u32 shader = glCreateShader(type);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, log);
    puts(log);
    exit(EXIT_FAILURE);
  }

  return shader;
}

static u32 gl_shader_link_program(u32 vertex_shader, u32 fragment_shader)
{
  char log[512] = {0};
  i32 success = 0;
  u32 shader = glCreateProgram();

  glAttachShader(shader, vertex_shader);
  glAttachShader(shader, fragment_shader);
  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shader, 512, NULL, log);
    puts(log);
    exit(EXIT_FAILURE);
  }

  return shader;
}

static gl_shader gl_shader_create(gl_shader_desc desc)
{
  u32 vertex   = gl_shader_compile(desc.vs, GL_VERTEX_SHADER);
  u32 fragment = gl_shader_compile(desc.fs, GL_FRAGMENT_SHADER);
  u32 program  = gl_shader_link_program(vertex, fragment);

  glUseProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  gl_shader shader = {0};
  shader.id = program;
  return shader;
}

static gl_shader gl_shader_load_from_file(const char *vs, const char *fs)
{
  gl_shader shader = {0};

  mem_scope()
  {
    s8 vs_content = file_read_s8(vs);
    s8 fs_content = file_read_s8(fs);

    gl_shader_desc desc = {0};

    desc.vs = (char*)vs_content.buf;
    desc.fs = (char*)fs_content.buf;

    shader = gl_shader_create(desc);
  }

  return shader;
}

static void gl_use(const gl_shader* shader)
{
  glUseProgram(shader->id);
}

static u32 gl_location(const gl_shader* shader, const char* name)
{
  return glGetUniformLocation(shader->id, name);
}

static void gl_uniform_i32(u32 location, i32 i)
{
  glUniform1i(location, i);
}

static void gl_uniform_f32(u32 location, f32 f)
{
  glUniform1f(location, f);
}

static void gl_uniform_v2(u32 location, v2 u)
{
  glUniform2f(location, u.x, u.y);
}

static void gl_uniform_v3(u32 location, v3 u)
{
  glUniform3f(location, u.x, u.y, u.z);
}

static void gl_uniform_v4(u32 location, v4 u)
{
  glUniform4f(location, u.x, u.y, u.z, u.w);
}

static void gl_uniform_m2(u32 location, m2 m)
{
  glUniformMatrix2fv(location, 1, GL_FALSE, m.e);
}

static void gl_uniform_m3(u32 location, m3 m)
{
  glUniformMatrix3fv(location, 1, GL_FALSE, m.e);
}

static void gl_uniform_m4(u32 location, m4 m)
{
  glUniformMatrix4fv(location, 1, GL_FALSE, m.e);
}

static gl_buffer gl_buffer_create(const gl_buffer_desc* desc)
{
  u32 vao = 0;
  u32 vbo = 0;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  for (u32 i = 0; i < countof(desc->layout); ++i)
  {
    const gl_layout* layout = &desc->layout[i];

    if (layout->size)
    {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i, layout->size, layout->type, layout->normalize, layout->stride, (void*)(u64)layout->offset);
    }
  }

  gl_buffer result = {0};

  result.vao = vao;
  result.vbo = vbo;

  return result;
}

static void gl_buffer_bind(const gl_buffer* buffer)
{
  glBindVertexArray(buffer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
}

static void gl_buffer_send(const gl_buffer* buffer, const void* data, u32 size)
{
  glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

static void gl_texture_update(gl_texture* texture, const void *pixels, u16 width, u16 height, b8 is_smooth)
{
  texture->width  = width;
  texture->height = height;

  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);

  glGenerateMipmap(GL_TEXTURE_2D);
}

static void gl_texture_bind(const gl_texture* texture)
{
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

// ======================================= FONT ====================================== //

#define BITMAP_COUNT (256)

static const u64 bitascii[BITMAP_COUNT] =
{
  0x0000000000000000,
  0x7e8199bd81a5817e,
  0x7effe7c3ffdbff7e,
  0x00081c3e7f7f7f36,
  0x00081c3e7f3e1c08,
  0x1c086b7f7f1c3e1c,
  0x1c082a7f7f3e1c08,
  0x0000183c3c180000,
  0xffffe7c3c3e7ffff,
  0x003c664242663c00,
  0xffc399bdbd99c3ff,
  0x1e333333bef0e0f0,
  0x187e183c6666663c,
  0x070f0e0c0c7c6c7c,
  0x03377666667e667e,
  0x0018dbff3cffdb18,
  0x00061e7e7e1e0600,
  0x0060787e7e786000,
  0x183c7e18187e3c18,
  0x0036003636363636,
  0x00d8d8d8dedbdbfe,
  0x3e613c66663c867c,
  0x007e7e0000000000,
  0xff183c7e187e3c18,
  0x001818187e3c1800,
  0x00183c7e18181800,
  0x0010307e7e301000,
  0x00080c7e7e0c0800,
  0x00007e0606000000,
  0x002466ffff662400,
  0x007e7e3c3c181800,
  0x0018183c3c7e7e00,
  0x0000000000000000,
  0x00180018183c3c18,
  0x0000000000143636,
  0x0036367f367f3636,
  0x00183e603c067c18,
  0x0063660c18336300,
  0x006e333b6e1c361c,
  0x00000000000c1818,
  0x00180c0606060c18,
  0x00060c1818180c06,
  0x0000361c7f1c3600,
  0x000018187e181800,
  0x0c18180000000000,
  0x000000007e000000,
  0x0018180000000000,
  0x0003070e1c387060,
  0x003c666e7e76663c,
  0x007e181818181e18,
  0x007e660c3860663c,
  0x003c66603860663c,
  0x0030307f33363c38,
  0x003c6660603e067e,
  0x003c66663e060c38,
  0x000c0c183060667e,
  0x003c66663c66663c,
  0x001c30607c66663c,
  0x0018180018180000,
  0x0c18180018180000,
  0x0030180c060c1830,
  0x0000007e007e0000,
  0x00060c1830180c06,
  0x001800183060663c,
  0x003c06767676663c,
  0x0066667e66663c18,
  0x003f66663e66663f,
  0x003c66030303663c,
  0x003f36666666363f,
  0x007f46161e16467f,
  0x000f06161e16467f,
  0x007c66730303663c,
  0x006666667e666666,
  0x003c18181818183c,
  0x001e333330303078,
  0x006766361e366667,
  0x007f66460606060f,
  0x006363636b7f7763,
  0x006363737b6f6763,
  0x001c36636363361c,
  0x000f06063e66663f,
  0x00703c766666663c,
  0x0067361e3e66663f,
  0x003c6670380e663c,
  0x003c181818185a7e,
  0x007e666666666666,
  0x00183c6666666666,
  0x0063777f6b636363,
  0x006363361c366363,
  0x003c18183c666666,
  0x007f63460c19337f,
  0x003c0c0c0c0c0c3c,
  0x006070381c0e0703,
  0x003c30303030303c,
  0x0000000063361c08,
  0x7e00000000000000,
  0x0000000000301818,
  0x006e333e301e0000,
  0x003d6666663e0607,
  0x003c6606663c0000,
  0x006e33333e303038,
  0x003c067e663c0000,
  0x001e0c0c1e0c6c38,
  0x1f303e33336e0000,
  0x006766666e360607,
  0x003c1818181c0018,
  0x1c363030303c0030,
  0x0067361e36660607,
  0x003c18181818181c,
  0x0063636b7f370000,
  0x00666666663e0000,
  0x001e3333331e0000,
  0x0f063e66663b0000,
  0x78303e33336e0000,
  0x001e0c6c6c360000,
  0x003e603c067c0000,
  0x00182c0c0c3e0c08,
  0x006e333333330000,
  0x00183c6666660000,
  0x00367f6b63630000,
  0x0063361c36630000,
  0x3e607c6666660000,
  0x007e4c18327e0000,
  0x00380c0c060c0c38,
  0x0018181800181818,
  0x000e18183018180e,
  0x0000000000003b6e,
  0x007e66663c180000,
  0x060c1e330303331e,
  0x007e333333003300,
  0x003c067e663c1830,
  0x00fc667c603cc37e,
  0x007e333e301e0033,
  0x007e333e301e0c06,
  0x00fc667c603c663c,
  0x0c183c6606663c00,
  0x003c067e663cc37e,
  0x003c067e663c0066,
  0x003c067e663c180c,
  0x003c1818181c0066,
  0x003c1818181c633e,
  0x003c1818181c180c,
  0x00667e66663c1866,
  0x00667e663c182418,
  0x003f061e063f0c18,
  0x007e337e307e0000,
  0x007333337f33367c,
  0x003c66663c00663c,
  0x003c66663c006600,
  0x003c66663c00180c,
  0x007e33333300331e,
  0x007e333333000c06,
  0x3e607e6666006600,
  0x003e6363633e0063,
  0x003c666666660066,
  0x18187e03037e1818,
  0x003f67060f26361c,
  0x18187e187e3c6666,
  0x00721a321e22221e,
  0x000e1b187e18d870,
  0x007e333e301e0c18,
  0x003c1818181c1830,
  0x003c66663c003060,
  0x007e333333001830,
  0x003333331f003b6e,
  0x00333b3f37003b6e,
  0x00007e007c36363c,
  0x00007e003c66663c,
  0x003c66060c180018,
  0x000006067e000000,
  0x000060607e000000,
  0xf03366cc7b3363c3,
  0xc0f3f6ecdb3363c3,
  0x183c3c1818001800,
  0x0000cc663366cc00,
  0x00003366cc663300,
  0x8822882288228822,
  0x55aa55aa55aa55aa,
  0xeebbeebbeebbeebb,
  0x1818181818181818,
  0x1818181f1f181818,
  0x181f1f18181f1f18,
  0x6666666767666666,
  0x6666667f7f000000,
  0x181f1f18181f1f00,
  0x6667676060676766,
  0x6666666666666666,
  0x66676760607f7f00,
  0x007f7f6060676766,
  0x0000007f7f666666,
  0x001f1f18181f1f18,
  0x1818181f1f000000,
  0x000000f8f8181818,
  0x000000ffff181818,
  0x181818ffff000000,
  0x181818f8f8181818,
  0x000000ffff000000,
  0x181818ffff181818,
  0x18f8f81818f8f818,
  0x666666e6e6666666,
  0x00fefe0606e6e666,
  0x66e6e60606fefe00,
  0x00ffff0000e7e766,
  0x66e7e70000ffff00,
  0x66e6e60606e6e666,
  0x00ffff0000ffff00,
  0x66e7e70000e7e766,
  0x00ffff0000ffff18,
  0x000000ffff666666,
  0x18ffff0000ffff00,
  0x666666ffff000000,
  0x000000fefe666666,
  0x00f8f81818f8f818,
  0x18f8f81818f8f800,
  0x666666fefe000000,
  0x666666ffff666666,
  0x18ffff1818ffff18,
  0x0000001f1f181818,
  0x181818f8f8000000,
  0xffffffffffffffff,
  0xffffffff00000000,
  0x0f0f0f0f0f0f0f0f,
  0xf0f0f0f0f0f0f0f0,
  0x00000000ffffffff,
  0x00006e3b3b6e0000,
  0x000c3e663e663c00,
  0x0000060606667e00,
  0x0066363636367f00,
  0x007e460c0c467e00,
  0x001c3636367c0000,
  0x03063e6666660000,
  0x001818183b6e0000,
  0x007e183c663c187e,
  0x001c36363e36361c,
  0x007736366363361c,
  0x003c66663c180c38,
  0x00003e6b6b3e0000,
  0x03063e6b6b3e1830,
  0x00380c063e060c38,
  0x0066666666663c00,
  0x00007e007e007e00,
  0x003f000c0c3f0c0c,
  0x003f00060c180c06,
  0x003f00180c060c18,
  0x0606060636361c00,
  0x001c363630303030,
  0x001818007e001818,
  0x00003b6e003b6e00,
  0x000000001c36361c,
  0x0000001818000000,
  0x0000001800000000,
  0x00181c1612101070,
  0x000000003636361e,
  0x000000003c18301c,
  0x007e7e7e7e7e7e00,
  0x007e424242427e00
};

typedef struct bitmap_vertex bitmap_vertex;
struct bitmap_vertex
{
  f32 pos[2];
  f32 uv[2];
  u32 color;
};

static gl_texture bitmap_texture;
static gl_shader  bitmap_shader;
static gl_buffer  bitmap_buffer;

static u32 bitmap_count;
static bitmap_vertex bitmap_array[1024 * 1024];

static void gl_init_bitmap_font(void)
{
  gl_shader_desc shader_desc = {0};

  shader_desc.vs = GLSL(
    layout (location = 0) in vec2 in_pos;
    layout (location = 1) in vec2 in_uv;
    layout (location = 2) in vec4 in_color;

    out vec2 frag_uv;
    out vec4 frag_color;

    uniform mat4 mvp;

    void main() {
      frag_color  = in_color;
      frag_uv     = in_uv;
      gl_Position = mvp * vec4(in_pos, 0, 1);
    });

  shader_desc.fs = GLSL(
    in vec2 frag_uv;
    in vec4 frag_color;

    out vec4 out_color;

    uniform sampler2D tex;

    void main() {
      out_color = frag_color * texture(tex, frag_uv / textureSize(tex, 0));
    });

  bitmap_shader = gl_shader_create(shader_desc);

  gl_buffer_desc buffer_desc = {0};

  buffer_desc.layout[0] = make(gl_layout) { 2, GL_FLOAT, sizeof (bitmap_vertex), offsetof(bitmap_vertex, pos) };
  buffer_desc.layout[1] = make(gl_layout) { 2, GL_FLOAT, sizeof (bitmap_vertex), offsetof(bitmap_vertex, uv) };
  buffer_desc.layout[2] = make(gl_layout) { 4, GL_UNSIGNED_BYTE, sizeof (bitmap_vertex), offsetof(bitmap_vertex, color), true };

  bitmap_buffer = gl_buffer_create(&buffer_desc);

  // create and set font texture!
  {
    u32 pixels[8][BITMAP_COUNT * 8] = {0};
    for (i32 i = 0; i < BITMAP_COUNT; ++i)
    {
      for (i32 y = 0; y < 8; ++y)
      {
        for (i32 x = 0; x < 8; ++x)
        {
          u64 bit = y * 8 + x;
          if (bitascii[i] & (1ull << bit))
          {
            pixels[7 - y][8 * i + x] = 0xffffffff;
          }
        }
      }
    }

    bitmap_texture = gl_texture_create(pixels, BITMAP_COUNT * 8, 8, false);
  }
}

static void gl_ascii(int c, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color)
{
  r2 tex_rect = { { c * 8.0f + 0.1f, 0.1f }, { c * 8.0f + 7.9f, 7.9f } };
  r2 rect = { { x, y }, { x + sx, y + sy } };

  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.min.x, rect.min.y, tex_rect.min.x, tex_rect.max.y, color };
  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.max.x, rect.min.y, tex_rect.max.x, tex_rect.max.y, color };
  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.max.x, rect.max.y, tex_rect.max.x, tex_rect.min.y, color };
  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.max.x, rect.max.y, tex_rect.max.x, tex_rect.min.y, color };
  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.min.x, rect.max.y, tex_rect.min.x, tex_rect.min.y, color };
  bitmap_array[bitmap_count++] = make(bitmap_vertex) { rect.min.x, rect.min.y, tex_rect.min.x, tex_rect.max.y, color };
}

static void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color)
{
  bitmap_count = 0;

  gl_use(&bitmap_shader);
  glActiveTexture(GL_TEXTURE0);
  gl_texture_bind(&bitmap_texture);
  gl_uniform_m4(gl_location(&bitmap_shader, "mvp"), m4_ortho(0, platform.width, platform.height, 0, -1, 1));

  for (int i = 0; str[i] != '\0'; i++)
  {
    gl_ascii(str[i], x + i * sx, y, z, sx, sy, color);
  }

  gl_buffer_bind(&bitmap_buffer);
  gl_buffer_send(&bitmap_buffer, bitmap_array, bitmap_count * sizeof (bitmap_vertex));
  glDrawArrays(GL_TRIANGLES, 0, bitmap_count);
  glUseProgram(0);

  bitmap_count = 0;
}

static void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...)
{
  va_list list;
  char buffer[256];

  va_start(list, fmt);
  vsnprintf(buffer, 256, fmt, list);
  gl_string(buffer, x, y, z, sx, sy, color);
  va_end(list);
}

static void gl_timer_print_result(f32 px, f32 py, f32 sx, f32 sy, u32 color)
{
  int y = 0;
  // @TODO: fix render order within scopes!
  for (int i = timer_count - 1; i >= 0; --i)
  {
    timer_entry e = timer_array[i];
    gl_string_format(px + 2 * sx * e.depth, py + y * (sy + 1), 0, sx, sy, color, "%s : %.2f", e.name, 1000.0 * (e.stop - e.start));
    y++;
  }
  timer_reset_all();
}

