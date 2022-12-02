
#ifdef ATS_OGL33
#include "ext/glad/glad.h"
#define GLSL(...) "#version 460 core\n" #__VA_ARGS__
#endif

#include "ext/GLFW/ats_glfw.h"

#ifdef ATS_OGL33
#include "ext/glad/glad.c"
#endif

struct platform platform;

static struct {
  GLFWwindow* window;
  GLFWmonitor* monitor;
} platform_internal;

static void window_key_callback(GLFWwindow* window, int key, int a, int action, int b) {
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

static void window_scroll_callback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
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

  platform.refresh_rate = mode->refreshRate;

#if defined(ATS_OGL33)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
      case MOUSE_MODE_NORMAL: {
        glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      } break;

      case MOUSE_MODE_HIDDEN: {
        glfwSetInputMode(platform_internal.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      } break;
      
      case MOUSE_MODE_DISABLED: {
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

extern f64 timer_get_current(void) {
  return glfwGetTime();
}
