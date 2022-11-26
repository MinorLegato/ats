#pragma once

#ifndef __ATS_H__
#define __ATS_H__

#include "ats_base.h"
#include "ats_math.h"
#include "ats_routine.h"
#include "ats_mem.h"
#include "ats_ds.h"
#include "ats_image.h"
#include "ats_file.h"

#include "ats_texture_table.h"
#include "ats_animation_table.h"
#include "ats_audio_table.h"

#include "ats_gl.h"
#include "ats_platform.h"
#include "ats_thread.h"

#endif // __ATS_H__

#ifdef ATS_IMPL
#ifndef ATS_IMPL_ONCE
#define ATS_IMPL_ONCE

#include "ats_platform.c"
#include "ats_gl.c"
#include "ats_file.c"
#include "ats_texture_table.c"
#include "ats_animation_table.c"
#include "ats_audio_table.c"
#include "ats_thread.c"

#endif // ATS_IMPL_ONCE
#endif // ATS_IMPL

