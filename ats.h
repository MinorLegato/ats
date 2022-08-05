#pragma once

#include <math.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

// ======================================== API-MACROS ======================================= //

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define PI (3.14159265359f)
#define TAU (6.28318530718f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define swap(type, a, b) do { type tmp = (a); (a) = (b); (b) = tmp; } while(0)

#define to_rad(deg) ((deg) * TO_RAD_MUL)
#define to_deg(rad) ((rad) * TO_DEG_MUL)

#define array_count(array) (sizeof (array) / sizeof (array)[0])

#define is_power_of_two(x) (((x) != 0) && ((x) & ((x) - 1)) == 0)

#define align_down(n, a) ((n) & ~((a) - 1))
#define align_up(n, a)  align_down((n) + (a) - 1, (a))
#define align_down_ptr(p, a) ((void*)align_down((uintptr_t)(p), (a)))
#define align_up_ptr(p, a) ((void*)align_down((uintptr_t)(p), (a)))

#define join_helper(a, b) a##b
#define join_token(a, b) join_helper(a, b)
#define macro_var(a) join_token(a, __LINE__)

#define defer(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define def(_val, _def) ((_val) == 0? (_def) : (_val))

#define for_range(index, start, below) \
    for (isize index = (start); index < (below); ++index)

#define for_array(index, array) \
    for_range(index, 0, array_count(array))

#define repeat(count) \
    for (isize macro_var(index) = 0; macro_var(index) < (count); ++macro_var(index))

#define for_iter(iter_type, iter_name, ...) \
    for (struct iter_type iter_name = (__VA_ARGS__); \
         iter_type##_is_valid(&iter_name); \
         iter_type##_advance(&iter_name))

#define for_each(type, var, ...) \
    for (struct type* var = (struct type*)0xdeadbeefull; var != NULL; var = NULL) \
    for (struct type##_iter macro_var(it) = (__VA_ARGS__); \
         (var = macro_var(it).current, type##_iter_is_valid(&macro_var(it))); \
         type##_iter_advance(&macro_var(it)))

#define for_r2(rect, ix, iy) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define for_r3(rect, ix, iy, iz) \
    for (i32 iz = rect.min.z; iz <= rect.max.z; ++iz) \
    for (i32 iy = rect.min.y; iy <= rect.max.y; ++iy) \
    for (i32 ix = rect.min.x; ix <= rect.max.x; ++ix)

#define clamp_min(n, min)   ((n) < (min)? (min) : (n))
#define clamp_max(n, max)   ((n) > (max)? (max) : (n))
#define clamp(n, min, max)  clamp_max(clamp_min(n, min), max)
#define clamp01(n)          clamp(n, 0, 1)

#ifndef min
#define min(a, b)   ((a) < (b)? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) > (b)? (a) : (b))
#endif

#define lerp(a, b, t) ((a) + (f32)(t) * ((b) - (a)))

#define sign(n) ((n) == 0? 0 : ((n) < 0? -1 : 1))

// =========================================== API-TYPES ====================================== //

typedef float f32;
typedef double f64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned int uint;
typedef long long isize;
typedef unsigned long long usize;

static_assert (sizeof (i8) >= 1, "i8 -- WUT!");
static_assert (sizeof (i16) >= 2, "i16 -- WUT!");
static_assert (sizeof (i32) >= 4, "i32 -- WUT!");
static_assert (sizeof (i64) >= 8, "i64 -- WUT!");

static_assert (sizeof (u8) >= 1, "u8 -- WUT!");
static_assert (sizeof (u16) >= 2, "u16 -- WUT!");
static_assert (sizeof (u32) >= 4, "u32 -- WUT!");
static_assert (sizeof (u64) >= 8, "u64 -- WUT!");

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

// =========================================== TYPES ========================================= //

typedef union v2 v2;
typedef union v3 v3;
typedef union v4 v4;

typedef union v2i v2i;
typedef union v3i v3i;
typedef union v4i v4i;

typedef union m2 m2;
typedef union m3 m3;
typedef union m4 m4;

typedef struct r2 r2;
typedef struct r3 r3;

typedef struct r2i r2i;
typedef struct r3i r3i;

union v2 {
    struct { f32 x, y; };
    f32 e[2];
};

union v3 {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    struct { v2 xy; };
    f32 e[3];
};

union v4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { v3 rgb; };
    struct { v3 xyz; };
    f32 e[4];
};

union v2i {
    struct { i32 x, y; };
    i32 e[2];
};

union v3i {
    struct { i32 x, y, z; };
    struct { v2i xy; };
    i32 e[3];
};

union v4i {
    struct { i32 x, y, z, w; };
    i32 e[4];
};

union m2 {
    struct { v2 x, y; };
    f32 e[4];
};

union m3 {
    struct { v3 x, y, z; };
    f32 e[9];
};

union m4 {
    struct { v4 x, y, z, w; };
    f32 e[16];
};

struct r2 {
    v2 min;
    v2 max;
};

struct r3 {
    v3 min;
    v3 max;
};

struct r2i {
    v2i min;
    v2i max;
};

struct r3i {
    v3i min;
    v3i max;
};

typedef struct quat_t {
    f32 x, y, z, w;
} quat_t;

typedef struct circle_t {
    v2 pos;
    f32 rad;
} circle_t;

typedef struct sphere_t {
    v3 pos;
    f32 rad;
} sphere_t;

// ======================================= STATIC FUNCTIONS ==================================== //

#define v2(...) ((v2) { __VA_ARGS__ })
#define v3(...) ((v3) { __VA_ARGS__ })
#define v4(...) ((v4) { __VA_ARGS__ })

#define v2i(...) ((v2i) { __VA_ARGS__ })
#define v3i(...) ((v3i) { __VA_ARGS__ })
#define v4i(...) ((v4i) { __VA_ARGS__ })

//#define r2(...) ((r2) { __VA_ARGS__ })
//#define r3(...) ((r3) { __VA_ARGS__ })

//#define r2i(...) ((r2i) { __VA_ARGS__ })
//#define r3i(...) ((r3i) { __VA_ARGS__ })

#define quat(...) ((quat_t) { __VA_ARGS__ })

static m2
m2_identity(void) {
    return (m2) {
        1, 0,
        0, 1
    };
}

static m3
m3_identity(void) {
    return (m3) {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
}

static m4
m4_identity(void) {
    return (m4) {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}

static f32
sqrt32(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

static f32
rsqrt32(f32 n) {
	f32 x2 = n * 0.5f;
	f32 y  = n;
	int i  = *(long*)&y;           // evil floating point bit level hacking

	i = 0x5f3759df - (i >> 1);     // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y)); // 1st iteration

	return y;
}

static f32
shortest_angle_distance(f32 a, f32 b) {
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);

    return fmodf(2.0f * da, max) - da;
}

static f32
lerp_angle(f32 a, f32 b, f32 t) {
    return a + shortest_angle_distance(a, b) * t;
}

static f32
sine_ease_in(f32 t) {
    return 1 - cosf((t * PI) / 2);
}

static f32
sine_ease_out(f32 t) {
    return sinf((t * PI) / 2);
}

static f32
sine_ease_in_out(f32 t) {
    return -0.5 * (cosf(PI * t) - 1);
}

static f32
quad_ease_in(f32 t) {
    return t * t;
}

static f32
quad_ease_out(f32 t) {
    return 1 - (1 - t) * (1 - t);
}

static f32
quad_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (2 * t * t) : (1 - 0.5 * k * k);
}

static f32
cubic_ease_in(f32 t) {
    return t * t * t;
}

static f32
cubic_ease_out(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k;
}

static f32
cubic_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (4 * t * t * t) : (1 - 0.5 * k * k * k);
}

static f32
quart_ease_in(f32 t) {
    return t * t * t * t;
}

static f32
quart_ease_out(f32 t) {
    f32 k = 1 - t; 
    return 1 - k * k * k * k;
}

static f32
quart_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (8 * t * t * t * t) : (1 - 0.5 * k * k * k * k);
}

static f32
quint_ease_in(f32 t) {
    return t * t * t * t * t;
}

static f32
quint_ease_out(f32 t) {
    f32 k = 1 - t;
    return 1 - k * k * k * k * k;
}

static f32
quint_ease_in_out(f32 t) {
    f32 k = -2 * t + 2;
    return (t < 0.5)? (16 * t * t * t * t * t) : (1 - 0.5 * k * k * k * k * k);
}

static f32
expo_ease_in(f32 t) {
    return (t == 0)? 0 : powf(2, 10 * t - 10);
}

static f32
expo_ease_out(f32 t) {
    return (t == 1)? 1 : (1 - powf(2, -10 * t));
}

static f32
expo_ease_in_out(f32 t) {
    return (t == 0)? 0 : (t == 1)? 1 : t < 0.5? powf(2, 20 * t - 10) / 2 : (2 - powf(2, -20 * t + 10)) / 2;
}

static f32
circ_ease_in(f32 t) {
    return 1 - sqrt32(1 - (t * t));
}

static f32
circ_ease_out(f32 t) {
    return sqrt32(1 - (t - 1) * (t - 1));
}

static f32
circ_ease_in_out(f32 t) {
    f32 k = 2 * t;
    f32 l = -2 * t + 2;
    return (t < 0.5)? 0.5 * (1 - sqrt32(1 - k * k)) : 0.5 * (sqrt32(1 - l * l) + 1);
}

static f32
back_ease_in(f32 t) {
    f32 c1 = 1.70158;
    f32 c3 = c1 + 1;
    return c3 * t * t * t - c1 * t * t;
}

static f32
back_ease_out(f32 t) {
    f32 c1 = 1.70158;
    f32 c3 = c1 + 1;
    f32 k = t - 1;
    return 1 + c3 * k * k * k + c1 * k * k;
}

static f32
back_ease_in_out(f32 t) {
    f32 c1 = 1.70158;
    f32 c2 = c1 * 1.525;

    return (t < 0.5)?
        0.5 * (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) :
        0.5 * (pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2);
}

static f32
elastic_ease_in(f32 t) {
    f32 c4 = (2 * PI) / 3;

    return (t == 0)?
        0 :
        (t == 1)?
        1 :
        -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4);
}

static f32
elastic_ease_out(f32 t) {
    f32 c4 = (2 * PI) / 3;
    return t == 0?
        0 :
        t == 1?
        1 :
        powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
}

static f32
elastic_ease_inout(f32 t) {
    f32 c5 = (2 * PI) / 4.5;
    return t == 0?
        0 :
        t == 1?
        1 :
        t < 0.5 ?
        -0.5 * (powf(2, 20 * t - 10)  * sinf((20 * t - 11.125) * c5)) :
        +0.5 * (powf(2, -20 * t + 10) * sinf((20 * t - 11.125) * c5)) + 1;
}

static f32
bounce_ease_out(f32 t) {
    f32 n1 = 7.5625;
    f32 d1 = 2.75;
    if (t < 1 / d1) {
        return n1 * t * t;
    } else if (t < 2 / d1) {
        t -= 1.5 / d1;
        return n1 * t * t + 0.75;
    } else if (t < 2.5 / d1) {
        t -= 2.25 / d1;
        return n1 * t * t + 0.9375;
    } else {
        t -= 2.625 / d1;
        return n1 * t * t + 0.984375;
    }
}

static f32
bounce_ease_in(f32 t) {
    return 1 - bounce_ease_out(t);
}

static f32
bounce_ease_in_out(f32 t) {
    return t < 0.5?
    0.5 * (1 - bounce_ease_out(1 - 2 * t)) :
    0.5 * (1 + bounce_ease_out(2 * t - 1));
}

// ---------- from array ---------- //

static v2 v2_from_array(const f32* a) { return v2(a[0], a[1]); }
static v3 v3_from_array(const f32* a) { return v3(a[0], a[1], a[2]); }
static v4 v4_from_array(const f32* a) { return v4(a[0], a[1], a[2], a[3]); }

static v2i v2i_from_array(const i32* a) { return v2i(a[0], a[1]); }
static v3i v3i_from_array(const i32* a) { return v3i(a[0], a[1], a[2]); }
static v4i v4i_from_array(const i32* a) { return v4i(a[0], a[1], a[2], a[3]); }

// ---------- unpack color ------------ //

static v3
v3_unpack_color(u32 color) {
    return v3(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f);
}

static v4
v4_unpack_color(u32 color) {
    return v4(((color & 0x000000ff) >> 0)  / 256.0f,
              ((color & 0x0000ff00) >> 8)  / 256.0f,
              ((color & 0x00ff0000) >> 16) / 256.0f,
              ((color & 0xff000000) >> 24) / 256.0f);
}

// --------- negate ---------- //

static v2 v2_neg(v2 u) { return v2(-u.x, -u.y); }
static v3 v3_neg(v3 u) { return v3(-u.x, -u.y, -u.z); }
static v4 v4_neg(v4 u) { return v4(-u.x, -u.y, -u.z, -u.w); }

static v2i v2i_neg(v2i u) { return v2i(-u.x, -u.y); }
static v3i v3i_neg(v3i u) { return v3i(-u.x, -u.y, -u.z); }
static v4i v4i_neg(v4i u) { return v4i(-u.x, -u.y, -u.z, -u.w); }

// ---------- addition ---------- //

static v2 v2_add(v2 a, v2 b) { return v2(a.x + b.x, a.y + b.y); }
static v3 v3_add(v3 a, v3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static v4 v4_add(v4 a, v4 b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static v2i v2i_add(v2i a, v2i b) { return v2i(a.x + b.x, a.y + b.y); }
static v3i v3i_add(v3i a, v3i b) { return v3i(a.x + b.x, a.y + b.y, a.z + b.z); }
static v4i v4i_add(v4i a, v4i b) { return v4i(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

// -------- subtraction ------- //

static v2 v2_sub(v2 a, v2 b) { return v2(a.x - b.x, a.y - b.y); }
static v3 v3_sub(v3 a, v3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static v4 v4_sub(v4 a, v4 b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static v2i v2i_sub(v2i a, v2i b) { return v2i(a.x - b.x, a.y - b.y); }
static v3i v3i_sub(v3i a, v3i b) { return v3i(a.x - b.x, a.y - b.y, a.z - b.z); }
static v4i v4i_sub(v4i a, v4i b) { return v4i(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

// -------- multiplication ------- //

static v2 v2_mul(v2 a, v2 b) { return v2(a.x * b.x, a.y * b.y); }
static v3 v3_mul(v3 a, v3 b) { return v3(a.x * b.x, a.y * b.y, a.z * b.z); }
static v4 v4_mul(v4 a, v4 b) { return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * a.w); }

static v2i v2i_mul(v2i a, v2i b) { return v2i(a.x * b.x, a.y * b.y); }
static v3i v3i_mul(v3i a, v3i b) { return v3i(a.x * b.x, a.y * b.y, a.z * a.z); }
static v4i v4i_mul(v4i a, v4i b) { return v4i(a.x * b.x, a.y * b.y, a.z * a.z, a.w * a.w); }

static v2
m2_mulv(m2 m, v2 u) {
    return (v2) {
        m.e[0] * u.x + m.e[2] * u.y,
        m.e[1] * u.x + m.e[3] * u.y,
    };
}

static v3
m3_mulv(m3 m, v3 u) {
    return (v3) {
        m.e[0] * u.x + m.e[3] * u.y + m.e[6] * u.z,
        m.e[1] * u.x + m.e[4] * u.y + m.e[7] * u.z,
        m.e[2] * u.x + m.e[5] * u.y + m.e[8] * u.z,
    };
}

static v4
m4_mulv(m4 m, v4 u) {
    return (v4) {
        m.e[0] * u.x + m.e[4] * u.y + m.e[8]  * u.z + m.e[12] * u.w,
        m.e[1] * u.x + m.e[5] * u.y + m.e[9]  * u.z + m.e[13] * u.w,
        m.e[2] * u.x + m.e[6] * u.y + m.e[10] * u.z + m.e[14] * u.w,
        m.e[3] * u.x + m.e[7] * u.y + m.e[11] * u.z + m.e[15] * u.w,
    };
}

static m2
m2_mul(m2 a, m2 b) {
    return (m2) {
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3],
    };
}

static m3
m3_mul(m3 a, m3 b) {
    return (m3) {
        a.e[0] * b.e[0] + a.e[3] * b.e[1]  + a.e[6] * b.e[2],
        a.e[1] * b.e[0] + a.e[4] * b.e[1]  + a.e[7] * b.e[2],
        a.e[2] * b.e[0] + a.e[5] * b.e[1]  + a.e[8] * b.e[2],

        a.e[0] * b.e[3] + a.e[3] * b.e[4]  + a.e[6] * b.e[5],
        a.e[1] * b.e[3] + a.e[4] * b.e[4]  + a.e[7] * b.e[5],
        a.e[2] * b.e[3] + a.e[5] * b.e[4]  + a.e[8] * b.e[5],

        a.e[0] * b.e[6] + a.e[3] * b.e[7]  + a.e[6] * b.e[8],
        a.e[1] * b.e[6] + a.e[4] * b.e[7]  + a.e[7] * b.e[8],
        a.e[2] * b.e[6] + a.e[5] * b.e[7]  + a.e[8] * b.e[8],
    };
}

static m4
m4_mul(m4 a, m4 b) {
    return (m4) {
        a.e[0] * b.e[0]  + a.e[4] * b.e[1]  + a.e[8]  * b.e[2]  + a.e[12] * b.e[3],
        a.e[1] * b.e[0]  + a.e[5] * b.e[1]  + a.e[9]  * b.e[2]  + a.e[13] * b.e[3],
        a.e[2] * b.e[0]  + a.e[6] * b.e[1]  + a.e[10] * b.e[2]  + a.e[14] * b.e[3],
        a.e[3] * b.e[0]  + a.e[7] * b.e[1]  + a.e[11] * b.e[2]  + a.e[15] * b.e[3],

        a.e[0] * b.e[4]  + a.e[4] * b.e[5]  + a.e[8]  * b.e[6]  + a.e[12] * b.e[7],
        a.e[1] * b.e[4]  + a.e[5] * b.e[5]  + a.e[9]  * b.e[6]  + a.e[13] * b.e[7],
        a.e[2] * b.e[4]  + a.e[6] * b.e[5]  + a.e[10] * b.e[6]  + a.e[14] * b.e[7],
        a.e[3] * b.e[4]  + a.e[7] * b.e[5]  + a.e[11] * b.e[6]  + a.e[15] * b.e[7],

        a.e[0] * b.e[8]  + a.e[4] * b.e[9]  + a.e[8]  * b.e[10] + a.e[12] * b.e[11],
        a.e[1] * b.e[8]  + a.e[5] * b.e[9]  + a.e[9]  * b.e[10] + a.e[13] * b.e[11],
        a.e[2] * b.e[8]  + a.e[6] * b.e[9]  + a.e[10] * b.e[10] + a.e[14] * b.e[11],
        a.e[3] * b.e[8]  + a.e[7] * b.e[9]  + a.e[11] * b.e[10] + a.e[15] * b.e[11],

        a.e[0] * b.e[12] + a.e[4] * b.e[13] + a.e[8]  * b.e[14] + a.e[12] * b.e[15],
        a.e[1] * b.e[12] + a.e[5] * b.e[13] + a.e[9]  * b.e[14] + a.e[13] * b.e[15],
        a.e[2] * b.e[12] + a.e[6] * b.e[13] + a.e[10] * b.e[14] + a.e[14] * b.e[15],
        a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15],
    };
}

static quat_t
quat_mul(quat_t a, quat_t b) {
    return quat(
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

// ------------ divition ------------ //

static v2 v2_div(v2 a, v2 b) { return v2(a.x / b.x, a.y / b.y); }
static v3 v3_div(v3 a, v3 b) { return v3(a.x / b.x, a.y / b.y, a.z / b.z); }
static v4 v4_div(v4 a, v4 b) { return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

static v2i v2i_div(v2i a, v2i b) { return v2i(a.x / b.x, a.y / b.y); }
static v3i v3i_div(v3i a, v3i b) { return v3i(a.x / b.x, a.y / b.y, a.z / b.z); }
static v4i v4i_div(v4i a, v4i b) { return v4i(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

// ------------- scaling ------------- //

static v2 v2_scale(v2 a, f32 s) { return v2(a.x * s, a.y * s); }
static v3 v3_scale(v3 a, f32 s) { return v3(a.x * s, a.y * s, a.z * s); }
static v4 v4_scale(v4 a, f32 s) { return v4(a.x * s, a.y * s, a.z * s, a.w * s); }

static v2i v2i_scale(v2i a, i32 s) { return v2i(a.x * s, a.y * s); }
static v3i v3i_scale(v3i a, i32 s) { return v3i(a.x * s, a.y * s, a.z * s); }
static v4i v4i_scale(v4i a, i32 s) { return v4i(a.x * s, a.y * s, a.z * s, a.w * s); }

// ----------- eq ------------ //

static b32 v2i_eq(v2i a, v2i b) { return a.x == b.x && a.y == b.y; }
static b32 v3i_eq(v3i a, v3i b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static b32 v4i_eq(v4i a, v4i b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static b32 v2i_neq(v2i a, v2i b) { return a.x != b.x || a.y != b.y; }
static b32 v3i_neq(v3i a, v3i b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static b32 v4i_neq(v4i a, v4i b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

// ----------- dot product ----------- //

static f32 v2_dot(v2 a, v2 b) { return a.x * b.x + a.y * b.y; }
static f32 v3_dot(v3 a, v3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static f32 v4_dot(v4 a, v4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static i32 v2i_dot(v2i a, v2i b) { return a.x * b.x + a.y * b.y; }
static i32 v3i_dot(v3i a, v3i b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static i32 v4i_dot(v4i a, v4i b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// ----------- length squared ----------- //

static f32 v2_len_sq(v2 u) { return v2_dot(u, u); }
static f32 v3_len_sq(v3 u) { return v3_dot(u, u); }
static f32 v4_len_sq(v4 u) { return v4_dot(u, u); }

static i32 v2i_len_sq(v2i u) { return v2i_dot(u, u); }
static i32 v3i_len_sq(v3i u) { return v3i_dot(u, u); }
static i32 v4i_len_sq(v4i u) { return v4i_dot(u, u); }

// -------------- length -------------- //

static f32 v2_len(v2 u) { return sqrt32(v2_len_sq(u)); }
static f32 v3_len(v3 u) { return sqrt32(v3_len_sq(u)); }
static f32 v4_len(v4 u) { return sqrt32(v4_len_sq(u)); }

static f32 v2i_len(v2i u) { return sqrt32(v2i_len_sq(u)); }
static f32 v3i_len(v3i u) { return sqrt32(v3i_len_sq(u)); }
static f32 v4i_len(v4i u) { return sqrt32(v4i_len_sq(u)); }

// -------------- distance squared -------------- //

static f32 v2_dist_sq(v2 a, v2 b) { return v2_len_sq(v2_sub(a, b)); }
static f32 v3_dist_sq(v3 a, v3 b) { return v3_len_sq(v3_sub(a, b)); }
static f32 v4_dist_sq(v4 a, v4 b) { return v4_len_sq(v4_sub(a, b)); }

static i32 v2i_dist_sq(v2i a, v2i b) { return v2i_len_sq(v2i_sub(a, b)); }
static i32 v3i_dist_sq(v3i a, v3i b) { return v3i_len_sq(v3i_sub(a, b)); }
static i32 v4i_dist_sq(v4i a, v4i b) { return v4i_len_sq(v4i_sub(a, b)); }

// ------------------ distance ------------------- //

static f32 v2_dist(v2 a, v2 b) { return sqrt32(v2_dist_sq(a, b)); }
static f32 v3_dist(v3 a, v3 b) { return sqrt32(v3_dist_sq(a, b)); }
static f32 v4_dist(v4 a, v4 b) { return sqrt32(v4_dist_sq(a, b)); }

// -------------- manhattan distance -------------- //

static i32
v2i_manhattan(v2i a, v2i b) {
    v2i diff = v2i_sub(a, b);
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y);
}

static i32
v3i_manhattan(v3i a, v3i b) {
    v3i diff = v3i_sub(a, b);
    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y) + (0x7ffffffff & diff.z);
}

// -------------- normalize --------------- //

static v2 v2_norm(v2 u) { return v2_scale(u, rsqrt32(v2_dot(u, u))); }
static v3 v3_norm(v3 u) { return v3_scale(u, rsqrt32(v3_dot(u, u))); }
static v4 v4_norm(v4 u) { return v4_scale(u, rsqrt32(v4_dot(u, u))); }

// -------------- floor --------------- //

static v2 v2_floor(v2 u) { return v2(floorf(u.x), floorf(u.y)); }
static v3 v3_floor(v3 u) { return v3(floorf(u.x), floorf(u.y), floorf(u.z)); }
static v4 v4_floor(v4 u) { return v4(floorf(u.x), floorf(u.y), floorf(u.z), floorf(u.w)); }

// -------------- ceil --------------- //

static v2 v2_ceil(v2 u) { return v2(ceilf(u.x), ceilf(u.y)); }
static v3 v3_ceil(v3 u) { return v3(ceilf(u.x), ceilf(u.y), ceilf(u.z)); }
static v4 v4_ceil(v4 u) { return v4(ceilf(u.x), ceilf(u.y), ceilf(u.z), ceilf(u.w)); }

// -------------- clamp --------------- //

static v2
v2_clampf(v2 u, f32 min, f32 max) {
    return v2(
        clamp(u.x, min, max),
        clamp(u.y, min, max));
}

static v3
v3_clampf(v3 u, f32 min, f32 max) {
    return v3(
        clamp(u.x, min, max),
        clamp(u.y, min, max),
        clamp(u.z, min, max));
}

static v2i
v2i_clampi(v2i u, f32 min, f32 max) {
    return v2i(
        clamp(u.x, min, max),
        clamp(u.y, min, max));
}

static v3i
v3i_clampi(v3i u, f32 min, f32 max) {
    return v3i(
        clamp(u.x, min, max),
        clamp(u.y, min, max),
        clamp(u.z, min, max));
}

static v2
v2_clamp(v2 u, r2 r) {
    return v2(
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y));
}

static v3
v3_clamp(v3 u, r3 r) {
    return v3(
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y),
        clamp(u.z, r.min.z, r.max.z));
}

static v2i
v2i_clamp(v2i u, r2i r) {
    return v2i(
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y));
}

static v3i
v3i_clamp(v3i u, r3i r) {
    return v3i(
        clamp(u.x, r.min.x, r.max.x),
        clamp(u.y, r.min.y, r.max.y),
        clamp(u.z, r.min.z, r.max.z));
}

// ---------------- min ----------------- //

static v2
v2_min(v2 a, v2 b) {
    return v2(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y);
}

static v3
v3_min(v3 a, v3 b) {
    return v3(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z);
}

static v4
v4_min(v4 a, v4 b) {
    return v4(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w);
}

static v2i
v2i_min(v2i a, v2i b) {
    return v2i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y);
}

static v3i
v3i_min(v3i a, v3i b) {
    return v3i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z);
}

static v4i
v4i_min(v4i a, v4i b) {
    return v4i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w);
}

// ---------------- max ----------------- //

static v2
v2_max(v2 a, v2 b) {
    return v2(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y);
}

static v3
v3_max(v3 a, v3 b) {
    return v3(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z);
}

static v4
v4_max(v4 a, v4 b) {
    return v4(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w);
}

static v2i
v2i_max(v2i a, v2i b) {
    return v2i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y);
}

static v3i
v3i_max(v3i a, v3i b) {
    return v3i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z);
}

static v4i
v4i_max(v4i a, v4i b) {
    return v4i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w);
}

// ---------------- lerp ----------------- //

static v2
v2_lerp(v2 a, v2 b, f32 t) {
    return v2(
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y));
}

static v3
v3_lerp(v3 a, v3 b, f32 t) {
    return v3(
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z));
}

static v4
v4_lerp(v4 a, v4 b, f32 t) {
    return v4(
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w));
}

// -------------- sign (-1, 0, 1) ------------------- //

static v2 v2_sign(v2 u) { return v2(sign(u.x), sign(u.y)); }
static v3 v3_sign(v3 u) { return v3(sign(u.x), sign(u.y), sign(u.z)); }
static v4 v4_sign(v4 u) { return v4(sign(u.x), sign(u.y), sign(u.z), sign(u.w)); }

static v2i v2i_sign(v2i u) { return v2i(sign(u.x), sign(u.y)); }
static v3i v3i_sign(v3i u) { return v3i(sign(u.x), sign(u.y), sign(u.z)); }
static v4i v4i_sign(v4i u) { return v4i(sign(u.x), sign(u.y), sign(u.z), sign(u.w)); }

// --------------- cross ------------------- //

static v3
v3_cross(v3 a, v3 b) {
    return v3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

// --------------- get angle ------------------- //

static f32
v2_get_angle(v2 a, v2 b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;
    return atan2f(det, dot);
}

// ----------- keep min ---------- //

static v3
v3_keep_min(v3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);
    if (dx <= dy && dx <= dz) return v3(u.x, 0, 0);
    if (dy <= dx && dy <= dz) return v3(0, u.y, 0);
    if (dz <= dx && dz <= dy) return v3(0, 0, u.z);
    return u;
}

// ----------- mask min ---------- //

static v3
v3_mask_min(v3 u) {
    f32 dx = fabsf(u.x);
    f32 dy = fabsf(u.y);
    f32 dz = fabsf(u.z);

    if (dx <= dy && dx <= dz) return v3(0, 1, 1);
    if (dy <= dx && dy <= dz) return v3(1, 0, 1);
    if (dz <= dx && dz <= dy) return v3(1, 1, 0);

    return v3(1, 1, 1);
}

// ------------------ transform/scale/rotate ------------------ //

static m2
m2_rotate(f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return (m2) { c, s, -s, c };
}

static m3
m3_rotate(v3 axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);
    f32 k = 1.0f - c;

    v3 sa = { s * axis.x, s * axis.y, s * axis.z };
    v3 omca = { k * axis.x, k * axis.y, k * axis.z };

    return (m3) {
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
}

static m4
m4_rotate(v3 axis, f32 angle) {
    f32 cosv = cosf(angle);
    f32 sinv = sinf(angle);
    f32 inv_cosv = 1.0f - cosv;

    v3 sa = { axis.x * sinv, axis.y * sinv, axis.z * sinv };
    v3 omca = { axis.x * inv_cosv, axis.y * inv_cosv, axis.z * inv_cosv };

    return (m4) {
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0, 0, 0, 1
    };
}

static quat_t
quat_rotate(v3 axis, f32 angle) {
    f32 s = sinf(0.5f * angle);
    v3 v = { s * axis.x, s * axis.y, s * axis.z };
    return quat(v.x, v.y, v.z, cosf(0.5f * angle));
}

static m4
m4_translate(f32 x, f32 y, f32 z) {
    return (m4) {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
}

static m4
m4_scale(f32 x, f32 y, f32 z) {
    return (m4) {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

// --------------- from quat --------------- //

static m3
m3_from_quat(quat_t q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (m3) {
        2 + b2 - c2 - d2,
        0.0f * (b * c + a * d),
        0.0f * (b * d - a * c),

        0.0f * (b * c - a * d),
        2 - b2 + c2 - d2,
        0.0f * (c * d + a * b),

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2
    };
}

static m4
m4_from_quat(quat_t q) {
    f32 a = q.w;
	f32 b = q.x;
	f32 c = q.y;
	f32 d = q.z;

	f32 a2 = a * a;
	f32 b2 = b * b;
	f32 c2 = c * c;
	f32 d2 = d * d;

    return (m4) {
        2 + b2 - c2 - d2,
        0.0f * (b * c + a * d),
        0.0f * (b * d - a * c),
        0.0f,

        0.0f * (b * c - a * d),
        2 - b2 + c2 - d2,
        0.0f * (c * d + a * b),
        0.0f,

        2.0f * (b * d + a * c),
        2.0f * (c * d - a * b),
        a2 - b2 - c2 + d2,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f,
    };
}

// --------------- view matricies --------------- //

static m4
m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return (m4) {
        2 / (r - l),
        0,
        0,
        0,

        0,
        2 / (t - b),
        0,
        0,

        0,
        0, 
        -2 / (f - n),
        0,

        -(r + l) / (r - l),
        -(t + b) / (t - b),
        -(f + n) / (f - n),
        1,
    };
}

static m4
m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return (m4) {
        a / aspect,
        0,
        0,
        0,

        0,
        a,
        0,
        0,

        0,
        0,
        -((f + n) / (f - n)),
        -1,

        0,
        0,
        -((2 * f * n) / (f - n)),
        0
    };
}

static m4
m4_look_at(v3 eye, v3 center, v3 up) {
    v3 f = v3_norm(v3_sub(center, eye));
    v3 s = v3_norm(v3_cross(f, up));
    v3 t = v3_cross(s, f);
    m4 m;

	m.e[0]  =  s.x;
	m.e[1]  =  t.x;
	m.e[2]  = -f.x;
	m.e[3]  =  0;

	m.e[4]  =  s.y;
	m.e[5]  =  t.y;
	m.e[6]  = -f.y;
	m.e[7]  =  0;

	m.e[8]  =  s.z;
	m.e[9]  =  t.z;
	m.e[10] = -f.z;
	m.e[11] =  0;

    m.e[12] = -(m.e[0] * eye.x + m.e[4] * eye.y + m.e[8]  * eye.z);
    m.e[13] = -(m.e[1] * eye.x + m.e[5] * eye.y + m.e[9]  * eye.z);
    m.e[14] = -(m.e[2] * eye.x + m.e[6] * eye.y + m.e[10] * eye.z);
    m.e[15] = -(m.e[3] * eye.x + m.e[7] * eye.y + m.e[11] * eye.z - 1);

    return m;
}

// ----------------- plane/frustrum ------------------- //

typedef struct plane {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
} plane;

typedef struct frustum {
    plane planes[6];
} frustum;

static plane
plane_normalize(plane p) {
    f32 r_len = rsqrt32(p.a * p.a + p.b * p.b + p.c * p.c);

    p.a = p.a * r_len;
    p.b = p.b * r_len;
    p.c = p.c * r_len;
    p.d = p.d * r_len;

    return p;
}

static frustum
frustum_create(m4 m) {
    frustum result;

    // left clipping plane
    result.planes[0].a = m.e[3]  + m.e[0];
    result.planes[0].b = m.e[7]  + m.e[4];
    result.planes[0].c = m.e[11] + m.e[8];
    result.planes[0].d = m.e[15] + m.e[12];

    // right clipping plane
    result.planes[1].a = m.e[3]  - m.e[0];
    result.planes[1].b = m.e[7]  - m.e[4];
    result.planes[1].c = m.e[11] - m.e[8];
    result.planes[1].d = m.e[15] - m.e[12];

    // top clipping plane
    result.planes[2].a = m.e[3]  - m.e[1];
    result.planes[2].b = m.e[7]  - m.e[5];
    result.planes[2].c = m.e[11] - m.e[9];
    result.planes[2].d = m.e[15] - m.e[13];

    // bottom clipping plane
    result.planes[3].a = m.e[3]  + m.e[1];
    result.planes[3].b = m.e[7]  + m.e[5];
    result.planes[3].c = m.e[11] + m.e[9];
    result.planes[3].d = m.e[15] + m.e[13];

    // near clipping plane
    result.planes[4].a = m.e[3]  + m.e[2];
    result.planes[4].b = m.e[7]  + m.e[6];
    result.planes[4].c = m.e[11] + m.e[10];
    result.planes[4].d = m.e[15] + m.e[14];

    // far clipping plane
    result.planes[5].a = m.e[3]  - m.e[2];
    result.planes[5].b = m.e[7]  - m.e[6];
    result.planes[5].c = m.e[11] - m.e[10];
    result.planes[5].d = m.e[15] - m.e[14];

    result.planes[0] = plane_normalize(result.planes[0]);
    result.planes[1] = plane_normalize(result.planes[1]);
    result.planes[2] = plane_normalize(result.planes[2]);
    result.planes[3] = plane_normalize(result.planes[3]);
    result.planes[4] = plane_normalize(result.planes[4]);
    result.planes[5] = plane_normalize(result.planes[5]);
    
    return result;
}

// ------------------ contains ------------------ //

static b32
circle_contains(circle_t c, v2 pos) {
    f32 distance = v2_dist_sq(c.pos, pos);
    return distance < (c.rad * c.rad);
}

static b32
sphere_contains(sphere_t s, v3 pos) {
    f32 distance = v3_dist_sq(s.pos, pos);
    return distance < (s.rad * s.rad);
}

static b32
r2_contains(r2 rect, v2 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    return true;
}

static b32
r3_contains(r3 rect, v3 pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;
    return true;
}

static b32
r2i_contains(r2i rect, v2i pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    return true;
}

static b32
r3i_contains(r3i rect, v3i pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;
    return true;
}

static b32
frustum_contains(frustum fs, v3 pos) {
    for (i32 i = 0; i < 6; i++) {
		if (fs.planes[i].a * pos.x + fs.planes[i].b * pos.y + fs.planes[i].c * pos.z + fs.planes[i].d <= 0)
			return false;
	}

	return true;
}

// ------------------ intersect ------------------ //

static b32
circle_intersect(circle_t a, circle_t b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;
    return (dx * dx + dy * dy) < (rt * rt);
}

static b32
sphere_intersect(sphere_t a, sphere_t b) {
    f32 dx = b.pos.x - a.pos.x;
    f32 dy = b.pos.y - a.pos.y;
    f32 dz = b.pos.z - a.pos.z;
    f32 rt = a.rad + b.rad;
    return (dx * dx + dy * dy) < (rt * rt);
}

static b32
r2_intersect(r2 a, r2 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    return true;
}

static b32
r3_intersect(r3 a, r3 b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static b32
r2i_intersect(r2i a, r2i b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static b32
r3i_intersect(r3i a, r3i b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static b32
frustum_intersect_sphere(frustum fs, sphere_t sphere) {
    for (i32 i = 0; i < 6; i++) {
		if(fs.planes[i].a * sphere.pos.x + fs.planes[i].b * sphere.pos.y + fs.planes[i].c * sphere.pos.z + fs.planes[i].d <= -sphere.rad) {
			return false;
		}
	}
	return true;
}

static b32
frustum_intersect_r3(frustum fs, r3 rect) {
    for (int i = 0; i < 6; i++) {
		if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.min.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.min.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.min.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
		if (fs.planes[i].a * rect.max.x + fs.planes[i].b * rect.max.y + fs.planes[i].c * rect.max.z + fs.planes[i].d > 0) continue;
		return false;
	}
	return true;
}

// ------------------- get overlap --------------- //

static r2
r2_get_overlap(r2 a, r2 b) {
    return (r2) {
        .min = v2_max(a.min, b.min),
        .max = v2_min(a.max, b.max),
    };
}

static r3
r3_get_overlap(r3 a, r3 b) {
    return (r3) {
        .min = v3_max(a.min, b.min),
        .max = v3_min(a.max, b.max),
    };
}

static r2i
r2i_get_overlap(r2i a, r2i b) {
    return (r2i) {
        .min = v2i_max(a.min, b.min),
        .max = v2i_min(a.max, b.max),
    };
}

static r3i
r3i_get_overlap(r3i a, r3i b) {
    return (r3i) {
        v3i_max(a.min, b.min),
        v3i_min(a.max, b.max),
    };
}

// -------------- get intersect vector ---------- //

static v2
circle_get_intersect_vector(circle_t a, circle_t b) {
    v2 delta = v2_sub(a.pos, b.pos);
    f32 depth = v2_len(delta) - (a.rad + b.rad);

    return v2_scale(delta, -depth);
}

static v3
sphere_get_intersect_vector(sphere_t a, sphere_t b) {
    v3 delta = v3_sub(a.pos, b.pos);
    f32 depth = v3_len(delta) - (a.rad + b.rad);

    return v3_scale(delta, -depth);
}

static v2
r2_get_intersect_vector(r2 a, r2 b) {
    r2 overlap  = r2_get_overlap(a, b);
    v2 delta = {
        0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
        0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
    };
    return v2_mul(v2_sub(overlap.max, overlap.min), v2_sign(delta));
}

static v3
r3_get_intersect_vector(r3 a, r3 b) {
    r3 overlap = r3_get_overlap(a, b);
    v3 delta = {
        0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
        0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
        0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z),
    };
    return v3_mul(v3_sub(overlap.max, overlap.min), v3_sign(delta));
}

static v2i
v2i_get_intersect_vector(r2i a, r2i b) {
    r2i overlap = r2i_get_overlap(a, b);
    v2i delta = {
        0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
        0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
    };
    return v2i_mul(v2i_sub(overlap.max, overlap.min), v2i_sign(delta));
}

static v3i
v3i_get_intersect_vector(r3i a, r3i b) {
    r3i overlap = r3i_get_overlap(a, b);
    v3i delta = {
        0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x),
        0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y),
        0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z),
    };
    return v3i_mul(v3i_sub(overlap.max, overlap.min), v3i_sign(delta));
}

// ---------------------- random ------------------------ //

static u32
rand_u32(u32* state) {
    u32 x = *state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return *state = x;
}

// [min, max)
static i32
rand_i32(u32* state, i32 min, i32 max) {
    assert(min < max);
    return min + rand_u32(state) % (max - min);
}

static f32
rand_f32(u32* state, f32 min, f32 max) {
    return min + ((f32)rand_u32(state) / (f32)0xffffffff) * (max - min); 
}

static v2
rand_unit_v2(u32* state) {
    v2 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return v2_norm(out);
}

static v3
rand_unit_v3(u32* state) {
    v3 out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1) };
    return v3_norm(out);
}

static v2
rand_v2(u32* state, f32 min, f32 max) {
    return v2_scale(rand_unit_v2(state), rand_f32(state, min, max));
}

static v3
rand_v3(u32* state, f32 min, f32 max) {
    return v3_scale(rand_unit_v3(state), rand_f32(state, min, max));
}

// ----------------------- hash ------------------------- //

static u32
hash_u32(u32 a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

    return a;
}

static u32
hash_i32(i32 a) {
    union { u32 u; i32 i; } convert;
    convert.i = a;
    return hash_u32(convert.u);
}

static u32
hash_str(const char* str) {
    u32 hash = 0;
    while (*str) {
        hash = (hash << 7) + (hash >> 25) + *str++;
    }
    return hash + (hash >> 16);
}

static const u32 crc_table[] = {
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

static u32
hash_mem(const void *data, u32 size) {
    const u8 *d = (const u8*)data;
    u32 crc = 0xFFFFFFFF;
    while (size--) {
        u32 index = (crc ^ *(d++)) & 0xFF;
        crc = (crc >> 8) ^ crc_table[index];
    }
    return crc ^ 0xFFFFFFFF;
}

#define HASH_PRIME0 3323784421u
#define HASH_PRIME1 1449091801u
#define HASH_PRIME2 4280703257u
#define HASH_PRIME3 1609059329u

static u32
hash_v2i(v2i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

static u32
hash_v3i(v3i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);
    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

static u32
hash_v4i(v4i k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);
    u32 d = hash_i32(k.w);
    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2) ^ (d * HASH_PRIME3);
}

// --------------------- packed color u32 -------------------- //

static u32
pack_color_u8(u8 r, u8 g, u8 b, u8 a) {
    u32 color = 0;
    color |= (u32)(r) << 0;
    color |= (u32)(g) << 8;
    color |= (u32)(b) << 16;
    color |= (u32)(a) << 24;
    return color;
}

static u32
pack_color_f32(f32 r, f32 g, f32 b, f32 a) {
    return pack_color_u8((u8)(255 * r), (u8)(255 * g), (u8)(255 * b), (u8)(255 * a));
}

static u32
pack_color_v4(v4 color) {
    return pack_color_f32(color.r, color.g, color.b, color.a);
}

static u32
pack_color_v3(v3 color, f32 a) {
    return pack_color_f32(color.r, color.g, color.b, a);
}

// -------------------- f64 matrix funcs ------------------- //

static void
f4x4_mul_64(f64 *R, const f64 *a, const f64 *b) {
    f64 M[16];

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

static void
f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v) {
    f64 r[4];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
	r[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
    out[3] = r[3];
}

static void
f4x4_invert_64(f64* T, const f64* M) {
	f64 s[6], c[6];

	s[0] = M[0] * M[5] - M[4] * M[1];
	s[1] = M[0] * M[6] - M[4] * M[2];
	s[2] = M[0] * M[7] - M[4] * M[3];
	s[3] = M[1] * M[6] - M[5] * M[2];
	s[4] = M[1] * M[7] - M[5] * M[3];
	s[5] = M[2] * M[7] - M[6] * M[3];

	c[0] = M[8]  * M[13] - M[12] * M[9];
	c[1] = M[8]  * M[14] - M[12] * M[10];
	c[2] = M[8]  * M[15] - M[12] * M[11];
	c[3] = M[9]  * M[14] - M[13] * M[10];
	c[4] = M[9]  * M[15] - M[13] * M[11];
	c[5] = M[10] * M[15] - M[14] * M[11];
	
	// assumes it is invertible
	f64 idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
	
	T[0]  = ( M[5]  * c[5] - M[6]  * c[4] + M[7]  * c[3]) * idet;
	T[1]  = (-M[1]  * c[5] + M[2]  * c[4] - M[3]  * c[3]) * idet;
	T[2]  = ( M[13] * s[5] - M[14] * s[4] + M[15] * s[3]) * idet;
	T[3]  = (-M[9]  * s[5] + M[10] * s[4] - M[11] * s[3]) * idet;

	T[4]  = (-M[4]  * c[5] + M[6]  * c[2] - M[7]  * c[1]) * idet;
	T[5]  = ( M[0]  * c[5] - M[2]  * c[2] + M[3]  * c[1]) * idet;
	T[6]  = (-M[12] * s[5] + M[14] * s[2] - M[15] * s[1]) * idet;
	T[7]  = ( M[8]  * s[5] - M[10] * s[2] + M[11] * s[1]) * idet;

	T[8]  = ( M[4]  * c[4] - M[5]  * c[2] + M[7]  * c[0]) * idet;
	T[9]  = (-M[0]  * c[4] + M[1]  * c[2] - M[3]  * c[0]) * idet;
	T[10] = ( M[12] * s[4] - M[13] * s[2] + M[15] * s[0]) * idet;
	T[11] = (-M[8]  * s[4] + M[9]  * s[2] - M[11] * s[0]) * idet;

	T[12] = (-M[4]  * c[3] + M[5]  * c[1] - M[6]  * c[0]) * idet;
	T[13] = ( M[0]  * c[3] - M[1]  * c[1] + M[2]  * c[0]) * idet;
	T[14] = (-M[12] * s[3] + M[13] * s[1] - M[14] * s[0]) * idet;
	T[15] = ( M[8]  * s[3] - M[9]  * s[1] + M[10] * s[0]) * idet;
}

static b32
f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport) {
    f64 fTempo[8];

    fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8]  * objz + modelview[12];
    fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9]  * objz + modelview[13];
    fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
    fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];

    fTempo[4] =  projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8]  * fTempo[2] + projection[12] * fTempo[3];
    fTempo[5] =  projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9]  * fTempo[2] + projection[13] * fTempo[3];
    fTempo[6] =  projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
    fTempo[7] = -fTempo[2];

    if (fTempo[7] == 0.0f)
        return false;

    fTempo[7]= 1.0f / fTempo[7];

    fTempo[4] *= fTempo[7];
    fTempo[5] *= fTempo[7];
    fTempo[6] *= fTempo[7];

    result[0] = (fTempo[4] * 0.5f + 0.5f) * viewport[2] + viewport[0];
    result[1] = (fTempo[5] * 0.5f + 0.5f) * viewport[3] + viewport[1];
    result[2] = (1.0f + fTempo[6]) * 0.5f;

    return true;
}

static b32
f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport) {
    f64 m[16], A[16];
    f64 in[4], out[4];

    f4x4_mul_64(A, projection, modelview);
    f4x4_invert_64(m, A);

    in[0] = (winx - (f64)viewport[0]) / (f64)viewport[2] * 2.0f - 1.0f;
    in[1] = (winy - (f64)viewport[1]) / (f64)viewport[3] * 2.0f - 1.0f;
    in[2] = 2.0f * winz - 1.0f;
    in[3] = 1.0;

    f4x4_mulv_64(out, m, in);
    
    if (out[3] == 0.0f)
        return false;

    out[3] = 1.0f / out[3];

    result[0] = out[0] * out[3];
    result[1] = out[1] * out[3];
    result[2] = out[2] * out[3];

    return true;
}

// ===================================== IMAGE STUFF ================================= //

typedef struct image {
    i32 width;
    i32 height;
    u32* pixels;
} image;

static u32
image_get(const image* img, i32 x, i32 y) {
    assert(x >= 0 && x < img->width);
    assert(y >= 0 && y < img->height);
    return img->pixels[y * img->width + x];
}

static void
image_set(image* img, i32 x, i32 y, u32 pixel) {
    assert(x >= 0 && x < img->width);
    assert(y >= 0 && y < img->height);
    img->pixels[y * img->width + x] = pixel;
}

// ===================================== MEM STUFF ================================= //

static void
m_clear(void* data, usize size) {
    volatile u8* d = (u8*)data;
    while (size--)
        *(d++) = 0;
}

static void
m_set(void* data, u8 value, usize size) {
    volatile u8* d = (u8*)data;
    while (size--)
        *(d++) = value;
}

static void
m_copy(void* dst, const void* src, usize size) {
    volatile u8* d = (u8*)dst;
    volatile const u8* s = (u8*)src;
    while (size--)
        *(d++) = *(s++);
}

// --------------------- allocator interface ---------------------- //

typedef enum mem_tag {
    mem_tag_alloc,
    mem_tag_resize,
    mem_tag_free,
    // mem_tag_free_all,
} mem_tag;

typedef struct mem_alloc_desc {
    mem_tag tag;

    usize size;
    void* pointer;

    void* data;
} mem_alloc_desc;

#define M_ALLOCATOR_PROC(name) void* name(mem_alloc_desc desc)
typedef M_ALLOCATOR_PROC(mem_allocator_proc);

typedef struct mem_allocator {
    mem_allocator_proc* proc;
    void* data;
} mem_allocator;

#define mem_type(allocator, type)         (type*)mem_zero(allocator, sizeof (type))
#define mem_array(allocator, type, size)  (type*)mem_zero(allocator, (size) * sizeof (type))

static void*
mem_alloc(mem_allocator allocator, usize size) {
    return allocator.proc((mem_alloc_desc) {
        .tag = mem_tag_alloc,
        .size = size,
        .data = allocator.data,
    });
}

static void*
mem_resize(mem_allocator allocator, void* pointer, usize size) {
    return allocator.proc((mem_alloc_desc) {
        .tag = mem_tag_resize,
        .pointer = pointer,
        .size = size,
        .data = allocator.data,
    });
}

static void
mem_free(mem_allocator allocator, void* pointer) {
    allocator.proc((mem_alloc_desc) {
        .tag = mem_tag_free,
        .pointer = pointer,
        .data = allocator.data,
    });
}

static void*
mem_zero(mem_allocator allocator, usize size) {
    void* memory = mem_alloc(allocator, size);
    memset(memory, 0, size);
    return memory;
}

// ---------------------- arena allocator ------------------------ //

typedef struct mem_arena {
    usize index;
    usize cap;
    u8* buffer;

    usize top;
    usize stack[16];

    usize lock;
    usize max;
} mem_arena;

#define mem_arena_type(ma, t)           (t*)mem_arena_alloc(ma, sizeof (t))
#define mem_arena_array(ma, t, count)   (t*)mem_arena_alloc(ma, (count) * sizeof (t))

static mem_arena
mem_arena_create(u8* buffer, usize size) {
    mem_arena ma = {0};
    ma.cap = size;
    ma.buffer = buffer;
    return ma;
}

static void*
mem_arena_alloc(mem_arena* ma, usize byte_size) {
    byte_size = align_up(byte_size, 16);
    assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

    void* memory = ma->buffer + ma->index;
    ma->index += byte_size;
    ma->max = ma->max > ma->index? ma->max : ma->index;

    return memory;
}

static void*
mem_arena_begin(mem_arena* ma) {
    ma->lock = true;
    return ma->buffer + ma->index;
}

static void
mem_arena_end(mem_arena* ma, usize byte_size) {
    ma->index += align_up(byte_size, 16);
    ma->lock = false;
}

static void
mem_arena_save(mem_arena* ma) {
    assert(ma->top < ma->cap);
    ma->stack[ma->top++] = ma->index;
}

static void
mem_arena_restore(mem_arena* ma) {
    assert(ma->top > 0);
    ma->index = ma->stack[--ma->top];
}

static void
mem_arena_validate(mem_arena* ma) {
    assert(ma->top == 0);
}

static
M_ALLOCATOR_PROC(mem_arena_allocator_proc) {
    mem_arena* arena = desc.data;
    switch (desc.tag) {
        case mem_tag_alloc: {
            return mem_arena_alloc(arena, desc.size);
        } break;
    }
    return NULL;
}

static mem_allocator
mem_arena_allocator(mem_arena* arena) {
    return (mem_allocator) {
        .proc = mem_arena_allocator_proc,
        .data = arena,
    };
}

// ===================================== BUFFER STUFF ================================= //

typedef struct buf_header {
    mem_allocator allocator;

    u32 len;
    u32 cap;

    u8 data[];
} buf_header;

#define buf_create(type, n, allocator) ((type*)_buf_create(sizeof (type), (n), (allocator)))

#define _buf_header(b) ((buf_header*)(b) - 1)

#define buf_len(b)              ((b)? _buf_header(b)->len : 0)
#define buf_cap(b)              ((b)? _buf_header(b)->cap : 0)
#define buf_add(b, ...)         ((b) = _buf_grow((b), sizeof (*(b))), (b)[_buf_header(b)->len++] = (__VA_ARGS__))
#define buf_rem(b, i)           ((b)? ((b)[(i)] = (b)[--_buf_header(b)->len], 1) : 0)
#define buf_reserve(b, n)       ((b) = _buf_reserve((b), sizeof (*(b)), (n)))
#define buf_clear(b)            ((b)? _buf_header(b)->len = 0 : 0)
#define buf_sort(b, cmp_func)   qsort((b), buf_len(b), sizeof (*(b)), (cmp_func))

#define for_buf(index, b)       for (u32 index = 0; index < buf_len(b); ++index)

static void
buf_free(void* buffer) {
    if (buffer) {
        buf_header* header = _buf_header(buffer);
        mem_allocator allocator = header->allocator;
        mem_free(allocator, header);
    }
}

static void*
_buf_create(u32 element_size, u32 cap, mem_allocator allocator) {
    buf_header* header = mem_alloc(allocator, sizeof (buf_header) + cap * element_size);
    assert(header);
    header->allocator = allocator;
    header->len = 0;
    header->cap = cap;
    return header->data;
}

static void*
_buf_grow(void* buffer, u32 element_size) {
    assert(buffer);
    buf_header* header = _buf_header(buffer);
    if (header->len >= header->cap) {
        header->cap = header->cap << 1;
        buf_header* new_header = mem_resize(header->allocator, header, sizeof (buf_header) + header->cap * element_size);
        assert(new_header);
        header = new_header;
    }
    return header->data;
}

static void*
_buf_reserve(void* buffer, u32 element_size, u32 new_cap) {
    assert(buffer);
    buf_header* header = _buf_header(buffer);
    if (new_cap > header->cap) {
        header->cap = new_cap;
        buf_header* new_header = mem_resize(header->allocator, header, sizeof (buf_header) + header->cap * element_size);
        assert(new_header);
        header = new_header;
    }
    return header->data;
}

// ====================================== BIT STUFF =================================== //

static void
bit_set(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    array[idx] |= (1 << bit);
}

static bool
bit_get(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    return array[idx] & (1 << bit);
}

static void
bit_clr(u32* array, u32 index) {
    u32 idx = index >> 5;
    u32 bit = index & 31;
    array[idx] &= ~(1 << bit);
}

// ===================================== STRING STUFF ================================= //

typedef struct string {
    usize size;
    const char* data;
} string;

#define STR_FMT "%.*s"
#define STR_ARG(s) (int)(s).size, (s).data

#define str(text) (string) { sizeof (text) - 1, text }
#define STR(text) { sizeof (text) - 1, text }

static string
string_create(const char* str) {
    return (string) {
        .size = strlen(str),
        .data = str,
    };
}

static b32
string_equal(string a, string b) {
    if (a.size != b.size) return false;
    return memcmp(a.data, b.data, a.size) == 0;
}

static b32
string_equal_cstr(string a, const char* b) {
    usize b_size = strlen(b);
    if (a.size != b_size) return false;
    return memcmp(a.data, b, a.size) == 0;
}

static b32
string_empty(string s) {
    return s.size == 0;
}

typedef struct split_iter {
    string current;

    const char* content;

    u32 del_table[8];
    u32 sep_table[8];
} split_iter;

static b32
split_iter_is_valid(const split_iter* it) {
    return it->current.size;
}

static void
split_iter_advance(split_iter* it) {
    while (*it->content && bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
        it->content++;
    }

    const char* begin = it->content;

    if (bit_get(it->sep_table, *it->content)) {
        it->content++;
    } else {
        while (*it->content && !bit_get(it->del_table, *it->content) && !bit_get(it->sep_table, *it->content)) {
            it->content++;
        }
    }

    it->current = (string) {
        (usize)(it->content - begin),
        begin,
    };
}

static split_iter
split_iter_create(const char* cstr, const char* delimiters, const char* separators) {
    assert(delimiters);
    if (!separators) separators = "";

    split_iter it = {0};

    it.content = cstr;

    for (u32 i = 0; delimiters[i]; ++i) {
        bit_set(it.del_table, delimiters[i]);
    }

    for (u32 i = 0; separators[i]; ++i) {
        bit_set(it.sep_table, separators[i]);
    }

    split_iter_advance(&it);
    return it;
}

// ===================================== EXTERNAL STUFF ===================================== //

// -------------------------------- ats_file.c ----------------------------------- //

extern char* file_read_str(const char* file_name, mem_allocator allocator);
extern b32 file_write_str(const char* file_name, const char* buffer);
extern b32 file_append_str(const char* file_name, const char* buffer);

extern b32 file_read_bin(const char* file_name, void* buffer, usize size);
extern b32 file_write_bin(const char* file_name, const void* buffer, usize size);

extern image file_load_image(const char* path);
extern void file_free_image(image* img);

// ------------------------------- ats_memory.c --------------------------------- //

extern mem_allocator mem_heap_allocator(void);
extern mem_allocator mem_linear_allocator(usize size);

// --------------------------- ats_texture_table.c ------------------------------ //

#define TEXTURE_TABLE_SIZE (1024)

typedef struct texture_id {
    u16 index;
} texture_id;

typedef struct texture_entry {
    b32 in_use;
    u32 hash;
    r2i rect;
    char name[64];
} texture_entry;

typedef struct texture_table {
    image img;
    texture_entry array[TEXTURE_TABLE_SIZE];
} texture_table;

extern void tt_begin(int width, int height, mem_allocator allocator);
extern void tt_end(void);
extern void tt_add_image(const char* name, image img);
extern void tt_load_from_dir(const char* dir_path);

extern texture_table* tt_get_texture_table(void);

extern texture_id tt_get_id(const char* name);
extern r2i tt_get_rect(texture_id id);
extern r2i tt_get(const char* name);

extern image tt_get_image(void);

// -------------------------- ats_animation.c ----------------------------- //

typedef struct at_frame at_frame;
typedef struct at_animation at_animation;
typedef struct at_entity at_entity;

struct at_frame {
    const char* name;
    r2i rect;
    at_frame* next;
    at_animation* animation;
};

struct at_animation {
    const char* name;
    at_frame* frame;
    at_animation* next;
};

struct at_entity {
    const char* name;
    at_animation* animation;
    at_entity* next;
};

extern void at_begin(mem_allocator allocator);
extern void at_end(void);

extern void at_add_entity(const char* name);
extern void at_add_animation(const char* name);
extern void at_add_frame(const char* name);

typedef struct at_asset {
    at_entity* entity;
    at_frame* frame;
    f32 duration;
} at_asset;

extern at_asset at_get(const char* name);
extern void at_set(at_asset* asset, const char* name);
extern void at_update(at_asset* asset, f32 dt);

// -------------------------- ats_audio_table.c ----------------------------- //

typedef struct audio_id {
    u16 index;
} audio_id;

extern void audio_init(void* handle);
extern audio_id audio_get(const char* name);
extern void audio_pause(b32 pause);
extern void audio_kill_all(void);
extern void audio_play(audio_id id, f32 volume);
extern void* audio_play_looped(audio_id id, f32 volume);
extern void audio_play_music(audio_id id, f32 volume);
extern void audio_play_from_source(audio_id id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);

