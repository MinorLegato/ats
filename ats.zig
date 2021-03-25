const math = @import("std").math;

pub usingnamespace @cImport(@cInclude("GLFW/glfw3.h"));

pub fn rsqrt(n: anytype) f32 {
    return if (n == 0) 0 else 1.0 / @sqrt(n);
}

pub fn add(a: anytype, b: @TypeOf(a)) @TypeOf(a) {
    var out: @TypeOf(a) = undefined;

    for (a) |_, i| out[i] = a[i] + b[i];
    
    return out;
}

pub fn sub(a: anytype, b: @TypeOf(a)) @TypeOf(a) {
    var out: @TypeOf(a) = undefined;

    for (a) |_, i| out[i] = a[i] - b[i];
    
    return out;
}

pub fn mul(a: anytype, b: @TypeOf(a)) @TypeOf(a) {
    var out: @TypeOf(a) = undefined;

    for (a) |_, i| out[i] = a[i] * b[i];
    
    return out;
}

pub fn div(a: anytype, b: @TypeOf(a)) @TypeOf(a) {
    var out: @TypeOf(a) = undefined;

    for (a) |_, i| out[i] = a[i] / b[i];
    
    return out;
}

pub fn scale(a: anytype, s: @TypeOf(a[0])) @TypeOf(a) {
    var out: @TypeOf(a) = undefined;

    for (a) |_, i| out[i] = a[i] * s;
    
    return out;
}

pub fn dot(a: anytype, b: @TypeOf(a)) @TypeOf(a[0]) {
    var d: @TypeOf(a[0]) = 0;

    for (a) |_, i| d += a[i] * b[i];

    return d;
}

pub fn lenSq(a: anytype) @TypeOf(a[0]) {
    return dot(a, a);
}

pub fn len(a: anytype) @TypeOf(a[0]) {
    return @sqrt(dot(a, a));
}

pub fn norm(a: anytype) @TypeOf(a) {
    var k               = rsqrt(dot(a, a));
    var out: @TypeOf(a) = undefined;

    for (out) |_, i| out[i] = a[i] * k;

    return out;
}

pub fn cross(a: anytype, b: @TypeOf(a)) @TypeOf(a) {
    return .{
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0],
    };
}

pub fn perspective(y_fov: f32, aspect: f32, n: f32, f: f32) [16]f32 {
    var a: f32 = 1.0 / math.tan(y_fov / 2.0);

    return .{
        a / aspect,     0.0,        0.0,                            0.0,
        0.0,            a,          0.0,                            0.0,
        0.0,            0.0,        -((f + n) / (f - n)),           -1.0,
        0.0,            0.0,        -((2.0 * f * n) / (f - n)),     0.0,
    };
}

pub fn lookAt(eye: [3]f32, center: [3]f32, up: [3]f32) [16]f32 {
    var f = norm(sub(center, eye));
    var s = norm(cross(f, up));
    var t = cross(s, f);

    var out: [16]f32 = undefined;

    out[0]  =  s[0];
    out[1]  =  t[0];
    out[2]  = -f[0];
    out[3]  =  0.0;

    out[4]  =  s[1];
    out[5]  =  t[1];
    out[6]  = -f[1];
    out[7]  =  0.0;

    out[8]  =  s[2];
    out[9]  =  t[2];
    out[10] = -f[2];
    out[11] =  0.0;

    out[12] = -(out[0] * eye[0] + out[4] * eye[1] + out[8]  * eye[2]);
    out[13] = -(out[1] * eye[0] + out[5] * eye[1] + out[9]  * eye[2]);
    out[14] = -(out[2] * eye[0] + out[6] * eye[1] + out[10] * eye[2]);
    out[15] = -(out[3] * eye[0] + out[7] * eye[1] + out[11] * eye[2] - 1.0);

    return out;
}

pub const platform = struct {
    pub var close = false;

    pub const window = struct {
        pub var handle:     ?*GLFWwindow    = undefined;
        pub var width:      i32             = 0;
        pub var height:     i32             = 0;
        pub var aspect:     f32             = 0;
    };

    pub const mouse = struct {
        pub var pos         = [_]f32 { 0, 0 };
        pub var delta       = [_]f32 { 0, 0 };
        pub var scroll      = [_]f32 { 0, 0 };

        pub var down        = [_]bool { false } ** (MOUSE_BUTTON_LAST + 1);
        pub var pressed     = [_]bool { false } ** (MOUSE_BUTTON_LAST + 1);
        pub var released    = [_]bool { false } ** (MOUSE_BUTTON_LAST + 1);
    };

    pub const keyboard = struct {
        pub var down       = [_]bool { false } ** (KEY_LAST + 1);
        pub var pressed    = [_]bool { false } ** (KEY_LAST + 1);
        pub var released   = [_]bool { false } ** (KEY_LAST + 1);
    };

    pub const time = struct {
        pub var total:  f32 = 0;
        pub var delta:  f32 = 0;
    };

    pub fn init(title: anytype, width: i32, height: i32) void {
        if (glfwInit() == 0) return;

        window.handle = glfwCreateWindow(width, height, title, null, null);
        glfwMakeContextCurrent(window.handle);

        _ = glfwSetKeyCallback(window.handle, keyCallback);
        _ = glfwSetMouseButtonCallback(window.handle, mouseButtonCallback);
        _ = glfwSetScrollCallback(window.handle, mouseScrollCallback);

        glfwSetTime(0.0);

        update();
    }

    pub fn update() void {
        glfwGetWindowSize(window.handle, &window.width, &window.height);
        glViewport(0, 0, window.width, window.height);

        window.aspect = @intToFloat(f32, window.width) / @intToFloat(f32, window.height);

        if (glfwWindowShouldClose(window.handle) == GLFW_TRUE) {
            close = true;
        }

        if (close) {
            glfwSetWindowShouldClose(window.handle, GLFW_TRUE);
        }

        {
            var x: f64 = 0;
            var y: f64 = 0;

            glfwGetCursorPos(window.handle, &x, &y);

            mouse.delta     = .{ @floatCast(f32, x) - mouse.pos[0], @floatCast(f32, y) - mouse.pos[1] };
            mouse.pos       = .{ @floatCast(f32, x), @floatCast(f32, y) };
            mouse.scroll    = .{ 0, 0 };
        }

        keyboard.pressed    = [_]bool { false } ** (KEY_LAST + 1);
        keyboard.released   = [_]bool { false } ** (KEY_LAST + 1);

        mouse.pressed       = [_]bool { false } ** (MOUSE_BUTTON_LAST + 1);
        mouse.released      = [_]bool { false } ** (MOUSE_BUTTON_LAST + 1);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }
};

fn keyCallback(window: ?*GLFWwindow, key: c_int, a: c_int, action: c_int, b: c_int) callconv(.C) void {
    switch (action) {
        GLFW_PRESS => { 
            platform.keyboard.down[@intCast(usize, key)]     = true;
            platform.keyboard.pressed[@intCast(usize, key)]  = true;
        },
        GLFW_RELEASE => {
            platform.keyboard.down[@intCast(usize, key)]     = false;
            platform.keyboard.released[@intCast(usize, key)] = true;
        },
        else => {},
    }
}

fn mouseButtonCallback(window: ?*GLFWwindow, button: c_int, action: c_int, a: c_int) callconv(.C) void {
    switch (action) {
        GLFW_PRESS => {
            platform.mouse.down[@intCast(usize, button)]     = true;
            platform.mouse.pressed[@intCast(usize, button)]  = true;
        },
        GLFW_RELEASE => {
            platform.mouse.down[@intCast(usize, button)]     = false;
            platform.mouse.released[@intCast(usize, button)] = true;
        },
        else => {},
    }
}

fn mouseScrollCallback(window: ?*GLFWwindow, x_offset: f64, y_offset: f64) callconv(.C) void {
    platform.mouse.scroll[0] = @floatCast(f32, x_offset);
    platform.mouse.scroll[1] = @floatCast(f32, y_offset);
}

// ===================================================== KEYS =================================================== //

pub const KEY_UNKNOWN            = -1;

pub const KEY_SPACE              = 32;
pub const KEY_APOSTROPHE         = 39;
pub const KEY_COMMA              = 44;
pub const KEY_MINUS              = 45;
pub const KEY_PERIOD             = 46;
pub const KEY_SLASH              = 47;
pub const KEY_0                  = 48;
pub const KEY_1                  = 49;
pub const KEY_2                  = 50;
pub const KEY_3                  = 51;
pub const KEY_4                  = 52;
pub const KEY_5                  = 53;
pub const KEY_6                  = 54;
pub const KEY_7                  = 55;
pub const KEY_8                  = 56;
pub const KEY_9                  = 57;
pub const KEY_SEMICOLON          = 59;
pub const KEY_EQUAL              = 61;
pub const KEY_A                  = 65;
pub const KEY_B                  = 66;
pub const KEY_C                  = 67;
pub const KEY_D                  = 68;
pub const KEY_E                  = 69;
pub const KEY_F                  = 70;
pub const KEY_G                  = 71;
pub const KEY_H                  = 72;
pub const KEY_I                  = 73;
pub const KEY_J                  = 74;
pub const KEY_K                  = 75;
pub const KEY_L                  = 76;
pub const KEY_M                  = 77;
pub const KEY_N                  = 78;
pub const KEY_O                  = 79;
pub const KEY_P                  = 80;
pub const KEY_Q                  = 81;
pub const KEY_R                  = 82;
pub const KEY_S                  = 83;
pub const KEY_T                  = 84;
pub const KEY_U                  = 85;
pub const KEY_V                  = 86;
pub const KEY_W                  = 87;
pub const KEY_X                  = 88;
pub const KEY_Y                  = 89;
pub const KEY_Z                  = 90;
pub const KEY_LEFT_BRACKET       = 91;
pub const KEY_BACKSLASH          = 92;
pub const KEY_RIGHT_BRACKET      = 93;
pub const KEY_GRAVE_ACCENT       = 96;
pub const KEY_WORLD_1            = 16;
pub const KEY_WORLD_2            = 16;

pub const KEY_ESCAPE             = 256;
pub const KEY_ENTER              = 257;
pub const KEY_TAB                = 258;
pub const KEY_BACKSPACE          = 259;
pub const KEY_INSERT             = 260;
pub const KEY_DELETE             = 261;
pub const KEY_RIGHT              = 262;
pub const KEY_LEFT               = 263;
pub const KEY_DOWN               = 264;
pub const KEY_UP                 = 265;
pub const KEY_PAGE_UP            = 266;
pub const KEY_PAGE_DOWN          = 267;
pub const KEY_HOME               = 268;
pub const KEY_END                = 269;
pub const KEY_CAPS_LOCK          = 280;
pub const KEY_SCROLL_LOCK        = 281;
pub const KEY_NUM_LOCK           = 282;
pub const KEY_PRINT_SCREEN       = 283;
pub const KEY_PAUSE              = 284;
pub const KEY_F1                 = 290;
pub const KEY_F2                 = 291;
pub const KEY_F3                 = 292;
pub const KEY_F4                 = 293;
pub const KEY_F5                 = 294;
pub const KEY_F6                 = 295;
pub const KEY_F7                 = 296;
pub const KEY_F8                 = 297;
pub const KEY_F9                 = 298;
pub const KEY_F10                = 299;
pub const KEY_F11                = 300;
pub const KEY_F12                = 301;
pub const KEY_F13                = 302;
pub const KEY_F14                = 303;
pub const KEY_F15                = 304;
pub const KEY_F16                = 305;
pub const KEY_F17                = 306;
pub const KEY_F18                = 307;
pub const KEY_F19                = 308;
pub const KEY_F20                = 309;
pub const KEY_F21                = 310;
pub const KEY_F22                = 311;
pub const KEY_F23                = 312;
pub const KEY_F24                = 313;
pub const KEY_F25                = 314;
pub const KEY_KP_0               = 320;
pub const KEY_KP_1               = 321;
pub const KEY_KP_2               = 322;
pub const KEY_KP_3               = 323;
pub const KEY_KP_4               = 324;
pub const KEY_KP_5               = 325;
pub const KEY_KP_6               = 326;
pub const KEY_KP_7               = 327;
pub const KEY_KP_8               = 328;
pub const KEY_KP_9               = 329;
pub const KEY_KP_DECIMAL         = 330;
pub const KEY_KP_DIVIDE          = 331;
pub const KEY_KP_MULTIPLY        = 332;
pub const KEY_KP_SUBTRACT        = 333;
pub const KEY_KP_ADD             = 334;
pub const KEY_KP_ENTER           = 335;
pub const KEY_KP_EQUAL           = 336;
pub const KEY_LEFT_SHIFT         = 340;
pub const KEY_LEFT_CONTROL       = 341;
pub const KEY_LEFT_ALT           = 342;
pub const KEY_LEFT_SUPER         = 343;
pub const KEY_RIGHT_SHIFT        = 344;
pub const KEY_RIGHT_CONTROL      = 345;
pub const KEY_RIGHT_ALT          = 346;
pub const KEY_RIGHT_SUPER        = 347;
pub const KEY_MENU               = 348;
pub const KEY_LAST               = KEY_MENU;

pub const MOUSE_BUTTON_1         = 0;
pub const MOUSE_BUTTON_2         = 1;
pub const MOUSE_BUTTON_3         = 2;
pub const MOUSE_BUTTON_4         = 3;
pub const MOUSE_BUTTON_5         = 4;
pub const MOUSE_BUTTON_6         = 5;
pub const MOUSE_BUTTON_7         = 6;
pub const MOUSE_BUTTON_8         = 7;
pub const MOUSE_BUTTON_LAST      = MOUSE_BUTTON_8;
pub const MOUSE_BUTTON_LEFT      = MOUSE_BUTTON_1;
pub const MOUSE_BUTTON_RIGHT     = MOUSE_BUTTON_2;
pub const MOUSE_BUTTON_MIDDLE    = MOUSE_BUTTON_3;

pub const JOYSTICK_1             = 0;
pub const JOYSTICK_2             = 1;
pub const JOYSTICK_3             = 2;
pub const JOYSTICK_4             = 3;
pub const JOYSTICK_5             = 4;
pub const JOYSTICK_6             = 5;
pub const JOYSTICK_7             = 6;
pub const JOYSTICK_8             = 7;
pub const JOYSTICK_9             = 8;
pub const JOYSTICK_10            = 9;
pub const JOYSTICK_11            = 10;
pub const JOYSTICK_12            = 11;
pub const JOYSTICK_13            = 12;
pub const JOYSTICK_14            = 13;
pub const JOYSTICK_15            = 14;
pub const JOYSTICK_16            = 15;
pub const JOYSTICK_LAST          = JOYSTICK_16;

pub const GAMEPAD_BUTTON_A               = 0;
pub const GAMEPAD_BUTTON_B               = 1;
pub const GAMEPAD_BUTTON_X               = 2;
pub const GAMEPAD_BUTTON_Y               = 3;
pub const GAMEPAD_BUTTON_LEFT_BUMPER     = 4;
pub const GAMEPAD_BUTTON_RIGHT_BUMPER    = 5;
pub const GAMEPAD_BUTTON_BACK            = 6;
pub const GAMEPAD_BUTTON_START           = 7;
pub const GAMEPAD_BUTTON_GUIDE           = 8;
pub const GAMEPAD_BUTTON_LEFT_THUMB      = 9;
pub const GAMEPAD_BUTTON_RIGHT_THUMB     = 10;
pub const GAMEPAD_BUTTON_DPAD_UP         = 11;
pub const GAMEPAD_BUTTON_DPAD_RIGHT      = 12;
pub const GAMEPAD_BUTTON_DPAD_DOWN       = 13;
pub const GAMEPAD_BUTTON_DPAD_LEFT       = 14;
pub const GAMEPAD_BUTTON_LAST            = GAMEPAD_BUTTON_DPAD_LEFT;

pub const GAMEPAD_BUTTON_CROSS       = GAMEPAD_BUTTON_A;
pub const GAMEPAD_BUTTON_CIRCLE      = GAMEPAD_BUTTON_B;
pub const GAMEPAD_BUTTON_SQUARE      = GAMEPAD_BUTTON_X;
pub const GAMEPAD_BUTTON_TRIANGLE    = GAMEPAD_BUTTON_Y;

pub const GAMEPAD_AXIS_LEFT_X        = 0;
pub const GAMEPAD_AXIS_LEFT_Y        = 1;
pub const GAMEPAD_AXIS_RIGHT_X       = 2;
pub const GAMEPAD_AXIS_RIGHT_Y       = 3;
pub const GAMEPAD_AXIS_LEFT_TRIGGER  = 4;
pub const GAMEPAD_AXIS_RIGHT_TRIGGER = 5;
pub const GAMEPAD_AXIS_LAST          = GAMEPAD_AXIS_RIGHT_TRIGGER;

