#pragma once

#include "ats_base.h"

#ifndef FX8_SHIFT
#define FX8_SHIFT  (4)
#endif

#ifndef FX16_SHIFT
#define FX16_SHIFT (7)
#endif

#ifndef FX32_SHIFT
#define FX32_SHIFT (11)
#endif

#ifndef FX64_SHIFT
#define FX64_SHIFT (21)
#endif

typedef i8  fx8;
typedef i16 fx16;
typedef i32 fx32;
typedef i64 fx64;

typedef union {
  struct { fx8 x, y; };
  fx8 e[2];
} v2fx8;

typedef union {
  struct { fx16 x, y, z; };
  fx16 e[3];
} v3fx8;

typedef union {
  struct { fx32 x, y, z, w; };
  fx32 e[4];
} v4fx8;

// to fixed

#define fx8_make(n)  ((fx8)((n)  * (1 << FX8_SHIFT)))
#define fx16_make(n) ((fx16)((n) * (1 << FX16_SHIFT)))
#define fx32_make(n) ((fx32)((n) * (1 << FX32_SHIFT)))
#define fx64_make(n) ((fx64)((n) * (1 << FX64_SHIFT)))

// fixed to float

#define fx8_f32(n)  ((f32)(n) / (f32)(1 << FX8_SHIFT))
#define fx16_f32(n) ((f32)(n) / (f32)(1 << FX16_SHIFT))
#define fx32_f32(n) ((f32)(n) / (f32)(1 << FX32_SHIFT))
#define fx64_f32(n) ((f32)(n) / (f32)(1 << FX64_SHIFT))

// lerp

static fix8 fix8_lerp(fix8 a, fix8 b, fix8 t) {
  return a + ((t * (b - a)) >> FX8_SHIFT);
}

static fix16 fix16_lerp(fix16 a, fix16 b, fix16 t) {
  return a + ((t * (b - a)) >> FX16_SHIFT);
}

static fix32 fix32_lerp(fix32 a, fix32 b, fix32 t) {
  return a + ((t * (b - a)) >> FX32_SHIFT);
}

static fix64 fix64_lerp(fix64 a, fix64 b, fix64 t) {
  return a + ((t * (b - a)) >> FX64_SHIFT);
}

