#pragma once

#include "ats_base.h"

// NOTE: might need to add some casts to protect against overflows.
// arithmetics:
// add: a + b
// sub: a - b
// mul: (a * b) >> FRAC_BITS
// div: (a << FRAC_BITS) / b

#ifndef FIX8_SHIFT
#define FIX8_SHIFT (4)
#endif

#ifndef FIX16_SHIFT
#define FIX16_SHIFT (7)
#endif

#ifndef FIX32_SHIFT
#define FIX32_SHIFT (11)
#endif

#ifndef FIX64_SHIFT
#define FIX64_SHIFT (21)
#endif

typedef i8 fix8;
typedef i16 fix16;
typedef i32 fix32;
typedef i64 fix64;

// to fixed:

#define fix8_make(n)  ((fix8)((n)  * (1 << FIX8_SHIFT)))
#define fix16_make(n) ((fix16)((n) * (1 << FIX16_SHIFT)))
#define fix32_make(n) ((fix32)((n) * (1 << FIX32_SHIFT)))
#define fix64_make(n) ((fix64)((n) * (1 << FIX64_SHIFT)))

// fixed to float:

#define fix8_cast(n)  ((f32)(n) / (f32)(1 << FIX8_SHIFT))
#define fix16_cast(n) ((f32)(n) / (f32)(1 << FIX16_SHIFT))
#define fix32_cast(n) ((f32)(n) / (f32)(1 << FIX32_SHIFT))
#define fix64_cast(n) ((f32)(n) / (f32)(1 << FIX64_SHIFT))

// trunc:

#define fix8_trunc(n)  ((n) >> FIX8_SHIFT)
#define fix16_trunc(n) ((n) >> FIX16_SHIFT)
#define fix32_trunc(n) ((n) >> FIX32_SHIFT)
#define fix64_trunc(n) ((n) >> FIX64_SHIFT)

// lerp:

static fix8 fix8_lerp(fix8 a, fix8 b, fix8 t) {
  return a + ((t * (b - a)) >> FIX8_SHIFT);
}

static fix16 fix16_lerp(fix16 a, fix16 b, fix16 t) {
  return a + ((t * (b - a)) >> FIX16_SHIFT);
}

static fix32 fix32_lerp(fix32 a, fix32 b, fix32 t) {
  return a + ((t * (b - a)) >> FIX32_SHIFT);
}

static fix64 fix64_lerp(fix64 a, fix64 b, fix64 t) {
  return a + ((t * (b - a)) >> FIX64_SHIFT);
}

