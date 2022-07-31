#pragma once

#ifdef ATS_OGL33
#include "ext/glad/glad.h"
#endif

#undef APIENTRY
#include "ext/GLFW/glfw3.h"

#if defined(ATS_OGL33)
#define GLSL(...) "#version 330 core\n" #__VA_ARGS__
#endif

// ====================================================== API =================================================== //

// ------------------- platform layer ------------------------ //

struct platform;
extern struct platform platform;

extern void platform_init(const char* title, int width, int height, int samples);
extern void platform_update(void);

extern f64 timer_get_current(void);

// ---------------- gl helper functions/types ---------------- //

extern void gl_init(void);

extern void gl_set_simple_light_emitter(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_simple_light_directed(int index, f32 bright, f32 x, f32 y, f32 z);
extern void gl_set_light_emitter(int index, v3 p, v3 color, f32 constant, f32 linear, f32 quadratic);
extern void gl_set_light_directed(int index, v3 pos, v3 color);
extern void gl_set_light_global_ambient(f32 r, f32 g, f32 b);

extern void gl_init_bitmap_font(void);
extern void gl_string(const char *str, f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color);
extern void gl_string_format(f32 x, f32 y, f32 z, f32 sx, f32 sy, u32 color, const char* fmt, ...);

extern void gl_begin(u32 type);
extern void gl_end(void);
extern void gl_color(u32 color);
extern void gl_normal(f32 x, f32 y, f32 z);
extern void gl_uv(f32 x, f32 y);
extern void gl_vertex(f32 x, f32 y, f32 z);
extern void gl_set_matrix(m4 projection, m4 view);
extern void gl_billboard(r2i tex_rect, v3 pos, v2 rad, v3 normal, u32 color, v3 right, v3 up);
extern void gl_texture_box(r2i tex_rect, r3 box, u32 color);
extern void gl_texture_rect(r2i tex_rect, r2 rect, f32 z, u32 color);
extern void gl_texture_rect_flip(r2i tex_rect, r2 rect, f32 z, u32 color, bool flip_x, bool flip_y);
extern void gl_box(r3 box, u32 color);
extern void gl_rect(r2 rect, f32 z, u32 color);

typedef struct gl_texture {
    u32 id;
    i32 width;
    i32 height;
} gl_texture;

extern gl_texture gl_texture_create(void *pixels, int width, int height, int is_smooth);
extern gl_texture gl_texture_create_from_image(image image, int is_smooth);
extern gl_texture gl_texture_load_from_file(const char *texture_path, int is_smooth);

extern void gl_texture_update(gl_texture* texture, void *pixels, int width, int height, int is_smooth);
extern void gl_texture_bind(const gl_texture* texture);
extern void gl_texture_destroy(gl_texture* texture);

typedef struct gl_shader {
    u32 id;
} gl_shader;

typedef struct gl_shader_desc {
    const char* vs;
    const char* fs;
} gl_shader_desc;

extern gl_shader gl_shader_create(const  gl_shader_desc* desc);
extern gl_shader gl_shader_load_from_file(const char *vs, const char *fs,  mem_allocator allocator);

extern void gl_use(const gl_shader* shader);
extern u32  gl_location(const gl_shader* shader, const char* name);

extern void gl_uniform_i32(u32 location, int u);
extern void gl_uniform_f32(u32 location, f32 u);
extern void gl_uniform_v2(u32 location, v2 u);
extern void gl_uniform_v3(u32 location, v3 u);
extern void gl_uniform_v4(u32 location, v4 u);
extern void gl_uniform_m2(u32 location, m2 m);
extern void gl_uniform_m3(u32 location, m3 m);
extern void gl_uniform_m4(u32 location, m4 m);

extern v3 gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview);

typedef struct gl_buffer {
    u32 vao;
    u32 vbo;
} gl_buffer;

typedef struct gl_layout {
    u32 size;
    u32 type;
    u32 stride;
    u32 offset;
    b32 normalize;
} gl_layout;

typedef struct gl_buffer_desc {
    gl_layout layout[32];
} gl_buffer_desc;

extern gl_buffer gl_buffer_create(const gl_buffer_desc* desc);
extern void gl_buffer_bind(const gl_buffer* buffer);
extern void gl_buffer_send(const gl_buffer* array, const void* data, u32 size);

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

typedef union gamepad_buttons {
    struct {
        u32 x : 1;
        u32 a : 1;
        u32 b : 1;
        u32 y : 1;

        u32 left_bumper     : 1;
        u32 right_bumper    : 1;
        u32 left_trigger    : 1;
        u32 right_trigger   : 1;
        
        u32 select          : 1;
        u32 start           : 1;
        u32 left_stick      : 1;
        u32 right_stick     : 1;

        u32 up      : 1;
        u32 right   : 1;
        u32 down    : 1;
        u32 left    : 1;
    } button;

    u32 data;
} gamepad_buttons;

typedef struct gamepad {
    b32 active;

    v2 left_stick;
    v2 right_stick;

    f32 left_trigger;
    f32 right_trigger;

    gamepad_buttons down;
    gamepad_buttons pressed;
    gamepad_buttons released;
} gamepad;

typedef u32 mouse_mode;
enum {
    mouse_mode_normal,
    mouse_mode_hidden,
    mouse_mode_disabled
};

struct platform {
    b32 close;

    i32 width;
    i32 height;
    f32 aspect_ratio;

    void* native;

    b32 fullscreen;
    b32 _fullscreen_state_last_update;

    struct {
        f64 total;
        f32 delta;
    } time;

    struct {
        u32 mode;

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
};

extern struct platform platform;

