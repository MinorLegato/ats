#pragma once

#include "include/GLFW/glfw3.h"

#define _GLFW_WIN32
#include "src/init.c"
#include "src/window.c"
#include "src/context.c"
#include "src/monitor.c"
#include "src/input.c"

#include "src/egl_context.c"
#include "src/osmesa_context.c"
#include "src/wgl_context.c"
#include "src/vulkan.c"

#include "src/win32_init.c"
#include "src/win32_window.c"
#include "src/win32_monitor.c"
#include "src/win32_thread.c"
#include "src/win32_joystick.c"
#include "src/win32_time.c"

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")
#endif

