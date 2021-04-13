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
#include <assert.h>

#ifdef ATS_MODERN_OPENGL
#include "dep/glad/glad.h"
#endif

#define internal static
#define local_persist static
#define global_variable static

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

union v2
{
    struct { f32 x, y; };

    f32 array[2];
};

union v3
{
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };

    struct { v2 xy; f32 _z; };

    f32 array[3];
};

union v4
{
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };

    struct { v3 xyz; f32 _w; };
    struct { v3 rgb; f32 _a; };

    f32 array[4];
};

union Quat
{
    struct { f32 x, y, z, w; };

    f32 array[4];
};

union m2
{
    struct { v2 x, y; };

    f32 array[4];
};

union m3
{
    struct { v3 x, y, z; };

    f32 array[9];
};

union m4
{
    struct { v4 x, y, z, w; };

    f32 array[16];
};

union v2i
{
    struct { i32 x, y; };

    i32 array[2];
};

union v3i
{
    struct { i32 x, y, z; };

    struct { v2i xy; i32 _z; };

    i32 array[3];
};

union v4i
{
    struct { i32 x, y, z, w; };

    i32 array[4];
};

// --------------------------------------------- FUNCTIONS ---------------------------------------- //

inline f32 Sqrt(f32 n)
{
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

inline f32 RSqrt(f32 n)
{
    f32 x2 = n * 0.5F;
    f32 y  = n;
    int i  = *(long*)&y;           // evil f32ing point bit level hacking

    i = 0x5f3759df - (i >> 1);     // what the fuck? 
    y = *(f32*) &i;
    y = y * (1.5f - (x2 * y * y)); // 1st iteration

    return y;
}

template <typename T>
inline T Square(T n)
{
    return n * n;
}

inline f32 Lerp(f32 a, f32 b, f32 t)
{
    return a + t * (b - a);
}

inline f32 UnLerp(f32 a, f32 b, f32 t)
{
    return (t - a) / (b - a);
}

inline f32 AngleDist(f32 a, f32 b)
{
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);
    return fmodf(2.0f * da, max) - da;
}

inline f32 LerpAngle(f32 a, f32 b, f32 t)
{
    return a + AngleDist(a, b) * t;
}

inline f32 Spline(f32 f, f32 a, f32 b, f32 c, f32 d)
{
    f32 i = 1.0f - f;
    return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

template <typename T, typename U>
inline T Min(T a, U b)
{
    return a < b? a : b;
}

template <typename T, typename U>
inline T Max(T a, U b)
{
    return a > b? a : b;
}

template <typename T, typename U, typename V>
inline T Clamp(T n, U min, V max)
{
    if (n < min) return min;
    if (n > max) return max;

    return n;
}

template <typename T, typename U>
inline T ClampMin(T n, U min)
{
    return n < min? min : n;
}

template <typename T, typename U>
inline T ClampMax(T n, U max)
{
    return n > max? max : n;
}

template <typename T>
inline T Sign(T n)
{
    return n < 0? -1 : 1;
}

template <typename T>
inline T SignOrZero(T n)
{
    if (n < 0) return -1;
    if (n > 0) return +1;

    return 0;
}

struct PriorityQueue
{
    struct Node { f32 weight; v2i e; };

    i32     len;
    Node*   array;

    inline bool Empty   ()  const   { return len == 0; }
    inline void Clear   ()          { len = 0; }

    inline void Push(v2i e, f32 weight)
    {
        Node node = { .weight = weight, .e = e };

        int i = len + 1;
        int j = i / 2;

        while (i > 1 && array[j].weight > node.weight) {
            array[i] = array[j];

            i = j;
            j = j / 2;
        }

        array[i] = node;
        len++;
    }

    inline Node Pop()
    {
        Node data = array[1];

        array[1] = array[len];
        len--;

        int i = 1;
        while (i != len + 1) {
            int k = len + 1;
            int j = 2 * i;

            if (j <= len && array[j].weight < array[k].weight)
                k = j;

            if (j + 1 <= len && array[j + 1].weight < array[k].weight)
                k = j + 1;

            array[i] = array[k];
            i = k;
        }
        return data;
    }
};

// ==================================================  MATHS ================================================== //

// ---------------------- v2 ---------------------- //

inline v2 V2(f32 x, f32 y)
{
    return { x, y };
}

inline v2 V2(const f32* u)
{
    return { u[0], u[1] };
}

inline v2 V2(v3 a)
{
    return { a.x, a.y };
}

inline v2 V2(v2i a)
{
    return { (f32)a.x, (f32)a.y };
}

inline v2 operator-(v2 a)
{
    return { -a.x, -a.y };
}

inline v2 operator+(v2 a, v2 b)
{
    return { a.x + b.x, a.y + b.y };
}

inline v2 operator-(v2 a, v2 b)
{
    return { a.x - b.x, a.y - b.y };
}

inline v2 operator*(v2 a, v2 b)
{
    return { a.x * b.x, a.y * b.y };
}

inline v2 operator*(v2 a, f32 s)
{
    return { a.x * s, a.y * s };
}

inline v2 operator*(f32 s, v2 a)
{
    return { a.x * s, a.y * s };
}

inline v2 operator/(v2 a, f32 s)
{
    return { a.x / s, a.y / s };
}

inline v2 operator+=(v2& a, v2 b)
{
    a = a + b;
    return a;
}

inline v2 operator-=(v2& a, v2 b)
{
    a = a - b;
    return a;
}

inline v2 operator*=(v2& a, v2 b)
{
    a = a * b;
    return a;
}

inline v2 operator*=(v2& a, f32 s)
{
    a = a * s;
    return a;
}

inline v2 operator/=(v2& a, f32 s)
{
    a = a / s;
    return a;
}

inline f32 Det(v2 a, v2 b)
{
    return a.x * b.y - a.y * b.x;
}

inline f32 Dot(v2 a, v2 b)
{
    return a.x * b.x + a.y * b.y;
}

inline f32 LenSq(v2 v)
{
    return Dot(v, v);
}

inline f32 Len(v2 v)
{
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v2 a, v2 b)
{
    return LenSq(b - a);
}

inline f32 Dist(v2 a, v2 b)
{
    return Sqrt(DistSq(a, b));
}

inline v2 Proj(v2 a, v2 b)
{
    f32 d = Dot(b, b);
    if (d == 0) return {};

    return (Dot(a, b) / d) * b;
}

inline v2 Norm(v2 v)
{
    return v * RSqrt(Dot(v, v));
}

inline v2 Min(v2 a, v2 b)
{
    return {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y)
    };
}

inline v2 Max(v2 a, v2 b)
{
    return {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y)
    };
}

inline v2 Lerp(v2 a, v2 b, f32 t)
{
    return a + t * (b - a);
}

inline f32 GetAngle(v2 a, v2 b)
{
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;

    return atan2f(det, dot);
}

inline v2 Spline(f32 f, v2 a, v2 b, v2 c, v2 d)
{
    f32 i = 1.0f - f;

    return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

inline b32 CircleIntersect(v2 p0, f32 r0, v2 p1, f32 r1)
{
    f32 dx = p1.x - p0.x;
    f32 dy = p1.y - p0.y;

    f32 r = r0 + r1;

    return (dx * dx + dy * dy) < (r * r);
}

inline b32 SegmentIsIntersectingCircle(v2 start, v2 end, v2 pos, f32 rad)
{
    v2 a = start - pos;
    v2 b = end - pos;

    if (Dot(a, a) > Dot(b, b)) return false;

    v2 seg = end - start;
    v2 cir = pos - start;

    f32 dot_sc = Dot(seg, cir);

    if (dot_sc < 0.0f) return false;

    seg = (dot_sc / Dot(seg, seg)) * seg - cir;

    return Dot(seg, seg) < (rad * rad);
}

inline m2 OuterProduct(v2 a, v2 b)
{
    return m2 {
        a.x * b.x, a.y * b.x,
        a.x * b.y, a.y * b.y,
    };
}

inline v2 Square(v2 a)
{
    return { a.x * a.x, a.y * a.y };
}

// ----------------------------- v3 ----------------------------- //

inline v3 V3(f32 x, f32 y, f32 z)
{
    return { x, y, z };
}

inline v3 V3(const f32* u)
{
    return { u[0], u[1], u[2] };
}

inline v3 V3(v2 a)
{
    return { a.x, a.y, 0.0f };
}

inline v3 V3(v2 a, f32 z)
{
    return { a.x, a.y, z };
}

inline v3 V3(v2i a, i32 z)
{
    return { f32(a.x), f32(a.y), f32(z) };
}

inline v3 V3(f32 x, v2 a)
{
    return { x, a.x, a.y };
}

inline v3 V3(v3i a)
{
    return { f32(a.x), f32(a.y), f32(a.z) };
}

inline v3 operator-(v3 a)
{
    return { -a.x, -a.y, -a.z };
}

inline v3 operator+(v3 a, v3 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline v3 operator-(v3 a, v3 b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline v3 operator*(v3 a, v3 b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

inline v3 operator*(v3 a, f32 s)
{
    return { a.x * s, a.y * s, a.z * s };
}

inline v3 operator*(f32 s, v3 a)
{
    return { a.x * s, a.y * s, a.z * s };
}

inline v3 operator/(v3 a, f32 s)
{
    return { a.x / s, a.y / s, a.z / s };
}

inline v3 operator+=(v3& a, v3 b)
{
    a = a + b;
    return a;
}

inline v3 operator-=(v3& a, v3 b)
{
    a = a - b;
    return a;
}

inline v3 operator*=(v3& a, v3 b)
{
    a = a * b;
    return a;
}

inline v3 operator*=(v3& a, f32 s)
{
    a = a * s;
    return a;
}

inline v3 operator/=(v3& a, f32 s)
{
    a = a / s;
    return a;
}

inline f32 Dot(v3 a, v3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 LenSq(v3 v)
{
    return Dot(v, v);
}

inline f32 Len(v3 v)
{
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v3 a, v3 b)
{
    return LenSq(b - a);
}

inline f32 Dist(v3 a, v3 b)
{
    return Sqrt(DistSq(a, b));
}

inline v3 Proj(v3 a, v3 b)
{
    f32 d = Dot(b, b);
    if (d == 0) return {};

    return (Dot(a, b) / d) * b;
}

inline v3 Norm(v3 v)
{
    return v * RSqrt(Dot(v, v));
}

inline v3 Min(v3 a, v3 b)
{
    return {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y),
        (a.z < b.z? a.z : b.z)
    };
}

inline v3 Max(v3 a, v3 b)
{
    return {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y),
        (a.z > b.z? a.z : b.z)
    };
}

inline v3 Lerp(v3 a, v3 b, f32 t)
{
    return a + t * (b - a);
}

inline v3 Cross(v3 a, v3 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
}

inline v3 Spline(f32 f, v3 a, v3 b, v3 c, v3 d)
{
    f32 i = 1.0f - f;

    return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

inline b32 RaycastTriangle(v3 orig, v3 dir, v3 vert0, v3 vert1, v3 vert2, v3* result)
{
    const f32 EPSILON = 0.0000001;

    v3  edge1   = vert1 - vert0;
    v3  edge2   = vert2 - vert0;
    v3  pvec    = Cross(dir, edge2);
    f32 det     = Dot(edge1, pvec);

    if (det > -EPSILON && det < EPSILON) {
        return false;
    }

    f32 inv_det = 1.0f / det;
    v3  tvec    = orig - vert0;

    result->x   = Dot(tvec, pvec) * inv_det;

    if (result->x < 0.0f || result->x > 1.0f) {
        return false;
    }

    v3 qvec     = Cross(tvec, edge1);
    result->y   = Dot(dir, qvec) * inv_det;

    if (result->y < 0.0f || result->x + result->y > 1.0f) {
        return false;
    }

    result->z   = Dot(edge2, qvec) * inv_det;

    v3 intersection = vert0 + result->x * edge2 + result->y * edge1;

    if (result->z < EPSILON) {
        return false;
    }

    *result = vert0 + result->x * edge2 + result->y * edge1;

    return true;
}

// ----------------------------- v4 ---------------------------- //

inline v4 V4(f32 x, f32 y, f32 z, f32 w)
{
    return { x, y, z, w };
}

inline v4 V4(const f32* u)
{
    return { u[0], u[1], u[2], u[3] };
}

inline v4 V4(v2 a, v2 b)
{
    return V4(a.x, a.y, b.x, b.y);
}

inline v4 V4(v2 a, f32 z, f32 w)
{
    return V4(a.x, a.y, z, w);
}

inline v4 V4(v3 a, f32 w)
{
    return V4(a.x, a.y, a.z, w);
}

inline v4 V4(f32 x, v3 a)
{
    return V4(x, a.x, a.y, a.z);
}

inline v4 V4(u32 color)
{
    v4 result = {};

    result.r = ((color & 0x000000FF) >> 0)  / 256.0f;
    result.g = ((color & 0x0000FF00) >> 8)  / 256.0f;
    result.b = ((color & 0x00FF0000) >> 16) / 256.0f;
    result.a = ((color & 0xFF000000) >> 24) / 256.0f;

    return result;
}

inline v4 operator-(v4 a)
{
    return { -a.x, -a.y, -a.z, -a.w };
}

inline v4 operator+(v4 a, v4 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline v4 operator-(v4 a, v4 b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline v4 operator*(v4 a, v4 b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline v4 operator*(v4 a, f32 s)
{
    return { a.x * s, a.y * s, a.z * s, a.w * s };
}

inline v4 operator*(f32 s, v4 a)
{
    return { a.x * s, a.y * s, a.z * s, a.w * s };
}

inline v4 operator/(v4 a, f32 s)
{
    return { a.x / s, a.y * s, a.z / s, a.w / s };
}

inline v4 operator+=(v4& a, v4 b)
{
    a = a + b;
    return a;
}

inline v4 operator-=(v4& a, v4 b)
{
    a = a - b;
    return a;
}

inline v4 operator*=(v4& a, v4 b)
{
    a = a * b;
    return a;
}

inline v4 operator*=(v4& a, f32 s)
{
    a = a * s;
    return a;
}

inline v4 operator/=(v4& a, f32 s)
{
    a = a / s;
    return a;
}

inline f32 Dot(v4 a, v4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 LenSq(v4 v)
{
    return Dot(v, v);
}

inline f32 Len(v4 v)
{
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v4 a, v4 b)
{
    return LenSq(b - a);
}

inline f32 Dist(v4 a, v4 b)
{
    return Sqrt(DistSq(a, b));
}

inline v4 Norm(v4 v)
{
    return v * RSqrt(Dot(v, v));
}

inline v4 Min(v4 a, v4 b)
{
    return {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y),
        (a.z < b.z? a.z : b.z),
        (a.w < b.w? a.w : b.w)
    };
}

inline v4 Max(v4 a, v4 b)
{
    return {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y),
        (a.z > b.z? a.z : b.z),
        (a.w > b.w? a.w : b.w)
    };
}

inline v4 Lerp(v4 a, v4 b, f32 t)
{
    return a + t * (b - a);
}

inline v4 Cross(v4 a, v4 b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        1.0f
    };
}

inline v4 Spline(f32 f, v4 a, v4 b, v4 c, v4 d)
{
    f32 i = 1.0f - f;

    return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

// m2:
inline m2 operator+(m2 a, m2 b)
{
    return { a.x + b.x, a.y + b.y };
}

inline m2 operator-(m2 a, m2 b)
{
    return { a.x - b.x, a.y - b.y };
}

inline m2 operator*(m2 a, f32 s)
{
    return { a.x * s, a.y * s };
}

inline m2 operator*(f32 s, m2 a)
{
    return { a.x * s, a.y * s };
}

inline m2 operator*(m2 a, m2 b)
{
    return {
        a.array[0] * b.array[0] + a.array[2] * b.array[1],
        a.array[1] * b.array[0] + a.array[3] * b.array[1],
        a.array[0] * b.array[2] + a.array[2] * b.array[3],
        a.array[1] * b.array[2] + a.array[3] * b.array[3]
    };
}

inline v2 operator*(m2 R, v2 v)
{
    return {
        R.array[0] * v.x + R.array[2] * v.y,
        R.array[1] * v.x + R.array[3] * v.y
    };
}

inline m2 m2_Identity(void)
{
    return { 1.0f, 0.0f, 0.0f, 1.0f };
}

inline m2 m2_Rotate(f32 co, f32 si)
{
    return { co, si, -si, co };
}

inline m2 m2_Rotate(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return { c, s, -s, c };
}

inline m2 m2_Scale(f32 sx, f32 sy)
{
    return { sx, 0.0f, 0.0f, sy };
}

inline f32 Det(m2 m)
{
    return m.x.x * m.y.y - m.x.y * m.y.x;
}

inline m2 Transpose(m2 m)
{
    return {
        m.x.x, m.y.x,
        m.x.y, m.y.y
    };
}

inline m2 Inverse(m2 m)
{
    f32 det = Det(m);

    return (1.0 / det) * m2 {
        m.y.y, -m.y.x,
        -m.x.y,  m.x.x
    };
}

inline void PolarDecomp(m2 m, m2* R, m2* S)
{
    f32 x = m.x.x + m.y.y;
    f32 y = m.y.x - m.x.y;
    f32 scale = RSqrt(x * x + y * y);
    f32 c = x * scale, s = y * scale;

    R->x.x = c;
    R->x.y = -s;
    R->y.x = s;
    R->y.y = c;

    *S = Transpose(*R) * m;
}

// m3:
inline m3 operator*(m3 a, m3 b)
{
    return {
        a.array[0] * b.array[0] + a.array[3] * b.array[1]  + a.array[6] * b.array[2],
        a.array[1] * b.array[0] + a.array[4] * b.array[1]  + a.array[7] * b.array[2],
        a.array[2] * b.array[0] + a.array[5] * b.array[1]  + a.array[8] * b.array[2],

        a.array[0] * b.array[3] + a.array[3] * b.array[4]  + a.array[6] * b.array[5],
        a.array[1] * b.array[3] + a.array[4] * b.array[4]  + a.array[7] * b.array[5],
        a.array[2] * b.array[3] + a.array[5] * b.array[4]  + a.array[8] * b.array[5],

        a.array[0] * b.array[6] + a.array[3] * b.array[7]  + a.array[6] * b.array[8],
        a.array[1] * b.array[6] + a.array[4] * b.array[7]  + a.array[7] * b.array[8],
        a.array[2] * b.array[6] + a.array[5] * b.array[7]  + a.array[8] * b.array[8]
    };
}

inline v3 operator*(m3 M, v3 v)
{
    return {
        M.array[0] * v.array[0] + M.array[3] * v.array[1] + M.array[6] * v.array[2],
        M.array[1] * v.array[0] + M.array[4] * v.array[1] + M.array[7] * v.array[2],
        M.array[2] * v.array[0] + M.array[5] * v.array[1] + M.array[8] * v.array[2]
    };
}

inline m3 m3_Identity(void)
{
    return {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
}

inline m3 m3_Rotate(v3 axis, f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    f32 k = 1.0f - c;

    v3 sa   = { s * axis.x, s * axis.y, s * axis.z };
    v3 omca = { k * axis.x, k * axis.y, k * axis.z };

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

inline m3 m3_Euler(f32 yaw, f32 pitch, f32 roll)
{
    f32 cy = cosf(yaw);
    f32 sy = sinf(yaw);
    f32 cp = cosf(pitch);
    f32 sp = sinf(pitch);
    f32 cr = cosf(roll);
    f32 sr = sinf(roll);

    return {
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

inline m3 m3_FromQuat(Quat quat)
{
    f32 a = quat.w;
    f32 b = quat.x;
    f32 c = quat.y;
    f32 d = quat.z;

    f32 a2 = a * a;
    f32 b2 = b * b;
    f32 c2 = c * c;
    f32 d2 = d * d;

    return {
        a2 + b2 - c2 - d2,
        2.0f * (b * c + a * d),
        2.0f * (b * d - a * c),

        2.0f * (b * c - a * d),
        a2 - b2 + c2 - d2,
        2.0f * (c * d + a * b),

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2
    };
}

// m4:

inline m4 operator*(m4 a, m4 b)
{
    return {
        a.array[0] * b.array[0]  + a.array[4] * b.array[1]  + a.array[8]  * b.array[2]  + a.array[12] * b.array[3],
        a.array[1] * b.array[0]  + a.array[5] * b.array[1]  + a.array[9]  * b.array[2]  + a.array[13] * b.array[3],
        a.array[2] * b.array[0]  + a.array[6] * b.array[1]  + a.array[10] * b.array[2]  + a.array[14] * b.array[3],
        a.array[3] * b.array[0]  + a.array[7] * b.array[1]  + a.array[11] * b.array[2]  + a.array[15] * b.array[3],

        a.array[0] * b.array[4]  + a.array[4] * b.array[5]  + a.array[8]  * b.array[6]  + a.array[12] * b.array[7],
        a.array[1] * b.array[4]  + a.array[5] * b.array[5]  + a.array[9]  * b.array[6]  + a.array[13] * b.array[7],
        a.array[2] * b.array[4]  + a.array[6] * b.array[5]  + a.array[10] * b.array[6]  + a.array[14] * b.array[7],
        a.array[3] * b.array[4]  + a.array[7] * b.array[5]  + a.array[11] * b.array[6]  + a.array[15] * b.array[7],

        a.array[0] * b.array[8]  + a.array[4] * b.array[9]  + a.array[8]  * b.array[10] + a.array[12] * b.array[11],
        a.array[1] * b.array[8]  + a.array[5] * b.array[9]  + a.array[9]  * b.array[10] + a.array[13] * b.array[11],
        a.array[2] * b.array[8]  + a.array[6] * b.array[9]  + a.array[10] * b.array[10] + a.array[14] * b.array[11],
        a.array[3] * b.array[8]  + a.array[7] * b.array[9]  + a.array[11] * b.array[10] + a.array[15] * b.array[11],

        a.array[0] * b.array[12] + a.array[4] * b.array[13] + a.array[8]  * b.array[14] + a.array[12] * b.array[15],
        a.array[1] * b.array[12] + a.array[5] * b.array[13] + a.array[9]  * b.array[14] + a.array[13] * b.array[15],
        a.array[2] * b.array[12] + a.array[6] * b.array[13] + a.array[10] * b.array[14] + a.array[14] * b.array[15],
        a.array[3] * b.array[12] + a.array[7] * b.array[13] + a.array[11] * b.array[14] + a.array[15] * b.array[15]
    };
}

inline v4 operator*(m4 M, v4 v)
{
    return {
        M.array[0] * v.array[0] + M.array[4] * v.array[1] + M.array[8]  * v.array[2] + M.array[12] * v.array[3],
        M.array[1] * v.array[0] + M.array[5] * v.array[1] + M.array[9]  * v.array[2] + M.array[13] * v.array[3],
        M.array[2] * v.array[0] + M.array[6] * v.array[1] + M.array[10] * v.array[2] + M.array[14] * v.array[3],
        M.array[3] * v.array[0] + M.array[7] * v.array[1] + M.array[11] * v.array[2] + M.array[15] * v.array[3]
    };
}

inline m4 m4_Identity(void)
{
    return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
}

inline m4 Transpose(m4 N)
{
    return {
        N.array[0], N.array[4], N.array[8],  N.array[12],
        N.array[1], N.array[5], N.array[9],  N.array[13],
        N.array[2], N.array[6], N.array[10], N.array[14],
        N.array[3], N.array[7], N.array[11], N.array[15]
    };
}

inline m4 m4_Translate(f32 x, f32 y, f32 z)
{
    return {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        x,      y,      z,      1.0f
    };
}

inline m4 m4_Translate(v3 pos)
{
    return m4_Translate(pos.x, pos.y, pos.z);
}

inline m4 m4_Scale(f32 x, f32 y, f32 z)
{
    return {
        x,      0.0f,   0.0f,   0.0f,
        0.0f,   y,      0.0f,   0.0f,
        0.0f,   0.0f,   z,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

inline m4 m4_Scale(v3 scale)
{
    return m4_Scale(scale.x, scale.y, scale.z);
}

inline m4 m4_Scale(f32 scale)
{
    return m4_Scale(scale, scale, scale);
}

inline m4 m4_RotateX(f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    return {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,      c,      s,   0.0f,
        0.0f,     -s,      c,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

inline m4 m4_RotateY(f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    return {
        c,   0.0f,      s,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        -s,   0.0f,      c,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

inline m4 m4_RotateZ(f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);

    return {
        c,      s,   0.0f,   0.0f,
        -s,      c,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

inline m4 m4_Rotate(v3 axis, f32 angle)
{
    f32 sa[3];
    f32 omca[3];

    f32 cosv = cosf(angle);
    f32 sinv = sinf(angle);
    f32 inv_cosv = 1.0f - cosv;

    sa[0] = axis.x * sinv;
    sa[1] = axis.y * sinv;
    sa[2] = axis.z * sinv;

    omca[0] = axis.x * inv_cosv;
    omca[1] = axis.y * inv_cosv;
    omca[2] = axis.z * inv_cosv;

    return {
        omca[0] * axis.x + cosv,   omca[0] * axis.y - sa[0],  omca[0] * axis.z + sa[1],  0.0f,
        omca[1] * axis.x + sa[2],  omca[1] * axis.y + cosv,   omca[1] * axis.z - sa[0],  0.0f,
        omca[2] * axis.x - sa[1],  omca[2] * axis.y + sa[0],  omca[2] * axis.z + cosv,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

inline m4 Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    return {
        2.0f / (r - l),
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        2.0f / (t - b),
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -2.0f / (f - n),
        0.f,

        -(r + l) / (r - l),
        -(t + b) / (t - b),
        -(f + n) / (f - n),
        1.0f
    };
}

inline m4 Perspective(f32 y_fov, f32 aspect, f32 n, f32 f)
{
    const f32 a = 1.0f / tanf(y_fov / 2.0f);

    return {
        a / aspect,
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        a,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -((f + n) / (f - n)),
        -1.0f,

        0.0f,
        0.0f,
        -((2.f * f * n) / (f - n)),
        0.0f
    };
}

inline m4 LookAt(v3 eye, v3 center, v3 up)
{
    v3 f = Norm(center - eye);
    v3 s = Norm(Cross(f, up));
    v3 t = Cross(s, f);

    m4 M;

    M.array[0]  =  s.x;
    M.array[1]  =  t.x;
    M.array[2]  = -f.x;
    M.array[3]  =   0.0f;

    M.array[4]  =  s.y;
    M.array[5]  =  t.y;
    M.array[6]  = -f.y;
    M.array[7]  =   0.0f;

    M.array[8]  =  s.z;
    M.array[9]  =  t.z;
    M.array[10] = -f.z;
    M.array[11] =   0.0f;

    M.array[12] = -(M.array[0] * eye.x + M.array[4] * eye.y + M.array[8]  * eye.z);
    M.array[13] = -(M.array[1] * eye.x + M.array[5] * eye.y + M.array[9]  * eye.z);
    M.array[14] = -(M.array[2] * eye.x + M.array[6] * eye.y + M.array[10] * eye.z);
    M.array[15] = -(M.array[3] * eye.x + M.array[7] * eye.y + M.array[11] * eye.z - 1.0f);

    return M;
}

inline m4 m4_FromQuat(Quat q)
{
    f32 a = q.w;
    f32 b = q.x;
    f32 c = q.y;
    f32 d = q.z;

    f32 a2 = a * a;
    f32 b2 = b * b;
    f32 c2 = c * c;
    f32 d2 = d * d;

    return {
        a2 + b2 - c2 - d2,
        2.0f * (b * c + a * d),
        2.0f * (b * d - a * c),
        0.f,

        2.0f * (b * c - a * d),
        a2 - b2 + c2 - d2,
        2.0f * (c * d + a * b),
        0.f,

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

inline m4 m4_Dir(v3 dir, v3 up)
{
    v3 left = Norm(Cross(up, dir));
    v3 cup  = Norm(Cross(dir, left));

    return {
        cup.x, left.x, dir.x, 0,
        cup.y, left.y, dir.y, 0,
        cup.z, left.z, dir.z, 0,
        0, 0, 0, 1,
    };
}

inline m4 Invert(m4 M)
{
    f32 s[6], c[6];

    s[0] = M.array[0] * M.array[5] - M.array[4] * M.array[1];
    s[1] = M.array[0] * M.array[6] - M.array[4] * M.array[2];
    s[2] = M.array[0] * M.array[7] - M.array[4] * M.array[3];
    s[3] = M.array[1] * M.array[6] - M.array[5] * M.array[2];
    s[4] = M.array[1] * M.array[7] - M.array[5] * M.array[3];
    s[5] = M.array[2] * M.array[7] - M.array[6] * M.array[3];

    c[0] = M.array[8]  * M.array[13] - M.array[12] * M.array[9];
    c[1] = M.array[8]  * M.array[14] - M.array[12] * M.array[10];
    c[2] = M.array[8]  * M.array[15] - M.array[12] * M.array[11];
    c[3] = M.array[9]  * M.array[14] - M.array[13] * M.array[10];
    c[4] = M.array[9]  * M.array[15] - M.array[13] * M.array[11];
    c[5] = M.array[10] * M.array[15] - M.array[14] * M.array[11];

    // assumes it is invertible
    f32 idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    return {
        ( M.array[5]  * c[5] - M.array[6]  * c[4] + M.array[7]  * c[3]) * idet,
        (-M.array[1]  * c[5] + M.array[2]  * c[4] - M.array[3]  * c[3]) * idet,
        ( M.array[13] * s[5] - M.array[14] * s[4] + M.array[15] * s[3]) * idet,
        (-M.array[9]  * s[5] + M.array[10] * s[4] - M.array[11] * s[3]) * idet,

        (-M.array[4]  * c[5] + M.array[6]  * c[2] - M.array[7]  * c[1]) * idet,
        ( M.array[0]  * c[5] - M.array[2]  * c[2] + M.array[3]  * c[1]) * idet,
        (-M.array[12] * s[5] + M.array[14] * s[2] - M.array[15] * s[1]) * idet,
        ( M.array[8]  * s[5] - M.array[10] * s[2] + M.array[11] * s[1]) * idet,

        ( M.array[4]  * c[4] - M.array[5]  * c[2] + M.array[7]  * c[0]) * idet,
        (-M.array[0]  * c[4] + M.array[1]  * c[2] - M.array[3]  * c[0]) * idet,
        ( M.array[12] * s[4] - M.array[13] * s[2] + M.array[15] * s[0]) * idet,
        (-M.array[8]  * s[4] + M.array[9]  * s[2] - M.array[11] * s[0]) * idet,

        (-M.array[4]  * c[3] + M.array[5]  * c[1] - M.array[6]  * c[0]) * idet,
        ( M.array[0]  * c[3] - M.array[1]  * c[1] + M.array[2]  * c[0]) * idet,
        (-M.array[12] * s[3] + M.array[13] * s[1] - M.array[14] * s[0]) * idet,
        ( M.array[8]  * s[3] - M.array[9]  * s[1] + M.array[10] * s[0]) * idet,
    };
}

// Quat:
inline Quat IdentityQuat(void)
{
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}

inline Quat MakeQuat(f32 x, f32 y, f32 z, f32 angle)
{
    f32 inv_len = RSqrt((x * x) + (y * y) + (z * z));
    f32 s = inv_len * sin(angle / 2.0f);

    return {
        x * s,
        y * s,
        z * s,
        cosf(angle / 2.0f)
    };
}

inline Quat AddQuat(Quat a, Quat b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline Quat ConjQuat(Quat q)
{
    return { -q.x, -q.y, -q.z, q.w };
}

inline Quat operator*(Quat a, Quat b)
{
    return {
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
}

inline Quat RotateQuat(v3 axis, f32 angle)
{
    f32 s = sin(0.5f * angle);
    v3 v = { s * axis.x, s * axis.y, s * axis.z };

    return { v.x, v.y, v.z, cosf(0.5f * angle) };
}

// --------------------- v2i ------------------------- // 

inline v2i V2i(i32 x, i32 y)
{
    return { x, y };
}

inline v2i V2i(v2 a)
{
    return { i32(a.x), i32(a.y) };
}

inline v2i V2i(v3 a)
{
    return { i32(a.x), i32(a.y) };
}

inline v2i operator-(v2i a)
{
    return { -a.x, -a.y };
}

inline v2i operator+(v2i a, v2i b)
{
    return { a.x + b.x, a.y + b.y };
}

inline v2i operator-(v2i a, v2i b)
{
    return { a.x - b.x, a.y - b.y };
}

inline v2i operator*(v2i a, v2i b)
{
    return { a.x * b.x, a.y * b.y };
}

inline v2i operator/(v2i a, v2i b)
{
    return { a.x / b.x, a.y / b.y };
}

inline v2i operator*(v2i a, i32 s)
{
    return { a.x * s, a.y * s };
}

inline v2i operator*(i32 s, v2i a)
{
    return { a.x * s, a.y * s };
}

inline v2i operator/(v2i a, i32 s)
{
    return { a.x / s, a.y / s };
}

inline bool operator==(v2i a, v2i b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(v2i a, v2i b)
{
    return a.x != b.x || a.y != b.y;
}

inline v2i operator+=(v2i& a, v2i b)
{
    return a = a + b;
}

inline v2i operator-=(v2i& a, v2i b)
{
    return a = a - b;
}

inline i32 DistSq(v2i a, v2i b)
{
    v2i d = a - b;
    return d.x * d.x + d.y * d.y;
}

inline i32 Manhattan(v2i a, v2i b)
{
    i32 dx = abs(a.x - b.x);
    i32 dy = abs(a.y - b.y);

    return dx + dy;
}

inline v2i Clamp(v2i a, i32 min, i32 max)
{
    return {
        Clamp(a.x, min, max),
        Clamp(a.y, min, max),
    };
}

inline v2i Min(v2i a, v2i b)
{
    return {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y)
    };
}

inline v2i Max(v2i a, v2i b)
{
    return {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y)
    };
}

// --------------------- v3i ------------------------- // 

inline v3i V3i(i32 x, i32 y, i32 z)
{
    return { x, y, z };
}

inline v3i V3i(v3 a)
{
    return { i32(a.x), i32(a.y), i32(a.z) };
}

inline v3i V3i(v2i a, i32 z)
{
    return { a.x, a.y, z };
}

inline v3i operator+(v3i a, v3i b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline v3i operator-(v3i a, v3i b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline v3i operator*(v3i a, v3i b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

inline v3i operator/(v3i a, v3i b)
{
    return { a.x / b.x, a.y / b.y, a.z / b.z };
}

inline v3i operator*(v3i a, i32 s)
{
    return { a.x * s, a.y * s, a.z * s };
}

inline v3i operator*(i32 s, v3i a)
{
    return { a.x * s, a.y * s, a.z * s };
}

inline v3i operator/(v3i a, i32 s)
{
    return { a.x / s, a.y / s, a.z / s };
}

inline bool operator==(v3i a, v3i b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator!=(v3i a, v3i b)
{
    return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}

inline i32 Manhattan(v3i a, v3i b)
{
    i32 dx = abs(a.x - b.x);
    i32 dy = abs(a.y - b.y);
    i32 dz = abs(a.z - b.z);

    return dx + dy + dz;
}

inline i32 DistSq(v3i a, v3i b)
{
    v3i d = a - b;
    return d.x * d.x + d.y * d.y + d.z * d.z;
}

// --------------------- v4i ------------------------- // 

inline v4i V4i(i32 x, i32 y, i32 z, i32 w)
{
    return { x, y, z, w };
}

inline v4i operator+(v4i a, v4i b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline v4i operator-(v4i a, v4i b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline v4i operator*(v4i a, v4i b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline v4i operator/(v4i a, v4i b)
{
    return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

inline v4i operator*(v4i a, i32 s)
{
    return { a.x * s, a.y * s, a.z * s, a.w * s };
}

inline v4i operator/(v4i a, i32 s)
{
    return { a.x / s, a.y / s, a.z / s, a.w / s };
}

// =================================================== SHAPES =============================================== //

// ---------------------------------------- CIRCLE -------------------------------------- //

struct Circle
{
    v2      pos;
    f32     rad;
};

inline b32 Intersect(Circle a, Circle b)
{
    v2  d   = b.pos - a.pos;
    f32 rt  = a.rad + b.rad;

    return d.x * d.x + d.y * d.y < rt * rt;
};

inline v2 GetIntersectVector(Circle a, Circle b)
{
    v2  delta   = b.pos - a.pos;
    f32 depth   = Len(delta) - (a.rad + b.rad);
    
    return depth * delta;
}

// --------------------------------------- SHPERE ------------------------------------ //

struct Sphere
{
    v3      pos;
    f32     rad;
};

inline b32 Intersect(Sphere a, Sphere b)
{
    v3  d   = b.pos - a.pos;
    f32 rt  = a.rad + b.rad;

    return d.x * d.x + d.y * d.y < rt * rt;
};

inline v3 GetIntersectVector(Sphere a, Sphere b)
{
    v3  delta   = b.pos - a.pos;
    f32 depth   = Len(delta) - (a.rad + b.rad);
    
    return depth * delta;
}

// ---------------------------------------- RECTANGLE ------------------------------------- //

struct Rect
{
    v2  min;
    v2  max;
};

inline b32 Contains(Rect rect, v2 pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

inline b32 Intersect(Rect a, Rect b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

inline Rect GetOverlap(Rect a, Rect b)
{
    return {
        Max(a.min, b.min),
        Min(a.max, b.max)
    };
}

inline v2 GetIntersectVector(Rect a, Rect b)
{
    Rect o      = GetOverlap(a, b);
    v2   delta  = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);

    return V2(Sign(delta.x), Sign(delta.y)) * (o.max - o.min);
}

inline Rect Move(Rect rect, v2 offset)
{
    return {
        rect.min + offset,
        rect.max + offset,
    };
}

// ------------------------------------- RECTANGLE - I32 ------------------------------------- //

struct RectI32
{
    v2i min;
    v2i max;
};

inline b32 Contains(RectI32 rect, v2i pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

inline b32 Intersect(RectI32 a, RectI32 b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

inline RectI32 GetOverlap(RectI32 a, RectI32 b)
{
    return {
        Max(a.min, b.min),
        Min(a.max, b.max)
    };
}

inline v2i GetIntersectVector(RectI32 a, RectI32 b)
{
    RectI32 o      = GetOverlap(a, b);
    v2i     delta  = (a.min + a.max) / 2 - (b.min + b.max) / 2;

    return V2i(Sign(delta.x), Sign(delta.y)) * (o.max - o.min);
}


// ----------------------------------------- BOX --------------------------------------- //

struct Box
{
    v3  min;
    v3  max;
};

inline b32 Contains(Box rect, v3 pos)
{
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

inline b32 Intersect(Box a, Box b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

inline Box GetOverlap(Box a, Box b)
{
    return {
        Max(a.min, b.min),
        Min(a.max, b.max)
    };
}

inline v3 GetIntersectVector(Box a, Box b)
{
    Box o       = GetOverlap(a, b);
    v3  delta   = 0.5f * (a.min + a.max) - 0.5f * (b.min + b.max);

    return V3(Sign(delta.x), Sign(delta.y), Sign(delta.z)) * (o.max - o.min);
}

inline Box Move(Box box, v3 offset)
{
    return {
        box.min + offset,
        box.max + offset,
    };
}

struct Plane
{
    f32     a;
    f32     b;
    f32     c;
    f32     d;
};

inline Plane NormalizePlane(Plane plane)
{
    f32 mag = Sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;

    return plane;
}

struct Frustum
{
    Plane   plane[6];
};

inline Frustum MakeFrustum(m4 combo_matrix, bool normalize = true)
{
    Frustum frustum = {};

    // left clipping plane
    frustum.plane[0].a = combo_matrix.x.w + combo_matrix.x.x;
    frustum.plane[0].b = combo_matrix.y.w + combo_matrix.y.x;
    frustum.plane[0].c = combo_matrix.z.w + combo_matrix.z.x;
    frustum.plane[0].d = combo_matrix.w.w + combo_matrix.w.x;

    // right clipping plane
    frustum.plane[1].a = combo_matrix.x.w - combo_matrix.x.x;
    frustum.plane[1].b = combo_matrix.y.w - combo_matrix.y.x;
    frustum.plane[1].c = combo_matrix.z.w - combo_matrix.z.x;
    frustum.plane[1].d = combo_matrix.w.w - combo_matrix.w.x;

    // top clipping plane
    frustum.plane[2].a = combo_matrix.x.w - combo_matrix.x.y;
    frustum.plane[2].b = combo_matrix.y.w - combo_matrix.y.y;
    frustum.plane[2].c = combo_matrix.z.w - combo_matrix.z.y;
    frustum.plane[2].d = combo_matrix.w.w - combo_matrix.w.y;

    // bottom clipping plane
    frustum.plane[3].a = combo_matrix.x.w + combo_matrix.x.y;
    frustum.plane[3].b = combo_matrix.y.w + combo_matrix.y.y;
    frustum.plane[3].c = combo_matrix.z.w + combo_matrix.z.y;
    frustum.plane[3].d = combo_matrix.w.w + combo_matrix.w.y;

    // near clipping plane
    frustum.plane[4].a = combo_matrix.x.w + combo_matrix.x.z;
    frustum.plane[4].b = combo_matrix.y.w + combo_matrix.y.z;
    frustum.plane[4].c = combo_matrix.z.w + combo_matrix.z.z;
    frustum.plane[4].d = combo_matrix.w.w + combo_matrix.w.z;

    // far clipping plane
    frustum.plane[5].a = combo_matrix.x.w - combo_matrix.x.z;
    frustum.plane[5].b = combo_matrix.y.w - combo_matrix.y.z;
    frustum.plane[5].c = combo_matrix.z.w - combo_matrix.z.z;
    frustum.plane[5].d = combo_matrix.w.w - combo_matrix.w.z;

    // Normalize the plane equations, if requested
    if (normalize == true) {
        frustum.plane[0] = NormalizePlane(frustum.plane[0]);
        frustum.plane[1] = NormalizePlane(frustum.plane[1]);
        frustum.plane[2] = NormalizePlane(frustum.plane[2]);
        frustum.plane[3] = NormalizePlane(frustum.plane[3]);
        frustum.plane[4] = NormalizePlane(frustum.plane[4]);
        frustum.plane[5] = NormalizePlane(frustum.plane[5]);
    }
    
    return frustum;
}

inline bool Contains(Frustum frustum, v3 pos)
{
    for(i32 i = 0; i < 6; i++ )	 {
		if(frustum.plane[i].a * pos.x + frustum.plane[i].b * pos.y + frustum.plane[i].c * pos.z + frustum.plane[i].d <= 0) {
			return false;
		}
	}

	return true;
}

inline bool Intersect(Frustum frustum, Sphere sphere)
{
    for(i32 i = 0; i < 6; i++ )	 {
		if(frustum.plane[i].a * sphere.pos.x + frustum.plane[i].b * sphere.pos.y + frustum.plane[i].c * sphere.pos.z + frustum.plane[i].d <= -sphere.rad) {
			return false;
		}
	}

	return true;
}

inline bool Intersect(Frustum frustum, Box box)
{
    for(int i = 0; i < 6; i++ ) {
		if(frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.min.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.min.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.min.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;
		if(frustum.plane[i].a * box.max.x + frustum.plane[i].b * box.max.y + frustum.plane[i].c * box.max.z + frustum.plane[i].d > 0) continue;

		return false;
	}

	return true;
}

// ================================================= COLOR PACKING =========================================== //

inline u32 PackColorU8(u8 r, u8 g, u8 b, u8 a)
{
    u32 color = 0;

    color |= u32(r) << 0;
    color |= u32(g) << 8;
    color |= u32(b) << 16;
    color |= u32(a) << 24;

    return color;
}

inline u32 PackColorF32(f32 r, f32 g, f32 b, f32 a)
{
    return PackColorU8(255 * r, 255 * g, 255 * b, 255 * a);
}

inline u32 PackColor(v4 color)
{
    return PackColorF32(color.r, color.g, color.b, color.a);
}

inline u32 PackColor(const f32 color[4])
{
    return PackColorF32(color[0], color[1], color[2], color[3]);
}

inline u32 PackColor(v3 color, f32 a = 1.0)
{
    return PackColorF32(color.r, color.g, color.b, a);
}

inline u32 GetRed(u32 color)
{
    return color & 0xff;
}

inline u32 GetGreen(u32 color)
{
    return (color & 0xff00) >> 8;
}

inline u32 GetBlue(u32 color)
{
    return (color & 0xff0000) >> 16;
}

inline u32 GetAlpha(u32 color)
{
    return (color & 0xff000000) >> 8;
}

// ============================================== RANDOM GENERATOR =========================================== //
// random number generator: xorshf96

struct RndGen
{
    u32     x;
    u32     y;
    u32     z;
};

inline u32 NextRandom(RndGen* g)
{
    g->x ^= g->x << 16;
    g->x ^= g->x >> 5;
    g->x ^= g->x << 1;

    u32 t = g->x;

    g->x = g->y;
    g->y = g->z;
    g->z = t ^ g->x ^ g->y;

    return g->z;
}

thread_local RndGen default_rnd_gen = { 123456789u, 362436069u, 521288629u };

inline void RandSeed(u32 x, u32 y, u32 z)
{
    assert(x && y && z);

    default_rnd_gen.x = x;
    default_rnd_gen.y = y;
    default_rnd_gen.z = z;
}

inline i32 RandI32(i32 min, i32 max)
{
    return min + NextRandom(&default_rnd_gen) % (max - min);
}

inline f32 RandF32(f32 min, f32 max)
{
    return min + ((f32)NextRandom(&default_rnd_gen) / (f32)0xFFFFFFFF) * (max - min); 
}

inline v2 RandV2(void)
{
    return Norm(V2(RandF32(-1, 1), RandF32(-1, 1)));
}

inline v3 RandV3(void)
{
    return Norm(V3(RandF32(-1, 1), RandF32(-1, 1), RandF32(-1, 1)));
}

inline v2 RandV2(f32 min, f32 max)
{
    return RandF32(min, max) * RandV2();
}

inline v3 RandV3(f32 min, f32 max)
{
    return RandF32(min, max) * RandV3();
}

inline Quat RandQuat(void)
{
    return MakeQuat(RandF32(-1.0f, 1.0f), RandF32(-1.0f, 1.0f), RandF32(-1.0f, 1.0f), RandF32(-PI, PI));
}

// =================================== XORSHIFT32 ============================================= //

inline u32 RandU32(u32* state)
{
	u32 x = *state;

	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

	return *state = x;
}

inline i32 RandI32(u32* state, i32 min, i32 max)
{
    return min + RandU32(state) % (max - min);
}

inline f32 RandF32(u32* state, f32 min, f32 max)
{
    return min + ((f32)RandU32(state) / (f32)0xFFFFFFFF) * (max - min); 
}

inline v2 RandV2(u32* state)
{
    return Norm(V2(RandF32(state, -1, 1), RandF32(state, -1, 1)));
}

inline v3 RandV3(u32* state)
{
    return Norm(V3(RandF32(state, -1, 1), RandF32(state, -1, 1), RandF32(state, -1, 1)));
}

inline v2 RandV2(u32* state, f32 min, f32 max)
{
    return RandF32(state, min, max) * RandV2(state);
}

inline v3 RandV3(u32* state, f32 min, f32 max)
{
    return RandF32(state, min, max) * RandV3(state);
}

inline Quat RandQuat(u32* state)
{
    return MakeQuat(RandF32(state, -1.0f, 1.0f), RandF32(state, -1.0f, 1.0f), RandF32(state, -1.0f, 1.0f), RandF32(state, -PI, PI));
}

// ============================ RANDOM(HASH) FUNCTION ======================= //

inline u32 HashU32(u32 a)
{
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

    return a;
}

inline u32 HashI32(i32 a)
{
    union { u32 u; i32 i; } convert;
    convert.i = a;
    return HashU32(convert.u);
}

inline u32 HashCStr(const char* str)
{
    u32 hash = 0;

    while (*str) {
        hash = (hash << 7) + (hash >> 25) + *str++;
    }

    return hash + (hash >> 16);
}

inline u32 HashMem(const void* mem, u32 size)
{
    const u8* m     = (const u8*)mem;
    u32 hash        = 0;

    for (u32 i = 0; i < size; ++i) {
        hash = (hash << 7) + (hash >> 25) + m[i];
    }

    return hash + (hash >> 16);
}

#define HASH_PRIME0 (3323784421u)
#define HASH_PRIME1 (1449091801u)
#define HASH_PRIME2 (4280703257u)
#define HASH_PRIME3 (1609059329u)

inline u32 HashV2i(v2i k)
{
    u32 a = HashI32(k.x);
    u32 b = HashI32(k.y);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

inline u32 HashV3i(v3i& k)
{
    u32 a = HashI32(k.x);
    u32 b = HashI32(k.y);
    u32 c = HashI32(k.z);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

inline u32 HashV4i(v4i k)
{
    u32 a = HashI32(k.x);
    u32 b = HashI32(k.y);
    u32 c = HashI32(k.z);
    u32 d = HashI32(k.w);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2) ^ (d * HASH_PRIME3);
}

// ==================================== FILES ==================================== //

inline size_t GetFileSize(FILE* fp)
{
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

inline char* ReadFileAsCStr(const char* file_name)
{
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0) {
        size_t size = GetFileSize(fp);

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

inline b32 WriteCStrToFile(const char* file_name, const char* buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return false;
}

inline b32 AppendCStrToFile(const char* file_name, const char* buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);

        fclose(fp);

        return n == size;
    }

    return false;
}

inline b32 ReadFileAsBin(const char* file_name, void* buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);

        return 1;
    }

    return false;
} 

inline b32 WriteBinToFile(const char* file_name, const void* buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }

    return false;
}

// ====================================================================================================== //

inline void F4x4Mul64(f64 *R, const f64 *a, const f64 *b)
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

inline void F4x4Mulv64(f64 *out, const f64 *M, const f64 *v)
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

inline void F4x4Invert64(f64* T, const f64* M)
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

inline b32 F4x4Project64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport)
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

inline b32 F4x4Unproject64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport)
{
    f64 m[16], A[16];
    f64 in[4], out[4];

    F4x4Mul64(A, projection, modelview);
    F4x4Invert64(m, A);

    in[0] = (winx - (f64)viewport[0]) / (f64)viewport[2] * 2.0f - 1.0f;
    in[1] = (winy - (f64)viewport[1]) / (f64)viewport[3] * 2.0f - 1.0f;
    in[2] = 2.0f * winz - 1.0f;
    in[3] = 1.0;

    F4x4Mulv64(out, m, in);
    
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

union GamepadButtons
{
    struct
    {
        u32     X       : 1;
        u32     A       : 1;
        u32     B       : 1;
        u32     Y       : 1;

        u32     LB      : 1;
        u32     RB      : 1;
        u32     LT      : 1;
        u32     RT      : 1;
        
        u32     select  : 1;
        u32     start   : 1;
        u32     LS      : 1;
        u32     RS      : 1;

        u32     UP      : 1;
        u32     RIGHT   : 1;
        u32     DOWN    : 1;
        u32     LEFT    : 1;
    } button;

    u32 data;
};

struct Gamepad
{
    b32             active;

    v2              LS;
    v2              RS;

    f32             LT;
    f32             RT;

    GamepadButtons  state;
    GamepadButtons  pressed;
    GamepadButtons  released;
};

// =========================================================== MOUSE MODES ================================================= //

enum MouseMode : u32
{
    MOUSE_NORMAL,
    MOUSE_HIDDEN,
    MOUSE_DISABLED
};

// ===========================================================  PLATFORM =================================================== //

struct Platform
{
    b32     close;

    i32     width;
    i32     height;
    f32     aspect_ratio;

    HWND    native;

    b32     fullscreen;
    b32     _fullscreen_state_last_update;

    struct Time
    {
        f64     total;
        f64     delta;
    } time;

    struct Mouse
    {
        u32     mode;

        b32     is_down : 1;
        b32     is_pressed : 1;
        b32     is_released : 1;

        v2      pos;
        v2      delta;
        v2      scroll;

        b8      state[MOUSE_BUTTON_LAST + 1];
        b8      pressed[MOUSE_BUTTON_LAST + 1];
        b8      released[MOUSE_BUTTON_LAST + 1];
    } mouse;

    struct Keyboard
    {
        i32     key;
        i32     ascii;

        b32     is_down : 1;
        b32     is_pressed : 1;
        b32     is_repeat : 1;
        b32     is_released : 1;
        b32     is_ascii : 1;
    
        b8      state[KEY_LAST + 1];
        b8      pressed[KEY_LAST + 1];
        b8      repeat[KEY_LAST + 1];
        b8      released[KEY_LAST + 1];
    } keyboard;

    Gamepad gamepad[JOYSTICK_LAST];

    inline Gamepad* GetGamepad(int index)
    {
        return gamepad[index].active? &gamepad[index] : NULL;
    }
};

global_variable Platform platform;

global_variable struct
{
    GLFWwindow* window;
    GLFWmonitor* monitor;
} platform_internal;

internal void WindowKeyCallback(GLFWwindow*, int key, int, int action, int)
{
    switch (action) {
        case GLFW_PRESS:
            platform.keyboard.key           = key;

            platform.keyboard.is_down       = 1;
            platform.keyboard.is_pressed    = 1;
            platform.keyboard.is_repeat     = 1;

            platform.keyboard.state[key]    = 1;
            platform.keyboard.pressed[key]  = 1;
            platform.keyboard.repeat[key]   = 1;
            break;
        case GLFW_REPEAT:
            platform.keyboard.is_repeat     = 1;
            platform.keyboard.repeat[key]   = 1;
            break;
        case GLFW_RELEASE:
            platform.keyboard.is_down       = 0;
            platform.keyboard.is_released   = 1;
            
            platform.keyboard.state[key]    = 0;
            platform.keyboard.released[key] = 1;
            break;
    }
}

internal void WindowCharCallback(GLFWwindow* window, unsigned int codepoint)
{
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

internal void WindowMouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    switch (action) {
        case GLFW_PRESS: {
            platform.mouse.is_down          = 1;
            platform.mouse.is_pressed       = 1;

            platform.mouse.state[button]    = 1;
            platform.mouse.pressed[button]  = 1;
            break;
        }
        case GLFW_RELEASE: {
            platform.mouse.is_down          = 0;
            platform.mouse.is_released      = 1;

            platform.mouse.state[button]    = 0;
            platform.mouse.released[button] = 1;
            break;
        }
    }
}

internal void WindowScrollCallback(GLFWwindow*, f64 xoffset, f64 yoffset)
{
    platform.mouse.scroll.x = xoffset;
    platform.mouse.scroll.y = yoffset;
}

internal void WindowJoystickCallback(int joy, int event)
{
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);

        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

void InitPlatform(const char* title, int width, int height, int samples)
{
    glfwInit();

    platform_internal.monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

    glfwWindowHint(GLFW_RED_BITS,       mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS,     mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS,      mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE,   mode->refreshRate);

#ifdef ATS_MODERN_OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.width              = width;
    platform.height             = height;

    platform_internal.window    = glfwCreateWindow(width, height, title, NULL, NULL);
    platform.native             = glfwGetWin32Window(platform_internal.window);

    glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform_internal.window);

#ifdef ATS_MODERN_OPENGL 
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

    glfwSetKeyCallback(platform_internal.window,            WindowKeyCallback);
    glfwSetCharCallback(platform_internal.window,           WindowCharCallback);
    glfwSetMouseButtonCallback(platform_internal.window,    WindowMouseButtonCallback);
    glfwSetScrollCallback(platform_internal.window,         WindowScrollCallback);
    glfwSetJoystickCallback(WindowJoystickCallback);

    // init mouse:
    {
        f64 x = 0.0;
        f64 y = 0.0;

        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i)) {
            platform.gamepad[i].active = 1;
        }
    }

    glfwSetTime(0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    glfwSwapBuffers(platform_internal.window);
}

void UpdatePlatform(void)
{
    if (glfwWindowShouldClose(platform_internal.window))
        platform.close = 1;

    if (platform.close)
        glfwSetWindowShouldClose(platform_internal.window, 1);

    platform.mouse.is_pressed = 0;
    platform.mouse.is_released = 0;
    platform.keyboard.is_pressed = 0;
    platform.keyboard.is_repeat = 0;
    platform.keyboard.is_released = 0;
    platform.keyboard.is_ascii = 0;

    // update mouse:
    {
        f64 x, y;
        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.delta.x = x - platform.mouse.pos.x;
        platform.mouse.delta.y = y - platform.mouse.pos.y;

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        switch (platform.mouse.mode) {
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

        for (int i = 0; i < JOYSTICK_LAST; ++i) {
            if (platform.gamepad[i].active) {
                GamepadButtons old = platform.gamepad[i].state;

                platform.gamepad[i].state.data = 0;
                platform.gamepad[i].pressed.data = 0;
                platform.gamepad[i].released.data = 0;

                glfwGetGamepadState(i, &state);

                platform.gamepad[i].LS.x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].LS.y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].RS.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
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

        if (platform.fullscreen) {
            glfwSetWindowMonitor(platform_internal.window, platform_internal.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            glfwSetWindowMonitor(platform_internal.window, NULL, 64, 64, mode->width - 256, mode->height - 256, mode->refreshRate);
        }
    }

    platform._fullscreen_state_last_update = platform.fullscreen;

    glfwGetWindowSize(platform_internal.window, &platform.width, &platform.height);
    platform.aspect_ratio = (f32)platform.width / (f32)platform.height;

    glViewport(0, 0, platform.width, platform.height);

    memset(platform.keyboard.pressed, 0, sizeof (platform.keyboard.pressed));
    memset(platform.keyboard.repeat, 0, sizeof (platform.keyboard.repeat));
    memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

    memset(platform.mouse.pressed, 0, sizeof (platform.mouse.pressed));
    memset(platform.mouse.released, 0, sizeof (platform.mouse.released));

    glfwPollEvents();
    glfwSwapBuffers(platform_internal.window);

    {
        platform.time.delta = glfwGetTime() - platform.time.total;
        platform.time.total += platform.time.delta;
    }
}

// =================================================== TIMER STUFF =================================================== //

inline f64 GetTime(void)
{
    return glfwGetTime();
}

#endif // ATS_PLATFORM_GLFW

// ===================================================== SHADERS ===================================================== //

#ifdef ATS_MODERN_OPENGL

struct Shader
{
    u32 program;

    inline void use() const { glUseProgram(program); }

    inline u32 GetLocation(const char *var_name) const { return glGetUniformLocation(program, var_name); }

    inline void Set(u32 loc, int n) const                      { Use(); glUniform1i(loc, n); }
    inline void Set(u32 loc, f32 n) const                      { Use(); glUniform1f(loc, n); }
    inline void Set(u32 loc, f32 a, f32 b) const               { Use(); glUniform2f(loc, a, b); }
    inline void Set(u32 loc, f32 a, f32 b, f32 c) const        { Use(); glUniform3f(loc, a, b, c); }
    inline void Set(u32 loc, f32 a, f32 b, f32 c, f32 d) const { Use(); glUniform4f(loc, a, b, c, d); }
    inline void Set(u32 loc, v2 u) const                       { Use(); glUniform2fv(loc, 1, u.array); }
    inline void Set(u32 loc, v3 u) const                       { Use(); glUniform3fv(loc, 1, u.array); }
    inline void Set(u32 loc, v4 u) const                       { Use(); glUniform4fv(loc, 1, u.array); }
    inline void Set(u32 loc, m4 m) const                       { Use(); glUniformMatrix4fv(loc, 1, GL_FALSE, m.array); }

    inline void Set(const char* loc, int n) const                      { Set(get_location(loc), n); }
    inline void Set(const char* loc, f32 n) const                      { Set(get_location(loc), n); }
    inline void Set(const char* loc, f32 a, f32 b) const               { Set(get_location(loc), a, b); }
    inline void Set(const char* loc, f32 a, f32 b, f32 c) const        { Set(get_location(loc), a, b, c); }
    inline void Set(const char* loc, f32 a, f32 b, f32 c, f32 d) const { Set(get_location(loc), a, b, c, d); }
    inline void Set(const char* loc, v2 u) const                       { Set(get_location(loc), u); }
    inline void Set(const char* loc, v3 u) const                       { Set(get_location(loc), u); }
    inline void Set(const char* loc, v4 u) const                       { Set(get_location(loc), u); }
    inline void Set(const char* loc, m4 m) const                       { Set(get_location(loc), m); }
};

inline u32 CompileShader(const char* source, unsigned int type)
{
    int success;

    char info_log[512];

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

inline u32 LinkShaderProgramVF(u32 vertex_shader, u32 fragment_shader)
{
    int     success;
    char    info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

inline u32 LinkShaderProgramVFG(u32 vertex_shader, u32 fragment_shader, u32 geometry_shader)
{
    int success;
    char info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, geometry_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

inline u32 LinkShaderProgramV(u32 vertex_shader)
{
    int success;
    char info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);

    const char* varyings[] = { "pos1", "vel1" };
    glTransformFeedbackVaryings(shader_program, 2, varyings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

inline Shader loadShaderFromMemory(const char *vs, const char *fs, const char *gs = NULL)
{
    Shader shader = {};

    if (vs != NULL && fs != NULL && gs != NULL) {
        u32 vertex   = CompileShader(vs, GL_VERTEX_SHADER);
        u32 fragment = CompileShader(fs, GL_FRAGMENT_SHADER);
        u32 geometry = CompileShader(gs, GL_GEOMETRY_SHADER);

        shader.program = LinkShaderProgramVFG(vertex, fragment, geometry);

        glUseProgram(shader.program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs != NULL && gs == NULL) {
        u32 vertex   = CompileShader(vs, GL_VERTEX_SHADER);
        u32 fragment = CompileShader(fs, GL_FRAGMENT_SHADER);

        shader.program = LinkShaderProgramVF(vertex, fragment);

        glUseProgram(shader.program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs == NULL) {
        u32 vertex = CompileShader(vs, GL_VERTEX_SHADER);

        shader.program = LinkShaderProgramV(vertex);

        glUseProgram(shader.program);
        glDeleteShader(vertex);
    }

    return shader;
}

inline Shader LoadShaderFromFile(const char *vs, const char *fs)
{
    char* vertex   = ReadFileAsCStr(vs);
    char* fragment = ReadFileAsCStr(fs);

    Shader shader = LoadShaderFromMemory(vertex, fragment);

    free(vertex);
    free(fragment);

    return shader;
}

inline u32 LinkShaderProgramC(u32 compute_shader)
{
    int success;
    char info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, compute_shader);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

inline Shader LoadComputeShaderFromMemory(const char* cs)
{
    Shader shader = {};

    u32 compute = CompileShader(cs, GL_COMPUTE_SHADER);

    shader.program = LinkShaderProgramC(compute);

    glUseProgram(shader.program);
    glDeleteShader(compute);

    return shader;
}

inline Shader LoadComputeShaderFromFile(const char* cs)
{
    char*   compute = ReadFileAsCStr(cs);
    Shader  shader  = LoadComputeShaderFromMemory(compute);

    free(compute);

    return shader;
}

#endif // ATS_MODERN_OPENGL

// =================================================== BITMAP FONT =================================================== //

#define BITMAP_COUNT    (256)

global_variable const u64 bitascii[BITMAP_COUNT] = {
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

// ================================================ CUBE ENGINE =================================================== //

#ifdef ATS_CUBE_ENGINE

#define SHADOW_WIDTH    (2048) 
#define SHADOW_HEIGHT   (2048) 

struct Vertex
{
    v4 position;
    v4 normal;
    v4 color;
    v2 texture;
};

// Layout:  vertex : normal : color : texture
global_variable const Vertex vertex_array_cube[] = {
    // back face
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right         
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    // front face
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f,  0.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    // left face
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    // right face
    { { 1.0f, -1.0f,  1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { { 1.0f, -1.0f, -1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f, -1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right         
    { { 1.0f, -1.0f, -1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f,  1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { { 1.0f, -1.0f,  1.0f, 1.0f },  { 1.0f,  0.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left     
    // bottom face
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    // top face
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { {  1.0f,  1.0f , 1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right     
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f,  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }  // bottom-left  
};

// Layout:  vertex : normal
global_variable const Vertex vertex_array_square[] = {
    { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { {  1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-right         
    { {  1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // bottom-left
    { { -1.0f,  1.0f, 0.0f, 1.0f }, { 0.0f,  0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // top-left
};

global_variable const char* ce_shader_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    uniform mat4 PV;

    out vec4 color;

    void main() {
        gl_Position = PV * M * vp;
        color       = C;
    }
);

global_variable const char* ce_shader_fragment = GLSL_SHADER(
    out vec4 frag_color;
    in  vec4 color;
    
    void main() {
        frag_color = color;
    }
);

global_variable const char* ce_shader_vertex_light = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec3 frag_pos;
    out vec3 normal;
    out vec4 frag_pos_light_space;
    out vec4 color;

    uniform mat4 PV;
    uniform mat4 light_space_matrix;

    void main() {
        vec4 mpos               = M * vp;
        gl_Position             = PV * mpos;
        frag_pos                = mpos.xyz;
        normal                  = normalize((M * np).xyz);
        frag_pos_light_space    = light_space_matrix * vec4(frag_pos, 1.0);
        color                   = C * vc;
    }
);

global_variable const char* ce_shader_fragment_light = GLSL_SHADER(
    out vec4 frag_color;

    in vec3 frag_pos;
    in vec3 normal;
    in vec2 tex_coords;
    in vec4 frag_pos_light_space;
    in vec4 color;

    uniform sampler2D shadow_map;
    uniform sampler2D color_map;

    uniform vec3 light_pos;
    uniform vec3 view_pos;
    //
    uniform vec3 light_color;

    float shadow_calculation(vec3 proj_coords) {
        if(proj_coords.z > 1.0)
            return 0.0;

        float current_depth = proj_coords.z;

        vec3 normal = normalize(normal);
        vec3 light_dir = normalize(light_pos - frag_pos);
        float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);

        float shadow = current_depth - bias > texture(shadow_map, proj_coords.xy).r? 1.0 : 0.0;

        return shadow;
    }

    vec4 color_calculation(vec3 proj_coords) {
        vec4 closest_depth  = texture(color_map, proj_coords.xy);
        return closest_depth;
    }
   
    void main() {
        float ambient_str = 0.2;
        vec3 ambient = ambient_str * light_color;
        vec3 light_dir = normalize(light_pos - frag_pos);
        float diff = max(dot(normal, light_dir), 0.0);
        vec3 diffuse = diff * light_color;
        float specular_str = 0.1;
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir  = reflect(-light_dir, normal);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 2);
        vec3 specular = specular_str * spec * light_color;

        vec3 proj_coords = 0.5 * (frag_pos_light_space.xyz / frag_pos_light_space.w) + 0.5;

        float shadow = shadow_calculation(proj_coords);
        vec4 caster = color_calculation(proj_coords);

        vec3 result = color.rgb * (ambient + (1.0 - 0.7 * caster.a * shadow) * (diffuse + specular));

        frag_color = vec4(result, color.a);
    }
);

global_variable const char* ce_depth_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec4 color;

    uniform mat4 light_space_matrix;

    void main() {
        gl_Position = light_space_matrix * M * vp;
        color = C;
    }
);

global_variable const char* ce_depth_fragment = GLSL_SHADER(
    in  vec4 color;
    out vec4 frag_color;

    void main() {
        gl_FragDepth = gl_FragCoord.z;
        frag_color = color;
    }
);

// ----------------------------------------------------------------------------------------------- //

global_variable Shader ce_shader;
global_variable Shader ce_shader_no_light;
global_variable Shader ce_shader_depth;

global_variable u32 ce_vao_cube;
global_variable u32 ce_vbo_cube;
global_variable u32 ce_vbo_cube_data;

global_variable u32 ce_depth_fbo;
global_variable u32 ce_depth_map;

global_variable u32 ce_color_map;

struct CE_Object {
    v4 color;
    m4 model;
};

global_variable m4 ce_projection_view  = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
global_variable m4 ce_light_space_matrix = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
global_variable v3 ce_view_pos = { 0.0f, 0.0f, 0.0f };
global_variable v3 ce_light_pos = { 0.0f, 0.0f, 0.0f };
global_variable v3 ce_light_color = { 1.0f, 1.0f, 1.0f };

#define RENDER_ARRAY_INIT_SIZE  (2048)

global_variable int render_capacity = 0;
global_variable int render_count = 0;

global_variable CE_Object render_array[1024 * 1024];

inline void ce_Push(const CE_Object& obj)
{
    if (render_count < ARRAY_COUNT(render_array)) {
        render_array[render_count++] = obj;
    }
}

inline int ce_GetCubeCount(void)
{
    return render_count;
}

inline void ce_SetLight(v3 pos, v3 color)
{
    ce_light_pos = pos;
    ce_light_color = color;
}

inline void ce_SetView(v3 pos, v3 look, v3 up, f32 fov, f32 near_plane, f32 far_plane)
{
    ce_projection_view = 
        Perspective(TO_RAD(fov), platform.aspect_ratio, near_plane, far_plane) * 
        LookA(pos, look, up);

    ce_view_pos = pos;
}

inline void ce_SetView2D(v3 pos, v3 look, v3 up, v2 rad, f32 near_plane, f32 far_plane)
{
    ce_projection_view =
        Ortho(-rad.x, rad.x, -rad.y, rad.y, near_plane, far_plane) *
        LookAt(pos, look, up);

    ce_view_pos = pos;
}

inline void ce_SetProjectionView(m4 projection_view)
{
    ce_projection_view = projection_view;

    ce_view_pos.x = projection_view.array[12];
    ce_view_pos.y = projection_view.array[13];
    ce_view_pos.z = projection_view.array[14];
}

inline void ce_SetLightView(v3 pos, v3 look, v3 up, f32 fov, f32 near_plane, f32 far_plane)
{
    m4 light_projection = Perspective(TO_RAD(fov), platform.aspect_ratio, near_plane, far_plane);
    m4 light_view       = LookAt(pos, look, up);

    ce_light_space_matrix = light_projection * light_view;
}

inline void ce_SetVertexArray(const Vertex* vertex_array, int vertex_count)
{
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof *vertex_array, vertex_array, GL_STATIC_DRAW);
}

inline void ce_SetRenderObjectArray(const CE_Object* robj_array, int robj_count)
{
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);
    glBufferData(GL_ARRAY_BUFFER, robj_count * sizeof *robj_array, robj_array, GL_STATIC_DRAW);
}

inline void ce_Render(int type, const Vertex* vertex_array, int vertex_count, const CE_Object* robj_array, int robj_count)
{
    ce_SetVertexArray(vertex_array, vertex_count);
    ce_SetRenderObjectArray(robj_array, robj_count);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // depth render:
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);

        ce_shader_depth.Use();
        ce_shader_depth.Set("light_space_matrix", ce_light_space_matrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);

        u32 attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, attachments);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glCullFace(GL_BACK);

    // normal render:
    {
        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);

        glViewport(0, 0, platform.width, platform.height);

        ce_shader.use();
        ce_shader.set("light_space_matrix", ce_light_space_matrix);
        ce_shader.set("PV",                 ce_projection_view);
        ce_shader.set("view_pos",           ce_view_pos);
        ce_shader.set("light_pos",          ce_light_pos);
        ce_shader.set("light_color",        ce_light_color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }

    glDisable(GL_CULL_FACE);
}

inline void ce_RenderCubes(void)
{
    glBindVertexArray(ce_vao_cube);

    ce_render(GL_TRIANGLES, vertex_array_cube, 36, render_array, render_count);
    render_count = 0;
}

inline void ce_RenderNoLight(int type, const Vertex* vertex_array, int vertex_count, const CE_Object* robj_array, int robj_count)
{
    // normal render:
    {
        glViewport(0, 0, platform.width, platform.height);

        ce_shader_no_light.Use();
        ce_shader_no_light.Set("PV", ce_projection_view);

        // render:
        ce_SetVertexArray(vertex_array, vertex_count);
        ce_SetRenderObjectArray(robj_array, robj_count);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }
}

inline void ce_RenderSquares(void)
{
    glBindVertexArray(ce_vao_cube);

    ce_RenderNoLight(GL_TRIANGLES, vertex_array_square, 6, render_array, render_count);
    render_count = 0;
}

inline void ce_PushCube(v3 pos, f32 scale, v4 color)
{
    CE_Object robj = {
        color,
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos.x,  pos.y,  pos.z,  1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_PushCubeQuat(v3 pos, f32 scale, Quat quat, v4 color)
{
    CE_Object robj = {
        color,
        // model:
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos.x,  pos.y,  pos.z,  1.0f
        }
    };

    robj.model = robj.model * m4_FromQuat(quat);

    ce_Push(robj);
}

inline void ce_PushBox(v3 pos, v3 rad, v4 color)
{
    CE_Object robj = {
        color,
        // model:
        {
            rad.x,  0.0f,   0.0f,   0.0f,
            0.0f,   rad.y,  0.0f,   0.0f,
            0.0f,   0.0f,   rad.z,  0.0f,
            pos.x,  pos.y,  pos.z,  1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_push_cube_rot_z(v3 pos, f32 scale, f32 rot, v4 color) {
    f32 s = sinf(rot);
    f32 c = cosf(rot);

    CE_Object robj = {
        color,
        {
             c * scale, s * scale,  0.0f,   0.0f,
            -s * scale, c * scale,  0.0f,   0.0f,
             0.0f,      0.0f,       scale,  0.0f,
             pos.x,     pos.y,      pos.z,  1.0f
        }
    };

    ce_push(robj);
}

inline void ce_PushCubeEuler(v3 pos, f32 scale, f32 yaw, f32 pitch, f32 roll, v4 color)
{
    f32 cy = cosf(yaw);
    f32 sy = sinf(yaw);
    f32 cp = cosf(pitch);
    f32 sp = sinf(pitch);
    f32 cr = cosf(roll);
    f32 sr = sinf(roll);

    CE_Object robj = {
        color,
        {
            scale * (cy * cp),                  scale * (sy * cp),                  scale * (-sp),          0.0f,
            scale * (cy * sp * sr - sy * cr),   scale * (sy * sp * sr + cy * cr),   scale * (cp * sr),      0.0f,
            scale * (cy * sp * cr + sy * sr),   scale * (sy * sp * cr - cy * sr),   scale * (cp * cr),      0.0f,
            pos.x,                              pos.y,                              pos.z,                  1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_PushCubeRotMat(v3 pos, f32 scale, m2 R, v4 color)
{
    CE_Object robj = {
        color,
        {
            R.x.x * scale,  R.x.y * scale,  0.0f,       0.0f,
            R.y.x * scale,  R.y.y * scale,  0.0f,       0.0f,
            0.0f,           0,              scale,      0.0f,
            pos.x,          pos.y,          pos.z,      1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_PushBoxRot(v3 pos, v3 rad, f32 rot, v4 color)
{
    f32 s = sinf(rot);
    f32 c = cosf(rot);

    CE_Object robj = {
        color,
        {
             c * rad.x, s * rad.x,  0.0f,   0.0f,
            -s * rad.y, c * rad.y,  0.0f,   0.0f,
             0.0f,      0.0f,       rad.z,  0.0f,
             pos.x,     pos.y,      pos.z,  1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_PushBoxRotMat(v3 pos, v3 rad, m2 R, v4 color)
{
    CE_Object robj = {
        color,
        {
            R.x.x * rad.x,  R.x.y * rad.x,  0.0f,   0.0f,
            R.y.x * rad.y,  R.y.y * rad.y,  0.0f,   0.0f,
            0.0f,           0.0f,           rad.z,  0.0f,
            pos.x,          pos.y,          pos.z,  1.0f
        }
    };

    ce_Push(robj);
}

inline void ce_PushLine(v2 p0, v2 p1, f32 z, f32 rad, v4 color)
{
    v2 line         = p1 - p0;
    f32 line_length = Len(line);

    v2 line_pos = p0 + 0.5f * line;

    v2  axis    = { 0.0f, -1.0f };
    f32 rot     = GetAngle(axis, line);
    v3  pos     = V3(line_pos, z);
    v3  scale   = V3(rad, 0.5f * line_length, rad);

    ce_PushBoxRot(pos, scale, rot, color);
}

inline void ce_Clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void ce_Init(void)
{
    ce_shader           = LoadShaderFromMemory(ce_shader_vertex_light, ce_shader_fragment_light, NULL);
    ce_shader_no_light  = LoadShaderFromMemory(ce_shader_vertex, ce_shader_fragment, NULL);

    // init opengl:
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
    }

    ce_shader.use();

    // setup vbo/vao:
    {
        glGenVertexArrays(1, &ce_vao_cube);
        glGenBuffers(1, &ce_vbo_cube);
        glGenBuffers(1, &ce_vbo_cube_data);

        glBindVertexArray(ce_vao_cube);

        // cube:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
            //
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            // position : normal : color
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, normal));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, color));
        }

        // cube models:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);

            // color:
            glEnableVertexAttribArray(4);

            glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof (CE_Object), (void*)(offsetof(CE_Object, color)));
            glVertexAttribDivisor(4, 1);

            // model:
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);

            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof (CE_Object), (void*)(offsetof(CE_Object, model) + 0 * sizeof (f32[4])));
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof (CE_Object), (void*)(offsetof(CE_Object, model) + 1 * sizeof (f32[4])));
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof (CE_Object), (void*)(offsetof(CE_Object, model) + 2 * sizeof (f32[4])));
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof (CE_Object), (void*)(offsetof(CE_Object, model) + 3 * sizeof (f32[4])));

            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
        }
    }

    // init shadow map stuff
    {
        ce_shader_depth = shader_load_from_memory(ce_depth_vertex, ce_depth_fragment, NULL);

        glGenFramebuffers(1, &ce_depth_fbo);
        // create depth texture
        glGenTextures(1, &ce_depth_map);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ce_depth_map, 0);

        glGenTextures(1, &ce_color_map);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach color texture as FBO's color buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ce_color_map, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ce_shader.use();
        ce_shader.set("shadow_map", 0);
        ce_shader.set("color_map",  1);
    }
}

inline void ce_bitmap_render_rect(f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 r, f32 g, f32 b, f32 a) {
    ce_push_box(
        { x, y, z }, 
        { 0.5f * sx, 0.5f * sy, 0.01 },
        { r, g, b, a });
}

inline void ce_push_ascii(unsigned char c, f32 px, f32 py, f32 pz, f32 x_scale, f32 y_scale, f32 r, f32 g, f32 b, f32 a) {
    u64 n = bitascii[c];

    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            u64 bit = j * 8 + i;

            if (n & (1ull << bit)) {
                ce_bitmap_render_rect(px + i * x_scale, py - j * y_scale, pz, x_scale, -y_scale, r, g, b, a);
            }
        }
    }
}

inline void ce_push_string(const char* str, v3 pos, v2 scale, v4 color) {
    for (int i = 0; str[i] != '\0'; i++)
        ce_push_ascii(str[i], pos.x + i * 8 * scale.x, pos.y, pos.z, scale.x, scale.y, color.r, color.g, color.b, color.a);
}

inline void ce_push_string_format(v3 pos, f32 rad, v4 color, const char* fmt, ...) {
    va_list list;
    char buffer[64];

    va_start(list, fmt);
    vsnprintf(buffer, 64, fmt, list);
    ce_push_string(buffer, pos, { rad, rad }, color);
    va_end(list);
}

inline v3 ce_get_world_position(int x, int y) {
    f64 identity[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    f64 projection_view[16];

    for (u32 i = 0; i < 16; ++i) {
        projection_view[i] = ce_projection_view.array[i];
    }

    GLint viewport[4];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetIntegerv(GL_VIEWPORT, viewport);
 
    winX = (f64)x;
    winY = (f64)viewport[3] - (f64)y;

    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
 
    f64 result[3];
    f4x4Unproject64(result, winX, winY, winZ, identity, projection_view, viewport);
 
    return V3(result[0], result[1], result[2]);
}

#endif // ATS_CUBE_ENGINE

// ======================================= TEXTURES ======================================== //

#if defined(ATS_PLATFORM_GLFW)

#ifdef STB_IMAGE_IMPLEMENTATION
#include "dep/stb_image.h" 

struct Image
{
    int     width;
    int     height;

    u32*    pixels;
};

inline Image LoadImageFromFile(const char* path)
{
    Image   image       = {};
    i32     channels    = 0;

    image.pixels        = (u32*)stbi_load(path, &image.width, &image.height, &channels, 0);

    assert(image.pixels);

    return image;
}

#endif

struct Texture
{
    u32     id;
    int     width;
    int     height;
};

inline Texture CreateTexture(void *pixels, int width, int height, int is_smooth)
{
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

inline void UpdateTexture(Texture* texture, void *pixels, int width, int height, int is_smooth)
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
inline Texture LoadTextureFromFile(const char *texture_path, int is_smooth)
{
    Texture         texture     = {};
    i32             channels    = 0;
    unsigned char*  pixels      = NULL;

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

#ifndef ATS_MODERN_OPENGL
inline void BindTexture(const Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);

    glMatrixMode(GL_MODELVIEW);
}
#endif

inline void DeleteTexture(Texture* texture)
{
    glDeleteTextures(1, &texture->id);
    
    memset(texture, 0, sizeof *texture);
}

#endif

// ============================================ RENDER (LEGACY OPENGL) ============================================ //

#if defined(ATS_PLATFORM_GLFW) && !defined(ATS_MODERN_OPENGL)

int cube_list;
int square_list;

inline void gl_InitShapes(void)
{
    square_list = glGenLists(1);
    cube_list   = glGenLists(1);

    {
        glNewList(square_list, GL_COMPILE);

        glBegin(GL_TRIANGLES);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, +1.0f, 0.0f);
        glVertex3f(+1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, +1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(-1.0f, +1.0f, 0.0f);

        glEnd();

        glEndList();
    }

    {
        glNewList(cube_list, GL_COMPILE);

        glBegin(GL_TRIANGLES);

        glNormal3f(0.0f, 0.0f, -1.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);

        glVertex3f(-1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);
        
        glNormal3f(1.0f, 0.0f, 0.0f);

        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);

        glNormal3f(0.0f, -1.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);

        glEnd();

        glEndList();
    }
}

inline void gl_InitBitmap(void);

inline void gl_Init(void) {
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

    gl_InitShapes();
    gl_InitBitmap();
}

inline void gl_RenderSquare(v3 pos, f32 rad, v4 color)
{
    glPushMatrix();

    glColor4fv(color.array);
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(rad, rad, 1.0f);

    glCallList(square_list);

    glPopMatrix();
}

inline void gl_RenderCube(v3 pos, f32 rad, v4 color)
{
    glPushMatrix();

    glColor4fv(color.array);
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(rad, rad, rad);

    glCallList(cube_list);

    glPopMatrix();
}

inline void gl_RenderCube(v3 pos, v3 rad, v4 color)
{
    glPushMatrix();

    glColor4fv(color.array);

    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(rad.x, rad.y, rad.z);

    glCallList(cube_list);

    glPopMatrix();
}

inline void gl_RenderLine(v3 a, v3 b, v4 color)
{
    glColor4fv(color.array);

    glLoadIdentity();
    glBegin(GL_LINES);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);

    glEnd();
}

inline void gl_RenderLineLoop2D(v3 pos, f32 rad, f32 rot, v4 color, const v2* point_array, int point_count)
{
    glLoadIdentity();
    glTranslatef(pos.x, pos.y, pos.z);
    glScalef(rad, rad, 1.0f);
    glRotatef(TO_DEG(rot), 0.0f, 0.0f, 1.0f);

    glColor4f(color.r, color.g, color.b, color.a);

    glBegin(GL_LINE_LOOP);

    glNormal3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < point_count; ++i) {
        v2 a = point_array[i];
        v2 b = point_array[(i + 1) & (point_count - 1)];

        glVertex3f(a.x, a.y, 0.0f);
        glVertex3f(b.x, b.y, 0.0f);
    }

    glEnd();
}

inline void gl_SetLightEmitter(int index, f32 bright, v3 p)
{
    f32 pos[4]  = { p.x, p.y, p.z, 1.0f };
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

inline void gl_SetLightEmitter(int index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic)
{
    f32 pos[4]  = { p.x, p.y, p.z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { color.r, color.g, color.b, 0.0f };
    u32 light   = GL_LIGHT0 + index;

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

inline void gl_SetLightDirected(int index, f32 bright, v3 pos)
{
    f32 d       = (f32)(1.0f / sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));
    f32 dir[4]  = { pos.x * d, pos.y * d, pos.z * d, 0.0f };
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

inline void gl_SetLightDirected(int index, v3 pos, v3 color)
{
    f32 d       = (f32)(1.0f / sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));
    f32 dir[4]  = { pos.x * d, pos.y * d, pos.z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { color.r, color.g, color.b, 0.0f };
    u32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

inline void gl_SetLightGlobalAmbient(v3 color)
{
    f32 v[4] = { color.r, color.g, color.b, 0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

inline m4 gl_GetProjectionMatrix(void)
{
    m4 projection;
    glGetFloatv(GL_PROJECTION_MATRIX, projection.array);
    return projection;
}

inline m4 gl_GetModelviewMatrix(void)
{
    m4 modelview;
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview.array);
    return modelview;
}

inline v3 gl_GetWorldPosition(int x, int y)
{
    GLint viewport[4];

    f64 modelview[16];
    f64 projection[16];

    GLfloat win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;
 
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f64)x;
    win_y = (f64)viewport[3] - (f64)y;

    glReadPixels(x, int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    F4x4Unproject64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    return V3(result[0], result[1], result[2]);
}

int bitmap_display_list[BITMAP_COUNT];

#define BITMAP_GETBIT(N, X, Y) ((u64(N)) & (1ull << ((u64(Y)) * 8ull + (u64(X)))))

inline void gl_InitBitmap(void)
{
    for (int i = 0; i < BITMAP_COUNT; ++i) {
        bitmap_display_list[i] = glGenLists(1);

        glNewList(bitmap_display_list[i], GL_COMPILE);

        uint64_t c = bitascii[i];

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

inline void gl_RenderAscii(u8 c, v3 pos, v2 scale)
{
    m4 T = m4_Translate(pos) * m4_Scale(V3(scale, 1.0f));

    glPushMatrix();

    glMultMatrixf(T.array);
    glCallList(bitmap_display_list[c]);

    glPopMatrix();
}

inline void gl_RenderString(const char *str, v3 pos, v2 scale, v4 color)
{
    glColor4f(color.r, color.g, color.b, color.a);

    for (int i = 0; str[i] != '\0'; i++) {
        gl_RenderAscii(str[i], pos + V3(i * scale.x, 0, 0), scale);
    }
}

inline void gl_RenderStringFormat(v3 pos, v2 scale, v4 color, const char* fmt, ...)
{
    va_list list;
    char    buffer[256];

    va_start(list, fmt);

    vsnprintf(buffer, 256, fmt, list);
    gl_RenderString(buffer, pos, scale, color);

    va_end(list);
}

#endif // ATS_MODERN_OPENGL

