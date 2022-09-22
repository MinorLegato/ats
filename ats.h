#pragma once

#ifndef __ATS_H__
#define __ATS_H__

#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// ======================================== API-MACROS ======================================= //

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define PI (3.14159265359f)
#define TAU (6.28318530718f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define Swap(type, a, b) do { type tmp = (a); (a) = (b); (b) = tmp; } while(0)

#define ToRad(deg) ((deg) * TO_RAD_MUL)
#define ToDeg(rad) ((rad) * TO_DEG_MUL)

#define ArrayCount(array) (sizeof (array) / sizeof (array)[0])

#define IsPowerOfTwo(x) (((x) != 0) && ((x) & ((x) - 1)) == 0)

#define AlignDown(n, a)     ((n) & ~((a) - 1))
#define AlignUp(n, a)       AlignDown((n) + (a) - 1, (a))
#define AlignDownPtr(p, a)  ((void*)AlignDown((uintptr_t)(p), (a)))
#define AlignUpPtr(p, a)    ((void*)AlignDown((uintptr_t)(p), (a)))

#define join_helper(a, b) a##b
#define join_token(a, b) join_helper(a, b)
#define macro_var(a) join_token(a, __LINE__)

#define defer(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_range(index, start, below) \
  for (isize index = (start); index < (below); ++index)

#define for_array(index, array) \
  for (usize index = 0; index < ArrayCount(array); ++index)

#define repeat(count) \
  for (isize macro_var(index) = 0; macro_var(index) < (count); ++macro_var(index))

#define for_iter(iter_type, iter_name, ...) \
  for (struct iter_type iter_name = (__VA_ARGS__); \
       iter_type##_is_valid(&iter_name); \
       iter_type##_advance(&iter_name))

#define for_each(type, var, ...) \
  for (struct type* var = (struct type*)0xdeadbeefull; var != NULL; var = NULL) \
  for (struct type##_iter macro_var(it) = (__VA_ARGS__); \
       (var = macro_var(it).current, type##_iter_is_valid(&macro_var(it))); \
       type##_iter_advance(&macro_var(it)))

#define for_r2(rect, ix, iy) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_r3(rect, ix, iy, iz) \
  for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define ClampMin(n, min)    ((n) < (min)? (min) : (n))
#define ClampMax(n, max)    ((n) > (max)? (max) : (n))
#define Clamp(n, min, max)  ClampMax(ClampMin(n, min), max)
#define Clamp01(n)          Clamp(n, 0, 1)
#define Min(a, b)           ((a) < (b)? (a) : (b))
#define Max(a, b)           ((a) > (b)? (a) : (b))
#define Lerp(a, b, t)       ((a) + (f32)(t) * ((b) - (a)))
#define Sign(n)             ((n) == 0? 0 : ((n) < 0? -1 : 1))


// =========================================== API-TYPES ====================================== //

typedef float f32;
typedef double f64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned int uint;
typedef long long isize;
typedef unsigned long long usize;

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

// =========================================== TYPES ========================================= //

typedef union {
  struct { f32 x, y; };
  f32 e[2];
} v2;

typedef union {
  struct { f32 x, y, z; };
  struct { f32 r, g, b; };
  struct { v2 xy; };
  f32 e[3];
} v3;

typedef union {
  struct { f32 x, y, z, w; };
  struct { f32 r, g, b, a; };
  struct { v3 rgb; };
  struct { v3 xyz; };
  f32 e[4];
} v4;

typedef union {
  struct { i32 x, y; };
  i32 e[2];
} v2i;

typedef union {
  struct { i32 x, y, z; };
  struct { v2i xy; };
  i32 e[3];
} v3i;

typedef union {
  struct { i32 x, y, z, w; };
  i32 e[4];
} v4i;

typedef union {
  struct { v2 x, y; };
  f32 e[4];
} m2;

typedef union {
  struct { v3 x, y, z; };
  f32 e[9];
} m3;

typedef union {
  struct { v4 x, y, z, w; };
  f32 e[16];
} m4;

typedef struct {
  v2 min;
  v2 max;
} r2;

typedef struct {
  v3 min;
  v3 max;
} r3;

typedef struct {
  v2i min;
  v2i max;
} r2i;

typedef struct {
  v3i min;
  v3i max;
} r3i;

typedef struct quat {
  f32 x, y, z, w;
} quat_t;

typedef struct circle {
  v2 pos;
  f32 rad;
} circle_t;

typedef struct sphere {
  v3 pos;
  f32 rad;
} sphere_t;

// ======================================= STATIC FUNCTIONS ==================================== //

#ifndef __cplusplus // C stuff

#define ATS_INIT {0}

#define Make(T) (T)

#define V2(...) ((v2) { __VA_ARGS__ })
#define V3(...) ((v3) { __VA_ARGS__ })
#define V4(...) ((v4) { __VA_ARGS__ })

#define V2i(...) ((v2i) { __VA_ARGS__ })
#define V3i(...) ((v3i) { __VA_ARGS__ })
#define V4i(...) ((v4i) { __VA_ARGS__ })

#define Quat(...) ((quat_t) { __VA_ARGS__ })

#define R2(...) ((r2) { __VA_ARGS__ })
#define R3(...) ((r3) { __VA_ARGS__ })

#define R2i(...) ((r2i) { __VA_ARGS__ })
#define R3i(...) ((r3i) { __VA_ARGS__ })

#define Circle(...) ((circle_t) { __VA_ARGS__ })
#define Sphere(...) ((sphere_t) { __VA_ARGS__ })

#define M2(...) ((m2) { __VA_ARGS__ })
#define M3(...) ((m3) { __VA_ARGS__ })
#define M4(...) ((m4) { __VA_ARGS__ })

#else // __cplusplus

#define ATS_INIT {}

#define Make(T) T

static inline v2 V2(f32 x, f32 y)               { return { x, y }; }
static inline v3 V3(f32 x, f32 y, f32 z)        { return { x, y, z }; }
static inline v4 V4(f32 x, f32 y, f32 z, f32 w) { return { x, y, z, w }; }

static inline v2i V2i(i32 x, i32 y)               { return { x, y }; }
static inline v3i V3i(i32 x, i32 y, i32 z)        { return { x, y, z }; }
static inline v4i V4i(i32 x, i32 y, i32 z, i32 w) { return { x, y, z, w }; }

static inline quat_t Quat(f32 x, f32 y, f32 z, f32 w) { return { x, y, z, w }; }

static inline r2 R2(f32 ax, f32 ay, f32 bx, f32 by) { return { { ax, ay }, { bx, by } }; }
static inline r2 R2(v2 a, v2 b) { return { a, b }; }

static inline r3 R3(f32 ax, f32 ay, f32 az, f32 bx, f32 by, f32 bz) { return { { ax, ay, az }, { bx, by, bz } }; }
static inline r3 R3(v3 a, v3 b) { return { a, b }; }

static inline r2i R2i(i32 ax, i32 ay, i32 bx, i32 by) { return { { ax, ay }, { bx, by } }; }
static inline r2i R2i(v2i a, v2i b) { return { a, b }; }

static inline r3i R3i(i32 ax, i32 ay, i32 az, i32 bx, i32 by, i32 bz) { return { { ax, ay, az }, { bx, by, bz } }; }
static inline r3i R3i(v3i a, v3i b)  { return { a, b }; }

static inline circle_t Circle(f32 x, f32 y, f32 rad) { return { x, y, rad }; }
static inline circle_t Circle(v2 p, f32 rad) { return { p, rad }; }

static inline sphere_t Sphere(f32 x, f32 y, f32 z, f32 rad) { return { x, y, z, rad }; }
static inline sphere_t Sphere(v3 p, f32 rad) { return { p, rad }; }

#define M2(...) (m2 { __VA_ARGS__ })
#define M3(...) (m3 { __VA_ARGS__ })
#define M4(...) (m4 { __VA_ARGS__ })

#endif

static m2
m2_identity(void) {
  return M2(
    1, 0,
    0, 1);
}

static m3
m3_identity(void) {
  return M3(
    1, 0, 0,
    0, 1, 0,
    0, 0, 1);
}

static m4
m4_identity(void) {
  return M4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);
}

static quat_t
quat_identity() {
  return Quat(0, 0, 0, 1);
}

static f32
sqrt32(f32 n) {
  f32 x = n * 0.5f;
  f32 y = n;
  int i = *(int*)&y;

  i = 0x5f3759df - (i >> 1);
  y = *(f32*)&i;
  y = y * (1.5f - (x * y * y));

  return n * y;
}

static f32
rsqrt32(f32 n) {
  f32 x2 = n * 0.5f;
  f32 y  = n;
  int i  = *(long*)&y;           // evil floating point bit level hacking

  i = 0x5f3759df - (i >> 1);     // what the fuck? 
  y = *(f32*) &i;
  y = y * (1.5f - (x2 * y * y)); // 1st iteration

  return y;
}

static f32
shortest_angle_distance(f32 a, f32 b) {
  f32 max = 2.0f * PI;
  f32 da  = fmodf(b - a, max);

  return fmodf(2.0f * da, max) - da;
}

static f32
lerp_angle(f32 a, f32 b, f32 t) {
  return a + shortest_angle_distance(a, b) * t;
}

static f32
sine_ease_in(f32 t) {
  return 1 - cosf((t * PI) / 2);
}

static f32
sine_ease_out(f32 t) {
  return sinf((t * PI) / 2);
}

static f32
sine_ease_in_out(f32 t) {
  return -0.5 * (cosf(PI * t) - 1);
}

static f32
quad_ease_in(f32 t) {
  return t * t;
}

static f32
quad_ease_out(f32 t) {
  return 1 - (1 - t) * (1 - t);
}

static f32
quad_ease_in_out(f32 t) {
  f32 k = -2 * t + 2;
  return (t < 0.5)? (2 * t * t) : (1 - 0.5 * k * k);
}

static f32
cubic_ease_in(f32 t) {
  return t * t * t;
}

static f32
cubic_ease_out(f32 t) {
  f32 k = 1 - t;
  return 1 - k * k * k;
}

static f32
cubic_ease_in_out(f32 t) {
  f32 k = -2 * t + 2;
  return (t < 0.5)? (4 * t * t * t) : (1 - 0.5 * k * k * k);
}

static f32
quart_ease_in(f32 t) {
  return t * t * t * t;
}

static f32
quart_ease_out(f32 t) {
  f32 k = 1 - t; 
  return 1 - k * k * k * k;
}

static f32
quart_ease_in_out(f32 t) {
  f32 k = -2 * t + 2;
  return (t < 0.5)? (8 * t * t * t * t) : (1 - 0.5 * k * k * k * k);
}

static f32
quint_ease_in(f32 t) {
  return t * t * t * t * t;
}

static f32
quint_ease_out(f32 t) {
  f32 k = 1 - t;
  return 1 - k * k * k * k * k;
}

static f32
quint_ease_in_out(f32 t) {
  f32 k = -2 * t + 2;
  return (t < 0.5)? (16 * t * t * t * t * t) : (1 - 0.5 * k * k * k * k * k);
}

static f32
expo_ease_in(f32 t) {
  return (t == 0)? 0 : powf(2, 10 * t - 10);
}

static f32
expo_ease_out(f32 t) {
  return (t == 1)? 1 : (1 - powf(2, -10 * t));
}

static f32
expo_ease_in_out(f32 t) {
  return (t == 0)? 0 : (t == 1)? 1 : t < 0.5? powf(2, 20 * t - 10) / 2 : (2 - powf(2, -20 * t + 10)) / 2;
}

static f32
circ_ease_in(f32 t) {
  return 1 - sqrt32(1 - (t * t));
}

static f32
circ_ease_out(f32 t) {
  return sqrt32(1 - (t - 1) * (t - 1));
}

static f32
circ_ease_in_out(f32 t) {
  f32 k = 2 * t;
  f32 l = -2 * t + 2;
  return (t < 0.5)? 0.5 * (1 - sqrt32(1 - k * k)) : 0.5 * (sqrt32(1 - l * l) + 1);
}

static f32
back_ease_in(f32 t) {
  f32 c1 = 1.70158;
  f32 c3 = c1 + 1;
  return c3 * t * t * t - c1 * t * t;
}

static f32
back_ease_out(f32 t) {
  f32 c1 = 1.70158;
  f32 c3 = c1 + 1;
  f32 k = t - 1;
  return 1 + c3 * k * k * k + c1 * k * k;
}

static f32
back_ease_in_out(f32 t) {
  f32 c1 = 1.70158;
  f32 c2 = c1 * 1.525;

  return (t < 0.5)?
    0.5 * (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) :
    0.5 * (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2);
}

static f32
elastic_ease_in(f32 t) {
  f32 c4 = (2 * PI) / 3;

  return (t == 0)?
    0 :
    (t == 1)?
    1 :
    -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4);
}

static f32
elastic_ease_out(f32 t) {
  f32 c4 = (2 * PI) / 3;
  return t == 0?
    0 :
    t == 1?
    1 :
    powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
}

static f32
elastic_ease_inout(f32 t) {
  f32 c5 = (2 * PI) / 4.5;
  return t == 0?
    0 :
    t == 1?
    1 :
    t < 0.5 ?
    -0.5 * (powf(2, 20 * t - 10)  * sinf((20 * t - 11.125) * c5)) :
    +0.5 * (powf(2, -20 * t + 10) * sinf((20 * t - 11.125) * c5)) + 1;
}

static f32
bounce_ease_out(f32 t) {
  f32 n1 = 7.5625;
  f32 d1 = 2.75;
  if (t < 1 / d1) {
    return n1 * t * t;
  } else if (t < 2 / d1) {
    t -= 1.5 / d1;
    return n1 * t * t + 0.75;
  } else if (t < 2.5 / d1) {
    t -= 2.25 / d1;
    return n1 * t * t + 0.9375;
  } else {
    t -= 2.625 / d1;
    return n1 * t * t + 0.984375;
  }
}

static f32
bounce_ease_in(f32 t) {
  return 1 - bounce_ease_out(t);
}

static f32
bounce_ease_in_out(f32 t) {
  return t < 0.5?
    0.5 * (1 - bounce_ease_out(1 - 2 * t)) :
    0.5 * (1 + bounce_ease_out(2 * t - 1));
}

// ---------- from array ---------- //

static v2 v2_from_array(const f32* a) { return V2(a[0], a[1]); }
static v3 v3_from_array(const f32* a) { return V3(a[0], a[1], a[2]); }
static v4 v4_from_array(const f32* a) { return V4(a[0], a[1], a[2], a[3]); }

static v2i v2i_from_array(const i32* a) { return V2i(a[0], a[1]); }
static v3i v3i_from_array(const i32* a) { return V3i(a[0], a[1], a[2]); }
static v4i v4i_from_array(const i32* a) { return V4i(a[0], a[1], a[2], a[3]); }

// ---------- unpack color ------------ //

static v3
v3_unpack_color(u32 color) {
  return V3(
    ((color & 0x000000ff) >> 0)  / 256.0f,
    ((color & 0x0000ff00) >> 8)  / 256.0f,
    ((color & 0x00ff0000) >> 16) / 256.0f);
}

static v4
v4_unpack_color(u32 color) {
  return V4(
    ((color & 0x000000ff) >> 0)  / 256.0f,
    ((color & 0x0000ff00) >> 8)  / 256.0f,
    ((color & 0x00ff0000) >> 16) / 256.0f,
    ((color & 0xff000000) >> 24) / 256.0f);
}

// --------- negate ---------- //

static v2 v2_neg(v2 u) { return V2(-u.x, -u.y); }
static v3 v3_neg(v3 u) { return V3(-u.x, -u.y, -u.z); }
static v4 v4_neg(v4 u) { return V4(-u.x, -u.y, -u.z, -u.w); }

static v2i v2i_neg(v2i u) { return V2i(-u.x, -u.y); }
static v3i v3i_neg(v3i u) { return V3i(-u.x, -u.y, -u.z); }
static v4i v4i_neg(v4i u) { return V4i(-u.x, -u.y, -u.z, -u.w); }

#ifdef __cplusplus

static v2 operator-(v2 u) { return { -u.x, -u.y }; }
static v3 operator-(v3 u) { return { -u.x, -u.y, -u.z }; }
static v4 operator-(v4 u) { return { -u.x, -u.y, -u.z, -u.w }; }

static v2i operator-(v2i u) { return { -u.x, -u.y }; }
static v3i operator-(v3i u) { return { -u.x, -u.y, -u.z }; }
static v4i operator-(v4i u) { return { -u.x, -u.y, -u.z, -u.w }; }

#endif

// ---------- addition ---------- //

static v2 v2_add(v2 a, v2 b) { return V2(a.x + b.x, a.y + b.y); }
static v3 v3_add(v3 a, v3 b) { return V3(a.x + b.x, a.y + b.y, a.z + b.z); }
static v4 v4_add(v4 a, v4 b) { return V4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static v2i v2i_add(v2i a, v2i b) { return V2i(a.x + b.x, a.y + b.y); }
static v3i v3i_add(v3i a, v3i b) { return V3i(a.x + b.x, a.y + b.y, a.z + b.z); }
static v4i v4i_add(v4i a, v4i b) { return V4i(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

#ifdef __cplusplus

static v2 operator+(v2 a, v2 b) { return { a.x + b.x, a.y + b.y }; }
static v3 operator+(v3 a, v3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
static v4 operator+(v4 a, v4 b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }

static v2i operator+(v2i a, v2i b) { return { a.x + b.x, a.y + b.y }; }
static v3i operator+(v3i a, v3i b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
static v4i operator+(v4i a, v4i b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }

static v2& operator+=(v2& a, v2 b) { a = a + b; return a; }
static v3& operator+=(v3& a, v3 b) { a = a + b; return a; }
static v4& operator+=(v4& a, v4 b) { a = a + b; return a; }

static v2i& operator+=(v2i& a, v2i b) { a = a + b; return a; }
static v3i& operator+=(v3i& a, v3i b) { a = a + b; return a; }
static v4i& operator+=(v4i& a, v4i b) { a = a + b; return a; }

#endif

// -------- subtraction ------- //

static v2 v2_sub(v2 a, v2 b) { return V2(a.x - b.x, a.y - b.y); }
static v3 v3_sub(v3 a, v3 b) { return V3(a.x - b.x, a.y - b.y, a.z - b.z); }
static v4 v4_sub(v4 a, v4 b) { return V4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static v2i v2i_sub(v2i a, v2i b) { return V2i(a.x - b.x, a.y - b.y); }
static v3i v3i_sub(v3i a, v3i b) { return V3i(a.x - b.x, a.y - b.y, a.z - b.z); }
static v4i v4i_sub(v4i a, v4i b) { return V4i(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

#ifdef __cplusplus

static v2 operator-(v2 a, v2 b) { return { a.x - b.x, a.y - b.y }; }
static v3 operator-(v3 a, v3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
static v4 operator-(v4 a, v4 b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

static v2i operator-(v2i a, v2i b) { return { a.x - b.x, a.y - b.y }; }
static v3i operator-(v3i a, v3i b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
static v4i operator-(v4i a, v4i b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

static v2& operator-=(v2& a, v2 b) { a = a - b; return a; }
static v3& operator-=(v3& a, v3 b) { a = a - b; return a; }
static v4& operator-=(v4& a, v4 b) { a = a - b; return a; }

static v2i& operator-=(v2i& a, v2i b) { a = a - b; return a; }
static v3i& operator-=(v3i& a, v3i b) { a = a - b; return a; }
static v4i& operator-=(v4i& a, v4i b) { a = a - b; return a; }

#endif

// -------- multiplication ------- //

static v2 v2_mul(v2 a, v2 b) { return V2(a.x * b.x, a.y * b.y); }
static v3 v3_mul(v3 a, v3 b) { return V3(a.x * b.x, a.y * b.y, a.z * b.z); }
static v4 v4_mul(v4 a, v4 b) { return V4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * a.w); }

static v2i v2i_mul(v2i a, v2i b) { return V2i(a.x * b.x, a.y * b.y); }
static v3i v3i_mul(v3i a, v3i b) { return V3i(a.x * b.x, a.y * b.y, a.z * a.z); }
static v4i v4i_mul(v4i a, v4i b) { return V4i(a.x * b.x, a.y * b.y, a.z * a.z, a.w * a.w); }

static v2
m2_mulv(m2 m, v2 u) {
  return V2(
    m.e[0] * u.x + m.e[2] * u.y,
    m.e[1] * u.x + m.e[3] * u.y);
}

static v3
m3_mulv(m3 m, v3 u) {
  return V3(
    m.e[0] * u.x + m.e[3] * u.y + m.e[6] * u.z,
    m.e[1] * u.x + m.e[4] * u.y + m.e[7] * u.z,
    m.e[2] * u.x + m.e[5] * u.y + m.e[8] * u.z);
}

static v4
m4_mulv(m4 m, v4 u) {
  return V4(
    m.e[0] * u.x + m.e[4] * u.y + m.e[8]  * u.z + m.e[12] * u.w,
    m.e[1] * u.x + m.e[5] * u.y + m.e[9]  * u.z + m.e[13] * u.w,
    m.e[2] * u.x + m.e[6] * u.y + m.e[10] * u.z + m.e[14] * u.w,
    m.e[3] * u.x + m.e[7] * u.y + m.e[11] * u.z + m.e[15] * u.w);
}

static m2
m2_mul(m2 a, m2 b) {
  return M2(
    a.e[0] * b.e[0] + a.e[2] * b.e[1],
    a.e[1] * b.e[0] + a.e[3] * b.e[1],
    a.e[0] * b.e[2] + a.e[2] * b.e[3],
    a.e[1] * b.e[2] + a.e[3] * b.e[3]);
}

static m3
m3_mul(m3 a, m3 b) {
  return M3(
    a.e[0] * b.e[0] + a.e[3] * b.e[1]  + a.e[6] * b.e[2],
    a.e[1] * b.e[0] + a.e[4] * b.e[1]  + a.e[7] * b.e[2],
    a.e[2] * b.e[0] + a.e[5] * b.e[1]  + a.e[8] * b.e[2],

    a.e[0] * b.e[3] + a.e[3] * b.e[4]  + a.e[6] * b.e[5],
    a.e[1] * b.e[3] + a.e[4] * b.e[4]  + a.e[7] * b.e[5],
    a.e[2] * b.e[3] + a.e[5] * b.e[4]  + a.e[8] * b.e[5],

    a.e[0] * b.e[6] + a.e[3] * b.e[7]  + a.e[6] * b.e[8],
    a.e[1] * b.e[6] + a.e[4] * b.e[7]  + a.e[7] * b.e[8],
    a.e[2] * b.e[6] + a.e[5] * b.e[7]  + a.e[8] * b.e[8]);
}

static m4
m4_mul(m4 a, m4 b) {
  return M4(
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
    a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15]);
}

static quat_t
quat_mul(quat_t a, quat_t b) {
  return Quat(
    a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
    a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
    a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

#ifdef __cplusplus

static v2 operator*(v2 a, v2 b) { return { a.x * b.x, a.y * b.y }; }
static v3 operator*(v3 a, v3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
static v4 operator*(v4 a, v4 b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }

static v2i operator*(v2i a, v2i b) { return { a.x * b.x, a.y * b.y }; }
static v3i operator*(v3i a, v3i b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
static v4i operator*(v4i a, v4i b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }

static v2& operator*=(v2& a, v2 b) { a = a * b; return a; }
static v3& operator*=(v3& a, v3 b) { a = a * b; return a; }
static v4& operator*=(v4& a, v4 b) { a = a * b; return a; }

static v2i& operator*=(v2i& a, v2i b) { a = a * b; return a; }
static v3i& operator*=(v3i& a, v3i b) { a = a * b; return a; }
static v4i& operator*=(v4i& a, v4i b) { a = a * b; return a; }

static v2 operator*(m2 m, v2 u) { return m2_mulv(m, u); }
static v3 operator*(m3 m, v3 u) { return m3_mulv(m, u); }
static v4 operator*(m4 m, v4 u) { return m4_mulv(m, u); }

static m2 operator*(m2 a, m2 b) { return m2_mul(a, b); }
static m3 operator*(m3 a, m3 b) { return m3_mul(a, b); }
static m4 operator*(m4 a, m4 b) { return m4_mul(a, b); }

static quat_t operator*(quat_t a, quat_t b) { return quat_mul(a, b); }

#endif

// ------------ divition ------------ //

static v2 v2_div(v2 a, v2 b) { return V2(a.x / b.x, a.y / b.y); }
static v3 v3_div(v3 a, v3 b) { return V3(a.x / b.x, a.y / b.y, a.z / b.z); }
static v4 v4_div(v4 a, v4 b) { return V4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static v2i v2i_div(v2i a, v2i b) { return V2i(a.x / b.x, a.y / b.y); }
static v3i v3i_div(v3i a, v3i b) { return V3i(a.x / b.x, a.y / b.y, a.z / b.z); }
static v4i v4i_div(v4i a, v4i b) { return V4i(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

#ifdef __cplusplus

static v2 operator/(v2 a, v2 b) { return { a.x / b.x, a.y / b.y }; }
static v3 operator/(v3 a, v3 b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
static v4 operator/(v4 a, v4 b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }

static v2i operator/(v2i a, v2i b) { return { a.x / b.x, a.y / b.y }; }
static v3i operator/(v3i a, v3i b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
static v4i operator/(v4i a, v4i b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }

static v2& operator/=(v2& a, v2 b) { a = a / b; return a; }
static v3& operator/=(v3& a, v3 b) { a = a / b; return a; }
static v4& operator/=(v4& a, v4 b) { a = a / b; return a; }

static v2i& operator/=(v2i& a, v2i b) { a = a / b; return a; }
static v3i& operator/=(v3i& a, v3i b) { a = a / b; return a; }
static v4i& operator/=(v4i& a, v4i b) { a = a / b; return a; }

#endif

// ------------- scaling ------------- //

static v2 v2_scale(v2 a, f32 s) { return V2(a.x * s, a.y * s); }
static v3 v3_scale(v3 a, f32 s) { return V3(a.x * s, a.y * s, a.z * s); }
static v4 v4_scale(v4 a, f32 s) { return V4(a.x * s, a.y * s, a.z * s, a.w * s); }

static v2i v2i_scale(v2i a, i32 s) { return V2i(a.x * s, a.y * s); }
static v3i v3i_scale(v3i a, i32 s) { return V3i(a.x * s, a.y * s, a.z * s); }
static v4i v4i_scale(v4i a, i32 s) { return V4i(a.x * s, a.y * s, a.z * s, a.w * s); }

#ifdef __cplusplus

static v2 operator*(v2 a, f32 s) { return { a.x * s, a.y * s }; }
static v3 operator*(v3 a, f32 s) { return { a.x * s, a.y * s, a.z * s }; }
static v4 operator*(v4 a, f32 s) { return { a.x * s, a.y * s, a.z * s, a.w * s }; }

static v2 operator*(f32 s, v2 a) { return { a.x * s, a.y * s }; }
static v3 operator*(f32 s, v3 a) { return { a.x * s, a.y * s, a.z * s }; }
static v4 operator*(f32 s, v4 a) { return { a.x * s, a.y * s, a.z * s, a.w * s }; }

static v2i operator*(v2i a, i32 s) { return { a.x * s, a.y * s }; }
static v3i operator*(v3i a, i32 s) { return { a.x * s, a.y * s, a.z * s }; }
static v4i operator*(v4i a, i32 s) { return { a.x * s, a.y * s, a.z * s, a.w * s }; }

static v2i operator*(i32 s, v2i a) { return { a.x * s, a.y * s }; }
static v3i operator*(i32 s, v3i a) { return { a.x * s, a.y * s, a.z * s }; }
static v4i operator*(i32 s, v4i a) { return { a.x * s, a.y * s, a.z * s, a.w * s }; }

static v2& operator*=(v2& a, f32 s) { a = a * s; return a; }
static v3& operator*=(v3& a, f32 s) { a = a * s; return a; }
static v4& operator*=(v4& a, f32 s) { a = a * s; return a; }

static v2i& operator*=(v2i& a, f32 s) { a = a * s; return a; }
static v3i& operator*=(v3i& a, f32 s) { a = a * s; return a; }
static v4i& operator*=(v4i& a, f32 s) { a = a * s; return a; }

#endif

// ----------- eq ------------ //

static b32 v2i_eq(v2i a, v2i b) { return a.x == b.x && a.y == b.y; }
static b32 v3i_eq(v3i a, v3i b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static b32 v4i_eq(v4i a, v4i b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static b32 v2i_neq(v2i a, v2i b) { return a.x != b.x || a.y != b.y; }
static b32 v3i_neq(v3i a, v3i b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static b32 v4i_neq(v4i a, v4i b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

#ifdef __cplusplus

static b32 operator==(v2i a, v2i b) { return a.x == b.x && a.y == b.y; }
static b32 operator==(v3i a, v3i b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static b32 operator==(v4i a, v4i b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static b32 operator!=(v2i a, v2i b) { return a.x != b.x || a.y != b.y; }
static b32 operator!=(v3i a, v3i b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static b32 operator!=(v4i a, v4i b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

#endif

// ----------- dot product ----------- //

static f32 v2_dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
static f32 v3_dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 v4_dot(v4 a, v4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static i32 v2i_dot(v2i a, v2i b) { return a.x * b.x + a.y * b.y; }
static i32 v3i_dot(v3i a, v3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static i32 v4i_dot(v4i a, v4i b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

#ifdef __cplusplus

static f32 dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
static f32 dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 dot(v4 a, v4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static i32 dot(v2i a, v2i b) { return a.x * b.x + a.y * b.y; }
static i32 dot(v3i a, v3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static i32 dot(v4i a, v4i b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

#endif

// ----------- length squared ----------- //

static f32 v2_len_sq(v2 u) { return v2_dot(u, u); }
static f32 v3_len_sq(v3 u) { return v3_dot(u, u); }
static f32 v4_len_sq(v4 u) { return v4_dot(u, u); }

static i32 v2i_len_sq(v2i u) { return v2i_dot(u, u); }
static i32 v3i_len_sq(v3i u) { return v3i_dot(u, u); }
static i32 v4i_len_sq(v4i u) { return v4i_dot(u, u); }

#ifdef __cplusplus

static f32 len_sq(v2 u) { return v2_dot(u, u); }
static f32 len_sq(v3 u) { return v3_dot(u, u); }
static f32 len_sq(v4 u) { return v4_dot(u, u); }

static i32 len_sq(v2i u) { return v2i_dot(u, u); }
static i32 len_sq(v3i u) { return v3i_dot(u, u); }
static i32 len_sq(v4i u) { return v4i_dot(u, u); }

#endif

// -------------- length -------------- //

static f32 v2_len(v2 u) { return sqrt32(v2_len_sq(u)); }
static f32 v3_len(v3 u) { return sqrt32(v3_len_sq(u)); }
static f32 v4_len(v4 u) { return sqrt32(v4_len_sq(u)); }

static f32 v2i_len(v2i u) { return sqrt32(v2i_len_sq(u)); }
static f32 v3i_len(v3i u) { return sqrt32(v3i_len_sq(u)); }
static f32 v4i_len(v4i u) { return sqrt32(v4i_len_sq(u)); }

#ifdef __cplusplus

static f32 len(v2 u) { return sqrt32(v2_len_sq(u)); }
static f32 len(v3 u) { return sqrt32(v3_len_sq(u)); }
static f32 len(v4 u) { return sqrt32(v4_len_sq(u)); }

static f32 len(v2i u) { return sqrt32(v2i_len_sq(u)); }
static f32 len(v3i u) { return sqrt32(v3i_len_sq(u)); }
static f32 len(v4i u) { return sqrt32(v4i_len_sq(u)); }

#endif

// -------------- distance squared -------------- //

static f32 v2_dist_sq(v2 a, v2 b) { return v2_len_sq(v2_sub(a, b)); }
static f32 v3_dist_sq(v3 a, v3 b) { return v3_len_sq(v3_sub(a, b)); }
static f32 v4_dist_sq(v4 a, v4 b) { return v4_len_sq(v4_sub(a, b)); }

static i32 v2i_dist_sq(v2i a, v2i b) { return v2i_len_sq(v2i_sub(a, b)); }
static i32 v3i_dist_sq(v3i a, v3i b) { return v3i_len_sq(v3i_sub(a, b)); }
static i32 v4i_dist_sq(v4i a, v4i b) { return v4i_len_sq(v4i_sub(a, b)); }

#ifdef __cplusplus

static f32 dist_sq(v2 a, v2 b) { return v2_len_sq(v2_sub(a, b)); }
static f32 dist_sq(v3 a, v3 b) { return v3_len_sq(v3_sub(a, b)); }
static f32 dist_sq(v4 a, v4 b) { return v4_len_sq(v4_sub(a, b)); }

static i32 dist_sq(v2i a, v2i b) { return v2i_len_sq(v2i_sub(a, b)); }
static i32 dist_sq(v3i a, v3i b) { return v3i_len_sq(v3i_sub(a, b)); }
static i32 dist_sq(v4i a, v4i b) { return v4i_len_sq(v4i_sub(a, b)); }

#endif

// ------------------ distance ------------------- //

static f32 v2_dist(v2 a, v2 b) { return sqrt32(v2_dist_sq(a, b)); }
static f32 v3_dist(v3 a, v3 b) { return sqrt32(v3_dist_sq(a, b)); }
static f32 v4_dist(v4 a, v4 b) { return sqrt32(v4_dist_sq(a, b)); }

#ifdef __cplusplus

static f32 dist(v2 a, v2 b) { return sqrt32(v2_dist_sq(a, b)); }
static f32 dist(v3 a, v3 b) { return sqrt32(v3_dist_sq(a, b)); }
static f32 dist(v4 a, v4 b) { return sqrt32(v4_dist_sq(a, b)); }

#endif

// -------------- manhattan distance -------------- //

static i32
v2i_manhattan(v2i a, v2i b) {
  v2i diff = v2i_sub(a, b);
  return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y);
}

static i32
v3i_manhattan(v3i a, v3i b) {
  v3i diff = v3i_sub(a, b);
  return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y) + (0x7ffffffff & diff.z);
}

#ifdef __cplusplus

static i32 manhattan(v2i a, v2i b) { return v2i_manhattan(a, b); }
static i32 manhattan(v3i a, v3i b) { return v3i_manhattan(a, b); }

#endif

// -------------- normalize --------------- //

static v2 v2_norm(v2 u) { return v2_scale(u, rsqrt32(v2_dot(u, u))); }
static v3 v3_norm(v3 u) { return v3_scale(u, rsqrt32(v3_dot(u, u))); }
static v4 v4_norm(v4 u) { return v4_scale(u, rsqrt32(v4_dot(u, u))); }

#ifdef __cplusplus

static v2 norm(v2 u) { return v2_scale(u, rsqrt32(v2_dot(u, u))); }
static v3 norm(v3 u) { return v3_scale(u, rsqrt32(v3_dot(u, u))); }
static v4 norm(v4 u) { return v4_scale(u, rsqrt32(v4_dot(u, u))); }

#endif

// -------------- project --------------- //

static v2
v2_project(v2 a, v2 b) {
  f32 d = v2_dot(b, b);
  if (d == 0) return V2(0);
  return v2_scale(b, v2_dot(a, b) / d);
}

static v3
v3_project(v3 a, v3 b) {
  f32 d = v3_dot(b, b);
  if (d == 0) return V3(0);
  return v3_scale(b, v3_dot(a, b) / d);
}

#ifdef __cplusplus

struct v2 project(v2 a, v2 b) { return v2_project(a, b); }
struct v3 project(v3 a, v3 b) { return v3_project(a, b); }

#endif

// -------------- floor --------------- //

static v2 v2_floor(v2 u) { return V2(floorf(u.x), floorf(u.y)); }
static v3 v3_floor(v3 u) { return V3(floorf(u.x), floorf(u.y), floorf(u.z)); }
static v4 v4_floor(v4 u) { return V4(floorf(u.x), floorf(u.y), floorf(u.z), floorf(u.w)); }

#ifdef __cplusplus

static v2 floor(v2 u) { return V2(floorf(u.x), floorf(u.y)); }
static v3 floor(v3 u) { return V3(floorf(u.x), floorf(u.y), floorf(u.z)); }
static v4 floor(v4 u) { return V4(floorf(u.x), floorf(u.y), floorf(u.z), floorf(u.w)); }

#endif

// -------------- ceil --------------- //

static v2 v2_ceil(v2 u) { return V2(ceilf(u.x), ceilf(u.y)); }
static v3 v3_ceil(v3 u) { return V3(ceilf(u.x), ceilf(u.y), ceilf(u.z)); }
static v4 v4_ceil(v4 u) { return V4(ceilf(u.x), ceilf(u.y), ceilf(u.z), ceilf(u.w)); }

#ifdef __cplusplus

static v2 ceil(v2 u) { return V2(ceilf(u.x), ceilf(u.y)); }
static v3 ceil(v3 u) { return V3(ceilf(u.x), ceilf(u.y), ceilf(u.z)); }
static v4 ceil(v4 u) { return V4(ceilf(u.x), ceilf(u.y), ceilf(u.z), ceilf(u.w)); }

#endif

// -------------- clamp --------------- //

static v2
v2_clampf(v2 u, f32 min, f32 max) {
  return V2(
    Clamp(u.x, min, max),
    Clamp(u.y, min, max));
}

static v3
v3_clampf(v3 u, f32 min, f32 max) {
  return V3(
    Clamp(u.x, min, max),
    Clamp(u.y, min, max),
    Clamp(u.z, min, max));
}

static v2i
v2i_clampi(v2i u, f32 min, f32 max) {
  return V2i(
    (i32)Clamp(u.x, min, max),
    (i32)Clamp(u.y, min, max));
}

static v3i
v3i_clampi(v3i u, f32 min, f32 max) {
  return V3i(
    (i32)Clamp(u.x, min, max),
    (i32)Clamp(u.y, min, max),
    (i32)Clamp(u.z, min, max));
}

static v2
v2_clamp(v2 u, r2 r) {
  return V2(
    Clamp(u.x, r.min.x, r.max.x),
    Clamp(u.y, r.min.y, r.max.y));
}

static v3
v3_clamp(v3 u, r3 r) {
  return V3(
    Clamp(u.x, r.min.x, r.max.x),
    Clamp(u.y, r.min.y, r.max.y),
    Clamp(u.z, r.min.z, r.max.z));
}

static v2i
v2i_clamp(v2i u, r2i r) {
  return V2i(
    Clamp(u.x, r.min.x, r.max.x),
    Clamp(u.y, r.min.y, r.max.y));
}

static v3i
v3i_clamp(v3i u, r3i r) {
  return V3i(
    Clamp(u.x, r.min.x, r.max.x),
    Clamp(u.y, r.min.y, r.max.y),
    Clamp(u.z, r.min.z, r.max.z));
}

#ifdef __cplusplus

static v2 clamp(v2 u, f32 min, f32 max) { return v2_clampf(u, min, max); }
static v3 clamp(v3 u, f32 min, f32 max) { return v3_clampf(u, min, max); }

static v2i clamp(v2i u, i32 min, i32 max) { return v2i_clampi(u, min, max); }
static v3i clamp(v3i u, i32 min, i32 max) { return v3i_clampi(u, min, max); }

static v2 clamp(v2 u, r2 r) { return v2_clamp(u, r); }
static v3 clamp(v3 u, r3 r) { return v3_clamp(u, r); }

static v2i clamp(v2i u, r2i r) { return v2i_clamp(u, r); }
static v3i clamp(v3i u, r3i r) { return v3i_clamp(u, r); }

#endif

// ---------------- min ----------------- //

static v2
v2_min(v2 a, v2 b) {
  return V2(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y);
}

static v3
v3_min(v3 a, v3 b) {
  return V3(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y,
    a.z < b.z? a.z : b.z);
}

static v4
v4_min(v4 a, v4 b) {
  return V4(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y,
    a.z < b.z? a.z : b.z,
    a.w < b.w? a.w : b.w);
}

static v2i
v2i_min(v2i a, v2i b) {
  return V2i(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y);
}

static v3i
v3i_min(v3i a, v3i b) {
  return V3i(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y,
    a.z < b.z? a.z : b.z);
}

static v4i
v4i_min(v4i a, v4i b) {
  return V4i(
    a.x < b.x? a.x : b.x,
    a.y < b.y? a.y : b.y,
    a.z < b.z? a.z : b.z,
    a.w < b.w? a.w : b.w);
}

#ifdef __cplusplus

static v2 min(v2 a, v2 b) { return v2_min(a, b); }
static v3 min(v3 a, v3 b) { return v3_min(a, b); }
static v4 min(v4 a, v4 b) { return v4_min(a, b); }

static v2i min(v2i a, v2i b) { return v2i_min(a, b); }
static v3i min(v3i a, v3i b) { return v3i_min(a, b); }
static v4i min(v4i a, v4i b) { return v4i_min(a, b); }

#endif

// ---------------- max ----------------- //

static v2
v2_max(v2 a, v2 b) {
  return V2(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y);
}

static v3
v3_max(v3 a, v3 b) {
  return V3(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y,
    a.z > b.z? a.z : b.z);
}

static v4
v4_max(v4 a, v4 b) {
  return V4(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y,
    a.z > b.z? a.z : b.z,
    a.w > b.w? a.w : b.w);
}

static v2i
v2i_max(v2i a, v2i b) {
  return V2i(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y);
}

static v3i
v3i_max(v3i a, v3i b) {
  return V3i(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y,
    a.z > b.z? a.z : b.z);
}

static v4i
v4i_max(v4i a, v4i b) {
  return V4i(
    a.x > b.x? a.x : b.x,
    a.y > b.y? a.y : b.y,
    a.z > b.z? a.z : b.z,
    a.w > b.w? a.w : b.w);
}

#ifdef __cplusplus

static v2 max(v2 a, v2 b) { return v2_max(a, b); }
static v3 max(v3 a, v3 b) { return v3_max(a, b); }
static v4 max(v4 a, v4 b) { return v4_max(a, b); }

static v2i max(v2i a, v2i b) { return v2i_max(a, b); }
static v3i max(v3i a, v3i b) { return v3i_max(a, b); }
static v4i max(v4i a, v4i b) { return v4i_max(a, b); }

#endif

// ---------------- lerp ----------------- //

static v2
v2_lerp(v2 a, v2 b, f32 t) {
  return V2(
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y));
}

static v3
v3_lerp(v3 a, v3 b, f32 t) {
  return V3(
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y),
    a.z + t * (b.z - a.z));
}

static v4
v4_lerp(v4 a, v4 b, f32 t) {
  return V4(
    a.x + t * (b.x - a.x),
    a.y + t * (b.y - a.y),
    a.z + t * (b.z - a.z),
    a.w + t * (b.w - a.w));
}

#ifdef __cplusplus

static v2 lerp(v2 a, v2 b, f32 t) { return v2_lerp(a, b, t); }
static v3 lerp(v3 a, v3 b, f32 t) { return v3_lerp(a, b, t); }
static v4 lerp(v4 a, v4 b, f32 t) { return v4_lerp(a, b, t); }

#endif

// -------------- sign (-1, 0, 1) ------------------- //

static v2 v2_sign(v2 u) { return V2((f32)Sign(u.x), (f32)Sign(u.y)); }
static v3 v3_sign(v3 u) { return V3((f32)Sign(u.x), (f32)Sign(u.y), (f32)Sign(u.z)); }
static v4 v4_sign(v4 u) { return V4((f32)Sign(u.x), (f32)Sign(u.y), (f32)Sign(u.z), (f32)Sign(u.w)); }

static v2i v2i_sign(v2i u) { return V2i(Sign(u.x), Sign(u.y)); }
static v3i v3i_sign(v3i u) { return V3i(Sign(u.x), Sign(u.y), Sign(u.z)); }
static v4i v4i_sign(v4i u) { return V4i(Sign(u.x), Sign(u.y), Sign(u.z), Sign(u.w)); }

#ifdef __cplusplus

static v2 sign(v2 u) { return v2_sign(u); }
static v3 sign(v3 u) { return v3_sign(u); }
static v4 sign(v4 u) { return v4_sign(u); }

static v2i sign(v2i u) { return v2i_sign(u); }
static v3i sign(v3i u) { return v3i_sign(u); }
static v4i sign(v4i u) { return v4i_sign(u); }

#endif

// --------------- cross ------------------- //

static v3
v3_cross(v3 a, v3 b) {
  return V3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x);
}

#ifdef __cplusplus

static v3 cross(v3 a, v3 b) { return v3_cross(a, b); }

#endif

// --------------- get angle ------------------- //

static f32
v2_get_angle(v2 a, v2 b) {
  f32 det = a.x * b.y - b.x * a.y;
  f32 dot = a.x * b.x + a.y * b.y;
  return atan2f(det, dot);
}

#ifdef __cplusplus

static f32 get_angle(v2 a, v2 b) { return v2_get_angle(a, b); }

#endif

// ----------- keep min ---------- //

static v3
v3_keep_min(v3 u) {
  f32 dx = fabsf(u.x);
  f32 dy = fabsf(u.y);
  f32 dz = fabsf(u.z);
  if (dx <= dy && dx <= dz) return V3(u.x, 0, 0);
  if (dy <= dx && dy <= dz) return V3(0, u.y, 0);
  if (dz <= dx && dz <= dy) return V3(0, 0, u.z);
  return u;
}

#ifdef __cplusplus

static v3 keep_min(v3 u) { return v3_keep_min(u); }

#endif

// ----------- mask min ---------- //

static v3
v3_mask_min(v3 u) {
  f32 dx = fabsf(u.x);
  f32 dy = fabsf(u.y);
  f32 dz = fabsf(u.z);

  if (dx <= dy && dx <= dz) return V3(0, 1, 1);
  if (dy <= dx && dy <= dz) return V3(1, 0, 1);
  if (dz <= dx && dz <= dy) return V3(1, 1, 0);

  return V3(1, 1, 1);
}

#ifdef __cplusplus

static v3 mask_min(v3 u) { return v3_mask_min(u); }

#endif

// ------------------ transform/scale/rotate ------------------ //

static m2
m2_rotate(f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);

  return M2(c, s, -s, c);
}

static m3
m3_rotate(v3 axis, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 k = 1.0f - c;

  v3 sa = { s * axis.x, s * axis.y, s * axis.z };
  v3 omca = { k * axis.x, k * axis.y, k * axis.z };

  return M3(
    omca.x * axis.x + c,
    omca.x * axis.y - sa.z,
    omca.x * axis.z + sa.y,
    omca.y * axis.x + sa.z,
    omca.y * axis.y + c,
    omca.y * axis.z - sa.x,
    omca.z * axis.x - sa.y,
    omca.z * axis.y + sa.x,
    omca.z * axis.z + c);
}

static m4
m4_rotate(v3 axis, f32 angle) {
  f32 cosv = cosf(angle);
  f32 sinv = sinf(angle);
  f32 inv_cosv = 1.0f - cosv;

  v3 sa = { axis.x * sinv, axis.y * sinv, axis.z * sinv };
  v3 omca = { axis.x * inv_cosv, axis.y * inv_cosv, axis.z * inv_cosv };

  return M4(
    omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
    omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
    omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
    0, 0, 0, 1);
}

static quat_t
quat_rotate(v3 axis, f32 angle) {
  f32 s = sinf(0.5f * angle);
  v3 v = { s * axis.x, s * axis.y, s * axis.z };
  return Quat(v.x, v.y, v.z, cosf(0.5f * angle));
}

static m4
m4_translate(f32 x, f32 y, f32 z) {
  return M4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    x, y, z, 1);
}

static m4
m4_scale(f32 x, f32 y, f32 z) {
  return M4(
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1);
}

// --------------- from quat --------------- //

static m3
m3_from_quat(quat_t q) {
  f32 a = q.w;
  f32 b = q.x;
  f32 c = q.y;
  f32 d = q.z;

  f32 a2 = a * a;
  f32 b2 = b * b;
  f32 c2 = c * c;
  f32 d2 = d * d;

  return M3(
    a2 + b2 - c2 - d2,
    2.0f * (b * c + a * d),
    2.0f * (b * d - a * c),

    2.0f * (b * c - a * d),
    a2 - b2 + c2 - d2,
    2.0f * (c * d + a * b),

    2.0f * (b * d + a * c),
    2.0f * (c * d - a * b),
    a2 - b2 - c2 + d2);
}

static m4
m4_from_quat(quat_t q) {
  f32 a = q.w;
  f32 b = q.x;
  f32 c = q.y;
  f32 d = q.z;

  f32 a2 = a * a;
  f32 b2 = b * b;
  f32 c2 = c * c;
  f32 d2 = d * d;

  return M4(
    a2 + b2 - c2 - d2,
    2.0f * (b * c + a * d),
    2.0f * (b * d - a * c),
    0.0f,

    2.0f * (b * c - a * d),
    a2 - b2 + c2 - d2,
    2.0f * (c * d + a * b),
    0.0f,

    2.0f * (b * d + a * c),
    2.0f * (c * d - a * b),
    a2 - b2 - c2 + d2,
    0.0f,

    0.0f,
    0.0f,
    0.0f,
    1.0f);
}

// --------------- view matricies --------------- //

static m4
m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
  return M4(
    2 / (r - l),
    0,
    0,
    0,

    0,
    2 / (t - b),
    0,
    0,

    0,
    0, 
    -2 / (f - n),
    0,

    -(r + l) / (r - l),
    -(t + b) / (t - b),
    -(f + n) / (f - n),
    1);
}

static m4
m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
  f32 a = 1.0f / tanf(y_fov / 2.0f);

  return M4(
    a / aspect, 0, 0, 0,
    0, a, 0, 0,
    0, 0, -((f + n) / (f - n)), -1,
    0, 0, -((2 * f * n) / (f - n)), 0);
}

static m4
m4_look_at(v3 eye, v3 center, v3 up) {
  v3 f = v3_norm(v3_sub(center, eye));
  v3 s = v3_norm(v3_cross(f, up));
  v3 t = v3_cross(s, f);
  m4 m;

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

typedef struct {
  f32 a;
  f32 b;
  f32 c;
  f32 d;
} plane_t;

typedef struct frustum {
  plane_t planes[6];
} frustum_t;

static plane_t
plane_normalize(plane_t p) {
  f32 r_len = rsqrt32(p.a * p.a + p.b * p.b + p.c * p.c);

  p.a = p.a * r_len;
  p.b = p.b * r_len;
  p.c = p.c * r_len;
  p.d = p.d * r_len;

  return p;
}

static frustum_t
frustum_create(m4 m) {
  frustum_t result;

  // left clipping plane
  result.planes[0].a = m.e[3]  + m.e[0];
  result.planes[0].b = m.e[7]  + m.e[4];
  result.planes[0].c = m.e[11] + m.e[8];
  result.planes[0].d = m.e[15] + m.e[12];

  // right clipping plane
  result.planes[1].a = m.e[3]  - m.e[0];
  result.planes[1].b = m.e[7]  - m.e[4];
  result.planes[1].c = m.e[11] - m.e[8];
  result.planes[1].d = m.e[15] - m.e[12];

  // top clipping plane
  result.planes[2].a = m.e[3]  - m.e[1];
  result.planes[2].b = m.e[7]  - m.e[5];
  result.planes[2].c = m.e[11] - m.e[9];
  result.planes[2].d = m.e[15] - m.e[13];

  // bottom clipping plane
  result.planes[3].a = m.e[3]  + m.e[1];
  result.planes[3].b = m.e[7]  + m.e[5];
  result.planes[3].c = m.e[11] + m.e[9];
  result.planes[3].d = m.e[15] + m.e[13];

  // near clipping plane
  result.planes[4].a = m.e[3]  + m.e[2];
  result.planes[4].b = m.e[7]  + m.e[6];
  result.planes[4].c = m.e[11] + m.e[10];
  result.planes[4].d = m.e[15] + m.e[14];

  // far clipping plane
  result.planes[5].a = m.e[3]  - m.e[2];
  result.planes[5].b = m.e[7]  - m.e[6];
  result.planes[5].c = m.e[11] - m.e[10];
  result.planes[5].d = m.e[15] - m.e[14];

  result.planes[0] = plane_normalize(result.planes[0]);
  result.planes[1] = plane_normalize(result.planes[1]);
  result.planes[2] = plane_normalize(result.planes[2]);
  result.planes[3] = plane_normalize(result.planes[3]);
  result.planes[4] = plane_normalize(result.planes[4]);
  result.planes[5] = plane_normalize(result.planes[5]);

  return result;
}

// ------------------ contains ------------------ //

static b32
circle_contains(circle_t c, v2 pos) {
  f32 distance = v2_dist_sq(c.pos, pos);
  return distance < (c.rad * c.rad);
}

static b32
sphere_contains(sphere_t s, v3 pos) {
  f32 distance = v3_dist_sq(s.pos, pos);
  return distance < (s.rad * s.rad);
}

static b32
r2_contains(r2 rect, v2 pos) {
  if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
  if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
  return true;
}

static b32
r3_contains(r3 rect, v3 pos) {
  if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
  if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
  if (pos.z < rect.min.z || pos.z > rect.max.z) return false;
  return true;
}

static b32
r2i_contains(r2i rect, v2i pos) {
  if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
  if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
  return true;
}

static b32
r3i_contains(r3i rect, v3i pos) {
  if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
  if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
  if (pos.z < rect.min.z || pos.z > rect.max.z) return false;
  return true;
}

static b32
frustum_contains(frustum_t fs, v3 pos) {
  for (i32 i = 0; i < 6; i++) {
    if (fs.planes[i].a * pos.x + fs.planes[i].b * pos.y + fs.planes[i].c * pos.z + fs.planes[i].d <= 0)
      return false;
  }

  return true;
}

#ifdef __cplusplus

static b32 contains(circle_t c, v2 pos) { return circle_contains(c, pos); }
static b32 contains(sphere_t s, v3 pos) { return sphere_contains(s, pos); }
static b32 contains(r2 rect, v2 pos)    { return r2_contains(rect, pos); }
static b32 contains(r3 rect, v3 pos)    { return r3_contains(rect, pos); }
static b32 contains(r2i rect, v2i pos)  { return r2i_contains(rect, pos); }
static b32 contains(r3i rect, v3i pos)  { return r3i_contains(rect, pos); }
static b32 contains(frustum_t fs, v3 pos) { return frustum_contains(fs, pos); }

#endif

// ------------------ intersect ------------------ //

static b32
circle_intersect(circle_t a, circle_t b) {
  f32 dx  = b.pos.x - a.pos.x;
  f32 dy  = b.pos.y - a.pos.y;
  f32 rt  = a.rad + b.rad;
  return (dx * dx + dy * dy) < (rt * rt);
}

static b32
sphere_intersect(sphere_t a, sphere_t b) {
  f32 dx = b.pos.x - a.pos.x;
  f32 dy = b.pos.y - a.pos.y;
  f32 dz = b.pos.z - a.pos.z;
  f32 rt = a.rad + b.rad;
  return (dx * dx + dy * dy) < (rt * rt);
}

static b32
r2_intersect(r2 a, r2 b) {
  if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
  if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
  return true;
}

static b32
r3_intersect(r3 a, r3 b) {
  if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
  if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
  if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

  return true;
}

static b32
r2i_intersect(r2i a, r2i b) {
  if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
  if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

  return true;
}

static b32
r3i_intersect(r3i a, r3i b) {
  if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
  if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
  if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

  return true;
}

static b32
frustum_intersect_sphere(frustum_t fs, sphere_t sphere) {
  for (i32 i = 0; i < 6; i++) {
    if(fs.planes[i].a * sphere.pos.x + fs.planes[i].b * sphere.pos.y + fs.planes[i].c * sphere.pos.z + fs.planes[i].d <= -sphere.rad) {
      return false;
    }
  }
  return true;
}

static b32
frustum_intersect_r3(frustum_t fs, r3 rect) {
  for (int i = 0; i < 6; i++) {
    if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
    if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
    return false;
  }
  return true;
}

#ifdef __cplusplus

static b32 intersect(circle_t a, circle_t b)        { return circle_intersect(a, b); }
static b32 intersect(sphere_t a, sphere_t b)        { return sphere_intersect(a, b); }
static b32 intersect(r2 a, r2 b)                    { return r2_intersect(a, b); }
static b32 intersect(r3 a, r3 b)                    { return r3_intersect(a, b); }
static b32 intersect(r2i a, r2i b)                  { return r2i_intersect(a, b); }
static b32 intersect(r3i a, r3i b)                  { return r3i_intersect(a, b); }
static b32 intersect(frustum_t fs, sphere_t sphere) { return frustum_intersect_sphere(fs, sphere); }
static b32 intersect(frustum_t fs, r3 rect)         { return frustum_intersect_r3(fs, rect); }

#endif

// ------------------- get overlap --------------- //

static r2
r2_get_overlap(r2 a, r2 b) {
  return R2(
    v2_max(a.min, b.min),
    v2_min(a.max, b.max));
}

static r3
r3_get_overlap(r3 a, r3 b) {
  return R3(
    v3_max(a.min, b.min),
    v3_min(a.max, b.max));
}

static r2i
r2i_get_overlap(r2i a, r2i b) {
  return R2i(
    v2i_max(a.min, b.min),
    v2i_min(a.max, b.max));
}

static r3i
r3i_get_overlap(r3i a, r3i b) {
  return R3i(
    v3i_max(a.min, b.min),
    v3i_min(a.max, b.max));
}

#ifdef __cplusplus

static r2  overlap(r2 a, r2 b)   { return r2_get_overlap(a, b); }
static r3  overlap(r3 a, r3 b)   { return r3_get_overlap(a, b); }
static r2i overlap(r2i a, r2i b) { return r2i_get_overlap(a, b); }
static r3i overlap(r3i a, r3i b) { return r3i_get_overlap(a, b); }

#endif

// -------------- get intersect vector ---------- //

static v2
circle_get_intersect_vector(circle_t a, circle_t b) {
  v2  delta = v2_sub(a.pos, b.pos);
  f32 depth = v2_len(delta) - (a.rad + b.rad);

  return v2_scale(delta, -depth);
}

static v3
sphere_get_intersect_vector(sphere_t a, sphere_t b) {
  v3 delta = v3_sub(a.pos, b.pos);
  f32 depth = v3_len(delta) - (a.rad + b.rad);

  return v3_scale(delta, -depth);
}

static v2
r2_get_intersect_vector(r2 a, r2 b) {
  r2 overlap  = r2_get_overlap(a, b);
  v2 delta = {
    0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
    0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
  };
  return v2_mul(v2_sub(overlap.max, overlap.min), v2_sign(delta));
}

static v3
r3_get_intersect_vector(r3 a, r3 b) {
  r3 overlap = r3_get_overlap(a, b);
  v3 delta = {
    0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
    0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
    0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z),
  };
  return v3_mul(v3_sub(overlap.max, overlap.min), v3_sign(delta));
}

static v2i
r2i_get_intersect_vector(r2i a, r2i b) {
  r2i overlap = r2i_get_overlap(a, b);
  v2i delta = {
    (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2,
    (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2,
  };
  return v2i_mul(v2i_sub(overlap.max, overlap.min), v2i_sign(delta));
}

static v3i
r3i_get_intersect_vector(r3i a, r3i b) {
  r3i overlap = r3i_get_overlap(a, b);
  v3i delta = {
    (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2,
    (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2,
    (a.min.z + a.max.z) / 2 - (b.min.z + b.max.z) / 2,
  };
  return v3i_mul(v3i_sub(overlap.max, overlap.min), v3i_sign(delta));
}

#ifdef __cplusplus

static v2  get_intersect_vector(circle_t a, circle_t b) { return circle_get_intersect_vector(a, b); }
static v3  get_intersect_vector(sphere_t a, sphere_t b) { return sphere_get_intersect_vector(a, b); }
static v2  get_intersect_vector(r2 a, r2 b)             { return r2_get_intersect_vector(a, b); }
static v3  get_intersect_vector(r3 a, r3 b)             { return r3_get_intersect_vector(a, b); }
static v2i get_intersect_vector(r2i a, r2i b)           { return r2i_get_intersect_vector(a, b); }
static v3i get_intersect_vector(r3i a, r3i b)           { return r3i_get_intersect_vector(a, b); }

#endif

// ---------------------- random ------------------------ //

static u32
rand_u32(u32* state) {
  u32 x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return *state = x;
}

// [min, max)
static i32
rand_i32(u32* state, i32 min, i32 max) {
  assert(min < max);
  return min + rand_u32(state) % (max - min);
}

static f32
rand_f32(u32* state, f32 min, f32 max) {
  return min + ((f32)rand_u32(state) / (f32)0xffffffff) * (max - min); 
}

static v2
rand_unit_v2(u32* state) {
  v2 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
  return v2_norm(out);
}

static v3
rand_unit_v3(u32* state) {
  v3 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
  return v3_norm(out);
}

static v2
rand_v2(u32* state, f32 min, f32 max) {
  return v2_scale(rand_unit_v2(state), rand_f32(state, min, max));
}

static v3
rand_v3(u32* state, f32 min, f32 max) {
  return v3_scale(rand_unit_v3(state), rand_f32(state, min, max));
}

// ----------------------- hash ------------------------- //

static u32
hash_u32(u32 a) {
  a = (a ^ 61) ^ (a >> 16);
  a = a + (a << 3);
  a = a ^ (a >> 4);
  a = a * 0x27d4eb2d;
  a = a ^ (a >> 15);

  return a;
}

static u32
hash_i32(i32 a) {
  union { u32 u; i32 i; } convert;
  convert.i = a;
  return hash_u32(convert.u);
}

static u32
hash_str(const char* str) {
  u32 hash = 0;
  while (*str) {
    hash = (hash << 7) + (hash >> 25) + *str++;
  }
  return hash + (hash >> 16);
}

static const u32 crc_table[] = {
  0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
  0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
  0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
  0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
  0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
  0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
  0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
  0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
  0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
  0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
  0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
  0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
  0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
  0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
  0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
  0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
  0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
  0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
  0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
  0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
  0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
  0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
  0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
  0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
  0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
  0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
  0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
  0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
  0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
  0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
  0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
  0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

static u32
hash_mem(const void *data, u32 size) {
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

static u32
hash_v2i(v2i k) {
  u32 a = hash_i32(k.x);
  u32 b = hash_i32(k.y);
  return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

static u32
hash_v3i(v3i k) {
  u32 a = hash_i32(k.x);
  u32 b = hash_i32(k.y);
  u32 c = hash_i32(k.z);
  return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

static u32
hash_v4i(v4i k) {
  u32 a = hash_i32(k.x);
  u32 b = hash_i32(k.y);
  u32 c = hash_i32(k.z);
  u32 d = hash_i32(k.w);
  return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2) ^ (d * HASH_PRIME3);
}

// --------------------- packed color u32 -------------------- //

static u32
pack_color_u8(u8 r, u8 g, u8 b, u8 a) {
  u32 color = 0;
  color |= (u32)(r) << 0;
  color |= (u32)(g) << 8;
  color |= (u32)(b) << 16;
  color |= (u32)(a) << 24;
  return color;
}

static u32
pack_color_f32(f32 r, f32 g, f32 b, f32 a) {
  return pack_color_u8((u8)(255 * r), (u8)(255 * g), (u8)(255 * b), (u8)(255 * a));
}

static u32
pack_color_v4(v4 color) {
  return pack_color_f32(color.r, color.g, color.b, color.a);
}

static u32
pack_color_v3(v3 color, f32 a) {
  return pack_color_f32(color.r, color.g, color.b, a);
}

// -------------------- priority queue ------------------- //

struct queue_node {
  f32 weight;
  v2i e;
};

struct priority_queue {
  u32 len;
  struct queue_node* array;
};

static b32
pq_empty(const struct priority_queue* queue) {
  return queue->len == 0;
}

static void
pq_clear(struct priority_queue* queue) {
  queue->len = 0;
}

static void
pq_push(struct priority_queue* queue, v2i e, f32 weight) {
  struct queue_node node = { weight, e };
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

static f32
pq_pop(v2i* out, struct priority_queue* queue) {
  struct queue_node data = queue->array[1];
  queue->array[1] = queue->array[queue->len];
  queue->len--;
  int i = 1;
  while (i != queue->len + 1) {
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

// -------------------- f64 matrix funcs ------------------- //

static void
f4x4_mul_64(f64 *R, const f64 *a, const f64 *b) {
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

static void
f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v) {
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

static void
f4x4_invert_64(f64* T, const f64* M) {
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

static b32
f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport) {
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

static b32
f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport) {
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

// ===================================== IMAGE STUFF ================================= //

typedef struct image {
  i32 width;
  i32 height;
  u32* pixels;
} image_t;

static u32
image_get(const image_t* img, i32 x, i32 y) {
  assert(x >= 0 && x < img->width);
  assert(y >= 0 && y < img->height);

  return img->pixels[y * img->width + x];
}

static void
image_set(image_t* img, i32 x, i32 y, u32 pixel) {
  assert(x >= 0 && x < img->width);
  assert(y >= 0 && y < img->height);
  img->pixels[y * img->width + x] = pixel;
}

// ===================================== MEM STUFF ================================= //

static void
m_clear(void* data, usize size) {
  volatile u8* d = (u8*)data;
  while (size--)
    *(d++) = 0;
}

static void
m_set(void* data, u8 value, usize size) {
  volatile u8* d = (u8*)data;
  while (size--)
    *(d++) = value;
}

static void
m_copy(void* dst, const void* src, usize size) {
  volatile u8* d = (u8*)dst;
  volatile const u8* s = (u8*)src;
  while (size--)
    *(d++) = *(s++);
}

// --------------------- allocator interface ---------------------- //

typedef u32 mem_tag;
enum {
  mem_tag_alloc,
  mem_tag_resize,
  mem_tag_free,
  // mem_tag_free_all,
};

typedef struct {
  mem_tag tag;

  usize size;
  void* pointer;

  void* data;
} mem_alloc_desc_t;

#define M_ALLOCATOR_PROC(name) void* name(mem_alloc_desc_t desc)
typedef M_ALLOCATOR_PROC(mem_allocator_proc_t);

typedef struct mem_allocator {
  mem_allocator_proc_t* proc;
  void* data;
} mem_allocator_t;

#define mem_type(allocator, type)         (type*)mem_zero(allocator, sizeof (type))
#define mem_array(allocator, type, size)  (type*)mem_zero(allocator, (size) * sizeof (type))

static void*
mem_alloc(mem_allocator_t allocator, usize size) {
  mem_alloc_desc_t desc = ATS_INIT;

  desc.tag = mem_tag_alloc;
  desc.size = size;
  desc.data = allocator.data;

  return allocator.proc(desc);
}

static void*
mem_resize(mem_allocator_t allocator, void* pointer, usize size) {
  mem_alloc_desc_t desc = ATS_INIT;

  desc.tag = mem_tag_resize;
  desc.pointer = pointer;
  desc.size = size;
  desc.data = allocator.data;

  return allocator.proc(desc);
}

static void
mem_free(mem_allocator_t allocator, void* pointer) {
  mem_alloc_desc_t desc = ATS_INIT;

  desc.tag = mem_tag_free;
  desc.pointer = pointer;
  desc.data = allocator.data;

  allocator.proc(desc);
}

static void*
mem_zero(mem_allocator_t allocator, usize size) {
  void* memory = mem_alloc(allocator, size);
  memset(memory, 0, size);
  return memory;
}

// ---------------------- arena allocator ------------------------ //

typedef struct mem_arena {
  usize index;
  usize cap;
  u8* buffer;

  usize top;
  usize stack[16];

  usize lock;
  usize max;
} mem_arena_t;

#define mem_arena_type(ma, t)           (t*)mem_arena_zero(ma, sizeof (t))
#define mem_arena_array(ma, t, count)   (t*)mem_arena_zero(ma, (count) * sizeof (t))

static mem_arena_t
mem_arena_create(u8* buffer, usize size) {
  mem_arena_t ma = {0};
  ma.cap = size;
  ma.buffer = buffer;
  return ma;
}

static void*
mem_arena_alloc(mem_arena_t* ma, usize byte_size) {
  byte_size = AlignUp(byte_size, 16);
  assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

  void* memory = ma->buffer + ma->index;
  ma->index += byte_size;
  ma->max = ma->max > ma->index? ma->max : ma->index;

  return memory;
}

static void*
mem_arena_zero(mem_arena_t* ma, usize byte_size) {
  void* ptr = mem_arena_alloc(ma, byte_size);;
  memset(ptr, 0, byte_size);
  return ptr;
}

static void*
mem_arena_begin(mem_arena_t* ma) {
  ma->lock = true;
  return ma->buffer + ma->index;
}

static void
mem_arena_end(mem_arena_t* ma, usize byte_size) {
  ma->index += AlignUp(byte_size, 16);
  ma->lock = false;
}

static void
mem_arena_save(mem_arena_t* ma) {
  assert(ma->top < ma->cap);
  ma->stack[ma->top++] = ma->index;
}

static void
mem_arena_restore(mem_arena_t* ma) {
  assert(ma->top > 0);
  ma->index = ma->stack[--ma->top];
}

static void
mem_arena_validate(mem_arena_t* ma) {
  assert(ma->top == 0);
}

static
M_ALLOCATOR_PROC(mem_arena_allocator_proc) {
  mem_arena_t* arena = (mem_arena_t*)desc.data;
  switch (desc.tag) {
    case mem_tag_alloc: {
      return mem_arena_alloc(arena, desc.size);
    } break;
  }
  return NULL;
}

static mem_allocator_t
mem_arena_allocator(mem_arena_t* arena) {
  mem_allocator_t allocator = {0};

  allocator.proc = mem_arena_allocator_proc;
  allocator.data = arena;

  return allocator;
}

// ====================================== BIT STUFF =================================== //

static void
bit_set(u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] |= (1 << bit);
}

static b32
bit_get(const u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  return array[idx] & (1 << bit);
}

static void
bit_clr(u32* array, u32 index) {
  u32 idx = index >> 5;
  u32 bit = index & 31;
  array[idx] &= ~(1 << bit);
}

// ===================================== STRING STUFF ================================= //

typedef struct string {
  usize size;
  const char* data;
} string;

#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).size, (s).data

#define str(text) Make(string) { sizeof (text) - 1, text }
#define STR(text) { sizeof (text) - 1, text }

static string
string_create(const char* str) {
  string s = {0};
  s.size = strlen(str);
  s.data = str;
  return s;
}

static b32
string_equal(string a, string b) {
  if (a.size != b.size) return false;
  return memcmp(a.data, b.data, a.size) == 0;
}

static b32
string_equal_cstr(string a, const char* b) {
  usize b_size = strlen(b);
  if (a.size != b_size) return false;
  return memcmp(a.data, b, a.size) == 0;
}

#ifdef __cplusplus

static b32 operator==(string a, string b)       { return string_equal(a, b); }
static b32 operator==(string a, const char* b)  { return string_equal_cstr(a, b); }

#endif

static b32
string_empty(string s) {
  return s.size == 0;
}

typedef struct split_iter {
  string current;

  const char* content;

  u32 del_table[8];
  u32 sep_table[8];
} split_iter;

static b32
split_iter_is_valid(const split_iter* it) {
  return it->current.size;
}

static void
split_iter_advance(split_iter* it) {
  while (*it->content && bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
    it->content++;
  }

  const char* begin = it->content;

  if (bit_get(it->sep_table, *it->content)) {
    it->content++;
  } else {
    while (*it->content && !bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
      it->content++;
    }
  }

  it->current = Make(string) {
    (usize)(it->content - begin),
    begin,
  };
}

static split_iter
split_iter_create(const char* cstr, const char* delimiters, const char* separators) {
  assert(delimiters);
  if (!separators) separators = "";

  split_iter it = {0};

  it.content = cstr;

  for (u32 i = 0; delimiters[i]; ++i) {
    bit_set(it.del_table, delimiters[i]);
  }

  for (u32 i = 0; separators[i]; ++i) {
    bit_set(it.sep_table, separators[i]);
  }

  split_iter_advance(&it);
  return it;
}

// =================================================== SPATIAL MAP =================================================== //

#define SPATIAL_MAX (8 * 4096)

typedef struct sm_cell {
  void* e;
  r2 rect;

  struct sm_cell* next;
} sm_cell_t;

typedef struct spatial_map {
  sm_cell_t* table[4096];

  usize count;
  sm_cell_t array[SPATIAL_MAX];
} spatial_map_t;

static void
sm_clear(spatial_map_t* map) {
  memset(map->table, 0, sizeof map->table);
  map->count = 0;
}

static u32
sm_index(const spatial_map_t* map, v2i pos) {
  u32 hash = hash_v2i(pos);
  return hash % ArrayCount(map->table);
}

static void
sm_add(spatial_map_t* map, void* e, r2 e_rect) {
  r2i rect = {
    (i32)e_rect.min.x, (i32)e_rect.min.y,
    (i32)e_rect.max.x, (i32)e_rect.max.y,
  };

  for_r2(rect, x, y) {
    u32 index = sm_index(map, V2i(x, y));
    sm_cell_t* cell = map->array + map->count++;

    cell->e = e;
    cell->rect = e_rect;
    cell->next = map->table[index];

    map->table[index] = cell;
  }
}

typedef struct sm_entry {
  void* e;
  r2 rect;
} sm_entry_t;

typedef struct sm_result {
  usize count;
  sm_entry_t* array;
} sm_result_t;

static sm_result_t
sm_in_range(spatial_map_t* map, v2 pos, v2 rad, const void* ignore) {
  static sm_entry_t spatial_array[SPATIAL_MAX];

  sm_result_t result = ATS_INIT;
  result.array = spatial_array;

  r2 rect = {
    pos.x - rad.x, pos.y - rad.y,
    pos.x + rad.x, pos.y + rad.y,
  };

  r2i irect = {
    (i32)(pos.x - rad.x), (i32)(pos.y - rad.y),
    (i32)(pos.x + rad.x), (i32)(pos.y + rad.y),
  };

  for_r2(irect, x, y) {
    u32 index = sm_index(map, V2i(x, y));
    for (sm_cell_t* it = map->table[index]; it; it = it->next) {
      b32 unique = true;

      if (it->e == ignore) continue;
      if (!r2_intersect(rect, it->rect)) continue;

      for_range(i, 0, (isize)result.count) {
        if (result.array[i].e == it->e) {
          unique = false;
          break;
        }
      }
      if (unique) {
        result.array[result.count++] = Make(sm_entry_t) {
          it->e,
          it->rect,
        };
      }
    }
  }

  return result;
}

typedef struct sm_iter {
  sm_entry_t* current;

  u32 index;
  sm_result_t result;
} sm_iter;

static sm_iter
sm_get_iterator(spatial_map_t* map, v2 pos, v2 rad, const void* ignore) {
  sm_iter it = ATS_INIT;

  it.result = sm_in_range(map, pos, rad, ignore);
  it.current = &it.result.array[0];

  return it;
}

static b32
sm_iter_is_valid(const sm_iter* it) {
  return it->index < it->result.count;
}

static void
sm_iter_advance(sm_iter* it) {
  it->index++;
  it->current = it->result.array + it->index;
}

static void*
sm_get_closest(spatial_map_t* map, v2 pos, f32 range, const void* ignore, b32 (*condition_proc)(void*)) {
  void* result = NULL;
  f32 distance = range;

  for_iter(sm_iter, it, sm_get_iterator(map, pos, V2(range, range), ignore)) {
    sm_entry_t* e = it.current;

    if (condition_proc && !condition_proc(e->e)) {
      continue;
    }

    v2 e_pos = {
      0.5f * (e->rect.min.x + e->rect.max.x),
      0.5f * (e->rect.min.y + e->rect.max.y),
    };

    f32 new_distance = v2_dist(e_pos, pos);

    if (new_distance <= distance) {
      result = e->e;
      distance = new_distance;
    }
  }

  return result;
}

static void*
sm_at_position(spatial_map_t* map, v2 pos) {
  u32 index = sm_index(map, V2i((i32)pos.x, (i32)pos.y));

  for (sm_cell_t* it = map->table[index]; it; it = it->next) {
    if (r2_contains(it->rect, pos)) {
      return it->e;
    }
  }
  return NULL;
}

// ============================================ RAYCAST 2D TILEMAP ========================================== //

#define RAY2D_MAP_SIZE      (512)
#define RAY2D_MOD           (511)
#define RAY2D_ARRAY_SIZE    (8192) // (512 * 512) / 32

typedef struct ray2d_map {
  u32 array[RAY2D_ARRAY_SIZE];
} ray2d_map_t;

static void
ray2d_init(ray2d_map_t* map) {
  memset(map, 0, sizeof (ray2d_map_t));
}

static inline u32
ray2d_index(const ray2d_map_t* map, u32 x, u32 y) {
  return (y & RAY2D_MOD) * RAY2D_MAP_SIZE + (x & RAY2D_MOD);
}

static inline void
ray2d_set_traversable(ray2d_map_t* map, u32 x, u32 y) {
  bit_set(map->array, ray2d_index(map, x, y));
}

static inline b32
ray2d_is_traversable(const ray2d_map_t* map, u32 x, u32 y) {
  return bit_get(map->array, ray2d_index(map, x, y));
}

static v2
ray2d_cast_dir(const ray2d_map_t* map, v2 pos, v2 dir, f32 max_range) {
#if 0
  //if (!ray2d_is_traversable(map, pos.x, pos.y)) return pos;
#endif
  b32 kill_on_wall_hit = ray2d_is_traversable(map, pos.x, pos.y);
  
  //which box of the map we're in
  int map_x = (int)(pos.x);
  int map_y = (int)(pos.y);

  //length of ray from current position to next x or y-side
  f32 side_dist_x = 0;
  f32 side_dist_y = 0;

  //length of ray from one x or y-side to next x or y-side
  f32 delta_dist_x = (dir.x == 0.0f) ? 1e30 : fabsf(1.0f / dir.x);
  f32 delta_dist_y = (dir.y == 0.0f) ? 1e30 : fabsf(1.0f / dir.y);

  f32 perp_wall_dist = 0;

  //what direction to step in x or y-direction (either +1 or -1)
  int step_x = 0;
  int step_y = 0;

  b32 hit  = false; //was there a wall hit?
  int side = 0; //was a NS or a EW wall hit?

  //calculate step and initial sideDist
  if (dir.x < 0) {
    step_x = -1;
    side_dist_x = (pos.x - map_x) * delta_dist_x;
  } else {
    step_x = 1;
    side_dist_x = (map_x + 1.0 - pos.x) * delta_dist_x;
  }

  if (dir.y < 0) {
    step_y = -1;
    side_dist_y = (pos.y - map_y) * delta_dist_y;
  } else {
    step_y = 1;
    side_dist_y = (map_y + 1.0 - pos.y) * delta_dist_y;
  }

  while (!hit) {
    //jump to next map square, either in x-direction, or in y-direction
    if (side_dist_x < side_dist_y) {
      side_dist_x += delta_dist_x;
      map_x += step_x;
      side = 0;
    } else {
      side_dist_y += delta_dist_y;
      map_y += step_y;
      side = 1;
    }

    if (v2_dist(pos, V2((f32)map_x, (f32)map_y)) > (max_range + 1.0f)) {
      hit = true;
    }

    if (kill_on_wall_hit) {
      if (!ray2d_is_traversable(map, map_x, map_y)) {
        hit = true;
      }
    }
  } 

  if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
  else           perp_wall_dist = (side_dist_y - delta_dist_y);

  return v2_add(pos, v2_scale(dir, (perp_wall_dist > max_range? max_range : perp_wall_dist)));
}

static v2
ray2d_cast_angle(const ray2d_map_t* map, v2 from, f32 angle, f32 max_range) {
  m2 rot = m2_rotate(angle);
  v2 dir = m2_mulv(rot, V2(0, 1));

  return ray2d_cast_dir(map, from, dir, max_range);
}


// ===================================== EXTERNAL STUFF ===================================== //

// -------------------------------- ats_file.c ----------------------------------- //

extern char* file_read_str(const char* file_name, mem_allocator_t allocator);
extern b32 file_write_str(const char* file_name, const char* buffer);
extern b32 file_append_str(const char* file_name, const char* buffer);

extern b32 file_read_bin(const char* file_name, void* buffer, usize size);
extern b32 file_write_bin(const char* file_name, const void* buffer, usize size);

extern image_t file_load_image(const char* path);
extern void file_free_image(image_t* img);

// ------------------------------- ats_memory.c --------------------------------- //

extern mem_allocator_t mem_heap_allocator(void);
extern mem_allocator_t mem_linear_allocator(usize size);

// --------------------------- ats_texture_table.c ------------------------------ //

#define TEXTURE_TABLE_SIZE (1024)

typedef struct tt_id {
  u16 index;
} tt_id_t;

typedef struct tt_entry {
  b32 in_use;
  u32 hash;
  r2i rect;
  char name[64];
} tt_entry_t;

typedef struct texture_table {
  image_t img;
  tt_entry_t array[TEXTURE_TABLE_SIZE];
} texture_table_t;

extern void tt_begin(int width, int height, mem_allocator_t allocator);
extern void tt_end(void);
extern void tt_add_image(const char* name, image_t img);
extern void tt_load_from_dir(const char* dir_path);

extern texture_table_t* tt_get_texture_table(void);

extern tt_id_t tt_get_id(const char* name);
extern r2i tt_get_rect(tt_id_t id);
extern r2i tt_get(const char* name);

extern image_t tt_get_image(void);

// -------------------------- ats_animation.c ----------------------------- //

typedef struct at_frame at_frame_t;
typedef struct at_animation at_animation_t;
typedef struct at_entity at_entity_t;

struct at_frame {
  const char* name;
  r2i rect;

  struct at_frame* next;
  struct at_animation* animation;
};

struct at_animation {
  const char* name;

  struct at_frame* frame;
  struct at_animation* next;
};

struct at_entity {
  const char* name;

  struct at_animation* animation;
  struct at_entity* next;
};

extern void at_begin(mem_allocator_t allocator);
extern void at_end(void);

extern void at_add_entity(const char* name);
extern void at_add_animation(const char* name);
extern void at_add_frame(const char* name);

typedef struct at_asset {
  at_entity_t* entity;
  at_frame_t* frame;
  f32 duration;
} at_asset_t;

extern at_asset_t at_get(const char* name);
extern void at_set(at_asset_t* asset, const char* name);
extern void at_update(at_asset_t* asset, f32 dt);

// -------------------------- ats_audio_table.c ----------------------------- //

typedef struct audio_id {
  u16 index;
} audio_id_t;

extern void audio_init(void* handle);
extern audio_id_t audio_get(const char* name);
extern void audio_pause(b32 pause);
extern void audio_kill_all(void);
extern void audio_play(audio_id_t id, f32 volume);
extern void* audio_play_looped(audio_id_t id, f32 volume);
extern void audio_play_music(audio_id_t id, f32 volume);
extern void audio_play_from_source(audio_id_t id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);

// ------------------------------- platform ---------------------------------- //

#ifdef ATS_OGL33
#include "ext/glad/glad.h"
#endif

#if defined(ATS_OGL33)
#define GLSL(...) "#version 330 core\n" #__VA_ARGS__
#endif

// ====================================================== API =================================================== //

// ------------------- platform layer ------------------------ //

struct platform_t;
extern struct platform_t platform;

extern void platform_init(const char* title, int width, int height, int samples);
extern void platform_update(void);

extern f64 timer_get_current(void);

// ---------------- gl helper functions/types ---------------- //

extern void gl_init(void);

extern void gl_set_simple_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_simple_light_directed(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_light_emitter(int index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic);
extern void gl_set_light_directed(int index, v3 pos, v3 color);
extern void gl_set_light_global_ambient(f32 r, f32 g, f32 b);

extern void gl_init_bitmap_font(void);
extern void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color);
extern void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...);

extern void gl_begin(u32 type);
extern void gl_end(void);
extern void gl_color(u32 color);
extern void gl_normal(f32 x, f32 y, f32 z);
extern void gl_uv(f32 x, f32 y);
extern void gl_vertex(f32 x, f32 y, f32 z);
extern void gl_set_matrix(m4 projection, m4 view);
extern void gl_billboard(r2i tex_rect, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up);
extern void gl_texture_box(r2i tex_rect, r3 box, u32 color);
extern void gl_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color);
extern void gl_texture_rect_flip(r2i tex_rect, r2 rect, f32 z, u32 color, bool flip_x, bool flip_y);
extern void gl_box(r3 box, u32 color);
extern void gl_rect(r2 rect, f32 z, u32 color);

typedef struct gl_texture {
  u32 id;
  i32 width;
  i32 height;
} gl_texture_t;

extern gl_texture_t gl_texture_create(void *pixels, int width, int height, int is_smooth);
extern gl_texture_t gl_texture_create_from_image(image_t image, int is_smooth);
extern gl_texture_t gl_texture_load_from_file(const char *texture_path, int is_smooth);

extern void gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth);
extern void gl_texture_bind(const gl_texture_t* texture);
extern void gl_texture_destroy(gl_texture_t* texture);

typedef struct gl_shader {
  u32 id;
} gl_shader_t;

typedef struct gl_shader_desc {
  const char* vs;
  const char* fs;
} gl_shader_desc_t;

extern gl_shader_t gl_shader_create(const  gl_shader_desc_t* desc);
extern gl_shader_t gl_shader_load_from_file(const char *vs, const char *fs,  mem_allocator_t allocator);

extern void gl_use(const gl_shader_t* shader);
extern u32  gl_location(const gl_shader_t* shader, const char* name);

extern void gl_uniform_i32(u32 location, int u);
extern void gl_uniform_f32(u32 location, f32 u);
extern void gl_uniform_v2(u32 location, v2 u);
extern void gl_uniform_v3(u32 location, v3 u);
extern void gl_uniform_v4(u32 location, v4 u);
extern void gl_uniform_m2(u32 location, m2 m);
extern void gl_uniform_m3(u32 location, m3 m);
extern void gl_uniform_m4(u32 location, m4 m);

extern v3 gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview);

typedef struct gl_buffer {
  u32 vao;
  u32 vbo;
} gl_buffer_t;

typedef struct gl_layout {
  u32 size;
  u32 type;
  u32 stride;
  u32 offset;
  b32 normalize;
} gl_layout_t;

typedef struct gl_buffer_desc {
  gl_layout_t layout[32];
} gl_buffer_desc_t;

extern gl_buffer_t gl_buffer_create(const gl_buffer_desc_t* desc);
extern void gl_buffer_bind(const gl_buffer_t* buffer);
extern void gl_buffer_send(const gl_buffer_t* array, const void* data, u32 size);

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

typedef union gamepad_buttons {
  struct {
    u32 x : 1;
    u32 a : 1;
    u32 b : 1;
    u32 y : 1;

    u32 left_bumper     : 1;
    u32 right_bumper    : 1;
    u32 left_trigger    : 1;
    u32 right_trigger   : 1;

    u32 select          : 1;
    u32 start           : 1;
    u32 left_stick      : 1;
    u32 right_stick     : 1;

    u32 up      : 1;
    u32 right   : 1;
    u32 down    : 1;
    u32 left    : 1;
  } button;

  u32 data;
} gamepad_buttons_t;

typedef struct gamepad {
  b32 active;

  v2 left_stick;
  v2 right_stick;

  f32 left_trigger;
  f32 right_trigger;

  gamepad_buttons_t down;
  gamepad_buttons_t pressed;
  gamepad_buttons_t released;
} gamepad_t;

typedef u32 mouse_mode_t;
enum {
  mouse_mode_normal,
  mouse_mode_hidden,
  mouse_mode_disabled
};

struct platform_t {
  b32 close;

  i32 width;
  i32 height;
  f32 aspect_ratio;

  void* native;

  b32 fullscreen;
  b32 _fullscreen_state_last_update;

  struct {
    f64 total;
    f32 delta;
  } time;

  struct {
    u32 mode;

    b32 is_down : 1;
    b32 is_pressed : 1;
    b32 is_released : 1;

    v2 pos;
    v2 delta;
    v2 scroll;

    b8 down[MOUSE_BUTTON_LAST + 1];
    b8 pressed[MOUSE_BUTTON_LAST + 1];
    b8 released[MOUSE_BUTTON_LAST + 1];
  } mouse;

  struct {
    i32 key;
    i32 ascii;

    b32 is_down : 1;
    b32 is_pressed : 1;
    b32 is_repeat : 1;
    b32 is_released : 1;
    b32 is_ascii : 1;

    b8 down[KEY_LAST + 1];
    b8 pressed[KEY_LAST + 1];
    b8 repeat[KEY_LAST + 1];
    b8 released[KEY_LAST + 1];
  } keyboard;

  gamepad_t gamepad[JOYSTICK_LAST];
};

extern struct platform_t platform;

#endif // __ATS_H__

// ================================================================================================= //
// ========================================= IMPLEMENTATION ======================================== //
// ================================================================================================= //
#ifdef ATS_IMPL
#ifndef ATS_IMPL_ONCE
#define ATS_IMPL_ONCE

// ----------------------------------- memory impl ---------------------------------- //

#include <stdlib.h>

// ==================================== HEAP ALLOCATOR ================================ //

static
M_ALLOCATOR_PROC(mem_heap_allocator_proc) {
  switch (desc.tag) {
    case mem_tag_alloc: {
      return malloc(desc.size);
    } break;

    case mem_tag_resize: {
      void* new_pointer = realloc(desc.pointer, desc.size);
      if (new_pointer != desc.pointer) {
        free(desc.pointer);
      }
      return new_pointer;
    } break;

    case mem_tag_free: {
      free(desc.pointer);
    } break;
  }
  return NULL;
}

extern mem_allocator_t
mem_heap_allocator(void) {
  return Make(mem_allocator_t) { mem_heap_allocator_proc };
}

// ====================================== LINEAR ALLOCATOR (using chunks) ================================ //

typedef struct mem_chunk_t {
  struct mem_chunk_t* prev;
  usize index;
  u8 data[];
} mem_chunk_t;

typedef struct mem_linear_t {
  usize chunk_size;
  usize chunk_count;
  usize memory_used;

  mem_chunk_t* chunk;
} mem_linear_t;

static void*
mem_linear_alloc(mem_linear_t* arena, usize size) {
  arena->memory_used += size;

  if (!arena->chunk || (arena->chunk->index + size > arena->chunk_size)) {
    mem_chunk_t* chunk = (mem_chunk_t*)malloc(sizeof (mem_chunk_t) + Max(arena->chunk_size, size));
    assert(chunk);

    chunk->index = 0;
    chunk->prev = arena->chunk;

    arena->chunk = chunk;
    arena->chunk_count++;
  }

  void* memory = arena->chunk->data + arena->chunk->index;
  arena->chunk->index += size;

  return memory;
}

static
M_ALLOCATOR_PROC(mem_linear_allocator_proc) {
  mem_linear_t* arena = (mem_linear_t*)desc.data;

  switch (desc.tag) {
    case mem_tag_alloc: {
      return mem_linear_alloc(arena, desc.size);
    } break;

    case mem_tag_resize: {
      void* memory = mem_linear_alloc(arena, desc.size);
      if (desc.pointer) {
        memcpy(memory, desc.pointer, desc.size);
      }
      return memory;
    } break;
  }
  return NULL;
}

extern mem_allocator_t
mem_linear_allocator(usize chunk_size) {
  mem_linear_t* arena = (mem_linear_t*)calloc(1, sizeof (mem_linear_t));
  assert(arena);

  arena->chunk_size = chunk_size;

  return Make(mem_allocator_t) {
    mem_linear_allocator_proc,
    arena,
  };
}

// ----------------------------------- platform impl ---------------------------------- //

#include "ext/GLFW/ats_glfw.h"

#ifdef ATS_OGL33
#include "ext/glad/glad.c"
#endif

struct platform_t platform;

static struct {
  GLFWwindow* window;
  GLFWmonitor* monitor;
} platform_internal;

static void
window_key_callback(GLFWwindow* window, int key, int a, int action, int b) {
  (void)window;
  (void)a;
  (void)b;

  switch (action) {
    case GLFW_PRESS: {
      platform.keyboard.key = key;
      platform.keyboard.is_down = 1;
      platform.keyboard.is_pressed = 1;
      platform.keyboard.is_repeat = 1;
      platform.keyboard.down[key] = 1;
      platform.keyboard.pressed[key] = 1;
      platform.keyboard.repeat[key] = 1;
    } break;
    case GLFW_REPEAT: {
      platform.keyboard.is_repeat = 1;
      platform.keyboard.repeat[key] = 1;
    } break;
    case GLFW_RELEASE: {
      platform.keyboard.is_down = 0;
      platform.keyboard.is_released = 1;

      platform.keyboard.down[key] = 0;
      platform.keyboard.released[key] = 1;
    } break;
  }
}

static void
window_char_callback(GLFWwindow* window, unsigned int codepoint) {
  platform.keyboard.is_ascii  = 1;
  platform.keyboard.ascii     = codepoint;
}

static void
window_mouse_button_callback(GLFWwindow* window, int button, int action, int a) {
  (void)window;
  (void)a;

  switch (action) {
    case GLFW_PRESS: {
      platform.mouse.is_down = 1;
      platform.mouse.is_pressed = 1;
      platform.mouse.down[button] = 1;
      platform.mouse.pressed[button] = 1;
    } break;
    case GLFW_RELEASE: {
      platform.mouse.is_down = 0;
      platform.mouse.is_released = 1;
      platform.mouse.down[button] = 0;
      platform.mouse.released[button] = 1;
    } break;
  }
}

static void
window_scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
  (void)window;

  platform.mouse.scroll.x = (f32)xoffset;
  platform.mouse.scroll.y = (f32)yoffset;
}

static void
window_joystick_callback(int joy, int event) {
  if (event == GLFW_CONNECTED) {
    memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    platform.gamepad[joy].active = 1;
  }

  if (event == GLFW_DISCONNECTED) {
    memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
  }
}

extern void
platform_init(const char* title, int width, int height, int samples) {
  glfwInit();

  platform_internal.monitor = glfwGetPrimaryMonitor();

  const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#if defined(ATS_OGL33)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  glfwWindowHint(GLFW_SAMPLES, samples);

  platform.width  = width;
  platform.height = height;

  platform_internal.window = glfwCreateWindow(width, height, title, NULL, NULL);
  platform.native = glfwGetWin32Window(platform_internal.window);

  glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

  glfwMakeContextCurrent(platform_internal.window);

#if defined(ATS_OGL33)
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

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  platform_update();
}

extern void
platform_update(void) {
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
      case mouse_mode_normal: {
        glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } break;
      case mouse_mode_hidden: {
        glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      } break;
      case mouse_mode_disabled: {
        glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      } break;
    }
  }

  // update gamepads:
  {
    GLFWgamepadstate state;

    for (int i = 0; i < JOYSTICK_LAST; ++i) {
      if (platform.gamepad[i].active) {
        gamepad_buttons_t old = platform.gamepad[i].down;

        platform.gamepad[i].down.data       = 0;
        platform.gamepad[i].pressed.data    = 0;
        platform.gamepad[i].released.data   = 0;

        glfwGetGamepadState(i, &state);

        platform.gamepad[i].left_stick.x    = +state.axes[GAMEPAD_AXIS_LEFT_X];
        platform.gamepad[i].left_stick.y    = -state.axes[GAMEPAD_AXIS_LEFT_Y];
        platform.gamepad[i].right_stick.x   = +state.axes[GAMEPAD_AXIS_RIGHT_X];
        platform.gamepad[i].right_stick.y   = -state.axes[GAMEPAD_AXIS_RIGHT_Y];

        platform.gamepad[i].left_trigger    = 0.5f * (state.axes[GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f);
        platform.gamepad[i].right_trigger   = 0.5f * (state.axes[GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f);

        if (state.buttons[GAMEPAD_BUTTON_X]) platform.gamepad[i].down.button.x = 1;
        if (state.buttons[GAMEPAD_BUTTON_A]) platform.gamepad[i].down.button.a = 1;
        if (state.buttons[GAMEPAD_BUTTON_B]) platform.gamepad[i].down.button.b = 1;
        if (state.buttons[GAMEPAD_BUTTON_Y]) platform.gamepad[i].down.button.y = 1;

        if (state.buttons[GAMEPAD_BUTTON_LEFT_BUMPER])  platform.gamepad[i].down.button.left_bumper     = 1;
        if (state.buttons[GAMEPAD_BUTTON_RIGHT_BUMPER]) platform.gamepad[i].down.button.right_bumper    = 1;

        if (platform.gamepad[i].left_trigger  > 0.0f)   platform.gamepad[i].down.button.left_trigger    = 1;
        if (platform.gamepad[i].right_trigger > 0.0f)   platform.gamepad[i].down.button.right_trigger   = 1;

        if (state.buttons[GAMEPAD_BUTTON_BACK])         platform.gamepad[i].down.button.select      = 1;
        if (state.buttons[GAMEPAD_BUTTON_START])        platform.gamepad[i].down.button.start       = 1;
        if (state.buttons[GAMEPAD_BUTTON_LEFT_THUMB])   platform.gamepad[i].down.button.left_stick  = 1;
        if (state.buttons[GAMEPAD_BUTTON_RIGHT_THUMB])  platform.gamepad[i].down.button.right_stick = 1;

        if (state.buttons[GAMEPAD_BUTTON_DPAD_UP])      platform.gamepad[i].down.button.up      = 1;
        if (state.buttons[GAMEPAD_BUTTON_DPAD_RIGHT])   platform.gamepad[i].down.button.right   = 1;
        if (state.buttons[GAMEPAD_BUTTON_DPAD_DOWN])    platform.gamepad[i].down.button.down    = 1;
        if (state.buttons[GAMEPAD_BUTTON_DPAD_LEFT])    platform.gamepad[i].down.button.left    = 1;

        platform.gamepad[i].pressed.data    =  platform.gamepad[i].down.data & ~old.data;
        platform.gamepad[i].released.data   = ~platform.gamepad[i].down.data &  old.data;
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

  platform.time.delta = (f32)(glfwGetTime() - platform.time.total);
  platform.time.total += platform.time.delta;
}

extern f64
timer_get_current(void) {
  return glfwGetTime();
}

extern gl_texture_t
gl_texture_create_from_image(image_t image, int is_smooth) {
  return gl_texture_create(image.pixels, image.width, image.height, is_smooth);
}

extern gl_texture_t
gl_texture_load_from_file(const char* texture_path, int is_smooth) {
  image_t img = file_load_image(texture_path);
  gl_texture_t texture = gl_texture_create_from_image(img, is_smooth);
  file_free_image(&img);
  return texture;
}

extern void
gl_texture_destroy(gl_texture_t* texture) {
  glDeleteTextures(1, &texture->id);
  memset(texture, 0, sizeof *texture);
}

extern v3
gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview) {
  GLint viewport[4] = {0};
  f64 modelview[16] = {0};
  f64 projection[16] = {0};

  GLfloat win_x, win_y, win_z;

  for (i32 i = 0; i < 16; ++i) projection[i]  = in_projection.e[i];
  for (i32 i = 0; i < 16; ++i) modelview[i]   = in_modelview.e[i];

  glGetIntegerv(GL_VIEWPORT, viewport);

  win_x = (f64)(x);
  win_y = (f64)(viewport[3]) - (f64)y;

  glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

  f64 result[3];
  f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);

  return V3((f32)result[0], (f32)result[1], (f32)result[2]);
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

#ifndef ATS_OGL33

#include <stdio.h>
#include <stdarg.h>

extern void
gl_init(void) {
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
  gl_init_bitmap_font();
}

extern void
gl_set_simple_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z) {
  f32 pos[4] = { x, y, z, 1.0f };
  f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  f32 c[4] = { bright, bright, bright, 0.0f };
  u32 light = GL_LIGHT0 + index;

  glLightfv(light, GL_POSITION, pos);
  glLightfv(light, GL_DIFFUSE, c);
  glLightfv(light, GL_AMBIENT, zero);
  glLightfv(light, GL_SPECULAR, zero);

  glEnable(light);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

extern void
gl_set_simple_light_directed(int index, f32 bright, f32 x, f32 y, f32 z) {
  f32 d = (f32)(1.0f / sqrt32(x * x + y * y + z * z));
  f32 dir[4] = { x * d, y * d, z * d, 0.0f };
  f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  f32 c[4] = { bright, bright, bright, 0.0f };
  u32 light = GL_LIGHT0 + index;

  glLightfv(light, GL_POSITION, dir);
  glLightfv(light, GL_DIFFUSE, c);
  glLightfv(light, GL_AMBIENT, zero);
  glLightfv(light, GL_SPECULAR, zero);

  glEnable(light);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

extern void
gl_set_light_emitter(int index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic) {
  f32 pos[4] = { p.x, p.y, p.z, 1.0f };
  f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  f32 c[4] = { color.r, color.g, color.b, 0.0f };
  u32 light = GL_LIGHT0 + index;

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

extern void
gl_set_light_directed(int index, v3 pos, v3 color) {
  f32 d = (f32)(1.0f / sqrt32(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));
  f32 dir[4] = { pos.x * d, pos.y * d, pos.z * d, 0.0f };
  f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  f32 c[4] = { color.r, color.g, color.b, 0.0f };
  u32 light = GL_LIGHT0 + index;

  glLightfv(light, GL_POSITION, dir);
  glLightfv(light, GL_DIFFUSE, c);
  glLightfv(light, GL_AMBIENT, zero);
  glLightfv(light, GL_SPECULAR, zero);

  glEnable(light);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

extern void
gl_set_light_global_ambient(f32 r, f32 g, f32 b) {
  f32 v[4] = { r, g, b, 0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

extern gl_texture_t
gl_texture_create(void *pixels, int width, int height, int is_smooth) {
  assert(pixels);

  gl_texture_t texture = {0};

  texture.width = width;
  texture.height = height;

  glGenTextures(1, &texture.id);

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

  return texture;
}

extern void
gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth) {
  texture->width  = width;
  texture->height = height;

  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
}

extern void
gl_texture_bind(const gl_texture_t* texture) {
  glBindTexture(GL_TEXTURE_2D, texture->id);

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);
}

// ======================================= GL ====================================== //

extern void
gl_begin(u32 type) {
  glBegin(type);
}

extern void
gl_end(void) {
  glEnd();
}

extern void
gl_color(u32 color) {
  glColor4ubv((const u8*)&color);
}

extern void
gl_normal(f32 x, f32 y, f32 z) {
  glNormal3f(x, y, z);
}

extern void
gl_uv(f32 x, f32 y) {
  glTexCoord2f(x, y);
}

extern void
gl_vertex(f32 x, f32 y, f32 z) {
  glVertex3f(x, y, z);
}

extern void
gl_set_matrix(m4 projection, m4 view) {
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection.e);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(view.e);
}

extern void
gl_billboard(r2i tex_rect, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up) {
  f32 ax = pos.x - right.x * rad.x - up.x * rad.y;
  f32 ay = pos.y - right.y * rad.x - up.y * rad.y;
  f32 az = pos.z - right.z * rad.x - up.z * rad.y;

  f32 bx = pos.x + right.x * rad.x - up.x * rad.y;
  f32 by = pos.y + right.y * rad.x - up.y * rad.y;
  f32 bz = pos.z + right.z * rad.x - up.z * rad.y;

  f32 cx = pos.x + right.x * rad.x + up.x * rad.y;
  f32 cy = pos.y + right.y * rad.x + up.y * rad.y;
  f32 cz = pos.z + right.z * rad.x + up.z * rad.y;

  f32 dx = pos.x - right.x * rad.x + up.x * rad.y;
  f32 dy = pos.y - right.y * rad.x + up.y * rad.y;
  f32 dz = pos.z - right.z * rad.x + up.z * rad.y;

  gl_color(color);
  gl_normal(normal.x, normal.y, normal.z);

  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(ax, ay, az);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(bx, by, bz);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(cx, cy, cz);

  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(cx, cy, cz);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(dx, dy, dz);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(ax, ay, az);
}

extern void
gl_texture_box(r2i tex_rect, r3 box, u32 color) {
  gl_color(color);

  gl_normal(0, 0, -1);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.min.x, box.max.y, box.min.z);

  gl_normal(0, 0, +1);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.max.z);

  gl_normal(-1, 0, 0);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.min.x, box.max.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.min.x, box.max.y, box.max.z);

  gl_normal(+1, 0, 0);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.max.x, box.max.y, box.min.z);

  gl_normal(0, -1, 0);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.max.x, box.min.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.min.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.min.x, box.min.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.min.y, box.min.z);

  gl_normal(0, +1, 0);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(box.max.x, box.max.y, box.min.z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(box.max.x, box.max.y, box.max.z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(box.min.x, box.max.y, box.min.z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(box.min.x, box.max.y, box.max.z);
}

extern void
gl_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color) {
  gl_color(color);
  gl_normal(0, 0, +1);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(rect.max.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(rect.min.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
}

extern void
gl_texture_rect_flip(r2i tex_rect, r2 rect, f32 z, u32 color, bool flip_x, bool flip_y) {
  if (flip_x) { Swap(i32, tex_rect.min.x, tex_rect.max.x); }
  if (flip_y) { Swap(i32, tex_rect.min.y, tex_rect.max.y); }

  gl_color(color);
  gl_normal(0, 0, +1);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(rect.max.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(rect.min.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
}

extern void
gl_box(r3 box, u32 color) {
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

extern void
gl_rect(r2 rect, f32 z, u32 color) {
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

static gl_texture_t bitmap_texture;

extern void
gl_init_bitmap_font(void) {
  u32 pixels[8][BITMAP_COUNT * 8] = {0};
  for (int i = 0; i < BITMAP_COUNT; ++i) {
    for (int y = 0; y < 8; ++y) {
      for (int x = 0; x < 8; ++x) {
        u64 bit = y * 8 + x;

        if (bitascii[i] & (1ull << bit)) {
          pixels[7 - y][8 * i + x] = 0xffffffff;
        }
      }
    }
  }
  bitmap_texture = gl_texture_create(pixels, BITMAP_COUNT * 8, 8, false);
}

static void
gl_ascii(u8 c, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
  r2 tex_rect = { c * 8 + 0.1, 0.1, c * 8 + 7.9, 7.9 };
  r2 rect = { x, y, x + sx, y + sy };

  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.max.y); gl_vertex(rect.max.x, rect.min.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.max.x, tex_rect.min.y); gl_vertex(rect.max.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.min.y); gl_vertex(rect.min.x, rect.max.y, z);
  gl_uv(tex_rect.min.x, tex_rect.max.y); gl_vertex(rect.min.x, rect.min.y, z);
}

extern void
gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
  glEnable(GL_TEXTURE_2D);
  gl_texture_bind(&bitmap_texture);

  gl_begin(GL_TRIANGLES);
  gl_color(color);
  gl_normal(0, 0, +1);
  for (int i = 0; str[i] != '\0'; i++) {
    gl_ascii(str[i], x + i * sx, y, z, sx, sy);
  }
  gl_end();
}

extern void
gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...) {
  va_list list;
  char buffer[256];

  va_start(list, fmt);
  vsnprintf(buffer, 256, fmt, list);
  gl_string(buffer, x, y, z, sx, sy, color);
  va_end(list);
}

#else // ATS_OGL33

extern void
gl_init(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
  glClearDepth(1.0f);

  glDepthFunc(GL_LESS);

  glEnable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  gl_init_bitmap_font();
}

static u32
gl_shader_compile(const char* source, unsigned int type) {
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

static u32
gl_shader_link_program(u32 vertex_shader, u32 fragment_shader) {
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

extern gl_shader_t
gl_shader_create(const gl_shader_desc_t* desc) {
  u32 vertex = gl_shader_compile(desc->vs, GL_VERTEX_SHADER);
  u32 fragment = gl_shader_compile(desc->fs, GL_FRAGMENT_SHADER);
  u32 program = gl_shader_link_program(vertex, fragment);

  glUseProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  gl_shader_t shader = ATS_INIT;
  shader.id = program;
  return shader;
}

extern gl_shader_t
gl_shader_load_from_file(const char *vs, const char *fs,  mem_allocator_t allocator) {
  char* vs_content = file_read_str(vs, allocator);
  char* fs_content = file_read_str(fs, allocator);

  gl_shader_desc_t desc = ATS_INIT;

  desc.vs = vs_content;
  desc.fs = fs_content;

  gl_shader_t program = gl_shader_create(&desc);

  mem_free(allocator, vs_content);
  mem_free(allocator, fs_content);

  return program;
}

extern void
gl_use(const gl_shader_t* shader) {
  glUseProgram(shader->id);
}

extern u32
gl_location(const gl_shader_t* shader, const char* name) {
  return glGetUniformLocation(shader->id, name);
}

extern void
gl_uniform_i32(u32 location, i32 i) {
  glUniform1i(location, i);
}

extern void
gl_uniform_f32(u32 location, f32 f) {
  glUniform1f(location, f);
}

extern void
gl_uniform_v2(u32 location, v2 u) {
  glUniform2f(location, u.x, u.y);
}

extern void
gl_uniform_v3(u32 location, v3 u) {
  glUniform3f(location, u.x, u.y, u.z);
}

extern void 
gl_uniform_v4(u32 location, v4 u) {
  glUniform4f(location, u.x, u.y, u.z, u.w);
}

extern void
gl_uniform_m2(u32 location, m2 m) {
  glUniformMatrix2fv(location, 1, GL_FALSE, m.e);
}

extern void
gl_uniform_m3(u32 location, m3 m) {
  glUniformMatrix3fv(location, 1, GL_FALSE, m.e);
}

extern void
gl_uniform_m4(u32 location, m4 m) {
  glUniformMatrix4fv(location, 1, GL_FALSE, m.e);
}

extern gl_buffer_t
gl_buffer_create(const gl_buffer_desc_t* desc) {
  u32 vao = 0;
  u32 vbo = 0;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  for (u32 i = 0; i < ArrayCount(desc->layout); ++i) {
    const gl_layout_t* layout = &desc->layout[i];

    if (layout->size) {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i, layout->size, layout->type, layout->normalize, layout->stride, (void*)(u64)layout->offset);
    }
  }

  gl_buffer_t result = {0};

  result.vao = vao;
  result.vbo = vbo;

  return result;
}

extern void
gl_buffer_bind(const gl_buffer_t* buffer) {
  glBindVertexArray(buffer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
}

extern void
gl_buffer_send(const gl_buffer_t* buffer, const void* data, u32 size) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

extern gl_texture_t
gl_texture_create(void *pixels, int width, int height, int is_smooth) {
  assert(pixels);

  gl_texture_t texture = {0};

  texture.width = width;
  texture.height = height;

  glGenTextures(1, &texture.id);

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

  glGenerateMipmap(GL_TEXTURE_2D);

  return texture;
}

extern void
gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth) {
  texture->width  = width;
  texture->height = height;

  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

  glGenerateMipmap(GL_TEXTURE_2D);
}

extern void
gl_texture_bind(const gl_texture_t* texture) {
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

// ======================================= FONT ====================================== //

typedef struct bitmap_vertex {
  v2 pos;
  v2 uv;
  u32 color;
} bitmap_vertex_t;

static gl_texture_t     bitmap_texture;
static gl_shader_t      bitmap_shader;
static gl_buffer_t      bitmap_buffer;

static usize            bitmap_count;
static bitmap_vertex_t  bitmap_array[1024 * 1024];

extern void
gl_init_bitmap_font(void) {
  gl_shader_desc_t shader_desc = ATS_INIT;

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

  bitmap_shader = gl_shader_create(&shader_desc);

  gl_buffer_desc_t buffer_desc = ATS_INIT;

  buffer_desc.layout[0] = Make(gl_layout_t) { 2, GL_FLOAT,          sizeof (bitmap_vertex_t), offsetof(bitmap_vertex_t, pos) };
  buffer_desc.layout[1] = Make(gl_layout_t) { 2, GL_FLOAT,          sizeof (bitmap_vertex_t), offsetof(bitmap_vertex_t, uv) };
  buffer_desc.layout[2] = Make(gl_layout_t) { 4, GL_UNSIGNED_BYTE,  sizeof (bitmap_vertex_t), offsetof(bitmap_vertex_t, color), true };

  bitmap_buffer = gl_buffer_create(&buffer_desc);

  // create and set font texture!
  {
    u32 pixels[8][BITMAP_COUNT * 8] = {0};
    for (int i = 0; i < BITMAP_COUNT; ++i) {
      for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
          u64 bit = y * 8 + x;

          if (bitascii[i] & (1ull << bit)) {
            pixels[7 - y][8 * i + x] = 0xffffffff;
          }
        }
      }
    }

    bitmap_texture = gl_texture_create(pixels, BITMAP_COUNT * 8, 8, false);
  }
}

static void
gl_ascii(u8 c, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
  r2 tex_rect = { c * 8.0f + 0.1f, 0.1f, c * 8.0f + 7.9f, 7.9f };
  r2 rect = { x, y, x + sx, y + sy };

  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.min.x, rect.min.y), V2(tex_rect.min.x, tex_rect.max.y), color };
  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.max.x, rect.min.y), V2(tex_rect.max.x, tex_rect.max.y), color };
  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.max.x, rect.max.y), V2(tex_rect.max.x, tex_rect.min.y), color };
  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.max.x, rect.max.y), V2(tex_rect.max.x, tex_rect.min.y), color };
  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.min.x, rect.max.y), V2(tex_rect.min.x, tex_rect.min.y), color };
  bitmap_array[bitmap_count++] = Make(bitmap_vertex_t) { V2(rect.min.x, rect.min.y), V2(tex_rect.min.x, tex_rect.max.y), color };
}

extern void
gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color) {
  bitmap_count = 0;

  gl_use(&bitmap_shader);
  glActiveTexture(GL_TEXTURE0);
  gl_texture_bind(&bitmap_texture);
  gl_uniform_m4(gl_location(&bitmap_shader, "mvp"), m4_ortho(0, platform.width, platform.height, 0, -1, 1));

  for (int i = 0; str[i] != '\0'; i++) {
    gl_ascii(str[i], x + i * sx, y, z, sx, sy, color);
  }

  gl_buffer_bind(&bitmap_buffer);
  gl_buffer_send(&bitmap_buffer, bitmap_array, bitmap_count * sizeof (bitmap_vertex_t));
  glDrawArrays(GL_TRIANGLES, 0, bitmap_count);
  glUseProgram(0);

  bitmap_count = 0;
}

extern void
gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...) {
  va_list list;
  char buffer[256];

  va_start(list, fmt);
  vsnprintf(buffer, 256, fmt, list);
  gl_string(buffer, x, y, z, sx, sy, color);
  va_end(list);
}

#endif // ATS_OGL33

// -------------------------------------- scoped timer ------------------------------------- //

typedef struct timer_entry {
  const char* name;

  f64 start;
  f64 stop;

  usize depth;
} timer_entry_t;

static usize timer_top;
static timer_entry_t timer_stack[512];

static usize timer_count;
static timer_entry_t timer_array[512];

#define timer_scope(name) defer(timer_start(name), timer_stop())

static void
timer_start(const char* name) {
  timer_entry_t* entry = timer_stack + timer_top++;

  entry->name = name;
  entry->start = timer_get_current();
  entry->stop = 0;
  entry->depth = timer_top - 1;
}

static void
timer_stop(void) {
  timer_entry_t* entry = timer_stack + (--timer_top);

  entry->stop = timer_get_current();
  timer_array[timer_count++] = *entry;
}

static void
timer_reset_all(void) {
  timer_top = 0;
  timer_count = 0;
}

static void
timer_print_result(f32 px, f32 py, f32 sx, f32 sy) {
  i32 y = 0;
  // @TODO: fix render order within scopes!
  for (i32 i = timer_count - 1; i >= 0; --i) {
    timer_entry_t e = timer_array[i];
    gl_string_format(px + 2 * sx * e.depth, py + y * (sy + 1), 0, sx, sy, 0xff77ccff, "%s : %.2f", e.name, 1000.0 * (e.stop - e.start));
    y++;
  }
  timer_reset_all();
}

// ---------------------------------------- file impl -------------------------------------- //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

static usize
file_get_size(FILE* fp) {
  fseek(fp, 0L, SEEK_END);
  usize size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  return size;
}

static FILE*
file_open(const char* path, const char* mode) {
  FILE* file = fopen(path, mode);
  return file;
}

extern char*
file_read_str(const char* file_name, mem_allocator_t allocator) {
  FILE* fp = NULL;
  char* buffer = NULL;
  if (fp = file_open(file_name, "rb")) {
    usize size = file_get_size(fp);
    buffer = (char*)mem_alloc(allocator, size + 1);
    if (buffer) {
      buffer[size] = 0;
      if (fread(buffer, 1, size, fp) == 0) {
        mem_free(allocator, buffer);
        buffer = NULL;
      }
    }
    fclose(fp);
  }
  return buffer;
}

extern b32
file_write_str(const char* file_name, const char* buffer) {
  FILE* fp = NULL;
  if (fp = file_open(file_name, "w")) {
    usize size = strlen(buffer);
    usize n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return false;
}

extern b32
file_append_str(const char* file_name, const char* buffer) {
  FILE* fp = NULL;
  if (fp = file_open(file_name, "a")) {
    size_t size = strlen(buffer);
    size_t n = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return n == size;
  }
  return false;
}

extern b32
file_read_bin(const char* file_name, void* buffer, usize size) {
  FILE *fp = NULL;
  if (fp = file_open(file_name, "rb")) {
    fread(buffer, size, 1, fp);
    fclose(fp);
    return true;
  }
  return false;
} 

extern b32
file_write_bin(const char* file_name, const void* buffer, usize size) {
  FILE *fp = NULL;
  if (fp = file_open(file_name, "wb")) {
    fwrite(buffer, size, 1, fp);
    fclose(fp);
    return 1;
  }
  return false;
}

extern image_t
file_load_image(const char* path) {
  image_t image = {0};
  i32 channels = 0;
  image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
  assert(image.pixels);

  return image;
}

extern void
file_free_image(image_t* img) {
  stbi_image_free(img->pixels);
  *img = Make(image_t) ATS_INIT;
}

typedef struct file_iter {
  char current[MAX_PATH];

  b32 done;
  const char* path;

  HANDLE handle;
  WIN32_FIND_DATAA data;
} file_iter;

static bool
file_iter_is_valid(const file_iter* it) {
  return !it->done;
}

static inline void
file_cstr_concat(char* out, const char* a, const char* b) {
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static void
file_iter_advance(file_iter* it) {
  it->done = !FindNextFileA(it->handle, &it->data);
  if (!it->done) {
    file_cstr_concat(it->current, it->path, it->data.cFileName);
  }
}

static file_iter
file_iter_create(const char* path, const char* ext) {
  if (!path) path = "";
  if (!ext) ext = "*";

  file_iter it = {0};

  it.path = path;

  char find_file_str[MAX_PATH] = {0};
  file_cstr_concat(find_file_str, path, ext);

  it.handle = FindFirstFileA(find_file_str, &it.data);
  it.done = it.handle == INVALID_HANDLE_VALUE;

  if (!it.done) {
    file_cstr_concat(it.current, it.path, it.data.cFileName);
  }

  return it;
}

// ------------------------------------ texture table ------------------------------------- //

typedef struct tt_image_t {
  b32 user_provided;

  image_t img;
  char name[256];
} tt_image_t;

static mem_allocator_t tt_allocator;

static texture_table_t tt_table;

static usize      tt_image_count;
static tt_image_t tt_image_array[1024];

extern texture_table_t*
tt_get_texture_table(void) {
  return &tt_table;
}

extern void
tt_add_image(const char* name, image_t img) {
  tt_image_t data = ATS_INIT;

  data.user_provided = true;
  data.img = img;
  strcpy(data.name, name);

  tt_image_array[tt_image_count++] = data;
}

extern image_t
tt_get_image(void) {
  return tt_table.img;
}

extern r2i
tt_get_rect(tt_id_t id) {
  return tt_table.array[id.index].rect;
}

extern tt_id_t
tt_get_id(const char* name) {
  u32 hash = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;
  while (tt_table.array[index].in_use) {
    if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0)) {
      tt_id_t id = { index };
      return id;
    }
    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }
  assert(false);
  return Make(tt_id_t) ATS_INIT;
}

extern r2i
tt_get(const char* name) {
  return tt_get_rect(tt_get_id(name));
}

static void
_tt_add_entry(const char* name, r2i rect) {
  u32 hash = hash_str(name);
  u16 index = hash % TEXTURE_TABLE_SIZE;

  while (tt_table.array[index].in_use) {
    if ((tt_table.array[index].hash == hash) && (strcmp(tt_table.array[index].name, name) == 0))
      assert(true);

    index = (index + 1) % TEXTURE_TABLE_SIZE;
  }

  tt_entry_t* entry = &tt_table.array[index];

  entry->in_use = true;
  entry->rect = rect;
  entry->hash = hash;

  strcpy_s(entry->name, 64, name);
}

static void
cstr_copy_without_extension(char* out, const char* str) {
  while (*str && *str != '.') {
    *(out++) = *(str++);
  }
  *out = '\0';
}

static void
cstr_concat(char* out, const char* a, const char* b) {
  while (*a) *out++ = *a++;
  while (*b) *out++ = *b++;
  *(out) = '\0';
}

static int
tt_cmp_image(const void* va, const void* vb) {
  tt_image_t* a = (tt_image_t*)va;
  tt_image_t* b = (tt_image_t*)vb;

  int dw = b->img.width  - a->img.width;
  int dh = a->img.height - a->img.height;

  return b->img.width - a->img.width;
}

extern b32
rect_contains_image(r2i rect, image_t image) {
  i32 rect_width  = rect.max.x - rect.min.x;
  i32 rect_height = rect.max.y - rect.min.y;
  return image.width <= rect_width && image.height <= rect_height;
}

extern void
tt_load_from_dir(const char* dir_path) {
  for_iter(file_iter, it, file_iter_create(dir_path, "*.png")) {
    tt_image_t data = ATS_INIT;

    data.img = file_load_image(it.current);
    cstr_copy_without_extension(data.name, it.data.cFileName);
    tt_image_array[tt_image_count++] = data;
  }
}

extern void
tt_begin(int width, int height, mem_allocator_t allocator) {
  tt_allocator = allocator;
  tt_image_count = 0;

  tt_table = Make(texture_table_t) {
    width,
    height, 
    (u32*)mem_zero(tt_allocator, width * height * sizeof (u32)),
  };

  tt_table.array[0].in_use = true;

  for (u32 i = 0; i < width * height; ++i) {
    tt_table.img.pixels[i] = 0xff000000;
  }
}

static usize tt_stack_top; 
static r2i   tt_stack_buf[4096];

static r2i
tt_get_fit(image_t img) {
  u32 j = 0;
  for (j = 0; j < tt_stack_top; ++j) {
    if (rect_contains_image(tt_stack_buf[j], img)) {
      break;
    }
  }
  r2i rect = tt_stack_buf[j];
  tt_stack_buf[j] = tt_stack_buf[--tt_stack_top];
  return rect;
}

extern void
tt_end(void) {
  tt_stack_top = 0;
  tt_stack_buf[tt_stack_top++] = Make(r2i) {
    { 0, 0 },
    { tt_table.img.width - 1, tt_table.img.height - 1 },
  };

  qsort(tt_image_array, tt_image_count, sizeof (tt_image_t), tt_cmp_image);

  for (usize i = 0; i < tt_image_count; ++i) {
    tt_image_t* data = &tt_image_array[i];

    r2i rect = tt_get_fit(data->img);
    v2i size = { data->img.width + 2, data->img.height + 2 };
    v2i offset = rect.min;

    _tt_add_entry(data->name, Make(r2i) {
      { offset.x + 1, offset.y + 1 },
      { offset.x + size.x - 1, offset.y + size.y - 1 },
    });

    for (i32 y = 0; y < data->img.height; ++y) {
      for (i32 x = 0; x < data->img.width; ++x) {
        u32 pixel = image_get(&data->img, x, y);
        image_set(&tt_table.img, x + offset.x + 1, y + offset.y + 1, pixel);
      }
    }

    {
      r2i a = {
        { rect.min.x, rect.min.y + size.y },
        { rect.min.x + size.x, rect.max.y },
      };

      r2i b = {
        { rect.min.x + size.x, rect.min.y },
        { rect.max.x, rect.max.y },
      };

      if (a.min.x + size.x <= rect.max.x && a.min.y + size.y <= rect.max.y) { tt_stack_buf[tt_stack_top++] = a; }
      if (b.min.x + size.x <= rect.max.x && b.min.y + size.y <= rect.max.y) { tt_stack_buf[tt_stack_top++] = b; }
    }

    if (!data->user_provided) {
      file_free_image(&data->img);
    }
  }

  tt_image_count = 0;
  tt_stack_top = 0;
}

// -------------------------------------- animation table --------------------------------------- //

static at_frame_t* at_current_frame = NULL;
static at_animation_t* at_current_animation = NULL;
static at_entity_t* at_current_entity = NULL;

static at_entity_t* at_entity_list = NULL;

static mem_allocator_t at_allocator;

extern void
at_add_entity(const char* name) {
  assert(name);
  at_current_animation = NULL;
  at_current_frame = NULL;
  at_entity_t* entity = mem_type(at_allocator, at_entity_t);
  entity->name = name;
  if (!at_entity_list) {
    at_entity_list = entity;
  } else {
    at_current_entity->next = entity;
  }
  at_current_entity = entity;
}

extern void
at_add_animation(const char* name) {
  assert(name);
  at_current_frame = NULL;
  at_animation_t* animation = mem_type(at_allocator, at_animation_t);
  animation->name = name;

  if (!at_current_entity->animation) {
    at_current_entity->animation = animation;
  } else {
    at_current_animation->next = animation;
  }
  at_current_animation = animation;
}

extern void
at_add_frame(const char* name) {
  assert(name);
  at_frame_t* frame = mem_type(at_allocator, at_frame_t);
  frame->name = name;
  frame->rect = tt_get(name);
  frame->animation = at_current_animation;
  if (!at_current_animation->frame) {
    at_current_animation->frame = frame;
  } else {
    at_current_frame->next = frame;
  }
  at_current_frame = frame;
  at_current_frame->next = at_current_animation->frame;
}

extern void
at_begin(mem_allocator_t allocator) {
  at_allocator = allocator;

  at_entity_list = NULL;
  at_current_entity = NULL;
  at_current_frame = NULL;
  at_current_animation = NULL;
}

extern void
at_end(void) {
  // @NOTE: do some cool shit here!
}

static inline b32
at_cstr_equal(const char* a, const char* b) {
  while (*a && *a == *b) {
    a++;
    b++;
  }
  return *a == *b;
}

extern void
at_set(at_asset_t* asset, const char* name) {
  if (at_cstr_equal(asset->frame->animation->name, name)) return;

  at_animation_t* animation = asset->entity->animation;
  while (animation && !at_cstr_equal(animation->name, name)) {
    animation = animation->next;
  }

  if (animation) {
    asset->frame = animation->frame;
    asset->duration = 0;
  }
}

extern void
at_update(at_asset_t* asset, f32 dt) {
  asset->duration += dt;
  if (asset->duration >= 1.0) {
    asset->frame = asset->frame->next;
    asset->duration = 0;
  }
}

static at_entity_t*
at_get_entity(const char* name) {
  at_entity_t* entity = at_entity_list;
  while (entity && !at_cstr_equal(entity->name, name)) {
    entity = entity->next;
  }
  return entity? entity : NULL;
}

extern at_asset_t
at_get(const char* name) {
  at_asset_t state = ATS_INIT;
  state.entity = at_get_entity(name);
  state.frame = state.entity->animation->frame;
  return state;
}

// ---------------------------------------- audio table --------------------------------------- //

#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

static const char* audio_path = AUDIO_PATH;

typedef struct {
  b32 in_use;
  cs_loaded_sound_t loaded;
  cs_play_sound_def_t playing;
  char name[64];
} audio_entry_t;

static struct {
  cs_context_t* context;
} audio;

static audio_entry_t audio_table[AUDIO_TABLE_SIZE];

extern void
audio_init(void* handle) {
  audio.context = cs_make_context(handle, 44100, 8 * 4096, 1024, NULL);

  cs_spawn_mix_thread(audio.context);
  cs_thread_sleep_delay(audio.context, 16);
}

static b32
audio_is_valid(audio_id_t id) {
  return id.index != 0;
}

extern audio_id_t
audio_get(const char* name) {
  u32 hash = hash_str(name);
  u16 index = hash & (AUDIO_TABLE_SIZE - 1);

  if (index == 0) index++;
  while (audio_table[index].in_use) {
    if (strcmp(audio_table[index].name, name) == 0) {
      audio_id_t id = { index };
      return id;
    }
    index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
    if (index == 0) index++;
  }

  char path[512] = {0};

  {
    int i = 0;
    for (i = 0; audio_path[i]; ++i) { path[i] = audio_path[i]; }
    for (int j = 0; name[j]; ++j, ++i) { path[i] = name[j]; }
    path[i++] = '.';
    path[i++] = 'w';
    path[i++] = 'a';
    path[i++] = 'v';
    path[i++] = '\0';
  }

  audio_entry_t* entry = &audio_table[index];

  entry->in_use = true;
  strcpy_s(entry->name, ArrayCount(entry->name), name);

  entry->loaded  = cs_load_wav(path);
  entry->playing = cs_make_def(&entry->loaded);

  if (cs_error_reason) {
    printf("%s ---- path: %s\n", cs_error_reason, path);
  }

  audio_id_t id = { index };
  return id;
}

extern void
audio_pause(b32 pause) {
  cs_lock(audio.context);
  cs_playing_sound_t* playing = cs_get_playing(audio.context);
  while (playing) {
    cs_pause_sound(playing, pause);
    playing = playing->next;
  }
  cs_unlock(audio.context);
}

extern void
audio_kill_all(void) {
  cs_stop_all_sounds(audio.context);
}

static audio_entry_t*
audio_get_entry(audio_id_t id) {
  if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;
  return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

extern void
audio_play(audio_id_t id, f32 volume) {
  audio_entry_t* entry = audio_get_entry(id);

  if (entry) {
    cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);

    if (!playing) return;

    cs_lock(audio.context);
    cs_set_volume(playing, volume, volume);
    cs_unlock(audio.context);
  }
}

extern void*
audio_play_looped(audio_id_t id, f32 volume) {
  audio_entry_t* entry = audio_get_entry(id);

  if (entry) {
    cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);

    if (!playing)
      return NULL;

    cs_lock(audio.context);
    playing->looped = 1;
    cs_set_volume(playing, volume, volume);
    cs_unlock(audio.context);

    return playing;
  }

  return NULL;
}

extern void
audio_play_music(audio_id_t id, f32 volume) {
  static cs_playing_sound_t* playing = NULL;

  if (playing && cs_is_active(playing))
    cs_stop_sound(playing);

  audio_entry_t* entry = audio_get_entry(id);
  if (entry) {
    playing = cs_play_sound(audio.context, entry->playing);

    if (!playing) return;

    cs_lock(audio.context);

    playing->looped = true;

    cs_set_volume(playing, volume, volume);
    cs_unlock(audio.context);
  }
}

extern void
audio_play_from_source(audio_id_t id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance) {
  f32 sound_distance = v3_dist(pos, source);
  f32 final_volume = volume * Max(1 - sound_distance / max_distance, 0);

  if (final_volume <= 0) return;

  audio_entry_t* entry = audio_get_entry(id);

  if (entry) {
    v2 source_dir = {
      source.x - pos.x,
      source.y - pos.y,
    };

    source_dir = v2_norm(source_dir);

    f32 pan = v2_get_angle(dir.xy, source_dir) / PI;

    if (pan > +0.5f) pan = 1.0f - pan;
    if (pan < -0.5f) pan =-1.0f - pan;

    pan += 0.5f;
    pan = 1.0f - pan;

    cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);

    if (!playing) return;

    cs_lock(audio.context);

    cs_set_pan(playing, pan);
    cs_set_volume(playing, final_volume, final_volume);

    cs_unlock(audio.context);
  }
}

#endif // ATS_IMPL_ONCE
#endif // ATS_IMPL

