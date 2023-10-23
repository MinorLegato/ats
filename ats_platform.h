#pragma once

#include "ats_gl.h"

#ifdef ATS_OGL46
#include "ext/glad/glad.h"
#define GLSL(...) "#version 460 core\n" #__VA_ARGS__
#endif

// ------------------- platform layer ------------------------ //

static void platform_init(const char* title, int width, int height, int samples);
static void platform_update(void);
static f64  platform_get_time(void);

// ===================================================== KEYS =================================================== //

#define KEY_UNKNOWN            -1

#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348
#define KEY_LAST               KEY_MENU

#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3

#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          JOYSTICK_16

#define GAMEPAD_BUTTON_A               0
#define GAMEPAD_BUTTON_B               1
#define GAMEPAD_BUTTON_X               2
#define GAMEPAD_BUTTON_Y               3
#define GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GAMEPAD_BUTTON_BACK            6
#define GAMEPAD_BUTTON_START           7
#define GAMEPAD_BUTTON_GUIDE           8
#define GAMEPAD_BUTTON_LEFT_THUMB      9
#define GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GAMEPAD_BUTTON_DPAD_UP         11
#define GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GAMEPAD_BUTTON_DPAD_DOWN       13
#define GAMEPAD_BUTTON_DPAD_LEFT       14
#define GAMEPAD_BUTTON_LAST            GAMEPAD_BUTTON_DPAD_LEFT

#define GAMEPAD_BUTTON_CROSS       GAMEPAD_BUTTON_A
#define GAMEPAD_BUTTON_CIRCLE      GAMEPAD_BUTTON_B
#define GAMEPAD_BUTTON_SQUARE      GAMEPAD_BUTTON_X
#define GAMEPAD_BUTTON_TRIANGLE    GAMEPAD_BUTTON_Y

#define GAMEPAD_AXIS_LEFT_X        0
#define GAMEPAD_AXIS_LEFT_Y        1
#define GAMEPAD_AXIS_RIGHT_X       2
#define GAMEPAD_AXIS_RIGHT_Y       3
#define GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GAMEPAD_AXIS_LAST          GAMEPAD_AXIS_RIGHT_TRIGGER

typedef union {
  struct {
    u32 x : 1;
    u32 a : 1;
    u32 b : 1;
    u32 y : 1;

    u32 left_bumper : 1;
    u32 right_bumper : 1;
    u32 left_trigger : 1;
    u32 right_trigger : 1;

    u32 select : 1;
    u32 start : 1;
    u32 left_stick : 1;
    u32 right_stick : 1;

    u32 up : 1;
    u32 right : 1;
    u32 down : 1;
    u32 left : 1;
  } button;

  u32 data;
} gamepad_buttons;

typedef struct {
  b32 active;

  v2 left_stick;
  v2 right_stick;

  f32 left_trigger;
  f32 right_trigger;

  gamepad_buttons down;
  gamepad_buttons pressed;
  gamepad_buttons released;
} gamepad;

typedef u8 mouse_mode;
enum {
  MOUSE_MODE_NORMAL,
  MOUSE_MODE_HIDDEN,
  MOUSE_MODE_DISABLED,
};

static struct {
  b32 close;

  i32 width;
  i32 height;
  i32 refresh_rate;
  f32 aspect_ratio;

  void* native;

  b32 fullscreen;
  b32 _fullscreen_state_last_update;

  struct {
    f64 total;
    f32 delta;
  } time;

  struct {
    mouse_mode mode;

    b32 is_down : 1;
    b32 is_pressed : 1;
    b32 is_released : 1;

    v2 pos;
    v2 delta;
    v2 scroll;

    b8 down[MOUSE_BUTTON_LAST + 1];
    b8 pressed[MOUSE_BUTTON_LAST + 1];
    b8 released[MOUSE_BUTTON_LAST + 1];
  } mouse;

  struct {
    i32 key;
    i32 ascii;

    b32 is_down : 1;
    b32 is_pressed : 1;
    b32 is_repeat : 1;
    b32 is_released : 1;
    b32 is_ascii : 1;

    b8 down[KEY_LAST + 1];
    b8 pressed[KEY_LAST + 1];
    b8 repeat[KEY_LAST + 1];
    b8 released[KEY_LAST + 1];
  } keyboard;

  gamepad gamepad[JOYSTICK_LAST];
} platform;

typedef struct {
  const char* name;

  f64 start;
  f64 stop;

  u32 depth;
} timer_entry;

static u32 timer_top;
static timer_entry timer_stack[512];

static u32 timer_count;
static timer_entry timer_array[512];

#define timer_scope(name) scope_guard(timer_start(name), timer_stop())

static void timer_start(const char* name)
{
  timer_entry* entry = timer_stack + timer_top++;

  entry->name = name;
  entry->start = platform_get_time();
  entry->stop = 0;
  entry->depth = timer_top - 1;
}

static void timer_stop(void)
{
  timer_entry* entry = timer_stack + (--timer_top);

  entry->stop = platform_get_time();
  timer_array[timer_count++] = *entry;
}

static void timer_reset_all(void)
{
  timer_top = 0;
  timer_count = 0;
}

static void timer_print_result(f32 px, f32 py, f32 sx, f32 sy)
{
  int y = 0;
  // @TODO: fix render order within scopes!
  for (int i = timer_count - 1; i >= 0; --i) {
    timer_entry e = timer_array[i];
    gl_string_format(px + 2 * sx * e.depth, py + y * (sy + 1), 0, sx, sy, 0xff77ccff, "%s : %.2f", e.name, 1000.0 * (e.stop - e.start));
    y++;
  }
  timer_reset_all();
}

