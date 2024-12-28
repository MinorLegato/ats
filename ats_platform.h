#pragma once

#ifndef ATS_BASE
#include "ats_base.h"
#endif

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

