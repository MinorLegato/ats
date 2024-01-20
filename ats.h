#ifndef __ATS_H__
#define __ATS_H__

#include "ats_base.h"
#include "ats_math.h"
#include "ats_routine.h"
#include "ats_mem.h"
#include "ats_ds.h"

#include "ats_platform.h"

#include "ats_file.h"
#include "ats_texture_table.h"
#include "ats_animation_table.h"
#include "ats_audio_table.h"
#include "ats_gl.h"
#include "ats_thread.h"

#endif // __ATS_H__

#ifdef ATS_IMPL
#ifndef ATS_IMPL_ONCE
#define ATS_IMPL_ONCE

#ifndef ATS_NO_MAIN
#ifndef MEM_DEFAULT_SIZE
#define MEM_DEFAULT_SIZE MEM_MIB(256)
#endif

static int ats_main(void);

#ifdef ATS_NO_CONSOLE
int WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
#else
int main(void)
#endif
{
#ifdef ATS_NO_CONSOLE
  (void)a; (void)b; (void)c; (void)d;
#endif
  static u8 memory_buffer[MEM_DEFAULT_SIZE];
  mem_arena default_arena = mem_create(memory_buffer, countof(memory_buffer));

  mem_context(&default_arena) {
    int result = ats_main();
    (void)result;
  }

  return 0;
}

#define main ats_main

#endif // ATS_NO_MAIN

#endif // ATS_IMPL_ONCE
#endif // ATS_IMPL

