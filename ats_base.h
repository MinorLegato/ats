#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
#define ATS_INIT {}
#define make(type_t) type_t
#else
#define ATS_INIT {0}
#define make(type_t) (type_t)
#endif

#if __STDC_VERSION__ >= 201112L
#define __c11
#endif

#define PI (3.14159265359f)
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
  for (isize macro_var(index) = 0; macro_var(index) < (count); ++macro_var(index))

#define for_r2(rect, ix, iy) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_r3(rect, ix, iy, iz) \
  for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
  for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
  for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_iter(iter_type, iter_name, ...) \
  for (struct iter_type iter_name = (__VA_ARGS__); \
       iter_type##_is_valid(&iter_name); \
       iter_type##_advance(&iter_name))

#define for_each(type, var, ...) \
  for (struct type* var = (struct type*)0xdeadbeefull; var != NULL; var = NULL) \
  for (struct type##_iter macro_var(it) = (__VA_ARGS__); \
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

typedef ptrdiff_t isize;
typedef size_t usize;

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#ifdef __cplusplus

constexpr u32 match_hash(const char* str) {
  u32 hash = 5381;
  for (int i = 0; str[i] != '\0'; i++)
    hash = ((hash << 5) + hash) + str[i];
  return hash;
}

#define match(str) switch(match_hash(str))
#define with(str)  case match_hash(str)

#else

static inline u32 match_hash(const char* str) {
  u32 hash = 5381;
  for (int i = 0; str[i] != '\0'; i++)
    hash = ((hash << 5) + hash) + str[i];
  return hash;
}

#define match(...) \
  for (u32 _match_hash = match_hash(__VA_ARGS__), _with_hash = 0, _match_found = 0; _match_hash; _match_hash = 0)

#define with(...) \
  if (!_match_found && (_match_found = ((_with_hash = match_hash(__VA_ARGS__)) == _match_hash)))

#endif

