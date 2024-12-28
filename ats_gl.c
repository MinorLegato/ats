#pragma once

#include "ats_base.h"

typedef struct gl_texture gl_texture;
struct gl_texture {
    u32 id;
    u16 width;
    u16 height;
};

static gl_texture gl_bitmap_texture;

// axis used for billboards:
static v3 gl_forward = { 0, 1, 0 };
static v3 gl_right = { 1, 0, 0 };
static v3 gl_up = { 0, 0, 1 };

static gl_texture gl_texture_create(const void *pixels, u16 width, u16 height, b32 is_smooth) {
    assert(pixels);

    gl_texture texture = {0};

    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    return texture;
}

static gl_texture gl_texture_load_from_file(const char* texture_path, b32 is_smooth) {
    u16 width  = 0;
    u16 height = 0;
    u32* pixels = file_load_image(texture_path, &width, &height);

    gl_texture texture = gl_texture_create(pixels, width, height, is_smooth);
    file_free_image(pixels);

    return texture;
}

static void gl_texture_update(gl_texture* texture, const void* pixels, u16 width, u16 height, b32 is_smooth) {
    texture->width  = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
}

static void gl_texture_bind(const gl_texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);
}

static void gl_texture_destroy(gl_texture* texture) {
    glDeleteTextures(1, &texture->id);
    memset(texture, 0, sizeof *texture);
}

static v3 gl_get_world_position(i32 x, i32 y, m4 in_projection, m4 in_modelview) {
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

static v3 gl_get_screen_position(f32 x, f32 y, f32 z, m4 mvp) {
    v4 clip_space = m4_mulv(mvp, v4(x, y, z, 1.0));
    v3 ndc = v3_scale(clip_space.xyz, (1.0 / clip_space.w));
    return v3(
        (0.5f * (ndc.x + 1.0f)) * platform.width,
        (0.5f * (1.0f - ndc.y)) * platform.height,
        ndc.z);
}

// ------------------------------------- opengl impl ------------------------------------ //

#define BITMAP_COUNT (256)

static const u64 bitascii[BITMAP_COUNT] = {
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

#include <stdio.h>
#include <stdarg.h>

static void gl_init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClearDepth(1.0f);

    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_NORMALIZE);

    // init bitmap font
    {
        u32 pixels[8 + 16][BITMAP_COUNT * 8 + 512] = {0};
        for (i32 i = 0; i < BITMAP_COUNT; ++i) {
            for (i32 y = 0; y < 8; ++y) {
                for (i32 x = 0; x < 8; ++x) {
                    u64 bit = y * 8 + x;
                    if (bitascii[i] & (1ull << bit)) {
                        pixels[7 - y][(8 + 2) * i + x] = 0xffffffff;
                    }
                }
            }
        }
        gl_bitmap_texture = gl_texture_create(pixels, BITMAP_COUNT * 8 + 512, 8 + 16, 0);
    }
}

static void gl_set_axis(v3 forward, v3 right, v3 up) {
    gl_forward = v3_norm(forward);
    gl_right = v3_norm(right);
    gl_up = v3_norm(up);
}

static void gl_set_simple_light_emitter(i32 index, f32 bright, f32 x, f32 y, f32 z) {
    f32 pos[4] = { x, y, z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4] = { bright, bright, bright, 0.0f };
    f32 light = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE, c);
    glLightfv(light, GL_AMBIENT, zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_simple_light_directed(i32 index, f32 bright, f32 x, f32 y, f32 z) {
    f32 d       = (f32)(1.0f / sqrt32(x * x + y * y + z * z));
    f32 dir[4]  = { x * d, y * d, z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { bright, bright, bright, 0.0f };
    i32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE, c);
    glLightfv(light, GL_AMBIENT, zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_emitter(i32 index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic) {
    f32 pos[4]  = { p.x, p.y, p.z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { color.r, color.g, color.b, 0.0f };
    i32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glLightf(light, GL_CONSTANT_ATTENUATION, constant);
    glLightf(light, GL_LINEAR_ATTENUATION, linear);
    glLightf(light, GL_QUADRATIC_ATTENUATION, quadratic);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_directed(i32 index, v3 pos, v3 color) {
    f32 d       = (f32)(1.0f / sqrt32(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));
    f32 dir[4]  = { pos.x * d, pos.y * d, pos.z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { color.r, color.g, color.b, 0.0f };
    i32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE, c);
    glLightfv(light, GL_AMBIENT, zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_global_ambient(f32 r, f32 g, f32 b) {
    f32 v[4] = { r, g, b, 0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

static void gl_begin(u32 type) {
    glBegin(type);
}

static void gl_end(void) {
    glEnd();
}

static void gl_color(u32 color) {
    glColor4ubv((const u8*)&color);
}

static void gl_normal(f32 x, f32 y, f32 z) {
    glNormal3f(x, y, z);
}

static void gl_uv(f32 x, f32 y) {
    glTexCoord2f(x, y);
}

static void gl_vertex(f32 x, f32 y, f32 z) {
    glVertex3f(x, y, z);
}

static void gl_set_matrix(m4 projection, m4 view) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.e);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(view.e);
}

static void gl_billboard(tex_rect tex, v3 pos, v2 rad, u32 color) {
    f32 ax = pos.x - gl_right.x * rad.x - gl_up.x * rad.y;
    f32 ay = pos.y - gl_right.y * rad.x - gl_up.y * rad.y;
    f32 az = pos.z - gl_right.z * rad.x - gl_up.z * rad.y;

    f32 bx = pos.x + gl_right.x * rad.x - gl_up.x * rad.y;
    f32 by = pos.y + gl_right.y * rad.x - gl_up.y * rad.y;
    f32 bz = pos.z + gl_right.z * rad.x - gl_up.z * rad.y;

    f32 cx = pos.x + gl_right.x * rad.x + gl_up.x * rad.y;
    f32 cy = pos.y + gl_right.y * rad.x + gl_up.y * rad.y;
    f32 cz = pos.z + gl_right.z * rad.x + gl_up.z * rad.y;

    f32 dx = pos.x - gl_right.x * rad.x + gl_up.x * rad.y;
    f32 dy = pos.y - gl_right.y * rad.x + gl_up.y * rad.y;
    f32 dz = pos.z - gl_right.z * rad.x + gl_up.z * rad.y;

    gl_color(color);
    gl_normal(-gl_forward.x, -gl_forward.y, -gl_forward.z);

    gl_uv(tex.min_x, tex.max_y); gl_vertex(ax, ay, az);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(bx, by, bz);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(cx, cy, cz);

    gl_uv(tex.max_x, tex.min_y); gl_vertex(cx, cy, cz);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(dx, dy, dz);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(ax, ay, az);
}

static void gl_texture_box(tex_rect tex, r3 box, u32 color) {
    gl_color(color);

    gl_normal(0, 0, -1);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.min.x, box.max.y, box.min.z);

    gl_normal(0, 0, +1);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.max.z);

    gl_normal(-1, 0, 0);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);

    gl_normal(+1, 0, 0);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.max.x, box.max.y, box.min.z);

    gl_normal(0, -1, 0);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.min.y, box.min.z);

    gl_normal(0, +1, 0);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(box.min.x, box.max.y, box.max.z);
}

static void gl_texture_rect(tex_rect tex, r2 rect, f32 z, u32 color) {
    gl_color(color);
    gl_normal(0, 0, +1);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(rect.max.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(rect.min.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
}

static void gl_texture_rect_flip(tex_rect tex, r2 rect, f32 z, u32 color, b32 flip_x, b32 flip_y) {
    if (flip_x) { swap(u16, tex.min_x, tex.max_x); }
    if (flip_y) { swap(u16, tex.min_y, tex.max_y); }

    gl_color(color);
    gl_normal(0, 0, +1);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(rect.max.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(rect.min.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
}

static void gl_box(r3 box, u32 color) {
    gl_color(color);

    gl_normal(0, 0, -1);
    gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_vertex(box.min.x, box.max.y, box.min.z);

    gl_normal(0, 0, +1);
    gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.min.x, box.max.y, box.max.z);
    gl_vertex(box.min.x, box.min.y, box.max.z);

    gl_normal(-1, 0, 0);
    gl_vertex(box.min.x, box.max.y, box.max.z);
    gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_vertex(box.min.x, box.max.y, box.max.z);

    gl_normal(+1, 0, 0);
    gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.min.z);

    gl_normal(0, -1, 0);
    gl_vertex(box.min.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.min.y, box.min.z);
    gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_vertex(box.max.x, box.min.y, box.max.z);
    gl_vertex(box.min.x, box.min.y, box.max.z);
    gl_vertex(box.min.x, box.min.y, box.min.z);

    gl_normal(0, +1, 0);
    gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.max.x, box.max.y, box.min.z);
    gl_vertex(box.max.x, box.max.y, box.max.z);
    gl_vertex(box.min.x, box.max.y, box.min.z);
    gl_vertex(box.min.x, box.max.y, box.max.z);
}

static void gl_rect(r2 rect, f32 z, u32 color) {
    gl_color(color);
    gl_normal(0, 0, +1);
    gl_vertex(rect.min.x, rect.min.y, z);
    gl_vertex(rect.max.x, rect.min.y, z);
    gl_vertex(rect.max.x, rect.max.y, z);
    gl_vertex(rect.max.x, rect.max.y, z);
    gl_vertex(rect.min.x, rect.max.y, z);
    gl_vertex(rect.min.x, rect.min.y, z);
}

// ======================================= FONT ====================================== //

static void gl_ascii(int c, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    tex_rect tex = {
        (u16)(c * (8 + 2)),
        0,
        (u16)(c * (8 + 2) + 8),
        8
    };

    r2 rect = { { x, y }, { x + sx, y + sy } };

    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.max_y); gl_vertex(rect.max.x, rect.min.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.max_x, tex.min_y); gl_vertex(rect.max.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.min_y); gl_vertex(rect.min.x, rect.max.y, z);
    gl_uv(tex.min_x, tex.max_y); gl_vertex(rect.min.x, rect.min.y, z);
}

static void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gl_bitmap_texture.id);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0 / gl_bitmap_texture.width, 1.0 / gl_bitmap_texture.height, 1.0);

    gl_begin(GL_TRIANGLES);
    gl_color(color);
    gl_normal(0, 0, 1);
    for (i32 i = 0; str[i] != '\0'; i++) {
        gl_ascii(str[i], x + i * sx, y, z, sx, sy);
    }
    gl_end();

    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}

static void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...) {
    va_list list;
    char buffer[256];

    va_start(list, fmt);
    vsnprintf(buffer, 256, fmt, list);
    va_end(list);
    gl_string(buffer, x, y, z, sx, sy, color);
}

static void gl_timer_print_result(f32 px, f32 py, f32 sx, f32 sy, u32 color) {
    int y = 0;
    // @TODO: fix render order within scopes!
    for (int i = timer_count - 1; i >= 0; --i) {
        timer_entry e = timer_array[i];
        gl_string_format(px + 2 * sx * e.depth, py + y * (sy + 1), 0, sx, sy, color, "%s : %.2f", e.name, 1000.0 * (e.stop - e.start));
        y++;
    }
    timer_reset_all();
}

