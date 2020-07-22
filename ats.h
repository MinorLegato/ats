// single file C11 'header' library, written in a .c file because some compilers treats h/c files differently.

// defines that enables different parts of the library:
// #define ATS_PLATFORM_GLFW
// #define ATS_OPENGL_LEGACY
// #define ATS_MODERN_OPENGL 
// #define ATS_CUTE_SOUND

#ifndef __ATS_H__ 
#define __ATS_H__

#include <windef.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <stdint.h>

#ifdef ATS_MODERN_OPENGL
#include "glad/glad.h"
#endif

#define MEMORY_KB   ((1024))
#define MEMORY_MB   ((1024) * (MEMORY_KB))
#define MEMORY_GB   ((1024) * (MEMORY_MB))

#ifndef PI
#define PI  (3.14159265359f)
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#define TO_RAD_MUL  (0.01745329251f)
#define TO_DEG_MUL  (57.2957795131f)

#define TO_RAD(deg) ((deg) * TO_RAD_MUL)
#define TO_DEG(rad) ((rad) * TO_DEG_MUL)

#define SETBIT(n, b)     ((n)  |=  (1ull << (b)))
#define CLRBIT(n, b)     ((n)  &= ~(1ull << (b)))
#define CHKBIT(n, b)     (((n) & (1ull << (b))) == (1ull << (b)))

#define SWAP(T, a, b) do { T t = (a); (a) = (b); (b) = t; } while(0)
//
#ifndef MIN
#define MIN(a, b) (((a) < (b))? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b))? (a) : (b))
#endif

#define CLAMP_MAX(x, hi) MIN((x), (hi))
#define CLAMP_MIN(x, lo) MAX((x), (lo))
//
#define CLAMP(x, lo, hi) (((x) < (lo))? (lo) : (((x) > (hi))? (hi) : (x)))
//
#define CLAMP_0(x)  ((x) < 0.0f? 0.0f : (x))
#define CLAMP_1(x)  ((x) > 1.0f? 1.0f : (x))
#define CLAMP_01(x) (((x) < 0.0f)? 0.0f : ((x) > 1.0f? 1.0f : (x)))
//
#define SIGN(x)             ((x) < 0? -1 : 1)
#define SQUARE(x)           ((x) * (x))
#define IS_POWER_OF_TWO(x)  (((x) != 0) && ((x) & ((x)-1)) == 0)
//
#define ALIGN_DOWN(n, a)      ((n) & ~((a) - 1))
#define ALIGN_UP(n, a)        ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a)  ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a)    ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))

#define U2(arg)     (arg)[0], (arg)[1]
#define U3(arg)     (arg)[0], (arg)[1], (arg)[2]
#define U4(arg)     (arg)[0], (arg)[1], (arg)[2], (arg)[3]

#define UV2(arg)     (arg).x, (arg).y
#define UV3(arg)     (arg).x, (arg).y, (arg).z
#define UV4(arg)     (arg).x, (arg).y, (arg).z, (arg).w

#define UM2(arg)     (arg).x.x, (arg).x.y, (arg).y.x, (arg).y.y

#define ARRAY_COUNT(array) (sizeof (array) / sizeof ((array)[0]))

#define Sign(x) ((x) < 0? -1 : 1)

// ================================================== TYPES ================================================= //

typedef float   f32;
typedef double  f64;

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef uint8_t     b8;
typedef uint16_t    b16;
typedef uint32_t    b32;
typedef uint64_t    b64;

union v2 {
    struct {
        f32     x;
        f32     y;
    };

    f32 array[2];
};

union v3 {
    struct {
        f32     x;
        f32     y;
        f32     z;
    };

    struct {
        v2      xy;
        f32     _z;
    };

    struct {
        f32     r;
        f32     g;
        f32     b;
    };

    f32 array[3];
};

union v4 {
    struct {
        f32     x;
        f32     y;
        f32     z;
        f32     w;
    };

    struct {
        f32     r;
        f32     g;
        f32     b;
        f32     a;
    };

    struct {
        v3      xyz;
        f32     _w;
    };

    struct {
        v3      rgb;
        f32     _a;
    };

    f32 array[4];
};

union Quat {
    struct {
        f32     x;
        f32     y;
        f32     z;
        f32     w;
    };

    f32 array[4];
};

union m2 {
    struct {
        v2  x;
        v2  y;
    };

    f32 array[4];
};

union m3 {
    struct {
        v3  x;
        v3  y;
        v3  z;
    };

    f32 array[9];
};

union m4 {
    struct {
        v4  x;
        v4  y;
        v4  z;
        v4  w;
    };

    f32 array[16];
};

union v2i {
    struct {
        i32     x;
        i32     y;
    };

    i32 array[2];
};

union v3i {
    struct {
        i32     x;
        i32     y;
        i32     z;
    };

    struct {
        v2i      xy;
        i32     _z;
    };

    i32 array[3];
};

union v4i {
    struct {
        i32     x;
        i32     y;
        i32     z;
        i32     w;
    };

    i32 array[4];
};

// ===================================================== KEYS =================================================== //

#ifdef ATS_PLATFORM_GLFW

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

// ==================================================================== FUNCTIONS ===================================================== //

extern void PlatformInit    (const char* title, int width, int height, int samples);
extern void PlatformUpdate  (void);
extern f32  TimerGetCurrent (void);

// ==================================================================== GAMEPAD ===================================================== //

union GamepadButtons {
    struct {
        u32 X       : 1;
        u32 A       : 1;
        u32 B       : 1;
        u32 Y       : 1;
        //
        u32 LB      : 1;
        u32 RB      : 1;
        u32 LT      : 1;
        u32 RT      : 1;
        //
        u32 select  : 1;
        u32 start   : 1;
        u32 LS      : 1;
        u32 RS      : 1;
        //
        u32 UP      : 1;
        u32 RIGHT   : 1;
        u32 DOWN    : 1;
        u32 LEFT    : 1;
    } button;

    u32 data;
};

struct Gamepad {
    b32     active;
    // axes:
    v2      LS;
    v2      RS;
    //
    f32     LT;
    f32     RT;
    // buttons
    GamepadButtons  state;
    GamepadButtons  pressed;
    GamepadButtons  released;
};

// =========================================================== MOUSE MODES ================================================= //

enum MouseMode {
    MOUSE_NORMAL,
    MOUSE_HIDDEN,
    MOUSE_DISABLED
};

// ===========================================================  PLATFORM =================================================== //

struct Platform {
    b32         close;
    //
    i32         width;
    i32         height;
    f32         aspect_ratio;

    HWND        native;

    b32         fullscreen;
    b32         _fullscreen_state_last_update;

    b32         vsync;
    b32         _vsync_state_last_update;

    struct Time {
        f32     total;
        f32     delta;
    } time;

    struct Mouse {
        u32     mode;

        b32     is_down        : 1;
        b32     is_pressed     : 1;
        b32     is_released    : 1;

        v2      pos;
        v2      delta;
        v2      scroll;

        b8      state     [MOUSE_BUTTON_LAST];
        b8      pressed   [MOUSE_BUTTON_LAST];
        b8      released  [MOUSE_BUTTON_LAST];
    } mouse;

    struct Keyboard {
        i32     key;
        i32     ascii;

        b32     is_down         : 1;
        b32     is_pressed      : 1;
        b32     is_repeat       : 1;
        b32     is_released     : 1;
        b32     is_ascii        : 1;
    
        b8      state     [KEY_LAST];
        b8      pressed   [KEY_LAST];
        b8      repeat    [KEY_LAST];
        b8      released  [KEY_LAST];
    } keyboard;

    Gamepad gamepad[JOYSTICK_LAST];
};

extern Platform platform;

#endif // ATS_PLATFORM_GLFW

// ============================================= SHADERS =================================================== //

#define GLSL_SHADER(shader) "#version 330 core\n" #shader

extern u32  ShaderLoadFromMemory    (const char *vs, const char *fs, const char *gs);

extern void ShaderUse               (u32 shader);
extern u32  ShaderGetLocation       (u32 shader, const char *var_name);
extern void ShaderSeti              (u32 shader, const char *loc, int n);
extern void ShaderSetf              (u32 shader, const char *loc, f32 n);
extern void ShaderSet2f             (u32 shader, const char *loc, f32 a, f32 b);
extern void ShaderSet3f             (u32 shader, const char *loc, f32 a, f32 b, f32 c);
extern void ShaderSet4f             (u32 shader, const char *loc, f32 a, f32 b, f32 c, f32 d);
extern void ShaderSet2v             (u32 shader, const char *loc, const f32 *u);
extern void ShaderSet3v             (u32 shader, const char *loc, const f32 *u);
extern void ShaderSet4v             (u32 shader, const char *loc, const f32 *u);
extern void ShaderSetMat4           (u32 shader, const char *loc, const f32 *m);

// ================================================ CUBE ENGINE =================================================== //

#ifdef ATS_CUBE_ENGINE

extern void ce_Init(void);

extern void ce_Clear(void);
extern void ce_EnableDepthTest (void);
extern void ce_DisableDepthTest(void);

extern void ce_SetLight     (f32 x, f32 y, f32 z, f32 r, f32 g, f32 b);
extern void ce_SetView      (f32 pos_x, f32 pos_y, f32 pos_z, f32 look_x, f32 look_y, f32 look_z, f32 up_x, f32 up_y, f32 up_z, f32 fov, f32 near_plane, f32 far_plane);
extern void ce_SetLightView (f32 pos_x, f32 pos_y, f32 pos_z, f32 look_x, f32 look_y, f32 look_z, f32 up_x, f32 up_y, f32 up_z, f32 fov, f32 near_plane, f32 far_plane);
extern void ce_SetView2D    (f32 pos_x, f32 pos_y, f32 pos_z, f32 look_x, f32 look_y, f32 look_z, f32 up_x, f32 up_y, f32 up_z, f32 rad_x,  f32 rad_y, f32 near_plane, f32 far_plane);

extern void ce_SetProjectionView(const f32* projection_view);

extern void ce_RenderCubes  (void);
extern void ce_RenderSquares(void);

extern void ce_PushCube         (f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushCubeQuat     (f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 qx, f32 qy, f32 qz, f32 qw, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushBox          (f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushCubeRot      (f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 rot, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushCubeRotMat   (f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 Rxx, f32 Rxy, f32 Ryx, f32 Ryy, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushCubeEuler    (f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 yaw, f32 pitch, f32 roll, f32 red, f32 green, f32 blue, f32 alpha);
extern void ce_PushBoxRot       (f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 rot, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushBoxRotMat    (f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 Rxx, f32 Rxy, f32 Ryx, f32 Ryy, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushLine         (const f32 p0[2], const f32 p1[2], f32 z, f32 rad, const f32 color[4]);
extern void ce_PushLineLoop     (const f32 pos[2], f32 z, f32 rad, const f32 color[4], const f32 (*point_array)[2], int point_count);

extern void ce_PushString       (const char* str, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushStringFormat (f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a, const char* fmt, ...);
extern void ce_PushInt          (int n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushF32          (f32 n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushStringBox    (const char* str, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a);
extern void ce_PushNumberBox    (int n, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a);

extern int ce_GetCubeCount(void);

static void ce_PushLinef(f32 ax, f32 ay, f32 bx, f32 by, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a) {
    const f32 p0[2] = { ax, ay };
    const f32 p1[2] = { bx, by };
    const f32 color[4] = { r, g, b, a };

    ce_PushLine(p0, p1, z, rad, color);
}

#endif // ATS_CUBE_ENGINE

// ============================================ RENDER (LEGACY OPENGL) ============================================ //

#ifndef ATS_MODERN_OPENGL

extern void RenderInit(void);
extern void RenderClear(void);
extern void RenderSetMatrix(const f32 *matrix);
extern void RenderSetProjectionView(const f32 *projection_view);
extern void RenderSetModel(f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz);
extern void RenderSetRotatedModel(f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz, f32 rot);
extern void RenderSetTranslateScaleRotateZ(f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz, f32 rot);

// camera:
extern void RenderSetCamera(f32 px,  f32 py, f32 pz, f32 tx,  f32 ty, f32 tz, f32 ux,  f32 uy, f32 uz, f32 fov, f32 aspect, f32 n,  f32 f);
extern void RenderSetCamera2D(f32 px,  f32 py, f32 pz, f32 tx,  f32 ty, f32 tz, f32 ux,  f32 uy, f32 uz, f32 sx,  f32 sy, f32 n,  f32 f);
extern void RenderSetCameraOrtho(f32 width, f32 height, f32 n, f32 f);
extern void RenderSetCameraTranslateOrtho(f32 x, f32 y, f32 rad_x, f32 rad_y, f32 n, f32 f);

extern void RenderSetColor(f32 r, f32 g, f32 b, f32 a);
extern void RenderSquare(f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a);
extern void RenderRect(f32 x, f32 y, f32 z, f32 width, f32 height, f32 r, f32 g, f32 b, f32 a);
extern void RenderCenterRect(f32 x, f32 y, f32 z, f32 half_width, f32 half_height, f32 r, f32 g, f32 b, f32 a);
extern void RenderRotatedSquare(f32 x, f32 y, f32 z, f32 rad, f32 rot, f32 r, f32 g, f32 b, f32 a);
extern void RenderCube(f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a);
extern void RenderLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 r, f32 g, f32 b, f32 a);
extern void RenderLineLoop2D(f32 pos_x, f32 pos_y, f32 pos_z, f32 rad, f32 rot, f32 red, f32 green, f32 blue, f32 alpha, const f32 (*point_array)[2], int point_count);

// Lighting:
extern void RenderSetLightEmitter(int index, f32 bright, f32 x, f32 y, f32 z);
extern void RenderSetLightDirected(int index, f32 bright, f32 x, f32 y, f32 z);
extern void RenderSetLightGlobalAmbient(f32 r, f32 g, f32 b);

// Texture:
extern struct Texture  TextureCreate(unsigned char *pixels, int width, int height, int is_smooth);
extern struct Texture  TextureLoadFromFile(const char *texture_path, int is_smooth);
extern void            TextureBind(const struct Texture *texture);
extern void            TextureDelete(struct Texture *texture);

// Text
extern void RenderAscii(unsigned char c, f32 px, f32 py, f32 pz, f32 x_scale, f32 y_scale, f32 r, f32 g, f32 b, f32 a);
extern void RenderString(const char *str, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void RenderStringFormat(f32 x, f32 y, f32 z, f32 rad_x, f32 rad_y, f32 r, f32 g, f32 b, f32 a, const char* fmt, ...);
extern void RenderInt(int n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void RenderF32(f32 n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a);
extern void RenderStringBox(const char* str, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a);
extern void RenderNumberBox(int n, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a);

#endif // ATS_MODERN_OPENGL

// --------------------------------------------- STATIC FUNCTIONS ---------------------------------------- //

inline f32 Sqrt(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

inline f32 RSqrt(f32 n) {
	f32 x2 = n * 0.5F;
	f32 y  = n;
	int i  = *(long*)&y;             // evil f32ing point bit level hacking

	i = 0x5f3759df - (i >> 1);         // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y));     // 1st iteration

	return y;
}

inline f32 Lerp(f32 a, f32 b, f32 t) {
    return a + t * (b - a);
}

inline f32 UnLerp(f32 a, f32 b, f32 t) {
    return (t - a) / (b - a);
}

inline f32 SmoothStep(f32 n) {
	if (n < 0.0f) { return 0.0f; }
	if (n < 1.0f) { return (3.0f * n * n - 2.0f * n * n * n); }

	return 1.0f;
}

inline f32 ShortAngleDist(f32 a, f32 b) {
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);
    return fmodf(2.0f * da, max) - da;
}

inline f32 LerpAngle(f32 a, f32 b, f32 t) {
    return a + ShortAngleDist(a, b) * t;
}

inline f32 Snap(f32 f, f32 step_size) {
	if (f > 0)  { return (f32)((int)(f / step_size + 0.5f)) * step_size; }
    else        { return (f32)((int)(f / step_size - 0.5f)) * step_size; }
}

inline f32 Spline(f32 f, f32 a, f32 b, f32 c, f32 d) {
	f32 i = 1.0f - f;
	return ((d * f + c * i) * f + (c * f + b * i) * i) * f + ((c * f + b * i) * f + (b * f + a * i) * i) * i;
}

inline int Overlap(int a0, int a1, int b0, int b1) {
    return MAX(a0, b0) - MAX(a1, b1);
}

inline int Overlap(f32 a0, f32 a1, f32 b0, f32 b1) {
    return MAX(a0, b0) - MAX(a1, b1);
}

inline f32 Min(f32 a, f32 b) {
    return a < b? a : b;
}

inline f32 Max(f32 a, f32 b) {
    return a < b? b : a;
}

inline int Min(int a, int b) {
    return a < b? a : b;
}

inline int Max(int a, int b) {
    return a < b? b : a;
}

inline f32 Clamp(f32 n, f32 min, f32 max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

inline f32 ClampMin(f32 n, f32 min) {
    return n < min? min : n;
}

inline f32 ClampMax(f32 n, f32 max) {
    return n > max? max : n;
}

inline int Clamp(int n, int min, int max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

inline int ClampMin(int n, int min) {
    return n < min? min : n;
}

inline int ClampMax(int n, int max) {
    return n > max? max : n;
}

inline f32 Floor(f32 n) {
    return floorf(n);
}

inline f32 Ceil(f32 n) {
    return ceilf(n);
}

inline f32 Abs(f32 n) {
    return fabsf(n);
}

inline f32 Cos(f32 n) {
    return cosf(n);
}

inline f32 Sin(f32 n) {
    return sinf(n);
}

inline f32 Round(f32 n) {
    return roundf(n);
}

// -------------------------------------------- 2D --------------------------------------- //

inline void f2_Set(f32* out, f32 x, f32 y) {
    out[0] = x;
    out[1] = y;
}

inline void f2_Copy(f32* r, const f32* a) {
    r[0] = a[0];
    r[1] = a[1];
}

inline void f2_Add(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

inline void f2_Sub(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

inline void f2_Scale(f32* r, const f32* v, f32 s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
}

inline void f2_Mul(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
}

inline f32 f2_Dot(const f32* a, const f32* b) {
    return a[0] * b[0] + a[1] * b[1];
}

inline f32 f2_Len(const f32* v) {
    return Sqrt(v[0] * v[0] + v[1] * v[1]);
}

inline f32 f2_LenSq(const f32* v) {
    return v[0] * v[0] + v[1] * v[1];
}

inline f32 f2_DistSq(const f32* a, const f32* b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];

    return dx * dx + dy * dy;
}

inline f32 f2_Dist(const f32* a, const f32* b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];

    return Sqrt(dx * dx + dy * dy);
}

inline void f2_Norm(f32* r, const f32* v) {
    f32 k = RSqrt(v[0] * v[0] + v[1] * v[1]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
}

inline void f2_Min(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
}

inline void f2_Max(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
}

inline void f2_Lerp(f32* r, const f32* a, const f32* b, f32 t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
}

inline void f2_SubNorm(f32* r, const f32* a, const f32* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

inline void f2_SubScale(f32* r, const f32* a, const f32* b, f32 t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
}

inline void f2_NormScale(f32* r, const f32* a, f32 t) {
    f32 k = RSqrt(a[0] * a[0] + a[1] * a[1]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
}

inline void f2_SubNormScale(f32* r, const f32* a, const f32* b, f32 t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

inline void f2_SAdd(f32* r, const f32* a, const f32* b, f32 t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
}

inline void f2_SSub(f32* r, const f32* a, const f32* b, f32 t) {
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
}

inline void f2_Zero(f32* r) {
    r[0] = 0;
    r[1] = 0;
}

inline void f2_Neg(f32 *r, const f32 *a) {
    r[0] = -a[0];
    r[1] = -a[1];
}

inline void f2_Proj(f32 *r, const f32 *a, const f32 *b) {
    f32 ab_bb = (a[0] * b[0] + a[1] * b[1]) / (b[0] * b[0] + b[1] * b[1]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
}

inline int f2_CircleIntersect(const f32 *p0, f32 r0, const f32 *p1, f32 r1) {
    f32 dx = p1[0] - p0[0];
    f32 dy = p1[1] - p0[1];

    f32 r = r0 + r1;

    return (dx * dx + dy * dy) < (r * r);
}

inline int f2_SegmentIsIntersectingCircle(const f32 *start, const f32 *end, const f32 *pos, f32 rad) {
    f32 a[2] = { start[0] - pos[0], start[1] - pos[1] };
    f32 b[2] = { end[0] - start[0], end[1] - start[1] };

    if ((a[0] * a[0] + a[1] * a[1]) > (b[0] * b[0] + b[1] * b[1]))
        return 0;

    f32 seg[2] = { end[0] - start[0], end[1] - start[1] };
    f32 cir[2] = { pos[0] - start[0], pos[1] - start[1] };

    f32 dot_sc = seg[0] * cir[0] + seg[1] * cir[1];

    if (dot_sc < 0.0f)
        return 0;

    f32 proj = dot_sc / (seg[0] * seg[0] + seg[1] * seg[1]);

    seg[0] *= proj;
    seg[1] *= proj;

    seg[0] = seg[0] - cir[0];
    seg[1] = seg[1] - cir[1];

    return (seg[0] * seg[0] + seg[1] * seg[1]) < (rad * rad);
}

// ----------------------------------- 3D ----------------------------------------

inline void f3_Set(f32 *out, f32 x, f32 y, f32 z) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

inline void f3_Copy(f32 *r, const f32 *a) {
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
}

inline void f3_Add(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

inline void f3_Sub(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

inline void f3_Scale(f32 *r, const f32 *v, f32 s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
}

inline f32 f3_Dot(const f32 *a, const f32 *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline f32 f3_Len(const f32 *v) {
    return Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

inline f32 f3_LenSq(const f32 *v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

inline f32 f3_DistSq(const f32 *a, const f32 *b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];
    f32 dz = b[2] - a[2];

    return dx * dx + dy * dy + dz * dz;
}

inline f32 f3_Dist(const f32 *a, const f32 *b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];
    f32 dz = b[2] - a[2];

    return Sqrt(dx * dx + dy * dy + dz * dz);
}

inline void f3_Norm(f32 *r, const f32 *v) {
    f32 k = RSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
}

inline void f3_Min(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
}

inline void f3_Max(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
}

inline void f3_Lerp(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
}

inline void f3_SubNorm(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

inline void f3_SubScale(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
}

inline void f3_NormScale(f32 *r, const f32 *a, f32 t) {
    f32 k = RSqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
}

inline void f3_SubNormScale(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

inline void f3_SAdd(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
}

inline void f3_SSub(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
    r[2] = a[2] - b[2] * t;
}

inline void f3_Zero(f32 *r) {
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
}

inline void f3_Neg(f32 *r, const f32 *a) {
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
}

inline void f3_Proj(f32 *r, const f32 *a, const f32 *b) {
    f32 ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
}

inline int f3_SphereIntersect(const f32 *p0, f32 r0, const f32 *p1, f32 r1) {
    f32 dx = p1[0] - p0[0];
    f32 dy = p1[1] - p0[1];
    f32 dz = p1[2] - p0[2];

    f32 r = r0 + r1;

    return (dx * dx + dy * dy + dz * dz) <  r * r;
}

// ------------------------------------ vec4 --------------------------------------- //

inline void f4_Set(f32 *out, f32 x, f32 y, f32 z, f32 w) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
    out[3] = w;
}

inline void f4_Copy(f32 *r, const f32 *a) {
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
    r[3] = a[3];
}

inline void f4_Add(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

inline void f4_Sub(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}

inline void f4_Mul(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
    r[3] = a[3] * b[3];
}

inline void f4_Scale(f32 *r, const f32 *v, f32 s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
    r[3] = v[3] * s;
}

inline f32 f4_Dot(const f32 *a, const f32 *b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

inline f32 f4_Len(const f32 *v) {
    return Sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

inline f32 f4_LenSq(const f32 *v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

inline f32 f4_DistSq(const f32 *a, const f32 *b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];
    f32 dz = b[2] - a[2];
    f32 dw = b[3] - a[3];
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

inline f32 f4_Dist(const f32 *a, const f32 *b) {
    f32 dx = b[0] - a[0];
    f32 dy = b[1] - a[1];
    f32 dz = b[2] - a[2];
    f32 dw = b[3] - a[3];

    return Sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

inline void f4_Norm(f32 *r, const f32 *v) {
    f32 k = RSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
    r[3] = v[3] * k;
}

inline void f4_Min(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
    r[3] = a[3] < b[3] ? a[3] : b[3];
}

inline void f4_Max(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
    r[3] = a[3] > b[3] ? a[3] : b[3];
}

inline void f4_Lerp(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
    r[3] = a[3] + t * (b[3] - a[3]);
}

inline void f4_SubNorm(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

inline void f4_SubScale(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
    r[3] = (a[3] - b[3]) * t;
}

inline void f4_NormScale(f32 *r, const f32 *a, f32 t) {
    f32 k = RSqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
    r[3] = a[3] * k;
}

inline void f4_SubNormScale(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

inline void f4_SAdd(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
    r[3] = a[3] + b[3] * t;
}

inline void f4_SSub(f32 *r, const f32 *a, const f32 *b, f32 t) {
    r[0] = a[0] - b[0] * t;
    r[1] = a[1] - b[1] * t;
    r[2] = a[2] - b[2] * t;
    r[3] = a[3] - b[3] * t;
}

inline void f4_Zero(f32 *r) {
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
}

inline void f4_Neg(f32 *r, const f32 *a) {
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
    r[3] = -a[3];
}

inline void f4_Proj(f32 *r, const f32 *a, const f32 *b) {
    f32 ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2] + b[3] * b[3]);

    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
    r[3] = b[3] * ab_bb;
}

// --------------------------------- vecN ---------------------------------- //

inline f32 f2_Det(const f32 *a, const f32 *b) {
    return a[0] * b[1] - b[0] * a[1];
}

inline f32 f2_GetAngle(const f32 *a, const f32 *b) {
    f32 det = a[0] * b[1] - b[0] * a[1];
    f32 dot = a[0] * b[0] + a[1] * b[1];

    return atan2f(det, dot);
}

inline void f2_Rotate(f32 *r, const f32 *a, f32 rot) {
    f32 c = Cos(rot);
    f32 s = Sin(rot);

    f32 b[2] = {
        a[0] * c - a[1] * s,
        a[0] * s + a[1] * c
    };

    r[0] = b[0];
    r[1] = b[1];
}

inline void f3_Cross(f32 *r, const f32 *a, const f32 *b) {
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

inline void f3_CrossNorm(f32 *r, const f32 *a, const f32 *b) {
    r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];

    f32 k = RSqrt(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

    r[0] *= k;
    r[1] *= k;
    r[2] *= k;
}

inline void f2_Reflect(f32 *r, const f32 *v, const f32 *n) {
	f32 p  = 2.0f * f2_Dot(v, n);

    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
}

inline void f3_Reflect(f32 *r, const f32 *v, const f32 *n) {
	f32 p  = 2.0f * f3_Dot(v, n);

    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
    r[2] = v[2] - p * n[2];
}

inline void f4_Reflect(f32 *r, const f32 *v, const f32 *n) {
	f32 p  = 2.0f * (v[0] * n[0] + v[1] * n[1] + v[2] * n[2] + v[3] * n[3]);

	r[0] = v[0] - p * n[0];
	r[1] = v[1] - p * n[1];
	r[2] = v[2] - p * n[2];
	r[3] = v[3] - p * n[3];
}

inline void f2_Spline(f32 *out, f32 f, const f32 *a, const f32 *b, const f32 *c, const f32 *d) {
	f32 i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
}

inline void f3_Spline(f32 *out, f32 f, const f32 *a, const f32 *b, const f32 *c, const f32 *d) {
	f32 i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
	out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
}

inline void f4_Spline(f32 *out, f32 f, const f32 *a, const f32 *b, const f32 *c, const f32 *d) {
	f32 i = 1.0f - f;
	out[0] = ((d[0] * f + c[0] * i) * f + (c[0] * f + b[0] * i) * i) * f + ((c[0] * f + b[0] * i) * f + (b[0] * f + a[0] * i) * i) * i;
	out[1] = ((d[1] * f + c[1] * i) * f + (c[1] * f + b[1] * i) * i) * f + ((c[1] * f + b[1] * i) * f + (b[1] * f + a[1] * i) * i) * i;
	out[2] = ((d[2] * f + c[2] * i) * f + (c[2] * f + b[2] * i) * i) * f + ((c[2] * f + b[2] * i) * f + (b[2] * f + a[2] * i) * i) * i;
	out[3] = ((d[3] * f + c[3] * i) * f + (c[3] * f + b[3] * i) * i) * f + ((c[3] * f + b[3] * i) * f + (b[3] * f + a[3] * i) * i) * i;
}

inline void f2_Flatten(f32 *out, const f32 *pos, const f32 *normal) {
	f32 f = pos[0] * normal[0] + pos[1] * normal[1];

	out[0] = pos[0] - normal[0] * f;
	out[1] = pos[1] - normal[1] * f;
}

inline void f3_Flatten(f32 *out, const f32 *pos, const f32 *normal) {
	f32 f = pos[0] * normal[0] + pos[1] * normal[1] + pos[2] * normal[2];

	out[0] = pos[0] - normal[0] * f;
	out[1] = pos[1] - normal[1] * f;
	out[2] = pos[2] - normal[2] * f;
}

inline void f4_Cross(f32 *r, const f32 *a, const f32 *b) {
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
	r[3] = 1.0f;
}

inline void f2_LineCenter(f32 *out, const f32 *a, const f32 *b) {
    out[0] = 0.5f * (a[0] + b[0]);
    out[1] = 0.5f * (a[1] + b[1]);
}

inline void f2_GetLinesIntersectPoint(f32 *out, const f32 *a0, const f32 *a1, const f32 *b0, const f32 *b1) {
    f32 x, y;

	x  = (a0[0] * a1[1]  -  a0[1] * a1[0]) * (b0[0] - b1[0]) - (b0[0] * b1[1] - b0[1] * b1[0]) * (a0[0] - a1[0]);
	x /= (a0[0] - a1[0]) * (b0[1] - b1[1]) - (a0[1] - a1[1]) * (b0[0] - b1[0]);
	y  = (a0[0] * a1[1]  -  a0[1] * a1[0]) * (b0[1] - b1[1]) - (b0[0] * b1[1] - b0[1] * b1[0]) * (a0[1] - a1[1]);
	y /= (a0[0] - a1[0]) * (b0[1] - b1[1]) - (a0[1] - a1[1]) * (b0[0] - b1[0]);

    out[0] = x;
    out[1] = y;
}

inline int f2_LinesContainPoint(const f32 *point, const f32 *a0, const f32 *a1, const f32 *b0, const f32 *b1) {
    f32 c0[2] = { 0.5f * (a0[0] + a1[0]), 0.5f * (a0[1] + a1[1]) };
    f32 c1[2] = { 0.5f * (b0[0] + b1[0]), 0.5f * (b0[1] + b1[1]) };

    f32 r0 = 0.5f * f2_Dist(a0, a1);
    f32 r1 = 0.5f * f2_Dist(b0, b1);

    return f2_DistSq(point, c0) < (r0 * r0) && f2_DistSq(point, c1) < (r1 * r1);
}

inline int f2_LinesIntersectTest(const f32 *a0, const f32 *a1, const f32 *b0, const f32 *b1) {
    f32 intersect[2];
    f2_GetLinesIntersectPoint(intersect, a0, a1, b0, b1);
    return f2_LinesContainPoint(intersect, a0, a1, b0, b1);
}

#define F_EPSILON 0.000001f

inline int f3_RaycastTri(f32 orig[3], f32 dir[3], f32 vert0[3], f32 vert1[3], f32 vert2[3], f32 *t, f32 *u, f32 *v) {
	f32 edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	f32 det, inv_det;

	edge1[0] = vert1[0] - vert0[0];
	edge1[1] = vert1[1] - vert0[1];
	edge1[2] = vert1[2] - vert0[2]; 
	edge2[0] = vert2[0] - vert0[0];
	edge2[1] = vert2[1] - vert0[1];
	edge2[2] = vert2[2] - vert0[2];

	pvec[0] = dir[1] * edge2[2] - dir[2] * edge2[1];
	pvec[1] = dir[2] * edge2[0] - dir[0] * edge2[2];
	pvec[2] = dir[0] * edge2[1] - dir[1] * edge2[0];

	det = edge1[0] * pvec[0] + edge1[1] * pvec[1] + edge1[2] * pvec[2];

	if(det > -F_EPSILON && det < F_EPSILON) {
		return 0;
    }

	inv_det = 1.0 / det;

	tvec[0] = orig[0] - vert0[0];
	tvec[1] = orig[1] - vert0[1];
	tvec[2] = orig[2] - vert0[2];

	*u = (tvec[0] * pvec[0] + tvec[1] * pvec[1] + tvec[2] * pvec[2]) * inv_det;

	if(*u < 0.0 || *u > 1.0) {
		return 0;
    }

	qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
	qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
	qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];

	*v = (dir[0] * qvec[0] + dir[1] * qvec[1] + dir[2] * qvec[2]) * inv_det;

	if(*v < 0.0 || *u + *v > 1.0) {
		return 0;
    }

	*t = (edge2[0] * qvec[0] + edge2[1] * qvec[1] + edge2[2] * qvec[2]) * inv_det;

	return 1;
}

inline int f3_RaycastTriCull(f32 orig[3], f32 dir[3], f32 vert0[3], f32 vert1[3], f32 vert2[3], f32 *t, f32 *u, f32 *v) {
	f32 edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	f32 det, inv_det;

	edge1[0] = vert1[0] - vert0[0];
	edge1[1] = vert1[1] - vert0[1];
	edge1[2] = vert1[2] - vert0[2]; 
	edge2[0] = vert2[0] - vert0[0];
	edge2[1] = vert2[1] - vert0[1];
	edge2[2] = vert2[2] - vert0[2];

	pvec[0] = dir[1] * edge2[2] - dir[2] * edge2[1];
	pvec[1] = dir[2] * edge2[0] - dir[0] * edge2[2];
	pvec[2] = dir[0] * edge2[1] - dir[1] * edge2[0];

	det = edge1[0] * pvec[0] + edge1[1] * pvec[1] + edge1[2] * pvec[2];
 
	if(det < F_EPSILON) {
		return 0;
    }

	tvec[0] = orig[0] - vert0[0];
	tvec[1] = orig[1] - vert0[1];
	tvec[2] = orig[2] - vert0[2];

	*u = tvec[0] * pvec[0] + tvec[1] * pvec[1] + tvec[2] * pvec[2];

	if(*u < 0.0 || *u > det) {
		return 0;
    }

	qvec[0] = tvec[1] * edge1[2] - tvec[2] * edge1[1];
	qvec[1] = tvec[2] * edge1[0] - tvec[0] * edge1[2];
	qvec[2] = tvec[0] * edge1[1] - tvec[1] * edge1[0];
	*v = dir[0] * qvec[0] + dir[1] * qvec[1] + dir[2] * qvec[2];

	if(*v < 0.0 || *u + *v > det) {
		return 0;
    }

	*t = edge2[0] * qvec[0] + edge2[1] * qvec[1] + edge2[2] * qvec[2];
	inv_det = 1.0 / det;
	*t *= inv_det;
	*u *= inv_det;
	*v *= inv_det;

	return 1;
}

// ---------------------------------------- matrix 2D --------------------------------------- //

inline void f2x2_Identity(f32 *M) {
    M[0] = 1;
    M[1] = 0;
    M[2] = 0;
    M[3] = 1;
}

inline void f2x2_Mul(f32 *M, const f32 *a, const f32 *b) {
    f32 T[4];

    T[0] = a[0] * b[0] + a[2] * b[1];
    T[1] = a[1] * b[0] + a[3] * b[1];
    T[2] = a[0] * b[2] + a[2] * b[3];
    T[3] = a[1] * b[2] + a[3] * b[3];

    M[0] = T[0];
    M[1] = T[1];
    M[2] = T[2];
    M[3] = T[3];
}

inline void f2x2_Mulv(f32 *r, const f32 *R, const f32 *v) {
    f32 t[2];

    t[0] = R[0] * v[0] + R[2] * v[1];
    t[1] = R[1] * v[0] + R[3] * v[1];

    r[0] = t[0];
    r[1] = t[1];
}

inline void f2x2_Rotate(f32 *M, f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    M[0] =  c;
    M[1] =  s;
    M[2] = -s;
    M[3] =  c;
}

inline void f2x2_Scale(f32 *M, f32 sx, f32 sy) {
    M[0] = sx;
    M[1] = 0;
    M[2] = 0;
    M[3] = sy;
}

// ---------------------------------------- Mat 3D --------------------------------------- //

inline void f3x3_Mul(f32 *R, const f32 *a, const f32 *b) {
    f32 M[16];

    M[0] = a[0] * b[0] + a[3] * b[1]  + a[6] * b[2];
    M[1] = a[1] * b[0] + a[4] * b[1]  + a[7] * b[2];
    M[2] = a[2] * b[0] + a[5] * b[1]  + a[8] * b[2];

    M[3] = a[0] * b[3] + a[3] * b[4]  + a[6] * b[5];
    M[4] = a[1] * b[3] + a[4] * b[4]  + a[7] * b[5];
    M[5] = a[2] * b[3] + a[5] * b[4]  + a[8] * b[5];

    M[6] = a[0] * b[6] + a[3] * b[7]  + a[6] * b[8];
    M[7] = a[1] * b[6] + a[4] * b[7]  + a[7] * b[8];
    M[8] = a[2] * b[6] + a[5] * b[7]  + a[8] * b[8];

    R[0] = M[0];
    R[1] = M[1];
    R[2] = M[2];

    R[3] = M[3];
    R[4] = M[4];
    R[5] = M[5];

    R[6] = M[6];
    R[7] = M[7];
    R[8] = M[8];
}

inline void f3x3_Mulv(f32 *out, const f32 *M, const f32 *v) {
    f32 r[4];

	r[0] = M[0] * v[0] + M[3] * v[1] + M[6] * v[2];
	r[1] = M[1] * v[0] + M[4] * v[1] + M[7] * v[2];
	r[2] = M[2] * v[0] + M[5] * v[1] + M[8] * v[2];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}

inline void f3x3_Rotate(f32 *R, const f32 *axis, f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    f32 k = 1.0f - c;

    f32 sa[3]   = { s * axis[0], s * axis[1], s * axis[2] };
    f32 omca[3] = { k * axis[0], k * axis[1], k * axis[2] };

    R[0] = omca[0] * axis[0] + c;
    R[1] = omca[0] * axis[1] - sa[2];
    R[2] = omca[0] * axis[2] + sa[1],

    R[3] = omca[1] * axis[0] + sa[2];
    R[4] = omca[1] * axis[1] + c;
    R[5] = omca[1] * axis[2] - sa[0];

    R[6] = omca[2] * axis[0] - sa[1];
    R[7] = omca[2] * axis[1] + sa[0];
    R[8] = omca[2] * axis[2] + c;
}

inline void f3x3_RotateEuler(f32 *R, f32 yaw, f32 pitch, f32 roll) {
    f32 cy = Cos(yaw);
    f32 sy = Sin(yaw);
    f32 cp = Cos(pitch);
    f32 sp = Sin(pitch);
    f32 cr = Cos(roll);
    f32 sr = Sin(roll);

    R[0] = cy * cp;
    R[1] = sy * cp;
    R[2] = -sp;

    R[3] = cy * sp * sr - sy * cr;
    R[4] = sy * sp * sr + cy * cr;
    R[5] = cp * sr;

    R[6] = cy * sp * cr + sy * sr;
    R[7] = sy * sp * cr - cy * sr;
    R[8] = cp * cr;
}

inline void f3x3_Rotatef3(f32 *out, const f32 *point, const f32 *axis, f32 angle) {
    f32 R[9];

    f3x3_Rotate(R, axis, angle);
    f3x3_Mulv(out, R, point);
}

inline void f3x3_FromQuat(f32 *M, const f32 *q) {
    f32 a = q[3];
	f32 b = q[0];
	f32 c = q[1];
	f32 d = q[2];

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

	M[0] = a2 + b2 - c2 - d2;
	M[1] = 2.0f * (b * c + a * d);
	M[2] = 2.0f * (b * d - a * c);

	M[3] = 2.0f * (b * c - a * d);
	M[4] = a2 - b2 + c2 - d2;
	M[5] = 2.0f * (c * d + a * b);

	M[6] = 2.0f * (b * d + a * c);
	M[7] = 2.0f * (c * d - a * b);
	M[8] = a2 - b2 - c2 + d2;
}

// ---------------------------------------- Mat 4D --------------------------------------- //

inline void f4x4_Identity(f32 *M) {
	M[0]  = 1;
	M[1]  = 0;
	M[2]  = 0;
	M[3]  = 0;

	M[4]  = 0;
	M[5]  = 1;
	M[6]  = 0;
	M[7]  = 0;

	M[8]  = 0;
	M[8]  = 0;
	M[10] = 1;
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = 0;
	M[15] = 1;
}

inline void f4x4_Copy(f32 *M, const f32 *N) {
	M[0]  = N[0];
	M[1]  = N[1];
	M[2]  = N[2];
	M[3]  = N[3];

	M[4]  = N[4];
	M[5]  = N[5];
	M[6]  = N[6];
	M[7]  = N[7];

	M[8]  = N[8];
	M[9]  = N[9];
	M[10] = N[10];
	M[11] = N[11];

	M[12] = N[12];
	M[13] = N[13];
	M[14] = N[14];
	M[15] = N[15];
}

inline void f4x4_Transpose(f32 *M, const f32 *N) {
	M[0]  = N[0];
	M[4]  = N[1];
	M[8]  = N[2];
	M[12] = N[3];

	M[1]  = N[4];
	M[5]  = N[5];
	M[9]  = N[6];
	M[13] = N[7];

	M[2]  = N[8];
	M[6]  = N[9];
	M[10] = N[10];
	M[14] = N[11];

	M[3]  = N[12];
	M[7]  = N[13];
	M[11] = N[14];
	M[15] = N[15];
}

inline void f4x4_Add(f32 *M, const f32 *a, const f32 *b) {
    M[0]  = a[0]  + b[0];
    M[1]  = a[1]  + b[1];
    M[2]  = a[2]  + b[2];
    M[3]  = a[3]  + b[3];

    M[4]  = a[4]  + b[4];
    M[5]  = a[5]  + b[5];
    M[6]  = a[6]  + b[6];
    M[7]  = a[7]  + b[7];

    M[8]  = a[8]  + b[8];
    M[9]  = a[9]  + b[9];
    M[10] = a[10] + b[10];
    M[11] = a[11] + b[11];

    M[12] = a[12] + b[12];
    M[13] = a[13] + b[13];
    M[14] = a[14] + b[14];
    M[15] = a[15] + b[15];
}

inline void f4x4_Sub(f32 *M, const f32 *a, const f32 *b) {
    M[0]  = a[0]  - b[0];
    M[1]  = a[1]  - b[1];
    M[2]  = a[2]  - b[2];
    M[3]  = a[3]  - b[3];

    M[4]  = a[4]  - b[4];
    M[5]  = a[5]  - b[5];
    M[6]  = a[6]  - b[6];
    M[7]  = a[7]  - b[7];

    M[8]  = a[8]  - b[8];
    M[9]  = a[9]  - b[9];
    M[10] = a[10] - b[10];
    M[11] = a[11] - b[11];

    M[12] = a[12] - b[12];
    M[13] = a[13] - b[13];
    M[14] = a[14] - b[14];
    M[15] = a[15] - b[15];
}

inline void f4x4_Scale(f32 *M, f32 x, f32 y, f32 z) {
    M[0]  = x;
    M[1]  = 0.0f;
    M[2]  = 0.0f;
    M[3]  = 0.0f;

    M[4]  = 0.0f;
    M[5]  = y;
    M[6]  = 0.0f;
    M[7]  = 0.0f;

    M[8]  = 0.0f;
    M[9]  = 0.0f;
    M[10] = z;
    M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
    M[15] = 1.0f;
}

inline void f4x4_ScaleAniso(f32 *M, const f32* a, f32 x, f32 y, f32 z) {
	M[0]  = a[0]  * x;
	M[1]  = a[1]  * x;
	M[2]  = a[2]  * x;
	M[3]  = a[3]  * x;

	M[4]  = a[4]  * y;
	M[5]  = a[5]  * y;
	M[6]  = a[6]  * y;
	M[7]  = a[7]  * y;

	M[8]  = a[8]  * z;
	M[9]  = a[9]  * z;
	M[10] = a[10] * z;
	M[11] = a[11] * z;

	M[12] = a[12];
	M[13] = a[13];
	M[14] = a[14];
	M[15] = a[15];
}

inline void f4x4_Mul(f32 *R, const f32 *a, const f32 *b) {
    f32 M[16];

    M[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
    M[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
    M[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
    M[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

    M[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
    M[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
    M[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
    M[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

    M[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
    M[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
    M[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
    M[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

    M[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
    M[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
    M[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    M[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

    R[0] = M[0];
    R[1] = M[1];
    R[2] = M[2];
    R[3] = M[3];

    R[4] = M[4];
    R[5] = M[5];
    R[6] = M[6];
    R[7] = M[7];

    R[8] = M[8];
    R[9] = M[9];
    R[10] = M[10];
    R[11] = M[11];

    R[12] = M[12];
    R[13] = M[13];
    R[14] = M[14];
    R[15] = M[15];
}

inline void f4x4_Mulv(f32 *out, const f32 *M, const f32 *v) {
    f32 r[4];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
	r[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
    out[3] = r[3];
}

inline void f4x4_Translate(f32 *T, f32 x, f32 y, f32 z) {
    T[0]  = 1;
	T[1]  = 0;
	T[2]  = 0;
	T[3]  = 0;

	T[4]  = 0;
	T[5]  = 1;
	T[6]  = 0;
	T[7]  = 0;

	T[8]  = 0;
	T[9]  = 0;
	T[10] = 1;
	T[11] = 0;

	T[12] = x;
	T[13] = y;
	T[14] = z;
	T[15] = 1;
}

inline void f4x4_RotateX(f32 *R, f32 angle) {
	f32 s = Sin(angle);
	f32 c = Cos(angle);

	R[0]    = 1.0f;
    R[1]    = 0.0f;
    R[2]    = 0.0f;
    R[3]    = 0.0f;

	R[4]    = 0.0f;
    R[5]    = c;
    R[6]    = s;
    R[7]    = 0.0f;

	R[8]    = 0.0f;
    R[9]    = -s;
    R[10]   = c;
    R[11]   = 0.0f;

	R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

inline void f4x4_RotateY(f32 *R, f32 angle) {
	f32 s = Sin(angle);
	f32 c = Cos(angle);

	R[0]    = c;
    R[1]    = 0.0f;
    R[2]    = s;
    R[3]    = 0.0f;

	R[4]    = 0.0f;
    R[5]    = 1.0f;
    R[6]    = 0.0f;
    R[7]    = 0.0f;

	R[8]    = -s;
    R[9]    = 0.0f;
    R[10]   = c;
    R[11]   = 0.0f;

    R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

inline void f4x4_RotateZ(f32 *R, f32 angle) {
	f32 s = Sin(angle);
	f32 c = Cos(angle);

	R[0]    = c;
    R[1]    = s;
    R[2]    = 0.0f;
    R[3]    = 0.0f;

	R[4]    = -s;
    R[5]    = c;
    R[6]    = 0.0f;
    R[7]    = 0.0f;

	R[8]    = 0.0f;
    R[9]    = 0.0f;
    R[10]   = 1.0f;
    R[11]   = 0.0f;

	R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

inline void f4x4_RotateEuler(f32 *R, f32 yaw, f32 pitch, f32 roll) {
    f32 cy  = Cos(yaw);
    f32 sy  = Sin(yaw);
    f32 cp  = Cos(pitch);
    f32 sp  = Sin(pitch);
    f32 cr  = Cos(roll);
    f32 sr  = Sin(roll);

    R[0]    = cy * cp;
    R[1]    = sy * cp;
    R[2]    = -sp;
    R[3]    = 0.0f;

    R[4]    = cy * sp * sr - sy * cr;
    R[5]    = sy * sp * sr + cy * cr;
    R[6]    = cp * sr;
    R[7]    = 0.0f;

    R[8]    = cy * sp * cr + sy * sr;
    R[9]    = sy * sp * cr - cy * sr;
    R[10]   = cp * cr;
    R[11]   = 0.0f;

    R[12]   = 0.0f;
    R[13]   = 0.0f;
    R[14]   = 0.0f;
    R[15]   = 1.0f;
}

inline void f4x4_TranslateEuler(f32 *M, f32 px, f32 py, f32 pz, f32 yaw, f32 pitch, f32 roll) {
    f32 cy  = Cos(yaw);
    f32 sy  = Sin(yaw);
    f32 cp  = Cos(pitch);
    f32 sp  = Sin(pitch);
    f32 cr  = Cos(roll);
    f32 sr  = Sin(roll);

    M[0]    = cy * cp;
    M[1]    = sy * cp;
    M[2]    = -sp;
    M[3]    = 0.0f;

    M[4]    = cy * sp * sr - sy * cr;
    M[5]    = sy * sp * sr + cy * cr;
    M[6]    = cp * sr;
    M[7]    = 0.0f;

    M[8]    = cy * sp * cr + sy * sr;
    M[9]    = sy * sp * cr - cy * sr;
    M[10]   = cp * cr;
    M[11]   = 0.0f;

    M[12]   = px;
    M[13]   = py;
    M[14]   = pz;
    M[15]   = 1.0f;
}

// Rodrigues rotation formula
inline void f4x4_Rotate(f32 *M, const f32* axis, f32 angle) {
    f32 sa[3];
    f32 omca[3];

    f32 cosv      = Cos(angle);
    f32 sinv      = Sin(angle);
    f32 inv_cosv  = 1.0f - cosv;

    sa[0] = axis[0] * sinv;
    sa[1] = axis[1] * sinv;
    sa[2] = axis[2] * sinv;

    omca[0] = axis[0] * inv_cosv;
    omca[1] = axis[1] * inv_cosv;
    omca[2] = axis[2] * inv_cosv;

    M[0] = omca[0] * axis[0] + cosv;
    M[1] = omca[0] * axis[1] - sa[0];
    M[2] = omca[0] * axis[2] + sa[1];
    M[3] = 0.0f;

    M[4] = omca[1] * axis[0] + sa[2];
    M[5] = omca[1] * axis[1] + cosv;
    M[6] = omca[1] * axis[2] - sa[0],
    M[7] = 0.0f;

    M[8]  = omca[2] * axis[0] - sa[1];
    M[9]  = omca[2] * axis[1] + sa[0];
    M[10] = omca[2] * axis[2] + cosv;
    M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
    M[15] = 1.0f;
}

inline void f4x4_Frustum(f32 *M, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
	M[0] = 2.0f * n / (r - l);
	M[1] = 0.0f;
    M[2] = 0.0f;
    M[3] = 0.0f;

    M[4] = 0.0f;
	M[5] = 2.0f * n / (t - b);
	M[6] = 0.0f;
    M[7] = 0.0f;

	M[8]  =  (r + l) / (r - l);
	M[9]  =  (t + b) / (t - b);
	M[10] = -(f + n) / (f - n);
	M[11] = -1.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
	M[14] = -2.0f * (f * n) / (f - n);
	M[15] = 0.0f;
}

inline void f4x4_Ortho(f32 *M, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
	M[0] = 2.0f / (r - l);
	M[1] = 0.0f;
    M[2] = 0.0f;
    M[3] = 0.0f;

    M[4] = 0.0f;
	M[5] = 2.0f / (t - b);
	M[6] = 0.0f;
    M[7] = 0.0f;

    M[8]  = 0.0f;
    M[9]  = 0.0f;
	M[10] = -2.0f / (f - n);
	M[11] = 0.f;

	M[12] = -(r + l) / (r - l);
	M[13] = -(t + b) / (t - b);
	M[14] = -(f + n) / (f - n);
	M[15] = 1.0f;
}

inline void f4x4_Perspective(f32 *M, f32 y_fov, f32 aspect, f32 n, f32 f) {
	const f32 a = 1.0f / tanf(y_fov / 2.0f);

	M[0]  = a / aspect;
	M[1]  = 0.0f;
	M[2]  = 0.0f;
	M[3]  = 0.0f;

	M[4]  = 0.0f;
	M[5]  = a;
	M[6]  = 0.0f;
	M[7]  = 0.0f;

	M[8]  = 0.0f;
	M[9]  = 0.0f;
	M[10] = -((f + n) / (f - n));
	M[11] = -1.0f;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -((2.f * f * n) / (f - n));
	M[15] = 0.0f;
}

inline void f4x4_LookAt(f32* M, const f32* eye, const f32* center, const f32* up) {
	f32 f[3], s[3], t[3];

    f3_SubNorm(f, center, eye);
    f3_CrossNorm(s, f, up);
	f3_Cross(t, s, f);

	M[0]  =  s[0];
	M[1]  =  t[0];
	M[2]  = -f[0];
	M[3]  =   0.0f;

	M[4]  =  s[1];
	M[5]  =  t[1];
	M[6]  = -f[1];
	M[7]  =   0.0f;

	M[8]  =  s[2];
	M[9]  =  t[2];
	M[10] = -f[2];
	M[11] =   0.0f;

    M[12] = -(M[0] * eye[0] + M[4] * eye[1] + M[8]  * eye[2]);
    M[13] = -(M[1] * eye[0] + M[5] * eye[1] + M[9]  * eye[2]);
    M[14] = -(M[2] * eye[0] + M[6] * eye[1] + M[10] * eye[2]);
    M[15] = -(M[3] * eye[0] + M[7] * eye[1] + M[11] * eye[2] - 1.0f);
}

inline void f4x4_PerspectiveView(f32 *PV, const f32 *pos, const f32 *look, const f32 *up, f32 fov, f32 aspect, f32 n, f32 f) {
    f32 P[16], V[16];

    f4x4_Perspective(P, TO_RAD(fov), aspect, n, f),
    f4x4_LookAt(V, pos, look, up);
    f4x4_Mul(PV, P, V);
}

inline void f4x4_OrthoView(f32 *PV, const f32 *pos, const f32 *look, const f32 *up, const f32 *rad, f32 n, f32 f) {
    f32 P[16], V[16];

    f4x4_Ortho(P, -rad[0], +rad[0], -rad[1], +rad[1], n, f);
    f4x4_LookAt(V, pos, look, up);
    f4x4_Mul(PV, P, V);
}

inline void f4x4_TranslateScale(f32 *M, f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz) {
    M[0]  = sx;
    M[1]  = 0;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = 0;
    M[5]  = sy;
    M[6]  = 0;
    M[7]  = 0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

inline void f4x4_TranslateScaleRotateZ(f32 *M, f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz, f32 angle) {
    f32 s = Sin(angle);
    f32 c = Cos(angle);

    M[0]  = c * sx;
    M[1]  = s * sx;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = -s * sy;
    M[5]  =  c * sy;
    M[6]  =  0;
    M[7]  =  0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

inline void f4x4_Fromfq(f32 *M, const f32 *q) {
    f32 a = q[3];
	f32 b = q[0];
	f32 c = q[1];
	f32 d = q[2];

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

	M[0] = a2 + b2 - c2 - d2;
	M[1] = 2.0f * (b * c + a * d);
	M[2] = 2.0f * (b * d - a * c);
	M[3] = 0.f;

	M[4] = 2.0f * (b * c - a * d);
	M[5] = a2 - b2 + c2 - d2;
	M[6] = 2.0f * (c * d + a * b);
	M[7] = 0.f;

	M[8]  = 2.0f * (b * d + a * c);
	M[9]  = 2.0f * (c * d - a * b);
	M[10] = a2 - b2 - c2 + d2;
	M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
	M[15] = 1.0f;
}

// -------------------------------------------------- QUATERNIONS ------------------------------------------------ //

inline void fq_Identity(f32 *out) {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
}

inline void fq_Make(f32 *out, f32 x, f32 y, f32 z, f32 angle) {
    f32 inv_len = RSqrt((x * x) + (y * y) + (z * z));
    f32 s = inv_len * Sin(angle / 2.0f);

    out[0] = x * s;
    out[1] = y * s;
    out[2] = z * s;
    out[3] = Cos(angle / 2.0f);
}

inline void fq_Add(f32 *out, const f32 *a, const f32 *b) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}

inline void fq_Conj(f32 *out, const f32 *q) {
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] =  q[3];
}

inline void fq_Mul(f32 *out, const f32 *a, const f32 *b) {
    f32 c[4];

    c[0] = a[1] * b[2] - a[2] * b[1] + a[3] * b[0] + b[3] * a[0];
    c[1] = a[2] * b[0] - a[0] * b[2] + a[3] * b[1] + b[3] * a[1];
    c[2] = a[0] * b[1] - a[1] * b[0] + a[3] * b[2] + b[3] * a[2];
    c[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];

    out[0] = c[0];
    out[1] = c[1];
    out[2] = c[2];
    out[3] = c[3];
}


// p = q * p * q^-1
inline void fq_Mulv(f32 *out, const f32 *quat, const f32 *point) {
    f32 p[4] = { point[0], point[1], point[2], 0 };

    f32 conj[4];

    fq_Conj(conj, quat);
    fq_Mul(out, quat, p);
    fq_Mul(p, out, conj);

    out[0] = p[0];
    out[1] = p[1];
    out[2] = p[2];
}

inline void fq_Rotate(f32 *r, const f32 *axis, f32 angle) {
    f32 s = Sin(0.5f * angle), v[3] = { s * axis[0], s * axis[1], s * axis[2] };

    r[0] = v[0];
    r[1] = v[1];
    r[2] = v[2];
    r[3] = Cos(0.5f * angle);
}

// ============================================== RANDOM GENERATOR =========================================== //
// random number generator: xorshf96

struct RndGen {
    u32    x;
    u32    y;
    u32    z;
};

thread_local RndGen default_rnd_gen = { 123456789u, 362436069u, 521288629u };

inline u32 NextRandom(RndGen* g) {
    g->x  ^= g->x << 16;
    g->x  ^= g->x >> 5;
    g->x  ^= g->x << 1;

    u32 t = g->x;

    g->x  = g->y;
    g->y  = g->z;
    g->z  = t ^ g->x ^ g->y;

    return g->z;
}

inline int iRand(int min, int max) {
    return min + NextRandom(&default_rnd_gen) % (max - min);
}

inline f32 fRand(f32 min, f32 max) {
    return min + ((f32)NextRandom(&default_rnd_gen) / (f32)0xFFFFFFFF) * (max - min); 
}

inline void f2_Rand(f32 *out, f32 min, f32 max) {
    out[0] = fRand(-1.0f, 1.0f);
    out[1] = fRand(-1.0f, 1.0f);

    f32 k = RSqrt(out[0] * out[0] + out[1] * out[1]) * fRand(min, max);

    out[0] *= k;
    out[1] *= k;
}

inline void f2_RandUnit(f32 *out) {
    out[0] = fRand(-1.0f, 1.0f);
    out[1] = fRand(-1.0f, 1.0f);

    f32 k = RSqrt(out[0] * out[0] + out[1] * out[1]);

    out[0] *= k;
    out[1] *= k;
}

inline void f3_Rand(f32 *out, f32 min, f32 max) {
    out[0] = fRand(-1.0f, 1.0f);
    out[1] = fRand(-1.0f, 1.0f);
    out[2] = fRand(-1.0f, 1.0f);

    f32 k = RSqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]) * fRand(min, max);

    out[0] *= k;
    out[1] *= k;
    out[2] *= k;
}

inline void f3_RandUnit(f32 *out) {
    out[0] = fRand(-1.0f, 1.0f);
    out[1] = fRand(-1.0f, 1.0f);
    out[2] = fRand(-1.0f, 1.0f);

    f32 k = RSqrt(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);

    out[0] *= k;
    out[1] *= k;
    out[2] *= k;
}

inline void f2_AddRand(f32 *out, f32* u, f32 min, f32 max) {
    f32 r[2];
    f2_Rand(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
}

inline void f3_AddRand(f32 *out, f32* u, f32 min, f32 max) {
    f32 r[3];
    f3_Rand(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
    out[2] = u[2] + r[2];
}

inline void fq_Rand(f32 *quat) {
    fq_Make(quat, fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f), fRand(-1.0f, 1.0f), fRand(-PI, PI));
}

inline v2 v2_Rand(f32 min, f32 max) {
    v2 out;
    f2_Rand(out.array, min, max);
    return out;
}

inline v2 v2_RandUnit(void) {
    v2 out;
    f2_RandUnit(out.array);
    return out;
}

inline v3 v3_Rand(f32 min, f32 max) {
    v3 out;
    f3_Rand(out.array, min, max);
    return out;
}

inline v3 v3_RandUnit(void) {
    v3 out;
    f3_RandUnit(out.array);
    return out;
}

inline Quat QuatRand(void) {
    Quat out;
    fq_Rand(out.array);
    return out;
}

// ===================================== CSTR ==================================== //

inline void cStringCopy(char* dst, const char* src) {
    while ((*(dst++) = *(src++)) != '\0');
}

inline int cStringIsDel(char c, const char* del) {
    while (*del != '\0')
        if (c == *(del++)) return 1;
    return 0;
}

// TODO: make the function return NULL if there were no splits:
inline char** cStringSplitInPlace(char* str, const char* del, size_t* count) {
    size_t  split_capacity  = 8;
    size_t  split_count     = 0;
    char**  split_array     = (char**)malloc(split_capacity * sizeof *split_array);

    while (*str != '\0') {
        while (*str && cStringIsDel(*str, del)) { *(str++) = '\0'; }

        if (!*str) break;

        if (split_count + 1 >= split_capacity) {
            split_capacity  <<= 1;
            split_array     = (char**)realloc(split_array, split_capacity * sizeof *split_array);
        }

        split_array[split_count++] = str;

        while (*str && !cStringIsDel(*(++str), del));

        if (!*str) break;

        *(str++) = '\0';
    }

    *count = split_count;

    return split_array;
}

inline int cStringFind(const char* str, const char *item) {
    int item_len = 0;
    while (item[item_len] != '\0')
        item_len++;

    int end = item_len - 1;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i + end] != item[end]) {
            i += end;
            continue;
        }

        int j = 0;

        while (item[j] != '\0' && str[j + i] == item[j])
            ++j;

        if (item[j] == '\0')
            return i;
    }

    return -1;
}

// ================================================== STRUCT MATH ================================================== //

// v2:
inline v2 V2(f32 x, f32 y) {
    v2 out = { x, y };
    return out;
}

inline v2 Neg(v2 a) {
    v2 out = { -a.x, -a.y };
    return out;
}

inline v2 Add(v2 a, v2 b) {
    v2 out = { a.x + b.x, a.y + b.y };
    return out;
}

inline v2 Sub(v2 a, v2 b) {
    v2 out = { a.x - b.x, a.y - b.y };
    return out;
}

inline v2 Mul(v2 a, v2 b) {
    v2 out = { a.x * b.x, a.y * b.y };
    return out;
}

inline v2 Scale(v2 a, f32 s) {
    v2 out = { a.x * s, a.y * s };
    return out;
}

inline f32 Dot(v2 a, v2 b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 LenSq(v2 v) {
    return Dot(v, v);
}

inline f32 Len(v2 v) {
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v2 a, v2 b) {
    return LenSq(Sub(b, a));
}

inline f32 Dist(v2 a, v2 b) {
    return Sqrt(DistSq(a, b));
}

inline v2 Norm(v2 v) {
    return Scale(v, RSqrt(Dot(v, v)));
}

inline v2 Min(v2 a, v2 b) {
    v2 out = {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y)
    };
    return out;
}

inline v2 Max(v2 a, v2 b) {
    v2 out = {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y)
    };
    return out;
}

inline v2 Lerp(v2 a, v2 b, f32 t) {
    v2 out = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y)
    };
    return out;
}

inline f32 GetAngle(v2 a, v2 b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;
    
    return atan2f(det, dot);
}

inline v2 Spline(f32 f, v2 a, v2 b, v2 c, v2 d) {
	f32 i = 1.0f - f;

    v2 out = {
	    ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
	    ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
    };

    return out;
}

inline int CircleIntersect(v2 p0, f32 r0, v2 p1, f32 r1) {
    f32 dx = p1.x - p0.x;
    f32 dy = p1.y - p0.y;

    f32 r = r0 + r1;

    return (dx * dx + dy * dy) < (r * r);
}

inline int SegmentIsIntersectingCircle(v2 start, v2 end, v2 pos, f32 rad) {
    v2 a = { start.x - pos.x, start.y - pos.y };
    v2 b = { end.x - start.x, end.y - start.y };

    if ((a.x * a.x + a.y * a.y) > (b.x * b.x + b.y * b.y))
        return 0;

    v2 seg = { end.x - start.x, end.y - start.y };
    v2 cir = { pos.x - start.x, pos.y - start.y };

    f32 dot_sc = seg.x * cir.x + seg.y * cir.y;

    if (dot_sc < 0.0f)
        return 0;

    f32 proj = dot_sc / (seg.x * seg.x + seg.y * seg.y);

    seg.x *= proj;
    seg.y *= proj;

    seg.x = seg.x - cir.x;
    seg.y = seg.y - cir.y;

    return (seg.x * seg.x + seg.y * seg.y) < (rad * rad);
}

inline v3 Neg(v3 a) {
    v3 out = { -a.x, -a.y, -a.z };
    return out;
}

inline v3 Add(v3 a, v3 b) {
    v3 out = { a.x + b.x, a.y + b.y, a.z + b.z };
    return out;
}

inline v3 Sub(v3 a, v3 b) {
    v3 out = { a.x - b.x, a.y - b.y, a.z - b.z };
    return out;
}

inline v3 Mul(v3 a, v3 b) {
    v3 out = { a.x * b.x, a.y * b.y, a.z * b.z };
    return out;
}

inline v3 Scale(v3 a, f32 s) {
    v3 out = { a.x * s, a.y * s, a.z * s };
    return out;
}

inline f32 Dot(v3 a, v3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 LenSq(v3 v) {
    return Dot(v, v);
}

inline f32 Len(v3 v) {
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v3 a, v3 b) {
    return LenSq(Sub(b, a));
}

inline f32 Dist(v3 a, v3 b) {
    return Sqrt(DistSq(a, b));
}

inline v3 Norm(v3 v) {
    return Scale(v, RSqrt(Dot(v, v)));
}

inline v3 Min(v3 a, v3 b) {
    v3 out = {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y),
        (a.z < b.z? a.z : b.z)
    };
    return out;
}

inline v3 Max(v3 a, v3 b) {
    v3 out = {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y),
        (a.z > b.z? a.z : b.z)
    };
    return out;
}

inline v3 Lerp(v3 a, v3 b, f32 t) {
    v3 out = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z)
    };

    return out;
}

inline v3 Cross(v3 a, v3 b) {
    v3 r;

	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

    return r;
}

inline v3 Spline(f32 f, v3 a, v3 b, v3 c, v3 d) {
	f32 i = 1.0f - f;

    v3 out = {
        ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
        ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
        ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i
    };

    return out;
}

// v4:
inline v4 V4(f32 x, f32 y, f32 z, f32 w) {
    v4 out = { x, y, z, w };
    return out;
}

inline v4 Neg(v4 a) {
    v4 out = { -a.x, -a.y, -a.z, -a.w };
    return out;
}

inline v4 Add(v4 a, v4 b) {
    v4 out = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return out;
}

inline v4 Sub(v4 a, v4 b) {
    v4 out = { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
    return out;
}

inline v4 Mul(v4 a, v4 b) {
    v4 out = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
    return out;
}

inline v4 Scale(v4 a, f32 s) {
    v4 out = { a.x * s, a.y * s, a.z * s, a.w * s };
    return out;
}

inline f32 Dot(v4 a, v4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 LenSq(v4 v) {
    return Dot(v, v);
}

inline f32 Len(v4 v) {
    return Sqrt(Dot(v, v));
}

inline f32 DistSq(v4 a, v4 b) {
    return LenSq(Sub(b, a));
}

inline f32 Dist(v4 a, v4 b) {
    return Sqrt(DistSq(a, b));
}

inline v4 Norm(v4 v) {
    return Scale(v, RSqrt(Dot(v, v)));
}

inline v4 Min(v4 a, v4 b) {
    v4 out = {
        (a.x < b.x? a.x : b.x),
        (a.y < b.y? a.y : b.y),
        (a.z < b.z? a.z : b.z),
        (a.w < b.w? a.w : b.w)
    };
    return out;
}

inline v4 Max(v4 a, v4 b) {
    v4 out = {
        (a.x > b.x? a.x : b.x),
        (a.y > b.y? a.y : b.y),
        (a.z > b.z? a.z : b.z),
        (a.w > b.w? a.w : b.w)
    };
    return out;
}

inline v4 Lerp(v4 a, v4 b, f32 t) {
    v4 out = {
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w)
    };
    return out;
}

inline v4 Cross(v4 a, v4 b) {
    v4 out = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        1.0f
    };
    return out;
}

inline v4 Spline(f32 f, v4 a, v4 b, v4 c, v4 d) {
	f32 i = 1.0f - f;

    v4 out = {
	    ((d.x * f + c.x * i) * f + (c.x * f + b.x * i) * i) * f + ((c.x * f + b.x * i) * f + (b.x * f + a.x * i) * i) * i,
	    ((d.y * f + c.y * i) * f + (c.y * f + b.y * i) * i) * f + ((c.y * f + b.y * i) * f + (b.y * f + a.y * i) * i) * i,
	    ((d.z * f + c.z * i) * f + (c.z * f + b.z * i) * i) * f + ((c.z * f + b.z * i) * f + (b.z * f + a.z * i) * i) * i,
	    ((d.w * f + c.w * i) * f + (c.w * f + b.w * i) * i) * f + ((c.w * f + b.w * i) * f + (b.w * f + a.w * i) * i) * i
    };

    return out;
}

// m2:
inline m2 Mul(m2 a, m2 b) {
    m2 out = {
        a.array[0] * b.array[0] + a.array[2] * b.array[1],
        a.array[1] * b.array[0] + a.array[3] * b.array[1],
        a.array[0] * b.array[2] + a.array[2] * b.array[3],
        a.array[1] * b.array[2] + a.array[3] * b.array[3]
    };
    return out;
}

inline v2 Mul(m2 R, v2 v) {
    v2 out = {
        R.array[0] * v.x + R.array[2] * v.y,
        R.array[1] * v.x + R.array[3] * v.y
    };
    return out;
}

inline m2 m2_Identity() {
    m2 out = { 1.0f, 0.0f, 0.0f, 1.0f };
    return out;
}

inline m2 m2_Rotate(f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    m2 out = { c, s, -s, c };

    return out;
}

inline m2 m2_Scale(f32 sx, f32 sy) {
    m2 out = { sx, 0.0f, 0.0f, sy };
    return out;
}

// m3:
inline m3 Mul(m3 a, m3 b) {
    m3 out = {
        a.array[0] * b.array[0] + a.array[3] * b.array[1]  + a.array[6] * b.array[2],
        a.array[1] * b.array[0] + a.array[4] * b.array[1]  + a.array[7] * b.array[2],
        a.array[2] * b.array[0] + a.array[5] * b.array[1]  + a.array[8] * b.array[2],

        a.array[0] * b.array[3] + a.array[3] * b.array[4]  + a.array[6] * b.array[5],
        a.array[1] * b.array[3] + a.array[4] * b.array[4]  + a.array[7] * b.array[5],
        a.array[2] * b.array[3] + a.array[5] * b.array[4]  + a.array[8] * b.array[5],

        a.array[0] * b.array[6] + a.array[3] * b.array[7]  + a.array[6] * b.array[8],
        a.array[1] * b.array[6] + a.array[4] * b.array[7]  + a.array[7] * b.array[8],
        a.array[2] * b.array[6] + a.array[5] * b.array[7]  + a.array[8] * b.array[8]
    };
    return out;
}

inline v3 Mul(m3 M, v3 v) {
    v3 out = {
        M.array[0] * v.array[0] + M.array[3] * v.array[1] + M.array[6] * v.array[2],
        M.array[1] * v.array[0] + M.array[4] * v.array[1] + M.array[7] * v.array[2],
        M.array[2] * v.array[0] + M.array[5] * v.array[1] + M.array[8] * v.array[2]
    };
    return out;
}

inline m3 m3_Identity(void) {
    m3 out = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    return out;
}

inline m3 m3_Rotate(v3 axis, f32 angle) {
    f32 c = Cos(angle);
    f32 s = Sin(angle);

    f32 k = 1.0f - c;

    v3 sa   = { s * axis.x, s * axis.y, s * axis.z };
    v3 omca = { k * axis.x, k * axis.y, k * axis.z };

    m3 out = {
        omca.x * axis.x + c,
        omca.x * axis.y - sa.z,
        omca.x * axis.z + sa.y,

        omca.y * axis.x + sa.z,
        omca.y * axis.y + c,
        omca.y * axis.z - sa.x,

        omca.z * axis.x - sa.y,
        omca.z * axis.y + sa.x,
        omca.z * axis.z + c
    };
    return out;
}

inline m3 m3_Euler(f32 yaw, f32 pitch, f32 roll) {
    f32 cy = Cos(yaw);
    f32 sy = Sin(yaw);
    f32 cp = Cos(pitch);
    f32 sp = Sin(pitch);
    f32 cr = Cos(roll);
    f32 sr = Sin(roll);

    m3 out = {
        cy * cp,
        sy * cp,
        -sp,

        cy * sp * sr - sy * cr,
        sy * sp * sr + cy * cr,
        cp * sr,

        cy * sp * cr + sy * sr,
        sy * sp * cr - cy * sr,
        cp * cr,
    };

    return out;
}

inline m3 m3_FromQuat(Quat quat) {
    f32 a = quat.w;
	f32 b = quat.x;
	f32 c = quat.y;
	f32 d = quat.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    m3 out = {
        a2 + b2 - c2 - d2,
        2.0f * (b * c + a * d),
        2.0f * (b * d - a * c),

        2.0f * (b * c - a * d),
        a2 - b2 + c2 - d2,
        2.0f * (c * d + a * b),

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2
    };
    return out;
}

// m4:

inline m4 Mul(m4 a, m4 b) {
    m4 out = {
        a.array[0] * b.array[0]  + a.array[4] * b.array[1]  + a.array[8]  * b.array[2]  + a.array[12] * b.array[3],
        a.array[1] * b.array[0]  + a.array[5] * b.array[1]  + a.array[9]  * b.array[2]  + a.array[13] * b.array[3],
        a.array[2] * b.array[0]  + a.array[6] * b.array[1]  + a.array[10] * b.array[2]  + a.array[14] * b.array[3],
        a.array[3] * b.array[0]  + a.array[7] * b.array[1]  + a.array[11] * b.array[2]  + a.array[15] * b.array[3],

        a.array[0] * b.array[4]  + a.array[4] * b.array[5]  + a.array[8]  * b.array[6]  + a.array[12] * b.array[7],
        a.array[1] * b.array[4]  + a.array[5] * b.array[5]  + a.array[9]  * b.array[6]  + a.array[13] * b.array[7],
        a.array[2] * b.array[4]  + a.array[6] * b.array[5]  + a.array[10] * b.array[6]  + a.array[14] * b.array[7],
        a.array[3] * b.array[4]  + a.array[7] * b.array[5]  + a.array[11] * b.array[6]  + a.array[15] * b.array[7],

        a.array[0] * b.array[8]  + a.array[4] * b.array[9]  + a.array[8]  * b.array[10] + a.array[12] * b.array[11],
        a.array[1] * b.array[8]  + a.array[5] * b.array[9]  + a.array[9]  * b.array[10] + a.array[13] * b.array[11],
        a.array[2] * b.array[8]  + a.array[6] * b.array[9]  + a.array[10] * b.array[10] + a.array[14] * b.array[11],
        a.array[3] * b.array[8]  + a.array[7] * b.array[9]  + a.array[11] * b.array[10] + a.array[15] * b.array[11],

        a.array[0] * b.array[12] + a.array[4] * b.array[13] + a.array[8]  * b.array[14] + a.array[12] * b.array[15],
        a.array[1] * b.array[12] + a.array[5] * b.array[13] + a.array[9]  * b.array[14] + a.array[13] * b.array[15],
        a.array[2] * b.array[12] + a.array[6] * b.array[13] + a.array[10] * b.array[14] + a.array[14] * b.array[15],
        a.array[3] * b.array[12] + a.array[7] * b.array[13] + a.array[11] * b.array[14] + a.array[15] * b.array[15]
    };
    return out;
}

inline v4 Mul(m4 M, v4 v) {
    v4 out = {
        M.array[0] * v.array[0] + M.array[4] * v.array[1] + M.array[8]  * v.array[2] + M.array[12] * v.array[3],
        M.array[1] * v.array[0] + M.array[5] * v.array[1] + M.array[9]  * v.array[2] + M.array[13] * v.array[3],
        M.array[2] * v.array[0] + M.array[6] * v.array[1] + M.array[10] * v.array[2] + M.array[14] * v.array[3],
        M.array[3] * v.array[0] + M.array[7] * v.array[1] + M.array[11] * v.array[2] + M.array[15] * v.array[3]
    };
    return out;
}

inline m4 m4_Identity(void) {
    m4 out = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    return out;
}

inline m4 m4_Transpose(m4 N) {
    m4 out = {
        N.array[0],   N.array[1],   N.array[2],   N.array[3],
	    N.array[4],   N.array[5],   N.array[6],   N.array[7],
	    N.array[8],   N.array[9],   N.array[10],  N.array[11],
	    N.array[12],  N.array[13],  N.array[14],  N.array[15]
    };
    return out;
}

inline m4 m4_Translate(f32 x, f32 y, f32 z) {
    m4 out = {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        x,      y,      z,      1.0f
    };
    return out;
}

inline m4 m4_Scale(f32 x, f32 y, f32 z) {
    m4 out = {
        x,      0.0f,   0.0f,   0.0f,
        0.0f,   y,      0.0f,   0.0f,
        0.0f,   0.0f,   z,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    return out;
}

inline m4 m4_RotateX(f32 angle) {
    f32 s = Sin(angle);
	f32 c = Cos(angle);

    m4 out = {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,      c,      s,   0.0f,
        0.0f,     -s,      c,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    return out;
}

inline m4 m4_RotateY(f32 angle) {
    f32 s = Sin(angle);
	f32 c = Cos(angle);

    m4 out = {
           c,   0.0f,      s,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
          -s,   0.0f,      c,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    return out;
}

inline m4 m4_RotateZ(f32 angle) {
	f32 s = Sin(angle);
	f32 c = Cos(angle);

    m4 out = {
           c,      s,   0.0f,   0.0f,
          -s,      c,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    return out;
}

inline m4 m4_Rotate(v3 axis, f32 angle) {
    f32 sa[3];
    f32 omca[3];

    f32 cosv      = Cos(angle);
    f32 sinv      = Sin(angle);
    f32 inv_cosv  = 1.0f - cosv;

    sa[0] = axis.x * sinv;
    sa[1] = axis.y * sinv;
    sa[2] = axis.z * sinv;

    omca[0] = axis.x * inv_cosv;
    omca[1] = axis.y * inv_cosv;
    omca[2] = axis.z * inv_cosv;

    m4 out = {
        omca[0] * axis.x + cosv,   omca[0] * axis.y - sa[0],  omca[0] * axis.z + sa[1],  0.0f,
        omca[1] * axis.x + sa[2],  omca[1] * axis.y + cosv,   omca[1] * axis.z - sa[0],  0.0f,
        omca[2] * axis.x - sa[1],  omca[2] * axis.y + sa[0],  omca[2] * axis.z + cosv,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };

    return out;
}

inline m4 m4_Frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    m4 out = {
        2.0f * n / (r - l),
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        2.0f * n / (t - b),
        0.0f,
        0.0f,

        (r + l) / (r - l),
        (t + b) / (t - b),
        -(f + n) / (f - n),
        -1.0f,

        0.0f,
        0.0f,
        -2.0f * (f * n) / (f - n),
        0.0f
    };
    return out;
}

inline m4 m4_Ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    m4 out = {
        2.0f / (r - l),
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        2.0f / (t - b),
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -2.0f / (f - n),
        0.f,

        -(r + l) / (r - l),
        -(t + b) / (t - b),
        -(f + n) / (f - n),
        1.0f
    };
    return out;
}

inline m4 m4_Perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    const f32 a = 1.0f / tanf(y_fov / 2.0f);

    m4 out = {
        a / aspect,
        0.0f,
        0.0f,
        0.0f,

        0.0f,
        a,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        -((f + n) / (f - n)),
        -1.0f,

        0.0f,
        0.0f,
        -((2.f * f * n) / (f - n)),
        0.0f
    };
    return out;
}

inline m4 m4_LookAt(v3 eye, v3 center, v3 up) {
    v3 f = Norm(Sub(center, eye));
    v3 s = Norm(Cross(f, up));
	v3 t = Cross(s, f);

    m4 M;

	M.array[0]  =  s.x;
	M.array[1]  =  t.x;
	M.array[2]  = -f.x;
	M.array[3]  =   0.0f;

	M.array[4]  =  s.y;
	M.array[5]  =  t.y;
	M.array[6]  = -f.y;
	M.array[7]  =   0.0f;

	M.array[8]  =  s.z;
	M.array[9]  =  t.z;
	M.array[10] = -f.z;
	M.array[11] =   0.0f;

    M.array[12] = -(M.array[0] * eye.x + M.array[4] * eye.y + M.array[8]  * eye.z);
    M.array[13] = -(M.array[1] * eye.x + M.array[5] * eye.y + M.array[9]  * eye.z);
    M.array[14] = -(M.array[2] * eye.x + M.array[6] * eye.y + M.array[10] * eye.z);
    M.array[15] = -(M.array[3] * eye.x + M.array[7] * eye.y + M.array[11] * eye.z - 1.0f);

    return M;
}

inline m4 m4_FromQuat(Quat q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    m4 out = {
        a2 + b2 - c2 - d2,
        2.0f * (b * c + a * d),
        2.0f * (b * d - a * c),
        0.f,

        2.0f * (b * c - a * d),
        a2 - b2 + c2 - d2,
        2.0f * (c * d + a * b),
        0.f,

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f
    };
    return out;
}

// Quat:
inline Quat QuatIdentity(void) {
    Quat out = { 0.0f, 0.0f, 0.0f, 1.0f };
    return out;
}

inline Quat QuatMake(f32 x, f32 y, f32 z, f32 angle) {
    f32 inv_len = RSqrt((x * x) + (y * y) + (z * z));
    f32 s       = inv_len * Sin(angle / 2.0f);

    Quat out = {
        x * s,
        y * s,
        z * s,
        Cos(angle / 2.0f)
    };

    return out;
}

inline Quat QuatAdd(Quat a, Quat b) {
    Quat out = { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
    return out;
}

inline Quat QuatConj(Quat q) {
    Quat out = { -q.x, -q.y, -q.z, q.w };
    return out;
}

inline Quat QuatMul(Quat a, Quat b) {
    Quat out = {
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    };
    return out;
}

// --------------------- v2 ------------------------- // 

inline v2 V2(v3 a)  { return { a.x, a.y }; }

inline v2 V2(v2i a) { return { f32(a.x), f32(a.y) }; }

inline v2 operator-(v2 a) { return Neg(a); }

inline v2 operator+(v2 a, v2 b)  { return Add(a, b); }
inline v2 operator-(v2 a, v2 b)  { return Sub(a, b); }
inline v2 operator*(v2 a, v2 b)  { return Mul(a, b); }
inline v2 operator*(v2 a, f32 s) { return Scale(a, s); }
inline v2 operator*(f32 s, v2 a) { return Scale(a, s); }
inline v2 operator/(v2 a, f32 s) { return Scale(a, 1.0f / s); }

inline v2 operator+=(v2& a, v2 b)  { a = a + b; return a; }
inline v2 operator-=(v2& a, v2 b)  { a = a - b; return a; }
inline v2 operator*=(v2& a, v2 b)  { a = a * b; return a; }
inline v2 operator*=(v2& a, f32 s) { a = a * s; return a; }
inline v2 operator/=(v2& a, f32 s) { a = a / s; return a; }

// --------------------- v3 ------------------------- // 

inline v3 V3(f32 x, f32 y, f32 z)   { return { x, y, z }; }
inline v3 V3(v2 a)                  { return { a.x, a.y, 0.0f }; }
inline v3 V3(v2 a, f32 z)           { return { a.x, a.y, z }; }
inline v3 V3(f32 x, v2 a)           { return { x, a.x, a.y }; }

inline v3 V3(v3i a)                  { return { f32(a.x), f32(a.y), f32(a.z) }; }

inline v3 operator-(v3 a) { return Neg(a); }

inline v3 operator+(v3 a, v3 b)  { return Add(a, b); }
inline v3 operator-(v3 a, v3 b)  { return Sub(a, b); }
inline v3 operator*(v3 a, v3 b)  { return Mul(a, b); }
inline v3 operator*(v3 a, f32 s) { return Scale(a, s); }
inline v3 operator*(f32 s, v3 a) { return Scale(a, s); }
inline v3 operator/(v3 a, f32 s) { return Scale(a, 1.0f / s); }

inline v3 operator+=(v3& a, v3 b)  { a = a + b; return a; }
inline v3 operator-=(v3& a, v3 b)  { a = a - b; return a; }
inline v3 operator*=(v3& a, v3 b)  { a = a * b; return a; }
inline v3 operator*=(v3& a, f32 s) { a = a * s; return a; }
inline v3 operator/=(v3& a, f32 s) { a = a / s; return a; }

inline v3 Clamp     (v3 a, f32 min, f32 max) { return { Clamp(a.x, min, max), Clamp(a.y, min, max), Clamp(a.z, min, max) }; }
inline v3 ClampMin  (v3 a, f32 min)          { return { ClampMin(a.x, min), ClampMin(a.y, min), ClampMin(a.z, min) }; }
inline v3 ClampMax  (v3 a, f32 max)          { return { ClampMax(a.x, max), ClampMax(a.y, max), ClampMax(a.z, max) }; }

// --------------------- v4 ------------------------- // 

inline v4 operator-(v4 a) { return Neg(a); }

inline v4 V4(v2 a, v2 b)            { return V4(a.x, a.y, b.x, b.y); }
inline v4 V4(v2 a, f32 z, f32 w)    { return V4(a.x, a.y, z, w); }
inline v4 V4(v3 a, f32 w)           { return V4(a.x, a.y, a.z, w); }
inline v4 V4(f32 x, v3 a)           { return V4(x, a.x, a.y, a.z); }

inline v4 operator+(v4 a, v4 b)  { return Add(a, b); }
inline v4 operator-(v4 a, v4 b)  { return Sub(a, b); }
inline v4 operator*(v4 a, v4 b)  { return Mul(a, b); }
inline v4 operator*(v4 a, f32 s) { return Scale(a, s); }
inline v4 operator*(f32 s, v4 a) { return Scale(a, s); }
inline v4 operator/(v4 a, f32 s) { return Scale(a, 1.0f / s); }

inline v4 operator+=(v4& a, v4 b)  { a = a + b; return a; }
inline v4 operator-=(v4& a, v4 b)  { a = a - b; return a; }
inline v4 operator*=(v4& a, v4 b)  { a = a * b; return a; }
inline v4 operator*=(v4& a, f32 s) { a = a * s; return a; }
inline v4 operator/=(v4& a, f32 s) { a = a / s; return a; }

// --------------------- v2i ------------------------- // 

inline v2i V2i(i32 x, i32 y)    { return { x, y }; }
inline v2i V2i(v2 a)            { return { i32(a.x), i32(a.y) }; }

inline v2i operator+(v2i a, v2i b)  { return { a.x + b.x, a.y + b.y }; }
inline v2i operator-(v2i a, v2i b)  { return { a.x - b.x, a.y - b.y }; }

inline bool operator==(v2i a, v2i b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(v2i a, v2i b) { return a.x != b.x || a.y != b.y; }

// --------------------- v3i ------------------------- // 

inline v3i V3i(i32 x, i32 y, i32 z) { return { x, y, z }; }
inline v3i V3i(v3 a)                { return { i32(a.x), i32(a.y), i32(a.z) }; }

inline v3i operator+(v3i a, v3i b)  { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
inline v3i operator-(v3i a, v3i b)  { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

inline i32 DistSq(v3i a, v3i b) {
    v3i d = a - b;

    return d.x * d.x + d.y * d.y + d.z * d.z;
}

// --------------------- v4i ------------------------- // 

inline v4i V4i(i32 x, i32 y, i32 z, i32 w) { return { x, y, z, w }; }

inline v4i operator+(v4i a, v4i b)  { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
inline v4i operator-(v4i a, v4i b)  { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

// --------------------- m2 ------------------------- // 

inline v2 operator*(m2 a, v2 u) { return Mul(a, u); }
inline m2 operator*(m2 a, m2 b) { return Mul(a, b); }

// --------------------- m3 ------------------------- // 

inline v3 operator*(m3 a, v3 u) { return Mul(a, u); }
inline m3 operator*(m3 a, m3 b) { return Mul(a, b); }

// --------------------- m4 ------------------------- // 

inline v4 operator*(m4 a, v4 u) { return Mul(a, u); }
inline m4 operator*(m4 a, m4 b) { return Mul(a, b); }

// -------------------- Quat ------------------------- // 

inline Quat operator*(Quat a, Quat b) { return QuatMul(a, b); }

#endif // __ATS_H__

// ========================================================================================================== //
// ============================================== IMPLEMENTATION ============================================ //
// ========================================================================================================== //

#ifdef ATS_IMPLEMENTATION

#ifndef ATS_IMPLEMENTATION_ONCE
#define ATS_IMPLEMENTATION_ONCE

#ifdef ATS_MODERN_OPENGL
#include "glad/glad.c"
#endif

// ==================================== FILES ==================================== //

static size_t FileGetSize(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

extern char* FileReadStr(const char* file_name) {
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0) {
        size_t size = FileGetSize(fp);

        buffer = (char*)malloc(size + 1);

        if (buffer) {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0) {
                free(buffer);
                buffer = 0;
            }
        }

        fclose(fp);
    }

    return buffer;
}

extern int FileWriteStr(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);
        fclose(fp);
        return n == size;
    }

    return 0;
}

extern int FileAppendStr(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n    = fwrite(buffer, 1, size, fp);

        fclose(fp);

        return n == size;
    }

    return 0;
}

extern int FileReadBin(const char* file_name, void* buffer, size_t size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return 0;
} 

extern int FileWriteBin(const char* file_name, const void* buffer, size_t size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }
    return 0;
}

// ================================================ PLATFORM ============================================= //

#ifdef ATS_PLATFORM_GLFW

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

Platform platform;

static struct {
    GLFWwindow*     window;
    GLFWmonitor*    monitor;
} platform_internal;

static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)scancode;
    (void)mods;

    switch (action) {
        case GLFW_PRESS:
            platform.keyboard.key           = key;
            platform.keyboard.is_down       = 1;
            platform.keyboard.is_pressed    = 1;
            platform.keyboard.is_repeat     = 1;
            platform.keyboard.state[key]    = 1;
            platform.keyboard.pressed[key]  = 1;
            platform.keyboard.repeat[key]   = 1;
            break;
        case GLFW_REPEAT:
            platform.keyboard.is_repeat     = 1;
            platform.keyboard.repeat[key]   = 1;
            break;
        case GLFW_RELEASE:
            platform.keyboard.is_down       = 0;
            platform.keyboard.is_released   = 1;
            platform.keyboard.state[key]    = 0;
            platform.keyboard.released[key] = 1;
            break;
    }
}

static void WindowCharCallback(GLFWwindow* window, unsigned int codepoint) {
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window;
    (void)mods;

    switch (action) {
        case GLFW_PRESS: {
            platform.mouse.is_down          = 1;
            platform.mouse.is_pressed       = 1;
            platform.mouse.state[button]    = 1;
            platform.mouse.pressed[button]  = 1;
            break;
        }
        case GLFW_RELEASE: {
            platform.mouse.is_down          = 0;
            platform.mouse.is_released      = 1;
            platform.mouse.state[button]    = 0;
            platform.mouse.released[button] = 1;
            break;
        }
    }
}

static void WindowScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window;

    platform.mouse.scroll.x = xoffset;
    platform.mouse.scroll.y = yoffset;
}

static void WindowJoystickCallback(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

extern void PlatformInit(const char* title, int width, int height, int samples) {
    glfwInit();

    platform_internal.monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#ifdef ATS_MODERN_OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.width  = width;
    platform.height = height;
    //
    platform_internal.window    = glfwCreateWindow(width, height, title, NULL, NULL);
    platform.native             = glfwGetWin32Window(platform_internal.window);

    glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform_internal.window);

#ifdef ATS_MODERN_OPENGL 
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

    glfwSetKeyCallback(platform_internal.window, WindowKeyCallback);
    glfwSetCharCallback(platform_internal.window, WindowCharCallback);
    glfwSetMouseButtonCallback(platform_internal.window, WindowMouseButtonCallback);
    glfwSetScrollCallback(platform_internal.window, WindowScrollCallback);
    glfwSetJoystickCallback(WindowJoystickCallback);

    // init mouse:
    {
        f64 x = 0.0;
        f64 y = 0.0;

        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i)) {
            platform.gamepad[i].active = 1;
        }
    }

    platform.vsync                      = true;
    platform._vsync_state_last_update   = true;

    glfwSetTime(0.0);
}

extern void PlatformUpdate(void) {
    if (glfwWindowShouldClose(platform_internal.window))
        platform.close = 1;

    if (platform.close)
        glfwSetWindowShouldClose(platform_internal.window, 1);

    platform.mouse.is_pressed       = 0;
    platform.mouse.is_released      = 0;
    platform.keyboard.is_pressed    = 0;
    platform.keyboard.is_repeat     = 0;
    platform.keyboard.is_released   = 0;
    platform.keyboard.is_ascii      = 0;

    // update mouse:
    {
        double x, y;
        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.delta.x = x - platform.mouse.pos.x;
        platform.mouse.delta.y = y - platform.mouse.pos.y;

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        switch (platform.mouse.mode) {
            case MOUSE_NORMAL:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case MOUSE_HIDDEN:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            case MOUSE_DISABLED:
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
        }
    }

    // TODO: make it possible to connect and use GLFW_JOYSTICK_LAST amount of controllers!
    // update gamepad:
    {
        GLFWgamepadstate state;

        for (int i = 0; i < 1; ++i) {
            if (platform.gamepad[i].active) {
                GamepadButtons old = platform.gamepad[i].state;

                platform.gamepad[i].state.data = 0;
                platform.gamepad[i].pressed.data = 0;
                platform.gamepad[i].released.data = 0;

                glfwGetGamepadState(i, &state);

                platform.gamepad[i].LS.x =  state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].LS.y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].RS.x =  state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                platform.gamepad[i].RS.y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
                platform.gamepad[i].LT   =  0.5f * (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f);
                platform.gamepad[i].RT   =  0.5f * (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f);

                // ps4 button mapping:
                if (state.buttons[GLFW_GAMEPAD_BUTTON_X]) platform.gamepad[i].state.button.X = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_A]) platform.gamepad[i].state.button.A = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_B]) platform.gamepad[i].state.button.B = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_Y]) platform.gamepad[i].state.button.Y = 1;

                if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])  platform.gamepad[i].state.button.LB = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER]) platform.gamepad[i].state.button.RB = 1;
                if (platform.gamepad[i].LT > 0.0f)                   platform.gamepad[i].state.button.LT = 1;
                if (platform.gamepad[i].RT > 0.0f)                   platform.gamepad[i].state.button.RT = 1;
                //
                if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK])        platform.gamepad[i].state.button.select = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_START])       platform.gamepad[i].state.button.start  = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])  platform.gamepad[i].state.button.LS     = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]) platform.gamepad[i].state.button.RS     = 1;
                //
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])     platform.gamepad[i].state.button.UP    = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])  platform.gamepad[i].state.button.RIGHT = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])   platform.gamepad[i].state.button.DOWN  = 1;
                if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])   platform.gamepad[i].state.button.LEFT  = 1;

                platform.gamepad[i].pressed.data  =  platform.gamepad[i].state.data & ~old.data;
                platform.gamepad[i].released.data = ~platform.gamepad[i].state.data & old.data;
            }
        }
    }

    if (platform.fullscreen != platform._fullscreen_state_last_update) {
        const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

        if (platform.fullscreen) {
            glfwSetWindowMonitor(platform_internal.window, platform_internal.monitor, 0, 0, mode->width, mode->height, 0);
        } else {
            glfwSetWindowMonitor(platform_internal.window, NULL, 64, 64, mode->width - 256, mode->height - 256, 0);
        }

        platform._fullscreen_state_last_update = platform.fullscreen;
    }

    if (platform.vsync != platform._vsync_state_last_update) {
        if (platform.vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        
        platform._vsync_state_last_update = platform.vsync;
    }

    glfwGetWindowSize(platform_internal.window, &platform.width, &platform.height);
    platform.aspect_ratio = (f32)platform.width / (f32)platform.height;

    glViewport(0, 0, platform.width, platform.height);

    platform.time.delta    = glfwGetTime() - platform.time.total;
    platform.time.total += platform.time.delta;

    memset(platform.keyboard.pressed,  0, sizeof (platform.keyboard.pressed));
    memset(platform.keyboard.repeat,   0, sizeof (platform.keyboard.repeat));
    memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

    memset(platform.mouse.pressed,  0, sizeof (platform.mouse.pressed));
    memset(platform.mouse.released, 0, sizeof (platform.mouse.released));

    glfwPollEvents();
    glfwSwapBuffers(platform_internal.window);
}

// =================================================== TIMER STUFF =================================================== //

extern f32 TimerGetCurrent(void) {
    return glfwGetTime();
}

#endif // ATS_PLATFORM_GLFW

// ===================================================== SHADERS ===================================================== //

#ifdef ATS_MODERN_OPENGL

static u32 ShaderCompile(const char* source, unsigned int type) {
    int success;

    char info_log[512];

    u32 shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader;
}

static u32 ShaderLinkProgramVF(u32 vertex_shader, u32 fragment_shader) {
    int     success;
    char    info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

static unsigned int ShaderLinkProgramVFG(u32 vertex_shader, u32 fragment_shader, u32 geometry_shader) {
    int     success;
    char    info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, geometry_shader);

    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

static u32 ShaderLinkProgramV(u32 vertex_shader) {
    int success;
    char info_log[512];

    u32 shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);

    const char* varyings[] = { "pos1", "vel1" };
    glTransformFeedbackVaryings(shader_program, 2, varyings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        puts(info_log);

        exit(EXIT_FAILURE);
    }

    return shader_program;
}

extern u32 ShaderLoadFromMemory(const char *vs, const char *fs, const char *gs) {
    unsigned int shader_program = 0;

    if (vs != NULL && fs != NULL && gs != NULL) {
        u32 vertex     = ShaderCompile(vs, GL_VERTEX_SHADER);
        u32 fragment   = ShaderCompile(fs, GL_FRAGMENT_SHADER);
        u32 geometry   = ShaderCompile(gs, GL_GEOMETRY_SHADER);

        shader_program  = ShaderLinkProgramVFG(vertex, fragment, geometry);

        glUseProgram(shader_program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs != NULL && gs == NULL) {
        u32 vertex     = ShaderCompile(vs, GL_VERTEX_SHADER);
        u32 fragment   = ShaderCompile(fs, GL_FRAGMENT_SHADER);

        shader_program  = ShaderLinkProgramVF(vertex, fragment);

        glUseProgram(shader_program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    } else if (vs != NULL && fs == NULL) {
        u32 vertex = ShaderCompile(vs, GL_VERTEX_SHADER);

        shader_program = ShaderLinkProgramV(vertex);

        glUseProgram(shader_program);
        glDeleteShader(vertex);
    }

    return shader_program;
}

extern void ShaderUse(u32 shader) {
    glUseProgram(shader);
}

extern u32 ShaderGetLocation(u32 shader, const char *var_name) {
    return glGetUniformLocation(shader, var_name);
}

extern void ShaderSeti(u32 shader, const char *loc, int n) {
    glUniform1i(glGetUniformLocation(shader, loc), n);
}

extern void ShaderSetf(u32 shader, const char *loc, f32 n) {
    glUniform1f(glGetUniformLocation(shader, loc), n);
}

extern void ShaderSet2f(u32 shader, const char *loc, f32 a, f32 b) {
    glUniform2f(glGetUniformLocation(shader, loc), a, b);
}

extern void ShaderSet3f(u32 shader, const char *loc, f32 a, f32 b, f32 c) {
    glUniform3f(glGetUniformLocation(shader, loc), a, b, c);
}

extern void ShaderSet4f(u32 shader, const char *loc, f32 a, f32 b, f32 c, f32 d) {
    glUniform4f(glGetUniformLocation(shader, loc), a, b, c, d);
}

extern void ShaderSet2v(u32 shader, const char *loc, const f32 *u) {
    glUniform2fv(glGetUniformLocation(shader, loc), 1, u);
}

extern void ShaderSet3v(u32 shader, const char *loc, const f32 *u) {
    glUniform3fv(glGetUniformLocation(shader, loc), 1, u);
}

extern void ShaderSet4v(u32 shader, const char *loc, const f32 *u) {
    glUniform4fv(glGetUniformLocation(shader, loc), 1, u);
}

extern void ShaderSetMat4(u32 shader, const char *loc, const f32 *m) {
    glUniformMatrix4fv(glGetUniformLocation(shader, loc), 1, GL_FALSE, m);
}

#endif // ATS_MODERN_OPENGL

// =================================================== BITMAP FONT =================================================== //

#define BITMAP_COUNT    (256)

static const u64 bitascii[BITMAP_COUNT] = {
    0x0000000000000000,
    0x7e8199bd81a5817e,
    0x7effe7c3ffdbff7e,
    0x00081c3e7f7f7f36,
    0x00081c3e7f3e1c08,
    0x1c086b7f7f1c3e1c,
    0x1c082a7f7f3e1c08,
    0x0000183c3c180000,
    0xffffe7c3c3e7ffff,
    0x003c664242663c00,
    0xffc399bdbd99c3ff,
    0x1e333333bef0e0f0,
    0x187e183c6666663c,
    0x070f0e0c0c7c6c7c,
    0x03377666667e667e,
    0x0018dbff3cffdb18,
    0x00061e7e7e1e0600,
    0x0060787e7e786000,
    0x183c7e18187e3c18,
    0x0036003636363636,
    0x00d8d8d8dedbdbfe,
    0x3e613c66663c867c,
    0x007e7e0000000000,
    0xff183c7e187e3c18,
    0x001818187e3c1800,
    0x00183c7e18181800,
    0x0010307e7e301000,
    0x00080c7e7e0c0800,
    0x00007e0606000000,
    0x002466ffff662400,
    0x007e7e3c3c181800,
    0x0018183c3c7e7e00,
    0x0000000000000000,
    0x00180018183c3c18,
    0x0000000000143636,
    0x0036367f367f3636,
    0x00183e603c067c18,
    0x0063660c18336300,
    0x006e333b6e1c361c,
    0x00000000000c1818,
    0x00180c0606060c18,
    0x00060c1818180c06,
    0x0000361c7f1c3600,
    0x000018187e181800,
    0x0c18180000000000,
    0x000000007e000000,
    0x0018180000000000,
    0x0003070e1c387060,
    0x003c666e7e76663c,
    0x007e181818181e18,
    0x007e660c3860663c,
    0x003c66603860663c,
    0x0030307f33363c38,
    0x003c6660603e067e,
    0x003c66663e060c38,
    0x000c0c183060667e,
    0x003c66663c66663c,
    0x001c30607c66663c,
    0x0018180018180000,
    0x0c18180018180000,
    0x0030180c060c1830,
    0x0000007e007e0000,
    0x00060c1830180c06,
    0x001800183060663c,
    0x003c06767676663c,
    0x0066667e66663c18,
    0x003f66663e66663f,
    0x003c66030303663c,
    0x003f36666666363f,
    0x007f46161e16467f,
    0x000f06161e16467f,
    0x007c66730303663c,
    0x006666667e666666,
    0x003c18181818183c,
    0x001e333330303078,
    0x006766361e366667,
    0x007f66460606060f,
    0x006363636b7f7763,
    0x006363737b6f6763,
    0x001c36636363361c,
    0x000f06063e66663f,
    0x00703c766666663c,
    0x0067361e3e66663f,
    0x003c6670380e663c,
    0x003c181818185a7e,
    0x007e666666666666,
    0x00183c6666666666,
    0x0063777f6b636363,
    0x006363361c366363,
    0x003c18183c666666,
    0x007f63460c19337f,
    0x003c0c0c0c0c0c3c,
    0x006070381c0e0703,
    0x003c30303030303c,
    0x0000000063361c08,
    0x7e00000000000000,
    0x0000000000301818,
    0x006e333e301e0000,
    0x003d6666663e0607,
    0x003c6606663c0000,
    0x006e33333e303038,
    0x003c067e663c0000,
    0x001e0c0c1e0c6c38,
    0x1f303e33336e0000,
    0x006766666e360607,
    0x003c1818181c0018,
    0x1c363030303c0030,
    0x0067361e36660607,
    0x003c18181818181c,
    0x0063636b7f370000,
    0x00666666663e0000,
    0x001e3333331e0000,
    0x0f063e66663b0000,
    0x78303e33336e0000,
    0x001e0c6c6c360000,
    0x003e603c067c0000,
    0x00182c0c0c3e0c08,
    0x006e333333330000,
    0x00183c6666660000,
    0x00367f6b63630000,
    0x0063361c36630000,
    0x3e607c6666660000,
    0x007e4c18327e0000,
    0x00380c0c060c0c38,
    0x0018181800181818,
    0x000e18183018180e,
    0x0000000000003b6e,
    0x007e66663c180000,
    0x060c1e330303331e,
    0x007e333333003300,
    0x003c067e663c1830,
    0x00fc667c603cc37e,
    0x007e333e301e0033,
    0x007e333e301e0c06,
    0x00fc667c603c663c,
    0x0c183c6606663c00,
    0x003c067e663cc37e,
    0x003c067e663c0066,
    0x003c067e663c180c,
    0x003c1818181c0066,
    0x003c1818181c633e,
    0x003c1818181c180c,
    0x00667e66663c1866,
    0x00667e663c182418,
    0x003f061e063f0c18,
    0x007e337e307e0000,
    0x007333337f33367c,
    0x003c66663c00663c,
    0x003c66663c006600,
    0x003c66663c00180c,
    0x007e33333300331e,
    0x007e333333000c06,
    0x3e607e6666006600,
    0x003e6363633e0063,
    0x003c666666660066,
    0x18187e03037e1818,
    0x003f67060f26361c,
    0x18187e187e3c6666,
    0x00721a321e22221e,
    0x000e1b187e18d870,
    0x007e333e301e0c18,
    0x003c1818181c1830,
    0x003c66663c003060,
    0x007e333333001830,
    0x003333331f003b6e,
    0x00333b3f37003b6e,
    0x00007e007c36363c,
    0x00007e003c66663c,
    0x003c66060c180018,
    0x000006067e000000,
    0x000060607e000000,
    0xf03366cc7b3363c3,
    0xc0f3f6ecdb3363c3,
    0x183c3c1818001800,
    0x0000cc663366cc00,
    0x00003366cc663300,
    0x8822882288228822,
    0x55aa55aa55aa55aa,
    0xeebbeebbeebbeebb,
    0x1818181818181818,
    0x1818181f1f181818,
    0x181f1f18181f1f18,
    0x6666666767666666,
    0x6666667f7f000000,
    0x181f1f18181f1f00,
    0x6667676060676766,
    0x6666666666666666,
    0x66676760607f7f00,
    0x007f7f6060676766,
    0x0000007f7f666666,
    0x001f1f18181f1f18,
    0x1818181f1f000000,
    0x000000f8f8181818,
    0x000000ffff181818,
    0x181818ffff000000,
    0x181818f8f8181818,
    0x000000ffff000000,
    0x181818ffff181818,
    0x18f8f81818f8f818,
    0x666666e6e6666666,
    0x00fefe0606e6e666,
    0x66e6e60606fefe00,
    0x00ffff0000e7e766,
    0x66e7e70000ffff00,
    0x66e6e60606e6e666,
    0x00ffff0000ffff00,
    0x66e7e70000e7e766,
    0x00ffff0000ffff18,
    0x000000ffff666666,
    0x18ffff0000ffff00,
    0x666666ffff000000,
    0x000000fefe666666,
    0x00f8f81818f8f818,
    0x18f8f81818f8f800,
    0x666666fefe000000,
    0x666666ffff666666,
    0x18ffff1818ffff18,
    0x0000001f1f181818,
    0x181818f8f8000000,
    0xffffffffffffffff,
    0xffffffff00000000,
    0x0f0f0f0f0f0f0f0f,
    0xf0f0f0f0f0f0f0f0,
    0x00000000ffffffff,
    0x00006e3b3b6e0000,
    0x000c3e663e663c00,
    0x0000060606667e00,
    0x0066363636367f00,
    0x007e460c0c467e00,
    0x001c3636367c0000,
    0x03063e6666660000,
    0x001818183b6e0000,
    0x007e183c663c187e,
    0x001c36363e36361c,
    0x007736366363361c,
    0x003c66663c180c38,
    0x00003e6b6b3e0000,
    0x03063e6b6b3e1830,
    0x00380c063e060c38,
    0x0066666666663c00,
    0x00007e007e007e00,
    0x003f000c0c3f0c0c,
    0x003f00060c180c06,
    0x003f00180c060c18,
    0x0606060636361c00,
    0x001c363630303030,
    0x001818007e001818,
    0x00003b6e003b6e00,
    0x000000001c36361c,
    0x0000001818000000,
    0x0000001800000000,
    0x00181c1612101070,
    0x000000003636361e,
    0x000000003c18301c,
    0x007e7e7e7e7e7e00,
    0x007e424242427e00
};

// ================================================ CUBE ENGINE =================================================== //

#ifdef ATS_CUBE_ENGINE

#define SHADOW_WIDTH    (2048) 
#define SHADOW_HEIGHT   (2048) 

struct Vertex {
    f32   position[4];
    f32   normal[4];
    f32   color[4];
    f32   texture[2];
};

// Layout:  vertex : normal : color : texture
static const Vertex vertex_array_cube[] = {
    // back face
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right         
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    // front face
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f,  0.0f,  1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    // left face
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-right
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-left
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-right
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { -1.0f,  0.0f,  0.0f, 0.0f },  { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-right
    // right face
    { { 1.0f,  1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-left
    { { 1.0f, -1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right         
    { { 1.0f, -1.0f, -1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // bottom-right
    { { 1.0f,  1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // top-left
    { { 1.0f, -1.0f,  1.0f, 1.0f },   { 1.0f,  0.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left     
    // bottom face
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-left
    { {  1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-left
    { { -1.0f, -1.0f,  1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-right
    { { -1.0f, -1.0f, -1.0f, 1.0f },  { 0.0f, -1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-right
    // top face
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { {  1.0f,  1.0f , 1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { {  1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right     
    { {  1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right
    { { -1.0f,  1.0f, -1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
    { { -1.0f,  1.0f,  1.0f, 1.0f },  { 0.0f,  1.0f,  0.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }  // bottom-left  
};

// Layout:  vertex : normal
static const Vertex vertex_array_square[] = {
    { { -1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { {  1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { {  1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 0.0f } }, // bottom-right         
    { {  1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 1.0f, 1.0f } }, // top-right
    { { -1.0f, -1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 0.0f } }, // bottom-left
    { { -1.0f,  1.0f, 0.0f, 1.0f },     { 0.0f,  0.0f, -1.0f, 0.0f },   { 1.0f, 1.0f, 1.0f, 1.0f },     { 0.0f, 1.0f } }, // top-left
};

static const char* ce_shader_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 3) in vec2 tx;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    uniform mat4 PV;

    out vec4 color;

    void main() {
        gl_Position = PV * M * vp;
        color       = C;
    }
);

static const char* ce_shader_fragment = GLSL_SHADER(
    out vec4 frag_color;
    in  vec4 color;
    
    void main() {
        frag_color = color;
    }
);

static const char* ce_shader_vertex_light = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 1) in vec4 np;
    layout (location = 2) in vec4 vc;
    layout (location = 3) in vec2 tx;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec3 frag_pos;
    out vec3 normal;
    out vec2 tex_coords;
    out vec4 frag_pos_light_space;
    out vec4 color;

    uniform mat4 PV;
    uniform mat4 light_space_matrix;

    void main() {
        vec4 mpos               = M * vp;
        gl_Position             = PV * mpos;
        frag_pos                = mpos.xyz;
        normal                  = normalize((M * np).xyz);
        tex_coords              = tx;
        frag_pos_light_space    = light_space_matrix * vec4(frag_pos, 1.0);
        color                   = C * vc;
    }
);

static const char* ce_shader_fragment_light = GLSL_SHADER(
    out vec4 frag_color;

    in vec3 frag_pos;
    in vec3 normal;
    in vec2 tex_coords;
    in vec4 frag_pos_light_space;
    in vec4 color;

    uniform sampler2D shadow_map;
    uniform sampler2D color_map;

    uniform vec3 light_pos;
    uniform vec3 view_pos;
    //
    uniform vec3 light_color;

    float shadowCalculation(vec3 proj_coords) {
        if(proj_coords.z > 1.0)
            return 0.0;

        float current_depth = proj_coords.z;

        vec3 normal     = normalize(normal);
        vec3 light_dir  = normalize(light_pos - frag_pos);
        float bias      = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);

        float shadow = current_depth - bias > texture(shadow_map, proj_coords.xy).r? 1.0 : 0.0;

        return shadow;
    }

    vec4 colorCalculation(vec3 proj_coords) {
        vec4 closest_depth  = texture(color_map, proj_coords.xy);
        return closest_depth;
    }
   
    void main() {
        float   ambient_str     = 0.2;
        vec3    ambient         = ambient_str * light_color;
        vec3    light_dir       = normalize(light_pos - frag_pos);
        float   diff            = max(dot(normal, light_dir), 0.0);
        vec3    diffuse         = diff * light_color;
        float   specular_str    = 0.1;
        vec3    view_dir        = normalize(view_pos - frag_pos);
        vec3    reflect_dir     = reflect(-light_dir, normal);
        float   spec            = pow(max(dot(view_dir, reflect_dir), 0.0), 2);
        vec3    specular        = specular_str * spec * light_color;

        vec3 proj_coords = 0.5 * (frag_pos_light_space.xyz / frag_pos_light_space.w) + 0.5;

        float   shadow          = shadowCalculation(proj_coords);
        vec4    caster          = colorCalculation(proj_coords);

        vec3 result = color.rgb * (ambient + (1.0 - 0.7 * caster.a * shadow) * (diffuse + specular));

        frag_color = vec4(result, color.a);
    }
);

static const char* ce_depth_vertex = GLSL_SHADER(
    layout (location = 0) in vec4 vp;
    layout (location = 4) in vec4 C;
    layout (location = 5) in mat4 M;

    out vec4 color;

    uniform mat4 light_space_matrix;

    void main() {
        gl_Position = light_space_matrix * M * vp;
        color       = C;
    }
);

static const char* ce_depth_fragment = GLSL_SHADER(
    in  vec4 color;
    out vec4 frag_color;

    void main() {
        gl_FragDepth    = gl_FragCoord.z;
        frag_color      = color;
    }
);

// ----------------------------------------------------------------------------------------------- //

static int ce_shader;
static int ce_shader_no_light;
static int ce_shader_depth;

static u32 ce_vao_cube;
static u32 ce_vbo_cube;
static u32 ce_vbo_cube_data;

static u32 ce_depth_fbo;
static u32 ce_depth_map;

static u32 ce_color_map;

struct RenderObject {
    f32     color[4];
    f32     model[16];
};

static f32 ce_projection_view[16]    = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
static f32 ce_light_space_matrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
static f32 ce_view_pos[3]            = { 0.0f, 0.0f, 0.0f };
static f32 ce_light_pos[3]           = { 0.0f, 0.0f, 0.0f };
static f32 ce_light_color[3]         = { 1.0f, 1.0f, 1.0f };

#define RENDER_ARRAY_INIT_SIZE  (2048)
//
static int            render_capacity = 0;
static int            render_count    = 0;
static RenderObject   render_array[1024 * 1024];

static void ce_Push(const RenderObject obj) {
    if (render_count < ARRAY_COUNT(render_array))
        render_array[render_count++] = obj;
}

extern int ce_GetCubeCount(void) {
    return render_count;
}

extern void ce_SetLight(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b) {
    ce_light_pos[0] = x;
    ce_light_pos[1] = y;
    ce_light_pos[2] = z;

    ce_light_color[0] = r;
    ce_light_color[1] = g;
    ce_light_color[2] = b;
}

extern void ce_SetView(
        f32 pos_x,  f32 pos_y,  f32 pos_z,
        f32 look_x, f32 look_y, f32 look_z,
        f32 up_x,   f32 up_y,   f32 up_z,
        f32 fov,
        f32 near_plane,
        f32 far_plane) {
    f32 pos[3]  = { pos_x,  pos_y,  pos_z  };
    f32 look[3] = { look_x, look_y, look_z };
    f32 up[3]   = { up_x,   up_y,   up_z   };

    f4x4_PerspectiveView(ce_projection_view, pos, look, up, fov, platform.aspect_ratio, near_plane, far_plane);

    ce_view_pos[0] = pos[0];
    ce_view_pos[1] = pos[1];
    ce_view_pos[2] = pos[2];
}

extern void ce_SetView2D(
        f32 pos_x,  f32 pos_y,  f32 pos_z,
        f32 look_x, f32 look_y, f32 look_z,
        f32 up_x,   f32 up_y,   f32 up_z,
        f32 rad_x,  f32 rad_y,
        f32 near_plane,
        f32 far_plane) {
    f32 pos[3]  = { pos_x,  pos_y,  pos_z  };
    f32 look[3] = { look_x, look_y, look_z };
    f32 up[3]   = { up_x,   up_y,   up_z   };
    f32 rad[2]  = { rad_x, rad_y };

    f4x4_OrthoView(ce_projection_view, pos, look, up, rad, near_plane, far_plane);

    ce_view_pos[0] = pos[0];
    ce_view_pos[1] = pos[1];
    ce_view_pos[2] = pos[2];
}

extern void ce_SetProjectionView(const f32* projection_view) {
    f4x4_Copy(ce_projection_view, projection_view);

    ce_view_pos[0] = projection_view[12];
    ce_view_pos[1] = projection_view[13];
    ce_view_pos[2] = projection_view[14];
}

extern void ce_SetLightView(
        f32 pos_x,  f32 pos_y,  f32 pos_z,
        f32 look_x, f32 look_y, f32 look_z,
        f32 up_x,   f32 up_y,   f32 up_z,
        f32 fov,
        f32 near_plane,
        f32 far_plane) {
    f32 pos[3]  = { pos_x,  pos_y,  pos_z  };
    f32 look[3] = { look_x, look_y, look_z };
    f32 up[3]   = { up_x,   up_y,   up_z   };

    f32 light_projection[16];
    f32 light_view[16];

    f4x4_Perspective(light_projection, TO_RAD(fov), platform.aspect_ratio, near_plane, far_plane);
    f4x4_LookAt(light_view, pos, look, up);
    f4x4_Mul(ce_light_space_matrix, light_projection, light_view);
}

static void ce_SetVertexArray(const Vertex* vertex_array, int vertex_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof *vertex_array, vertex_array, GL_STATIC_DRAW);
}

static void ce_SetRenderObjectArray(const RenderObject* robj_array, int robj_count) {
    glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);
    glBufferData(GL_ARRAY_BUFFER, robj_count * sizeof *robj_array, robj_array, GL_STATIC_DRAW);
}

static void ce_Render(int type, const Vertex* vertex_array, int vertex_count, const RenderObject* robj_array, int robj_count) {
    ce_SetVertexArray(vertex_array, vertex_count);
    ce_SetRenderObjectArray(robj_array, robj_count);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // depth render:
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);

        ShaderUse(ce_shader_depth);
        ShaderSetMat4(ce_shader_depth, "light_space_matrix", ce_light_space_matrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);

        u32 attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, attachments);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glCullFace(GL_BACK);

    // normal render:
    {
        glEnable(GL_BLEND);
        glEnable(GL_ALPHA_TEST);

        glViewport(0, 0, platform.width, platform.height);

        ShaderUse(ce_shader);

        ShaderSetMat4(ce_shader,  "light_space_matrix", ce_light_space_matrix);
        ShaderSetMat4(ce_shader,  "PV",                 ce_projection_view);
        ShaderSet3v(ce_shader,    "view_pos",           ce_view_pos);
        ShaderSet3v(ce_shader,    "light_pos",          ce_light_pos);
        ShaderSet3v(ce_shader,    "light_color",        ce_light_color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }

    glDisable(GL_CULL_FACE);
}

extern void ce_RenderCubes(void) {
    ce_Render(GL_TRIANGLES, vertex_array_cube, 36, render_array, render_count);
    render_count = 0;
}

static void ce_RenderNoLight(int type, const Vertex* vertex_array, int vertex_count, const RenderObject* robj_array, int robj_count) {
    // normal render:
    {
        glViewport(0, 0, platform.width, platform.height);

        ShaderUse(ce_shader_no_light);

        ShaderSetMat4(ce_shader, "PV", ce_projection_view);

        // render:
        ce_SetVertexArray(vertex_array, vertex_count);
        ce_SetRenderObjectArray(robj_array, robj_count);

        glDrawArraysInstanced(type, 0, vertex_count, robj_count);
    }
}

extern void ce_RenderSquares(void) {
    ce_RenderNoLight(GL_TRIANGLES, vertex_array_square, 6, render_array, render_count);
    render_count = 0;
}

extern void ce_PushCube(f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 r, f32 g, f32 b, f32 a) {
    RenderObject robj = {
        { r, g, b, a },
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushCubeQuat(f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 qx, f32 qy, f32 qz, f32 qw, f32 r, f32 g, f32 b, f32 a) {
    f32 quat[4] = { qx, qy, qz, qw }, rot[16];

    RenderObject robj = {
        { r, g, b, a },
        // model:
        {
            scale,  0.0f,   0.0f,   0.0f,
            0.0f,   scale,  0.0f,   0.0f,
            0.0f,   0.0f,   scale,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    f4x4_Fromfq(rot, quat);
    f4x4_Mul(robj.model, robj.model, rot);

    ce_Push(robj);
}

extern void ce_PushBox(f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 r, f32 g, f32 b, f32 a) {
    RenderObject robj = {
        { r, g, b, a },
        // model:
        {
            rad_x,  0.0f,   0.0f,   0.0f,
            0.0f,   rad_y,  0.0f,   0.0f,
            0.0f,   0.0f,   rad_z,  0.0f,
            pos_x,  pos_y,  pos_z,  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushCubeRot(f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 rot, f32 r, f32 g, f32 b, f32 a) {
    f32 s = sinf(rot);
    f32 c = cosf(rot);

    RenderObject robj = {
        { r, g, b, a },
        {
             c * scale, s * scale,  0.0f,   0.0f,
            -s * scale, c * scale,  0.0f,   0.0f,
             0.0f,      0.0f,       scale,  0.0f,
             pos_x,     pos_y,      pos_z,  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushCubeEuler(f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 yaw, f32 pitch, f32 roll, f32 red, f32 green, f32 blue, f32 alpha) {
    f32 cy = Cos(yaw);
    f32 sy = Sin(yaw);
    f32 cp = Cos(pitch);
    f32 sp = Sin(pitch);
    f32 cr = Cos(roll);
    f32 sr = Sin(roll);

    RenderObject robj = {
        { red, green, blue, alpha },
        {
            scale * (cy * cp),                  scale * (sy * cp),                  scale * (-sp),          0.0f,
            scale * (cy * sp * sr - sy * cr),   scale * (sy * sp * sr + cy * cr),   scale * (cp * sr),      0.0f,
            scale * (cy * sp * cr + sy * sr),   scale * (sy * sp * cr - cy * sr),   scale * (cp * cr),      0.0f,
            pos_x,                              pos_y,                              pos_z,                  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushCubeRotMat(f32 pos_x, f32 pos_y, f32 pos_z, f32 scale, f32 Rxx, f32 Rxy, f32 Ryx, f32 Ryy, f32 r, f32 g, f32 b, f32 a) {
    RenderObject robj = {
        { r, g, b, a },
        {
            Rxx * scale,    Rxy * scale,    0.0f,       0.0f,
            Ryx * scale,    Ryy * scale,    0.0f,       0.0f,
            0.0f,           0,              scale,      0.0f,
            pos_x,          pos_y,          pos_z,      1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushBoxRot(f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 rot, f32 r, f32 g, f32 b, f32 a) {
    f32 s = Sin(rot);
    f32 c = Cos(rot);

    RenderObject robj = {
        { r, g, b, a },
        {
             c * rad_x, s * rad_x,  0.0f,   0.0f,
            -s * rad_y, c * rad_y,  0.0f,   0.0f,
             0.0f,      0.0f,       rad_z,  0.0f,
             pos_x,     pos_y,      pos_z,  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushBoxRotMat(f32 pos_x, f32 pos_y, f32 pos_z, f32 rad_x, f32 rad_y, f32 rad_z, f32 Rxx, f32 Rxy, f32 Ryx, f32 Ryy, f32 r, f32 g, f32 b, f32 a) {
    RenderObject robj = {
        { r, g, b, a },
        {
            Rxx * rad_x,    Rxy * rad_x,    0.0f,   0.0f,
            Ryx * rad_y,    Ryy * rad_y,    0.0f,   0.0f,
            0.0f,           0.0f,           rad_z,  0.0f,
            pos_x,          pos_y,          pos_z,  1.0f
        }
    };

    ce_Push(robj);
}

extern void ce_PushLine(const f32 p0[2], const f32 p1[2], f32 z, f32 rad, const f32 color[4]) {
    f32 line[2]     = { p1[0] - p0[0], p1[1] - p0[1] };
    f32 line_length = f2_Len(line);
    f32 line_pos[2] = { p0[0] + line[0] * 0.5f, p0[1] + line[1] * 0.5f };
    f32 axis[2]     = { 0.0f, -1.0f };
    f32 rot         = f2_GetAngle(axis, line);
    f32 pos[3]      = { line_pos[0], line_pos[1], z };
    f32 scale[3]    = { rad, 0.5f * line_length, rad };

    ce_PushBoxRot(pos[0], pos[1], pos[2], scale[0], scale[1], scale[2], rot, color[0], color[1], color[2], color[3]);
}

extern void ce_PushLineLoop(const f32 pos[2], f32 z, f32 rad, const f32 color[4], const f32 (*point_array)[2], int point_count) {
    f32 a[2], b[2];

    for (int i = 0; i < point_count; ++i) {
        int j = (i + 1) % point_count;

        a[0] = pos[0] + point_array[i][0];
        a[1] = pos[1] + point_array[i][1];

        b[0] = pos[0] + point_array[j][0];
        b[1] = pos[1] + point_array[j][1];

        ce_PushLine(a, b, z, rad, color);
    }
}

extern void ce_EnableDepthTest(void) {
    glEnable(GL_DEPTH_TEST);
}

extern void ce_DisableDepthTest(void) {
    glDisable(GL_DEPTH_TEST);
}

extern void ce_Clear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

extern void ce_Init(void) {
    ce_shader          = ShaderLoadFromMemory(ce_shader_vertex_light, ce_shader_fragment_light, NULL);
    ce_shader_no_light = ShaderLoadFromMemory(ce_shader_vertex, ce_shader_fragment, NULL);

    // init opengl:
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0f);
        glDepthFunc(GL_LESS);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
    }

    ShaderUse(ce_shader);

    // setup vbo/vao:
    {
        glGenVertexArrays(1, &ce_vao_cube);
        glGenBuffers(1, &ce_vbo_cube);
        glGenBuffers(1, &ce_vbo_cube_data);

        glBindVertexArray(ce_vao_cube);

        // cube:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube);
            //
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);

            // position : normal : color
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, normal));
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, color));
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, texture));
        }

        // cube models:
        {
            glBindBuffer(GL_ARRAY_BUFFER, ce_vbo_cube_data);

            // color:
            glEnableVertexAttribArray(4);

            glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof (RenderObject), (void*)(offsetof(RenderObject, color)));
            glVertexAttribDivisor(4, 1);

            // model:
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);

            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof (RenderObject), (void*)(offsetof(RenderObject, model) + 0 * sizeof (f32[4])));
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof (RenderObject), (void*)(offsetof(RenderObject, model) + 1 * sizeof (f32[4])));
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof (RenderObject), (void*)(offsetof(RenderObject, model) + 2 * sizeof (f32[4])));
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof (RenderObject), (void*)(offsetof(RenderObject, model) + 3 * sizeof (f32[4])));

            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
        }
    }

    // init shadow map stuff
    {
        ce_shader_depth = ShaderLoadFromMemory(ce_depth_vertex, ce_depth_fragment, NULL);

        glGenFramebuffers(1, &ce_depth_fbo);
        // create depth texture
        glGenTextures(1, &ce_depth_map);
        glBindTexture(GL_TEXTURE_2D, ce_depth_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, ce_depth_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ce_depth_map, 0);

        glGenTextures(1, &ce_color_map);
        glBindTexture(GL_TEXTURE_2D, ce_color_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        // attach color texture as FBO's color buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ce_color_map, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ShaderUse(ce_shader);
        ShaderSeti(ce_shader, "shadow_map", 0);
        ShaderSeti(ce_shader, "color_map",  1);
    }
}

static void ce_BitmapRenderRect(f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 r, f32 g, f32 b, f32 a) {
    ce_PushBox(
            x, y, z, 
            0.5f * sx, 0.5f * sy, 0.01,
            r, g, b, a);
}

// @TODO: make less shit!!
static void ce_PushAscii(unsigned char c, f32 px, f32 py, f32 pz, f32 x_scale, f32 y_scale, f32 r, f32 g, f32 b, f32 a) {
    u64 n = bitascii[c];

    for (int j = 0; j < 8; ++j) {
        for (int i = 0; i < 8; ++i) {
            int bit = j * 8 + i;

            if (CHKBIT(n, bit)) {
                ce_BitmapRenderRect(px + i * x_scale, py - j * y_scale, pz, x_scale, -y_scale, r, g, b, a);
            }
        }
    }
}

extern void ce_PushString(const char* str, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    for (int i = 0; str[i] != '\0'; i++)
        ce_PushAscii(str[i], x + i * 8 * scale_x, y, z, scale_x, scale_y, r, g, b, a);
}

extern void ce_PushStringFormat(f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a, const char* fmt, ...) {
    va_list list;
    char    buffer[64];

    va_start(list, fmt);
    vsnprintf(buffer, 64, fmt, list);
    ce_PushString(buffer, x, y, z, rad, rad, r, g, b, a);
    va_end(list);
}

extern void ce_PushInt(int n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    char buffer[64];

    sprintf(buffer, "%d", n);
    ce_PushString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

extern void ce_PushF32(f32 n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    char buffer[64];

    sprintf(buffer, "%.1f", n);
    ce_PushString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

extern void ce_PushStringBox(const char* str, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a) {
    int slen    = strlen(str);
    f32 x_scale = w / ((f32)slen * 8.0f);
    f32 y_scale = h / 8.0f;

    for (int i = 0; i < slen; i++) {
        ce_PushAscii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, r, g, b, a);
    }
}

extern void ce_PushNumberBox(int n, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a) {
    char buffer[32];

    sprintf(buffer, "%d", n);
    ce_PushStringBox(buffer, x, y, z, w, h, r, g, b, a);
}

#endif // ATS_CUBE_ENGINE

// ============================================ RENDER (LEGACY OPENGL) ============================================ //

#if defined(ATS_PLATFORM_GLFW) && !defined(ATS_MODERN_OPENGL)

static int cube_list;
static int square_list;

static void RenderBeginShapes(void) {
    square_list = glGenLists(1);
    cube_list   = glGenLists(1);

    {
        glNewList(square_list, GL_COMPILE);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, -1.0f, 0.0f);
        glVertex3f(+1.0f, +1.0f, 0.0f);
        glVertex3f(-1.0f, +1.0f, 0.0f);

        glEnd();

        glEndList();
    }

    {
        glNewList(cube_list, GL_COMPILE);

        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, -1.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);

        glNormal3f(0.0f, -1.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);

        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, -1.0f);
        glVertex3f(-1.0f, +1.0f, +1.0f);
        glVertex3f(-1.0f, -1.0f, +1.0f);
        
        glNormal3f(1.0f, 0.0f, 0.0f);

        glVertex3f(+1.0f, -1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, -1.0f);
        glVertex3f(+1.0f, +1.0f, +1.0f);
        glVertex3f(+1.0f, -1.0f, +1.0f);

        glEnd();

        glEndList();
    }
}

static void BitmapInit(void);

extern void RenderInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);

    glEnable(GL_NORMALIZE);

    RenderBeginShapes();
    
    BitmapInit();
}

extern void RenderClear(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

extern void RenderSetMatrix(const f32* matrix) {
    glLoadMatrixf(matrix);
}

extern void RenderSetProjectionView(const f32 *projection_view) {
    glMatrixMode(GL_PROJECTION);
    RenderSetMatrix(projection_view);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

extern void RenderSetModel(f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz) {
    f32 T[16];
    f4x4_TranslateScale(T, px, py, pz, sx, sy, sz);
    RenderSetMatrix(T);
}

extern void RenderSetRotatedModel(f32 px, f32 py, f32 pz, f32 sx, f32 sy, f32 sz, f32 rot) {
    f32 T[16];
    f4x4_TranslateScaleRotateZ(T, px, py, pz, sx, sy, sz, rot);
    RenderSetMatrix(T);
}

extern void RenderSetTranslateScaleRotateZ(
        f32 px, f32 py, f32 pz,
        f32 sx, f32 sy, f32 sz,
        f32 rot) {
    f32 T[16];
    f4x4_TranslateScaleRotateZ(T, px, py, pz, sx, sy, sz, rot);
    RenderSetMatrix(T);
}

extern void RenderSetCamera(
        f32 px,  f32 py, f32 pz,
        f32 tx,  f32 ty, f32 tz,
        f32 ux,  f32 uy, f32 uz,
        f32 fov, f32 aspect, f32 n,  f32 f) {
    f32 P[16];
    f32 V[16];
    f32 PV[16];

    f32 pos[3]  = { px, py, pz };
    f32 look[3] = { tx, ty, tz };
    f32 up[3]   = { ux, uy, uz };

    f4x4_Perspective(P, TO_RAD(fov), aspect, n, f);
    f4x4_LookAt(V, pos, look, up);
    f4x4_Mul(PV, P, V);

    RenderSetProjectionView(PV);
}

extern void RenderSetCamera2D(
        f32 px,  f32 py, f32 pz,
        f32 tx,  f32 ty, f32 tz,
        f32 ux,  f32 uy, f32 uz,
        f32 sx,  f32 sy,
        f32 n,  f32 f) {
    f32 PV[16];

    f32 pos[3]  = { px, py, pz };
    f32 look[3] = { tx, ty, tz };
    f32 up[3]   = { ux, uy, uz };
    f32 rad[2]  = { sx, sy };

    f4x4_OrthoView(PV, pos, look, up, rad, n, f);

    RenderSetProjectionView(PV);
}

extern void RenderSetCameraOrtho(f32 width, f32 height, f32 n, f32 f) {
    f32 PV[16];

    f4x4_Ortho(PV, 0, width, height, 0, n, f);
    RenderSetProjectionView(PV);
}

extern void RenderSetCameraTranslateOrtho(f32 x, f32 y, f32 rad_x, f32 rad_y, f32 n, f32 f) {
    f32 PV[16];

    f4x4_Ortho(PV, x - rad_x, x + rad_x, y + rad_y, y - rad_y, n, f);
    RenderSetProjectionView(PV);
}

extern void RenderSetColor(f32 r, f32 g, f32 b, f32 a) {
    glColor4f(r, g, b, a);
}

extern void RenderSquare(f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a) {
    RenderSetColor(r, g, b, a);
    RenderSetModel(x, y, z, rad, rad, rad);

    glCallList(square_list);
}

extern void RenderRotatedSquare(f32 x, f32 y, f32 z, f32 rad, f32 rot, f32 r, f32 g, f32 b, f32 a) {
    RenderSetColor(r, g, b, a);
    RenderSetRotatedModel(x, y, z, rad, rad, rad, rot);

    glCallList(square_list);
}

extern void RenderRect(f32 x, f32 y, f32 z, f32 width, f32 height, f32 r, f32 g, f32 b, f32 a) {
    f32 half_width  = 0.5f * width;
    f32 half_height = 0.5f * height;

    RenderSetColor(r, g, b, a);
    RenderSetModel(x + half_width, y + half_height, z, half_width, half_height, 1.0f);

    glCallList(square_list);
}

extern void RenderCenterRect(f32 x, f32 y, f32 z, f32 half_width, f32 half_height, f32 r, f32 g, f32 b, f32 a) {
    RenderSetColor(r, g, b, a);
    RenderSetModel(x, y, z, half_width, half_height, 1.0f);

    glCallList(square_list);
}

extern void RenderCube(f32 x, f32 y, f32 z, f32 rad, f32 r, f32 g, f32 b, f32 a) {
    RenderSetColor(r, g, b, a);
    RenderSetModel(x, y, z, rad, rad, rad);

    glCallList(cube_list);
}

extern void RenderLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 r, f32 g, f32 b, f32 a) {
    RenderSetColor(r, g, b, a);

    glLoadIdentity();
    glBegin(GL_LINES);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y1, z1);

    glEnd();
}

extern void RenderLineLoop2D(
        f32 pos_x, f32 pos_y, f32 pos_z,
        f32 rad, f32 rot,
        f32 red, f32 green, f32 blue, f32 alpha,
        const f32 (*point_array)[2], int point_count) {
    glLoadIdentity();
    glTranslatef(pos_x, pos_y, pos_z);
    glScalef(rad, rad, 1.0f);
    glRotatef(TO_DEG(rot), 0.0f, 0.0f, 1.0f);

    glColor4f(red, green, blue, alpha);

    glBegin(GL_LINE_LOOP);

    glNormal3f(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < point_count; ++i) {
        const f32* a = point_array[i];
        const f32* b = point_array[(i + 1) & (point_count - 1)];

        glVertex3f(a[0], a[1], 0.0f);
        glVertex3f(b[0], b[1], 0.0f);
    }

    glEnd();
}

extern void RenderSetLightEmitter(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 pos[4]  = { x, y, z, 1.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { bright, bright, bright, 0.0f };
    u32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, pos);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

extern void RenderSetLightDirected(int index, f32 bright, f32 x, f32 y, f32 z) {
    f32 d       = (f32)(1.0f / __builtin_sqrtf(x * x + y * y + z * z));
    f32 dir[4]  = { x * d, y * d, z * d, 0.0f };
    f32 zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    f32 c[4]    = { bright, bright, bright, 0.0f };
    u32 light   = GL_LIGHT0 + index;

    glLightfv(light, GL_POSITION, dir);
    glLightfv(light, GL_DIFFUSE,  c);
    glLightfv(light, GL_AMBIENT,  zero);
    glLightfv(light, GL_SPECULAR, zero);

    glEnable(light);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

extern void RenderSetLightGlobalAmbient(f32 r, f32 g, f32 b) {
    f32 v[4] = { r, g, b, 0 };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, v);
}

// ======================================= TEXTURES ======================================== //

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb/stb_image.h" 

struct Texture {
    u32     id;
    int     width;
    int     height;
};

extern Texture TextureCreate(unsigned char *pixels, int width, int height, int is_smooth) {
    assert(pixels);

    Texture texture  = {0};

    texture.width   = width;
    texture.height  = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

    return texture;
}

extern Texture TextureLoadFromFile(const char *texture_path, int is_smooth) {
    Texture         texture     = {0};
    i32             channels    = 0;
    unsigned char*  pixels      = NULL;

    pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 0);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);

    stbi_image_free(pixels);

    return texture;
}

extern void TextureBind(const Texture *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);

    glMatrixMode(GL_MODELVIEW);
}

extern void TextureDelete(Texture* texture) {
    glDeleteTextures(1, &texture->id);
    
    memset(texture, 0, sizeof *texture);
}

static int bitmap_display_list[BITMAP_COUNT];

#define BITMAP_GETBIT(N, X, Y) (((u64)(N)) & (1ull << (((u64)(Y)) * 8ull + ((u64)(X)))))

static void BitmapInit(void) {
    for (int i = 0; i < BITMAP_COUNT; ++i) {
        bitmap_display_list[i] = glGenLists(1);

        glNewList(bitmap_display_list[i], GL_COMPILE);

        uint64_t c = bitascii[i];

        glBegin(GL_QUADS);

        f32 scale = 1.0f / 8.0f;

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < 8; ++i) {
                f32 x   = i * scale;
                f32 y   = j * scale;

                if (BITMAP_GETBIT(c, i, j)) {
                    glVertex3f(x - 0, y - 0,  0.0f);
                    glVertex3f(x + scale, y - 0,  0.0f);
                    glVertex3f(x + scale, y + scale,  0.0f);
                    glVertex3f(x - 0, y + scale,  0.0f);
                }
            }
        }

        glEnd();

        glEndList();
    }
}

extern void RenderAscii(unsigned char c, f32 px, f32 py, f32 pz, f32 x_scale, f32 y_scale, f32 r, f32 g, f32 b, f32 a) {
    glColor4f(r, g, b, a);

    f32 T[16];
    f4x4_TranslateScale(T, px, py, pz, x_scale, y_scale, 1.0f);
    RenderSetMatrix(T);

    glCallList(bitmap_display_list[c]);
}

extern void RenderString(const char *str, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    for (int i = 0; str[i] != '\0'; i++) {
        RenderAscii(str[i], x + i * scale_x, y, z, scale_x, scale_y, r, g, b, a);
    }
}

extern void RenderStringFormat(f32 x, f32 y, f32 z, f32 rad_x, f32 rad_y, f32 r, f32 g, f32 b, f32 a, const char* fmt, ...) {
    va_list list;
    char    buffer[256];

    va_start(list, fmt);

    vsnprintf(buffer, 256, fmt, list);
    RenderString(buffer, x, y, z, rad_x, rad_y, r, g, b, a);

    va_end(list);
}

extern void RenderInt(int n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    char buffer[32];
    sprintf(buffer, "%d", n);
    RenderString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

extern void RenderF32(f32 n, f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 r, f32 g, f32 b, f32 a) {
    char buffer[32];
    sprintf(buffer, "%.1f", n);
    RenderString(buffer, x, y, z, scale_x, scale_y, r, g, b, a);
}

extern void RenderStringBox(const char* str, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a) {
    int   slen = strlen(str);
    f32 x_scale = w / ((f32)slen * 8.0f);
    f32 y_scale = h / 8.0f;
    
    for (int i = 0; i < slen; i++) {
        RenderAscii(str[i], x + i * 8 * x_scale, y, z, x_scale, y_scale, r, g, b, a);
    }
}

extern void RenderNumberBox(int n, f32 x, f32 y, f32 z, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a) {
    char buffer[32];
    sprintf(buffer, "%d", n);
    RenderStringBox(buffer, x, y, z, w, h, r, g, b, a);
}

#endif // ATS_MODERN_OPENGL

#endif // ATS_IMPLEMENTATION_ONCE
#endif // ATS_IMPLEMENTATION

