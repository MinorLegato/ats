#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#ifndef static_assert
#define static_assert _Static_assert
#endif

#define MEMORY_KB   ((1024))
#define MEMORY_MB   ((1024) * (MEMORY_KB))
#define MEMORY_GB   ((1024) * (MEMORY_MB))

#define PI          (3.14159265359f)
#define TO_RAD_MUL  (0.01745329251f)
#define TO_DEG_MUL  (57.2957795131f)

#define TO_RAD(deg) ((deg) * TO_RAD_MUL)
#define TO_DEG(rad) ((rad) * TO_DEG_MUL)

#define SETBIT(n, b)     ((n)  |=  (1ull << (b)))
#define CLRBIT(n, b)     ((n)  &= ~(1ull << (b)))
#define CHKBIT(n, b)     (((n) & (1ull << (b))) == (1ull << (b)))

#define ARRAY_COUNT(arr) (sizeof (arr) / sizeof (arr[0]))

#define SWAP(T, a, b) do { T t = (a); (a) = (b); (b) = t; } while(0)
//
#ifndef MIN
#define MIN(a, b) (((a) < (b))? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b))? (a) : (b))
#endif

#define CLAMP_MAX(x, hi) MIN((x), (hi))
#define CLAMP_MIN(x, lo) MAX((x), (lo))
//
#define CLAMP(x, lo, hi) (((x) < (lo))? (lo) : (((x) > (hi))? (hi) : (x)))
//
#define CLAMP_0(x)  ((x) < 0.0f? 0.0f : (x))
#define CLAMP_1(x)  ((x) > 1.0f? 1.0f : (x))
#define CLAMP_01(x) (((x) < 0.0f)? 0.0f : ((x) > 1.0f? 1.0f : (x)))
//
#define SIGN(x)     ((x) < 0? -1 : 1)
#define SQUARE(x)   ((x) * (x))
#define IS_POS2(x)  (((x) != 0) && ((x) & ((x)-1)) == 0)
//
#define ALIGN_DOWN(n, a)      ((n) & ~((a) - 1))
#define ALIGN_UP(n, a)        ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a)  ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a)    ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))

#define allocItem(Type)                 ((Type*)malloc(sizeof (Type)))
#define allocArray(Type, count)         ((Type*)malloc((count) * sizeof (Type)))
#define reallocArray(ptr, Type, count)  ((Type*)realloc((ptr), (count) * sizeof (Type)))

#define ARRAY_GROW(ctx, array, len, cap, Type, new_cap) do { \
        (ctx)->array = (Type *)realloc((ctx)->array, sizeof (Type) * (new_cap)); \
        (ctx)->cap   = (new_cap); \
    } while (0);

#define ARRAY_CHECK_THEN_GROW(ctx, array, len, cap, Type, init_cap) do { \
        if ((ctx)->len == (ctx)->cap) \
            ARRAY_GROW(ctx, array, len, cap, Type, (ctx)->cap? ((ctx)->cap << 1) : init_cap); \
    } while (0);

// ===================================================== MATH =========================================== //

// ==================================================== 1D MATH ========================================== //

static float Sqrt(float number)
{
    float x = number * 0.5f;
    float y = number;
    int   i = *(int *)&y;

    i = 0x5f3759df - (i >> 1);
    y = * (float *) &i;
    y = y * (1.5f - (x * y * y));

    return number * y;
}

static float RSqrt(float number)
{
	float x2 = number * 0.5F;
	float y  = number;
	int i    = *(long *)&y;             // evil floating point bit level hacking

	i  = 0x5f3759df - (i >> 1);         // what the fuck? 
	y  = *(float *) &i;
	y  = y * (1.5f - (x2 * y * y));     // 1st iteration

	return y;
}

static float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

static float UnLerp(float a, float b, float t)
{
    return (t - a) / (b - a);
}

static float SmoothStep(float n)
{
	if (n < 0.0f) { return 0.0f; }
	if (n < 1.0f) { return (3.0f * n * n - 2.0f * n * n * n); }

	return 1.0f;
}

static float ShortAngleDist(float a, float b)
{
    float max = 2.0f * PI;
    float da  = fmodf(b - a, max);
    return fmodf(2.0f * da, max) - da;
}

static float LerpAngle(float a, float b, float t)
{
    return a + ShortAngleDist(a, b) * t;
}

static float Snap(float f, float step_size)
{
	if (f > 0)  { return (float)((int)(f / step_size + 0.5f)) * step_size; }
    else        { return (float)((int)(f / step_size - 0.5f)) * step_size; }
}

static float Spline(float f, float a, float b, float c, float d)
{
	float i = 1.0f - f;
	return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

static int IntOverlap(int a0, int a1, int b0, int b1)
{
    return MAX(a0, b0) - MAX(a1, b1);
}

// ========================================= ARRAY MATH ==================================== //

#define F2_ARG(arg)     (arg)[0], (arg)[1]
#define F3_ARG(arg)     (arg)[0], (arg)[1], (arg)[2]
#define F4_ARG(arg)     (arg)[0], (arg)[1], (arg)[2], (arg)[3]

#define FM2_ARG(arg)    F4_ARG(arg)

const float origo[4]  = { 0.0f, 0.0f, 0.0f, 0.0f };
const float axis_x[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
const float axis_y[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
const float axis_z[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
const float axis_w[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

// -------------------------------------------- 2D --------------------------------------- //

static void Float2Set(float *out, float x, float y)
{
    out[0] = x;
    out[1] = y;
}

static void Float2Copy(float *r, const float *a)
{
    r[0] = a[0];
    r[1] = a[1];
}

static void Float2Add(float *r, const float *a, const float *b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

static void Float2Sub(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

static void Float2Scale(float *r, const float *v, float s)
{
    r[0] = v[0] * s;
    r[1] = v[1] * s;
}

static void Float2Mul(float *r, const float *a, const float *b)
{
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
}

static float Float2Dot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1];
}

static float Float2Len(const float *v)
{
    return Sqrt(v[0] * v[0] + v[1] * v[1]);
}

static float Float2LenSq(const float *v)
{
    return v[0] * v[0] + v[1] * v[1];
}

static float Float2DistSq(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    return dx * dx + dy * dy;
}

static float Float2Dist(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];

    return Sqrt(dx * dx + dy * dy);
}

static void Float2Norm(float *r, const float *v)
{
    float k = RSqrt(v[0] * v[0] + v[1] * v[1]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
}

static void Float2Min(float *r, const float *a, const float *b)
{
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
}

static void Float2Max(float *r, const float *a, const float *b)
{
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
}

static void Float2Lerp(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
}

static void Float2SubNorm(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

static void Float2SubScale(float *r, const float *a, const float *b, float t)
{
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
}

static void Float2NormScale(float *r, const float *a, float t)
{
    float k = RSqrt(a[0] * a[0] + a[1] * a[1]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
}

static void Float2SubNormScale(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

static void Float2SAdd(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
}

static void Float2Ssub(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
}

static void Float2Zero(float *r)
{
    r[0] = 0;
    r[1] = 0;
}

static void Float2Neg(float *r, const float *a)
{
    r[0] = -a[0];
    r[1] = -a[1];
}

static void Float2Proj(float *r, const float *a, const float *b)
{
    float ab_bb = (a[0] * b[0] + a[1] * b[1]) / (b[0] * b[0] + b[1] * b[1]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
}

static int Float2CircleIntersect(const float *p0, float r0, const float *p1, float r1)
{
    float dx = p1[0] - p0[0];
    float dy = p1[1] - p0[1];

    float r = r0 + r1;

    return (dx * dx + dy * dy) < (r * r);
}

static int Float2SegmentIsIntersectingCircle(const float *start, const float *end, const float *pos, float rad)
{
    float a[2] = { start[0] - pos[0], start[1] - pos[1] };
    float b[2] = { end[0] - start[0], end[1] - start[1] };

    if ((a[0] * a[0] + a[1] * a[1]) > (b[0] * b[0] + b[1] * b[1]))
        return 0;

    float seg[2] = { end[0] - start[0], end[1] - start[1] };
    float cir[2] = { pos[0] - start[0], pos[1] - start[1] };

    float dot_sc = seg[0] * cir[0] + seg[1] * cir[1];

    if (dot_sc < 0.0f)
        return 0;

    float proj = dot_sc / (seg[0] * seg[0] + seg[1] * seg[1]);

    seg[0] *= proj;
    seg[1] *= proj;

    seg[0] = seg[0] - cir[0];
    seg[1] = seg[1] - cir[1];

    return (seg[0] * seg[0] + seg[1] * seg[1]) < (rad * rad);
}

// ------------------------------- vec3 ------------------------------------

static void Float3Set(float *out, float x, float y, float z)
{
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

static void Float3Copy(float *r, const float *a)
{
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
}

static void Float3Add(float *r, const float *a, const float *b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

static void Float3Sub(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

static void Float3Scale(float *r, const float *v, float s)
{
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
}

static float Float3Dot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float Float3Len(const float *v)
{
    return Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static float Float3LenSq(const float *v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

static float Float3DistSq(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];

    return dx * dx + dy * dy + dz * dz;
}

static float Float3Dist(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];

    return Sqrt(dx * dx + dy * dy + dz * dz);
}

static void Float3Norm(float *r, const float *v)
{
    float k = RSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
}

static void Float3Min(float *r, const float *a, const float *b)
{
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
}

static void Float3Max(float *r, const float *a, const float *b)
{
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
}

static void Float3Lerp(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
}

static void Float3SubNorm(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

static void Float3SubScale(float *r, const float *a, const float *b, float t)
{
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
}

static void Float3NormScale(float *r, const float *a, float t)
{
    float k = RSqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
}

static void Float3SubNormScale(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

static void Float3SAdd(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
}

static void Float3Ssub(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
    r[2] = a[2] - b[2] * t;
}

static void Float3Zero(float *r)
{
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
}

static void Float3Neg(float *r, const float *a)
{
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
}

static void Float3Proj(float *r, const float *a, const float *b)
{
    float ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
}

static int Float3CircleIntersect(const float *p0, float r0, const float *p1, float r1)
{
    float dx = p1[0] - p0[0];
    float dy = p1[1] - p0[1];
    float dz = p1[2] - p0[2];

    float r = r0 + r1;

    return (dx * dx + dy * dy + dz * dz) <  r * r;
}

// ------------------------------- vec4 ------------------------------------

static void Float4Set(float *out, float x, float y, float z, float w)
{
    out[0] = x;
    out[1] = y;
    out[2] = z;
    out[3] = w;
}

static void Float4Copy(float *r, const float *a)
{
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
    r[3] = a[3];
}

static void Float4Add(float *r, const float *a, const float *b)
{
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

static void Float4Sub(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}

static void Float4Mul(float *r, const float *a, const float *b)
{
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
    r[3] = a[3] * b[3];
}

static void Float4Scale(float *r, const float *v, float s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
    r[3] = v[3] * s;
}

static float Float4Dot(const float *a, const float *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

static float Float4Len(const float *v)
{
    return Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

static float Float4LenSq(const float *v)
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

static float Float4DistSq(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];
    float dw = b[3] - a[3];
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

static float Float4Dist(const float *a, const float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];
    float dw = b[3] - a[3];

    return Sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

static void Float4Norm(float *r, const float *v)
{
    float k = RSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
    r[3] = v[3] * k;
}

static void Float4Min(float *r, const float *a, const float *b)
{
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
    r[3] = a[3] < b[3] ? a[3] : b[3];
}

static void Float4Max(float *r, const float *a, const float *b)
{
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
    r[3] = a[3] > b[3] ? a[3] : b[3];
}

static void Float4Lerp(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
    r[3] = a[3] + t * (b[3] - a[3]);
}

static void Float4SubNorm(float *r, const float *a, const float *b)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

static void Float4SubScale(float *r, const float *a, const float *b, float t)
{
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
    r[3] = (a[3] - b[3]) * t;
}

static void Float4NormScale(float *r, const float *a, float t)
{
    float k = RSqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
    r[3] = a[3] * k;
}

static void Float4SubNormScale(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

static void Float4SAdd(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
    r[3] = a[3] + b[3] * t;
}

static void Float4Ssub(float *r, const float *a, const float *b, float t)
{
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
    r[2] = a[2] - b[2] * t;
    r[3] = a[3] - b[3] * t;
}

static void Float4Zero(float *r)
{
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
}

static void Float4Neg(float *r, const float *a)
{
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
    r[3] = -a[3];
}

static void Float4Proj(float *r, const float *a, const float *b)
{
    float ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2] + b[3] * b[3]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
    r[3] = b[3] * ab_bb;
}

// --------------------------------- vecN ---------------------------------- //

static float Float2Det(const float *a, const float *b)
{
    return a[0] * b[1] - b[0] * a[1];
}

static float Float2GetAngle(const float *a, const float *b)
{
    float det = a[0] * b[1] - b[0] * a[1];
    float dot = a[0] * b[0] + a[1] * b[1];

    return atan2f(det, dot);
}

static void Float2Rotate(float *r, const float *a, float rot)
{
    float c = cosf(rot);
    float s = sinf(rot);

    float b[2] = {
        a[0] * c - a[1] * s,
        a[0] * s + a[1] * c
    };

    r[0] = b[0];
    r[1] = b[1];
}

static void Float3Cross(float *r, const float *a, const float *b)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

static void Float3CrossNorm(float *r, const float *a, const float *b)
{
    r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];

    float k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

    r[0] *= k;
    r[1] *= k;
    r[2] *= k;
}

static void Float2Reflect(float *r, const float *v, const float *n)
{
	float p  = 2.0f * Float2Dot(v, n);

    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
}

static void Float3Reflect(float *r, const float *v, const float *n)
{
	float p  = 2.0f * Float3Dot(v, n);

    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
    r[2] = v[2] - p * n[2];
}

static void Float4Reflect(float *r, const float *v, const float *n)
{
	float p  = 2.0f * (v[0] * n[0] + v[1] * n[1] + v[2] * n[2] + v[3] * n[3]);
	r[0] = v[0] - p * n[0];
	r[1] = v[1] - p * n[1];
	r[2] = v[2] - p * n[2];
	r[3] = v[3] - p * n[3];
}

static void Float2Spline(float *out, float f, const float *a, const float *b, const float *c, const float *d)
{
	float i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
}

static void Float3Spline(float *out, float f, const float *a, const float *b, const float *c, const float *d)
{
	float i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
	out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
}

static void Float4Spline(float *out, float f, const float *a, const float *b, const float *c, const float *d)
{
	float i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
	out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
	out[3] = ((d[3] * f + c[3] * i) * f + (c[3] * f + b[3] * i) * i) * f + ((c[3] * f + b[3] * i) * f + (b[3] * f + a[3] * i) * i) * i;
}

static void Float2Flatten(float *out, const float *pos, const float *normal)
{
	float f = pos[0] * normal[0] + pos[1] * normal[1];

	out[0] = pos[0] - normal[0] * f;
	out[1] = pos[1] - normal[1] * f;
}

static void Float3Flatten(float *out, const float *pos, const float *normal)
{
	float f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];

	out[0] = pos[0] - normal[0] * f;
	out[1] = pos[1] - normal[1] * f;
	out[2] = pos[2] - normal[2] * f;
}

static void Float4Cross(float *r, const float *a, const float *b)
{
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
	r[3] = 1.f;
}

static void Float2LineCenter(float *out, const float *a, const float *b)
{
    out[0] = 0.5f * (a[0] + b[0]);
    out[1] = 0.5f * (a[1] + b[1]);
}

static void Float2GetLinesIntersectPoint(float *out, const float *a0, const float *a1, const float *b0, const float *b1)
{
    float x, y;

	x  = (a0[0] * a1[1]  -  a0[1] * a1[0]) * (b0[0] - b1[0]) - (b0[0] * b1[1] - b0[1] * b1[0]) * (a0[0] - a1[0]);
	x /= (a0[0] - a1[0]) * (b0[1] - b1[1]) - (a0[1] - a1[1]) * (b0[0] - b1[0]);
	y  = (a0[0] * a1[1]  -  a0[1] * a1[0]) * (b0[1] - b1[1]) - (b0[0] * b1[1] - b0[1] * b1[0]) * (a0[1] - a1[1]);
	y /= (a0[0] - a1[0]) * (b0[1] - b1[1]) - (a0[1] - a1[1]) * (b0[0] - b1[0]);

    out[0] = x;
    out[1] = y;
}

static int Float2LinesContainPoint(const float *point, const float *a0, const float *a1, const float *b0, const float *b1)
{
    float c0[2] = { 0.5f * (a0[0] + a1[0]), 0.5f * (a0[1] + a1[1]) };
    float c1[2] = { 0.5f * (b0[0] + b1[0]), 0.5f * (b0[1] + b1[1]) };

    float r0 = 0.5f * Float2Dist(a0, a1);
    float r1 = 0.5f * Float2Dist(b0, b1);

    return Float2DistSq(point, c0) < (r0 * r0) && Float2DistSq(point, c1) < (r1 * r1);
}

static int Float2LinesIntersectTest(const float *a0, const float *a1, const float *b0, const float *b1)
{
    float intersect[2];
    Float2GetLinesIntersectPoint(intersect, a0, a1, b0, b1);
    return Float2LinesContainPoint(intersect, a0, a1, b0, b1);
}

static void Float2Update(float *pos, float *vel, float fx, float fy, float t)
{
    vel[0] -= vel[0] * t * fx;
    vel[1] -= vel[1] * t * fy;
    pos[0] += vel[0] * t;
    pos[1] += vel[1] * t;
}

static void Float2Apply(float *vel, const float *acc, float t)
{
    vel[0] += acc[0] * t;
    vel[1] += acc[1] * t;
}

static void Float3Update(float *pos, float *vel, float friction, float t)
{
    float k = friction * t;
    
    vel[0] -= vel[0] * k;
    vel[1] -= vel[1] * k;
    vel[2] -= vel[2] * k;

    pos[0] += vel[0] * t;
    pos[1] += vel[1] * t;
    pos[2] += vel[2] * t;
}

static void Float3Apply(float *vel, const float *acc, float t)
{
    vel[0] += acc[0] * t;
    vel[1] += acc[1] * t;
    vel[2] += acc[2] * t;
}

#define F_EPSILON 0.000001f

static bool Float3RaycastTri(float orig[3], float dir[3], float vert0[3], float vert1[3], float vert2[3], float *t, float *u, float *v)
{
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	edge1[0] = vert1[0] - vert0[0];
	edge1[1] = vert1[1] - vert0[1];
	edge1[2] = vert1[2] - vert0[2]; 
	edge2[0] = vert2[0] - vert0[0];
	edge2[1] = vert2[1] - vert0[1];
	edge2[2] = vert2[2] - vert0[2];

	pvec[0] = dir[1] * edge2[2] - dir[2] * edge2[1];
	pvec[1] = dir[2] * edge2[0] - dir[0] * edge2[2];
	pvec[2] = dir[0] * edge2[1] - dir[1] * edge2[0];

	det = edge1[0] * pvec[0] + edge1[1] * pvec[1] + edge1[2] * pvec[2];

	if(det > -F_EPSILON && det < F_EPSILON) {
		return false;
    }

	inv_det = 1.0 / det;

	tvec[0] = orig[0] - vert0[0];
	tvec[1] = orig[1] - vert0[1];
	tvec[2] = orig[2] - vert0[2];

	*u = (tvec[0] * pvec[0] + tvec[1] * pvec[1] + tvec[2] * pvec[2]) * inv_det;

	if(*u < 0.0 || *u > 1.0) {
		return false;
    }

	qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
	qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
	qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];

	*v = (dir[0] * qvec[0] + dir[1] * qvec[1] + dir[2] * qvec[2]) * inv_det;

	if(*v < 0.0 || *u + *v > 1.0) {
		return false;
    }

	*t = (edge2[0] * qvec[0] + edge2[1] * qvec[1] + edge2[2] * qvec[2]) * inv_det;

	return true;
}


static bool Float3RaycastTriCull(float orig[3], float dir[3], float vert0[3], float vert1[3], float vert2[3], float *t, float *u, float *v)
{
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	edge1[0] = vert1[0] - vert0[0];
	edge1[1] = vert1[1] - vert0[1];
	edge1[2] = vert1[2] - vert0[2]; 
	edge2[0] = vert2[0] - vert0[0];
	edge2[1] = vert2[1] - vert0[1];
	edge2[2] = vert2[2] - vert0[2];

	pvec[0] = dir[1] * edge2[2] - dir[2] * edge2[1];
	pvec[1] = dir[2] * edge2[0] - dir[0] * edge2[2];
	pvec[2] = dir[0] * edge2[1] - dir[1] * edge2[0];

	det = edge1[0] * pvec[0] + edge1[1] * pvec[1] + edge1[2] * pvec[2];
 
	if(det < F_EPSILON) {
		return false;
    }

	tvec[0] = orig[0] - vert0[0];
	tvec[1] = orig[1] - vert0[1];
	tvec[2] = orig[2] - vert0[2];

	*u = tvec[0] * pvec[0] + tvec[1] * pvec[1] + tvec[2] * pvec[2];

	if(*u < 0.0 || *u > det) {
		return false;
    }

	qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
	qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
	qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];
	*v = dir[0] * qvec[0] + dir[1] * qvec[1] + dir[2] * qvec[2];

	if(*v < 0.0 || *u + *v > det) {
		return false;
    }

	*t = edge2[0] * qvec[0] + edge2[1] * qvec[1] + edge2[2] * qvec[2];
	inv_det = 1.0 / det;
	*t *= inv_det;
	*u *= inv_det;
	*v *= inv_det;

	return true;
}

// ---------------------------------------- matrix 2D --------------------------------------- //

static void Float2x2Identity(float *M)
{
    M[0] = 1;
    M[1] = 0;
    M[2] = 0;
    M[3] = 1;
}

static void Float2x2Mul(float *M, const float *a, const float *b)
{
    float T[4];

    T[0] = a[0] * b[0] + a[2] * b[1];
    T[1] = a[1] * b[0] + a[3] * b[1];
    T[2] = a[0] * b[2] + a[2] * b[3];
    T[3] = a[1] * b[2] + a[3] * b[3];

    M[0] = T[0];
    M[1] = T[1];
    M[2] = T[2];
    M[3] = T[3];
}

static void Float2x2Mulv(float *r, const float *R, const float *v)
{
    float t[2];

    t[0] = R[0] * v[0] + R[2] * v[1];
    t[1] = R[1] * v[0] + R[3] * v[1];

    r[0] = t[0];
    r[1] = t[1];
}

static void Float2x2Rotate(float *M, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    M[0] =  c;
    M[1] =  s;
    M[2] = -s;
    M[3] =  c;
}

static void Float2x2Scale(float *M, float sx, float sy) {
    M[0] = sx;
    M[1] = 0;
    M[2] = 0;
    M[3] = sy;
}

// ---------------------------------------- matrix 3D --------------------------------------- //

static void Float3x3Mul(float *R, const float *a, const float *b)
{
    float M[16];

    M[0] = a[0] * b[0] + a[3] * b[1]  + a[6] * b[2];
    M[1] = a[1] * b[0] + a[4] * b[1]  + a[7] * b[2];
    M[2] = a[2] * b[0] + a[5] * b[1]  + a[8] * b[2];

    M[3] = a[0] * b[3] + a[3] * b[4]  + a[6] * b[5];
    M[4] = a[1] * b[3] + a[4] * b[4]  + a[7] * b[5];
    M[5] = a[2] * b[3] + a[5] * b[4]  + a[8] * b[5];

    M[6] = a[0] * b[6] + a[3] * b[7]  + a[6] * b[8];
    M[7] = a[1] * b[6] + a[4] * b[7]  + a[7] * b[8];
    M[8] = a[2] * b[6] + a[5] * b[7]  + a[8] * b[8];

    R[0] = M[0];
    R[1] = M[1];
    R[2] = M[2];

    R[3] = M[3];
    R[4] = M[4];
    R[5] = M[5];

    R[6] = M[6];
    R[7] = M[7];
    R[8] = M[8];
}

static void Float3x3Mulv(float *out, const float *M, const float *v)
{
    float r[4];

	r[0] = M[0] * v[0] + M[3] * v[1] + M[6] * v[2];
	r[1] = M[1] * v[0] + M[4] * v[1] + M[7] * v[2];
	r[2] = M[2] * v[0] + M[5] * v[1] + M[8] * v[2];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}

static void Float3x3Rotate(float *R, const float *axis, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    float k = 1.0f - c;

    float sa[3]   = { s * axis[0], s * axis[1], s * axis[2] };
    float omca[3] = { k * axis[0], k * axis[1], k * axis[2] };

    R[0] = omca[0] * axis[0] + c;
    R[1] = omca[0] * axis[1] - sa[2];
    R[2] = omca[0] * axis[2] + sa[1],

    R[3] = omca[1] * axis[0] + sa[2];
    R[4] = omca[1] * axis[1] + c;
    R[5] = omca[1] * axis[2] - sa[0];

    R[6] = omca[2] * axis[0] - sa[1];
    R[7] = omca[2] * axis[1] + sa[0];
    R[8] = omca[2] * axis[2] + c;
}

static void Float3x3RotateEuler(float *R, float yaw, float pitch, float roll)
{
    float cy = cosf(yaw);
    float sy = sinf(yaw);
    float cp = cosf(pitch);
    float sp = sinf(pitch);
    float cr = cosf(roll);
    float sr = sinf(roll);

    R[0] =  cy * cp;
    R[1] = -cy * sp * sr - sy * cr;
    R[2] = -cy * sp * cr + sy * sr;

    R[3] =  sy * cp;
    R[4] = -sy * sp * sr + cy * cr;
    R[5] = -sy * sp * cr - cy * sr;

    R[6] = sp;
    R[7] = cp * sr;
    R[8] = cp * sr;
}

static void Float3x3RotateFloat3(float *out, const float *point, const float *axis, float angle)
{
    float R[9];

    Float3x3Rotate(R, axis, angle);
    Float3x3Mulv(out, R, point);
}

static void Float3x3FromQuat(float *M, const float *q)
{
    float a = q[3];
	float b = q[0];
	float c = q[1];
	float d = q[2];

	float a2 = a * a;
	float b2 = b * b;
	float c2 = c * c;
	float d2 = d * d;

	M[0] = a2 + b2 - c2 - d2;
	M[1] = 2.0f * (b * c + a * d);
	M[2] = 2.0f * (b * d - a * c);

	M[3] = 2.0f * (b * c - a * d);
	M[4] = a2 - b2 + c2 - d2;
	M[5] = 2.0f * (c * d + a * b);

	M[6] = 2.0f * (b * d + a * c);
	M[7] = 2.0f * (c * d - a * b);
	M[8] = a2 - b2 - c2 + d2;
}


// ---------------------------------------- matrix 4D --------------------------------------- //

static void Float4x4Identity(float *M)
{
	M[0]  = 1;
	M[1]  = 0;
	M[2]  = 0;
	M[3]  = 0;

	M[4]  = 0;
	M[5]  = 1;
	M[6]  = 0;
	M[7]  = 0;

	M[8]  = 0;
	M[8]  = 0;
	M[10] = 1;
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = 0;
	M[15] = 1;
}

static void Float4x4Copy(float *M, const float *N)
{
	M[0]  = N[0];
	M[1]  = N[1];
	M[2]  = N[2];
	M[3]  = N[3];

	M[4]  = N[4];
	M[5]  = N[5];
	M[6]  = N[6];
	M[7]  = N[7];

	M[8]  = N[8];
	M[9]  = N[9];
	M[10] = N[10];
	M[11] = N[11];

	M[12] = N[12];
	M[13] = N[13];
	M[14] = N[14];
	M[15] = N[15];
}

static void Float4x4Transpose(float *M, const float *N)
{
	M[0]  = N[0];
	M[4]  = N[1];
	M[8]  = N[2];
	M[12] = N[3];

	M[1]  = N[4];
	M[5]  = N[5];
	M[9]  = N[6];
	M[13] = N[7];

	M[2]  = N[8];
	M[6]  = N[9];
	M[10] = N[10];
	M[14] = N[11];

	M[3]  = N[12];
	M[7]  = N[13];
	M[11] = N[14];
	M[15] = N[15];
}

static void Float4x4Add(float *M, const float *a, const float *b)
{
    M[0]  = a[0]  + b[0];
    M[1]  = a[1]  + b[1];
    M[2]  = a[2]  + b[2];
    M[3]  = a[3]  + b[3];

    M[4]  = a[4]  + b[4];
    M[5]  = a[5]  + b[5];
    M[6]  = a[6]  + b[6];
    M[7]  = a[7]  + b[7];

    M[8]  = a[8]  + b[8];
    M[9]  = a[9]  + b[9];
    M[10] = a[10] + b[10];
    M[11] = a[11] + b[11];

    M[12] = a[12] + b[12];
    M[13] = a[13] + b[13];
    M[14] = a[14] + b[14];
    M[15] = a[15] + b[15];
}

static void Float4x4Sub(float *M, const float *a, const float *b)
{
    M[0]  = a[0]  - b[0];
    M[1]  = a[1]  - b[1];
    M[2]  = a[2]  - b[2];
    M[3]  = a[3]  - b[3];

    M[4]  = a[4]  - b[4];
    M[5]  = a[5]  - b[5];
    M[6]  = a[6]  - b[6];
    M[7]  = a[7]  - b[7];

    M[8]  = a[8]  - b[8];
    M[9]  = a[9]  - b[9];
    M[10] = a[10] - b[10];
    M[11] = a[11] - b[11];

    M[12] = a[12] - b[12];
    M[13] = a[13] - b[13];
    M[14] = a[14] - b[14];
    M[15] = a[15] - b[15];
}

static void Float4x4Scale(float *M, float x, float y, float z)
{
    M[0]  = x;
    M[1]  = 0.0f;
    M[2]  = 0.0f;
    M[3]  = 0.0f;

    M[4]  = 0.0f;
    M[5]  = y;
    M[6]  = 0.0f;
    M[7]  = 0.0f;

    M[8]  = 0.0f;
    M[9]  = 0.0f;
    M[10] = z;
    M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
    M[15] = 1.0f;
}

static void Float4x4ScaleAniso(float *M, const float* a, float x, float y, float z)
{
	M[0]  = a[0]  * x;
	M[1]  = a[1]  * x;
	M[2]  = a[2]  * x;
	M[3]  = a[3]  * x;

	M[4]  = a[4]  * y;
	M[5]  = a[5]  * y;
	M[6]  = a[6]  * y;
	M[7]  = a[7]  * y;

	M[8]  = a[8]  * z;
	M[9]  = a[9]  * z;
	M[10] = a[10] * z;
	M[11] = a[11] * z;

	M[12] = a[12];
	M[13] = a[13];
	M[14] = a[14];
	M[15] = a[15];
}

static void Float4x4Mul(float *R, const float *a, const float *b)
{
    float M[16];

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

static void Float4x4Mulv(float *out, const float *M, const float *v)
{
    float r[4];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
	r[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
    out[3] = r[3];
}

static void Float4x4Translate(float *T, float x, float y, float z)
{
    T[0]  = 1;
	T[1]  = 0;
	T[2]  = 0;
	T[3]  = 0;

	T[4]  = 0;
	T[5]  = 1;
	T[6]  = 0;
	T[7]  = 0;

	T[8]  = 0;
	T[9]  = 0;
	T[10] = 1;
	T[11] = 0;

	T[12] = x;
	T[13] = y;
	T[14] = z;
	T[15] = 1;
}

static void Float4x4RotateX(float *R, float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

	R[0]    = 1.0f;
    R[1]    = 0.0f;
    R[2]    = 0.0f;
    R[3]    = 0.0f;

	R[4]    = 0.0f;
    R[5]    = c;
    R[6]    = s;
    R[7]    = 0.0f;

	R[8]    = 0.0f;
    R[9]    = -s;
    R[10]   = c;
    R[11]   = 0.0f;

	R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

static void Float4x4RotateY(float *R, float angle)
{
	float s = sinf(angle);
	float c = cosf(angle);

	R[0]    = c;
    R[1]    = 0.0f;
    R[2]    = s;
    R[3]    = 0.0f;

	R[4]    = 0.0f;
    R[5]    = 1.0f;
    R[6]    = 0.0f;
    R[7]    = 0.0f;

	R[8]    = -s;
    R[9]    = 0.0f;
    R[10]   = c;
    R[11]   = 0.0f;

    R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

static void Float4x4RotateZ(float *R, float angle) 
{
	float s = sinf(angle);
	float c = cosf(angle);

	R[0]    = c;
    R[1]    = s;
    R[2]    = 0.0f;
    R[3]    = 0.0f;

	R[4]    = -s;
    R[5]    = c;
    R[6]    = 0.0f;
    R[7]    = 0.0f;

	R[8]    = 0.0f;
    R[9]    = 0.0f;
    R[10]   = 1.0f;
    R[11]   = 0.0f;

	R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

// Rodrigues rotation formula
static void Float4x4Rotate(float *M, const float* axis, float angle)
{
    float sa[3];
    float omca[3];

    float cosv      = cosf(angle);
    float sinv      = sinf(angle);
    float inv_cosv  = 1.0f - cosv;

    sa[0] = axis[0] * sinv;
    sa[1] = axis[1] * sinv;
    sa[2] = axis[2] * sinv;

    omca[0] = axis[0] * inv_cosv;
    omca[1] = axis[1] * inv_cosv;
    omca[2] = axis[2] * inv_cosv;

    M[0] = omca[0] * axis[0] + cosv;
    M[1] = omca[0] * axis[1] - sa[0];
    M[2] = omca[0] * axis[2] + sa[1];
    M[3] = 0.0f;

    M[4] = omca[1] * axis[0] + sa[2];
    M[5] = omca[1] * axis[1] + cosv;
    M[6] = omca[1] * axis[2] - sa[0],
    M[7] = 0.0f;

    M[8]  = omca[2] * axis[0] - sa[1];
    M[9]  = omca[2] * axis[1] + sa[0];
    M[10] = omca[2] * axis[2] + cosv;
    M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
    M[15] = 1.0f;
}

static void Float4x4Frustum(float *M, float l, float r, float b, float t, float n, float f)
{
	M[0] = 2.0f * n / (r - l);
	M[1] = 0.0f;
    M[2] = 0.0f;
    M[3] = 0.0f;

    M[4] = 0.0f;
	M[5] = 2.0f * n / (t - b);
	M[6] = 0.0f;
    M[7] = 0.0f;

	M[8]  =  (r + l) / (r - l);
	M[9]  =  (t + b) / (t - b);
	M[10] = -(f + n) / (f - n);
	M[11] = -1.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
	M[14] = -2.0f * (f * n) / (f - n);
	M[15] = 0.0f;
}

static void Float4x4Ortho(float *M, float l, float r, float b, float t, float n, float f)
{
	M[0] = 2.0f / (r - l);
	M[1] = 0.0f;
    M[2] = 0.0f;
    M[3] = 0.0f;

    M[4] = 0.0f;
	M[5] = 2.0f / (t - b);
	M[6] = 0.0f;
    M[7] = 0.0f;

    M[8]  = 0.0f;
    M[9]  = 0.0f;
	M[10] = -2.0f / (f - n);
	M[11] = 0.f;

	M[12] = -(r + l) / (r - l);
	M[13] = -(t + b) / (t - b);
	M[14] = -(f + n) / (f - n);
	M[15] = 1.0f;
}

static void Float4x4Perspective(float *M, float y_fov, float aspect, float n, float f)
{
	const float a = 1.0f / tanf(y_fov / 2.0f);

	M[0]  = a / aspect;
	M[1]  = 0.0f;
	M[2]  = 0.0f;
	M[3]  = 0.0f;

	M[4]  = 0.0f;
	M[5]  = a;
	M[6]  = 0.0f;
	M[7]  = 0.0f;

	M[8]  = 0.0f;
	M[9]  = 0.0f;
	M[10] = -((f + n) / (f - n));
	M[11] = -1.0f;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -((2.f * f * n) / (f - n));
	M[15] = 0.0f;
}

static void Float4x4LookAt(float* M, const float* eye, const float* center, const float* up)
{
	float f[3], s[3], t[3];

    Float3SubNorm(f, center, eye);
    Float3CrossNorm(s, f, up);
	Float3Cross(t, s, f);

	M[0]  =  s[0];
	M[1]  =  t[0];
	M[2]  = -f[0];
	M[3]  =   0.0f;

	M[4]  =  s[1];
	M[5]  =  t[1];
	M[6]  = -f[1];
	M[7]  =   0.0f;

	M[8]  =  s[2];
	M[9]  =  t[2];
	M[10] = -f[2];
	M[11] =   0.0f;

    M[12] = -(M[0] * eye[0] + M[4] * eye[1] + M[8]  * eye[2]);
    M[13] = -(M[1] * eye[0] + M[5] * eye[1] + M[9]  * eye[2]);
    M[14] = -(M[2] * eye[0] + M[6] * eye[1] + M[10] * eye[2]);
    M[15] = -(M[3] * eye[0] + M[7] * eye[1] + M[11] * eye[2] - 1.0f);
}

static void Float4x4PerspectiveView(float *PV, const float *pos, const float *look, const float *up, float fov, float aspect, float n, float f)
{
    float P[16], V[16];

    Float4x4Perspective(P, TO_RAD(fov), aspect, n, f),
    Float4x4LookAt(V, pos, look, up);
    Float4x4Mul(PV, P, V);
}

static void Float4x4OrthoView(float *PV, const float *pos, const float *look, const float *up, const float *rad, float n, float f)
{
    float P[16], V[16];

    Float4x4Ortho(P, -rad[0], +rad[0], -rad[1], +rad[1], n, f);
    Float4x4LookAt(V, pos, look, up);
    Float4x4Mul(PV, P, V);
}

static void Float4x4TranslateScale(float *M, float px, float py, float pz, float sx, float sy, float sz)
{
    M[0]  = sx;
    M[1]  = 0;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = 0;
    M[5]  = sy;
    M[6]  = 0;
    M[7]  = 0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

static void Float4x4TranslateScaleRotateZ(float *M, float px, float py, float pz, float sx, float sy, float sz, float angle)
{
    float s = sinf(angle);
    float c = cosf(angle);

    M[0]  = c * sx;
    M[1]  = s * sx;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = -s * sy;
    M[5]  =  c * sy;
    M[6]  =  0;
    M[7]  =  0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

static void Float4x4FromQuat(float *M, const float *q)
{
    float a = q[3];
	float b = q[0];
	float c = q[1];
	float d = q[2];

	float a2 = a * a;
	float b2 = b * b;
	float c2 = c * c;
	float d2 = d * d;

	M[0] = a2 + b2 - c2 - d2;
	M[1] = 2.0f * (b * c + a * d);
	M[2] = 2.0f * (b * d - a * c);
	M[3] = 0.f;

	M[4] = 2.0f * (b * c - a * d);
	M[5] = a2 - b2 + c2 - d2;
	M[6] = 2.0f * (c * d + a * b);
	M[7] = 0.f;

	M[8]  = 2.0f * (b * d + a * c);
	M[9]  = 2.0f * (c * d - a * b);
	M[10] = a2 - b2 - c2 + d2;
	M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
	M[15] = 1.0f;
}

// -------------------------------------------------- QUATERNIONS ------------------------------------------------ //

static void QuatIdentity(float *out)
{
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
}

static void QuatMake(float *out, float x, float y, float z, float angle)
{
    float inv_len = RSqrt((x * x) + (y * y) + (z * z));
    float s = inv_len * sinf(angle / 2.0f);

    out[0] = x * s;
    out[1] = y * s;
    out[2] = z * s;
    out[3] = cosf(angle / 2.0f);
}

static void QuatAdd(float *out, const float *a, const float *b)
{
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}

static void QuatConj(float *out, const float *q)
{
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] =  q[3];
}

static void QuatMul(float *out, const float *a, const float *b)
{
    float c[4];

    c[0] = a[1] * b[2] - a[2] * b[1] + a[3] * b[0] + b[3] * a[0];
    c[1] = a[2] * b[0] - a[0] * b[2] + a[3] * b[1] + b[3] * a[1];
    c[2] = a[0] * b[1] - a[1] * b[0] + a[3] * b[2] + b[3] * a[2];
    c[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];

    out[0] = c[0];
    out[1] = c[1];
    out[2] = c[2];
    out[3] = c[3];
}


// p = q * p * q^-1
static void QuatMulv(float *out, const float *quat, const float *point)
{
    float p[4] = { point[0], point[1], point[2], 0 };

    float conj[4];

    QuatConj(conj, quat);
    QuatMul(out, quat, p);
    QuatMul(p, out, conj);

    out[0] = p[0];
    out[1] = p[1];
    out[2] = p[2];
}

static void QuatRotate(float *r, const float *axis, float angle)
{
    float s = sinf(0.5f * angle), v[3] = { s * axis[0], s * axis[1], s * axis[2] };

    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = cosf(0.5f * angle);
}

// ============================================== RANDOM GENERATOR =========================================== //
// random number generator: xorshf96

typedef struct {
    unsigned    x;
    unsigned    y;
    unsigned    z;
} RndGen;

RndGen default_rnd_gen = { 123456789u, 362436069u, 521288629u };

static unsigned RndGenNext(RndGen* g)
{
    g->x  ^= g->x << 16;
    g->x  ^= g->x >> 5;
    g->x  ^= g->x << 1;

    unsigned t = g->x;

    g->x  = g->y;
    g->y  = g->z;
    g->z  = t ^ g->x ^ g->y;

    return g->z;
}

static int RandInt(int min, int max)
{
    return min + RndGenNext(&default_rnd_gen) % (max - min);
}

static float RandFloat(float min, float max)
{
    return min + ((float)RndGenNext(&default_rnd_gen) / (float)0xFFFFFFFF) * (max - min); 
}

static void Float2Rand(float *out, float min, float max)
{
    out[0] = RandFloat(-1.0f, 1.0f);
    out[1] = RandFloat(-1.0f, 1.0f);

    float k = RSqrt(out[0] * out[0] + out[1] * out[1]) * RandFloat(min, max);

    out[0] *= k;
    out[1] *= k;
}

static void Float2RandUnit(float *out)
{
    out[0] = RandFloat(-1.0f, 1.0f);
    out[1] = RandFloat(-1.0f, 1.0f);

    float k = RSqrt(out[0] * out[0] + out[1] * out[1]);

    out[0] *= k;
    out[1] *= k;
}

static void Float3Rand(float *out, float min, float max)
{
    out[0] = RandFloat(-1.0f, 1.0f);
    out[1] = RandFloat(-1.0f, 1.0f);
    out[2] = RandFloat(-1.0f, 1.0f);

    float k = RSqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]) * RandFloat(min, max);

    out[0] *= k;
    out[1] *= k;
    out[2] *= k;
}

static void Float3RandUnit(float *out)
{
    out[0] = RandFloat(-1.0f, 1.0f);
    out[1] = RandFloat(-1.0f, 1.0f);
    out[2] = RandFloat(-1.0f, 1.0f);

    float k = RSqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

    out[0] *= k;
    out[1] *= k;
    out[2] *= k;
}

static void Float2AddRand(float *out, float* u, float min, float max)
{
    float r[2];
    Float2Rand(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
}

static void Float3AddRand(float *out, float* u, float min, float max)
{
    float r[3];
    Float3Rand(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
    out[2] = u[2] + r[2];
}

static void QuatRand(float *quat)
{
    QuatMake(quat, RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-1.0f, 1.0f), RandFloat(-PI, PI));
}

// =============================================== HASH ================================================= //

// =========================================== HASH FUNCS ================================================ //

static int HashInt(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x =  (x >> 16) ^ x;
    return x;
}

static int UnhashInt(int x)
{
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x =  (x >> 16) ^ x;
    return x;
}

static int HashStr(const char* s)
{
    int hash = 0;

    while (*s != '\0')
        hash += *(s++);

    return HashInt(hash);
}

// ============================================================================================================================ //

static int HashToIndex(int hash, int key_count)
{
    assert(hash && "0 not allowed!!");
    return hash & (key_count - 1);
}

// ============================================== HASH TABLE linear probing! ================================================== //
// hash table implementation without any associated type, which instead is kept in one or more other arrays!

static int TableInsert(int hash, int *key_table, int key_count)
{
    int desired_index   = HashToIndex(hash, key_count);
    int index           = desired_index;

    while (key_table[index] != hash && key_table[index] > 0) {
        index = (index + 1) & (key_count - 1);
        
        if (index == desired_index) {
            assert("table is full!");
            return -1;
        }
    }

    key_table[index] = hash;

    return index;
}

static void TableRemove(int hash, int *key_table, int key_count)
{
    int start_index = HashToIndex(hash, key_count);
    int index       = start_index;

    while (key_table[index] != hash) {
        index = (index + 1) & (key_count - 1);

        if (index == start_index) {
            assert(0 && "element does not exist!");
            return;
        }
    }

    key_table[index] = -1;
}

static int TableFind(int hash, int *key_table, int key_count)
{
    int start_index = HashToIndex(hash, key_count);
    int index       = start_index;

    while (key_table[index] != 0) {
        if (key_table[index] == hash) { return index; }

        index = (index + 1) & (key_count - 1);

        if (index == start_index) { return -1; }
    }

    return -1;
}

static int TableDefragment(int *key_table, int key_count)
{
    // TODO: try to remove as many -1 elements as possible without breaking the table!
    (void)key_table;
    (void)key_count;

    return 1;
}

// ========================== INDEX TABLE linear probing! ========================== //
// works a little like a hash table but the user have to save the index to access to correct element later!

static int IndexInsert(int hash, int *index_table, int index_count)
{
    int index_start = HashToIndex(hash, index_count);
    int index       = index_start;

    while (index_table[index] != 0) {
        index = (index + 1) & (index - 1);

        if (index == index_start) {
            assert(0 && "element does not exist!");
            return -1;
        }
    }

    index_table[index] = 1;

    return index;
}

static void IndexRemove(int index, int *index_table)
{
    index_table[index] = 0;
}

// ===================================== CSTR ==================================== //

static void CStringCopy(char *dst, const char *src)
{
    while ((*(dst++) = *(src++)) != '\0');
}

static int CStringIsDel(char c, const char *del)
{
    while (*del != '\0')
        if (c == *(del++)) return 1;
    return 0;
}

static char **CStringSplitInPlace(char *str, const char *del, size_t *count)
{
    size_t  split_capacity  = 8;
    size_t  split_count     = 0;
    char    **split_array   = malloc(split_capacity * sizeof *split_array);

    while (*str != '\0') {
        while (*str && CStringIsDel(*str, del)) { *(str++) = '\0'; }

        if (!*str) break;

        if (split_count + 1 >= split_capacity) {
            split_capacity  <<= 1;
            split_array     = realloc(split_array, split_capacity * sizeof *split_array);
        }

        split_array[split_count++] = str;

        while (*str && !CStringIsDel(*(++str), del));

        if (!*str) break;

        *(str++) = '\0';
    }

    *count = split_count;

    return split_array;
}

static int CStringFind(const char *str, const char *item)
{
    int item_len = 0;
    while (item[item_len] != '\0')
        item_len++;

    int end = item_len - 1;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i + end] != item[end]) {
            i += end;
            continue;
        }

        int j = 0;

        while (item[j] != '\0' && str[j + i] == item[j])
            ++j;

        if (item[j] == '\0')
            return i;
    }

    return -1;
}

// ==================================== FILES ==================================== //

static size_t FileGetSize(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

static char *FileReadStr(const char *file_name)
{
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0) {
        size_t size = FileGetSize(fp);

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

static int FileWriteStr(const char *file_name, const char *buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return 0;
}

static int FileAppendStr(const char *file_name, const char *buffer)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);

        fclose(fp);

        return n == size;
    }

    return 0;
}

static int FileReadBin(const char *file_name, void *buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return 0;
} 

static int FileWriteBin(const char *file_name, const void *buffer, size_t size)
{
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return 0;
}

