#pragma once

#ifdef ATS_OGL33
#include "dep/glad/glad.h"
#endif

// ====================================================== API =================================================== //

// platform layer:

typedef struct platform_t   platform_t;
extern platform_t platform;

extern void platform_init(const char* title, int width, int height, int samples);
extern void platform_update(void);

extern f64 timer_get_current(void);

// gl helper types/functions:

typedef struct gl_texture_t gl_texture_t;
struct gl_texture_t {
    u32     id;
    int     width;
    int     height;
};

extern gl_texture_t gl_texture_create(void *pixels, int width, int height, int is_smooth);
extern void         gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth);
extern gl_texture_t gl_texture_load_from_file(const char *texture_path, int is_smooth);
extern void         gl_texture_bind(const gl_texture_t *texture);
extern void         gl_texture_delete(gl_texture_t* texture);

#ifdef ATS_OGL33

#define GLSL_SHADER(shader) "#version 330 core\n" #shader

typedef struct gl_shader_t {
    u32 id;
} gl_shader_t;

extern gl_shader_t gl_shader_load_from_memory(const char *vs, const char *fs);
extern gl_shader_t gl_shader_load_from_file(const char *vs, const char *fs, memory_arena_t* ma);

extern vec3_t gl_get_world_position(int x, int y, mat4_t in_projection, mat4_t in_modelview);

#endif

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

typedef union gamepad_buttons_t gamepad_buttons_t;
union gamepad_buttons_t {
    struct {
        u32 X : 1;
        u32 A : 1;
        u32 B : 1;
        u32 Y : 1;

        u32 LB : 1;
        u32 RB : 1;
        u32 LT : 1;
        u32 RT : 1;
        
        u32 select : 1;
        u32 start : 1;
        u32 LS : 1;
        u32 RS : 1;

        u32 UP : 1;
        u32 RIGHT : 1;
        u32 DOWN : 1;
        u32 LEFT : 1;
    } button;

    u32 data;
};

typedef struct gamepad_t gamepad_t;
struct gamepad_t {
    b32 active;

    vec2_t LS;
    vec2_t RS;

    f32 LT;
    f32 RT;

    gamepad_buttons_t down;
    gamepad_buttons_t pressed;
    gamepad_buttons_t released;
};

enum {
    MOUSE_NORMAL,
    MOUSE_HIDDEN,
    MOUSE_DISABLED
};

typedef struct platform_t platform_t;
struct platform_t {
    b32 close;

    i32 width;
    i32 height;
    f32 aspect_ratio;

    void* native;

    b32 fullscreen;
    b32 _fullscreen_state_last_update;

    struct {
        f64 total;
        f64 delta;
    } time;

    struct {
        u32 mode;

        b32 is_down     : 1;
        b32 is_pressed  : 1;
        b32 is_released : 1;

        vec2_t pos;
        vec2_t delta;
        vec2_t scroll;

        b8 down[MOUSE_BUTTON_LAST + 1];
        b8 pressed[MOUSE_BUTTON_LAST + 1];
        b8 released[MOUSE_BUTTON_LAST + 1];
    } mouse;

    struct {
        i32 key;
        i32 ascii;

        b32 is_down     : 1;
        b32 is_pressed  : 1;
        b32 is_repeat   : 1;
        b32 is_released : 1;
        b32 is_ascii    : 1;
    
        b8 down[KEY_LAST + 1];
        b8 pressed[KEY_LAST + 1];
        b8 repeat[KEY_LAST + 1];
        b8 released[KEY_LAST + 1];
    } keyboard;

    gamepad_t gamepad[JOYSTICK_LAST];
} platform;

// ===================================================================================================== //
// ======================================= PLATFORM IMPL =============================================== //
// ===================================================================================================== //
#ifdef ATS_IMPL

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#endif

#ifdef ATS_OGL33
#include "dep/glad/glad.c"
#endif

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

extern platform_t platform;

static struct {
    GLFWwindow* window;
    GLFWmonitor* monitor;
} platform_internal;

static void window_key_callback(GLFWwindow* window, int key, int a, int action, int b) {
    (void)window;
    (void)a;
    (void)b;

    switch (action) {
        case GLFW_PRESS:
            platform.keyboard.key = key;
            platform.keyboard.is_down = 1;
            platform.keyboard.is_pressed = 1;
            platform.keyboard.is_repeat = 1;
            platform.keyboard.down[key] = 1;
            platform.keyboard.pressed[key] = 1;
            platform.keyboard.repeat[key] = 1;
            break;
        case GLFW_REPEAT:
            platform.keyboard.is_repeat = 1;
            platform.keyboard.repeat[key] = 1;
            break;
        case GLFW_RELEASE:
            platform.keyboard.is_down = 0;
            platform.keyboard.is_released = 1;
            
            platform.keyboard.down[key] = 0;
            platform.keyboard.released[key] = 1;
            break;
    }
}

static void window_char_callback(GLFWwindow* window, unsigned int codepoint) {
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

static void window_mouse_button_callback(GLFWwindow* window, int button, int action, int a) {
    (void)window;
    (void)a;

    switch (action) {
        case GLFW_PRESS: {
            platform.mouse.is_down = 1;
            platform.mouse.is_pressed = 1;
            platform.mouse.down[button] = 1;
            platform.mouse.pressed[button] = 1;
        } break;
        case GLFW_RELEASE: {
            platform.mouse.is_down = 0;
            platform.mouse.is_released = 1;
            platform.mouse.down[button] = 0;
            platform.mouse.released[button] = 1;
        } break;
    }
}

static void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window;

    platform.mouse.scroll.x = (f32)xoffset;
    platform.mouse.scroll.y = (f32)yoffset;
}

static void window_joystick_callback(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

extern void platform_init(const char* title, int width, int height, int samples) {
    glfwInit();

    platform_internal.monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#ifdef ATS_OGL33
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.width = width;
    platform.height = height;

    platform_internal.window = glfwCreateWindow(width, height, title, NULL, NULL);
    platform.native = glfwGetWin32Window(platform_internal.window);

    glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform_internal.window);

#ifdef ATS_OGL33 
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

    glfwSetKeyCallback(platform_internal.window, window_key_callback);
    glfwSetCharCallback(platform_internal.window, window_char_callback);
    glfwSetMouseButtonCallback(platform_internal.window, window_mouse_button_callback);
    glfwSetScrollCallback(platform_internal.window, window_scroll_callback);
    glfwSetJoystickCallback(window_joystick_callback);

    // init mouse:
    {
        f64 x = 0.0;
        f64 y = 0.0;

        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.pos.x = (f32)x;
        platform.mouse.pos.y = (f32)y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i))
            platform.gamepad[i].active = 1;
    }

    glfwSetTime(0.0);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glfwSwapBuffers(platform_internal.window);
    glfwPollEvents();
}

extern void platform_update(void) {
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
        f64 x, y;
        glfwGetCursorPos(platform_internal.window, &x, &y);

        platform.mouse.delta.x  = (f32)(x - platform.mouse.pos.x);
        platform.mouse.delta.y  = (f32)(y - platform.mouse.pos.y);

        platform.mouse.pos.x    = (f32)x;
        platform.mouse.pos.y    = (f32)y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        switch (platform.mouse.mode) {
            case MOUSE_NORMAL: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } break;
            case MOUSE_HIDDEN: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            } break;
            case MOUSE_DISABLED: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } break;
        }
    }

    // update gamepads:
    {
        GLFWgamepadstate state;

        for (int i = 0; i < JOYSTICK_LAST; ++i) {
            if (platform.gamepad[i].active) {
                gamepad_buttons_t old = platform.gamepad[i].down;

                platform.gamepad[i].down.data = 0;
                platform.gamepad[i].pressed.data = 0;
                platform.gamepad[i].released.data = 0;

                glfwGetGamepadState(i, &state);

                platform.gamepad[i].LS.x = +state.axes[GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].LS.y = -state.axes[GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].RS.x = +state.axes[GAMEPAD_AXIS_RIGHT_X];
                platform.gamepad[i].RS.y = -state.axes[GAMEPAD_AXIS_RIGHT_Y];

                platform.gamepad[i].LT = 0.5f * (state.axes[GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f);
                platform.gamepad[i].RT = 0.5f * (state.axes[GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f);

                if (state.buttons[GAMEPAD_BUTTON_X]) platform.gamepad[i].down.button.X = 1;
                if (state.buttons[GAMEPAD_BUTTON_A]) platform.gamepad[i].down.button.A = 1;
                if (state.buttons[GAMEPAD_BUTTON_B]) platform.gamepad[i].down.button.B = 1;
                if (state.buttons[GAMEPAD_BUTTON_Y]) platform.gamepad[i].down.button.Y = 1;

                if (state.buttons[GAMEPAD_BUTTON_LEFT_BUMPER])  platform.gamepad[i].down.button.LB = 1;
                if (state.buttons[GAMEPAD_BUTTON_RIGHT_BUMPER]) platform.gamepad[i].down.button.RB = 1;

                if (platform.gamepad[i].LT > 0.0f) platform.gamepad[i].down.button.LT = 1;
                if (platform.gamepad[i].RT > 0.0f) platform.gamepad[i].down.button.RT = 1;

                if (state.buttons[GAMEPAD_BUTTON_BACK]) platform.gamepad[i].down.button.select = 1;
                if (state.buttons[GAMEPAD_BUTTON_START]) platform.gamepad[i].down.button.start = 1;
                if (state.buttons[GAMEPAD_BUTTON_LEFT_THUMB]) platform.gamepad[i].down.button.LS = 1;
                if (state.buttons[GAMEPAD_BUTTON_RIGHT_THUMB]) platform.gamepad[i].down.button.RS = 1;

                if (state.buttons[GAMEPAD_BUTTON_DPAD_UP]) platform.gamepad[i].down.button.UP = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_RIGHT]) platform.gamepad[i].down.button.RIGHT = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_DOWN]) platform.gamepad[i].down.button.DOWN = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_LEFT]) platform.gamepad[i].down.button.LEFT = 1;

                platform.gamepad[i].pressed.data = platform.gamepad[i].down.data & ~old.data;
                platform.gamepad[i].released.data = ~platform.gamepad[i].down.data & old.data;
            }
        }
    }

    if (platform.fullscreen != platform._fullscreen_state_last_update) {
        const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

        if (platform.fullscreen)
            glfwSetWindowMonitor(platform_internal.window, platform_internal.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        else
            glfwSetWindowMonitor(platform_internal.window, NULL, 64, 64, mode->width - 256, mode->height - 256, mode->refreshRate);
    }

    platform._fullscreen_state_last_update = platform.fullscreen;

    glfwGetWindowSize(platform_internal.window, &platform.width, &platform.height);
    platform.aspect_ratio = (f32)platform.width / (f32)platform.height;

    glViewport(0, 0, platform.width, platform.height);

    memset(platform.keyboard.pressed,  0, sizeof (platform.keyboard.pressed));
    memset(platform.keyboard.repeat,   0, sizeof (platform.keyboard.repeat));
    memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

    memset(platform.mouse.pressed,  0, sizeof (platform.mouse.pressed));
    memset(platform.mouse.released, 0, sizeof (platform.mouse.released));

    glfwPollEvents();
    glfwSwapBuffers(platform_internal.window);

    platform.time.delta = glfwGetTime() - platform.time.total;
    platform.time.total += platform.time.delta;
}

extern f64 timer_get_current(void) {
    return glfwGetTime();
}

extern gl_texture_t gl_texture_create(void *pixels, int width, int height, int is_smooth) {
    assert(pixels);

    gl_texture_t texture = {0};

    texture.width = width;
    texture.height = height;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    
#ifdef ATS_OGL33
    glGenerateMipmap(GL_TEXTURE_2D);
#endif

    return texture;
}

extern void gl_texture_update(gl_texture_t* texture, void *pixels, int width, int height, int is_smooth) {
    texture->width = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

#ifdef ATS_OGL33
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

extern gl_texture_t gl_texture_load_from_file(const char *texture_path, int is_smooth) {
    gl_texture_t texture = ATS_ZERO_INIT;
    i32 channels = 0;

    unsigned char* pixels = stbi_load(texture_path, &texture.width, &texture.height, &channels, 4);

    assert(pixels);

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth? GL_LINEAR : GL_NEAREST);

#ifdef ATS_OGL33
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
    stbi_image_free(pixels);

    return texture;
}

#ifndef ATS_OGL33
extern void gl_texture_bind(const gl_texture_t *texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glMatrixMode(GL_TEXTURE);

    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);
}
#endif

extern void gl_texture_delete(gl_texture_t* texture) {
    glDeleteTextures(1, &texture->id);
    memset(texture, 0, sizeof *texture);
}

#ifdef ATS_OGL33

static u32 gl_shader_compile(const char* source, unsigned int type) {
    int success = 0;
    char info_log[512] = {0};
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

static u32 gl_shader_link_program(u32 vertex_shader, u32 fragment_shader) {
    int success = 0;
    char info_log[512] = {0};

    u32 shader = glCreateProgram();

    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);

    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, info_log);
        puts(info_log);
        exit(EXIT_FAILURE);
    }

    return shader;
}

extern gl_shader_t gl_shader_load_from_memory(const char *vs, const char *fs) {
    u32 vertex   = gl_shader_compile(vs, GL_VERTEX_SHADER);
    u32 fragment = gl_shader_compile(fs, GL_FRAGMENT_SHADER);
    u32 program  = gl_shader_link_program(vertex, fragment);

    glUseProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return (gl_shader_t) { .id = program };
}

extern gl_shader_t gl_shader_load_from_file(const char *vs, const char *fs, memory_arena_t* ma) {
    ma_save(ma);

    char* vs_content = file_read_str(vs, ma);
    char* fs_content = file_read_str(fs, ma);

    gl_shader_t program = gl_shader_load_from_memory(vs_content, fs_content);

    ma_restore(ma);

    return program;
}

extern vec3_t gl_get_world_position(int x, int y, mat4_t in_projection, mat4_t in_modelview) {
    GLint viewport[4] = {0};
    f64 modelview[16] = {0};
    f64 projection[16] = {0};

    GLfloat  win_x, win_y, win_z;
    GLdouble pos_x, pos_y, pos_z;
 
    for (i32 i = 0; i < 16; ++i) projection[i]  = in_projection.e[i];
    for (i32 i = 0; i < 16; ++i) modelview[i]   = in_modelview.e[i];

    glGetIntegerv(GL_VIEWPORT, viewport);
 
    win_x = (f64)(x);
    win_y = (f64)(viewport[3]) - (f64)y;

    glReadPixels(x, (int)(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);
 
    f64 result[3];
    f4x4_unproject_64(result, win_x, win_y, win_z, modelview, projection, viewport);
 
    return v3(result[0], result[1], result[2]);
}

#endif // ATS_OGL33
#endif // ATS_IMPL

