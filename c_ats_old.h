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

typedef f32 v2[2];
typedef f32 v3[3];
typedef f32 v4[4];

typedef i32 v2i[2];
typedef i32 v3i[3];
typedef i32 v4i[4];

typedef f32 m2[4];
typedef f32 m3[9];
typedef f32 m4[16];

typedef f32 quat_t[4];

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
v2_dot(const v2 a, const v2 b)
{
    return a[0] * b[0] + a[1] * b[1];
}

inline f32
v2_det(const v2 a, const v2 b)
{
    return a[0] * b[1] - a[1] * b[0];
}

inline f32
v2_len_sq(const v2 v)
{
    return v2_dot(v, v);
}

inline f32
v2_len(const v2 v)
{
    return sqrt(v2_dot(v, v));
}

inline f32
v2_dist_sq(const v2 a, const v2 b)
{
    f32 delta[2] =
    {
        a[0] - b[0],
        a[1] - b[1]
    };

    return v2_len_sq(delta);
}

inline f32
v2_dist(const v2 a, const v2 b)
{
    return sqrt(v2_dist_sq(a, b));
}

inline void
v2_proj(v2 out, const v2 a, const v2 b)
{
    f32 k = v2_dot(a, a) / v2_dot(b, b);

    out[0] = k * b[0];
    out[1] = k * b[1];
}

inline void
v2_norm(v2 out, const v2 v)
{
    f32 k = rsqrt(v2_dot(v, v));

    out[0] = k * v[0];
    out[1] = k * v[1];
}

inline void
v2_min(v2 out, const v2 a, const v2 b)
{
    out[0] = a[0] < b[0]? a[0] : b[0];
    out[1] = a[1] < b[1]? a[1] : b[1];
}

inline void
v2_max(v2 out, const v2 a, const v2 b)
{
    out[0] = a[0] > b[0]? a[0] : b[0];
    out[1] = a[1] > b[1]? a[1] : b[1];
}

inline void
v2_lerp(v2 out, const v2 a, const v2 b, f32 t)
{
    out[0] = a[0] + t * (b[0] - a[0]);
    out[1] = a[1] + t * (b[1] - a[1]);
}

inline f32
v2_get_angle(const v2 a, const v2 b)
{
    f32 det = a[0] * b[1] - b[0] * a[1];
    f32 dot = a[0] * b[0] + a[1] * b[1];
    
    return atan2f(det, dot);
}

inline void
v2_spline(v2 out, f32 f, const v2 a, const v2 b, const v2 c, const v2 d)
{
	f32 i = 1.0f - f;

	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
}

// ----------------------------- v3 ----------------------------- //

inline f32
v3_dot(const v3 a, const v3 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline f32
v3_len_sq(const v3 v)
{
    return v3_dot(v, v);
}

inline f32
v3_len(const v3 v)
{
    return sqrt(v3_dot(v, v));
}

inline f32
v3_dist_sq(const v3 a, const v3 b)
{
    v3 delta = {
        a[0] - b[0],
        a[1] - b[1],
        a[2] - b[2]
    };

    return v3_len_sq(delta);
}

inline f32
v3_dist(const v3 a, const v3 b)
{
    return sqrt(v3_dist_sq(a, b));
}

inline void
v3_proj(v3 out, const v3 a, const v3 b)
{
    f32 k = v3_dot(a, b) / v3_dot(b, b);

    out[0] = k * b[0];
    out[1] = k * b[1];
    out[2] = k * b[2];
}

inline void
v3_norm(v3 out, const v3 v)
{
    f32 k = rsqrt(v3_dot(v, v));

    out[0] = k * v[0];
    out[1] = k * v[1];
    out[2] = k * v[2];
}

inline void
v3_min(v3 out, const v3 a, const v3 b)
{
    out[0] = a[0] < b[0]? a[0] : b[0];
    out[1] = a[1] < b[1]? a[1] : b[1];
    out[2] = a[2] < b[2]? a[2] : b[2];
}

inline void
v3_max(v3 out, const v3 a, const v3 b)
{
    out[0] = a[0] > b[0]? a[0] : b[0];
    out[1] = a[1] > b[1]? a[1] : b[1];
    out[2] = a[2] > b[2]? a[2] : b[2];
}

inline void
v3_lerp(v3 out, const v3 a, const v3 b, f32 t)
{
    out[0] = a[0] + t * (b[0] - a[0]);
    out[1] = a[1] + t * (b[1] - a[1]);
    out[2] = a[2] + t * (b[2] - a[2]);
}

inline void
v3_cross(v3 out, const v3 a, const v3 b)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

inline void
v3_spline(v3 out, f32 f, const v3 a, const v3 b, const v3 c, const v3 d)
{
	f32 i = 1.0f - f;

    out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
    out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
    out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
}

// ----------------------------- v4 ---------------------------- //

inline void
v4_from_packed_color(v4 out, u32 color)
{
    out[0] = ((color & 0x000000ff) >> 0)  / 256.0f;
    out[1] = ((color & 0x0000ff00) >> 8)  / 256.0f;
    out[2] = ((color & 0x00ff0000) >> 16) / 256.0f;
    out[3] = ((color & 0xff000000) >> 24) / 256.0f;
}

inline f32
v4_dot(const v4 a, const v4 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

inline f32
v4_len_sq(const v4 v)
{
    return v4_dot(v, v);
}

inline f32
v4_len(const v4 v)
{
    return sqrt(v4_dot(v, v));
}

inline f32
v4_dist_sq(const v4 a, const v4 b)
{
    f32 delta[4] = {
        b[0] - a[0],
        b[1] - a[1],
        b[2] - a[2],
        b[3] - a[3],
    };

    return v4_len_sq(delta);
}

inline f32
v4_dist(const v4 a, const v4 b)
{
    return sqrt(v4_dist_sq(a, b));
}

inline void
v4_norm(v4 out, const v4 v)
{
    f32 k = rsqrt(v4_dot(v, v));

    out[0] = k * v[0];
    out[1] = k * v[1];
    out[2] = k * v[2];
    out[3] = k * v[3];
}

inline void
v4_min(v4 out, const v4 a, const v4 b)
{
    out[0] = a[0] < b[0]? a[0] : b[0];
    out[1] = a[1] < b[1]? a[1] : b[1];
    out[2] = a[2] < b[2]? a[2] : b[2];
    out[3] = a[3] < b[3]? a[3] : b[3];
}

inline void
v4_max(v4 out, const v4 a, const v4 b)
{
    out[0] = a[0] > b[0]? a[0] : b[0];
    out[1] = a[1] > b[1]? a[1] : b[1];
    out[2] = a[2] > b[2]? a[2] : b[2];
    out[3] = a[3] > b[3]? a[3] : b[3];
}

inline void
v4_lerp(v4 out, const v4 a, const v4 b, f32 t)
{
    out[0] = a[0] + t * (b[0] - a[0]);
    out[1] = a[1] + t * (b[1] - a[1]);
    out[2] = a[2] + t * (b[2] - a[2]);
    out[3] = a[3] + t * (b[3] - a[3]);
}

inline void
v4_cross(v4 out, const v4 a, const v4 b)
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
    out[3] = 1.0;
}

inline void
v4_spline(v4 out, f32 f, const v4 a, const v4 b, const v4 c, const v4 d)
{
	f32 i = 1.0f - f;

    out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
    out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
    out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
    out[3] = ((d[3] * f + c[3] * i) * f + (c[3] * f + b[3] * i) * i) * f + ((c[3] * f + b[3] * i) * f + (b[3] * f + a[3] * i) * i) * i;
}

// m2:
inline void
m2_mul(m2 out, const m2 A, const m2 B)
{
    m2 tmp;

    tmp[0] = A[0] * B[0] + A[2] * B[1];
    tmp[1] = A[1] * B[0] + A[3] * B[1];
    tmp[2] = A[0] * B[2] + A[2] * B[3];
    tmp[3] = A[1] * B[2] + A[3] * B[3];

    out[0] = tmp[0];
    out[1] = tmp[1];
    out[2] = tmp[2];
    out[3] = tmp[3];
}

inline void
m2_mulv(m2 out, const m2 R, const v2 v)
{
    m2 tmp;

    tmp[0] = R[0] * v[0] + R[2] * v[1];
    tmp[1] = R[1] * v[0] + R[3] * v[1];

    out[0] = tmp[0];
    out[1] = tmp[1];
}

inline void
m2_identity(m2 out)
{
    out[0] = 1.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
}

inline void
m2_rotate_cs(m2 out, f32 co, f32 si)
{
    out[0] =  co;
    out[1] =  si;
    out[2] = -si;
    out[3] =  co;
}

inline void
m2_rotate(m2 out, f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    out[0] =  c;
    out[1] =  s;
    out[2] = -s;
    out[3] =  c;
}

inline void
m2_scale(m2 out, f32 sx, f32 sy)
{
    out[0] = sx;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = sy;
}

inline f32
m2_det(const m2 m)
{
    return m[0] * m[2] - m[1] * m[3];
}

inline void
m2_transpose(m2 out, const m2 m)
{
    out[0] = m[0];
    out[1] = m[2];
    out[2] = m[1];
    out[3] = m[3];
}

inline void
m2_inverse(m2 out, const m2 m)
{
    f32 k = 1.0 / m2_det(m);

    out[0] =  k * m[3];
    out[1] = -k * m[2];
    out[2] = -k * m[1];
    out[3] =  k * m[0];
}

// m3:
inline void
m3_mul(m3 out, const m3 A, const m3 B)
{
    m3 tmp;

    tmp[0] = A[0] * B[0] + A[3] * B[1]  + A[6] * B[2];
    tmp[1] = A[1] * B[0] + A[4] * B[1]  + A[7] * B[2];
    tmp[2] = A[2] * B[0] + A[5] * B[1]  + A[8] * B[2];

    tmp[3] = A[0] * B[3] + A[3] * B[4]  + A[6] * B[5];
    tmp[4] = A[1] * B[3] + A[4] * B[4]  + A[7] * B[5];
    tmp[5] = A[2] * B[3] + A[5] * B[4]  + A[8] * B[5];

    tmp[6] = A[0] * B[6] + A[3] * B[7]  + A[6] * B[8];
    tmp[7] = A[1] * B[6] + A[4] * B[7]  + A[7] * B[8];
    tmp[8] = A[2] * B[6] + A[5] * B[7]  + A[8] * B[8];

    out[0] = tmp[0];
    out[1] = tmp[1];
    out[2] = tmp[2];

    out[3] = tmp[3];
    out[4] = tmp[4];
    out[5] = tmp[5];

    out[6] = tmp[6];
    out[7] = tmp[7];
    out[8] = tmp[8];
}

inline void
m3_mulv(m3 out, const m3 M, const v3 v)
{
    out[0] = M[0] * v[0] + M[3] * v[1] + M[6] * v[2];
    out[1] = M[1] * v[0] + M[4] * v[1] + M[7] * v[2];
    out[2] = M[2] * v[0] + M[5] * v[1] + M[8] * v[2];
}

inline void m3_identity(m3 out)
{
    out[0] = 1.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;

    out[3] = 0.0f;
    out[4] = 1.0f;
    out[5] = 0.0f;

    out[6] = 0.0f;
    out[7] = 0.0f;
    out[8] = 1.0f;
}

inline void
m3_rotate(m3 out, const v3 axis, f32 angle)
{
    f32 c       = cosf(angle);
    f32 s       = sinf(angle);

    f32 k       = 1.0f - c;

    v3  sa      = { s * axis[0], s * axis[1], s * axis[2] };
    v3  omca    = { k * axis[0], k * axis[1], k * axis[2] };

    out[0] = omca[0] * axis[0] + c;
    out[1] = omca[0] * axis[1] - sa[2];
    out[2] = omca[0] * axis[2] + sa[1];

    out[3] = omca[1] * axis[0] + sa[2];
    out[4] = omca[1] * axis[1] + c;
    out[5] = omca[1] * axis[2] - sa[0];

    out[6] = omca[2] * axis[0] - sa[1];
    out[7] = omca[2] * axis[1] + sa[0];
    out[8] = omca[2] * axis[2] + c;
}

inline void
m3_euler(m3 out, f32 yaw, f32 pitch, f32 roll)
{
    f32 cy = cosf(yaw);
    f32 sy = sinf(yaw);
    f32 cp = cosf(pitch);
    f32 sp = sinf(pitch);
    f32 cr = cosf(roll);
    f32 sr = sinf(roll);

    out[0] = cy * cp;
    out[1] = sy * cp;
    out[2] = -sp;

    out[3] = cy * sp * sr - sy * cr;
    out[4] = sy * sp * sr + cy * cr;
    out[5] = cp * sr;

    out[6] = cy * sp * cr + sy * sr;
    out[7] = sy * sp * cr - cy * sr;
    out[8] = cp * cr;
}

inline void
m3_from_quat(m3 out, const quat_t quat)
{
    f32 a = quat[3];
	f32 b = quat[0];
	f32 c = quat[1];
	f32 d = quat[2];

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    out[0] = a2 + b2 - c2 - d2;
    out[1] = 2.0f * (b * c + a * d);
    out[2] = 2.0f * (b * d - a * c);

    out[3] = 2.0f * (b * c - a * d);
    out[4] = a2 - b2 + c2 - d2;
    out[5] = 2.0f * (c * d + a * b);

    out[6] = 2.0f * (b * d + a * c);
    out[7] = 2.0f * (c * d - a * b);
    out[8] = a2 - b2 - c2 + d2;
}

// m4:
inline void
m4_mul(m4 out, const m4 A, const m4 B)
{
    m4 tmp;

    tmp[0]  = A[0] * B[0]  + A[4] * B[1]  + A[8]  * B[2]  + A[12] * B[3];
    tmp[1]  = A[1] * B[0]  + A[5] * B[1]  + A[9]  * B[2]  + A[13] * B[3];
    tmp[2]  = A[2] * B[0]  + A[6] * B[1]  + A[10] * B[2]  + A[14] * B[3];
    tmp[3]  = A[3] * B[0]  + A[7] * B[1]  + A[11] * B[2]  + A[15] * B[3];

    tmp[4]  = A[0] * B[4]  + A[4] * B[5]  + A[8]  * B[6]  + A[12] * B[7];
    tmp[5]  = A[1] * B[4]  + A[5] * B[5]  + A[9]  * B[6]  + A[13] * B[7];
    tmp[6]  = A[2] * B[4]  + A[6] * B[5]  + A[10] * B[6]  + A[14] * B[7],
    tmp[7]  = A[3] * B[4]  + A[7] * B[5]  + A[11] * B[6]  + A[15] * B[7];

    tmp[8]  = A[0] * B[8]  + A[4] * B[9]  + A[8]  * B[10] + A[12] * B[11];
    tmp[9]  = A[1] * B[8]  + A[5] * B[9]  + A[9]  * B[10] + A[13] * B[11];
    tmp[10] = A[2] * B[8]  + A[6] * B[9]  + A[10] * B[10] + A[14] * B[11];
    tmp[11] = A[3] * B[8]  + A[7] * B[9]  + A[11] * B[10] + A[15] * B[11];

    tmp[12] = A[0] * B[12] + A[4] * B[13] + A[8]  * B[14] + A[12] * B[15];
    tmp[13] = A[1] * B[12] + A[5] * B[13] + A[9]  * B[14] + A[13] * B[15];
    tmp[14] = A[2] * B[12] + A[6] * B[13] + A[10] * B[14] + A[14] * B[15];
    tmp[15] = A[3] * B[12] + A[7] * B[13] + A[11] * B[14] + A[15] * B[15];

    out[0]  = tmp[0];
    out[1]  = tmp[1];
    out[2]  = tmp[2];
    out[3]  = tmp[3];

    out[4]  = tmp[4];
    out[5]  = tmp[5];
    out[6]  = tmp[6];
    out[7]  = tmp[7];

    out[8]  = tmp[8];
    out[9]  = tmp[9];
    out[10] = tmp[10];
    out[11] = tmp[11];

    out[12] = tmp[12];
    out[13] = tmp[13];
    out[14] = tmp[14];
    out[15] = tmp[15];
}

inline void
m4_mulv(m4 out, const m4 M, const v4 v)
{
    out[4] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
    out[4] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
    out[4] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
    out[4] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];
}

inline void
m4_identity(m4 out)
{
    out[0]  = 1.0f;
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = 1.0f;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = 1.0f;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_transpose(m4 out, const m4 N)
{
    out[0]  = N[0];
    out[1]  = N[4];
    out[2]  = N[8];
    out[3]  = N[12];

    out[4]  = N[1];
    out[5]  = N[5];
    out[6]  = N[9];
    out[7]  = N[13];

    out[8]  = N[2];
    out[9]  = N[6];
    out[10] = N[10];
    out[11] = N[14];
    
    out[12] = N[3];
    out[13] = N[7];
    out[14] = N[11];
    out[15] = N[15];
}

inline void
m4_translate(m4 out, f32 x, f32 y, f32 z)
{
    out[0]  = 1.0f;
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = 1.0f;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = 1.0f;
    out[11] = 0.0f;

    out[12] = x;
    out[13] = y;
    out[14] = z;
    out[15] = 1.0f;
}

inline void
m4_scale(m4 out, f32 x, f32 y, f32 z)
{
    out[0]  = x;
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = y;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = z;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_rotate_x(m4 out, f32 angle)
{
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    out[0]  = 1.0f;
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = c;
    out[6]  = s;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = -s;
    out[10] = c;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_rotate_y(m4 out, f32 angle)
{
    f32 s = sinf(angle);
	f32 c = cosf(angle);

    out[0]  = c;
    out[1]  = 0.0f;
    out[2]  = s;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = 1.0f;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = -s;
    out[9]  = 0.0f;
    out[10] = c;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_rotate_z(m4 out, f32 angle)
{
	f32 s = sinf(angle);
	f32 c = cosf(angle);

    out[0]  = c;
    out[1]  = s;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = -s;
    out[5]  = c;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = 1.0f;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_rotate(m4 out, const v3 axis, f32 angle)
{
    v3 sa;
    v3 omca;

    f32 cosv        = cosf(angle);
    f32 sinv        = sinf(angle);
    f32 inv_cosv    = 1.0f - cosv;

    sa[0]   = axis[0] * sinv;
    sa[1]   = axis[1] * sinv;
    sa[2]   = axis[2] * sinv;

    omca[0] = axis[0] * inv_cosv;
    omca[1] = axis[1] * inv_cosv;
    omca[2] = axis[2] * inv_cosv;

    out[0]  = omca[0] * axis[0] + cosv;
    out[1]  = omca[0] * axis[1] - sa[0];
    out[2]  = omca[0] * axis[2] + sa[1];
    out[3]  = 0.0f;

    out[4]  = omca[1] * axis[0] + sa[2];
    out[5]  = omca[1] * axis[1] + cosv;
    out[6]  = omca[1] * axis[2] - sa[0];
    out[7]  = 0.0f;

    out[8]  = omca[2] * axis[0] - sa[1];
    out[9]  = omca[2] * axis[1] + sa[0];
    out[10] = omca[2] * axis[2] + cosv;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_frustum(m4 out, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    out[0]  = 2.0f * n / (r - l);
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = 2.0f * n / (t - b);
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = (r + l) / (r - l);
    out[9]  = (t + b) / (t - b);
    out[10] = -(f + n) / (f - n);
    out[11] = -1.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = -2.0f * (f * n) / (f - n);
    out[15] = 0.0f;
}

inline void
m4_ortho(m4 out, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    out[0]  = 2.0f / (r - l);
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = 2.0f / (t - b);
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = -2.0f / (f - n);
    out[11] = 0.f;

    out[12] = -(r + l) / (r - l);
    out[13] = -(t + b) / (t - b);
    out[14] = -(f + n) / (f - n);
    out[15] = 1.0f;
}

inline void
m4_perspective(m4 out, f32 y_fov, f32 aspect, f32 n, f32 f)
{
    const f32 a = 1.0f / tanf(y_fov / 2.0f);

    out[0]  = a / aspect;
    out[1]  = 0.0f;
    out[2]  = 0.0f;
    out[3]  = 0.0f;

    out[4]  = 0.0f;
    out[5]  = a;
    out[6]  = 0.0f;
    out[7]  = 0.0f;

    out[8]  = 0.0f;
    out[9]  = 0.0f;
    out[10] = -((f + n) / (f - n));
    out[11] = -1.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = -((2.f * f * n) / (f - n));
    out[15] = 0.0f;
}

inline void
m4_look_at(m4 out, const v3 eye, const v3 center, const v3 up)
{
    v3 f = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    v3 s = {0};
    v3 t = {0};

    v3_norm(f, f);

    v3_cross(s, f, up);
    v3_norm(s, s);
    v3_cross(t, s, f);

	out[0]  =  s[0];
	out[1]  =  t[0];
	out[2]  = -f[0];
	out[3]  =  0.0f;

	out[4]  =  s[1];
	out[5]  =  t[1];
	out[6]  = -f[1];
	out[7]  =  0.0f;

	out[8]  =  s[2];
	out[9]  =  t[2];
	out[10] = -f[2];
	out[11] =  0.0f;

    out[12] = -(out[0] * eye[0] + out[4] * eye[1] + out[8]  * eye[2]);
    out[13] = -(out[1] * eye[0] + out[5] * eye[1] + out[9]  * eye[2]);
    out[14] = -(out[2] * eye[0] + out[6] * eye[1] + out[10] * eye[2]);
    out[15] = -(out[3] * eye[0] + out[7] * eye[1] + out[11] * eye[2] - 1.0f);
}

inline void
m4_from_quat(m4 out, const quat_t q)
{
    f32 a = q[3];
	f32 b = q[0];
	f32 c = q[1];
	f32 d = q[2];

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    out[0]  = a2 + b2 - c2 - d2;
    out[1]  = 2.0f * (b * c + a * d);
    out[2]  = 2.0f * (b * d - a * c);
    out[3]  = 0.f;

    out[4]  = 2.0f * (b * c - a * d);
    out[5]  = a2 - b2 + c2 - d2;
    out[6]  = 2.0f * (c * d + a * b);
    out[7]  = 0.f,

    out[8]  = 2.0f * (b * d + a * c);
    out[9]  = 2.0f * (c * d - a * b);
    out[10] = a2 - b2 - c2 + d2;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

inline void
m4_invert(m4 out, const m4 M)
{
	f32 s[6], c[6];

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
	f32 idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    out[0]  = ( M[5]  * c[5] - M[6]  * c[4] + M[7]  * c[3]) * idet;
    out[1]  = (-M[1]  * c[5] + M[2]  * c[4] - M[3]  * c[3]) * idet;
    out[2]  = ( M[13] * s[5] - M[14] * s[4] + M[15] * s[3]) * idet;
    out[3]  = (-M[9]  * s[5] + M[10] * s[4] - M[11] * s[3]) * idet;

    out[4]  = (-M[4]  * c[5] + M[6]  * c[2] - M[7]  * c[1]) * idet;
    out[5]  = ( M[0]  * c[5] - M[2]  * c[2] + M[3]  * c[1]) * idet;
    out[6]  = (-M[12] * s[5] + M[14] * s[2] - M[15] * s[1]) * idet;
    out[7]  = ( M[8]  * s[5] - M[10] * s[2] + M[11] * s[1]) * idet;

    out[8]  = ( M[4]  * c[4] - M[5]  * c[2] + M[7]  * c[0]) * idet;
    out[9]  = (-M[0]  * c[4] + M[1]  * c[2] - M[3]  * c[0]) * idet;
    out[10] = ( M[12] * s[4] - M[13] * s[2] + M[15] * s[0]) * idet;
    out[11] = (-M[8]  * s[4] + M[9]  * s[2] - M[11] * s[0]) * idet;

    out[12] = (-M[4]  * c[3] + M[5]  * c[1] - M[6]  * c[0]) * idet;
    out[13] = ( M[0]  * c[3] - M[1]  * c[1] + M[2]  * c[0]) * idet;
    out[14] = (-M[12] * s[3] + M[13] * s[1] - M[14] * s[0]) * idet;
    out[15] = ( M[8]  * s[3] - M[9]  * s[1] + M[10] * s[0]) * idet;
}

// quat_t:
inline void
quat_identity(quat_t out)
{
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
}

inline void
quat_make(quat_t out, f32 x, f32 y, f32 z, f32 angle)
{
    f32 inv_len = rsqrt((x * x) + (y * y) + (z * z));
    f32 s       = inv_len * sin(angle / 2.0f);

    out[0] = x * s;
    out[1] = y * s;
    out[2] = z * s;
    out[3] = cosf(angle / 2.0f);
}

inline void
quat_conj(quat_t out, const quat_t q)
{
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] =  q[3];
}

inline void
quat_mul(quat_t out, const quat_t a, const quat_t b)
{
    out[0] = a[1] * b[2] - a[2] * b[1] + a[3] * b[0] + b[3] * a[0];
    out[1] = a[2] * b[0] - a[0] * b[2] + a[3] * b[1] + b[3] * a[1];
    out[2] = a[0] * b[1] - a[1] * b[0] + a[3] * b[2] + b[3] * a[2];
    out[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

inline void
quat_rotate(quat_t out, const v3 axis, f32 angle)
{
    f32 s   = sinf(0.5f * angle);
    v3  v   = { s * axis[0], s * axis[1], s * axis[2] };

    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];
    out[3] = cosf(0.5f * angle);
}

// --------------------- v2i ------------------------- // 

inline b32
v2i_equal(const v2i a, const v2i b)
{
    return a[0] == b[0] && a[1] == b[1];
}

inline i32
v2i_dist_sq(const v2i a, const v2i b)
{
    i32 dx = a[0] - b[0];
    i32 dy = a[1] - b[1];

    return dx * dx + dy * dy;
}

inline i32
v2i_manhattan(const v2i a, const v2i b)
{
    i32 dx = abs(a[0] - b[0]);
    i32 dy = abs(a[1] - b[1]);

    return dx + dy;
}

inline void
v2i_clamp(v2i out, const v2i a, i32 min, i32 max)
{
    out[0] = clamp(a[0], min, max);
    out[1] = clamp(a[1], min, max);
}

inline void
v2i_min(v2i out, const v2i a, const v2i b)
{
    out[0] = (a[0] < b[0]? a[0] : b[0]);
    out[1] = (a[1] < b[1]? a[1] : b[1]);
}

inline void
v2i_max(v2i out, const v2i a, const v2i b)
{
    out[0] = (a[0] > b[0]? a[0] : b[0]);
    out[1] = (a[1] > b[1]? a[1] : b[1]);
}

// --------------------- v3i ------------------------- // 

inline i32
v3i_dist_sq(const v2i a, const v2i b)
{
    i32 dx = a[0] - b[0];
    i32 dy = a[1] - b[1];
    i32 dz = a[2] - b[2];

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
    f32 dx  = b.pos[0] - a.pos[0];
    f32 dy  = b.pos[1] - a.pos[1];
    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

inline void
circle_get_intersect_vector(v2 out, circle_t a, circle_t b)
{
    v2 delta =
    {
        a.pos[0] - b.pos[0],
        a.pos[1] - b.pos[1]
    };

    f32 depth = v2_len(delta) - (a.rad + b.rad);
    
    out[0]  = depth * delta[0];
    out[1]  = depth * delta[1];
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
    f32 dx  = b.pos[0] - a.pos[0];
    f32 dy  = b.pos[1] - a.pos[1];
    f32 dz  = b.pos[2] - a.pos[2];

    f32 rt  = a.rad + b.rad;

    return dx * dx + dy * dy < rt * rt;
};

inline void
sphere_get_intersect_vector(v3 out, sphere_t a, sphere_t b)
{
    v3 delta =
    {
        b.pos[0] - a.pos[0],
        b.pos[1] - a.pos[1],
        b.pos[2] - a.pos[2],
    };

    f32 depth = v3_len(delta) - (a.rad + b.rad);
    
    out[0] = depth * delta[0];
    out[1] = depth * delta[1];
    out[2] = depth * delta[2];
}

// ---------------------------------------- RECTANGLE ------------------------------------- //

typedef struct rect_t rect_t;
struct rect_t
{
    v2  min;
    v2  max;
};

inline b32
rect_contains(rect_t rect, const v2 pos)
{
    if (pos[0] < rect.min[0] || pos[0] > rect.max[0]) return false;
    if (pos[1] < rect.min[1] || pos[1] > rect.max[1]) return false;

    return true;
}

inline b32
rect_intersect(rect_t a, rect_t b)
{
    if (a.min[0] > b.max[0] || a.max[0] < b.min[0]) return false;
    if (a.min[1] > b.max[1] || a.max[1] < b.min[1]) return false;

    return true;
}

inline rect_t
rect_get_overlap(rect_t a, rect_t b)
{
    rect_t rect;

    v2_max(rect.min, a.min, b.min);
    v2_min(rect.max, a.max, b.max);

    return rect;
}

inline void
rect_get_intersect_vector(v2 out, rect_t a, rect_t b)
{
    rect_t    o   = rect_get_overlap(a, b);
    f32     dx  = 0.5f * (a.min[0] + a.max[0]) - 0.5f * (b.min[0] + b.max[0]);
    f32     dy  = 0.5f * (a.min[1] + a.max[1]) - 0.5f * (b.min[1] + b.max[1]);

    out[0] = sign(dx) * (o.max[0] - o.min[0]);
    out[1] = sign(dy) * (o.max[1] - o.min[1]);
}

inline rect_t
rect_move(rect_t rect, const v2 offset)
{
    rect.min[0] += offset[0];
    rect.min[1] += offset[1];

    rect.max[0] += offset[0];
    rect.max[1] += offset[1];

    return rect;
}

// ---------------------------------------- RECTANGLE - INT ------------------------------------- //

typedef struct rect_int_t rect_int_t;
struct rect_int_t
{
    v2i  min;
    v2i  max;
};

inline b32
recti_contains(rect_int_t rect, const v2 pos)
{
    if (pos[0] < rect.min[0] || pos[0] > rect.max[0]) return false;
    if (pos[1] < rect.min[1] || pos[1] > rect.max[1]) return false;

    return true;
}

inline b32
recti_intersect(rect_int_t a, rect_int_t b)
{
    if (a.min[0] > b.max[0] || a.max[0] < b.min[0]) return false;
    if (a.min[1] > b.max[1] || a.max[1] < b.min[1]) return false;

    return true;
}

inline rect_int_t
recti_get_overlap(rect_int_t a, rect_int_t b)
{
    rect_int_t rect;

    v2i_max(rect.min, a.min, b.min);
    v2i_min(rect.max, a.max, b.max);

    return rect;
}

inline void
recti_get_intersect_vector(v2 out, rect_int_t a, rect_int_t b)
{
    rect_int_t o = recti_get_overlap(a, b);

    f32 dx  = 0.5f * (a.min[0] + a.max[0]) - 0.5f * (b.min[0] + b.max[0]);
    f32 dy  = 0.5f * (a.min[1] + a.max[1]) - 0.5f * (b.min[1] + b.max[1]);

    out[0] = sign(dx) * (o.max[0] - o.min[0]);
    out[1] = sign(dy) * (o.max[1] - o.min[1]);
}

inline rect_int_t
recti_move(rect_int_t rect, const v2 offset)
{
    rect.min[0] += offset[0];
    rect.min[1] += offset[1];

    rect.max[0] += offset[0];
    rect.max[1] += offset[1];

    return rect;
}

// ----------------------------------------- BOX --------------------------------------- //

typedef struct box_t box_t;
struct box_t
{
    v3  min;
    v3  max;
};

inline b32
box_contains(box_t rect, const v3 pos)
{
    if (pos[0] < rect.min[0] || pos[0] > rect.max[0]) return false;
    if (pos[1] < rect.min[1] || pos[1] > rect.max[1]) return false;
    if (pos[2] < rect.min[2] || pos[2] > rect.max[2]) return false;

    return true;
}

inline b32
box_intersect(box_t a, box_t b)
{
    if (a.min[0] > b.max[0] || a.max[0] < b.min[0]) return false;
    if (a.min[1] > b.max[1] || a.max[1] < b.min[1]) return false;
    if (a.min[2] > b.max[2] || a.max[2] < b.min[2]) return false;

    return true;
}

inline box_t
box_get_overlap(box_t a, box_t b)
{
    box_t box;

    v2_max(box.min, a.min, b.min);
    v2_min(box.max, a.max, b.max);

    return box;
}

inline void
box_get_intersect_vector(v3 out, box_t a, box_t b)
{
    box_t o   = box_get_overlap(a, b);

    f32 dx  = 0.5f * (a.min[0] + a.max[0]) - 0.5f * (b.min[0] + b.max[0]);
    f32 dy  = 0.5f * (a.min[1] + a.max[1]) - 0.5f * (b.min[1] + b.max[1]);
    f32 dz  = 0.5f * (a.min[2] + a.max[2]) - 0.5f * (b.min[2] + b.max[2]);

    out[0] = sign(dx) * (o.max[0] - o.min[0]);
    out[1] = sign(dy) * (o.max[1] - o.min[1]);
    out[2] = sign(dz) * (o.max[2] - o.min[2]);
}

inline box_t
box_move(box_t box, const v3 offset)
{
    box.min[0] += offset[0];
    box.min[1] += offset[1];
    box.min[2] += offset[2];

    box.max[0] += offset[0];
    box.max[1] += offset[1];
    box.max[2] += offset[2];

    return box;
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
frustum_create(const m4 matrix, bool normalize)
{
    frustum_t frustum = {0};

    // left clipping plane
    frustum.plane[0].a = matrix[3]  + matrix[0];
    frustum.plane[0].b = matrix[7]  + matrix[4];
    frustum.plane[0].c = matrix[11] + matrix[8];
    frustum.plane[0].d = matrix[15] + matrix[12];

    // right clipping plane
    frustum.plane[1].a = matrix[3]  - matrix[0];
    frustum.plane[1].b = matrix[7]  - matrix[4];
    frustum.plane[1].c = matrix[11] - matrix[8];
    frustum.plane[1].d = matrix[15] - matrix[12];

    // top clipping plane
    frustum.plane[2].a = matrix[3]  - matrix[1];
    frustum.plane[2].b = matrix[7]  - matrix[5];
    frustum.plane[2].c = matrix[11] - matrix[9];
    frustum.plane[2].d = matrix[15] - matrix[13];

    // bottom clipping plane
    frustum.plane[3].a = matrix[3]  + matrix[1];
    frustum.plane[3].b = matrix[7]  + matrix[5];
    frustum.plane[3].c = matrix[11] + matrix[9];
    frustum.plane[3].d = matrix[15] + matrix[13];

    // near clipping plane
    frustum.plane[4].a = matrix[3]  + matrix[2];
    frustum.plane[4].b = matrix[7]  + matrix[6];
    frustum.plane[4].c = matrix[11] + matrix[10];
    frustum.plane[4].d = matrix[15] + matrix[14];

    // far clipping plane
    frustum.plane[5].a = matrix[3]  - matrix[2];
    frustum.plane[5].b = matrix[7]  - matrix[6];
    frustum.plane[5].c = matrix[11] - matrix[10];
    frustum.plane[5].d = matrix[15] - matrix[14];

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
frustum_contains(frustum_t frustum, const v3 pos)
{
    for(i32 i = 0; i < 6; i++)
    {
		if (frustum.plane[i].a * pos[0] + frustum.plane[i].b * pos[1] + frustum.plane[i].c * pos[2] + frustum.plane[i].d <= 0)
			return false;
	}

	return true;
}

inline b32
frustum_intersect_sphere(frustum_t frustum, sphere_t sphere)
{
    for(i32 i = 0; i < 6; i++)
    {
		if(frustum.plane[i].a * sphere.pos[0] + frustum.plane[i].b * sphere.pos[1] + frustum.plane[i].c * sphere.pos[2] + frustum.plane[i].d <= -sphere.rad) {
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
		if (frustum.plane[i].a * box.min[0] + frustum.plane[i].b * box.min[1] + frustum.plane[i].c * box.min[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max[0] + frustum.plane[i].b * box.min[1] + frustum.plane[i].c * box.min[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min[0] + frustum.plane[i].b * box.max[1] + frustum.plane[i].c * box.min[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max[0] + frustum.plane[i].b * box.max[1] + frustum.plane[i].c * box.min[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min[0] + frustum.plane[i].b * box.min[1] + frustum.plane[i].c * box.max[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max[0] + frustum.plane[i].b * box.min[1] + frustum.plane[i].c * box.max[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.min[0] + frustum.plane[i].b * box.max[1] + frustum.plane[i].c * box.max[2] + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * box.max[0] + frustum.plane[i].b * box.max[1] + frustum.plane[i].c * box.max[2] + frustum.plane[i].d > 0) continue;

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
pack_color_v4(const f32 color[4])
{
    return pack_color_f32(color[0], color[1], color[2], color[3]);
}

inline u32
pack_color_v3(const f32 color[3])
{
    return pack_color_f32(color[0], color[1], color[2], 1.0);
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
queue_push(priority_queue_t* queue, const v2i e, f32 weight)
{
    queue_node_t node = { weight, e[0], e[1] };

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
queue_pop(v2i out, priority_queue_t* queue)
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

    out[0] = data.e[0];
    out[1] = data.e[1];

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

    f32     LS[2];
    f32     RS[2];

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

        f32     pos[2];
        f32     delta[2];
        f32     scroll[2];

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

    platform.mouse.scroll[0] = xoffset;
    platform.mouse.scroll[1] = yoffset;
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

        platform.mouse.pos[0] = x;
        platform.mouse.pos[1] = y;
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

        platform.mouse.delta[0]  = x - platform.mouse.pos[0];
        platform.mouse.delta[1]  = y - platform.mouse.pos[1];

        platform.mouse.pos[0]    = x;
        platform.mouse.pos[1]    = y;

        platform.mouse.scroll[0] = 0;
        platform.mouse.scroll[1] = 0;

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

                platform.gamepad[i].LS[0] = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].LS[1] = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].RS[0] = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                platform.gamepad[i].RS[1] = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

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
    m4 TS, T, S;

    m4_translate(T, x, y, z);
    m4_scale(S, sx, sy, 1);
    m4_mul(TS, T, S);

    glPushMatrix();

    glMultMatrixf(TS);
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

