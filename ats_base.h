#pragma once

#include <math.h>

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

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

static_assert (sizeof (f32) == 4, "type width mismatch");
static_assert (sizeof (f64) == 8, "type width mismatch");

static_assert (sizeof (i8)  == 1, "type width mismatch");
static_assert (sizeof (i16) == 2, "type width mismatch");
static_assert (sizeof (i32) == 4, "type width mismatch");
static_assert (sizeof (i64) == 8, "type width mismatch");

static_assert (sizeof (u8)  == 1, "type width mismatch");
static_assert (sizeof (u16) == 2, "type width mismatch");
static_assert (sizeof (u32) == 4, "type width mismatch");
static_assert (sizeof (u64) == 8, "type width mismatch");

typedef u8  b8;
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

#define for_arr(var, array)     for (auto var = arr_begin(array); var < arr_end(array); ++var)

#include "ats_math.c"
#include "ats_routine.h"
#include "ats_mem.c"
#include "ats_ds.c"

