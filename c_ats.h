#pragma once
// defines that enables different parts of the library:
// #define ATS_PLATFORM_GLFW
// #define ATS_MODERN_OPENGL 

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#ifdef ATS_MODERN_OPENGL
#include "dep/glad/glad.h"
#endif

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#ifndef PI
#define PI (3.14159265359f)
#endif

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define TO_RAD(deg) ((deg) * TO_RAD_MUL)
#define TO_DEG(rad) ((rad) * TO_DEG_MUL)

#define IS_POWER_OF_TWO(x)  (((x) != 0) && ((x) & ((x)-1)) == 0)

#define ALIGN_DOWN(n, a)      ((n) & ~((a) - 1))
#define ALIGN_UP(n, a)        ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a)  ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a)    ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))

#define ARRAY_COUNT(array) (sizeof (array) / sizeof ((array)[0]))

#define GLSL_SHADER(shader) "#version 330 core\n" #shader

#define join_token(a, b)    a##b
#define macro_var(a)        join_token(a, __LINE__)

#define defer(start, end) for (int macro_var(i) = ((start), 0); \
                               !macro_var(i); \
                               (macro_var(i)++, (end)))

// ================================================== TYPES ================================================= //

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uint8_t b8;
typedef uint16_t b16;
typedef uint32_t b32;
typedef uint64_t b64;

typedef union Vec2 Vec2;
union Vec2 {
    struct {
        f32     x;
        f32     y;
    };

    f32 e[2];
};

typedef union Vec3 Vec3;
union Vec3 {
    struct {
        f32     x;
        f32     y;
        f32     z;
    };

    struct {
        Vec2 xy;
    };

    struct {
        f32     r;
        f32     g;
        f32     b;
    };

    f32 e[3];
};

typedef union Vec4 Vec4;
union Vec4 {
    struct {
        f32     x;
        f32     y;
        f32     z;
        f32     w;
    };

    struct {
        Vec2 xy;
    };

    struct {
        Vec3 xyz;
    };

    struct {
        f32     r;
        f32     g;
        f32     b;
        f32     a;
    };

    f32 e[4];
};

typedef union Vec2i Vec2i;
union Vec2i {
    struct {
        i32     x;
        i32     y;
    };

    i32 e[2];
};

typedef union Vec3i Vec3i;
union Vec3i {
    struct {
        i32     x;
        i32     y;
        i32     z;
    };

    i32 e[3];
};

typedef union Vec4i Vec4i;
union Vec4i {
    struct {
        i32     x;
        i32     y;
        i32     z;
        i32     w;
    };

    i32 e[4];
};

typedef union Mat2 Mat2;
union Mat2 {
    struct {
        Vec2  x;
        Vec2  y;
    };

    f32 e[4];
};

typedef union Mat3 Mat3;
union Mat3 {
    struct {
        Vec3  x;
        Vec3  y;
        Vec3  z;
    };

    f32 e[9];
};

typedef union Mat4 Mat4;
union Mat4 {
    struct {
        Vec4  x;
        Vec4  y;
        Vec4  z;
        Vec4  w;
    };

    f32 e[16];
};

typedef struct Quat Quat;
struct Quat {
    f32     x;
    f32     y;
    f32     z;
    f32     w;
};

#define v2(...)     ((Vec2) { __VA_ARGS__ })
#define v3(...)     ((Vec3) { __VA_ARGS__ })
#define v4(...)     ((Vec4) { __VA_ARGS__ })

#define v2i(...)    ((Vec2i) { __VA_ARGS__ })
#define v3i(...)    ((Vec3i) { __VA_ARGS__ })
#define v4i(...)    ((Vec4i) { __VA_ARGS__ })

#define v2_cast(type, u) ((type) { .x = (u).x, .y = (u).y })
#define v3_cast(type, u) ((type) { .x = (u).x, .y = (u).y, .z = (u).z })
#define v4_cast(type, u) ((type) { .x = (u).x, .y = (u).y, .z = (u).z, .w = (u).w })

// --------------------------------------------- FUNCTIONS ---------------------------------------- //

#define sqrt  __sqrt
#define rsqrt __rsqrt

static f32 __sqrt(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

static f32 __rsqrt(f32 n) {
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

static f32 spline(f32 f, f32 a, f32 b, f32 c, f32 d) {
	f32 i = 1.0f - f;
	return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

#undef min
#undef max

#define min(a, b)   ((a) < (b)? (a) : (b))
#define max(a, b)   ((a) > (b)? (a) : (b))

#define clamp_min(n, lo)    ((n) < (lo)? (lo) : (n))
#define clamp_max(n, hi)    ((n) > (hi)? (hi) : (n))
#define clamp(n, lo, hi)    clamp_min(clamp_max(n, (hi)), (lo))

#define sign(n)             ((n) < 0? -1 : 1)
#define sign_or_zero(n)     ((n) < 0? -1 : ((n) > 0? 1 : 0))

#define square(n)       ((n) * (n))
#define lerp(a, b, t)   ((a) + (t) * ((b) - (a)))
#define unlerp(a, b, t) (((t) - (a)) / ((b) - (a)))

// ==================================================  MATHS ================================================== //

// ---------------------- v2 ---------------------- //

static Vec2 v2_from_array(const f32* a) {
    return (Vec2) { a[0], a[1] };
}

static Vec2 v2_neg(Vec2 a) {
    return (Vec2) { -a.x, -a.y };
}

static Vec2 v2_add(Vec2 a, Vec2 b) {
    return (Vec2) { a.x + b.x, a.y + b.y };
}

static Vec2 v2_sub(Vec2 a, Vec2 b) {
    return (Vec2) { a.x - b.x, a.y - b.y };
}

static Vec2 v2_mul(Vec2 a, Vec2 b){
    return (Vec2) { a.x * b.x, a.y * b.y };
}

static Vec2 v2_scale(Vec2 a, f32 s) {
    return (Vec2) { a.x * s, a.y * s };
}

static f32 v2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

static f32 v2_det(Vec2 a, Vec2 b) {
    return a.x * b.y - a.y * b.x;
}

static f32 v2_len_sq(Vec2 v) {
    return v2_dot(v, v);
}

static f32 v2_len(Vec2 v) {
    return sqrt(v2_dot(v, v));
}

static f32 v2_dist_sq(Vec2 a, Vec2 b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;

    return dx * dx + dy * dy;
}

static f32 v2_dist(Vec2 a, Vec2 b) {
    return sqrt(v2_dist_sq(a, b));
}

static Vec2 v2_proj(Vec2 a, Vec2 b) {
    f32 k = v2_dot(a, a) / v2_dot(b, b);

    return (Vec2) { k * b.x, k * b.y };
}

static Vec2 v2_norm(Vec2 v) {
    f32 k = rsqrt(v2_dot(v, v));

    return (Vec2) { k * v.x, k * v.y };
}

static Vec2 v2_min(Vec2 a, Vec2 b) {
    return (Vec2) {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
    };
}

static Vec2 v2_max(Vec2 a, Vec2 b) {
    return (Vec2) {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
    };
}

static Vec2 v2_lerp(Vec2 a, Vec2 b, f32 t) {
    return (Vec2) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
    };
}

static f32 v2_get_angle(Vec2 a, Vec2 b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;
    
    return atan2f(det, dot);
}

static Vec2 v2_spline(f32 f, Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
	f32 i = 1.0f - f;

    return (Vec2) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
    };
}

// ----------------------------- v3 ----------------------------- //

static Vec3 v3_from_array(const f32* a) {
    return (Vec3) { a[0], a[1], a[2] };
}

static Vec3 v3_from_packed_color(u32 color) {
    return (Vec3) {
        ((color & 0x000000ff) >> 0)  / 256.0f,
        ((color & 0x0000ff00) >> 8)  / 256.0f,
        ((color & 0x00ff0000) >> 16) / 256.0f,
    };
}

static Vec3 v3_neg(Vec3 a) {
    return (Vec3) { -a.x, -a.y, -a.z };
}

static Vec3 v3_add(Vec3 a, Vec3 b) {
    return (Vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static Vec3 v3_sub(Vec3 a, Vec3 b) {
    return (Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static Vec3 v3_mul(Vec3 a, Vec3 b) {
    return (Vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

static Vec3 v3_scale(Vec3 a, f32 s) {
    return (Vec3) { a.x * s, a.y * s, a.z * s };
}

static f32 v3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static f32 v3_len_sq(Vec3 v) {
    return v3_dot(v, v);
}

static f32 v3_len(Vec3 v) {
    return sqrt(v3_dot(v, v));
}

static f32 v3_dist_sq(Vec3 a, Vec3 b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

static f32 v3_dist(Vec3 a, Vec3 b) {
    return sqrt(v3_dist_sq(a, b));
}

static Vec3 v3_proj(Vec3 a, Vec3 b) {
    f32 k = v3_dot(a, b) / v3_dot(b, b);

    return (Vec3) {
        k * b.x,
        k * b.y,
        k * b.z,
    };
}

static Vec3 v3_norm(Vec3 v) {
    f32 k = rsqrt(v3_dot(v, v));

    return (Vec3) {
        k * v.x,
        k * v.y,
        k * v.z,
    };
}

static Vec3 v3_min(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
    };
}

static Vec3 v3_max(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
    };
}

static Vec3 v3_lerp(Vec3 a, Vec3 b, f32 t) {
    return (Vec3) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
    };
}

static Vec3 v3_cross(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

static Vec3 v3_spline(f32 f, Vec3 a, Vec3 b, Vec3 c, Vec3 d) {
	f32 i = 1.0f - f;

    return (Vec3) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
    };
}

// ----------------------------- v4 ---------------------------- //

static Vec4 v4_from_array(const f32* a) {
    return (Vec4) { a[0], a[1], a[2], a[3] };
}

static Vec4 v4_from_packed_color(u32 color) {
    return (Vec4) {
        ((color & 0x000000ff) >> 0)  / 256.0f,
        ((color & 0x0000ff00) >> 8)  / 256.0f,
        ((color & 0x00ff0000) >> 16) / 256.0f,
        ((color & 0xff000000) >> 24) / 256.0f,
    };
}

static Vec4 v4_neg(Vec4 a) {
    return (Vec4) { -a.x, -a.y, -a.z, -a.w };
}

static Vec4 v4_add(Vec4 a, Vec4 b) {
    return (Vec4) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

static Vec4 v4_sub(Vec4 a, Vec4 b) {
    return (Vec4) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

static Vec4 v4_mul(Vec4 a, Vec4 b) {
    return (Vec4) { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

static Vec4 v4_scale(Vec4 a, f32 s) {
    return (Vec4) { a.x * s, a.y * s, a.z * s, a.w * s };
}

static f32 v4_dot(Vec4 a, Vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static f32 v4_len_sq(Vec4 v) {
    return v4_dot(v, v);
}

static f32 v4_len(Vec4 v) {
    return sqrt(v4_dot(v, v));
}

static f32 v4_dist_sq(Vec4 a, Vec4 b) {
    f32 dx = b.x - a.x;
    f32 dy = b.y - a.y;
    f32 dz = b.z - a.z;
    f32 dw = b.w - a.w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

static f32 v4_dist(Vec4 a, Vec4 b) {
    return sqrt(v4_dist_sq(a, b));
}

static Vec4 v4_norm(Vec4 v) {
    f32 k = rsqrt(v4_dot(v, v));

    return (Vec4) {
        k * v.x,
        k * v.y,
        k * v.z,
        k * v.w,
    };
}

static Vec4 v4_min(Vec4 a, Vec4 b) {
    return (Vec4) {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w,
    };
}

static Vec4 v4_max(Vec4 a, Vec4 b) {
    return (Vec4) {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w,
    };
}

static Vec4 v4_lerp(Vec4 a, Vec4 b, f32 t) {
    return (Vec4) {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w),
    };
}

static Vec4 v4_spline(f32 f, Vec4 a, Vec4 b, Vec4 c, Vec4 d) {
	f32 i = 1.0f - f;

    return (Vec4) {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
        ((d.w * f + c.w * i) * f + (c.w * f + b.w * i) * i) * f + ((c.w * f + b.w * i) * f + (b.w * f + a.w * i) * i) * i,
    };
}

// m2:
static Mat2 m2_mul(Mat2 a, Mat2 b) {
    return (Mat2) {
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3],
    };
}

static Vec2 m2_mulv(Mat2 m, Vec2 v) {
    return (Vec2) {
        m.e[0] * v.x + m.e[2] * v.y,
        m.e[1] * v.x + m.e[3] * v.y,
    };
}

static Mat2 m2_identity(void) {
    return (Mat2) {
        1.0f, 0.0f,
        0.0f, 1.0f
    };
}

static Mat2 m2_rotate_cs(f32 c, f32 s) {
    return (Mat2) {
         c, s,
        -s, c,
    };
}

static Mat2 m2_rotate(f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return (Mat2) {
         c,  s,
        -s,  c,
    };
}

static Mat2 m2_scale(f32 sx, f32 sy) {
    return (Mat2) {
        sx, 0,
        0,  sy,
    };
}

static f32 m2_det(Mat2 m) {
    return m.e[0] * m.e[2] - m.e[1] * m.e[3];
}

static Mat2 m2_transpose(Mat2 m) {
    return (Mat2) {
        m.e[0], m.e[2],
        m.e[1], m.e[3],
    };
}

static Mat2 m2_inverse(Mat2 m) {
    f32 k = 1.0f / m2_det(m);

    return (Mat2) {
         k * m.e[3],
        -k * m.e[2],
        -k * m.e[1],
         k * m.e[0],
    };
}

// m3:
static Mat3 m3_mul(Mat3 a, Mat3 b) {
    return (Mat3) {
        a.e[0] * b.e[0] + a.e[3] * b.e[1]  + a.e[6] * b.e[2],
        a.e[1] * b.e[0] + a.e[4] * b.e[1]  + a.e[7] * b.e[2],
        a.e[2] * b.e[0] + a.e[5] * b.e[1]  + a.e[8] * b.e[2],

        a.e[0] * b.e[3] + a.e[3] * b.e[4]  + a.e[6] * b.e[5],
        a.e[1] * b.e[3] + a.e[4] * b.e[4]  + a.e[7] * b.e[5],
        a.e[2] * b.e[3] + a.e[5] * b.e[4]  + a.e[8] * b.e[5],

        a.e[0] * b.e[6] + a.e[3] * b.e[7]  + a.e[6] * b.e[8],
        a.e[1] * b.e[6] + a.e[4] * b.e[7]  + a.e[7] * b.e[8],
        a.e[2] * b.e[6] + a.e[5] * b.e[7]  + a.e[8] * b.e[8],
    };
}

static Vec3 m3_mulv(Mat3 m, Vec3 v) {
    return (Vec3) {
        m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z,
        m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z,
        m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z,
    };
}

static Mat3 m3_identity(void) {
    return (Mat3) {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    };
}

static Mat3 m3_rotate(Vec3 axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    f32 k = 1.0f - c;

    Vec3 sa = { s * axis.x, s * axis.y, s * axis.z };
    Vec3 omca = { k * axis.x, k * axis.y, k * axis.z };

    return (Mat3) {
        omca.x * axis.x + c,
        omca.x * axis.y - sa.z,
        omca.x * axis.z + sa.y,

        omca.y * axis.x + sa.z,
        omca.y * axis.y + c,
        omca.y * axis.z - sa.x,

        omca.z * axis.x - sa.y,
        omca.z * axis.y + sa.x,
        omca.z * axis.z + c,
    };
}

static Mat3 m3_euler(f32 yaw, f32 pitch, f32 roll) {
    f32 cy = cosf(yaw);
    f32 sy = sinf(yaw);
    f32 cp = cosf(pitch);
    f32 sp = sinf(pitch);
    f32 cr = cosf(roll);
    f32 sr = sinf(roll);

    return (Mat3) {
        cy * cp,
        sy * cp,
        -sp,

        cy * sp * sr - sy * cr,
        sy * sp * sr + cy * cr,
        cp * sr,

        cy * sp * cr + sy * sr,
        sy * sp * cr - cy * sr,
        cp * cr,
    };
}

static Mat3 m3_from_quat(Quat quat) {
    f32 a = quat.w;
	f32 b = quat.x;
	f32 c = quat.y;
	f32 d = quat.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (Mat3) {
        a2 + b2 - c2 - d2,
        2  * (b * c + a * d),
        2  * (b * d - a * c),

        2  * (b * c - a * d),
        a2 - b2 + c2 - d2,
        2  * (c * d + a * b),

        2  * (b * d + a * c),
        2  * (c * d - a * b),
        a2 - b2 - c2 + d2,
    };
}

// m4:
static void m4_copy_to_array(f32* out, Mat4 m) {
    for (u32 i = 0; i < ARRAY_COUNT(m.e); ++i)
        out[i] = m.e[i];
}

static Mat4 m4_mul(Mat4 a, Mat4 b) {
    return (Mat4) {
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
        a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15],
    };
}

static Vec4 m4_mulv(Mat4 m, Vec4 v) {
    return (Vec4) {
        m.e[0] * v.x + m.e[4] * v.y + m.e[8]  * v.z + m.e[12] * v.w,
        m.e[1] * v.x + m.e[5] * v.y + m.e[9]  * v.z + m.e[13] * v.w,
        m.e[2] * v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14] * v.w,
        m.e[3] * v.x + m.e[7] * v.y + m.e[11] * v.z + m.e[15] * v.w,
    };
}

static Mat4 m4_identity(void) {
    return (Mat4) {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

static Mat4 m4_transpose(Mat4 m) {
    return (Mat4) {
        m.e[0], m.e[4], m.e[8],  m.e[12],
        m.e[1], m.e[5], m.e[9],  m.e[13],
        m.e[2], m.e[6], m.e[10], m.e[14],
        m.e[3], m.e[7], m.e[11], m.e[15],
    };
}

static Mat4 m4_translate(f32 x, f32 y, f32 z) {
    return (Mat4) {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
}

static Mat4 m4_scale(f32 x, f32 y, f32 z) {
    return (Mat4) {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1,
    };
}

static Mat4 m4_rotate_x(f32 angle) {
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (Mat4) {
        1,  0, 0, 0,
        0,  c, s, 0,
        0, -s, c, 0,
        0,  0, 0, 1,
    };
}

static Mat4 m4_rotate_y(f32 angle) {
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (Mat4) {
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1,
    };
}

static Mat4 m4_rotate_z(f32 angle) {
	f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (Mat4) {
         c, s, 0, 0,
        -s, c, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1,
    };
}

static Mat4 m4_rotate(Vec3 axis, f32 angle) {
    f32 cosv = cosf(angle);
    f32 sinv = sinf(angle);
    f32 inv_cosv = 1.0f - cosv;

    Vec3 sa = { axis.x * sinv,      axis.y * sinv,      axis.z * sinv };
    Vec3 omca = { axis.x * inv_cosv,  axis.y * inv_cosv,  axis.z * inv_cosv };

    return (Mat4) {
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0,  0,  0,  1,
    };
}

static Mat4 m4_frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return (Mat4) {
        2 * n / (r - l),    0,                  0,                      0,
        0,                  2 * n / (t - b),    0,                      0,
        (r + l) / (r - l),  (t + b) / (t - b),  -(f + n) / (f - n),     -1,
        0,                  0,                  -2 * (f * n) / (f - n), 0,
    };
}

static Mat4 m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return (Mat4) {
        2 / (r - l),            0,                      0,                      0,
        0,                      2 / (t - b),            0,                      0,
        0,                      0,                      -2 / (f - n),           0,
        -(r + l) / (r - l),     -(t + b) / (t - b),     -(f + n) / (f - n),     1,
    };
}

static Mat4 m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return (Mat4) {
        a / aspect,     0,      0,                          0,
        0,              a,      0,                          0,
        0,              0,      -((f + n) / (f - n)),       -1,
        0,              0,      -((2 * f * n) / (f - n)),   0,
    };
}

static Mat4 m4_look_at(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 f = v3_norm(v3(center.x - eye.x, center.y - eye.y, center.z - eye.z));
    Vec3 s = v3_norm(v3_cross(f, up));
    Vec3 t = v3_cross(s, f);

    Mat4 m = {0};

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

static Mat4 m4_from_quat(Quat q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (Mat4) {
        a2 + b2 - c2 - d2,      2  * (b * c + a * d),   2  * (b * d - a * c),   0,
        2  * (b * c - a * d),   a2 - b2 + c2 - d2,      2  * (c * d + a * b),   0,
        2  * (b * d + a * c),   2  * (c * d - a * b),   a2 - b2 - c2 + d2,      0,
        0,                      0,                      0,                      1,
    };
}

static Mat4 m4_invert(Mat4 m) {
	f32 s[6], c[6];

	s[0] = m.e[0] * m.e[5] - m.e[4] * m.e[1];
	s[1] = m.e[0] * m.e[6] - m.e[4] * m.e[2];
	s[2] = m.e[0] * m.e[7] - m.e[4] * m.e[3];
	s[3] = m.e[1] * m.e[6] - m.e[5] * m.e[2];
	s[4] = m.e[1] * m.e[7] - m.e[5] * m.e[3];
	s[5] = m.e[2] * m.e[7] - m.e[6] * m.e[3];

	c[0] = m.e[8]  * m.e[13] - m.e[12] * m.e[9];
	c[1] = m.e[8]  * m.e[14] - m.e[12] * m.e[10];
	c[2] = m.e[8]  * m.e[15] - m.e[12] * m.e[11];
	c[3] = m.e[9]  * m.e[14] - m.e[13] * m.e[10];
	c[4] = m.e[9]  * m.e[15] - m.e[13] * m.e[11];
	c[5] = m.e[10] * m.e[15] - m.e[14] * m.e[11];
	
	// assumes it is invertible
	f32 idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    return (Mat4) {
        ( m.e[5]  * c[5] - m.e[6]  * c[4] + m.e[7]  * c[3]) * idet,
        (-m.e[1]  * c[5] + m.e[2]  * c[4] - m.e[3]  * c[3]) * idet,
        ( m.e[13] * s[5] - m.e[14] * s[4] + m.e[15] * s[3]) * idet,
        (-m.e[9]  * s[5] + m.e[10] * s[4] - m.e[11] * s[3]) * idet,

        (-m.e[4]  * c[5] + m.e[6]  * c[2] - m.e[7]  * c[1]) * idet,
        ( m.e[0]  * c[5] - m.e[2]  * c[2] + m.e[3]  * c[1]) * idet,
        (-m.e[12] * s[5] + m.e[14] * s[2] - m.e[15] * s[1]) * idet,
        ( m.e[8]  * s[5] - m.e[10] * s[2] + m.e[11] * s[1]) * idet,

        ( m.e[4]  * c[4] - m.e[5]  * c[2] + m.e[7]  * c[0]) * idet,
        (-m.e[0]  * c[4] + m.e[1]  * c[2] - m.e[3]  * c[0]) * idet,
        ( m.e[12] * s[4] - m.e[13] * s[2] + m.e[15] * s[0]) * idet,
        (-m.e[8]  * s[4] + m.e[9]  * s[2] - m.e[11] * s[0]) * idet,

        (-m.e[4]  * c[3] + m.e[5]  * c[1] - m.e[6]  * c[0]) * idet,
        ( m.e[0]  * c[3] - m.e[1]  * c[1] + m.e[2]  * c[0]) * idet,
        (-m.e[12] * s[3] + m.e[13] * s[1] - m.e[14] * s[0]) * idet,
        ( m.e[8]  * s[3] - m.e[9]  * s[1] + m.e[10] * s[0]) * idet,
    };
}

// quat:
static Quat quat_identity(void) {
    return (Quat) { 0, 0, 0, 1 };
}

static Quat quat_make(f32 x, f32 y, f32 z, f32 angle) {
    f32 inv_len = rsqrt((x * x) + (y * y) + (z * z));
    f32 s = inv_len * sinf(angle / 2.0f);

    return (Quat) {
        x * s,
        y * s,
        z * s,
        cosf(angle / 2.0f),
    };
}

static Quat quat_conj(Quat q) {
    return (Quat) { -q.x, -q.y, -q.z, q.w };
}

static Quat quat_mul(Quat a, Quat b) {
    return (Quat) {
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
}

static Quat quat_rotate(Vec3 axis, f32 angle) {
    f32 s = sinf(0.5f * angle);
    Vec3  v = { s * axis.x, s * axis.y, s * axis.z };

    return (Quat) { v.x, v.y, v.z, cosf(0.5f * angle) };
}

// --------------------- v2i ------------------------- // 

static Vec2i v2i_add(Vec2i a, Vec2i b) {
    return (Vec2i) { a.x + b.x, a.y + b.y };
}

static Vec2i v2i_sub(Vec2i a, Vec2i b) {
    return (Vec2i) { a.x - b.x, a.y - b.y };
}

static b32 v2i_equal(Vec2i a, Vec2i b) {
    return a.x == b.x && a.y == b.y;
}

static i32 v2i_dist_sq(Vec2i a, Vec2i b) {
    i32 dx = a.x - b.x;
    i32 dy = a.y - b.y;

    return dx * dx + dy * dy;
}

static i32 v2i_manhattan(Vec2i a, Vec2i b) {
    i32 dx = abs(a.x - b.x);
    i32 dy = abs(a.y - b.y);

    return dx + dy;
}

static Vec2i v2i_clamp(Vec2i a, i32 min, i32 max) {
    return (Vec2i) {
        clamp(a.x, min, max),
        clamp(a.y, min, max),
    };
}

static Vec2i v2i_min(Vec2i a, Vec2i b) {
    return (Vec2i) {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
    };
}

static Vec2i v2i_max(Vec2i a, Vec2i b) {
    return (Vec2i) {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
    };
}

// --------------------- v3i ------------------------- // 

static Vec3i v3i_add(Vec3i a, Vec3i b) {
    return (Vec3i) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static Vec3i v3i_sub(Vec3i a, Vec3i b) {
    return (Vec3i) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static i32 v3i_dist_sq(Vec3i a, Vec3i b) {
    i32 dx = a.x - b.x;
    i32 dy = a.y - b.y;
    i32 dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

static Vec3i v3i_max(Vec3i a, Vec3i b) {
    return (Vec3i) {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
    };
}

static Vec3i v3i_min(Vec3i a, Vec3i b) {
    return (Vec3i) {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
    };
}

// =================================================== SHAPES =============================================== //

// ---------------------------------------- CIRCLE -------------------------------------- //

typedef struct Circle Circle;
struct Circle {
    Vec3 pos;
    f32 rad;
};

static b32 circle_intersect(Circle a, Circle b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

static Vec2 circle_get_intersect_vector(Circle a, Circle b) {
    Vec2 delta = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
    f32 depth = v2_len(delta) - (a.rad + b.rad);
    
    return (Vec2) {
        depth * delta.x,
        depth * delta.y,
    };
}

// --------------------------------------- SHPERE ------------------------------------ //

typedef struct Sphere Sphere;
struct Sphere {
    Vec3 pos;
    f32 rad;
};

static b32 sphere_intersect(Sphere a, Sphere b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 dz  = b.pos.z - a.pos.z;

    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

static Vec3 sphere_get_intersect_vector(Sphere a, Sphere b) {
    Vec3 delta  = { b.pos.x - a.pos.x, b.pos.y - a.pos.y, b.pos.z - a.pos.z };
    f32 depth = v3_len(delta) - (a.rad + b.rad);
    
    return (Vec3) {
        depth * delta.x,
        depth * delta.y,
        depth * delta.z,
    };
}

// ---------------------------------------- RECTANGLE ------------------------------------- //

typedef struct Rect Rect;
struct Rect {
    Vec2 min;
    Vec2 max;
};

static b32 rect_contains(Rect rect, Vec2 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static b32 rect_intersect(Rect a, Rect b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static Rect rect_get_overlap(Rect a, Rect b) {
    return (Rect) {
        v2_max(a.min, b.min),
        v2_min(a.max, b.max),
    };
}

static Vec2 rect_get_intersect_vector(Rect a, Rect b) {
    Rect o = rect_get_overlap(a, b);
    f32 dx = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32 dy = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);

    return (Vec2) {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
    };
}

static Rect rect_move(Rect rect, Vec2 offset) {
    return (Rect) {
        rect.min.x + offset.x,
        rect.min.y + offset.y,
        rect.max.x + offset.x,
        rect.max.y + offset.y,
    };
}

// ---------------------------------------- RECTANGLE - INT ------------------------------------- //

typedef struct RectI32 RectI32;
struct RectI32 {
    Vec2i min;
    Vec2i max;
};

static b32 recti_contains(RectI32 rect, Vec2i pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static b32 recti_intersect(RectI32 a, RectI32 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static RectI32 recti_get_overlap(RectI32 a, RectI32 b) {
    return (RectI32) {
        v2i_max(a.min, b.min),
        v2i_min(a.max, b.max),
    };
}

static Vec2i recti_get_intersect_vector(RectI32 a, RectI32 b) {
    RectI32 o = recti_get_overlap(a, b);

    i32 dx  = (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2;
    i32 dy  = (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2;

    return (Vec2i) {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
    };
}

static RectI32 recti_move(RectI32 rect, Vec2i offset) {
    return (RectI32) {
        rect.min.x + offset.x,
        rect.min.y + offset.y,
        rect.max.x + offset.x,
        rect.max.y + offset.y,
    };
}

// ----------------------------------------- BOX --------------------------------------- //

typedef struct Box Box;
struct Box {
    Vec3 min;
    Vec3 max;
};

static b32 box_contains(Box box, Vec3 pos) {
    if (pos.x < box.min.x || pos.x > box.max.x) return false;
    if (pos.y < box.min.y || pos.y > box.max.y) return false;
    if (pos.z < box.min.z || pos.z > box.max.z) return false;

    return true;
}

static b32 box_intersect(Box a, Box b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static Box box_get_overlap(Box a, Box b) {
    return (Box) {
        v3_max(a.min, b.min),
        v3_min(a.max, b.max),
    };
}

static Vec3 box_get_intersect_vector(Box a, Box b) {
    Box o = box_get_overlap(a, b);
    f32 dx  = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32 dy  = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);
    f32 dz  = 0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z);

    return (Vec3) {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
        sign(dz) * (o.max.z - o.min.z),
    };
}

static Box box_move(Box box, Vec3 offset) {
    return (Box) {
        box.min.x + offset.x,
        box.min.y + offset.y,
        box.min.z + offset.z,

        box.max.x + offset.x,
        box.max.y + offset.y,
        box.max.z + offset.z,
    };
}

// ----------------------------------------- BOX - INT --------------------------------------- //

typedef struct BoxI32 BoxI32;
struct BoxI32 {
    Vec3i  min;
    Vec3i  max;
};

static b32 boxi_contains(BoxI32 box, Vec3i pos) {
    if (pos.x < box.min.x || pos.x > box.max.x) return false;
    if (pos.y < box.min.y || pos.y > box.max.y) return false;
    if (pos.z < box.min.z || pos.z > box.max.z) return false;

    return true;
}

static b32 boxi_intersect(BoxI32 a, BoxI32 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static BoxI32 boxi_get_overlap(BoxI32 a, BoxI32 b) {
    return (BoxI32) {
        v3i_max(a.min, b.min),
        v3i_min(a.max, b.max),
    };
}

static Vec3i boxi_get_intersect_vector(BoxI32 a, BoxI32 b) {
    BoxI32 o = boxi_get_overlap(a, b);

    i32 dx  = (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2;
    i32 dy  = (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2;
    i32 dz  = (a.min.z + a.max.z) / 2 - (b.min.z + b.max.z) / 2;

    return (Vec3i) {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
        sign(dz) * (o.max.z - o.min.z),
    };
}

static BoxI32 boxi_move(BoxI32 box, Vec3i offset) {
    return (BoxI32) {
        box.min.x + offset.x,
        box.min.y + offset.y,
        box.min.z + offset.z,

        box.max.x + offset.x,
        box.max.y + offset.y,
        box.max.z + offset.z,
    };
}

// ----------------------------------------- FRUSTUM --------------------------------------- //

typedef struct Plane Plane;
struct Plane {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
};

static Plane plane_normalize(Plane plane) {
    f32 mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;

    return plane;
}

typedef struct Frustum Frustum;
struct Frustum {
    Plane   plane[6];
};

static Frustum frustum_create(Mat4 m, b32 normalize) {
    Frustum frustum = {0};

    // left clipping plane
    frustum.plane[0].a = m.e[3]  + m.e[0];
    frustum.plane[0].b = m.e[7]  + m.e[4];
    frustum.plane[0].c = m.e[11] + m.e[8];
    frustum.plane[0].d = m.e[15] + m.e[12];

    // right clipping plane
    frustum.plane[1].a = m.e[3]  - m.e[0];
    frustum.plane[1].b = m.e[7]  - m.e[4];
    frustum.plane[1].c = m.e[11] - m.e[8];
    frustum.plane[1].d = m.e[15] - m.e[12];

    // top clipping plane
    frustum.plane[2].a = m.e[3]  - m.e[1];
    frustum.plane[2].b = m.e[7]  - m.e[5];
    frustum.plane[2].c = m.e[11] - m.e[9];
    frustum.plane[2].d = m.e[15] - m.e[13];

    // bottom clipping plane
    frustum.plane[3].a = m.e[3]  + m.e[1];
    frustum.plane[3].b = m.e[7]  + m.e[5];
    frustum.plane[3].c = m.e[11] + m.e[9];
    frustum.plane[3].d = m.e[15] + m.e[13];

    // near clipping plane
    frustum.plane[4].a = m.e[3]  + m.e[2];
    frustum.plane[4].b = m.e[7]  + m.e[6];
    frustum.plane[4].c = m.e[11] + m.e[10];
    frustum.plane[4].d = m.e[15] + m.e[14];

    // far clipping plane
    frustum.plane[5].a = m.e[3]  - m.e[2];
    frustum.plane[5].b = m.e[7]  - m.e[6];
    frustum.plane[5].c = m.e[11] - m.e[10];
    frustum.plane[5].d = m.e[15] - m.e[14];

    if (normalize) {
        frustum.plane[0] = plane_normalize(frustum.plane[0]);
        frustum.plane[1] = plane_normalize(frustum.plane[1]);
        frustum.plane[2] = plane_normalize(frustum.plane[2]);
        frustum.plane[3] = plane_normalize(frustum.plane[3]);
        frustum.plane[4] = plane_normalize(frustum.plane[4]);
        frustum.plane[5] = plane_normalize(frustum.plane[5]);
    }
    
    return frustum;
}

static b32 frustum_contains(Frustum frustum, Vec3 pos) {
    for(i32 i = 0; i < 6; i++) {
		if (frustum.plane[i].a * pos.x + frustum.plane[i].b * pos.y + frustum.plane[i].c * pos.z + frustum.plane[i].d <= 0)
			return false;
	}

	return true;
}

static b32 frustum_intersect_sphere(Frustum frustum, Sphere sphere) {
    for(i32 i = 0; i < 6; i++) {
		if(frustum.plane[i].a * sphere.pos.x + frustum.plane[i].b * sphere.pos.y + frustum.plane[i].c * sphere.pos.z + frustum.plane[i].d <= -sphere.rad) {
			return false;
		}
	}

	return true;
}

static b32 frustum_intersect_box(Frustum frustum, Box box) {
    for (int i = 0; i < 6; i++) {
		if (frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;

		return false;
	}

	return true;
}

// ================================================= COLOR PACKING =========================================== //

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

static u32 pack_color_v3(Vec3 color) {
    return pack_color_f32(color.r, color.g, color.b, 1.0);
}

// =================================== XORSHIFT32 ============================================= //

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
    return min + ((f32)rand_u32(state) / (f32)0xFFFFFFFF) * (max - min); 
}

static Vec2 rand_unit_v2(u32* state) {
    Vec2 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return v2_norm(out);
}

static Vec3 rand_unit_v3(u32* state) {
    Vec3 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return v3_norm(out);
}

// ============================ RANDOM(HASH) FUNCTION ======================= //

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

    while (*str)
        hash = (hash << 7) + (hash >> 25) + *str++;

    return hash + (hash >> 16);
}

static u32 hash_mem(const void* ptr, u32 size) {
    const u8* m = ptr; 
    u32 hash = 0;

    for (u32 i = 0; i < size; ++i)
        hash = (hash << 7) + (hash >> 25) + m[i];

    return hash + (hash >> 16);
}

#define HASH_PRIME0 (3323784421u)
#define HASH_PRIME1 (1449091801u)
#define HASH_PRIME2 (4280703257u)
#define HASH_PRIME3 (1609059329u)

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

// ========================================== ARENA ALLOCATOR ================================== //

#ifdef ATS_ARENA_ALLOCATOR

#ifndef ATS_ALLOC_STACK_SIZE
#define ATS_ALLOC_STACK_SIZE  (16)
#endif

#ifndef ATS_ALLOC_BUFFER_SIZE
#define ATS_ALLOC_BUFFER_SIZE (GB)
#endif

static u32 alloc_index    = 0;
static u32 alloc_max      = 0;
static u32 alloc_top      = 0;
static u32 alloc_lock     = false;

static u32 alloc_stack    [ATS_ALLOC_STACK_SIZE];
static u8  alloc_buffer   [ATS_ALLOC_BUFFER_SIZE];

#define alloc_type(T)           (T*)alloc_size(sizeof (T))
#define alloc_array(T, count)   (T*)alloc_size((count) * sizeof (T))

static void* alloc_size(u32 byte_size) {
    byte_size = ALIGN_UP(byte_size, 16);

    assert((alloc_index + byte_size) < ATS_ALLOC_BUFFER_SIZE && !alloc_lock);

    void* memory    = alloc_buffer + alloc_index;
    alloc_index     += byte_size;
    alloc_max       = max(alloc_max, alloc_index);

    memset(memory, 0, byte_size);

    return memory;
}

static void* alloc_begin(void) {
    alloc_lock = true;
    return alloc_buffer + alloc_index;
}

static void alloc_end(u32 byte_size) {
    alloc_index += ALIGN_UP(byte_size, 16);
    alloc_lock = false;
}

static void alloc_save(void) {
    assert(alloc_top < ATS_ALLOC_STACK_SIZE);
    alloc_stack[alloc_top++] = alloc_index;
}

static void alloc_restore(void) {
    assert(alloc_top > 0);
    alloc_index = alloc_stack[--alloc_top];
}

static void alloc_validate(void) {
    assert(alloc_top == 0);
}

#endif // ATS_ARENA_ALLOCATOR

// ========================================= PRIORITY QUEUE =================================== //

typedef struct QueueNode QueueNode;
struct QueueNode {
    f32 weight;
    Vec2i e;
};

typedef struct PriorityQueue PriorityQueue;
struct PriorityQueue {
    i32         len;
    QueueNode*  array;
};

static b32 queue_empty(const PriorityQueue* queue) {
    return queue->len == 0;
}

static void queue_clear(PriorityQueue* queue) {
    queue->len = 0;
}

static void queue_push(PriorityQueue* queue, Vec2i e, f32 weight) {
    QueueNode node = { weight, e };

    int i = queue->len + 1;
    int j = i / 2;

    while (i > 1 && queue->array[j].weight > node.weight) {
        queue->array[i] = queue->array[j];

        i = j;
        j = j / 2;
    }

    queue->array[i] = node;
    queue->len++;
}

static f32 queue_pop(Vec2i* out, PriorityQueue* queue) {
    QueueNode data = queue->array[1];

    queue->array[1] = queue->array[queue->len];
    queue->len--;

    int i = 1;
    while (i != queue->len + 1)
    {
        int k = queue->len + 1;
        int j = 2 * i;

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

// ==================================== FILES ==================================== //

static size_t file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

static char* file_read_str(const char* file_name) {
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0) {
        size_t size = file_get_size(fp);
        buffer = (char*)malloc(size + 1);

        if (buffer) {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0) {
                free(buffer);
                buffer = 0;
            }
        }

        fclose(fp);
    }

    return buffer;
}

static b32 file_write_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return false;
}

static b32 file_append_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);

        fclose(fp);
        return n == size;
    }

    return false;
}

static b32 file_read_bin(const char* file_name, void* buffer, size_t size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);

        return 1;
    }

    return false;
} 

static b32 file_write_bin(const char* file_name, const void* buffer, size_t size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }

    return false;
}

// ====================================================================================================== //

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

// ================================================ PLATFORM ============================================= //

#ifdef ATS_PLATFORM_GLFW

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#endif

#ifdef ATS_MODERN_OPENGL
#include "dep/glad/glad.h"
#include "dep/glad/glad.c"
#endif

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

// ===================================================== KEYS =================================================== //

#define KEY_UNKNOWN            -1

#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348
#define KEY_LAST               KEY_MENU

#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3

#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          JOYSTICK_16

#define GAMEPAD_BUTTON_A               0
#define GAMEPAD_BUTTON_B               1
#define GAMEPAD_BUTTON_X               2
#define GAMEPAD_BUTTON_Y               3
#define GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GAMEPAD_BUTTON_BACK            6
#define GAMEPAD_BUTTON_START           7
#define GAMEPAD_BUTTON_GUIDE           8
#define GAMEPAD_BUTTON_LEFT_THUMB      9
#define GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GAMEPAD_BUTTON_DPAD_UP         11
#define GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GAMEPAD_BUTTON_DPAD_DOWN       13
#define GAMEPAD_BUTTON_DPAD_LEFT       14
#define GAMEPAD_BUTTON_LAST            GAMEPAD_BUTTON_DPAD_LEFT

#define GAMEPAD_BUTTON_CROSS       GAMEPAD_BUTTON_A
#define GAMEPAD_BUTTON_CIRCLE      GAMEPAD_BUTTON_B
#define GAMEPAD_BUTTON_SQUARE      GAMEPAD_BUTTON_X
#define GAMEPAD_BUTTON_TRIANGLE    GAMEPAD_BUTTON_Y

#define GAMEPAD_AXIS_LEFT_X        0
#define GAMEPAD_AXIS_LEFT_Y        1
#define GAMEPAD_AXIS_RIGHT_X       2
#define GAMEPAD_AXIS_RIGHT_Y       3
#define GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GAMEPAD_AXIS_LAST          GAMEPAD_AXIS_RIGHT_TRIGGER

// ==================================================================== GAMEPAD ===================================================== //

typedef union GamepadButtons GamepadButtons;
union GamepadButtons {
    struct {
        u32 X : 1;
        u32 A : 1;
        u32 B : 1;
        u32 Y : 1;

        u32 LB : 1;
        u32 RB : 1;
        u32 LT : 1;
        u32 RT : 1;
        
        u32 select : 1;
        u32 start : 1;
        u32 LS : 1;
        u32 RS : 1;

        u32 UP : 1;
        u32 RIGHT : 1;
        u32 DOWN : 1;
        u32 LEFT : 1;
    } button;

    u32 data;
};

typedef struct Gamepad Gamepad;
struct Gamepad {
    b32 active;

    Vec2 LS;
    Vec2 RS;

    f32 LT;
    f32 RT;

    GamepadButtons state;
    GamepadButtons pressed;
    GamepadButtons released;
};

// =========================================================== MOUSE MODES ================================================= //

enum {
    MOUSE_NORMAL,
    MOUSE_HIDDEN,
    MOUSE_DISABLED
};

// ===========================================================  PLATFORM =================================================== //

static struct {
    b32 close;

    i32 width;
    i32 height;
    f32 aspect_ratio;

    HWND native;

    b32 fullscreen;
    b32 _fullscreen_state_last_update;

    struct {
        f64 total;
        f64 delta;
    } time;

    struct {
        u32 mode;

        b32 is_down     : 1;
        b32 is_pressed  : 1;
        b32 is_released : 1;

        Vec2 pos;
        Vec2 delta;
        Vec2 scroll;

        b8 state[MOUSE_BUTTON_LAST + 1];
        b8 pressed[MOUSE_BUTTON_LAST + 1];
        b8 released[MOUSE_BUTTON_LAST + 1];
    } mouse;

    struct {
        i32 key;
        i32 ascii;

        b32 is_down     : 1;
        b32 is_pressed  : 1;
        b32 is_repeat   : 1;
        b32 is_released : 1;
        b32 is_ascii    : 1;
    
        b8 state[KEY_LAST + 1];
        b8 pressed[KEY_LAST + 1];
        b8 repeat[KEY_LAST + 1];
        b8 released[KEY_LAST + 1];
    } keyboard;

    Gamepad gamepad[JOYSTICK_LAST];
} platform;

static struct {
    GLFWwindow* window;
    GLFWmonitor* monitor;
} platform_internal;

static void window_key_callback(GLFWwindow* window, int key, int a, int action, int b)
{
    (void)window;
    (void)a;
    (void)b;

    switch (action)
    {
        case GLFW_PRESS:
            platform.keyboard.key = key;
            platform.keyboard.is_down = 1;
            platform.keyboard.is_pressed = 1;
            platform.keyboard.is_repeat = 1;
            platform.keyboard.state[key] = 1;
            platform.keyboard.pressed[key] = 1;
            platform.keyboard.repeat[key] = 1;
            break;
        case GLFW_REPEAT:
            platform.keyboard.is_repeat = 1;
            platform.keyboard.repeat[key] = 1;
            break;
        case GLFW_RELEASE:
            platform.keyboard.is_down = 0;
            platform.keyboard.is_released = 1;
            
            platform.keyboard.state[key] = 0;
            platform.keyboard.released[key] = 1;
            break;
    }
}

static void window_char_callback(GLFWwindow* window, unsigned int codepoint) {
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

static void window_mouse_button_callback(GLFWwindow* window, int button, int action, int a) {
    (void)window;
    (void)a;

    switch (action) {
        case GLFW_PRESS: {
            platform.mouse.is_down = 1;
            platform.mouse.is_pressed = 1;
            platform.mouse.state[button] = 1;
            platform.mouse.pressed[button] = 1;
        } break;
        case GLFW_RELEASE: {
            platform.mouse.is_down = 0;
            platform.mouse.is_released = 1;
            platform.mouse.state[button] = 0;
            platform.mouse.released[button] = 1;
        } break;
    }
}

static void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window;

    platform.mouse.scroll.x = (f32)xoffset;
    platform.mouse.scroll.y = (f32)yoffset;
}

static void window_joystick_callback(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

static void platform_init(const char* title, int width, int height, int samples) {
    glfwInit();

    platform_internal.monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#ifdef ATS_MODERN_OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.width = width;
    platform.height = height;

    platform_internal.window = glfwCreateWindow(width, height, title, NULL, NULL);
    platform.native = glfwGetWin32Window(platform_internal.window);

    glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform_internal.window);

#ifdef ATS_MODERN_OPENGL 
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

    glfwSetKeyCallback(platform_internal.window, window_key_callback);
    glfwSetCharCallback(platform_internal.window, window_char_callback);
    glfwSetMouseButtonCallback(platform_internal.window, window_mouse_button_callback);
    glfwSetScrollCallback(platform_internal.window, window_scroll_callback);
    glfwSetJoystickCallback(window_joystick_callback);

    // init mouse:
    {
        f64 x = 0.0;
        f64 y = 0.0;

        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.pos.x = (f32)x;
        platform.mouse.pos.y = (f32)y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i))
            platform.gamepad[i].active = 1;
    }

    glfwSetTime(0.0);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwSwapBuffers(platform_internal.window);
    glfwPollEvents();
}

static void platform_update(void) {
    if (glfwWindowShouldClose(platform_internal.window))
        platform.close = 1;

    if (platform.close)
        glfwSetWindowShouldClose(platform_internal.window, 1);

    platform.mouse.is_pressed       = 0;
    platform.mouse.is_released      = 0;
    platform.keyboard.is_pressed    = 0;
    platform.keyboard.is_repeat     = 0;
    platform.keyboard.is_released   = 0;
    platform.keyboard.is_ascii      = 0;

    // update mouse:
    {
        f64 x, y;
        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.delta.x  = (f32)(x - platform.mouse.pos.x);
        platform.mouse.delta.y  = (f32)(y - platform.mouse.pos.y);

        platform.mouse.pos.x    = (f32)x;
        platform.mouse.pos.y    = (f32)y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        switch (platform.mouse.mode) {
            case MOUSE_NORMAL: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } break;
            case MOUSE_HIDDEN: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            } break;
            case MOUSE_DISABLED: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } break;
        }
    }

    // update gamepads:
    {
        GLFWgamepadstate state;

        for (int i = 0; i < JOYSTICK_LAST; ++i) {
            if (platform.gamepad[i].active) {
                GamepadButtons old = platform.gamepad[i].state;

                platform.gamepad[i].state.data = 0;
                platform.gamepad[i].pressed.data = 0;
                platform.gamepad[i].released.data = 0;

                glfwGetGamepadState(i, &state);

                platform.gamepad[i].LS.x = +state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].LS.y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].RS.x = +state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                platform.gamepad[i].RS.y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

                platform.gamepad[i].LT = 0.5f * (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f);
                platform.gamepad[i].RT = 0.5f * (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f);

                if (state.buttons[GLFW_GAMEPAD_BUTTON_X]) platform.gamepad[i].state.button.X = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_A]) platform.gamepad[i].state.button.A = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_B]) platform.gamepad[i].state.button.B = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_Y]) platform.gamepad[i].state.button.Y = 1;

                if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])  platform.gamepad[i].state.button.LB = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) platform.gamepad[i].state.button.RB = 1;

                if (platform.gamepad[i].LT > 0.0f) platform.gamepad[i].state.button.LT = 1;
                if (platform.gamepad[i].RT > 0.0f) platform.gamepad[i].state.button.RT = 1;

                if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK]) platform.gamepad[i].state.button.select = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_START]) platform.gamepad[i].state.button.start = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]) platform.gamepad[i].state.button.LS = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]) platform.gamepad[i].state.button.RS = 1;

                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) platform.gamepad[i].state.button.UP = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]) platform.gamepad[i].state.button.RIGHT = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) platform.gamepad[i].state.button.DOWN = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]) platform.gamepad[i].state.button.LEFT = 1;

                platform.gamepad[i].pressed.data = platform.gamepad[i].state.data & ~old.data;
                platform.gamepad[i].released.data = ~platform.gamepad[i].state.data & old.data;
            }
        }
    }

    if (platform.fullscreen != platform._fullscreen_state_last_update) {
        const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

        if (platform.fullscreen)
            glfwSetWindowMonitor(platform_internal.window, platform_internal.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        else
            glfwSetWindowMonitor(platform_internal.window, NULL, 64, 64, mode->width - 256, mode->height - 256, mode->refreshRate);
    }

    platform._fullscreen_state_last_update = platform.fullscreen;

    glfwGetWindowSize(platform_internal.window, &platform.width, &platform.height);
    platform.aspect_ratio = (f32)platform.width / (f32)platform.height;

    glViewport(0, 0, platform.width, platform.height);

    memset(platform.keyboard.pressed,  0, sizeof (platform.keyboard.pressed));
    memset(platform.keyboard.repeat,   0, sizeof (platform.keyboard.repeat));
    memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

    memset(platform.mouse.pressed,  0, sizeof (platform.mouse.pressed));
    memset(platform.mouse.released, 0, sizeof (platform.mouse.released));

    glfwPollEvents();
    glfwSwapBuffers(platform_internal.window);

    platform.time.delta = glfwGetTime() - platform.time.total;
    platform.time.total += platform.time.delta;
}

// =================================================== TIMER STUFF =================================================== //

static f64 timer_get_current(void) {
    return glfwGetTime();
}

#endif // ATS_PLATFORM_GLFW

// ===================================================== SHADERS ===================================================== //

#ifdef ATS_MODERN_OPENGL

static u32 shader_compile(const char* source, unsigned int type) {
    int success = 0;
    char info_log[512] = {0};
    u32 shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        puts(info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

static u32 shader_link_program(u32 vertex_shader, u32 fragment_shader) {
    int success = 0;
    char info_log[512] = {0};

    u32 shader = glCreateProgram();

    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, info_log);
        puts(info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

static u32 shader_load_from_memory(const char *vs, const char *fs) {
    u32 vertex = shader_compile(vs, GL_VERTEX_SHADER);
    u32 fragment = shader_compile(fs, GL_FRAGMENT_SHADER);
    u32 program = shader_link_program(vertex, fragment);

    glUseProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

static u32 shader_load_from_file(const char *vs, const char *fs) {
    char* vs_content = file_read_str(vs);
    char* fs_content = file_read_str(fs);

    u32 program = shader_load_from_memory(vs_content, fs_content);

    free(vs_content);
    free(fs_content);

    return program;
}

static Vec3 gl_get_world_position(int x, int y, Mat4 in_projection, Mat4 in_modelview) {
    GLint viewport[4] = {0};
    f64 modelview[16] = {0};
    f64 projection[16] = {0};

    GLfloat  win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;
 
    for (i32 i = 0; i < 16; ++i) projection[i]  = in_projection.e[i];
    for (i32 i = 0; i < 16; ++i) modelview[i]   = in_modelview.e[i];

    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f64)(x);
    win_y = (f64)(viewport[3]) - (f64)y;

    glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    return (Vec3) { result[0], result[1], result[2] };
}

#endif // ATS_MODERN_OPENGL

// ======================================= TEXTURES ======================================== //

#ifdef STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "dep/stb_image.h" 

typedef struct Image Image;
struct Image {
    int width;
    int height;

    u32* pixels;
};

static Image image_load_from_file(const char* path) {
    Image image = {0};
    i32 channels = 0;

    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);

    return image;
}

static u32 image_get_pixel(Image* img, i32 x, i32 y) {
    return img->pixels[y * img->width + x];
}

static void image_set_pixel(Image* img, i32 x, i32 y, u32 pixel) {
    img->pixels[y * img->width + x] = pixel;
}

#endif

#if defined(ATS_PLATFORM_GLFW)

typedef struct Texture Texture;
struct Texture {
    u32     id;
    int     width;
    int     height;
};

static Texture texture_create(void *pixels, int width, int height, int is_smooth) {
    assert(pixels);

    Texture texture = {0};

    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

#ifdef ATS_MODERN_OPENGL 
    glGenerateMipmap(GL_TEXTURE_2D);
#endif

    return texture;
}

static void texture_update(Texture* texture, void *pixels, int width, int height, int is_smooth) {
    texture->width = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

#ifdef ATS_MODERN_OPENGL 
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

#ifdef STB_IMAGE_IMPLEMENTATION
static Texture texture_load_from_file(const char *texture_path, int is_smooth) {
    Texture texture = {0};
    i32 channels = 0;
    unsigned char* pixels = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 4);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);

#ifdef ATS_MODERN_OPENGL 
    glGenerateMipmap(GL_TEXTURE_2D);
#endif

    stbi_image_free(pixels);

    return texture;
}
#endif

static void texture_bind(const Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);

#ifndef ATS_MODERN_OPENGL
    glMatrixMode(GL_MODELVIEW);
#endif
}

static void texture_delete(Texture* texture) {
    glDeleteTextures(1, &texture->id);
    memset(texture, 0, sizeof *texture);
}

#endif

// ============================================ RENDER (LEGACY OPENGL) ============================================ //

#if defined(ATS_PLATFORM_GLFW) && !defined(ATS_MODERN_OPENGL)

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
}

static void gl_set_simple_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 pos[4] = { x, y, z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4] = { bright, bright, bright, 0.0f };
    u32 light = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_simple_light_directed(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 d = (f32)(1.0f / sqrt(x * x + y * y + z * z));
    f32 dir[4] = { x * d, y * d, z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4] = { bright, bright, bright, 0.0f };
    u32 light = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_emitter(int index, Vec3 p, Vec3 color, f32 constant, f32 linear, f32 quadratic) {
    f32 pos[4] = { p.x, p.y, p.z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4] = { color.r, color.g, color.b, 0.0f };
    u32 light = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);
    
    glLightf(light, GL_CONSTANT_ATTENUATION,    constant);
    glLightf(light, GL_LINEAR_ATTENUATION,      linear);
    glLightf(light, GL_QUADRATIC_ATTENUATION,   quadratic);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_directed(int index, Vec3 pos, Vec3 color) {
    f32 d = (f32)(1.0f / sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));
    f32 dir[4] = { pos.x * d, pos.y * d, pos.z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4] = { color.r, color.g, color.b, 0.0f };
    u32 light = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

static void gl_set_light_global_ambient(f32 r, f32 g, f32 b) {
    f32 v[4] = { r, g, b, 0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

static Vec3 gl_get_world_position(int x, int y) {
    GLint viewport[4] = {0};
    f64 modelview[16] = {0};
    f64 projection[16] = {0};

    GLfloat win_x, win_y, win_z;
 
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f32)(x);
    win_y = (f32)(viewport[3]) - (f32)y;

    glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    return (Vec3) { (f32)result[0], (f32)result[1], (f32)result[2] };
}

// ======================================= FONT ====================================== //

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

int bitmap_display_list[BITMAP_COUNT];

#define BITMAP_GETBIT(N, X, Y) (((u64)(N)) & (1ull << (((u64)(Y)) * 8ull + ((u64)(X)))))

static void gl_init_bitmap(void) {
    for (int i = 0; i < BITMAP_COUNT; ++i) {
        bitmap_display_list[i] = glGenLists(1);

        glNewList(bitmap_display_list[i], GL_COMPILE);

        u64 c = bitascii[i];

        glBegin(GL_QUADS);

        f32 scale = 1.0f / 8.0f;

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < 8; ++i) {
                f32 x   = i * scale;
                f32 y   = j * scale;

                if (BITMAP_GETBIT(c, i, j)) {
                    glVertex3f(x - 0,       y + scale,  0.0f);
                    glVertex3f(x + scale,   y + scale,  0.0f);
                    glVertex3f(x + scale,   y - 0,      0.0f);
                    glVertex3f(x - 0,       y - 0,      0.0f);
                }
            }
        }

        glEnd();

        glEndList();
    }
}

static void gl_render_ascii(u8 c, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    Mat4 t = m4_translate(x, y, z);
    Mat4 s = m4_scale(sx, sy, 1);
    Mat4 ts = m4_mul(t, s);

    glPushMatrix();

    glMultMatrixf(ts.e);
    glCallList(bitmap_display_list[c]);

    glPopMatrix();
}

static void gl_render_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
    glColor4ubv((u8*)&color);

    for (int i = 0; str[i] != '\0'; i++) {
        gl_render_ascii(str[i], x + i * sx, y, z, sx, sy);
    }
}

static void gl_render_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...) {
    va_list list;
    char buffer[256];

    va_start(list, fmt);

    vsnprintf(buffer, 256, fmt, list);
    gl_render_string(buffer, x, y, z, sx, sy, color);

    va_end(list);
}

#endif // !ATS_MODERN_OPENGL
