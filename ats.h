#ifndef __ATS_H__
#define __ATS_H__

#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>

// ======================================== API-MACROS ======================================= //

#ifndef __cplusplus
#define true  1
#define false 0
#endif

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define PI (3.14159265359f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define to_rad(deg) ((deg) * TO_RAD_MUL)
#define to_deg(rad) ((rad) * TO_DEG_MUL)

#define is_power_of_two(x)  (((x) != 0) && ((x) & ((x)-1)) == 0)

#define align_down(n, a)      ((n) & ~((a) - 1))
#define align_up(n, a)        align_down((n) + (a) - 1, (a))
#define align_down_ptr(p, a)  ((void*)align_down((uintptr_t)(p), (a)))
#define align_up_ptr(p, a)    ((void*)align_down((uintptr_t)(p), (a)))

#define array_count(array) (sizeof (array) / sizeof ((array)[0]))

#define join_helper(a, b)   a##b
#define join_token(a, b)    join_helper(a, b)
#define macro_var(a)        join_token(a, __LINE__)

#define defer(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_rect2(rect, ix, iy) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_rect3(rect, ix, iy, iz) \
    for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

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

union vec2 {
    struct { f32 x, y; };

    f32 e[2];
};

union vec3 {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { vec2 xy; };

    f32 e[3];
};

union vec4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { vec3 rgb; };
    struct { vec3 xyz; };

    f32 e[4];
};

union ivec2 {
    struct { i32 x, y; };

    i32 e[2];
};

union ivec3 {
    struct { i32 x, y, z; };
    struct { ivec2 xy; };

    i32 e[3];
};

union ivec4 {
    struct { i32 x, y, z, w; };

    i32 e[4];
};

union bvec2 {
    u8 x : 1;
    u8 y : 1;
};

struct bvec3 {
    u8 x : 1;
    u8 y : 1;
    u8 z : 1;
};

union bvec4 {
    u8 x : 1;
    u8 y : 1;
    u8 z : 1;
    u8 w : 1;
};

union mat2 {
    struct { vec2 x, y; };
    f32 e[4];
};

union mat3 {
    struct { vec3 x, y, z; };
    f32 e[9];
};

union mat4 {
    struct { vec4 x, y, z, w; };
    f32 e[16];
};

struct quat_t {
    f32 x, y, z, w;
};

struct circle_t {
    vec2    pos;
    f32     rad;
};

struct sphere_t {
    vec3    pos;
    f32     rad;
};

struct rect2 {
    vec2    min;
    vec2    max;
};

struct rect3 {
    vec3    min;
    vec3    max;
};

struct irect2 {
    ivec2   min;
    ivec2   max;
};

struct irect3 {
    ivec3   min;
    ivec3   max;
};

struct plane_t {
    f32     a;
    f32     b;
    f32     c;
    f32     d;
};

struct frustum_t {
    plane_t     plane[6];
};

struct memory_arena {
    usize       index;
    usize       cap;
    u8*         buffer;

    usize       top;
    usize       stack[16];

    usize       lock;
    usize       max;
};

struct image_t {
    int     width;
    int     height;
    u32*    pixels;
};

struct queue_node {
    f32     weight;
    ivec2   e;
};

struct priority_queue {
    u32             len;
    queue_node*     array;
};

// ---------------------- arena allocator ------------------------ //

extern memory_arena ma_create(u8* buffer, usize size);

#define ma_type(ma, t)           (t*)ma_alloc(ma, sizeof (t))
#define ma_array(ma, t, count)   (t*)ma_alloc(ma, (count) * sizeof (t))

extern void*    ma_alloc(memory_arena* ma, usize byte_size);
extern void*    ma_begin(memory_arena* ma);
extern void     ma_end(memory_arena* ma, usize byte_size);
extern void     ma_save(memory_arena* ma);
extern void     ma_restore(memory_arena* ma);
extern void     ma_validate(memory_arena* ma);

// --------------------- read/write files ------------------------- //

extern char*    file_read_str(const char* file_name, memory_arena* ma);
extern b32      file_write_str(const char* file_name, const char* buffer);
extern b32      file_append_str(const char* file_name, const char* buffer);

extern b32      file_read_bin(const char* file_name, void* buffer, u32 size);
extern b32      file_write_bin(const char* file_name, const void* buffer, u32 size);

// ----------------------- image -------------------------- //

extern image_t  image_load_from_file(const char* path);
extern u32      image_get_pixel(image_t* img, i32 x, i32 y);
extern void     image_set_pixel(image_t* img, i32 x, i32 y, u32 pixel);

// ======================================= STATIC FUNCTIONS ==================================== //

static vec2 v2(f32 x, f32 y)                     { return { x, y }; }
static vec2 v2(f32 n)                            { return v2(n, n); }
static vec2 v2(vec3 u)                           { return v2(u.x, u.y); }
static vec2 v2(vec4 u)                           { return v2(u.x, u.y); }

static vec3 v3(f32 x, f32 y, f32 z)              { return { x, y, z }; }
static vec3 v3(f32 n)                            { return v3(n, n, n); }
static vec3 v3(vec3 u, f32 z = 0)                { return v3(u.x, u.y, z); }
static vec3 v3(vec4 u)                           { return v3(u.x, u.y, u.z); }

static vec4 v4(f32 x, f32 y, f32 z, f32 w)       { return { x, y, z, w }; }
static vec4 v4(f32 n)                            { return v4(n, n, n, n); }
static vec4 v4(vec2 u, f32 z = 0, f32 w = 0)     { return v4(u.x, u.y, z, w); }
static vec4 v4(vec3 u, f32 w = 0)                { return v4(u.x, u.y, u.z, w); }

static ivec2 iv2(i32 x, i32 y)                   { return { x, y }; }
static ivec2 iv2(i32 n)                          { return iv2(n, n); }
static ivec2 iv2(ivec3 u)                        { return iv2(u.x, u.y); }
static ivec2 iv2(ivec4 u)                        { return iv2(u.x, u.y); }

static ivec3 iv3(i32 x, i32 y, i32 z)            { return { x, y, z }; }
static ivec3 iv3(i32 n)                          { return iv3(n, n, n); }
static ivec3 iv3(ivec3 u, i32 z = 0)             { return iv3(u.x, u.y, z); }
static ivec3 iv3(ivec4 u)                        { return iv3(u.x, u.y, u.z); }

static ivec4 iv4(i32 x, i32 y, i32 z, i32 w)     { return { x, y, z, w }; }
static ivec4 iv4(i32 n)                          { return iv4(n, n, n, n); }
static ivec4 iv4(ivec2 u, i32 z = 0, i32 w = 0)  { return iv4(u.x, u.y, z, w); }
static ivec4 iv4(ivec3 u, i32 w = 0)             { return iv4(u.x, u.y, u.z, w); }

static bvec2 bv2(bool x, bool y) {
    bvec2 result = {};
    result.x = x;
    result.y = y;
    return result;
}

static bvec3 bv3(bool x, bool y, bool z) {
    bvec3 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

static bvec4 bv4(bool x, bool y, bool z, bool w) {
    bvec4 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

static mat2 m2() {
    return {
        1, 0,
        0, 1,
    };
}

static mat2 m2(f32 xx, f32 xy, f32 yx, f32 yy) {
    return {
        xx, xy,
        yx, yy,
    };
}

static mat2 m2(vec2 x, vec2 y) {
    return {
        x.x, x.y,
        y.x, y.y,
    };
}

static mat3 m3() {
    return {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    };
}

static mat3 m3(f32 xx, f32 xy, f32 xz, f32 yx, f32 yy, f32 yz, f32 zx, f32 zy, f32 zz) {
    return {
        xx, xy, xz,
        yx, yy, yz,
        zx, zy, zz,
    };
}

static mat3 m3(vec3 x, vec3 y, vec3 z) {
    return {
        x.x, x.y, x.z,
        y.x, y.y, y.z,
        z.x, z.y, z.z
    };
}

static mat4 m4() {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

static mat4 m4(f32 xx, f32 xy, f32 xz, f32 xw,
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

static mat4 m4(vec4 x, vec4 y, vec4 z, vec4 w) {
    return {
        x.x, x.y, x.z, x.w,
        y.x, y.y, y.z, y.w,
        z.x, z.y, z.z, z.w,
        w.x, w.y, w.z, w.w,
    };
}

static quat_t quat() {
    return { 0, 0, 0, 1 };
}

static quat_t quat(f32 x, f32 y, f32 z, f32 w) {
    return { x, y, z, w };
}

static circle_t circle(f32 x, f32 y, f32 rad) {
    return { x, y, rad };
}

static circle_t circle(vec2 pos, f32 rad) {
    return { pos, rad };
}

static sphere_t sphere(vec3 pos, f32 rad) {
    return { pos, rad };
}

static rect2 r2(f32 min_x, f32 min_y, f32 max_x, f32 max_y) {
    return { min_x, min_y, max_x, max_y };
}

static rect2 r2(vec2 min, vec2 max) {
    return { min, max };
}

static rect3 r3(f32 min_x, f32 min_y, f32 min_z, f32 max_x, f32 max_y, f32 max_z) {
    return { min_x, min_y, min_z, max_x, max_y, max_z };
}

static rect3 r3(vec3 min, vec3 max) {
    return { min, max };
}

static irect2 ir2(i32 min_x, i32 min_y, i32 max_x, i32 max_y) {
    return { min_x, min_y, max_x, max_y };
}

static irect2 ir2(ivec2 min, ivec2 max) {
    return { min, max };
}

static irect3 ir3(i32 min_x, i32 min_y, i32 min_z, i32 max_x, i32 max_y, i32 max_z) {
    return { min_x, min_y, min_z, max_x, max_y, max_z };
}

static irect3 ir3(ivec3 min, ivec3 max) {
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

static vec2 v2(const f32* a) { return v2(a[0], a[1]); }
static vec3 v3(const f32* a) { return v3(a[0], a[1], a[2]); }
static vec4 v4(const f32* a) { return v4(a[0], a[1], a[2], a[3]); }

static ivec2 iv2(const i32* a) { return iv2(a[0], a[1]); }
static ivec3 iv3(const i32* a) { return iv3(a[0], a[1], a[2]); }
static ivec4 iv4(const i32* a) { return iv4(a[0], a[1], a[2], a[3]); }

// ---------- from color ------------ //

static vec3 v3_from_packed_color(u32 color) {
    return v3(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f);
}

static vec4 v4_from_packed_color(u32 color) {
    return v4(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f,
              ((color & 0xff000000) >> 24) / 256.0f);
}

// --------- negate ---------- //

static vec2 operator-(vec2 u) { return v2(-u.x, -u.y); }
static vec3 operator-(vec3 u) { return v3(-u.x, -u.y, -u.z); }
static vec4 operator-(vec4 u) { return v4(-u.x, -u.y, -u.z, -u.w); }

static ivec2 operator-(ivec2 u) { return iv2(-u.x, -u.y); }
static ivec3 operator-(ivec3 u) { return iv3(-u.x, -u.y, -u.z); }
static ivec4 operator-(ivec4 u) { return iv4(-u.x, -u.y, -u.z, -u.w); }

// ---------- addition ---------- //

static vec2 operator+(vec2 a, vec2 b) { return v2(a.x + b.x, a.y + b.y); }
static vec3 operator+(vec3 a, vec3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static vec4 operator+(vec4 a, vec4 b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static ivec2 operator+(ivec2 a, ivec2 b) { return iv2(a.x + b.x, a.y + b.y); }
static ivec3 operator+(ivec3 a, ivec3 b) { return iv3(a.x + b.x, a.y + b.y, a.z + b.z); }
static ivec4 operator+(ivec4 a, ivec4 b) { return iv4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static vec2 operator+=(vec2& a, vec2 b) { return a = a + b; }
static vec3 operator+=(vec3& a, vec3 b) { return a = a + b; }
static vec4 operator+=(vec4& a, vec4 b) { return a = a + b; }

static ivec2 operator+=(ivec2& a, ivec2 b) { return a = a + b; }
static ivec3 operator+=(ivec3& a, ivec3 b) { return a = a + b; }
static ivec4 operator+=(ivec4& a, ivec4 b) { return a = a + b; }

// -------- subtraction ------- //

static vec2 operator-(vec2 a, vec2 b) { return v2(a.x - b.x, a.y - b.y); }
static vec3 operator-(vec3 a, vec3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static vec4 operator-(vec4 a, vec4 b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static ivec2 operator-(ivec2 a, ivec2 b) { return iv2(a.x - b.x, a.y - b.y); }
static ivec3 operator-(ivec3 a, ivec3 b) { return iv3(a.x - b.x, a.y - b.y, a.z - b.z); }
static ivec4 operator-(ivec4 a, ivec4 b) { return iv4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static vec2 operator-=(vec2& a, vec2 b) { return a = a - b; }
static vec3 operator-=(vec3& a, vec3 b) { return a = a - b; }
static vec4 operator-=(vec4& a, vec4 b) { return a = a - b; }

static ivec2 operator-=(ivec2& a, ivec2 b) { return a = a - b; }
static ivec3 operator-=(ivec3& a, ivec3 b) { return a = a - b; }
static ivec4 operator-=(ivec4& a, ivec4 b) { return a = a - b; }

// -------- multiplication ------- //

static vec2 operator*(vec2 a, vec2 b) { return v2(a.x * b.x, a.y * b.y); }
static vec3 operator*(vec3 a, vec3 b) { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
static vec4 operator*(vec4 a, vec4 b) { return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * a.w); }

static ivec2 operator*(ivec2 a, ivec2 b) { return iv2(a.x * b.x, a.y * b.y); }
static ivec3 operator*(ivec3 a, ivec3 b) { return iv3(a.x * b.x, a.y * b.y, a.z * a.z); }
static ivec4 operator*(ivec4 a, ivec4 b) { return iv4(a.x * b.x, a.y * b.y, a.z * a.z, a.w * a.w); }

static vec2 operator*=(vec2& a, vec2 b) { return a = a * b; }
static vec3 operator*=(vec3& a, vec3 b) { return a = a * b; }
static vec4 operator*=(vec4& a, vec4 b) { return a = a * b; }

static ivec2 operator*=(ivec2& a, ivec2 b) { return a = a * b; }
static ivec3 operator*=(ivec3& a, ivec3 b) { return a = a * b; }
static ivec4 operator*=(ivec4& a, ivec4 b) { return a = a * b; }

static vec2 operator*(mat2 m, vec2 u) {
    return {
        m.e[0] * u.x + m.e[2] * u.y,
        m.e[1] * u.x + m.e[3] * u.y
    };
}

static vec3 operator*(mat3 m, vec3 u) {
    return {
        m.e[0] * u.x + m.e[3] * u.y + m.e[6] * u.z,
        m.e[1] * u.x + m.e[4] * u.y + m.e[7] * u.z,
        m.e[2] * u.x + m.e[5] * u.y + m.e[8] * u.z
    };
}

static vec4 operator*(mat4 m, vec4 u) {
    return {
        m.e[0] * u.x + m.e[4] * u.y + m.e[8]  * u.z + m.e[12] * u.w,
        m.e[1] * u.x + m.e[5] * u.y + m.e[9]  * u.z + m.e[13] * u.w,
        m.e[2] * u.x + m.e[6] * u.y + m.e[10] * u.z + m.e[14] * u.w,
        m.e[3] * u.x + m.e[7] * u.y + m.e[11] * u.z + m.e[15] * u.w
    };
}

static mat2 operator*(mat2 a, mat2 b) {
    return {
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3]
    };
}

static mat3 operator*(mat3 a, mat3 b) {
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

static mat4 operator*(mat4 a, mat4 b) {
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

static quat_t operator*(quat_t a, quat_t b) {
    return quat(
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

// ------------ divition ------------ //

static vec2 operator/(vec2 a, vec2 b) { return v2(a.x / b.x, a.y / b.y); }
static vec3 operator/(vec3 a, vec3 b) { return v3(a.x / b.x, a.y / b.y, a.z / b.z); }
static vec4 operator/(vec4 a, vec4 b) { return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static ivec2 operator/(ivec2 a, ivec2 b) { return iv2(a.x / b.x, a.y / b.y); }
static ivec3 operator/(ivec3 a, ivec3 b) { return iv3(a.x / b.x, a.y / b.y, a.z / b.z); }
static ivec4 operator/(ivec4 a, ivec4 b) { return iv4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static vec2 operator/=(vec2& a, vec2 b) { return a = a / b; }
static vec3 operator/=(vec3& a, vec3 b) { return a = a / b; }
static vec4 operator/=(vec4& a, vec4 b) { return a = a / b; }

static ivec2 operator/=(ivec2& a, ivec2 b) { return a = a / b; }
static ivec3 operator/=(ivec3& a, ivec3 b) { return a = a / b; }
static ivec4 operator/=(ivec4& a, ivec4 b) { return a = a / b; }

// ------------- scaling ------------- //

static vec2 operator*(vec2 a, f32 s) { return v2(a.x * s, a.y * s); }
static vec3 operator*(vec3 a, f32 s) { return v3(a.x * s, a.y * s, a.z * s); }
static vec4 operator*(vec4 a, f32 s) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }

static vec2 operator*(f32 s, vec2 a) { return v2(a.x * s, a.y * s); }
static vec3 operator*(f32 s, vec3 a) { return v3(a.x * s, a.y * s, a.z * s); }
static vec4 operator*(f32 s, vec4 a) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }

static vec2 operator/(vec2 a, f32 s) { return v2(a.x / s, a.y / s); }
static vec3 operator/(vec3 a, f32 s) { return v3(a.x / s, a.y / s, a.z / s); }
static vec4 operator/(vec4 a, f32 s) { return v4(a.x / s, a.y / s, a.z / s, a.w / s); }

static ivec2 operator*(ivec2 a, i32 s) { return iv2(a.x * s, a.y * s); }
static ivec3 operator*(ivec3 a, i32 s) { return iv3(a.x * s, a.y * s, a.z * s); }
static ivec4 operator*(ivec4 a, i32 s) { return iv4(a.x * s, a.y * s, a.z * s, a.w * s); }

static ivec2 operator*(i32 s, ivec2 a) { return iv2(a.x * s, a.y * s); }
static ivec3 operator*(i32 s, ivec3 a) { return iv3(a.x * s, a.y * s, a.z * s); }
static ivec4 operator*(i32 s, ivec4 a) { return iv4(a.x * s, a.y * s, a.z * s, a.w * s); }

static ivec2 operator/(ivec2 a, i32 s) { return iv2(a.x / s, a.y / s); }
static ivec3 operator/(ivec3 a, i32 s) { return iv3(a.x / s, a.y / s, a.z / s); }
static ivec4 operator/(ivec4 a, i32 s) { return iv4(a.x / s, a.y / s, a.z / s, a.w / s); }

static vec2 operator*=(vec2& a, f32 s) { return a = a * s; }
static vec3 operator*=(vec3& a, f32 s) { return a = a * s; }
static vec4 operator*=(vec4& a, f32 s) { return a = a * s; }

static vec2 operator/=(vec2& a, f32 s) { return a = a / s; }
static vec3 operator/=(vec3& a, f32 s) { return a = a / s; }
static vec4 operator/=(vec4& a, f32 s) { return a = a / s; }

static ivec2 operator*=(ivec2& a, f32 s) { return a = a * s; }
static ivec3 operator*=(ivec3& a, f32 s) { return a = a * s; }
static ivec4 operator*=(ivec4& a, f32 s) { return a = a * s; }

static ivec2 operator/=(ivec2& a, f32 s) { return a = a / s; }
static ivec3 operator/=(ivec3& a, f32 s) { return a = a / s; }
static ivec4 operator/=(ivec4& a, f32 s) { return a = a / s; }

// ----------- eq ------------ //

static bool operator==(ivec2 a, ivec2 b) { return a.x == b.x && a.y == b.y; }
static bool operator==(ivec3 a, ivec3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static bool operator==(ivec4 a, ivec4 b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static bool operator!=(ivec2 a, ivec2 b) { return a.x != b.x || a.y != b.y; }
static bool operator!=(ivec3 a, ivec3 b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static bool operator!=(ivec4 a, ivec4 b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

// ------------- lt --------------- //

static bvec2 operator<(vec2 a, vec2 b) { return bv2(a.x < b.x, a.y < b.y); }
static bvec3 operator<(vec3 a, vec3 b) { return bv3(a.x < b.x, a.y < b.y, a.z < b.z); }
static bvec4 operator<(vec4 a, vec4 b) { return bv4(a.x < b.x, a.y < b.y, a.z < b.z, a.w < a.w); }

static bvec2 operator<(ivec2 a, ivec2 b) { return bv2(a.x < b.x, a.y < b.y); }
static bvec3 operator<(ivec3 a, ivec3 b) { return bv3(a.x < b.x, a.y < b.y, a.z < b.z); }
static bvec4 operator<(ivec4 a, ivec4 b) { return bv4(a.x < b.x, a.y < b.y, a.z < b.z, a.w < a.w); }

// ------------- bt --------------- //

static bvec2 operator>(vec2 a, vec2 b) { return bv2(a.x > b.x, a.y > b.y); }
static bvec3 operator>(vec3 a, vec3 b) { return bv3(a.x > b.x, a.y > b.y, a.z > b.z); }
static bvec4 operator>(vec4 a, vec4 b) { return bv4(a.x > b.x, a.y > b.y, a.z > b.z, a.w < a.w); }

static bvec2 operator>(ivec2 a, ivec2 b) { return bv2(a.x > b.x, a.y > b.y); }
static bvec3 operator>(ivec3 a, ivec3 b) { return bv3(a.x > b.x, a.y > b.y, a.z > b.z); }
static bvec4 operator>(ivec4 a, ivec4 b) { return bv4(a.x > b.x, a.y > b.y, a.z > b.z, a.w > a.w); }

// ------------- lteq --------------- //

static bvec2 operator<=(vec2 a, vec2 b) { return bv2(a.x <= b.x, a.y <= b.y); }
static bvec3 operator<=(vec3 a, vec3 b) { return bv3(a.x <= b.x, a.y <= b.y, a.z < b.z); }
static bvec4 operator<=(vec4 a, vec4 b) { return bv4(a.x <= b.x, a.y <= b.y, a.z < b.z, a.w < a.w); }

static bvec2 operator<=(ivec2 a, ivec2 b) { return bv2(a.x <= b.x, a.y <= b.y); }
static bvec3 operator<=(ivec3 a, ivec3 b) { return bv3(a.x <= b.x, a.y <= b.y, a.z <= b.z); }
static bvec4 operator<=(ivec4 a, ivec4 b) { return bv4(a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= a.w); }

// ------------- bteq --------------- //

static bvec2 operator>=(vec2 a, vec2 b) { return bv2(a.x >= b.x, a.y >= b.y); }
static bvec3 operator>=(vec3 a, vec3 b) { return bv3(a.x >= b.x, a.y >= b.y, a.z > b.z); }
static bvec4 operator>=(vec4 a, vec4 b) { return bv4(a.x >= b.x, a.y >= b.y, a.z > b.z, a.w < a.w); }

static bvec2 operator>=(ivec2 a, ivec2 b) { return bv2(a.x >= b.x, a.y >= b.y); }
static bvec3 operator>=(ivec3 a, ivec3 b) { return bv3(a.x >= b.x, a.y >= b.y, a.z >= b.z); }
static bvec4 operator>=(ivec4 a, ivec4 b) { return bv4(a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= a.w); }

// ------------- any ---------------- //

static bool any(bvec2 a) { return a.x || a.y;  }
static bool any(bvec3 a) { return a.x || a.y || a.z;  }
static bool any(bvec4 a) { return a.x || a.y || a.z || a.w;  }

// ------------- all ---------------- //

static bool all(bvec2 a) { return a.x && a.y;  }
static bool all(bvec3 a) { return a.x && a.y && a.z;  }
static bool all(bvec4 a) { return a.x && a.y && a.z && a.w;  }

// ------------- not ---------------- //

static bvec2 not(bvec2 a) { return bv2(!a.x, !a.y);  }
static bvec3 not(bvec3 a) { return bv3(!a.x, !a.y, !a.z);  }
static bvec4 not(bvec4 a) { return bv4(!a.x, !a.y, !a.z, !a.w);  }

// ----------- dot product ----------- //

static f32 dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
static f32 dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 dot(vec4 a, vec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static i32 dot(ivec2 a, ivec2 b) { return a.x * b.x + a.y * b.y; }
static i32 dot(ivec3 a, ivec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static i32 dot(ivec4 a, ivec4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// ----------- length squared ----------- //

static f32 len_sq(vec2 u) { return dot(u, u); }
static f32 len_sq(vec3 u) { return dot(u, u); }
static f32 len_sq(vec4 u) { return dot(u, u); }

static i32 len_sq(ivec2 u) { return dot(u, u); }
static i32 len_sq(ivec3 u) { return dot(u, u); }
static i32 len_sq(ivec4 u) { return dot(u, u); }

// -------------- length -------------- //

static f32 len(vec2 u) { return sqrt32(len_sq(u)); }
static f32 len(vec3 u) { return sqrt32(len_sq(u)); }
static f32 len(vec4 u) { return sqrt32(len_sq(u)); }

static f32 len(ivec2 u) { return sqrt32(len_sq(u)); }
static f32 len(ivec3 u) { return sqrt32(len_sq(u)); }
static f32 len(ivec4 u) { return sqrt32(len_sq(u)); }

// -------------- distance squared -------------- //

static f32 dist_sq(vec2 a, vec2 b) { return len_sq(a - b); }
static f32 dist_sq(vec3 a, vec3 b) { return len_sq(a - b); }
static f32 dist_sq(vec4 a, vec4 b) { return len_sq(a - b); }

static i32 dist_sq(ivec2 a, ivec2 b) { return len_sq(a - b); }
static i32 dist_sq(ivec3 a, ivec3 b) { return len_sq(a - b); }
static i32 dist_sq(ivec4 a, ivec4 b) { return len_sq(a - b); }

// ------------------ distance ------------------- //

static f32 dist(vec2 a, vec2 b) { return sqrt32(dist_sq(a, b)); }
static f32 dist(vec3 a, vec3 b) { return sqrt32(dist_sq(a, b)); }
static f32 dist(vec4 a, vec4 b) { return sqrt32(dist_sq(a, b)); }

// -------------- manhattan distance -------------- //

static i32 manhattan(ivec2 a, ivec2 b) {
    ivec2 diff = a - b;
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y);
}

static i32 manhattan(ivec3 a, ivec3 b) {
    ivec3 diff = a - b;
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y) + (0x7ffffffff & diff.z);
}

// -------------- normalize --------------- //

static vec2 normalize(vec2 u) { return u * rsqrt32(dot(u, u)); }
static vec3 normalize(vec3 u) { return u * rsqrt32(dot(u, u)); }
static vec4 normalize(vec4 u) { return u * rsqrt32(dot(u, u)); }

// -------------- floor --------------- //

static vec2 floor(vec2 u) { return v2(floorf(u.x), floorf(u.y)); }
static vec3 floor(vec3 u) { return v3(floorf(u.x), floorf(u.y), floorf(u.z)); }
static vec4 floor(vec4 u) { return v4(floorf(u.x), floorf(u.y), floorf(u.z), floorf(u.w)); }

// -------------- ceil --------------- //

static vec2 ceil(vec2 u) { return v2(ceilf(u.x), ceilf(u.y)); }
static vec3 ceil(vec3 u) { return v3(ceilf(u.x), ceilf(u.y), ceilf(u.z)); }
static vec4 ceil(vec4 u) { return v4(ceilf(u.x), ceilf(u.y), ceilf(u.z), ceilf(u.w)); }

// -------------- clamp_min --------------- //

static f32 clamp_min(f32 n, f32 min) { return n < min? min : n; }
static i32 clamp_min(i32 n, i32 min) { return n < min? min : n; }

// -------------- clamp_max --------------- //

static f32 clamp_max(f32 n, f32 max) { return n > max? max : n; }
static i32 clamp_max(i32 n, i32 max) { return n > max? max : n; }

// -------------- clamp --------------- //

static f32 clamp(f32 n, f32 min, f32 max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

static i32 clamp(i32 n, i32 min, i32 max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

static vec2 clamp(vec2 u, rect2 r) {
    return {
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y)
    };
}

static vec3 clamp(vec3 u, rect3 r) {
    return {
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y),
        clamp(u.z, r.min.z, r.max.z)
    };

}

static ivec2 clamp(ivec2 u, irect2 r) {
    return {
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y)
    };
}

static ivec3 clamp(ivec3 u, irect3 r) {
    return {
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y),
        clamp(u.z, r.min.z, r.max.z)
    };
}

// ---------------- min ----------------- //

static vec2 min(vec2 a, vec2 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y
    };
}

static vec3 min(vec3 a, vec3 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z
    };
}

static vec4 min(vec4 a, vec4 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w
    };
}

static ivec2 min(ivec2 a, ivec2 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y
    };
}

static ivec3 min(ivec3 a, ivec3 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z
    };
}

static ivec4 min(ivec4 a, ivec4 b) {
    return {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w
    };
}

// ---------------- max ----------------- //

static vec2 max(vec2 a, vec2 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y
    };
}

static vec3 max(vec3 a, vec3 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z
    };
}

static vec4 max(vec4 a, vec4 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w
    };
}

static ivec2 max(ivec2 a, ivec2 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y
    };
}

static ivec3 max(ivec3 a, ivec3 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z
    };
}

static ivec4 v4i_max(ivec4 a, ivec4 b) {
    return {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w
    };
}

// ---------------- lerp ----------------- //

static f32 lerp(f32 a, f32 b, f32 t) { return a + t * (b - a); }

static vec2 lerp(vec2 a, vec2 b, f32 t) { return a + t * (b - a); }
static vec3 lerp(vec3 a, vec3 b, f32 t) { return a + t * (b - a); }
static vec4 lerp(vec4 a, vec4 b, f32 t) { return a + t * (b - a); }

// -------------- sign (-1, 0, 1) ------------------- //

static f32 sign(f32 n) { return n == 0? 0 : (n < 0? -1 : 1); }
static i32 sign(i32 n) { return n == 0? 0 : (n < 0? -1 : 1); }

static vec2 sign(vec2 u) { return { sign(u.x), sign(u.y) }; }
static vec3 sign(vec3 u) { return { sign(u.x), sign(u.y), sign(u.z) }; }
static vec4 sign(vec4 u) { return { sign(u.x), sign(u.y), sign(u.z), sign(u.w) }; }

static ivec2 sign(ivec2 u) { return { sign(u.x), sign(u.y) }; }
static ivec3 sign(ivec3 u) { return { sign(u.x), sign(u.y), sign(u.z) }; }
static ivec4 sign(ivec4 u) { return { sign(u.x), sign(u.y), sign(u.z), sign(u.w) }; }

// --------------- cross ------------------- //

static vec3 cross(vec3 a, vec3 b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// --------------- get angle ------------------- //

static f32 get_angle(vec2 a, vec2 b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;

    return atan2f(det, dot);
}

// ----------- keep min ---------- //

static vec3 keep_min(vec3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);

    if (dx <= dy && dx <= dz) return v3(u.x, 0, 0);
    if (dy <= dx && dy <= dz) return v3(0, u.y, 0);
    if (dz <= dx && dz <= dy) return v3(0, 0, u.z);

    return u;
}

// ----------- mask min ---------- //

static vec3 mask_min(vec3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);

    if (dx <= dy && dx <= dz) return v3(0, 1, 1);
    if (dy <= dx && dy <= dz) return v3(1, 0, 1);
    if (dz <= dx && dz <= dy) return v3(1, 1, 0);

    return v3(1, 1, 1);
}

// ------------------ transform/scale/rotate ------------------ //

static mat2 m2_rotate(f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return { c, s, -s, c };
}

static mat3 m3_rotate(vec3 axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    f32 k = 1.0f - c;

    vec3 sa   = { s * axis.x, s * axis.y, s * axis.z };
    vec3 omca = { k * axis.x, k * axis.y, k * axis.z };

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

static mat4 m4_rotate(vec3 axis, f32 angle) {
    f32 cosv = cosf(angle);
    f32 sinv = sinf(angle);
    f32 inv_cosv = 1.0f - cosv;

    vec3 sa   = { axis.x * sinv,      axis.y * sinv,      axis.z * sinv };
    vec3 omca = { axis.x * inv_cosv,  axis.y * inv_cosv,  axis.z * inv_cosv };

    return {
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0,  0,  0,  1
    };
}

static quat_t quat_rotate(vec3 axis, f32 angle) {
    f32  s = sinf(0.5f * angle);
    vec3 v = { s * axis.x, s * axis.y, s * axis.z };

    return { v.x, v.y, v.z, cosf(0.5f * angle) };
}

static mat4 m4_translate(f32 x, f32 y, f32 z) {
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
}

static mat4 m4_scale(f32 x, f32 y, f32 z) {
    return {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

// --------------- from quat --------------- //

static mat3 m3_from_quat(quat_t q) {
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

static mat4 m4_from_quat(quat_t q) {
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

static mat4 m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return {
        2 / (r - l),            0,                      0,                      0,
        0,                      2 / (t - b),            0,                      0,
        0,                      0,                      -2 / (f - n),           0,
        -(r + l) / (r - l),     -(t + b) / (t - b),     -(f + n) / (f - n),     1
    };
}

static mat4 m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return {
        a / aspect,     0,      0,                          0,
        0,              a,      0,                          0,
        0,              0,      -((f + n) / (f - n)),      -1,
        0,              0,      -((2 * f * n) / (f - n)),   0
    };
}

static mat4 m4_look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 t = cross(s, f);
    mat4 m;

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

static plane_t normalize(plane_t plane) {
    f32 mag = sqrt32(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;

    return plane;
}

static frustum_t frustum_create(mat4 m) {
    frustum_t result;

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

    result.plane[0] = normalize(result.plane[0]);
    result.plane[1] = normalize(result.plane[1]);
    result.plane[2] = normalize(result.plane[2]);
    result.plane[3] = normalize(result.plane[3]);
    result.plane[4] = normalize(result.plane[4]);
    result.plane[5] = normalize(result.plane[5]);
    
    return result;
}

// ------------------ contains ------------------ //

static bool contains(circle_t c, vec2 pos) {
    f32 distance = dist_sq(c.pos, pos);
    return distance < (c.rad * c.rad);
}

static bool contains(sphere_t s, vec3 pos) {
    f32 distance = dist_sq(s.pos, pos);
    return distance < (s.rad * s.rad);
}

static bool contains(rect2 rect, vec2 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static bool contains(rect3 rect, vec3 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static bool contains(irect2 rect, ivec2 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static bool contains(irect3 rect, ivec3 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static bool contains(frustum_t fs, vec3 pos) {
    for (i32 i = 0; i < 6; i++) {
		if (fs.plane[i].a * pos.x + fs.plane[i].b * pos.y + fs.plane[i].c * pos.z + fs.plane[i].d <= 0)
			return false;
	}

	return true;
}

// ------------------ intersect ------------------ //

static bool intersect(circle_t a, circle_t b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static bool intersect(sphere_t a, sphere_t b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 dz  = b.pos.z - a.pos.z;

    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static bool intersect(rect2 a, rect2 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static bool intersect(rect3 a, rect3 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static bool intersect(irect2 a, irect2 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static bool intersect(irect3 a, irect3 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static bool intersect(frustum_t fs, sphere_t sphere) {
    for (i32 i = 0; i < 6; i++) {
		if(fs.plane[i].a * sphere.pos.x + fs.plane[i].b * sphere.pos.y + fs.plane[i].c * sphere.pos.z + fs.plane[i].d <= -sphere.rad) {
			return false;
		}
	}
	return true;
}

static bool intersect(frustum_t fs, rect3 rect) {
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

static rect2 get_overlap(rect2 a, rect2 b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static rect3 get_overlap(rect3 a, rect3 b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static irect2 get_overlap(irect2 a, irect2 b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

static irect3 get_overlap(irect3 a, irect3 b) {
    return {
        max(a.min, b.min),
        min(a.max, b.max)
    };
}

// -------------- get intersect vector ---------- //

static vec2 get_intersect_vector(circle_t a, circle_t b) {
    vec2 delta = a.pos - b.pos;
    f32  depth = len(delta) - (a.rad + b.rad);
    
    return -depth * delta;
}

static vec3 get_intersect_vector(sphere_t a, sphere_t b) {
    vec3 delta = a.pos - b.pos;
    f32  depth = len(delta) - (a.rad + b.rad);
    
    return -depth * delta;
}

static vec2 get_intersect_vector(rect2 a, rect2 b) {
    rect2   overlap = get_overlap(a, b);
    vec2    delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);
    return sign(delta) * (overlap.max - overlap.min);
}

static vec3 get_intersect_vector(rect3 a, rect3 b) {
    rect3   overlap = get_overlap(a, b);
    vec3    delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);
    return sign(delta) * (overlap.max - overlap.min);
}

static ivec2 get_intersect_vector(irect2 a, irect2 b) {
    irect2  overlap = get_overlap(a, b);
    ivec2   delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);

    return sign(delta) * (overlap.max - overlap.min);
}

static ivec3 get_intersect_vector(irect3 a, irect3 b) {
    irect3  overlap = get_overlap(a, b);
    ivec3   delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);

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

static vec2 rand_v2(u32* state) {
    vec2 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return normalize(out);
}

static vec3 rand_v3(u32* state) {
    vec3 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return normalize(out);
}

static vec2 rand_v2(u32* state, f32 min, f32 max) {
    return rand_v2(state) * rand_f32(state, min, max);
}

static vec3 rand_v3(u32* state, f32 min, f32 max) {
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

static u32 hash_mem(const void *data, u32 size) {
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

static u32 hash_iv2(ivec2 k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

static u32 hash_iv3(ivec3 k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

static u32 hash_iv4(ivec4 k) {
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

static u32 pack_color_v4(vec4 color) {
    return pack_color_f32(color.r, color.g, color.b, color.a);
}

static u32 pack_color_v3(vec3 color, f32 a = 1.0) {
    return pack_color_f32(color.r, color.g, color.b, a);
}

// ---------------------- priority queue --------------------- //

static b32 queue_empty(const priority_queue* queue) {
    return queue->len == 0;
}

static void queue_clear(priority_queue* queue) {
    queue->len = 0;
}

static void queue_push(priority_queue* queue, ivec2 e, f32 weight) {
    queue_node node = { weight, e };

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

static f32 queue_pop(ivec2* out, priority_queue* queue) {
    queue_node data = queue->array[1];

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

static void zero_memory(void* data, usize size) {
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

extern memory_arena ma_create(u8* buffer, usize size) {
    memory_arena ma = {};

    ma.cap    = size;
    ma.buffer = buffer;

    return ma;
}

extern void* ma_alloc(memory_arena* ma, usize byte_size) {
    byte_size = align_up(byte_size, 16);

    assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

    void* memory  = ma->buffer + ma->index;
    ma->index     += byte_size;
    ma->max       = ma->max > ma->index? ma->max : ma->index;

    zero_memory(memory, byte_size);

    return memory;
}

extern void* ma_begin(memory_arena* ma) {
    ma->lock = true;
    return ma->buffer + ma->index;
}

extern void ma_end(memory_arena* ma, usize byte_size) {
    ma->index += align_up(byte_size, 16);
    ma->lock = false;
}

extern void ma_save(memory_arena* ma) {
    assert(ma->top < ma->cap);
    ma->stack[ma->top++] = ma->index;
}

extern void ma_restore(memory_arena* ma) {
    assert(ma->top > 0);
    ma->index = ma->stack[--ma->top];
}

extern void ma_validate(memory_arena* ma) {
    assert(ma->top == 0);
}

// --------------------- read/write files ------------------------- //

static usize file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    usize size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

extern char* file_read_str(const char* file_name, memory_arena* ma) {
    FILE *fp      = NULL;
    char *buffer  = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        usize size = file_get_size(fp);

        memory_arena state = *ma;
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

extern image_t image_load_from_file(const char* path) {
    image_t image       = {};
    i32     channels    = 0;

    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);

    return image;
}

#endif

extern u32 image_get_pixel(image_t* img, i32 x, i32 y) {
    return img->pixels[y * img->width + x];
}

extern void image_set_pixel(image_t* img, i32 x, i32 y, u32 pixel) {
    img->pixels[y * img->width + x] = pixel;
}

#endif // ATS_IMPL
