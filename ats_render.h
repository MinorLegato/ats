#pragma once

#include "ats_tool.h"

#include <stdint.h>
#include <stdarg.h>

static int cube_list;
static int square_list;

static void BitmapInit();

static void RenderBeginShapes(void)
{
    square_list = glGenLists(1);
    cube_list   = glGenLists(1);

    {
        glNewList(square_list, GL_COMPILE);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, +1.0f, 0.0f);
        glVertex3f(-1.0f, +1.0f, 0.0f);

        glEnd();

        glEndList();
    }

    {
        glNewList(cube_list, GL_COMPILE);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, -1.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);

        glNormal3f(0.0f, -1.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);
        
        glNormal3f(1.0f, 0.0f, 0.0f);

        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);

        glEnd();

        glEndList();
    }
}

static void RenderInit(void)
{
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

    RenderBeginShapes();
    
    BitmapInit();
}

static void RenderClear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void RenderSetMatrix(const float *matrix) 
{
    glLoadMatrixf(matrix);
}

static void RenderSetProjectionView(const float *projection_view)
{
    glMatrixMode(GL_PROJECTION);
    RenderSetMatrix(projection_view);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void RenderSetModel(float px, float py, float pz, float sx, float sy, float sz)
{
    float T[16];
    Float4x4TranslateScale(T, px, py, pz, sx, sy, sz);
    RenderSetMatrix(T);
}

static void RenderSetRotatedModel(float px, float py, float pz, float sx, float sy, float sz, float rot)
{
    float T[16];
    Float4x4TranslateScaleRotateZ(T, px, py, pz, sx, sy, sz, rot);
    RenderSetMatrix(T);
}

static void RenderSetTranslateScaleRotateZ(
        float px, float py, float pz,
        float sx, float sy, float sz,
        float rot)
{
    float T[16];
    Float4x4TranslateScaleRotateZ(T, px, py, pz, sx, sy, sz, rot);
    RenderSetMatrix(T);
}

static void RenderSetCamera(
        float px,  float py, float pz,
        float tx,  float ty, float tz,
        float ux,  float uy, float uz,
        float fov, float aspect, float n,  float f)
{
    float P[16];
    float V[16];
    float PV[16];

    float pos[3]  = { px, py, pz };
    float look[3] = { tx, ty, tz };
    float up[3]   = { ux, uy, uz };

    Float4x4Perspective(P, TO_RAD(fov), aspect, n, f);
    Float4x4LookAt(V, pos, look, up);
    Float4x4Mul(PV, P, V);

    RenderSetProjectionView(PV);
}

static void RenderSetCamera2D(
        float px,  float py, float pz,
        float tx,  float ty, float tz,
        float ux,  float uy, float uz,
        float sx,  float sy,
        float n,  float f)
{
    float PV[16];

    float pos[3]  = { px, py, pz };
    float look[3] = { tx, ty, tz };
    float up[3]   = { ux, uy, uz };
    float rad[2]  = { sx, sy };

    Float4x4OrthoView(PV, pos, look, up, rad, n, f);

    RenderSetProjectionView(PV);
}

static void RenderSetCameraOrtho(float width, float height, float n, float f)
{
    float PV[16];

    Float4x4Ortho(PV, 0, width, height, 0, n, f);
    RenderSetProjectionView(PV);
}

static void RenderSetCameraTranslateOrtho(float x, float y, float rad_x, float rad_y, float n, float f)
{
    float PV[16];

    Float4x4Ortho(PV, x - rad_x, x + rad_x, y + rad_y, y - rad_y, n, f);
    RenderSetProjectionView(PV);
}

static void RenderSetColor(float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
}

static void RenderSquare(float x, float y, float z, float rad, float r, float g, float b, float a)
{
    RenderSetColor(r, g, b, a);
    RenderSetModel(x, y, z, rad, rad, rad);
    glCallList(square_list);
}

static void RenderRotatedSquare(float x, float y, float z, float rad, float rot, float r, float g, float b, float a)
{
    RenderSetColor(r, g, b, a);
    RenderSetRotatedModel(x, y, z, rad, rad, rad, rot);
    glCallList(square_list);
}

static void RenderCube(float x, float y, float z, float rad, float r, float g, float b, float a)
{
    RenderSetColor(r, g, b, a);
    RenderSetModel(x, y, z, rad, rad, rad);
    glCallList(cube_list);
}

static void RenderLine(float x0, float y0, float z0, float x1, float y1, float z1, float r, float g, float b, float a)
{
    RenderSetColor(r, g, b, a);

    glLoadIdentity();
    glBegin(GL_LINES);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y1, z1);

    glEnd();
}

static void RenderLineLoop2D(
        float pos_x, float pos_y, float pos_z,
        float rad, float rot,
        float red, float green, float blue, float alpha,
        const float (*point_array)[2], int point_count)
{
    glLoadIdentity();
    glTranslatef(pos_x, pos_y, pos_z);
    glScalef(rad, rad, 1.0f);
    glRotatef(TO_DEG(rot), 0.0f, 0.0f, 1.0f);

    glColor4f(red, green, blue, alpha);

    glBegin(GL_LINE_LOOP);

    glNormal3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < point_count; ++i) {
        const float* a = point_array[i];
        const float* b = point_array[(i + 1) & (point_count - 1)];

        glVertex3f(a[0], a[1], 0.0f);
        glVertex3f(b[0], b[1], 0.0f);
    }

    glEnd();
}

static void RenderSetLightEmitter(int index, float bright, float x, float y, float z)
{
    float    pos[4]  = { x, y, z, 1.0f };
    float    zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float    c[4]    = { bright, bright, bright, 0.0f };
    unsigned light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void RenderSetLightDirected(int index, float bright, float x, float y, float z)
{
    float    d       = (float)(1.0f / __builtin_sqrtf(x * x + y * y + z * z));
    float    dir[4]  = { x * d, y * d, z * d, 0.0f };
    float    zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float    c[4]    = { bright, bright, bright, 0.0f };
    unsigned light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void RenderSetLightGlobalAmbient(float r, float g, float b)
{
   float v[4] = { r, g, b, 0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

// ======================================= TEXTURES ======================================== //

#ifndef ATS_NO_TEXTURE
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb/stb_image.h" 

typedef struct {
    unsigned    id;
    int         width;
    int         height;
} Texture;

static Texture TextureCreate(unsigned char *pixels, int width, int height, int is_smooth)
{
    assert(pixels);

    Texture texture  = {0};

    texture.width   = width;
    texture.height  = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    return texture;
}

static Texture TextureLoadFromFile(const char *texture_path, int is_smooth)
{
    Texture         texture     = {0};
    int32_t         channels    = 0;
    unsigned char*  pixels      = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    stbi_image_free(pixels);

    return texture;
}

static void TextureBind(const Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);

    glMatrixMode(GL_MODELVIEW);
}

#endif

// ================================================= SLOW SIMPLE BITMAP FONT ======================================= //

#define BITMAP_COUNT    (256)

static const unsigned long long bitascii[BITMAP_COUNT] = {
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

static int bitmap_display_list[BITMAP_COUNT];

#define bitmap_getbit(N, X, Y) (((unsigned long long)(N)) & (1ull << (((unsigned long long)(Y)) * 8ull + ((unsigned long long)(X)))))

static void BitmapInit(void)
{
    for (int i = 0; i < BITMAP_COUNT; ++i) {
        bitmap_display_list[i] = glGenLists(1);

        glNewList(bitmap_display_list[i], GL_COMPILE);

        uint64_t c = bitascii[i];

        glBegin(GL_QUADS);

        float scale = 1.0f / 8.0f;

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < 8; ++i) {
                float x   = i * scale;
                float y   = j * scale;

                if (bitmap_getbit(c, i, j)) {
                    glVertex3f(x - 0, y - 0,  0.0f);
                    glVertex3f(x + scale, y - 0,  0.0f);
                    glVertex3f(x + scale, y + scale,  0.0f);
                    glVertex3f(x - 0, y + scale,  0.0f);
                }
            }
        }

        glEnd();

        glEndList();
    }
}

static void RenderAscii(unsigned char c, float px, float py, float pz, float x_scale, float y_scale, float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);

    float T[16];
    Float4x4TranslateScale(T, px, py, pz, x_scale, y_scale, 1.0f);
    RenderSetMatrix(T);

    glCallList(bitmap_display_list[c]);
}

static void RenderString(const char *str, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a)
{
    for (int i = 0; str[i] != '\0'; i++) {
        RenderAscii(str[i], x + i * scale_x, y, z, scale_x, scale_y, r, g, b, a);
    }
}

static void RenderStringFormat(float x, float y, float z, float rad_x, float rad_y, float r, float g, float b, float a, const char* fmt, ...)
{
    va_list list;
    char    buffer[256];

    va_start(list, fmt);
    vsnprintf(buffer, 256, fmt, list);
    RenderString(buffer, x, y, z, rad_x, rad_y, r, g, b, a);
    va_end(list);
}

static void RenderInt(int n, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a)
{
    char buffer[32];
    sprintf(buffer, "%d", n);
    RenderString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

static void RenderF32(float n, float x, float y, float z, float scale_x, float scale_y, float r, float g, float b, float a)
{
    char buffer[32];
    sprintf(buffer, "%.1f", n);
    RenderString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

static void RenderStringBox(const char* str, float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
    int   slen = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;
    
    for (int i = 0; i < slen; i++) {
        RenderAscii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, r, g, b, a);
    }
}

static void RenderNumberBox(int n, float x, float y, float z, float w, float h, float r, float g, float b, float a)
{
    char buffer[32];
    sprintf(buffer, "%d", n);
    RenderStringBox(buffer, x, y, z, w, h, r, g, b, a);
}

