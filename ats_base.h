#pragma once

#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#define ATS_INIT {}
#define Make(T) T
#else
#define ATS_INIT {0}
#define Make(T) (T)
#endif

#define KIB (1024)
#define MIB (1024 * KIB)
#define GIB (1024 * MIB)

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

#define scope_guard(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_range(index, start, below) \
  for (isize index = (start); index < (below); ++index)

#define for_array(index, array) \
  for (usize index = 0; index < ArrayCount(array); ++index)

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

#define ClampMin(n, min)    ((n) < (min)? (min) : (n))
#define ClampMax(n, max)    ((n) > (max)? (max) : (n))
#define Clamp(n, min, max)  ClampMax(ClampMin(n, min), max)
#define Clamp01(n)          Clamp(n, 0, 1)
#define Min(a, b)           ((a) < (b)? (a) : (b))
#define Max(a, b)           ((a) > (b)? (a) : (b))
#define Lerp(a, b, t)       ((a) + (f32)(t) * ((b) - (a)))
#define Sign(n)             ((n) == 0? 0 : ((n) < 0? -1 : 1))

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

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

