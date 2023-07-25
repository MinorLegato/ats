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

#define fx8_create(n)  ((fx8)((n)  * (1 << FX8_SHIFT)))
#define fx16_create(n) ((fx16)((n) * (1 << FX16_SHIFT)))
#define fx32_create(n) ((fx32)((n) * (1 << FX32_SHIFT)))
#define fx64_create(n) ((fx64)((n) * (1 << FX64_SHIFT)))

