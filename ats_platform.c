
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma comment(lib, "ats/lib/glfw3_mt.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#endif

#ifdef ATS_OGL33
#include "ext/glad/glad.c"
#endif

#define GLFW_EXPOSE_NATIVE_WIN32
#include "ext/GLFW/glfw3native.h"

struct platform platform;

static struct {
    GLFWwindow* window;
    GLFWmonitor* monitor;
} platform_internal;

static void
window_key_callback(GLFWwindow* window, int key, int a, int action, int b) {
    (void)window;
    (void)a;
    (void)b;

    switch (action) {
        case GLFW_PRESS: {
            platform.keyboard.key = key;
            platform.keyboard.is_down = 1;
            platform.keyboard.is_pressed = 1;
            platform.keyboard.is_repeat = 1;
            platform.keyboard.down[key] = 1;
            platform.keyboard.pressed[key] = 1;
            platform.keyboard.repeat[key] = 1;
        } break;
        case GLFW_REPEAT: {
            platform.keyboard.is_repeat = 1;
            platform.keyboard.repeat[key] = 1;
        } break;
        case GLFW_RELEASE: {
            platform.keyboard.is_down = 0;
            platform.keyboard.is_released = 1;
            
            platform.keyboard.down[key] = 0;
            platform.keyboard.released[key] = 1;
        } break;
    }
}

static void
window_char_callback(GLFWwindow* window, unsigned int codepoint) {
    platform.keyboard.is_ascii  = 1;
    platform.keyboard.ascii     = codepoint;
}

static void
window_mouse_button_callback(GLFWwindow* window, int button, int action, int a) {
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

static void
window_scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
    (void)window;

    platform.mouse.scroll.x = (f32)xoffset;
    platform.mouse.scroll.y = (f32)yoffset;
}

static void
window_joystick_callback(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

extern void
platform_init(const char* title, int width, int height, int samples) {
    glfwInit();

    platform_internal.monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(platform_internal.monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#if defined(ATS_OGL33)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.width  = width;
    platform.height = height;

    platform_internal.window = glfwCreateWindow(width, height, title, NULL, NULL);
    platform.native = glfwGetWin32Window(platform_internal.window);

    glfwSetWindowPos(platform_internal.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform_internal.window);

#if defined(ATS_OGL33)
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    platform_update();
}

extern void
platform_update(void) {
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
            case mouse_mode_normal: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } break;
            case mouse_mode_hidden: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            } break;
            case mouse_mode_disabled: {
                glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } break;
        }
    }

    // update gamepads:
    {
        GLFWgamepadstate state;

        for (int i = 0; i < JOYSTICK_LAST; ++i) {
            if (platform.gamepad[i].active) {
                gamepad_buttons old = platform.gamepad[i].down;

                platform.gamepad[i].down.data       = 0;
                platform.gamepad[i].pressed.data    = 0;
                platform.gamepad[i].released.data   = 0;

                glfwGetGamepadState(i, &state);

                platform.gamepad[i].left_stick.x    = +state.axes[GAMEPAD_AXIS_LEFT_X];
                platform.gamepad[i].left_stick.y    = -state.axes[GAMEPAD_AXIS_LEFT_Y];
                platform.gamepad[i].right_stick.x   = +state.axes[GAMEPAD_AXIS_RIGHT_X];
                platform.gamepad[i].right_stick.y   = -state.axes[GAMEPAD_AXIS_RIGHT_Y];

                platform.gamepad[i].left_trigger    = 0.5f * (state.axes[GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f);
                platform.gamepad[i].right_trigger   = 0.5f * (state.axes[GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f);

                if (state.buttons[GAMEPAD_BUTTON_X]) platform.gamepad[i].down.button.x = 1;
                if (state.buttons[GAMEPAD_BUTTON_A]) platform.gamepad[i].down.button.a = 1;
                if (state.buttons[GAMEPAD_BUTTON_B]) platform.gamepad[i].down.button.b = 1;
                if (state.buttons[GAMEPAD_BUTTON_Y]) platform.gamepad[i].down.button.y = 1;

                if (state.buttons[GAMEPAD_BUTTON_LEFT_BUMPER])  platform.gamepad[i].down.button.left_bumper     = 1;
                if (state.buttons[GAMEPAD_BUTTON_RIGHT_BUMPER]) platform.gamepad[i].down.button.right_bumper    = 1;

                if (platform.gamepad[i].left_trigger  > 0.0f)   platform.gamepad[i].down.button.left_trigger    = 1;
                if (platform.gamepad[i].right_trigger > 0.0f)   platform.gamepad[i].down.button.right_trigger   = 1;

                if (state.buttons[GAMEPAD_BUTTON_BACK])         platform.gamepad[i].down.button.select      = 1;
                if (state.buttons[GAMEPAD_BUTTON_START])        platform.gamepad[i].down.button.start       = 1;
                if (state.buttons[GAMEPAD_BUTTON_LEFT_THUMB])   platform.gamepad[i].down.button.left_stick  = 1;
                if (state.buttons[GAMEPAD_BUTTON_RIGHT_THUMB])  platform.gamepad[i].down.button.right_stick = 1;

                if (state.buttons[GAMEPAD_BUTTON_DPAD_UP])      platform.gamepad[i].down.button.up      = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_RIGHT])   platform.gamepad[i].down.button.right   = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_DOWN])    platform.gamepad[i].down.button.down    = 1;
                if (state.buttons[GAMEPAD_BUTTON_DPAD_LEFT])    platform.gamepad[i].down.button.left    = 1;

                platform.gamepad[i].pressed.data    =  platform.gamepad[i].down.data & ~old.data;
                platform.gamepad[i].released.data   = ~platform.gamepad[i].down.data &  old.data;
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

    platform.time.delta = (f32)(glfwGetTime() - platform.time.total);
    platform.time.total += platform.time.delta;
}

extern f64
timer_get_current(void) {
    return glfwGetTime();
}

extern gl_texture
gl_texture_create(void *pixels, int width, int height, int is_smooth) {
    assert(pixels);

    gl_texture texture = {0};

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

extern gl_texture
gl_texture_create_from_image(image image, int is_smooth) {
    return gl_texture_create(image.pixels, image.width, image.height, is_smooth);
}

extern gl_texture
gl_texture_load_from_file(const char* texture_path, int is_smooth) {
    image img = file_load_image(texture_path);
    gl_texture texture = gl_texture_create_from_image(img, is_smooth);
    file_free_image(&img);
    return texture;
}

extern void
gl_texture_update(gl_texture* texture, void *pixels, int width, int height, int is_smooth) {
    texture->width  = width;
    texture->height = height;

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, is_smooth ? GL_LINEAR : GL_NEAREST);

#ifdef ATS_OGL33
    glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

extern void
gl_texture_bind(const gl_texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id);

#if !defined(ATS_OGL33)
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f / texture->width, 1.0f / texture->height, 1.0f);
#endif
}

extern void
gl_texture_destroy(gl_texture* texture) {
    glDeleteTextures(1, &texture->id);
    memset(texture, 0, sizeof *texture);
}

#ifdef ATS_OGL33

internal u32
gl_shader_compile(const char* source, unsigned int type) {
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

internal u32
gl_shader_link_program(u32 vertex_shader, u32 fragment_shader) {
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

extern gl_shader
gl_shader_create(const gl_shader_desc* desc) {
    u32 vertex = gl_shader_compile(desc->vs, GL_VERTEX_SHADER);
    u32 fragment = gl_shader_compile(desc->fs, GL_FRAGMENT_SHADER);
    u32 program = gl_shader_link_program(vertex, fragment);

    glUseProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    gl_shader shader = {0};
    shader.id = program;
    return shader;
}

extern gl_shader
gl_shader_load_from_file(const char *vs, const char *fs,  memory_arena* ma) {
    ma_save(ma);
    char* vs_content = file_read_str(vs, ma);
    char* fs_content = file_read_str(fs, ma);
    gl_shader program = gl_shader_create(&(gl_shader_desc) {
        .vs = vs_content,
        .fs = fs_content,
    });
    ma_restore(ma);
    return program;
}

extern void
gl_use(const gl_shader* shader) {
    glUseProgram(shader->id);
}

extern u32
gl_location(const gl_shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}

extern void
gl_uniform_i32(u32 location, i32 i) {
    glUniform1i(location, i);
}

extern void
gl_uniform_f32(u32 location, f32 f) {
    glUniform1f(location, f);
}

extern void
gl_uniform_v2(u32 location, v2 u) {
    glUniform2f(location, u.x, u.y);
}

extern void
gl_uniform_v3(u32 location, v3 u) {
    glUniform3f(location, u.x, u.y, u.z);
}

extern void 
gl_uniform_v4(u32 location, v4 u) {
    glUniform4f(location, u.x, u.y, u.z, u.w);
}

extern void
gl_uniform_m2(u32 location, m2 m) {
    glUniformMatrix2fv(location, 1, GL_FALSE, m.e);
}

extern void
gl_uniform_m3(u32 location, m3 m) {
    glUniformMatrix3fv(location, 1, GL_FALSE, m.e);
}

extern void
gl_uniform_m4(u32 location, m4 m) {
    glUniformMatrix4fv(location, 1, GL_FALSE, m.e);
}

extern v3
gl_get_world_position(int x, int y, m4 in_projection, m4 in_modelview) {
    GLint viewport[4] = {0};
    f64 modelview[16] = {0};
    f64 projection[16] = {0};

    GLfloat win_x, win_y, win_z;
 
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

extern gl_array
gl_array_create(const gl_array_desc* desc) {
    u32 vao = 0;
    u32 vbo = 0;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (u32 i = 0; i < array_count(desc->layout); ++i) {
        const gl_layout* layout = &desc->layout[i];

        if (layout->size) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, layout->size, layout->type, layout->normalize, layout->stride, (void*)(u64)layout->offset);
        }
    }

    gl_array result = {0};

    result.vao = vao;
    result.vbo = vbo;

    return result;
}

extern void
gl_array_send(const gl_array* array, const void* data, u32 size) {
    glBindBuffer(GL_ARRAY_BUFFER, array->vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

#endif // ATS_OGL33

