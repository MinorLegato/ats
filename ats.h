#ifndef __ATS_H__
#define __ATS_H__

#include "ats_math.h"
#include "ats_fixed.h"
#include "ats_routine.h"
#include "ats_mem.h"
#include "ats_ds.h"
#include "ats_file.h"

#include "ats_texture_table.h"
#include "ats_animation_table.h"
#include "ats_audio_table.h"

#include "ats_gl.h"
#include "ats_platform.h"

#include "ats_thread.h"

#endif // __ATS_H__

#ifdef ATS_IMPL
#include "ats.c"
#endif

#include "ats_base.h"

typedef float   f32;
typedef double  f64;

typedef char      i8;
typedef short     i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef long long           isize;
typedef unsigned long long  usize;

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

