#ifndef __ATS_H__
#define __ATS_H__

#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>

// ======================================== API-MACROS ======================================= //

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define PI (3.14159265359f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define ToRad(deg)          ((deg) * TO_RAD_MUL)
#define ToDeg(rad)          ((rad) * TO_DEG_MUL)

#define IsPowerOfTwo(x)     (((x) != 0) && ((x) & ((x)-1)) == 0)

#define AlignDown(n, a)     ((n) & ~((a) - 1))
#define AlignUp(n, a)       AlignDown((n) + (a) - 1, (a))
#define AlignDownPtr(p, a)  ((void*)AlignDown((uintptr_t)(p), (a)))
#define AlignUpPtr(p, a)    ((void*)AlignDown((uintptr_t)(p), (a)))

#define ArrayCount(array)   (sizeof (array) / sizeof ((array)[0]))

#define JoinHelper(a, b)    a##b
#define JoinToken(a, b)     JoinHelper(a, b)
#define MacroVar(a)         JoinToken(a, __LINE__)

#define Defer(start, end) for (int MacroVar(i) = ((start), 0); !MacroVar(i); (MacroVar(i)++, (end)))

#define Def(_val, _def) ((_val) == 0? (_def) : (_val))

#define ForR2(rect, ix, iy) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define ForR3(rect, ix, iy, iz) \
    for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define Min(a, b)           ((a) < (b)? (a) : (b))
#define Max(a, b)           ((a) > (b)? (a) : (b))
#define ClampMin(n, min)    ((n) < (min)? (min) : (n))
#define ClampMax(n, max)    ((n) > (max)? (max) : (n))
#define Clamp(n, lo, hi)    ClampMax(ClampMin(n, lo), hi)
#define Lerp(a, b, t)       ((a) + (t) * ((b) - (a)))
#define Sign(n)             ((n) == 0? 0 : ((n) < 0? -1 : 1))

// =========================================== API-TYPES ====================================== //

typedef float   f32;
typedef double  f64;

typedef char        i8;
typedef short       i16;
typedef int         i32;
typedef long long   i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef unsigned int        uint;
typedef long long           isize;
typedef unsigned long long  usize;

static_assert (sizeof (i8)  >= 1, "i8 -- not enough bits!");
static_assert (sizeof (i16) >= 2, "i16 -- not enough bits!");
static_assert (sizeof (i32) >= 4, "i32 -- not enough bits!");
static_assert (sizeof (i64) >= 8, "i64 -- not enough bits!");

static_assert (sizeof (u8)  >= 1, "u8 -- not enough bits!");
static_assert (sizeof (u16) >= 2, "u16 -- not enough bits!");
static_assert (sizeof (u32) >= 4, "u32 -- not enough bits!");
static_assert (sizeof (u64) >= 8, "u64 -- not enough bits!");

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

// =========================================== TYPES ========================================= //

union Vec2 {
    struct { f32 x, y; };

    f32 e[2];
};

union Vec3 {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { Vec2 xy; };

    f32 e[3];
};

union Vec4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { Vec3 rgb; };
    struct { Vec3 xyz; };

    f32 e[4];
};

union Vec2i {
    struct { i32 x, y; };

    i32 e[2];
};

union Vec3i {
    struct { i32 x, y, z; };
    struct { Vec2i xy; };

    i32 e[3];
};

union Vec4i {
    struct { i32 x, y, z, w; };

    i32 e[4];
};

union Mat2 {
    struct { Vec2 x, y; };
    f32 e[4];
};

union Mat3 {
    struct { Vec3 x, y, z; };
    f32 e[9];
};

union Mat4 {
    struct { Vec4 x, y, z, w; };
    f32 e[16];
};

struct Quat {
    f32 x, y, z, w;
};

struct Circle {
    Vec2    pos;
    f32     rad;
};

struct Sphere {
    Vec3    pos;
    f32     rad;
};

struct Rect2 {
    Vec2    min;
    Vec2    max;
};

struct Rect3 {
    Vec3    min;
    Vec3    max;
};

struct Rect2i {
    Vec2i   min;
    Vec2i   max;
};

struct Rect3i {
    Vec3i   min;
    Vec3i   max;
};

struct Plane {
    f32     a;
    f32     b;
    f32     c;
    f32     d;
};

struct Frustum {
    Plane   plane[6];
};

struct Memory_Arena {
    usize       index;
    usize       cap;
    u8*         buffer;

    usize       top;
    usize       stack[16];

    usize       lock;
    usize       max;
};

struct Image {
    int     width;
    int     height;

    u32*    pixels;
};

struct Queue_Node {
    f32     weight;
    Vec2i   e;
};

struct Priority_Queue {
    u32         len;
    Queue_Node* array;
};

// ---------------------- arena allocator ------------------------ //

extern Memory_Arena  ma_create(u8* buffer, usize size);

#define MaType(ma, T)           (T*)ma_alloc(ma, sizeof (T))
#define MaArray(ma, T, count)   (T*)ma_alloc(ma, (count) * sizeof (T))

extern void*    ma_alloc(Memory_Arena* ma, usize byte_size);
extern void*    ma_begin(Memory_Arena* ma);
extern void     ma_end(Memory_Arena* ma, usize byte_size);
extern void     ma_save(Memory_Arena* ma);
extern void     ma_restore(Memory_Arena* ma);
extern void     ma_validate(const Memory_Arena* ma);

// --------------------- read/write files ------------------------- //

extern char*    file_read_str(const char* file_name, Memory_Arena* ma);
extern b32      file_write_str(const char* file_name, const char* buffer);
extern b32      file_append_Str(const char* file_name, const char* buffer);

extern b32      file_read_bin(const char* file_name, void* buffer, u32 size);
extern b32      file_write_bin(const char* file_name, const void* buffer, u32 size);

// ----------------------- image -------------------------- //

extern Image    image_load_from_file(const char* path);
extern u32      get_pixel(Image* img, i32 x, i32 y);
extern void     set_pixel(Image* img, i32 x, i32 y, u32 pixel);

// ======================================= STATIC FUNCTIONS ==================================== //

static Vec2 v2(f32 x, f32 y)                        { return { x, y }; }
static Vec2 v2(f32 n)                               { return v2(n, n); }
static Vec2 v2(Vec3 u)                              { return v2(u.x, u.y); }
static Vec2 v2(Vec4 u)                              { return v2(u.x, u.y); }
static Vec2 v2(Vec2i u)                             { return v2(u.x, u.y); }

static Vec3 v3(f32 x, f32 y, f32 z)                 { return { x, y, z }; }
static Vec3 v3(f32 n)                               { return v3(n, n, n); }
static Vec3 v3(Vec2 u, f32 z = 0)                   { return v3(u.x, u.y, z); }
static Vec3 v3(Vec4 u)                              { return v3(u.x, u.y, u.z); }
static Vec3 v3(Vec2i u, f32 z = 0)                  { return v3(u.x, u.y, z); }
static Vec3 v3(Vec4i u)                             { return v3(u.x, u.y, u.z); }

static Vec4 v4(f32 x, f32 y, f32 z, f32 w)          { return { x, y, z, w }; }
static Vec4 v4(f32 n)                               { return v4(n, n, n, n); }
static Vec4 v4(Vec2 u, f32 z = 0, f32 w = 0)        { return v4(u.x, u.y, z, w); }
static Vec4 v4(Vec3 u, f32 w = 0)                   { return v4(u.x, u.y, u.z, w); }
static Vec4 v4(Vec2i u, f32 z = 0, f32 w = 0)       { return v4(u.x, u.y, z, w); }
static Vec4 v4(Vec3i u, f32 w = 0)                  { return v4(u.x, u.y, u.z, w); }

static Vec2i v2i(i32 x, i32 y)                      { return { x, y }; }
static Vec2i v2i(i32 n)                             { return v2i(n, n); }
static Vec2i v2i(Vec2i u)                           { return v2i(u.x, u.y); }
static Vec2i v2i(Vec4i u)                           { return v2i(u.x, u.y); }
static Vec2i v2i(Vec2 u)                            { return v2i(u.x, u.y); }
static Vec2i v2i(Vec4 u)                            { return v2i(u.x, u.y); }

static Vec3i v3i(i32 x, i32 y, i32 z)               { return { x, y, z }; }
static Vec3i v3i(i32 n)                             { return v3i(n, n, n); }
static Vec3i v3i(Vec3i u, i32 z = 0)                { return v3i(u.x, u.y, z); }
static Vec3i v3i(Vec4i u)                           { return v3i(u.x, u.y, u.z); }
static Vec3i v3i(Vec3 u, i32 z = 0)                 { return v3i(u.x, u.y, z); }
static Vec3i v3i(Vec4 u)                            { return v3i(u.x, u.y, u.z); }

static Vec4i v4i(i32 x, i32 y, i32 z, i32 w)        { return { x, y, z, w }; }
static Vec4i v4i(i32 n)                             { return v4i(n, n, n, n); }
static Vec4i v4i(Vec2i u, i32 z = 0, i32 w = 0)     { return v4i(u.x, u.y, z, w); }
static Vec4i v4i(Vec3i u, i32 w = 0)                { return v4i(u.x, u.y, u.z, w); }
static Vec4i v4i(Vec2 u, i32 z = 0, i32 w = 0)      { return v4i(u.x, u.y, z, w); }
static Vec4i v4i(Vec3 u, i32 w = 0)                 { return v4i(u.x, u.y, u.z, w); }

static Mat2 m2(void) {
    return {
        1, 0,
        0, 1,
    };
}

static Mat2 m2(f32 xx, f32 xy, f32 yx, f32 yy) {
    return {
        xx, xy,
        yx, yy,
    };
}

static Mat2 m2(Vec2 x, Vec2 y) {
    return {
        x.x, x.y,
        y.x, y.y,
    };
}

static Mat3 m3() {
    return {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    };
}

static Mat3 m3(f32 xx, f32 xy, f32 xz, f32 yx, f32 yy, f32 yz, f32 zx, f32 zy, f32 zz) {
    return {
        xx, xy, xz,
        yx, yy, yz,
        zx, zy, zz,
    };
}

static Mat3 m3(Vec3 x, Vec3 y, Vec3 z) {
    return {
        x.x, x.y, x.z,
        y.x, y.y, y.z,
        z.x, z.y, z.z
    };
}

static Mat4 m4() {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

static Mat4 m4(f32 xx, f32 xy, f32 xz, f32 xw,
               f32 yx, f32 yy, f32 yz, f32 yw,
               f32 zx, f32 zy, f32 zz, f32 zw,
               f32 wx, f32 wy, f32 wz, f32 ww) {
    return {
        xx, xy, xz, xw,
        yx, yy, yz, yw,
        zx, zy, zz, zw,
        wx, wy, wz, ww,
    };
}

static Mat4 m4(Vec4 x, Vec4 y, Vec4 z, Vec4 w) {
    return {
        x.x, x.y, x.z, x.w,
        y.x, y.y, y.z, y.w,
        z.x, z.y, z.z, z.w,
        w.x, w.y, w.z, w.w,
    };
}

static Rect2 r2(f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
    return { min_x, min_y, max_x, max_y };
}

static Rect2 r2(Vec2 min, Vec2 max) {
    return { min, max };
}

static Rect3 r3(f32 min_x, f32 min_y, f32 min_z, f32 max_x, f32 max_y, f32 max_z) {
    return { min_x, min_y, min_z, max_x, max_y, max_z };
}

static Rect3 r3(Vec3 min, Vec3 max) {
    return { min, max };
}

static Rect2i r2i(i32 min_x, i32 min_y, i32 max_x, i32 max_y) {
    return { min_x, min_y, max_x, max_y };
}

static Rect2i r2i(Vec2i min, Vec2i max) {
    return { min, max };
}

static Rect3i r3i(i32 min_x, i32 min_y, i32 min_z, i32 max_x, i32 max_y, i32 max_z) {
    return { min_x, min_y, min_z, max_x, max_y, max_z };
}

static Rect3i r3i(Vec3i min, Vec3i max) {
    return { min, max };
}

// ======================================= STATIC FUNCTIONS ==================================== //

static f32 sqrt32(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

static f32 rsqrt32(f32 n) {
	f32 x2 = n * 0.5f;
	f32 y  = n;
	int i  = *(long*)&y;           // evil floating point bit level hacking

	i = 0x5f3759df - (i >> 1);     // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y)); // 1st iteration

	return y;
}

static f32 shortest_angle_distance(f32 a, f32 b) {
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);

    return fmodf(2.0f * da, max) - da;
}

static f32 lerp_angle(f32 a, f32 b, f32 t) {
    return a + shortest_angle_distance(a, b) * t;
}

// ---------- from array ---------- //

static Vec2 v2(const f32* a) { return v2(a[0], a[1]); }
static Vec3 v3(const f32* a) { return v3(a[0], a[1], a[2]); }
static Vec4 v4(const f32* a) { return v4(a[0], a[1], a[2], a[3]); }

static Vec2i v2i(const i32* a) { return v2i(a[0], a[1]); }
static Vec3i v3i(const i32* a) { return v3i(a[0], a[1], a[2]); }
static Vec4i v4i(const i32* a) { return v4i(a[0], a[1], a[2], a[3]); }

// ---------- unpack color ------------ //

static Vec3 unpack_color3(u32 color) {
    return v3(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f);
}

static Vec4 unpack_color4(u32 color) {
    return v4(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f,
              ((color & 0xff000000) >> 24) / 256.0f);
}

// --------- negate ---------- //

static Vec2 operator-(Vec2 u) { return v2(-u.x, -u.y); }
static Vec3 operator-(Vec3 u) { return v3(-u.x, -u.y, -u.z); }
static Vec4 operator-(Vec4 u) { return v4(-u.x, -u.y, -u.z, -u.w); }

static Vec2i operator-(Vec2i u) { return v2i(-u.x, -u.y); }
static Vec3i operator-(Vec3i u) { return v3i(-u.x, -u.y, -u.z); }
static Vec4i operator-(Vec4i u) { return v4i(-u.x, -u.y, -u.z, -u.w); }

// ---------- addition ---------- //

static Vec2 operator+(Vec2 a, Vec2 b) { return v2(a.x + b.x, a.y + b.y); }
static Vec3 operator+(Vec3 a, Vec3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static Vec4 operator+(Vec4 a, Vec4 b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static Vec2i operator+(Vec2i a, Vec2i b) { return v2i(a.x + b.x, a.y + b.y); }
static Vec3i operator+(Vec3i a, Vec3i b) { return v3i(a.x + b.x, a.y + b.y, a.z + b.z); }
static Vec4i operator+(Vec4i a, Vec4i b) { return v4i(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static Vec2 operator+=(Vec2& a, Vec2 b) { return a = a + b; }
static Vec3 operator+=(Vec3& a, Vec3 b) { return a = a + b; }
static Vec4 operator+=(Vec4& a, Vec4 b) { return a = a + b; }

static Vec2i operator+=(Vec2i& a, Vec2i b) { return a = a + b; }
static Vec3i operator+=(Vec3i& a, Vec3i b) { return a = a + b; }
static Vec4i operator+=(Vec4i& a, Vec4i b) { return a = a + b; }

// -------- subtraction ------- //

static Vec2 operator-(Vec2 a, Vec2 b) { return v2(a.x - b.x, a.y - b.y); }
static Vec3 operator-(Vec3 a, Vec3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static Vec4 operator-(Vec4 a, Vec4 b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static Vec2i operator-(Vec2i a, Vec2i b) { return v2i(a.x - b.x, a.y - b.y); }
static Vec3i operator-(Vec3i a, Vec3i b) { return v3i(a.x - b.x, a.y - b.y, a.z - b.z); }
static Vec4i operator-(Vec4i a, Vec4i b) { return v4i(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static Vec2 operator-=(Vec2& a, Vec2 b) { return a = a - b; }
static Vec3 operator-=(Vec3& a, Vec3 b) { return a = a - b; }
static Vec4 operator-=(Vec4& a, Vec4 b) { return a = a - b; }

static Vec2i operator-=(Vec2i& a, Vec2i b) { return a = a - b; }
static Vec3i operator-=(Vec3i& a, Vec3i b) { return a = a - b; }
static Vec4i operator-=(Vec4i& a, Vec4i b) { return a = a - b; }

// -------- multiplication ------- //

static Vec2 operator*(Vec2 a, Vec2 b) { return v2(a.x * b.x, a.y * b.y); }
static Vec3 operator*(Vec3 a, Vec3 b) { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
static Vec4 operator*(Vec4 a, Vec4 b) { return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * a.w); }

static Vec2i operator*(Vec2i a, Vec2i b) { return v2i(a.x * b.x, a.y * b.y); }
static Vec3i operator*(Vec3i a, Vec3i b) { return v3i(a.x * b.x, a.y * b.y, a.z * a.z); }
static Vec4i operator*(Vec4i a, Vec4i b) { return v4i(a.x * b.x, a.y * b.y, a.z * a.z, a.w * a.w); }

static Vec2 operator*=(Vec2& a, Vec2 b) { return a = a * b; }
static Vec3 operator*=(Vec3& a, Vec3 b) { return a = a * b; }
static Vec4 operator*=(Vec4& a, Vec4 b) { return a = a * b; }

static Vec2i operator*=(Vec2i& a, Vec2i b) { return a = a * b; }
static Vec3i operator*=(Vec3i& a, Vec3i b) { return a = a * b; }
static Vec4i operator*=(Vec4i& a, Vec4i b) { return a = a * b; }

static Vec2 operator*(Mat2 m, Vec2 u) {
    return {
        m.e[0] * u.x + m.e[2] * u.y,
        m.e[1] * u.x + m.e[3] * u.y
    };
}

static Vec3 operator*(const Mat3& m, Vec3 u) {
    return {
        m.e[0] * u.x + m.e[3] * u.y + m.e[6] * u.z,
        m.e[1] * u.x + m.e[4] * u.y + m.e[7] * u.z,
        m.e[2] * u.x + m.e[5] * u.y + m.e[8] * u.z
    };
}

static Vec4 operator*(const Mat4& m, Vec4 u) {
    return {
        m.e[0] * u.x + m.e[4] * u.y + m.e[8]  * u.z + m.e[12] * u.w,
        m.e[1] * u.x + m.e[5] * u.y + m.e[9]  * u.z + m.e[13] * u.w,
        m.e[2] * u.x + m.e[6] * u.y + m.e[10] * u.z + m.e[14] * u.w,
        m.e[3] * u.x + m.e[7] * u.y + m.e[11] * u.z + m.e[15] * u.w
    };
}

static Mat2 operator*(Mat2 a, Mat2 b) {
    return {
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3]
    };
}

static Mat3 operator*(const Mat3& a, const Mat3& b) {
    return {
        a.e[0] * b.e[0] + a.e[3] * b.e[1]  + a.e[6] * b.e[2],
        a.e[1] * b.e[0] + a.e[4] * b.e[1]  + a.e[7] * b.e[2],
        a.e[2] * b.e[0] + a.e[5] * b.e[1]  + a.e[8] * b.e[2],

        a.e[0] * b.e[3] + a.e[3] * b.e[4]  + a.e[6] * b.e[5],
        a.e[1] * b.e[3] + a.e[4] * b.e[4]  + a.e[7] * b.e[5],
        a.e[2] * b.e[3] + a.e[5] * b.e[4]  + a.e[8] * b.e[5],

        a.e[0] * b.e[6] + a.e[3] * b.e[7]  + a.e[6] * b.e[8],
        a.e[1] * b.e[6] + a.e[4] * b.e[7]  + a.e[7] * b.e[8],
        a.e[2] * b.e[6] + a.e[5] * b.e[7]  + a.e[8] * b.e[8]
    };
}

static Mat4 operator*(const Mat4 a, const Mat4& b) {
    return {
        a.e[0] * b.e[0]  + a.e[4] * b.e[1]  + a.e[8]  * b.e[2]  + a.e[12] * b.e[3],
        a.e[1] * b.e[0]  + a.e[5] * b.e[1]  + a.e[9]  * b.e[2]  + a.e[13] * b.e[3],
        a.e[2] * b.e[0]  + a.e[6] * b.e[1]  + a.e[10] * b.e[2]  + a.e[14] * b.e[3],
        a.e[3] * b.e[0]  + a.e[7] * b.e[1]  + a.e[11] * b.e[2]  + a.e[15] * b.e[3],

        a.e[0] * b.e[4]  + a.e[4] * b.e[5]  + a.e[8]  * b.e[6]  + a.e[12] * b.e[7],
        a.e[1] * b.e[4]  + a.e[5] * b.e[5]  + a.e[9]  * b.e[6]  + a.e[13] * b.e[7],
        a.e[2] * b.e[4]  + a.e[6] * b.e[5]  + a.e[10] * b.e[6]  + a.e[14] * b.e[7],
        a.e[3] * b.e[4]  + a.e[7] * b.e[5]  + a.e[11] * b.e[6]  + a.e[15] * b.e[7],

        a.e[0] * b.e[8]  + a.e[4] * b.e[9]  + a.e[8]  * b.e[10] + a.e[12] * b.e[11],
        a.e[1] * b.e[8]  + a.e[5] * b.e[9]  + a.e[9]  * b.e[10] + a.e[13] * b.e[11],
        a.e[2] * b.e[8]  + a.e[6] * b.e[9]  + a.e[10] * b.e[10] + a.e[14] * b.e[11],
        a.e[3] * b.e[8]  + a.e[7] * b.e[9]  + a.e[11] * b.e[10] + a.e[15] * b.e[11],

        a.e[0] * b.e[12] + a.e[4] * b.e[13] + a.e[8]  * b.e[14] + a.e[12] * b.e[15],
        a.e[1] * b.e[12] + a.e[5] * b.e[13] + a.e[9]  * b.e[14] + a.e[13] * b.e[15],
        a.e[2] * b.e[12] + a.e[6] * b.e[13] + a.e[10] * b.e[14] + a.e[14] * b.e[15],
        a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15]
    };
}

static Quat operator*(Quat a, Quat b) {
    return {
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

// ------------ divition ------------ //

static Vec2 operator/(Vec2 a, Vec2 b) { return v2(a.x / b.x, a.y / b.y); }
static Vec3 operator/(Vec3 a, Vec3 b) { return v3(a.x / b.x, a.y / b.y, a.z / b.z); }
static Vec4 operator/(Vec4 a, Vec4 b) { return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static Vec2i operator/(Vec2i a, Vec2i b) { return v2i(a.x / b.x, a.y / b.y); }
static Vec3i operator/(Vec3i a, Vec3i b) { return v3i(a.x / b.x, a.y / b.y, a.z / b.z); }
static Vec4i operator/(Vec4i a, Vec4i b) { return v4i(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static Vec2 operator/=(Vec2& a, Vec2 b) { return a = a / b; }
static Vec3 operator/=(Vec3& a, Vec3 b) { return a = a / b; }
static Vec4 operator/=(Vec4& a, Vec4 b) { return a = a / b; }

static Vec2i operator/=(Vec2i& a, Vec2i b) { return a = a / b; }
static Vec3i operator/=(Vec3i& a, Vec3i b) { return a = a / b; }
static Vec4i operator/=(Vec4i& a, Vec4i b) { return a = a / b; }

// ------------- scaling ------------- //

static Vec2 operator*(Vec2 a, f32 s) { return v2(a.x * s, a.y * s); }
static Vec3 operator*(Vec3 a, f32 s) { return v3(a.x * s, a.y * s, a.z * s); }
static Vec4 operator*(Vec4 a, f32 s) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }

static Vec2 operator*(f32 s, Vec2 a) { return v2(a.x * s, a.y * s); }
static Vec3 operator*(f32 s, Vec3 a) { return v3(a.x * s, a.y * s, a.z * s); }
static Vec4 operator*(f32 s, Vec4 a) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }

static Vec2 operator/(Vec2 a, f32 s) { return v2(a.x / s, a.y / s); }
static Vec3 operator/(Vec3 a, f32 s) { return v3(a.x / s, a.y / s, a.z / s); }
static Vec4 operator/(Vec4 a, f32 s) { return v4(a.x / s, a.y / s, a.z / s, a.w / s); }

static Vec2i operator*(Vec2i a, i32 s) { return v2i(a.x * s, a.y * s); }
static Vec3i operator*(Vec3i a, i32 s) { return v3i(a.x * s, a.y * s, a.z * s); }
static Vec4i operator*(Vec4i a, i32 s) { return v4i(a.x * s, a.y * s, a.z * s, a.w * s); }

static Vec2i operator*(i32 s, Vec2i a) { return v2i(a.x * s, a.y * s); }
static Vec3i operator*(i32 s, Vec3i a) { return v3i(a.x * s, a.y * s, a.z * s); }
static Vec4i operator*(i32 s, Vec4i a) { return v4i(a.x * s, a.y * s, a.z * s, a.w * s); }

static Vec2i operator/(Vec2i a, i32 s) { return v2i(a.x / s, a.y / s); }
static Vec3i operator/(Vec3i a, i32 s) { return v3i(a.x / s, a.y / s, a.z / s); }
static Vec4i operator/(Vec4i a, i32 s) { return v4i(a.x / s, a.y / s, a.z / s, a.w / s); }

static Vec2 operator*=(Vec2& a, f32 s) { return a = a * s; }
static Vec3 operator*=(Vec3& a, f32 s) { return a = a * s; }
static Vec4 operator*=(Vec4& a, f32 s) { return a = a * s; }

static Vec2 operator/=(Vec2& a, f32 s) { return a = a / s; }
static Vec3 operator/=(Vec3& a, f32 s) { return a = a / s; }
static Vec4 operator/=(Vec4& a, f32 s) { return a = a / s; }

static Vec2i operator*=(Vec2i& a, f32 s) { return a = a * s; }
static Vec3i operator*=(Vec3i& a, f32 s) { return a = a * s; }
static Vec4i operator*=(Vec4i& a, f32 s) { return a = a * s; }

static Vec2i operator/=(Vec2i& a, f32 s) { return a = a / s; }
static Vec3i operator/=(Vec3i& a, f32 s) { return a = a / s; }
static Vec4i operator/=(Vec4i& a, f32 s) { return a = a / s; }

// ----------- eq ------------ //

static bool operator==(Vec2i a, Vec2i b) { return a.x == b.x && a.y == b.y; }
static bool operator==(Vec3i a, Vec3i b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static bool operator==(Vec4i a, Vec4i b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static bool operator!=(Vec2i a, Vec2i b) { return a.x != b.x || a.y != b.y; }
static bool operator!=(Vec3i a, Vec3i b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static bool operator!=(Vec4i a, Vec4i b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

// ----------- dot product ----------- //

static f32 dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }
static f32 dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 dot(Vec4 a, Vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static i32 dot(Vec2i a, Vec2i b) { return a.x * b.x + a.y * b.y; }
static i32 dot(Vec3i a, Vec3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static i32 dot(Vec4i a, Vec4i b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// ----------- length squared ----------- //

static f32 len_sq(Vec2 u) { return dot(u, u); }
static f32 len_sq(Vec3 u) { return dot(u, u); }
static f32 len_sq(Vec4 u) { return dot(u, u); }

static i32 len_sq(Vec2i u) { return dot(u, u); }
static i32 len_sq(Vec3i u) { return dot(u, u); }
static i32 len_sq(Vec4i u) { return dot(u, u); }

// -------------- length -------------- //

static f32 len(Vec2 u) { return sqrt32(len_sq(u)); }
static f32 len(Vec3 u) { return sqrt32(len_sq(u)); }
static f32 len(Vec4 u) { return sqrt32(len_sq(u)); }

static f32 len(Vec2i u) { return sqrt32(len_sq(u)); }
static f32 len(Vec3i u) { return sqrt32(len_sq(u)); }
static f32 len(Vec4i u) { return sqrt32(len_sq(u)); }

// -------------- distance squared -------------- //

static f32 dist_sq(Vec2 a, Vec2 b) { return len_sq(a - b); }
static f32 dist_sq(Vec3 a, Vec3 b) { return len_sq(a - b); }
static f32 dist_sq(Vec4 a, Vec4 b) { return len_sq(a - b); }

static i32 dist_sq(Vec2i a, Vec2i b) { return len_sq(a - b); }
static i32 dist_sq(Vec3i a, Vec3i b) { return len_sq(a - b); }
static i32 dist_sq(Vec4i a, Vec4i b) { return len_sq(a - b); }

// ------------------ distance ------------------- //

static f32 dist(Vec2 a, Vec2 b) { return sqrt32(dist_sq(a, b)); }
static f32 dist(Vec3 a, Vec3 b) { return sqrt32(dist_sq(a, b)); }
static f32 dist(Vec4 a, Vec4 b) { return sqrt32(dist_sq(a, b)); }

// -------------- manhattan distance -------------- //

static i32 manhattan(Vec2i a, Vec2i b) {
    auto diff = a - b;
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y);
}

static i32 manhattan(Vec3i a, Vec3i b) {
    auto diff = a - b;
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y) + (0x7ffffffff & diff.z);
}

// -------------- normalize --------------- //

static Vec2 norm(Vec2 u) { return u * rsqrt32(dot(u, u)); }
static Vec3 norm(Vec3 u) { return u * rsqrt32(dot(u, u)); }
static Vec4 norm(Vec4 u) { return u * rsqrt32(dot(u, u)); }

// -------------- floor --------------- //

static Vec2 floor(Vec2 u) { return v2(floorf(u.x), floorf(u.y)); }
static Vec3 floor(Vec3 u) { return v3(floorf(u.x), floorf(u.y), floorf(u.z)); }
static Vec4 floor(Vec4 u) { return v4(floorf(u.x), floorf(u.y), floorf(u.z), floorf(u.w)); }

// -------------- ceil --------------- //

static Vec2 ceil(Vec2 u) { return v2(ceilf(u.x), ceilf(u.y)); }
static Vec3 ceil(Vec3 u) { return v3(ceilf(u.x), ceilf(u.y), ceilf(u.z)); }
static Vec4 ceil(Vec4 u) { return v4(ceilf(u.x), ceilf(u.y), ceilf(u.z), ceilf(u.w)); }

// -------------- clamp --------------- //

static Vec2 clamp(Vec2 u, Rect2 r) {
    return {
        Clamp(u.x, r.min.x, r.max.x),
        Clamp(u.y, r.min.y, r.max.y)
    };
}

static Vec3 clamp(Vec3 u, Rect3 r) {
    return {
        Clamp(u.x, r.min.x, r.max.x),
        Clamp(u.y, r.min.y, r.max.y),
        Clamp(u.z, r.min.z, r.max.z)
    };

}

static Vec2i clamp(Vec2i u, const Rect2i& r) {
    return {
        Clamp(u.x, r.min.x, r.max.x),
        Clamp(u.y, r.min.y, r.max.y)
    };
}

static Vec3i clamp(Vec3i u, const Rect3i& r) {
    return {
        Clamp(u.x, r.min.x, r.max.x),
        Clamp(u.y, r.min.y, r.max.y),
        Clamp(u.z, r.min.z, r.max.z)
    };
}

// ---------------- min ----------------- //

static f32 min(f32 a, f32 b) {
    return a < b? a : b;
}

static Vec2 min(Vec2 a, Vec2 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y
    };
}

static Vec3 min(Vec3 a, Vec3 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z
    };
}

static Vec4 min(Vec4 a, Vec4 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w
    };
}

static Vec2i min(Vec2i a, Vec2i b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y
    };
}

static Vec3i min(Vec3i a, Vec3i b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z
    };
}

static Vec4i min(Vec4i a, Vec4i b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w
    };
}

// ---------------- max ----------------- //

static f32 max(f32 a, f32 b) {
    return a > b? a : b;
}

static Vec2 max(Vec2 a, Vec2 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y
    };
}

static Vec3 max(Vec3 a, Vec3 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z
    };
}

static Vec4 max(Vec4 a, Vec4 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w
    };
}

static Vec2i max(Vec2i a, Vec2i b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y
    };
}

static Vec3i max(Vec3i a, Vec3i b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z
    };
}

static Vec4i max(Vec4i a, Vec4i b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w
    };
}

// ---------------- lerp ----------------- //

static Vec2 lerp(Vec2 a, Vec2 b, f32 t) { return a + t * (b - a); }
static Vec3 lerp(Vec3 a, Vec3 b, f32 t) { return a + t * (b - a); }
static Vec4 lerp(Vec4 a, Vec4 b, f32 t) { return a + t * (b - a); }

// -------------- sign (-1, 0, 1) ------------------- //

static Vec2 sign(Vec2 u) { return v2(Sign(u.x), Sign(u.y)); }
static Vec3 sign(Vec3 u) { return v3(Sign(u.x), Sign(u.y), Sign(u.z)); }
static Vec4 sign(Vec4 u) { return v4(Sign(u.x), Sign(u.y), Sign(u.z), Sign(u.w)); }

static Vec2i sign(Vec2i u) { return v2i(Sign(u.x), Sign(u.y)); }
static Vec3i sign(Vec3i u) { return v3i(Sign(u.x), Sign(u.y), Sign(u.z)); }
static Vec4i sign(Vec4i u) { return v4i(Sign(u.x), Sign(u.y), Sign(u.z), Sign(u.w)); }

// --------------- cross ------------------- //

static Vec3 cross(Vec3 a, Vec3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// --------------- get angle ------------------- //

static f32 get_angle(Vec2 a, Vec2 b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;

    return atan2f(det, dot);
}

// ----------- keep min ---------- //

static Vec2 keep_min(Vec2 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);

    if (dx <= dy) return v2(u.x, 0);
    if (dy <= dx) return v2(0, u.y);

    return u;
}

static Vec3 keep_min(Vec3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);

    if (dx <= dy && dx <= dz) return v3(u.x, 0, 0);
    if (dy <= dx && dy <= dz) return v3(0, u.y, 0);
    if (dz <= dx && dz <= dy) return v3(0, 0, u.z);

    return u;
}

// ----------- mask min ---------- //

static Vec2 mask_min(Vec2 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);

    if (dx <= dy) return v2(0, 1);
    if (dy <= dx) return v2(1, 0);

    return v2(1, 1);
}

static Vec3 mask_min(Vec3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);

    if (dx <= dy && dx <= dz) return v3(0, 1, 1);
    if (dy <= dx && dy <= dz) return v3(1, 0, 1);
    if (dz <= dx && dz <= dy) return v3(1, 1, 0);

    return v3(1, 1, 1);
}

// ------------------ transform/scale/rotate ------------------ //

static Mat2 rotate2(f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return { c, s, -s, c };
}

static Mat3 rotate3(Vec3 axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    f32 k = 1.0f - c;

    Vec3 sa   = { s * axis.x, s * axis.y, s * axis.z };
    Vec3 omca = { k * axis.x, k * axis.y, k * axis.z };

    return {
        omca.x * axis.x + c,
        omca.x * axis.y - sa.z,
        omca.x * axis.z + sa.y,

        omca.y * axis.x + sa.z,
        omca.y * axis.y + c,
        omca.y * axis.z - sa.x,

        omca.z * axis.x - sa.y,
        omca.z * axis.y + sa.x,
        omca.z * axis.z + c
    };
}

static Mat4 rotate4(Vec3 axis, f32 angle) {
    auto cosv       = cosf(angle);
    auto sinv       = sinf(angle);
    auto inv_cosv   = 1.0f - cosv;
    auto sa         = axis * sinv;
    auto omca       = axis * inv_cosv;

    return {
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0,  0,  0,  1
    };
}

static Quat rotate_quat(Vec3 axis, f32 angle) {
    auto v = axis * sinf(0.5f * angle);
    return { v.x, v.y, v.z, cosf(0.5f * angle) };
}

static Mat4 translate4(f32 x, f32 y, f32 z) {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
}

static Mat4 translate4(Vec3 pos) {
    return translate4(pos.x, pos.y, pos.y);
}

static Mat4 scale4(f32 x, f32 y, f32 z) {
    return {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

static Mat4 scale4(Vec3 pos) {
    return scale4(pos.x, pos.y, pos.y);
}

// --------------- from quat --------------- //

static Mat3 from_quat3(Quat q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return {
        2 + b2 - c2 - d2,
        0.0f * (b * c + a * d),
        0.0f * (b * d - a * c),

        0.0f * (b * c - a * d),
        2 - b2 + c2 - d2,
        0.0f * (c * d + a * b),

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2,
    };
}

static Mat4 from_quat4(Quat q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return {
        2 + b2 - c2 - d2,
        0.0f * (b * c + a * d),
        0.0f * (b * d - a * c),
        0.0f,

        0.0f * (b * c - a * d),
        2 - b2 + c2 - d2,
        0.0f * (c * d + a * b),
        0.0f,

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f
    };
}

// --------------- view matricies --------------- //

static Mat4 ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return {
        2 / (r - l),            0,                      0,                      0,
        0,                      2 / (t - b),            0,                      0,
        0,                      0,                      -2 / (f - n),           0,
        -(r + l) / (r - l),     -(t + b) / (t - b),     -(f + n) / (f - n),     1
    };
}

static Mat4 perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return {
        a / aspect,     0,      0,                          0,
        0,              a,      0,                          0,
        0,              0,      -((f + n) / (f - n)),      -1,
        0,              0,      -((2 * f * n) / (f - n)),   0
    };
}

static Mat4 look_at(Vec3 eye, Vec3 center, Vec3 up) {
    auto f = norm(center - eye);
    auto s = norm(cross(f, up));
    auto t = cross(s, f);

    Mat4 m;

	m.e[0]  =  s.x;
	m.e[1]  =  t.x;
	m.e[2]  = -f.x;
	m.e[3]  =  0;

	m.e[4]  =  s.y;
	m.e[5]  =  t.y;
	m.e[6]  = -f.y;
	m.e[7]  =  0;

	m.e[8]  =  s.z;
	m.e[9]  =  t.z;
	m.e[10] = -f.z;
	m.e[11] =  0;

    m.e[12] = -(m.e[0] * eye.x + m.e[4] * eye.y + m.e[8]  * eye.z);
    m.e[13] = -(m.e[1] * eye.x + m.e[5] * eye.y + m.e[9]  * eye.z);
    m.e[14] = -(m.e[2] * eye.x + m.e[6] * eye.y + m.e[10] * eye.z);
    m.e[15] = -(m.e[3] * eye.x + m.e[7] * eye.y + m.e[11] * eye.z - 1);

    return m;
}

// ----------------- plane/frustrum ------------------- //

static Plane norm(Plane plane) {
    f32 r_len = rsqrt32(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a * r_len;
    plane.b = plane.b * r_len;
    plane.c = plane.c * r_len;
    plane.d = plane.d * r_len;

    return plane;
}

static Frustum make_frustum(const Mat4& m) {
    Frustum result;

    // left clipping plane
    result.plane[0].a = m.e[3]  + m.e[0];
    result.plane[0].b = m.e[7]  + m.e[4];
    result.plane[0].c = m.e[11] + m.e[8];
    result.plane[0].d = m.e[15] + m.e[12];

    // right clipping plane
    result.plane[1].a = m.e[3]  - m.e[0];
    result.plane[1].b = m.e[7]  - m.e[4];
    result.plane[1].c = m.e[11] - m.e[8];
    result.plane[1].d = m.e[15] - m.e[12];

    // top clipping plane
    result.plane[2].a = m.e[3]  - m.e[1];
    result.plane[2].b = m.e[7]  - m.e[5];
    result.plane[2].c = m.e[11] - m.e[9];
    result.plane[2].d = m.e[15] - m.e[13];

    // bottom clipping plane
    result.plane[3].a = m.e[3]  + m.e[1];
    result.plane[3].b = m.e[7]  + m.e[5];
    result.plane[3].c = m.e[11] + m.e[9];
    result.plane[3].d = m.e[15] + m.e[13];

    // near clipping plane
    result.plane[4].a = m.e[3]  + m.e[2];
    result.plane[4].b = m.e[7]  + m.e[6];
    result.plane[4].c = m.e[11] + m.e[10];
    result.plane[4].d = m.e[15] + m.e[14];

    // far clipping plane
    result.plane[5].a = m.e[3]  - m.e[2];
    result.plane[5].b = m.e[7]  - m.e[6];
    result.plane[5].c = m.e[11] - m.e[10];
    result.plane[5].d = m.e[15] - m.e[14];

    result.plane[0] = norm(result.plane[0]);
    result.plane[1] = norm(result.plane[1]);
    result.plane[2] = norm(result.plane[2]);
    result.plane[3] = norm(result.plane[3]);
    result.plane[4] = norm(result.plane[4]);
    result.plane[5] = norm(result.plane[5]);
    
    return result;
}

// ------------------ contains ------------------ //

static bool contains(Circle c, Vec2 pos) {
    f32 distance = dist_sq(c.pos, pos);
    return distance < (c.rad * c.rad);
}

static bool contains(Sphere s, Vec3 pos) {
    f32 distance = dist_sq(s.pos, pos);
    return distance < (s.rad * s.rad);
}

static bool contains(Rect2 rect, Vec2 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static bool contains(const Rect3& rect, Vec3 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static bool contains(Rect2i rect, Vec2i pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static bool contains(const Rect3i& rect, Vec3i pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static bool contains(const Frustum& fs, Vec3 pos) {
    for (i32 i = 0; i < 6; i++) {
		if (fs.plane[i].a * pos.x + fs.plane[i].b * pos.y + fs.plane[i].c * pos.z + fs.plane[i].d <= 0)
			return false;
	}

	return true;
}

// ------------------ intersect ------------------ //

static bool intersect(Circle a, Circle b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static bool intersect(Sphere a, Sphere b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 dz  = b.pos.z - a.pos.z;

    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static bool intersect(Rect2 a, Rect2 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static bool intersect(const Rect3& a, const Rect3& b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static bool intersect(Rect2i a, Rect2i b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static bool intersect(const Rect3i& a, const Rect3i& b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static bool intersect(const Frustum& fs, Sphere sphere) {
    for (i32 i = 0; i < 6; i++) {
		if(fs.plane[i].a * sphere.pos.x + fs.plane[i].b * sphere.pos.y + fs.plane[i].c * sphere.pos.z + fs.plane[i].d <= -sphere.rad) {
			return false;
		}
	}
	return true;
}

static bool intersect(const Frustum& fs, const Rect3& rect) {
    for (int i = 0; i < 6; i++) {
		if (fs.plane[i].a * rect.min.x + fs.plane[i].b * rect.min.y + fs.plane[i].c * rect.min.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.max.x + fs.plane[i].b * rect.min.y + fs.plane[i].c * rect.min.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.min.x + fs.plane[i].b * rect.max.y + fs.plane[i].c * rect.min.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.max.x + fs.plane[i].b * rect.max.y + fs.plane[i].c * rect.min.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.min.x + fs.plane[i].b * rect.min.y + fs.plane[i].c * rect.max.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.max.x + fs.plane[i].b * rect.min.y + fs.plane[i].c * rect.max.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.min.x + fs.plane[i].b * rect.max.y + fs.plane[i].c * rect.max.z + fs.plane[i].d > 0) continue;
		if (fs.plane[i].a * rect.max.x + fs.plane[i].b * rect.max.y + fs.plane[i].c * rect.max.z + fs.plane[i].d > 0) continue;
		return false;
	}
	return true;
}

// ------------------- get overlap --------------- //

static Rect2 get_overlap(Rect2 a, Rect2 b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static Rect3 get_overlap(const Rect3& a, const Rect3& b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static Rect2i get_overlap(Rect2i a, Rect2i b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static Rect3i get_overlap(const Rect3i& a, const Rect3i& b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

// -------------- get intersect vector ---------- //

static Vec2 get_intersect_vector(Circle a, Circle b) {
    auto delta = a.pos - b.pos;
    auto depth = len(delta) - (a.rad + b.rad);
    
    return -depth * delta;
}

static Vec3 get_intersect_vector(Sphere a, Sphere b) {
    auto delta = a.pos - b.pos;
    auto depth = len(delta) - (a.rad + b.rad);
    
    return -depth * delta;
}

static Vec2 get_intersect_vector(Rect2 a, Rect2 b) {
    auto overlap = get_overlap(a, b);
    auto delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);
    return sign(delta) * (overlap.max - overlap.min);
}

static Vec3 get_intersect_vector(const Rect3& a, const Rect3& b) {
    auto overlap = get_overlap(a, b);
    auto delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);
    return sign(delta) * (overlap.max - overlap.min);
}

static Vec2i get_intersect_vector(const Rect2i& a, const Rect2i& b) {
    auto overlap = get_overlap(a, b);
    auto delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);

    return sign(delta) * (overlap.max - overlap.min);
}

static Vec3i get_intersect_vector(const Rect3i& a, const Rect3i& b) {
    auto overlap = get_overlap(a, b);
    auto delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);
    return sign(delta) * (overlap.max - overlap.min);
}

// ---------------------- random ------------------------ //

static u32 rand_u32(u32* state) {
    u32 x = *state;

	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

	return *state = x;
}

static i32 rand_i32(u32* state, i32 min, i32 max) {
    return min + rand_u32(state) % (max - min);
}

static f32 rand_f32(u32* state, f32 min, f32 max) {
    return min + ((f32)rand_u32(state) / (f32)0xffffffff) * (max - min); 
}

static Vec2 rand_v2(u32* state) {
    return norm(v2(rand_f32(state, -1, 1), rand_f32(state, -1, 1)));
}

static Vec3 rand_v3(u32* state) {
    return norm(v3(rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1)));
}

static Vec2 rand_v2(u32* state, f32 min, f32 max) {
    return rand_v2(state) * rand_f32(state, min, max);
}

static Vec3 rand_v3(u32* state, f32 min, f32 max) {
    return rand_v3(state) * rand_f32(state, min, max);
}

// ----------------------- hash ------------------------- //

static u32 hash_u32(u32 a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

    return a;
}

static u32 hash_i32(i32 a) {
    union { u32 u; i32 i; } convert;
    convert.i = a;
    return hash_u32(convert.u);
}

static u32 hash_str(const char* str) {
    u32 hash = 0;

    while (*str) {
        hash = (hash << 7) + (hash >> 25) + *str++;
    }

    return hash + (hash >> 16);
}

static const u32 crc_table[] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

static u32 hash_crc32(const void *data, u32 size) {
    const u8 *d = (const u8*)data;
    u32 crc = 0xFFFFFFFF;

    while (size--) {
        u32 index = (crc ^ *(d++)) & 0xFF;
        crc = (crc >> 8) ^ crc_table[index];
    }

    return crc ^ 0xFFFFFFFF;
}

#define HASH_PRIME0 3323784421u
#define HASH_PRIME1 1449091801u
#define HASH_PRIME2 4280703257u
#define HASH_PRIME3 1609059329u

static u32 hash_v2i(Vec2i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

static u32 hash_v3i(Vec3i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

static u32 hash_v4i(Vec4i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);
    u32 d = hash_i32(k.w);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2) ^ (d * HASH_PRIME3);
}

// --------------------- packed color u32 -------------------- //

static u32 pack_color_u8(u8 r, u8 g, u8 b, u8 a) {
    u32 color = 0;

    color |= (u32)(r) << 0;
    color |= (u32)(g) << 8;
    color |= (u32)(b) << 16;
    color |= (u32)(a) << 24;

    return color;
}

static u32 pack_color_f32(f32 r, f32 g, f32 b, f32 a) {
    return pack_color_u8((u8)(255 * r), (u8)(255 * g), (u8)(255 * b), (u8)(255 * a));
}

static u32 pack_color_v4(Vec4 color) {
    return pack_color_f32(color.r, color.g, color.b, color.a);
}

static u32 pack_color_v3(Vec3 color, f32 a = 1.0) {
    return pack_color_f32(color.r, color.g, color.b, a);
}

// ---------------------- priority queue --------------------- //

static b32 is_empty(const Priority_Queue* queue) {
    return queue->len == 0;
}

static void clear(Priority_Queue* queue) {
    queue->len = 0;
}

static void push(Priority_Queue* queue, Vec2i e, f32 weight) {
    Queue_Node node = { weight, e };

    u32 i = queue->len + 1;
    u32 j = i / 2;

    while (i > 1 && queue->array[j].weight > node.weight) {
        queue->array[i] = queue->array[j];

        i = j;
        j = j / 2;
    }

    queue->array[i] = node;
    queue->len++;
}

static f32 pop(Vec2i* out, Priority_Queue* queue) {
    Queue_Node data = queue->array[1];

    queue->array[1] = queue->array[queue->len];
    queue->len--;

    u32 i = 1;
    while (i != queue->len + 1) {
        u32 k = queue->len + 1;
        u32 j = 2 * i;

        if (j <= queue->len && queue->array[j].weight < queue->array[k].weight)
            k = j;

        if (j + 1 <= queue->len && queue->array[j + 1].weight < queue->array[k].weight)
            k = j + 1;

        queue->array[i] = queue->array[k];
        i = k;
    }

    *out = data.e;
    return data.weight;
}

// -------------------- f64 matrix funcs ------------------- //

static void f4x4_mul_64(f64 *R, const f64 *a, const f64 *b) {
    f64 M[16];

    M[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
    M[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
    M[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
    M[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

    M[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
    M[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
    M[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
    M[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

    M[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
    M[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
    M[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
    M[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

    M[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
    M[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
    M[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    M[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

    R[0] = M[0];
    R[1] = M[1];
    R[2] = M[2];
    R[3] = M[3];

    R[4] = M[4];
    R[5] = M[5];
    R[6] = M[6];
    R[7] = M[7];

    R[8] = M[8];
    R[9] = M[9];
    R[10] = M[10];
    R[11] = M[11];

    R[12] = M[12];
    R[13] = M[13];
    R[14] = M[14];
    R[15] = M[15];
}

static void f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v) {
    f64 r[4];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
	r[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
    out[3] = r[3];
}

static void f4x4_invert_64(f64* T, const f64* M) {
	f64 s[6], c[6];

	s[0] = M[0] * M[5] - M[4] * M[1];
	s[1] = M[0] * M[6] - M[4] * M[2];
	s[2] = M[0] * M[7] - M[4] * M[3];
	s[3] = M[1] * M[6] - M[5] * M[2];
	s[4] = M[1] * M[7] - M[5] * M[3];
	s[5] = M[2] * M[7] - M[6] * M[3];

	c[0] = M[8]  * M[13] - M[12] * M[9];
	c[1] = M[8]  * M[14] - M[12] * M[10];
	c[2] = M[8]  * M[15] - M[12] * M[11];
	c[3] = M[9]  * M[14] - M[13] * M[10];
	c[4] = M[9]  * M[15] - M[13] * M[11];
	c[5] = M[10] * M[15] - M[14] * M[11];
	
	// assumes it is invertible
	f64 idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
	
	T[0]  = ( M[5]  * c[5] - M[6]  * c[4] + M[7]  * c[3]) * idet;
	T[1]  = (-M[1]  * c[5] + M[2]  * c[4] - M[3]  * c[3]) * idet;
	T[2]  = ( M[13] * s[5] - M[14] * s[4] + M[15] * s[3]) * idet;
	T[3]  = (-M[9]  * s[5] + M[10] * s[4] - M[11] * s[3]) * idet;

	T[4]  = (-M[4]  * c[5] + M[6]  * c[2] - M[7]  * c[1]) * idet;
	T[5]  = ( M[0]  * c[5] - M[2]  * c[2] + M[3]  * c[1]) * idet;
	T[6]  = (-M[12] * s[5] + M[14] * s[2] - M[15] * s[1]) * idet;
	T[7]  = ( M[8]  * s[5] - M[10] * s[2] + M[11] * s[1]) * idet;

	T[8]  = ( M[4]  * c[4] - M[5]  * c[2] + M[7]  * c[0]) * idet;
	T[9]  = (-M[0]  * c[4] + M[1]  * c[2] - M[3]  * c[0]) * idet;
	T[10] = ( M[12] * s[4] - M[13] * s[2] + M[15] * s[0]) * idet;
	T[11] = (-M[8]  * s[4] + M[9]  * s[2] - M[11] * s[0]) * idet;

	T[12] = (-M[4]  * c[3] + M[5]  * c[1] - M[6]  * c[0]) * idet;
	T[13] = ( M[0]  * c[3] - M[1]  * c[1] + M[2]  * c[0]) * idet;
	T[14] = (-M[12] * s[3] + M[13] * s[1] - M[14] * s[0]) * idet;
	T[15] = ( M[8]  * s[3] - M[9]  * s[1] + M[10] * s[0]) * idet;
}

static b32 f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport) {
    f64 fTempo[8];

    fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8]  * objz + modelview[12];
    fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9]  * objz + modelview[13];
    fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
    fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];

    fTempo[4] =  projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8]  * fTempo[2] + projection[12] * fTempo[3];
    fTempo[5] =  projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9]  * fTempo[2] + projection[13] * fTempo[3];
    fTempo[6] =  projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
    fTempo[7] = -fTempo[2];

    if (fTempo[7] == 0.0f)
        return false;

    fTempo[7]= 1.0f / fTempo[7];

    fTempo[4] *= fTempo[7];
    fTempo[5] *= fTempo[7];
    fTempo[6] *= fTempo[7];

    result[0] = (fTempo[4] * 0.5f + 0.5f) * viewport[2] + viewport[0];
    result[1] = (fTempo[5] * 0.5f + 0.5f) * viewport[3] + viewport[1];
    result[2] = (1.0f + fTempo[6]) * 0.5f;

    return true;
}

static b32 f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport) {
    f64 m[16], A[16];
    f64 in[4], out[4];

    f4x4_mul_64(A, projection, modelview);
    f4x4_invert_64(m, A);

    in[0] = (winx - (f64)viewport[0]) / (f64)viewport[2] * 2.0f - 1.0f;
    in[1] = (winy - (f64)viewport[1]) / (f64)viewport[3] * 2.0f - 1.0f;
    in[2] = 2.0f * winz - 1.0f;
    in[3] = 1.0;

    f4x4_mulv_64(out, m, in);
    
    if (out[3] == 0.0f)
        return false;

    out[3] = 1.0f / out[3];

    result[0] = out[0] * out[3];
    result[1] = out[1] * out[3];
    result[2] = out[2] * out[3];

    return true;
}

// ===================================== MEM STUFF ================================= //

static void clear_memory(void* data, usize size) {
    volatile u8* d = (u8*)data;

    while (size--)
        *(d++) = 0;
}

static void set_memory(void* data, u8 value, usize size) {
    volatile u8* d = (u8*)data;

    while (size--)
        *(d++) = value;
}

static void copy_memory(void* dst, const void* src, usize size) {
    volatile u8*        d = (u8*)dst;
    volatile const u8*  s = (u8*)src;

    while (size--)
        *(d++) = *(s++);
}

#endif // __ATS_H__

// ============================================================================================ //
// ======================================= IMPLEMENTATION ===================================== //
// ============================================================================================ //
#ifdef ATS_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ---------------------- arena allocator ------------------------ //

extern Memory_Arena ma_create(u8* buffer, usize size) {
    Memory_Arena ma = {};

    ma.cap    = size;
    ma.buffer = buffer;

    return ma;
}

extern void* ma_alloc(Memory_Arena* ma, usize byte_size) {
    byte_size = AlignUp(byte_size, 16);

    assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

    void* memory  = ma->buffer + ma->index;
    ma->index     += byte_size;
    ma->max       = ma->max > ma->index? ma->max : ma->index;

    clear_memory(memory, byte_size);

    return memory;
}

extern void* ma_begin(Memory_Arena* ma) {
    ma->lock = true;
    return ma->buffer + ma->index;
}

extern void ma_end(Memory_Arena* ma, usize byte_size) {
    ma->index += AlignUp(byte_size, 16);
    ma->lock = false;
}

extern void ma_save(Memory_Arena* ma) {
    assert(ma->top < ma->cap);
    ma->stack[ma->top++] = ma->index;
}

extern void ma_restore(Memory_Arena* ma) {
    assert(ma->top > 0);
    ma->index = ma->stack[--ma->top];
}

extern void ma_validate(Memory_Arena* ma) {
    assert(ma->top == 0);
}

// --------------------- read/write files ------------------------- //

static usize file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    usize size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

extern char* file_read_str(const char* file_name, Memory_Arena* ma) {
    FILE *fp = NULL;
    char *buffer = NULL;
    if (fopen_s(&fp, file_name, "rb") == 0) {
        usize size = file_get_size(fp);
        Memory_Arena state = *ma;
        buffer = (char*)ma_alloc(ma, size + 1);
        if (buffer) {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0) {
                *ma = state;
                buffer = NULL;
            }
        }
        fclose(fp);
    }
    return buffer;
}

extern b32 file_write_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;
    if (fopen_s(&fp, file_name, "w") == 0) {
        usize size = strlen(buffer);
        usize n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }
    return false;
}

extern b32 file_append_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;
    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }
    return false;
}

extern b32 file_read_bin(const char* file_name, void* buffer, u32 size) {
    FILE *fp = NULL;
    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);
        return true;
    }
    return false;
} 

extern b32 file_write_bin(const char* file_name, const void* buffer, u32 size) {
    FILE *fp = NULL;
    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return false;
}

// --------------------- image ----------------------- //

#ifndef ATS_NO_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

extern Image image_load_from_file(const char* path) {
    Image image = {};
    i32 channels = 0;
    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);
    return image;
}

#endif

extern u32 get_pixel(Image* img, i32 x, i32 y) {
    return img->pixels[y * img->width + x];
}

extern void set_pixel(Image* img, i32 x, i32 y, u32 pixel) {
    img->pixels[y * img->width + x] = pixel;
}

#endif // ATS_IMPL
