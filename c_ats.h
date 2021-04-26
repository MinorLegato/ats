#pragma once
// defines that enables different parts of the library:
// #define ATS_PLATFORM_GLFW
// #define ATS_MODERN_OPENGL 
// #define ATS_CUBE_ENGINE

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

#define internal        static
#define global          static
#define local_persist   static

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

typedef union v2 v2;
union v2
{
    struct
    {
        f32     x;
        f32     y;
    };

    f32 e[2];
};

typedef union v3 v3;
union v3
{
    struct
    {
        f32     x;
        f32     y;
        f32     z;
    };

    struct
    {
        f32     r;
        f32     g;
        f32     b;
    };

    f32 e[3];
};

typedef union v4 v4;
union v4
{
    struct
    {
        f32     x;
        f32     y;
        f32     z;
        f32     w;
    };

    struct
    {
        f32     r;
        f32     g;
        f32     b;
        f32     a;
    };

    f32 e[4];
};

typedef union v2i v2i;
union v2i
{
    struct
    {
        i32     x;
        i32     y;
    };

    i32 e[2];
};

typedef union v3i v3i;
union v3i
{
    struct
    {
        i32     x;
        i32     y;
        i32     z;
    };

    i32 e[3];
};

typedef union v4i v4i;
union v4i
{
    struct
    {
        i32     x;
        i32     y;
        i32     z;
        i32     w;
    };

    i32 e[4];
};

typedef union m2 m2;
union m2
{
    struct
    {
        v2  x;
        v2  y;
    };

    f32 e[4];
};

typedef union m3 m3;
union m3
{
    struct
    {
        v3  x;
        v3  y;
        v3  z;
    };

    f32 e[9];
};

typedef union m4 m4;
union m4
{
    struct
    {
        v4  x;
        v4  y;
        v4  z;
        v4  w;
    };

    f32 e[16];
};

typedef struct quat_t quat_t;
struct quat_t
{
    f32     x;
    f32     y;
    f32     z;
    f32     w;
};

#define V2(...)     ((v2) { __VA_ARGS__ })
#define V3(...)     ((v3) { __VA_ARGS__ })
#define V4(...)     ((v4) { __VA_ARGS__ })

#define V2i(...)    ((v2i) { __VA_ARGS__ })
#define V3i(...)    ((v3i) { __VA_ARGS__ })
#define V4i(...)    ((v4i) { __VA_ARGS__ })

// --------------------------------------------- FUNCTIONS ---------------------------------------- //

#define sqrt    __sqrt
#define rsqrt   __rsqrt

inline f32
__sqrt(f32 n)
{
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

inline f32
__rsqrt(f32 n)
{
	f32 x2 = n * 0.5F;
	f32 y  = n;
	int i  = *(long*)&y;           // evil f32ing point bit level hacking

	i = 0x5f3759df - (i >> 1);     // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y)); // 1st iteration

	return y;
}

#define square(n)       ((n) * (n))
#define lerp(a, b, t)   ((a) + (t) * ((b) - (a)))
#define unlerp(a, b, t) (((t) - (a)) / ((b) - (a)))

inline f32
shortest_angle_distance(f32 a, f32 b)
{
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);

    return fmodf(2.0f * da, max) - da;
}

inline f32
lerp_angle(f32 a, f32 b, f32 t)
{
    return a + shortest_angle_distance(a, b) * t;
}

inline f32
spline(f32 f, f32 a, f32 b, f32 c, f32 d)
{
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

// ==================================================  MATHS ================================================== //

// ---------------------- v2 ---------------------- //

inline f32
v2_dot(v2 a, v2 b)
{
    return a.x * b.x + a.y * b.y;
}

inline f32
v2_det(v2 a, v2 b)
{
    return a.x * b.y - a.y * b.x;
}

inline f32
v2_len_sq(v2 v)
{
    return v2_dot(v, v);
}

inline f32
v2_len(v2 v)
{
    return sqrt(v2_dot(v, v));
}

inline f32
v2_dist_sq(v2 a, v2 b)
{
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;

    return dx * dx + dy * dy;
}

inline f32
v2_dist(v2 a, v2 b)
{
    return sqrt(v2_dist_sq(a, b));
}

inline v2
v2_proj(v2 a, v2 b)
{
    f32 k = v2_dot(a, a) / v2_dot(b, b);
    return (v2) { k * b.x, k * b.y };
}

inline v2
v2_norm(v2 out, const v2 v)
{
    f32 k = rsqrt(v2_dot(v, v));
    return (v2) { k * v.x, k * v.y };
}

inline v2
v2_min(v2 a, v2 b)
{
    return (v2)
    {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
    };
}

inline v2
v2_max(v2 a, v2 b)
{
    return (v2)
    {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
    };
}

inline v2
v2_lerp(v2 a, v2 b, f32 t)
{
    return (v2)
    {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
    };
}

inline f32
v2_get_angle(v2 a, v2 b)
{
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;
    
    return atan2f(det, dot);
}

inline v2
v2_spline(f32 f, v2 a, v2 b, v2 c, v2 d)
{
	f32 i = 1.0f - f;

    return (v2)
    {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
    };
}

// ----------------------------- v3 ----------------------------- //

inline f32
v3_dot(v3 a, v3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32
v3_len_sq(v3 v)
{
    return v3_dot(v, v);
}

inline f32
v3_len(v3 v)
{
    return sqrt(v3_dot(v, v));
}

inline f32
v3_dist_sq(v3 a, v3 b)
{
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    f32 dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

inline f32
v3_dist(v3 a, v3 b)
{
    return sqrt(v3_dist_sq(a, b));
}

inline v3
v3_proj(v3 a, v3 b)
{
    f32 k = v3_dot(a, b) / v3_dot(b, b);

    return (v3)
    {
        k * b.x,
        k * b.y,
        k * b.z,
    };
}

inline v3
v3_norm(v3 v)
{
    f32 k = rsqrt(v3_dot(v, v));

    return (v3)
    {
        k * v.x,
        k * v.y,
        k * v.z,
    };
}

inline v3
v3_min(v3 a, v3 b)
{
    return (v3)
    {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
    };
}

inline v3
v3_max(v3 a, v3 b)
{
    return (v3)
    {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
    };
}

inline v3
v3_lerp(v3 a, v3 b, f32 t)
{
    return (v3)
    {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
    };
}

inline v3
v3_cross(v3 a, v3 b)
{
    return (v3)
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

inline v3
v3_spline(f32 f, const v3 a, const v3 b, const v3 c, const v3 d)
{
	f32 i = 1.0f - f;

    return (v3)
    {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
    };
}

// ----------------------------- v4 ---------------------------- //

inline v4
v4_from_packed_color(u32 color)
{
    return (v4)
    {
        ((color & 0x000000ff) >> 0)  / 256.0f,
        ((color & 0x0000ff00) >> 8)  / 256.0f,
        ((color & 0x00ff0000) >> 16) / 256.0f,
        ((color & 0xff000000) >> 24) / 256.0f,
    };
}

inline f32
v4_dot(v4 a, v4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32
v4_len_sq(v4 v)
{
    return v4_dot(v, v);
}

inline f32
v4_len(v4 v)
{
    return sqrt(v4_dot(v, v));
}

inline f32
v4_dist_sq(const v4 a, const v4 b)
{
    f32 dx = b.x - a.x;
    f32 dy = b.y - a.y;
    f32 dz = b.z - a.z;
    f32 dw = b.w - a.w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

inline f32
v4_dist(v4 a, v4 b)
{
    return sqrt(v4_dist_sq(a, b));
}

inline v4
v4_norm(v4 v)
{
    f32 k = rsqrt(v4_dot(v, v));

    return (v4)
    {
        k * v.x,
        k * v.y,
        k * v.z,
        k * v.w,
    };
}

inline v4
v4_min(v4 a, v4 b)
{
    return (v4)
    {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w,
    };
}

inline v4
v4_max(v4 a, v4 b)
{
    return (v4)
    {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w,
    };
}

inline v4
v4_lerp(v4 out, v4 a, v4 b, f32 t)
{
    return (v4)
    {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w),
    };
}

inline v4
v4_spline(f32 f, v4 a, v4 b, v4 c, v4 d)
{
	f32 i = 1.0f - f;

    return (v4)
    {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
        ((d.w * f + c.w * i) * f + (c.w * f + b.w * i) * i) * f + ((c.w * f + b.w * i) * f + (b.w * f + a.w * i) * i) * i,
    };
}

// m2:
inline m2
m2_mul(m2 a, m2 b)
{
    return (m2)
    {
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3],
    };
}

inline v2
m2_mulv(m2 m, v2 v)
{
    return (v2)
    {
        m.e[0] * v.x + m.e[2] * v.y,
        m.e[1] * v.x + m.e[3] * v.y,
    };
}

inline m2
m2_identity(void)
{
    return (m2)
    {
        1.0f, 0.0f,
        0.0f, 1.0f
    };
}

inline m2
m2_rotate_cs(f32 c, f32 s)
{
    return (m2)
    {
         c, s,
        -s, c,
    };
}

inline m2
m2_rotate(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return (m2)
    {
         c,  s,
        -s,  c,
    };
}

inline m2
m2_scale(f32 sx, f32 sy)
{
    return (m2)
    {
        sx, 0,
        0,  sy,
    };
}

inline f32
m2_det(m2 m)
{
    return m.e[0] * m.e[2] - m.e[1] * m.e[3];
}

inline m2
m2_transpose(m2 m)
{
    return (m2)
    {
        m.e[0], m.e[2],
        m.e[1], m.e[3],
    };
}

inline m2
m2_inverse(m2 m)
{
    f32 k = 1.0 / m2_det(m);

    return (m2)
    {
         k * m.e[3],
        -k * m.e[2],
        -k * m.e[1],
         k * m.e[0],
    };
}

// m3:
inline m3
m3_mul(m3 a, m3 b)
{
    return (m3)
    {
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

inline v3
m3_mulv(m3 m, v3 v)
{
    return (v3)
    {
        m.e[0] * v.x + m.e[3] * v.y + m.e[6] * v.z,
        m.e[1] * v.x + m.e[4] * v.y + m.e[7] * v.z,
        m.e[2] * v.x + m.e[5] * v.y + m.e[8] * v.z,
    };
}

inline m3
m3_identity(m3 out)
{
    return (m3)
    {
        1,  0,  0,
        0,  1,  0,
        0,  0,  1,
    };
}

inline m3
m3_rotate(v3 axis, f32 angle)
{
    f32 c       = cosf(angle);
    f32 s       = sinf(angle);

    f32 k       = 1.0f - c;

    v3  sa      = { s * axis.x, s * axis.y, s * axis.z };
    v3  omca    = { k * axis.x, k * axis.y, k * axis.z };

    return (m3)
    {
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

inline m3
m3_euler(f32 yaw, f32 pitch, f32 roll)
{
    f32 cy = cosf(yaw);
    f32 sy = sinf(yaw);
    f32 cp = cosf(pitch);
    f32 sp = sinf(pitch);
    f32 cr = cosf(roll);
    f32 sr = sinf(roll);

    return (m3)
    {
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

inline m3
m3_from_quat(quat_t quat)
{
    f32 a = quat.w;
	f32 b = quat.x;
	f32 c = quat.y;
	f32 d = quat.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (m3)
    {
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
inline m4
m4_mul(m4 a, m4 b)
{
    return (m4)
    {
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

inline v4
m4_mulv(m4 m, v4 v)
{
    return (v4)
    {
        v.x + m.e[4] * v.y + m.e[8]  * v.z + m.e[12] * v.w,
        v.x + m.e[5] * v.y + m.e[9]  * v.z + m.e[13] * v.w,
        v.x + m.e[6] * v.y + m.e[10] * v.z + m.e[14] * v.w,
        v.x + m.e[7] * v.y + m.e[11] * v.z + m.e[15] * v.w,
    };
}

inline m4
m4_identity(void)
{
    return (m4)
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

inline m4
m4_transpose(m4 m)
{
    return (m4)
    {
        m.e[0], m.e[4], m.e[8],  m.e[12],
        m.e[1], m.e[5], m.e[9],  m.e[13],
        m.e[2], m.e[6], m.e[10], m.e[14],
        m.e[3], m.e[7], m.e[11], m.e[15],
    };
}

inline m4
m4_translate(f32 x, f32 y, f32 z)
{
    return (m4)
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
}

inline m4
m4_scale(f32 x, f32 y, f32 z)
{
    return (m4)
    {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1,
    };
}

inline m4
m4_rotate_x(f32 angle)
{
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (m4)
    {
        1,  0, 0, 0,
        0,  c, s, 0,
        0, -s, c, 0,
        0,  0, 0, 1,
    };
}

inline m4
m4_rotate_y(f32 angle)
{
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (m4)
    {
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1,
    };
}

inline m4
m4_rotate_z(f32 angle)
{
	f32 s = sinf(angle);
	f32 c = cosf(angle);

    return (m4)
    {
         c, s, 0, 0,
        -s, c, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1,
    };
}

inline m4
m4_rotate(v3 axis, f32 angle)
{
    f32 cosv        = cosf(angle);
    f32 sinv        = sinf(angle);
    f32 inv_cosv    = 1.0f - cosv;
    v3 sa           = { axis.x * sinv,      axis.y * sinv,      axis.z * sinv };
    v3 omca         = { axis.x * inv_cosv,  axis.y * inv_cosv,  axis.z * inv_cosv };

    return (m4)
    {
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0,  0,  0,  1,
    };
}

inline m4
m4_frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    return (m4)
    {
        2 * n / (r - l),    0,                  0,                      0,
        0,                  2 * n / (t - b),    0,                      0,
        (r + l) / (r - l),  (t + b) / (t - b),  -(f + n) / (f - n),     -1,
        0,                  0,                  -2 * (f * n) / (f - n), 0,
    };
}

inline m4
m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    return (m4)
    {
        2 / (r - l),            0,                      0,                      0,
        0,                      2 / (t - b),            0,                      0,
        0,                      0,                      -2 / (f - n),           0,
        -(r + l) / (r - l),     -(t + b) / (t - b),     -(f + n) / (f - n),     1,
    };
}

inline m4
m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f)
{
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return (m4)
    {
        a / aspect,     0,      0,                          0,
        0,              a,      0,                          0,
        0,              0,      -((f + n) / (f - n)),       -1,
        0,              0,      -((2 * f * n) / (f - n)),   0,
    };
}

inline m4
m4_look_at(v3 eye, v3 center, v3 up)
{
    v3 f = v3_norm((v3) { center.x - eye.x, center.y - eye.y, center.z - eye.z });
    v3 s = v3_norm(v3_cross(f, up));
    v3 t = v3_cross(s, f);

    m4 m = {0};

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

inline m4
m4_from_quat(quat_t q)
{
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (m4)
    {
        a2 + b2 - c2 - d2,      2  * (b * c + a * d),   2  * (b * d - a * c),   0,
        2  * (b * c - a * d),   a2 - b2 + c2 - d2,      2  * (c * d + a * b),   0,
        2  * (b * d + a * c),   2  * (c * d - a * b),   a2 - b2 - c2 + d2,      0,
        0,                      0,                      0,                      1,
    };
}

inline m4
m4_invert(m4 m)
{
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

    return (m4)
    {
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

// quat_t:
inline quat_t
quat_identity(void)
{
    return (quat_t) { 0, 0, 0, 1 };
}

inline quat_t
quat_make(f32 x, f32 y, f32 z, f32 angle)
{
    f32 inv_len = rsqrt((x * x) + (y * y) + (z * z));
    f32 s       = inv_len * sin(angle / 2.0f);

    return (quat_t)
    {
        x * s,
        y * s,
        z * s,
        cosf(angle / 2.0f),
    };
}

inline quat_t
quat_conj(quat_t q)
{
    return (quat_t) { -q.x, -q.y, -q.z, q.w };
}

inline quat_t
quat_mul(quat_t a, quat_t b)
{
    return (quat_t)
    {
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
}

inline quat_t
quat_rotate(v3 axis, f32 angle)
{
    f32 s = sinf(0.5f * angle);
    v3  v = { s * axis.x, s * axis.y, s * axis.z };

    return (quat_t) { v.x, v.y, v.z, cosf(0.5f * angle) };
}

// --------------------- v2i ------------------------- // 

inline b32
v2i_equal(v2i a, v2i b)
{
    return a.x == b.x && a.y == b.y;
}

inline i32
v2i_dist_sq(v2i a, v2i b)
{
    i32 dx = a.x - b.x;
    i32 dy = a.y - b.y;

    return dx * dx + dy * dy;
}

inline i32
v2i_manhattan(v2i a, v2i b)
{
    i32 dx = abs(a.x - b.x);
    i32 dy = abs(a.y - b.y);

    return dx + dy;
}

inline v2i
v2i_clamp(v2i a, i32 min, i32 max)
{
    return (v2i)
    {
        clamp(a.x, min, max),
        clamp(a.y, min, max),
    };
}

inline v2i
v2i_min(v2i a, v2i b)
{
    return (v2i)
    {
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
    };
}

inline v2i
v2i_max(v2i a, v2i b)
{
    return (v2i)
    {
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
    };
}

// --------------------- v3i ------------------------- // 

inline i32
v3i_dist_sq(v3i a, v3i b)
{
    i32 dx = a.x - b.x;
    i32 dy = a.y - b.y;
    i32 dz = a.z - b.z;

    return dx * dx + dy * dy + dz * dz;
}

// =================================================== SHAPES =============================================== //

// ---------------------------------------- CIRCLE -------------------------------------- //

typedef struct circle_t circle_t;
struct circle_t
{
    v3      pos;
    f32     rad;
};

inline b32
circle_intersect(circle_t a, circle_t b)
{
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

inline v2
circle_get_intersect_vector(circle_t a, circle_t b)
{
    v2  delta = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
    f32 depth = v2_len(delta) - (a.rad + b.rad);
    
    return (v2)
    {
        depth * delta.x,
        depth * delta.y,
    };
}

// --------------------------------------- SHPERE ------------------------------------ //

typedef struct sphere_t sphere_t;
struct sphere_t
{
    v3      pos;
    f32     rad;
};

inline b32
sphere_intersect(sphere_t a, sphere_t b)
{
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 dz  = b.pos.z - a.pos.z;

    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

inline v3
sphere_get_intersect_vector(sphere_t a, sphere_t b)
{
    v3 delta  = { b.pos.x - a.pos.x, b.pos.y - a.pos.y, b.pos.z - a.pos.z };
    f32 depth = v3_len(delta) - (a.rad + b.rad);
    
    return (v3)
    {
        depth * delta.x,
        depth * delta.y,
        depth * delta.z,
    };
}

// ---------------------------------------- RECTANGLE ------------------------------------- //

typedef struct rect_t rect_t;
struct rect_t
{
    v2  min;
    v2  max;
};

inline b32
rect_contains(rect_t rect, v2 pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

inline b32
rect_intersect(rect_t a, rect_t b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

inline rect_t
rect_get_overlap(rect_t a, rect_t b)
{
    return (rect_t)
    {
        v2_max(a.min, b.min),
        v2_min(a.max, b.max),
    };
}

inline v2
rect_get_intersect_vector(rect_t a, rect_t b)
{
    rect_t  o   = rect_get_overlap(a, b);
    f32     dx  = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32     dy  = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);

    return (v2)
    {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
    };
}

inline rect_t
rect_move(rect_t rect, v2 offset)
{
    return (rect_t)
    {
        rect.min.x + offset.x,
        rect.min.y + offset.y,
        rect.max.x + offset.x,
        rect.max.y + offset.y,
    };
}

// ---------------------------------------- RECTANGLE - INT ------------------------------------- //

typedef struct rect_int_t rect_int_t;
struct rect_int_t
{
    v2i  min;
    v2i  max;
};

inline b32
rect_int_contains(rect_int_t rect, v2i pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

inline b32
rect_int_intersect(rect_int_t a, rect_int_t b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

inline rect_int_t
rect_int_get_overlap(rect_int_t a, rect_int_t b)
{
    return (rect_int_t)
    {
        v2i_max(a.min, b.min),
        v2i_min(a.max, b.max),
    };
}

inline v2i
rect_int_get_intersect_vector(rect_int_t a, rect_int_t b)
{
    rect_int_t  o   = rect_int_get_overlap(a, b);
    f32     dx  = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32     dy  = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);

    return (v2i)
    {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
    };
}

inline rect_int_t
rect_int_move(rect_int_t rect, v2i offset)
{
    return (rect_int_t)
    {
        rect.min.x + offset.x,
        rect.min.y + offset.y,
        rect.max.x + offset.x,
        rect.max.y + offset.y,
    };
}

// ----------------------------------------- BOX --------------------------------------- //

typedef struct box_t box_t;
struct box_t
{
    v3  min;
    v3  max;
};

inline b32
box_contains(box_t rect, v3 pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

inline b32
box_intersect(box_t a, box_t b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

inline box_t
box_get_overlap(box_t a, box_t b)
{
    return (box_t)
    {
        v3_max(a.min, b.min),
        v3_min(a.max, b.max),
    };
}

inline v3
box_get_intersect_vector(box_t a, box_t b)
{
    box_t o = box_get_overlap(a, b);
    f32 dx  = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32 dy  = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);
    f32 dz  = 0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z);

    return (v3)
    {
        sign(dx) * (o.max.x - o.min.x),
        sign(dy) * (o.max.y - o.min.y),
        sign(dz) * (o.max.z - o.min.z),
    };
}

inline box_t
box_move(box_t box, v3 offset)
{
    return (box_t)
    {
        box.min.x + offset.x,
        box.min.y + offset.y,
        box.min.z + offset.z,

        box.max.x + offset.x,
        box.max.y + offset.y,
        box.max.z + offset.z,
    };
}

// ----------------------------------------- FRUSTUM --------------------------------------- //

typedef struct plane_t plane_t;
struct plane_t
{
    f32     a;
    f32     b;
    f32     c;
    f32     d;
};

inline plane_t
plane_normalize(plane_t plane)
{
    f32 mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;

    return plane;
}

typedef struct frustum_t frustum_t;
struct frustum_t
{
    plane_t   plane[6];
};

inline frustum_t
frustum_create(m4 m, bool normalize)
{
    frustum_t frustum = {0};

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

    if (normalize)
    {
        frustum.plane[0] = plane_normalize(frustum.plane[0]);
        frustum.plane[1] = plane_normalize(frustum.plane[1]);
        frustum.plane[2] = plane_normalize(frustum.plane[2]);
        frustum.plane[3] = plane_normalize(frustum.plane[3]);
        frustum.plane[4] = plane_normalize(frustum.plane[4]);
        frustum.plane[5] = plane_normalize(frustum.plane[5]);
    }
    
    return frustum;
}

inline b32
frustum_contains(frustum_t frustum, v3 pos)
{
    for(i32 i = 0; i < 6; i++)
    {
		if (frustum.plane[i].a * pos.x + frustum.plane[i].b * pos.y + frustum.plane[i].c * pos.z + frustum.plane[i].d <= 0)
			return false;
	}

	return true;
}

inline b32
frustum_intersect_sphere(frustum_t frustum, sphere_t sphere)
{
    for(i32 i = 0; i < 6; i++)
    {
		if(frustum.plane[i].a * sphere.pos.x + frustum.plane[i].b * sphere.pos.y + frustum.plane[i].c * sphere.pos.z + frustum.plane[i].d <= -sphere.rad) {
			return false;
		}
	}

	return true;
}

inline b32
frustum_intersect_box(frustum_t frustum, box_t box)
{
    for (int i = 0; i < 6; i++)
    {
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

inline u32
pack_color_u8(u8 r, u8 g, u8 b, u8 a)
{
    u32 color = 0;

    color |= (u32)(r) << 0;
    color |= (u32)(g) << 8;
    color |= (u32)(b) << 16;
    color |= (u32)(a) << 24;

    return color;
}

inline u32
pack_color_f32(f32 r, f32 g, f32 b, f32 a)
{
    return pack_color_u8(255 * r, 255 * g, 255 * b, 255 * a);
}

inline u32
pack_color_v4(v4 color)
{
    return pack_color_f32(color.r, color.g, color.b, color.a);
}

inline u32
pack_color_v3(v3 color)
{
    return pack_color_f32(color.r, color.g, color.b, 1.0);
}

// =================================== XORSHIFT32 ============================================= //

inline u32
rand_u32(u32* state)
{
	u32 x = *state;

	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

	return *state = x;
}

inline i32
rand_i32(u32* state, i32 min, i32 max)
{
    return min + rand_u32(state) % (max - min);
}

inline f32
rand_f32(u32* state, f32 min, f32 max)
{
    return min + ((f32)rand_u32(state) / (f32)0xFFFFFFFF) * (max - min); 
}

// ============================ RANDOM(HASH) FUNCTION ======================= //

inline u32
hash_u32(u32 a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

    return a;
}

inline u32
hash_str(const char* str)
{
    u32 hash = 0;

    while (*str)
        hash = (hash << 7) + (hash >> 25) + *str++;

    return hash + (hash >> 16);
}

inline u32
hash_mem(const void* ptr, u32 size)
{
    const u8*   m       = ptr; 
    u32         hash    = 0;

    for (u32 i = 0; i < size; ++i)
        hash = (hash << 7) + (hash >> 25) + m[i];

    return hash + (hash >> 16);
}

// ========================================= PRIORITY QUEUE =================================== //

typedef struct queue_node_t queue_node_t;
struct queue_node_t
{
    f32     weight;
    v2i     e;
};

typedef struct priority_queue_t priority_queue_t;
struct priority_queue_t
{
    i32             len;
    queue_node_t*   array;
};

inline b32
queue_empty(const priority_queue_t* queue)
{
    return queue->len == 0;
}

inline void
queue_clear(priority_queue_t* queue)
{
    queue->len = 0;
}

inline void
queue_push(priority_queue_t* queue, v2i e, f32 weight)
{
    queue_node_t node = { weight, e };

    int i = queue->len + 1;
    int j = i / 2;

    while (i > 1 && queue->array[j].weight > node.weight)
    {
        queue->array[i] = queue->array[j];

        i = j;
        j = j / 2;
    }

    queue->array[i] = node;
    queue->len++;
}

inline f32
queue_pop(v2i* out, priority_queue_t* queue)
{
    queue_node_t data = queue->array[1];

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

inline size_t
file_get_size(FILE* fp)
{
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

inline char*
file_read_str(const char* file_name)
{
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0)
    {
        size_t size = file_get_size(fp);
        buffer = (char*)malloc(size + 1);

        if (buffer)
        {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0)
            {
                free(buffer);
                buffer = 0;
            }
        }

        fclose(fp);
    }

    return buffer;
}

inline b32
file_write_str(const char* file_name, const char* buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0)
    {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return false;
}

inline b32
file_append_str(const char* file_name, const char* buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0)
    {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);

        fclose(fp);
        return n == size;
    }

    return false;
}

inline b32
file_read_bin(const char* file_name, void* buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0)
    {
        fread(buffer, size, 1, fp);
        fclose(fp);

        return 1;
    }

    return false;
} 

inline b32
file_write_bin(const char* file_name, const void* buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0)
    {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }

    return false;
}

// ====================================================================================================== //

inline void
f4x4_mul_64(f64 *R, const f64 *a, const f64 *b)
{
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

inline void
f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v)
{
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

inline void
f4x4_invert_64(f64* T, const f64* M)
{
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

inline b32
f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport)
{
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

inline b32
f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport)
{
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

typedef struct gamepad_buttons_t gamepad_buttons_t;
union gamepad_buttons_t
{
    struct
    {
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

typedef struct gamepad_t gamepad_t;
struct gamepad_t
{
    b32     active;

    v2      LS;
    v2      RS;

    f32     LT;
    f32     RT;

    gamepad_buttons_t  state;
    gamepad_buttons_t  pressed;
    gamepad_buttons_t  released;
};

// =========================================================== MOUSE MODES ================================================= //

enum
{
    MOUSE_NORMAL,
    MOUSE_HIDDEN,
    MOUSE_DISABLED
};

// ===========================================================  PLATFORM =================================================== //

global struct
{
    b32     close;

    i32     width;
    i32     height;
    f32     aspect_ratio;

    HWND    native;

    b32     fullscreen;
    b32     _fullscreen_state_last_update;

    struct
    {
        f64     total;
        f64     delta;
    } time;

    struct
    {
        u32     mode;

        b32     is_down     : 1;
        b32     is_pressed  : 1;
        b32     is_released : 1;

        v2      pos;
        v2      delta;
        v2      scroll;

        b8      state[MOUSE_BUTTON_LAST + 1];
        b8      pressed[MOUSE_BUTTON_LAST + 1];
        b8      released[MOUSE_BUTTON_LAST + 1];
    } mouse;

    struct
    {
        i32     key;
        i32     ascii;

        b32     is_down     : 1;
        b32     is_pressed  : 1;
        b32     is_repeat   : 1;
        b32     is_released : 1;
        b32     is_ascii    : 1;
    
        b8      state[KEY_LAST + 1];
        b8      pressed[KEY_LAST + 1];
        b8      repeat[KEY_LAST + 1];
        b8      released[KEY_LAST + 1];
    } keyboard;

    gamepad_t gamepad[JOYSTICK_LAST];
} platform;

global struct
{
    GLFWwindow*     window;
    GLFWmonitor*    monitor;
} platform_internal;

internal void
window_key_callback(GLFWwindow* window, int key, int a, int action, int b)
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

internal void
window_char_callback(GLFWwindow* window, unsigned int codepoint)
{
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

internal void
window_mouse_button_callback(GLFWwindow* window, int button, int action, int a)
{
    (void)window;
    (void)a;

    switch (action)
    {
        case GLFW_PRESS:
            platform.mouse.is_down = 1;
            platform.mouse.is_pressed = 1;
            platform.mouse.state[button] = 1;
            platform.mouse.pressed[button] = 1;
            break;
        case GLFW_RELEASE:
            platform.mouse.is_down = 0;
            platform.mouse.is_released = 1;
            platform.mouse.state[button] = 0;
            platform.mouse.released[button] = 1;
            break;
    }
}

internal void
window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)window;

    platform.mouse.scroll.x = xoffset;
    platform.mouse.scroll.y = yoffset;
}

internal void
window_joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED)
    {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED)
    {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

void
platform_init(const char* title, int width, int height, int samples)
{
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

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i)
    {
        if (glfwJoystickPresent(i))
            platform.gamepad[i].active = 1;
    }

    glfwSetTime(0.0);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwSwapBuffers(platform_internal.window);
    glfwPollEvents();
}

void
platform_update(void)
{
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

        platform.mouse.delta.x  = x - platform.mouse.pos.x;
        platform.mouse.delta.y  = y - platform.mouse.pos.y;

        platform.mouse.pos.x    = x;
        platform.mouse.pos.y    = y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        switch (platform.mouse.mode)
        {
            case MOUSE_NORMAL:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case MOUSE_HIDDEN:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            case MOUSE_DISABLED:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
        }
    }

    // update gamepads:
    {
        GLFWgamepadstate state;

        for (int i = 0; i < JOYSTICK_LAST; ++i)
        {
            if (platform.gamepad[i].active)
            {
                gamepad_buttons_t old = platform.gamepad[i].state;

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

    if (platform.fullscreen != platform._fullscreen_state_last_update)
    {
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

inline f64
timer_get_current(void)
{
    return glfwGetTime();
}

#endif // ATS_PLATFORM_GLFW

// ===================================================== SHADERS ===================================================== //

#ifdef ATS_MODERN_OPENGL

inline u32
shader_compile(const char* source, unsigned int type)
{
    int     success;
    char    info_log[512];
    u32     shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        puts(info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

inline u32
shader_link_program(u32 vertex_shader, u32 fragment_shader)
{
    int     success;
    char    info_log[512];

    u32 shader = glCreateProgram();

    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shader, 512, NULL, info_log);
        puts(info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

inline u32
shader_load_from_memory(const char *vs, const char *fs)
{
    u32 vertex      = shader_compile(vs, GL_VERTEX_SHADER);
    u32 fragment    = shader_compile(fs, GL_FRAGMENT_SHADER);
    u32 program     = shader_link_program(vertex, fragment);

    glUseProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

inline u32
shader_load_from_file(const char *vs, const char *fs)
{
    char* vs_content = file_read_str(vs);
    char* fs_content = file_read_str(fs);

    u32 program = shader_load_from_memory(vs_content, fs_content);

    free(vs_content);
    free(fs_content);

    return program;
}

inline void
gl_get_world_position(f32 out[3], int x, int y, const f32 in_projection[16], const f32 in_modelview[16])
{
    GLint   viewport[4]     = {0};
    f64     modelview[16]   = {0};
    f64     projection[16]  = {0};

    GLfloat  win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;
 
    for (i32 i = 0; i < 16; ++i) projection[i]  = in_projection[i];
    for (i32 i = 0; i < 16; ++i) modelview[i]   = in_modelview[i];

    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f64)(x);
    win_y = (f64)(viewport[3]) - (f64)y;

    glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
}

#endif // ATS_MODERN_OPENGL

// ======================================= TEXTURES ======================================== //

#if defined(ATS_PLATFORM_GLFW)

#ifdef STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "dep/stb_image.h" 

typedef struct image_t image_t;
struct image_t
{
    int width;
    int height;

    u32* pixels;
};

inline image_t
image_load_from_file(const char* path)
{
    image_t image     = {0};
    i32     channels  = 0;

    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 0);
    assert(image.pixels);

    return image;
}

#endif

typedef struct texture_t texture_t;
struct texture_t
{
    u32     id;
    int     width;
    int     height;
};

inline texture_t
texture_create(void *pixels, int width, int height, int is_smooth)
{
    assert(pixels);

    texture_t texture = {0};

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

inline void
texture_update(texture_t* texture, void *pixels, int width, int height, int is_smooth)
{
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
inline texture_t
texture_load_from_file(const char *texture_path, int is_smooth)
{
    texture_t       texture     = {0};
    i32             channels    = 0;
    unsigned char*  pixels      = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);

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

inline void
texture_bind(const texture_t *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);

#ifndef ATS_MODERN_OPENGL
    glMatrixMode(GL_MODELVIEW);
#endif
}

inline void
texture_delete(texture_t* texture)
{
    glDeleteTextures(1, &texture->id);
    memset(texture, 0, sizeof *texture);
}

#endif

// ============================================ RENDER (LEGACY OPENGL) ============================================ //

#if defined(ATS_PLATFORM_GLFW) && !defined(ATS_MODERN_OPENGL)

inline void
gl_init(void)
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
}

inline void
gl_set_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z)
{
    f32 pos[4]  = { x, y, z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { bright, bright, bright, 0.0f };
    u32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

inline void
gl_set_light_directed(int index, f32 bright, f32 x, f32 y, f32 z)
{
    f32 d       = (f32)(1.0f / sqrt(x * x + y * y + z * z));
    f32 dir[4]  = { x * d, y * d, z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { bright, bright, bright, 0.0f };
    u32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

inline void
gl_set_light_global_ambient(f32 r, f32 g, f32 b)
{
    f32 v[4] = { r, g, b, 0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

inline void
gl_get_world_position(f32 out[3], int x, int y)
{
    GLint   viewport[4]     = {0};
    f64     modelview[16]   = {0};
    f64     projection[16]  = {0};

    GLfloat  win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;
 
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f64)(x);
    win_y = (f64)(viewport[3]) - (f64)y;

    glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
}

// ======================================= FONT ====================================== //

#define BITMAP_COUNT (256)

global const u64 bitascii[BITMAP_COUNT] =
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


int bitmap_display_list[BITMAP_COUNT];

#define BITMAP_GETBIT(N, X, Y) (((u64)(N)) & (1ull << (((u64)(Y)) * 8ull + ((u64)(X)))))

inline void
gl_init_bitmap(void)
{
    for (int i = 0; i < BITMAP_COUNT; ++i)
    {
        bitmap_display_list[i] = glGenLists(1);

        glNewList(bitmap_display_list[i], GL_COMPILE);

        u64 c = bitascii[i];

        glBegin(GL_QUADS);

        f32 scale = 1.0f / 8.0f;

        for (int j = 0; j < 8; ++j)
        {
            for (int i = 0; i < 8; ++i) {
                f32 x   = i * scale;
                f32 y   = j * scale;

                if (BITMAP_GETBIT(c, i, j))
                {
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

inline void
gl_render_ascii(u8 c, f32 x, f32 y, f32 z, f32 sx, f32 sy)
{
    m4 t  = m4_translate(x, y, z);
    m4 s  = m4_scale(sx, sy, 1);
    m4 ts = m4_mul(t, s);

    glPushMatrix();

    glMultMatrixf(ts.e);
    glCallList(bitmap_display_list[c]);

    glPopMatrix();
}

inline void
gl_render_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color)
{
    glColor4ubv((u8*)&color);

    for (int i = 0; str[i] != '\0'; i++)
    {
        gl_render_ascii(str[i], x + i * sx, y, z, sx, sy);
    }
}

inline void
gl_render_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...)
{
    va_list list;
    char    buffer[256];

    va_start(list, fmt);

    vsnprintf(buffer, 256, fmt, list);
    gl_render_string(buffer, x, y, z, sx, sy, color);

    va_end(list);
}

#endif // !ATS_MODERN_OPENGL
