#ifndef ATS_IMPL_ONCE
#define ATS_IMPL_ONCE

#include "ats_platform.c"
#include "ats_gl.c"
#include "ats_file.c"
#include "ats_texture_table.c"
#include "ats_animation_table.c"
#include "ats_audio_table.c"
#if 0
#include "ats_thread.c"
#endif

#ifndef ATS_NO_MAIN
#ifndef MEM_DEFAULT_SIZE
#define MEM_DEFAULT_SIZE MEM_MIB(256)
#endif

int ats_main(void);

#ifdef ATS_NO_CONSOLE
int WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
  (void)a; (void)b; (void)c; (void)d;
#else
int main(void) {
#endif
  static u8 memory_buffer[MEM_DEFAULT_SIZE];
  struct mem_arena default_arena = mem_create(memory_buffer, countof(memory_buffer));

  mem_context(&default_arena) {
    int result = ats_main();
    (void)result;
  }

  return 0;
}

#define main ats_main

#endif // ATS_MAIN

#endif // ATS_IMPL_ONCE
