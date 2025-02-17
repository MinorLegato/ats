#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#ifdef ATS_STATIC
#define ATS_API static
#else
#define ATS_API extern
#endif

#define PI  (3.14159265359f)
#define TAU (6.28318530718f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define to_rad(deg) ((deg) * TO_RAD_MUL)
#define to_deg(rad) ((rad) * TO_DEG_MUL)

#define swap(type, a, b) do { type tmp = (a); (a) = (b); (b) = tmp; } while(0)

#define countof(array) (sizeof (array) / sizeof (array)[0])

#define is_power_of_two(x) (((x) != 0) && ((x) & ((x) - 1)) == 0)

#define align_down(n, a)      ((n) & ~((a) - 1))
#define align_up(n, a)        align_down((n) + (a) - 1, (a))
#define align_down_ptr(p, a)  ((void*)align_down((uintptr_t)(p), (a)))
#define align_up_ptr(p, a)    ((void*)align_down((uintptr_t)(p), (a)))

#define clamp_min(n, min)      ((n) < (min)? (min) : (n))
#define clamp_max(n, max)      ((n) > (max)? (max) : (n))
#define clamp(n, min, max)    clamp_max(clamp_min(n, min), max)
#define clamp01(n)            clamp(n, 0, 1)
#undef min
#define min(a, b)             ((a) < (b)? (a) : (b))
#undef max
#define max(a, b)             ((a) > (b)? (a) : (b))
#define lerp(a, b, t)         ((a) + (t) * ((b) - (a)))
#define sign(n)               ((n) == 0? 0 : ((n) < 0? -1 : 1))

#define join_helper(a, b) a##b
#define join_token(a, b) join_helper(a, b)
#define macro_var(a) join_token(a, __LINE__)

#define scope_guard(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_range(index, start, below) \
  for (isize index = (start); index < (below); ++index)

#define for_array(index, array) \
  for (usize index = 0; index < countof(array); ++index)

#define repeat(count) \
  for (isize macro_var(index) = (count); macro_var(index) > 0; --macro_var(index))

#define for_r2(rect, ix, iy) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_r3(rect, ix, iy, iz) \
  for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_iter(iter_type, iter_name, ...) \
  for (iter_type iter_name = (__VA_ARGS__); \
       iter_type##_is_valid(&iter_name); \
       iter_type##_advance(&iter_name))

#define for_each(type, var, ...) \
  for (type* var = (type*)0xdeadbeefull; var != NULL; var = NULL) \
  for (type##_iter macro_var(it) = (__VA_ARGS__); \
       (var = macro_var(it).current, type##_iter_is_valid(&macro_var(it))); \
       type##_iter_advance(&macro_var(it)))

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

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

typedef i64 isize;
typedef u64 usize;

#define cast(type, ...)   ((type)(__VA_ARGS__))

#define castf32(...)      cast(f32, __VA_ARGS__)
#define castf64(...)      cast(f64, __VA_ARGS__)

#define casti8(...)       cast(i8,  __VA_ARGS__)
#define casti16(...)      cast(i16, __VA_ARGS__)
#define casti32(...)      cast(i32, __VA_ARGS__)
#define casti64(...)      cast(i64, __VA_ARGS__)

#define castu8(...)       cast(u8,  __VA_ARGS__)
#define castu16(...)      cast(u16, __VA_ARGS__)
#define castu32(...)      cast(u32, __VA_ARGS__)
#define castu64(...)      cast(u64, __VA_ARGS__)

// shorthand
#define castf(...)        cast(f32, __VA_ARGS__)
#define casti(...)        cast(i32, __VA_ARGS__)
#define castu(...)        cast(u32, __VA_ARGS__)

#define sort(array, count, cmp_func) qsort((array), (count), sizeof ((array)[0]), (cmp_func))

#define __block(...)            do { __VA_ARGS__; } while (0)

#define arr_push(a, ...)        ((a)->array[(a)->count++] = (__VA_ARGS__), arr_end(a) - 1)
#define arr_pop(a)              (a)->array[(a)->count--]
#define arr_new(a)              memset((a)->array + (a)->count++, 0, sizeof (a)->array[0])
#define arr_remove(a, element)  __block((a)->array[(element) - arr_begin(a)] = (a)->array[--(a)->count]; --(element))
#define arr_sort(a, cmp_func)   sort((a)->array, (a)->count, (cmp_func))
#define arr_begin(a)            (a)->array
#define arr_end(a)              (a)->array + (a)->count
#define arr_get(a, index)       ((a)->array + (index))

#define for_arr(type, var, array) for (type* var = arr_begin(array); var < arr_end(array); ++var)

// ================================================= MATH =========================================== //
// ------------------------------------- implementation in ats_math.c ------------------------------- //
// ===================================================================================================//

#define castv2(u)  ((v2) { (f32)(u).x, (f32)(u).y })
#define castv3(u)  ((v3) { (f32)(u).x, (f32)(u).y, (f32)(u).z })
#define castv4(u)  ((v4) { (f32)(u).x, (f32)(u).y, (f32)(u).z, (f32)(u).w })

#define castv2i(u) ((v2i) { (i32)(u).x, (i32)(u).y })
#define castv3i(u) ((v3i) { (i32)(u).x, (i32)(u).y, (i32)(u).z })
#define castv4i(u) ((v4i) { (i32)(u).x, (i32)(u).y, (i32)(u).z, (i32)(u).w })

#define v2(...) (v2) { __VA_ARGS__ }
#ifdef __clang__
typedef f32 v2 __attribute__((ext_vector_type(2)));
#else
typedef union v2 v2;
union v2
{
  struct { f32 x, y; };
  f32 e[2];
};
#endif

#define v3(...) (v3) { __VA_ARGS__ }
#ifdef __clang__
typedef f32 v3 __attribute__((ext_vector_type(3)));
#else
typedef union v3 v3;
union v3
{
  struct { f32 x, y, z; };
  struct { f32 r, g, b; };
  struct { v2 xy; };
  struct { f32 _x; v2 yz; };
  f32 e[3];
};
#endif

#define v4(...) (v4) { __VA_ARGS__ }
#ifdef __clang__
typedef f32 v4 __attribute__((ext_vector_type(4)));
typedef f32 v4 __attribute__((ext_vector_type(4)));
#else
typedef union v4 v4;
union v4
{
  struct { f32 x, y, z, w; };
  struct { f32 r, g, b, a; };
  struct { v3 rgb; };
  struct { v2 xy; };
  struct { v3 xyz; };
  f32 e[4];
};
#endif

#define quat(...) (quat) { __VA_ARGS__ }
typedef v4 quat;

#define v2i(...) (v2i) { __VA_ARGS__ }
#ifdef __clang__
typedef i32 v2i __attribute__((ext_vector_type(2)));
#else
typedef union v2i v2i;
union v2i
{
  struct { i32 x, y; };
  i32 e[2];
};
#endif

#define v3i(...) (v3i) { __VA_ARGS__ }
#ifdef __clang__
typedef i32 v3i __attribute__((ext_vector_type(3)));
#else
typedef union v3i v3i;
union v3i
{
  struct { i32 x, y, z; };
  struct { v2i xy; };
  i32 e[3];
};
#endif

#define v4i(...) (v4i) { __VA_ARGS__ }
#ifdef __clang__
typedef i32 v4i __attribute__((ext_vector_type(4)));
#else
typedef union v4i v4i;
union v4i
{
  struct { i32 x, y, z, w; };
  i32 e[4];
};
#endif

#define m2(...) (m2) { __VA_ARGS__ }
typedef union m2 m2;
union m2
{
  f32 e[4];
  struct { v2 x, y; };
};

#define m3(...) (m3) { __VA_ARGS__ }
typedef union m3 m3;
union m3
{
  f32 e[9];
  struct { v3 x, y, z; };
};

#define m4(...) (m4) { __VA_ARGS__ }
typedef union m4 m4;
union m4
{
  f32 e[16];
  struct { v4 x, y, z, w; };
};

#define r2(...) (r2) { __VA_ARGS__ }
typedef struct r2 r2;
struct r2
{
  v2 min;
  v2 max;
};

#define r3(...) (r3) { __VA_ARGS__ }
typedef struct r3 r3;
struct r3
{
  v3 min;
  v3 max;
};

#define r2i(...) (r2i) { __VA_ARGS__ }
typedef struct r2i r2i;
struct r2i
{
  v2i min;
  v2i max;
};

#define r3i(...) (r3i) { __VA_ARGS__ }
typedef struct r3i r3i;
struct r3i
{
  v3i min;
  v3i max;
};

#define circle(...) (circle) { __VA_ARGS__ }
typedef struct circle circle;
struct circle
{
  v2 p;
  f32 r;
};

#define sphere(...) (sphere) { __VA_ARGS__ }
typedef struct sphere sphere;
struct sphere {
  v3 p;
  f32 r;
};

typedef struct frustum_plane frustum_plane;
struct frustum_plane
{
  f32 a;
  f32 b;
  f32 c;
  f32 d;
};

typedef struct frustum frustum;
struct frustum
{
  frustum_plane planes[6];
};


ATS_API f32 sqrt32(f32 n);
ATS_API f32 rsqrt32(f32 n);
ATS_API i32 absi(i32 x);
ATS_API f32 cos_turn(f32 turns);
ATS_API f32 sin_turn(f32 turns);
ATS_API f32 cos_turn01(f32 turns);
ATS_API f32 sin_turn01(f32 turns);
ATS_API f32 shortest_angle_distance(f32 a, f32 b);
ATS_API f32 lerp_angle(f32 a, f32 b, f32 t);
ATS_API f32 sine_ease_in(f32 t);
ATS_API f32 sine_ease_out(f32 t);
ATS_API f32 sine_ease_in_out(f32 t);
ATS_API f32 quad_ease_in(f32 t);
ATS_API f32 quad_ease_out(f32 t);
ATS_API f32 quad_ease_in_out(f32 t);
ATS_API f32 cubic_ease_in(f32 t);
ATS_API f32 cubic_ease_out(f32 t);
ATS_API f32 cubic_ease_in_out(f32 t);
ATS_API f32 quart_ease_in(f32 t);
ATS_API f32 quart_ease_out(f32 t);
ATS_API f32 quart_ease_in_out(f32 t);
ATS_API f32 quint_ease_in(f32 t);
ATS_API f32 quint_ease_out(f32 t);
ATS_API f32 quint_ease_in_out(f32 t);
ATS_API f32 expo_ease_in(f32 t);
ATS_API f32 expo_ease_out(f32 t);
ATS_API f32 expo_ease_in_out(f32 t);
ATS_API f32 circ_ease_in(f32 t);
ATS_API f32 circ_ease_out(f32 t);
ATS_API f32 circ_ease_in_out(f32 t);
ATS_API f32 back_ease_in(f32 t);
ATS_API f32 back_ease_out(f32 t);
ATS_API f32 back_ease_in_out(f32 t);
ATS_API f32 elastic_ease_in(f32 t);
ATS_API f32 elastic_ease_out(f32 t);
ATS_API f32 elastic_ease_in_out(f32 t);
ATS_API f32 bounce_ease_out(f32 t);
ATS_API f32 bounce_ease_in(f32 t);
ATS_API f32 bounce_ease_in_out(f32 t);

ATS_API f32 spline(f32 f, f32 a, f32 b, f32 c, f32 d);

ATS_API m2 m2_identity(void);
ATS_API m3 m3_identity(void);
ATS_API m4 m4_identity(void);

ATS_API quat quat_identity(void);

ATS_API frustum frustum_create(m4 m);

ATS_API v2 v2_from_array(const f32* a);
ATS_API v3 v3_from_array(const f32* a);
ATS_API v4 v4_from_array(const f32* a);

ATS_API v2i v2i_from_array(const i32* a);
ATS_API v3i v3i_from_array(const i32* a);
ATS_API v4i v4i_from_array(const i32* a);

ATS_API v3 v3_unpack_color(u32 color);
ATS_API v4 v4_unpack_color(u32 color);

ATS_API v2 v2_neg(v2 u);
ATS_API v3 v3_neg(v3 u);
ATS_API v4 v4_neg(v4 u);

ATS_API v2i v2i_neg(v2i u);
ATS_API v3i v3i_neg(v3i u);
ATS_API v4i v4i_neg(v4i u);

ATS_API v2 v2_add(v2 a, v2 b);
ATS_API v3 v3_add(v3 a, v3 b);
ATS_API v4 v4_add(v4 a, v4 b);

ATS_API v2i v2i_add(v2i a, v2i b);
ATS_API v3i v3i_add(v3i a, v3i b);
ATS_API v4i v4i_add(v4i a, v4i b);

ATS_API v2 v2_sub(v2 a, v2 b);
ATS_API v3 v3_sub(v3 a, v3 b);
ATS_API v4 v4_sub(v4 a, v4 b);

ATS_API v2i v2i_sub(v2i a, v2i b);
ATS_API v3i v3i_sub(v3i a, v3i b);
ATS_API v4i v4i_sub(v4i a, v4i b);

ATS_API v2 v2_mul(v2 a, v2 b);
ATS_API v3 v3_mul(v3 a, v3 b);
ATS_API v4 v4_mul(v4 a, v4 b);

ATS_API v2i v2i_mul(v2i a, v2i b);
ATS_API v3i v3i_mul(v3i a, v3i b);
ATS_API v4i v4i_mul(v4i a, v4i b);

ATS_API v2 m2_mulv(m2 m, v2 u);
ATS_API v3 m3_mulv(m3 m, v3 u);
ATS_API v4 m4_mulv(m4 m, v4 u);

ATS_API m2 m2_mul(m2 a, m2 b);
ATS_API m3 m3_mul(m3 a, m3 b);
ATS_API m4 m4_mul(m4 a, m4 b);

ATS_API quat quat_mul(quat a, quat b);

ATS_API v2 v2_div(v2 a, v2 b);
ATS_API v3 v3_div(v3 a, v3 b);
ATS_API v4 v4_div(v4 a, v4 b);

ATS_API v2i v2i_div(v2i a, v2i b);
ATS_API v3i v3i_div(v3i a, v3i b);
ATS_API v4i v4i_div(v4i a, v4i b);

ATS_API v2 v2_scale(v2 a, f32 s);
ATS_API v3 v3_scale(v3 a, f32 s);
ATS_API v4 v4_scale(v4 a, f32 s);

ATS_API v2i v2i_scale(v2i a, i32 s);
ATS_API v3i v3i_scale(v3i a, i32 s);
ATS_API v4i v4i_scale(v4i a, i32 s);

ATS_API b32 v2i_eq(v2i a, v2i b);
ATS_API b32 v3i_eq(v3i a, v3i b);
ATS_API b32 v4i_eq(v4i a, v4i b);

ATS_API b32 v2i_neq(v2i a, v2i b);
ATS_API b32 v3i_neq(v3i a, v3i b);
ATS_API b32 v4i_neq(v4i a, v4i b);

ATS_API f32 v2_dot(v2 a, v2 b);
ATS_API f32 v3_dot(v3 a, v3 b);
ATS_API f32 v4_dot(v4 a, v4 b);
ATS_API i32 v2i_dot(v2i a, v2i b);
ATS_API i32 v3i_dot(v3i a, v3i b);
ATS_API i32 v4i_dot(v4i a, v4i b);

ATS_API f32 v2_len_sq(v2 u);
ATS_API f32 v3_len_sq(v3 u);
ATS_API f32 v4_len_sq(v4 u);

ATS_API i32 v2i_len_sq(v2i u);
ATS_API i32 v3i_len_sq(v3i u);
ATS_API i32 v4i_len_sq(v4i u);

ATS_API f32 v2_len(v2 u);
ATS_API f32 v3_len(v3 u);
ATS_API f32 v4_len(v4 u);

ATS_API f32 v2i_len(v2i u);
ATS_API f32 v3i_len(v3i u);
ATS_API f32 v4i_len(v4i u);

ATS_API f32 v2_dist_sq(v2 a, v2 b);
ATS_API f32 v3_dist_sq(v3 a, v3 b);
ATS_API f32 v4_dist_sq(v4 a, v4 b);
ATS_API i32 v2i_dist_sq(v2i a, v2i b);
ATS_API i32 v3i_dist_sq(v3i a, v3i b);
ATS_API i32 v4i_dist_sq(v4i a, v4i b);

ATS_API f32 v2_dist(v2 a, v2 b);
ATS_API f32 v3_dist(v3 a, v3 b);
ATS_API f32 v4_dist(v4 a, v4 b);

ATS_API i32 v2i_manhattan(v2i a, v2i b);
ATS_API i32 v3i_manhattan(v3i a, v3i b);

ATS_API v2 v2_norm(v2 u);
ATS_API v3 v3_norm(v3 u);
ATS_API v4 v4_norm(v4 u);

ATS_API v2 v2_project(v2 a, v2 b);
ATS_API v3 v3_project(v3 a, v3 b);

ATS_API v2 v2_floor(v2 u);
ATS_API v3 v3_floor(v3 u);
ATS_API v4 v4_floor(v4 u);

ATS_API v2 v2_ceil(v2 u);
ATS_API v3 v3_ceil(v3 u);
ATS_API v4 v4_ceil(v4 u);

ATS_API v2 v2_clampf(v2 u, f32 min, f32 max);
ATS_API v3 v3_clampf(v3 u, f32 min, f32 max);

ATS_API v2i v2i_clampi(v2i u, f32 min, f32 max);
ATS_API v3i v3i_clampi(v3i u, f32 min, f32 max);

ATS_API v2 v2_clamp(v2 u, r2 r);
ATS_API v3 v3_clamp(v3 u, r3 r);

ATS_API v2i v2i_clamp(v2i u, r2i r);
ATS_API v3i v3i_clamp(v3i u, r3i r);

ATS_API v2 v2_min(v2 a, v2 b);
ATS_API v3 v3_min(v3 a, v3 b);
ATS_API v4 v4_min(v4 a, v4 b);

ATS_API v2i v2i_min(v2i a, v2i b);
ATS_API v3i v3i_min(v3i a, v3i b);
ATS_API v4i v4i_min(v4i a, v4i b);

ATS_API v2 v2_max(v2 a, v2 b);
ATS_API v3 v3_max(v3 a, v3 b);
ATS_API v4 v4_max(v4 a, v4 b);

ATS_API v2i v2i_max(v2i a, v2i b);
ATS_API v3i v3i_max(v3i a, v3i b);
ATS_API v4i v4i_max(v4i a, v4i b);

ATS_API v2 v2_lerp(v2 a, v2 b, f32 t);
ATS_API v3 v3_lerp(v3 a, v3 b, f32 t);
ATS_API v4 v4_lerp(v4 a, v4 b, f32 t);

ATS_API v2 v2_spline(f32 f, v2 a, v2 b, v2 c, v2 d);
ATS_API v3 v3_spline(f32 f, v3 a, v3 b, v3 c, v3 d);
ATS_API v4 v4_spline(f32 f, v4 a, v4 b, v4 c, v4 d);

ATS_API v2 v2_sign(v2 u);
ATS_API v3 v3_sign(v3 u);
ATS_API v4 v4_sign(v4 u);

ATS_API v2i v2i_sign(v2i u);
ATS_API v3i v3i_sign(v3i u);
ATS_API v4i v4i_sign(v4i u);

ATS_API v3 v3_cross(v3 a, v3 b);

ATS_API f32 v2_get_angle(v2 a, v2 b);
ATS_API v2  v2_from_angle(f32 angle);

ATS_API v2 v2_keep_min(v2 u);
ATS_API v3 v3_keep_min(v3 u);

ATS_API v2 v2_mask_min(v2 u);
ATS_API v3 v3_mask_min(v3 u);

ATS_API m2 m2_rotate(f32 angle);
ATS_API m3 m3_rotate(v3 axis, f32 angle);
ATS_API m4 m4_rotate(v3 axis, f32 angle);

ATS_API quat quat_conj(quat q);
ATS_API quat quat_rotate(v3 axis, f32 angle);
ATS_API v3 quat_mulv(quat q, v3 u);

ATS_API m4 m4_translate(f32 x, f32 y, f32 z);
ATS_API m4 m4_scale(f32 x, f32 y, f32 z);

ATS_API m3 m3_from_quat(quat q);
ATS_API m4 m4_from_quat(quat q);

ATS_API m4 m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
ATS_API m4 m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f);
ATS_API m4 m4_look_at(v3 eye, v3 center, v3 up);

ATS_API b32 circle_contains(circle c, v2 pos);
ATS_API b32 sphere_contains(sphere s, v3 pos);
ATS_API b32 r2_contains(r2 rect, v2 pos);
ATS_API b32 r3_contains(r3 rect, v3 pos);
ATS_API b32 r2i_contains(r2i rect, v2i pos);
ATS_API b32 r3i_contains(r3i rect, v3i pos);
ATS_API b32 frustum_contains(frustum fs, v3 pos);

ATS_API b32 circle_intersect(circle a, circle b);
ATS_API b32 sphere_intersect(sphere a, sphere b);
ATS_API b32 r2_intersect(r2 a, r2 b);
ATS_API b32 r3_intersect(r3 a, r3 b);
ATS_API b32 r2i_intersect(r2i a, r2i b);
ATS_API b32 r3i_intersect(r3i a, r3i b);
ATS_API b32 frustum_intersect_sphere(frustum fs, sphere s);
ATS_API b32 frustum_intersect_r3(frustum fs, r3 rect);

ATS_API r2 r2_get_overlap(r2 a, r2 b);
ATS_API r3 r3_get_overlap(r3 a, r3 b);
ATS_API r2i r2i_get_overlap(r2i a, r2i b);
ATS_API r3i r3i_get_overlap(r3i a, r3i b);

ATS_API v2 circle_get_intersect_vector(circle a, circle b);
ATS_API v3 sphere_get_intersect_vector(sphere a, sphere b);
ATS_API v2 r2_get_intersect_vector(r2 a, r2 b);
ATS_API v3 r3_get_intersect_vector(r3 a, r3 b);
ATS_API v2i r2i_get_intersect_vector(r2i a, r2i b);
ATS_API v3i r3i_get_intersect_vector(r3i a, r3i b);

ATS_API u32 rand_u32(u32* state);
ATS_API i32 rand_i32(u32* state, i32 min, i32 max);
ATS_API f32 rand_f32(u32* state, f32 min, f32 max);
ATS_API v2  rand_unit_v2(u32* state);
ATS_API v3  rand_unit_v3(u32* state);
ATS_API v2  rand_v2(u32* state, f32 min, f32 max);
ATS_API v3  rand_v3(u32* state, f32 min, f32 max);

ATS_API u32 crc32(const void *data, u32 size);

ATS_API u32 hash_str(const char* str);
ATS_API u32 hashu(u32 a);
ATS_API u32 hashi(i32 a);
ATS_API u32 hash2u(u32 x, u32 y);
ATS_API u32 hash3u(u32 x, u32 y, u32 z);
ATS_API u32 hash4u(u32 x, u32 y, u32 z, u32 w);
ATS_API u32 hash2i(i32 x, i32 y);
ATS_API u32 hash3i(i32 x, i32 y, i32 z);
ATS_API u32 hash4i(i32 x, i32 y, i32 z, i32 w);
ATS_API u32 hash_v2i(v2i k);
ATS_API u32 hash_v3i(v3i k);
ATS_API u32 hash_v4i(v4i k);

ATS_API u32 pack_color_u8(u8 r, u8 g, u8 b, u8 a);
ATS_API u32 pack_color_f32(f32 r, f32 g, f32 b, f32 a);
ATS_API u32 pack_color_v4(v4 color);
ATS_API u32 pack_color_v3(v3 color, f32 a);

ATS_API void f4x4_mul_64(f64 *R, const f64 *a, const f64 *b);
ATS_API void f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v);
ATS_API void f4x4_invert_64(f64* T, const f64* M);
ATS_API b32 f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport);
ATS_API b32 f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport);

ATS_API m4 m4_invert(m4 m); // assumes it is invertible

// ================================================= MEM =========================================== //
// ------------------------------------- implementation in ats_mem.c ------------------------------- //
// ================================================================================================= //

#define MEM_KIB(n) (1024 * (n))
#define MEM_MIB(n) (1024 * MEM_KIB(n))
#define MEM_GIB(n) (1024 * MEM_MIB(n))

typedef struct mem_index mem_index;
struct mem_index
{
  usize pos;
  mem_index* next;
};

typedef struct mem_arena mem_arena;
struct mem_arena
{
  usize pos;
  usize cap;
  u8* buf;

  mem_index* stack;
  mem_arena* next;
};

ATS_API mem_arena mem_create(void* data, usize size);
ATS_API void mem_push(mem_arena* arena);
ATS_API void mem_pop(void);

#define mem_alloc(...)                  mem__alloc((struct mem__alloc_desc) { __VA_ARGS__ })
#define mem_type(type, ...)             (type*)mem_alloc((sizeof (type)), 0, __VA_ARGS__)
#define mem_array(type, count, ...)     (type*)mem_alloc(((count) * sizeof (type)), (usize)(count), __VA_ARGS__)

#define mem_context(arena)      scope_guard(mem_push(arena), mem_pop())
#define mem_save(...)           mem__save((struct mem__arena_desc) { 0, __VA_ARGS__ })
#define mem_restore(...)        mem__restore((struct mem__arena_desc) { 0, __VA_ARGS__ })
#define mem_begin(...)          mem__begin((struct mem__arena_desc) { 0, __VA_ARGS__ })
#define mem_end(size, ...)      mem__end((size), (struct mem__arena_desc) { 0, __VA_ARGS__ })
#define mem_temp                mem_begin
#define mem_scope(...)          scope_guard(mem__save((struct mem__arena_desc) { 0, __VA_ARGS__ }), mem__restore((struct mem__arena_desc) { 0, __VA_ARGS__ }))
#define mem_size(ptr)           ((mem_header*)(ptr) - 1)->size
#define mem_count(ptr)          ((mem_header*)(ptr) - 1)->count

// -------------------- internal --------------- //

struct mem__arena_desc
{
  usize pad;
  mem_arena* arena;
};

struct mem__alloc_desc
{
  usize size;
  usize count;
  mem_arena* arena;
};

ATS_API void* mem__alloc(struct mem__alloc_desc desc);
ATS_API void  mem__save(struct mem__arena_desc desc);
ATS_API void  mem__restore(struct mem__arena_desc desc);
ATS_API void* mem__begin(struct mem__arena_desc desc);
ATS_API void  mem__end(usize size, struct mem__arena_desc desc);

// ================================================= DS ============================================= //
// ------------------------------------- implementation in ats_ds.c --------------------------------- //
// ================================================================================================== //

ATS_API void bit_set(u32* array, u32 index);
ATS_API b32  bit_get(u32* array, u32 index);
ATS_API void bit_clr(u32* array, u32 index);

#define STR_ITER_TABLE (256 >> 5)
typedef struct
{
  char* current;
  char* end;

  char saved;

  u32 del_table[STR_ITER_TABLE];
  u32 sep_table[STR_ITER_TABLE];
} str_iter;

ATS_API str_iter str_iter_create(char* content, const char* delimiters, const char* separators);
ATS_API b32 str_iter_is_valid(str_iter* it);
ATS_API void str_iter_advance(str_iter* it);

#define str_iter(var, ...) \
  for (char* var = "start"; var; var = 0) \
  for (str_iter macro_var(it) = str_iter_create(__VA_ARGS__); \
       (var = macro_var(it).current, str_iter_is_valid(&macro_var(it))); \
       str_iter_advance(&macro_var(it)))

typedef struct
{
  v2 pos;
  v2 dir;

  //which box of the map we're in
  i32 map_x;
  i32 map_y;

  //length of ray from current position to next x or y-side
  f32 side_dist_x;
  f32 side_dist_y;

  //length of ray from one x or y-side to next x or y-side
  f32 delta_dist_x;
  f32 delta_dist_y;

  //what direction to step in x or y-direction (either +1 or -1)
  i32 step_x;
  i32 step_y;

  i32 side; // was a NS or a EW wall hit?
} ray_iter;

ATS_API ray_iter ray_iter_create(v2 pos, v2 dir);
ATS_API b32 ray_iter_is_valid(ray_iter* it);
ATS_API void ray_iter_advance(ray_iter* it);
ATS_API v2 ray_iter_get_position(ray_iter* it);
ATS_API v2 ray_iter_get_normal(ray_iter* it);

typedef struct
{
  v3 pos;
  v3 dir;

  //which box of the map we're in
  i32 map_x;
  i32 map_y;
  i32 map_z;

  //length of ray from current position to next x or y-side
  f32 side_dist_x;
  f32 side_dist_y;
  f32 side_dist_z;

  //length of ray from one x or y-side to next x or y-side
  f32 delta_dist_x;
  f32 delta_dist_y;
  f32 delta_dist_z;

  //what direction to step in x or y-direction (either +1 or -1)
  i32 step_x;
  i32 step_y;
  i32 step_z;

  i32 side; // was a NS or a EW wall hit?
} ray3_iter;

ATS_API ray3_iter ray3_iter_create(v3 pos, v3 dir);
ATS_API b32 ray3_iter_is_valid(ray3_iter* it);
ATS_API void ray3_iter_advance(ray3_iter* it);
ATS_API v3 ray3_iter_get_position(ray3_iter* it);
ATS_API v3 ray3_iter_get_normal(ray3_iter* it);

#define path_node(...) (path_node) { __VA_ARGS__ }
typedef struct
{
  f32 w;
  i32 x;
  i32 y;
  i32 z;
} path_node;

typedef struct
{
  u32 len;
  path_node* buf;
} path_queue;

ATS_API path_queue path_queue_create(usize capacity);
ATS_API b32 path_queue_empty(path_queue* queue);
ATS_API void path_queue_clear(path_queue* queue);
ATS_API void path_queue_push(path_queue* queue, path_node node);
ATS_API path_node path_queue_pop(path_queue* queue);

#define SPATIAL_TABLE_MAX 4096
#define SPATIAL_TABLE_MOD 4095

typedef struct sm_node sm_node;
struct sm_node
{
  sm_node* next;
  void* e;
  r2 rect;
};

typedef struct
{
  sm_node* table[SPATIAL_TABLE_MAX];
} spatial_map;

ATS_API void sm_clear(spatial_map* map);
ATS_API u32 sm_index(spatial_map* map, i32 x, i32 y);
ATS_API void sm_add(spatial_map* map, void* e, r2 e_rect);
ATS_API sm_node* sm_in_range(spatial_map* map, v2 pos, v2 rad, void* ignore);
ATS_API void* sm_get_closest(spatial_map* map, v2 pos, f32 range, void* ignore, b32 (*condition_proc)(void*));
ATS_API void* sm_at_position(spatial_map* map, v2 pos);

// ================================================================================================== //
// ---------------------------------------------- ROUTINE ------------------------------------------- //
// ================================================================================================== //
// copied and modefied from: https://gist.github.com/NoelFB/7a5fa66fc29dd7ed1c11042c30f1b00e
// Example:
// // Assuming you have a `routine` variable stored somewhere
// rt_begin(routine);
// {
//   // stuff that happens frame 1
// }
// rt_wait(1.0f);
// {
//   // after 1.0s, this block runs
// }
// rt_for(0.25f);
// {
//   // this block repeats for 0.25s
// }
// rt_step();
// {
//   // the following frame, this block is run
// }
// rt_label(0);
// {
//   if (something)
//     rt_repeat();
//
//   // not run if rt_repeat() was called
//   something_else();
// }
// rt_step();
// {
//   if (another) rt_goto(0); // jumps to label 0
//   if (another2) rt_restart();  // jumps to rt_begin
//   // otherwise the next block will be run next frame
// }
// rt_while(condition_is_true);
// {
//   // this is repeated until condition_is_true is false
// }
// rt_end();

typedef struct
{
  i32 at;
  f32 timer;
} rt;

#define RT_LABEL_OFFSET 1147483647

#define rt_hash(tag) (RT_LABEL_OFFSET + (tag))

#define rt_begin(routine, delta_time) \
  if ((routine).timer < 0) { \
    (routine).timer += (delta_time); \
    (routine).timer = clamp_max((routine).timer, 0); \
  } else { \
    rt* __rt = &(routine); \
    f32 __dt = (delta_time); \
    int __mn = 1; \
    switch (__rt->at) { \
      case 0: { \

#define rt_step() \
      } if (__mn) __rt->at = __LINE__; \
      break; \
      case __LINE__: { \

#define rt_label(name) \
      } if (__mn) __rt->at = rt_hash(name); \
      break; \
      case rt_hash(name): { \

#define rt_for(time) \
        rt_step(); \
        if (__rt->timer < time) {  \
          __rt->timer += __dt; \
          __mn = __rt->timer >= time; \
          if (__mn) __rt->timer = 0; \
        } \

#define rt_while(condition) \
      } if (__mn) __rt->at = __LINE__; \
      break; \
      case __LINE__: if (condition) { \
        __mn = 0 \

#define rt_until(condition) \
      } if (__mn) __rt->at = ((condition) ? __LINE__ : -__LINE__); \
      break; \
      case -__LINE__: \
                      if (condition) __rt->at = __LINE__; \
      break; \
      case __LINE__: { \

#define rt_wait(time) \
      } if (__mn) { \
        __rt->timer = -(time); \
        __rt->at = __LINE__; \
      } \
      break; \
      case __LINE__: { \

#define rt_end() \
      } if (__mn) __rt->at = -1; \
      break; \
    } \
  } \
  goto rt_end_of_routine;  \
  rt_end_of_routine: \

// Flow Statements:
// These can be used anywhere between rt_begin and rt_end,
// including if statements, while loops, etc.

// Repeats the block that this is contained within
// Skips the remainder of the block
#define rt_repeat() \
  goto rt_end_of_routine \

// Goes to a given block labeled with `rt_label`
#define rt_goto(name) \
  do { \
    __rt->at = rt_hash(name); \
    goto rt_end_of_routine; \
  } while(0) \

// Restarts the entire Coroutine;
// Jumps back to `rt_begin` on the next frame
#define rt_restart() \
  do { \
    __rt->at = 0; \
    __rt->timer = 0; \
    goto rt_end_of_routine; \
  } while(0) \

// ================================================ FILE ============================================ //
// ------------------------------------ implementation in ats_file.c -------------------------------- //
// ================================================================================================== //

ATS_API usize file_get_size(const char* path);
ATS_API char* file_read(const char* file_name);
ATS_API b32 file_write(const char* file_name, const char* buffer);
ATS_API b32 file_append(const char* file_name, const char* buffer);

ATS_API usize file_read_bin(const char* file_name, void* buffer, usize size);
ATS_API b32 file_write_bin(const char* file_name, const void* buffer, usize size);

ATS_API u32* file_load_image(const char* path, u16* width, u16* height);
ATS_API void file_free_image(const u32* pixels);

#define dir_iter(...) for (dir_open(__VA_ARGS__); dir_is_valid(); dir_advance())

ATS_API i32 dir_depth(void);

ATS_API void dir_open(const char* path);
ATS_API b32 dir_is_valid(void);
ATS_API void dir_advance(void);

ATS_API char* dir_path(void);
ATS_API char* dir_name(void);
ATS_API char* dir_extension(void);

// =========================================== TEXTURE TABLE ======================================== //
// ------------------------------- implementation in ats_texture_table.c ---------------------------- //
// ================================================================================================== //

#define tex_rect(...) ((tex_rect) { __VA_ARGS__ })
typedef struct
{
  u16 min_x;
  u16 min_y;
  u16 max_x;
  u16 max_y;
} tex_rect;

ATS_API u32* tex_get_pixels(void);
ATS_API u16 tex_get_width(void);
ATS_API u16 tex_get_height(void);
ATS_API tex_rect tex_get(const char* name);
ATS_API void tex_save(const char* name);

ATS_API void tex_begin(u16 width, u16 height);
ATS_API void tex_end(void);
ATS_API void tex_add_image(const char* name, void* pixels, u16 width, u16 height);
ATS_API void tex_load_dir(const char* path);

//ATS_API void tex_load_and_scale_dir(const char* path, u16 denominator);

// ========================================= ANIMATION TABLE ======================================== //
// ----------------------------- implementation in ats_animation_table.c ---------------------------- //
// ================================================================================================== //

typedef struct at_frame at_frame;
typedef struct at_animation at_animation;
typedef struct at_entity at_entity;

struct at_frame
{
  const char* name;
  tex_rect rect;
  at_frame* next;
  at_animation* animation;
};

struct at_animation
{
  const char* name;
  at_frame* frame;
  at_animation* next;
};

struct at_entity
{
  const char* name;
  at_animation* animation;
  at_entity* next;
};

typedef struct
{
  at_entity* entity;
  at_frame* frame;
  f32 duration;
} at_state;

ATS_API void at_begin(void);
ATS_API void at_end(void);
ATS_API void at_add_entity(const char* name);
ATS_API void at_add_animation(const char* name);
ATS_API void at_add_frame(const char* name);

ATS_API void at_set(at_state* state, const char* name);
ATS_API at_state at_get(const char* name);
ATS_API void at_update(at_state* state, f32 dt);

// ============================================ PLATFORM ============================================ //
// ------------------ implementation depended on ATS_PLATFORM_... (default glfw)  ------------------- //
// ================================================================================================== //

ATS_API void platform_init(const char* title, int width, int height, int samples);
ATS_API void platform_update(void);

// ================================================================================================== //
// -------------------------------------------------------------------------------------------------- //
// ------------------------------------------- IMPLEMENTATION --------------------------------------- //
// -------------------------------------------------------------------------------------------------- //
// ================================================================================================== //
#ifdef ATS_IMPL //
#ifndef ATS_IMPL_ONCE
#define ATS_IMPL_ONCE

#include "ats_math.c"
#include "ats_mem.c"
#include "ats_ds.c"

#include "ats_glfw.c"

#include "ats_file.c"
#include "ats_texture_table.c"
#include "ats_animation_table.c"

#include "ats_audio_table.c"
#include "ats_timer.c"

#ifdef ATS_OGL46
#include "ats_gl46.c"
#else
#include "ats_gl.c"
#endif

#endif // ATS_IMPL_ONCE
#endif // ATS_IMPL

