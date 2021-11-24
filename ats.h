#pragma once

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>
#include <assert.h>

// ======================================== API-MACROS ======================================= //

#ifndef __cplusplus
#define true  1
#define false 0
#endif

#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define PI (3.14159265359f)

#define TO_RAD_MUL (0.01745329251f)
#define TO_DEG_MUL (57.2957795131f)

#define TO_RAD(deg) ((deg) * TO_RAD_MUL)
#define TO_DEG(rad) ((rad) * TO_DEG_MUL)

#define IS_POWER_OF_TWO(x)  (((x) != 0) && ((x) & ((x)-1)) == 0)

#define ALIGN_DOWN(n, a)      ((n) & ~((a) - 1))
#define ALIGN_UP(n, a)        ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a)  ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a)    ((void*)ALIGN_DOWN((uintptr_t)(p), (a)))

#define ARRAY_COUNT(array) (sizeof (array) / sizeof ((array)[0]))

#define join_helper(a, b)   a##b
#define join_token(a, b)    join_helper(a, b)
#define macro_var(a)        join_token(a, __LINE__)

#define defer(start, end) for (int macro_var(i) = ((start), 0); !macro_var(i); (macro_var(i)++, (end)))

#define for_rect2(rect, x, y) \
    for (i32 y = rect.min.y; y <= rect.max.y; ++y) \
    for (i32 x = rect.min.x; x <= rect.max.x; ++x)

#define for_rect3(rect, x, y, z) \
    for (i32 z = rect.min.z; z <= rect.max.z; ++z) \
    for (i32 y = rect.min.y; y <= rect.max.y; ++y) \
    for (i32 x = rect.min.x; x <= rect.max.x; ++x)

#ifndef __cplusplus
#define ATS_INIT_ZERO {0}
#else
#define ATS_INIT_ZERO {}
#endif

#define LERP(a, b, t)       ((a) + (t) * ((b) - (a)))
#define MIN(a, b)           ((a) < (b)? (a) : (b))
#define MAX(a, b)           ((a) > (b)? (a) : (b))
#define SIGN(n)             ((n) < 0? (-1) : (1))
#define SIGN_OR_ZERO(n)     ((n) == 0? 0 : SIGN(n))
#define CLAMP_MIN(a, lo)    ((a) < (lo)? (lo) : (a))
#define CLAMP_MAX(a, hi)    ((a) > (hi)? (hi) : (a))
#define CLAMP(a, lo, hi)    CLAMP_MIN(CLAMP_MAX(a, hi), lo)

// =========================================== API-TYPES ====================================== //

typedef float   f32;
typedef double  f64;

typedef char        i8;
typedef short       i16;
typedef int         i32;
typedef long long   i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

static_assert (sizeof (i8)  >= 1, "i8 -- not enough bits!");
static_assert (sizeof (i16) >= 2, "i16 -- not enough bits!");
static_assert (sizeof (i32) >= 4, "i32 -- not enough bits!");
static_assert (sizeof (i64) >= 8, "i64 -- not enough bits!");

static_assert (sizeof (u8)  >= 1, "u8 -- not enough bits!");
static_assert (sizeof (u16) >= 2, "u16 -- not enough bits!");
static_assert (sizeof (u32) >= 4, "u32 -- not enough bits!");
static_assert (sizeof (u64) >= 8, "u64 -- not enough bits!");

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

typedef union vec2_t vec2_t;
typedef union vec3_t vec3_t;
typedef union vec4_t vec4_t;

typedef union vec2i_t vec2i_t;
typedef union vec3i_t vec3i_t;
typedef union vec4i_t vec4i_t;

typedef union color_t color_t;

typedef union mat2_t    mat2_t;
typedef union mat3_t    mat3_t;
typedef union mat4_t    mat4_t;

typedef struct quat_t   quat_t;

typedef struct circle_t circle_t;
typedef struct sphere_t sphere_t;
typedef struct rect2_t  rect2_t;
typedef struct rect3_t  rect3_t;
typedef struct rect2i_t rect2i_t;
typedef struct rect3i_t rect3i_t;

typedef struct plane_t      plane_t;
typedef struct frustum_t    frustum_t;

typedef struct memory_arena_t memory_arena_t;

typedef struct image_t image_t;

typedef struct queue_node_t     queue_node_t;
typedef struct priority_queue_t priority_queue_t;

// =========================================== TYPES ========================================= //

union vec2_t {
    struct { f32 x, y; };

    f32 e[2];
};

union vec3_t {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };

    struct { vec2_t xy; };
    struct { f32 _p; vec2_t yz; };

    f32 e[3];
};

union vec4_t {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    struct { vec3_t rgb; };
    struct { vec3_t xyz; };

    f32 e[4];
};

union vec2i_t {
    struct { i32 x, y; };

    i32 e[2];
};

union vec3i_t {
    struct { i32 x, y, z; };
    struct { vec2i_t xy; };

    struct { i32 _p; vec2i_t yz; };

    i32 e[3];
};

union vec4i_t {
    struct { i32 x, y, z, w; };

    i32 e[4];
};

union color_t {
    struct {
        u8 r, g, b, a;
    };

    u32 packed;
};

union mat2_t {
    f32 e[4];
    struct { vec2_t x, y; };
};

union mat3_t {
    f32 e[9];
    struct { vec3_t x, y, z; };
};

union mat4_t {
    f32 e[16];
    struct { vec4_t x, y, z, w; };
};

struct quat_t {
    f32 x, y, z, w;
};

struct circle_t {
    vec2_t  pos;
    f32     rad;
};

struct sphere_t {
    vec3_t  pos;
    f32     rad;
};

struct rect2_t {
    vec2_t min;
    vec2_t max;
};

struct rect3_t {
    vec3_t min;
    vec3_t max;
};

struct rect2i_t {
    vec2i_t min;
    vec2i_t max;
};

struct rect3i_t {
    vec3i_t min;
    vec3i_t max;
};

struct plane_t {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
};

struct frustum_t {
    plane_t plane[6];
};

struct memory_arena_t {
    u32 index;
    u32 cap;
    u8* buffer;

    u32 top;
    u32 stack[16];

    u32 lock;
    u32 max;
};

struct image_t {
    int width;
    int height;

    u32* pixels;
};

struct queue_node_t {
    f32 weight;
    vec2i_t e;
};

struct priority_queue_t {
    u32 len;
    queue_node_t* array;
};

// ======================================== FUNCTIONS ======================================= //

static f32 sqrt_f32(f32 n);
static f32 rsqrt_f32(f32 n);

static f32 shortest_angle_distance_f32(f32 a, f32 b);
static f32 lerp_angle_f32(f32 a, f32 b, f32 t);

static f32 clamp_f32(f32 n, f32 min, f32 max);

static f32 lerp_f32(f32 a, f32 b, f32 t);

// ---------- type creation ---------- //

static vec2_t v2_from_array(const f32* a);
static vec3_t v3_from_array(const f32* a);
static vec4_t v4_from_array(const f32* a);

static vec2i_t v2i_from_array(const i32* a);
static vec3i_t v3i_from_array(const i32* a);
static vec4i_t v4i_from_array(const i32* a);

static vec3_t v3_from_packed_color(u32 color);
static vec4_t v4_from_packed_color(u32 color);

// ---------- identity ----------- //

static mat2_t m2_identity(void);
static mat3_t m3_identity(void);
static mat4_t m4_identity(void);

static quat_t quat_identity(void);

// --------- negate ---------- //

static vec2_t v2_neg(vec2_t u);
static vec3_t v3_neg(vec3_t u);
static vec4_t v4_neg(vec4_t u);

static vec2i_t v2i_neg(vec2i_t u);
static vec3i_t v3i_neg(vec3i_t u);
static vec4i_t v4i_neg(vec4i_t u);

// -------- addition ------- //

static vec2_t v2_add(vec2_t a, vec2_t b);
static vec3_t v3_add(vec3_t a, vec3_t b);
static vec4_t v4_add(vec4_t a, vec4_t b);

static vec2i_t v2i_add(vec2i_t a, vec2i_t b);
static vec3i_t v3i_add(vec3i_t a, vec3i_t b);
static vec4i_t v4i_add(vec4i_t a, vec4i_t b);

// -------- subtraction ------- //

static vec2_t v2_sub(vec2_t a, vec2_t b);
static vec3_t v3_sub(vec3_t a, vec3_t b);
static vec4_t v4_sub(vec4_t a, vec4_t b);

static vec2i_t v2i_sub(vec2i_t a, vec2i_t b);
static vec3i_t v3i_sub(vec3i_t a, vec3i_t b);
static vec4i_t v4i_sub(vec4i_t a, vec4i_t b);

// -------- multiplication ------- //

static vec2_t v2_mul(vec2_t a, vec2_t b);
static vec3_t v3_mul(vec3_t a, vec3_t b);
static vec4_t v4_mul(vec4_t a, vec4_t b);

static vec2i_t v2i_mul(vec2i_t a, vec2i_t b);
static vec3i_t v3i_mul(vec3i_t a, vec3i_t b);
static vec4i_t v4i_mul(vec4i_t a, vec4i_t b);

static vec2_t m2_mulv(mat2_t m, vec2_t u);
static vec3_t m3_mulv(mat3_t m, vec3_t u);
static vec4_t m4_mulv(mat4_t m, vec4_t u);

static mat2_t m2_mul(mat2_t a, mat2_t b);
static mat3_t m3_mul(mat3_t a, mat3_t b);
static mat4_t m4_mul(mat4_t a, mat4_t b);

static quat_t quat_mul(quat_t a, quat_t b);

// ------------ divition ------------ //

static vec2_t v2_div(vec2_t a, vec2_t b);
static vec3_t v3_div(vec3_t a, vec3_t b);
static vec4_t v4_div(vec4_t a, vec4_t b);

static vec2i_t v2i_div(vec2i_t a, vec2i_t b);
static vec3i_t v3i_div(vec3i_t a, vec3i_t b);
static vec4i_t v4i_div(vec4i_t a, vec4i_t b);

// ------------- scaling ------------- //

static vec2_t v2_scale(vec2_t a, f32 s);
static vec3_t v3_scale(vec3_t a, f32 s);
static vec4_t v4_scale(vec4_t a, f32 s);

static vec2i_t v2i_scale(vec2i_t a, i32 s);
static vec3i_t v3i_scale(vec3i_t a, i32 s);
static vec4i_t v4i_scale(vec4i_t a, i32 s);

// ----------- dot product ----------- //

static f32 v2_dot(vec2_t a, vec2_t b);
static f32 v3_dot(vec3_t a, vec3_t b);
static f32 v4_dot(vec4_t a, vec4_t b);

static i32 v2i_dot(vec2i_t a, vec2i_t b);
static i32 v3i_dot(vec3i_t a, vec3i_t b);
static i32 v4i_dot(vec4i_t a, vec4i_t b);

// ----------- length squared ----------- //

static f32 v2_len_sq(vec2_t u);
static f32 v3_len_sq(vec3_t u);
static f32 v4_len_sq(vec4_t u);

static i32 v2i_len_sq(vec2i_t u);
static i32 v3i_len_sq(vec3i_t u);
static i32 v4i_len_sq(vec4i_t u);

// -------------- length -------------- //

static f32 v2_len(vec2_t u);
static f32 v3_len(vec3_t u);
static f32 v4_len(vec4_t u);

// -------------- distance squared -------------- //

static f32 v2_dist_sq(vec2_t a, vec2_t b);
static f32 v3_dist_sq(vec3_t a, vec3_t b);
static f32 v4_dist_sq(vec4_t a, vec4_t b);

static i32 v2i_dist_sq(vec2i_t a, vec2i_t b);
static i32 v3i_dist_sq(vec3i_t a, vec3i_t b);
static i32 v4i_dist_sq(vec4i_t a, vec4i_t b);

// ----------------- distance ------------------ //

static f32 v2_dist(vec2_t a, vec2_t b);
static f32 v3_dist(vec3_t a, vec3_t b);
static f32 v4_dist(vec4_t a, vec4_t b);

// -------------- manhattan distance -------------- //

static i32 v2i_manhattan(vec2i_t a, vec2i_t b);
static i32 v3i_manhattan(vec3i_t a, vec3i_t b);

// -------------- normalize --------------- //

static vec2_t v2_norm(vec2_t u);
static vec3_t v3_norm(vec3_t u);
static vec4_t v4_norm(vec4_t u);

// ---------------- min ----------------- //

static vec2_t v2_min(vec2_t a, vec2_t b);
static vec3_t v3_min(vec3_t a, vec3_t b);
static vec4_t v4_min(vec4_t a, vec4_t b);

static vec2i_t v2i_min(vec2i_t a, vec2i_t b);
static vec3i_t v3i_min(vec3i_t a, vec3i_t b);
static vec4i_t v4i_min(vec4i_t a, vec4i_t b);

// ----------- equal ------------ //

static b32 v2i_equal(vec2i_t a, vec2i_t b);
static b32 v3i_equal(vec3i_t a, vec3i_t b);
static b32 v4i_equal(vec4i_t a, vec4i_t b);

// ---------------- max ----------------- //

static vec2_t v2_max(vec2_t a, vec2_t b);
static vec3_t v3_max(vec3_t a, vec3_t b);
static vec4_t v4_max(vec4_t a, vec4_t b);

static vec2i_t v2i_max(vec2i_t a, vec2i_t b);
static vec3i_t v3i_max(vec3i_t a, vec3i_t b);
static vec4i_t v4i_max(vec4i_t a, vec4i_t b);

// ---------------- lerp ----------------- //

static vec2_t v2_lerp(vec2_t a, vec2_t b, f32 t);
static vec3_t v3_lerp(vec3_t a, vec3_t b, f32 t);
static vec4_t v4_lerp(vec4_t a, vec4_t b, f32 t);

static color_t color_lerp(color_t a, color_t b, f32 t);

// --------------- cross ------------------- //

static vec3_t v3_cross(vec3_t a, vec3_t b);

// --------------- get angle --------------- //

static f32 v2_get_angle(vec2_t a, vec2_t b);

// --------------- transform/scale/rotation ---------------- //

static mat2_t m2_rotate(f32 angle);
static mat3_t m3_rotate(vec3_t axis, f32 angle);
static mat4_t m4_rotate(vec3_t axis, f32 angle);

static quat_t quat_rotate(vec3_t axis, f32 angle);

static mat4_t m4_translate(f32 x, f32 y, f32 z);
static mat4_t m4_scale(f32 x, f32 y, f32 z);

// --------------- view matricies --------------- //

static mat4_t m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
static mat4_t m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f);
static mat4_t m4_look_at(vec3_t eye, vec3_t center, vec3_t up);

// ----------------- plane/frustrum ------------------- //

static plane_t   plane_normalize(plane_t plane);
static frustum_t frustum_create(mat4_t m, b32 normalize);

// ------------------ contains ------------------ //

static b32 circle_contains(circle_t circle, vec2_t pos);
static b32 sphere_contains(sphere_t sphere, vec3_t pos);
static b32 rect2_contains(rect2_t rect, vec2_t pos);
static b32 rect3_contains(rect3_t rect, vec3_t pos);
static b32 rect2i_contains(rect2i_t rect, vec2i_t pos);
static b32 rect3i_contains(rect3i_t rect, vec3i_t pos);
static b32 frustum_contains(frustum_t frustum, vec3_t pos);

// ------------------ intersect ------------------ //

static b32 circle_intersect(circle_t a, circle_t b);
static b32 sphere_intersect(sphere_t a, sphere_t b);
static b32 rect2_intersect(rect2_t a, rect2_t b);
static b32 rect3_intersect(rect3_t a, rect3_t b);
static b32 rect2i_intersect(rect2i_t a, rect2i_t b);
static b32 rect3i_intersect(rect3i_t a, rect3i_t b);

static b32 frustum_intersect_sphere(frustum_t frustum, sphere_t sphere);
static b32 frustum_intersect_rect3(frustum_t frustum, rect3_t rect);

// ------------------- get overlap --------------- //

static rect2_t rect2_get_overlap(rect2_t a, rect2_t b);
static rect3_t rect3_get_overlap(rect3_t a, rect3_t b);
static rect2i_t rect2i_get_overlap(rect2i_t a, rect2i_t b);
static rect3i_t rect3i_get_overlap(rect3i_t a, rect3i_t b);

// -------------- get intersect vector ---------- //

static vec2_t  circle_get_intersect_vector(circle_t a, circle_t b);
static vec3_t  sphere_get_intersect_vector(sphere_t a, sphere_t b);
static vec2_t  rect2_get_intersect_vector(rect2_t a, rect2_t b);
static vec3_t  rect3_get_intersect_vector(rect3_t a, rect3_t b);
static vec2i_t rect2i_get_intersect_vector(rect2i_t a, rect2i_t b);
static vec3i_t rect3i_get_intersect_vector(rect3i_t a, rect3i_t b);

// ---------------------- random ------------------------ //

static u32 rand_u32(u32* state);
static i32 rand_i32(u32* state, i32 min, i32 max);
static f32 rand_f32(u32* state, f32 min, f32 max);

static vec2_t rand_unit_v2(u32* state);
static vec3_t rand_unit_v3(u32* state);

static vec2_t rand_v2(u32* state, f32 min, f32 max);
static vec3_t rand_v3(u32* state, f32 min, f32 max);

// ----------------------- hash ------------------------- //

static u32 hash_u32(u32 a);
static u32 hash_i32(i32 a);
static u32 hash_str(const char* str);
static u32 hash_mem(const void* ptr, u32 size);

static u32 hash_v2i(vec2i_t k);
static u32 hash_v3i(vec3i_t k);
static u32 hash_v4i(vec4i_t k);

// --------------------- packed color u32 -------------------- //

static u32 pack_color_u8(u8 r, u8 g, u8 b, u8 a);
static u32 pack_color_f32(f32 r, f32 g, f32 b, f32 a);
static u32 pack_color_v4(vec4_t color);
static u32 pack_color_v3(vec3_t color);

// ---------------------- priority queue --------------------- //

static b32  queue_empty(const priority_queue_t* queue);
static void queue_clear(priority_queue_t* queue);
static void queue_push(priority_queue_t* queue, vec2i_t e, f32 weight);
static f32  queue_pop(vec2i_t* out, priority_queue_t* queue);

// -------------------- f64 matrix funcs ------------------- //

static void f4x4_mul_64(f64 *R, const f64 *a, const f64 *b);
static void f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v);
static void f4x4_invert_64(f64* T, const f64* M);
static b32  f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport);
static b32  f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport);

// ---------------------- arena allocator ------------------------ //

static memory_arena_t ma_create(u8* buffer, u32 size);

#define ma_type(ma, T)           (T*)ma_alloc(ma, sizeof (T))
#define ma_array(ma, T, count)   (T*)ma_alloc(ma, (count) * sizeof (T))

extern void* ma_alloc   (memory_arena_t* ma, u32 byte_size);
extern void* ma_begin   (memory_arena_t* ma);
extern void  ma_end     (memory_arena_t* ma, u32 byte_size);
extern void  ma_save    (memory_arena_t* ma);
extern void  ma_restore (memory_arena_t* ma);
extern void  ma_validate(memory_arena_t* ma);

// --------------------- read/write files ------------------------- //

extern char* file_read_str(const char* file_name, memory_arena_t* ma);
extern b32   file_write_str(const char* file_name, const char* buffer);
extern b32   file_append_str(const char* file_name, const char* buffer);

extern b32 file_read_bin(const char* file_name, void* buffer, u32 size);
extern b32 file_write_bin(const char* file_name, const void* buffer, u32 size);

// ----------------------- image -------------------------- //

extern image_t  image_load_from_file(const char* path);
extern u32      image_get_pixel(image_t* img, i32 x, i32 y);
extern void     image_set_pixel(image_t* img, i32 x, i32 y, u32 pixel);

// ----------------------------- C / C++ specific stuff ------------------------------- //

#ifndef __cplusplus
#define ctor(type_t, ...) ((type_t) { __VA_ARGS__ })
#else
#define ctor(type_t, ...) (type_t { __VA_ARGS__ })
#endif

#define m2(...)     ctor(mat2_t, __VA_ARGS__)
#define m3(...)     ctor(mat3_t, __VA_ARGS__)
#define m4(...)     ctor(mat4_t, __VA_ARGS__)

#define quat(...)   ctor(quat_t, __VA_ARGS__)

#define circle(...) ctor(circle_t,  __VA_ARGS__)

#define rect2(...)  ctor(rect2_t, __VA_ARGS__)
#define rect3(...)  ctor(rect3_t, __VA_ARGS__)

#define rect2i(...) ctor(rect2i_t, __VA_ARGS__)
#define rect3i(...) ctor(rect3i_t, __VA_ARGS__)

#ifndef __cplusplus // --------- c stuff-----------------//

#define v2(...)  ctor(vec2_t, __VA_ARGS__)
#define v3(...)  ctor(vec3_t, __VA_ARGS__)
#define v4(...)  ctor(vec4_t, __VA_ARGS__)

#define v2i(...) ctor(vec2i_t, __VA_ARGS__)
#define v3i(...) ctor(vec3i_t, __VA_ARGS__)
#define v4i(...) ctor(vec4i_t, __VA_ARGS__)

#define color(...)  ctor(color_t, __VA_ARGS__)

#define v2_cast(vec_t, u) ctor(vec_t, (u).x, (u).y)
#define v3_cast(vec_t, u) ctor(vec_t, (u).x, (u).y, (u).z)
#define v4_cast(vec_t, u) ctor(vec_t, (u).x, (u).y, (u).z, (u).w)

#else // ------ c++ stuff ---------- //

// -------------------------- creation ---------------------------- //

// ------------ v2 ----------- //
static vec2_t v2(f32 x, f32 y)                  { return { x, y }; }
static vec2_t v2(f32 n)                         { return { n, n }; }
static vec2_t v2(vec3_t u)                      { return v2(u.x, u.y); }
static vec2_t v2(vec2i_t u)                     { return v2(u.x, u.y); }
static vec2_t v2(vec3i_t u)                     { return v2(u.x, u.y); }
static vec2_t v2(vec4i_t u)                     { return v2(u.x, u.y); }
static vec2_t v2(const f32* a)                  { return v2_from_array(a); }

// ------------ v3 ----------- //
static vec3_t v3(f32 x, f32 y, f32 z)           { return { x, y, z }; }
static vec3_t v3(f32 n)                         { return { n, n, n }; }
static vec3_t v3(vec2_t u, f32 z = 0)           { return v3(u.x, u.y, z); }
static vec3_t v3(vec4_t u)                      { return v3(u.x, u.y, u.z); }
static vec3_t v3(vec2i_t u, f32 z = 0)          { return v3(u.x, u.y, z); }
static vec3_t v3(vec3i_t u)                     { return v3(u.x, u.y, u.z); }
static vec3_t v3(vec4i_t u)                     { return v3(u.x, u.y, u.z); }

// ------------ v4 ----------- //
static vec4_t v4(f32 x, f32 y, f32 z, f32 w)    { return { x, y, z, w }; }
static vec4_t v4(f32 n)                         { return { n, n, n, n }; }
static vec4_t v4(vec3_t u, f32 w = 0)           { return v4(u.x, u.y, u.z, w); }
static vec4_t v4(vec4i_t u)                     { return v4(u.x, u.y, u.z, u.w); }

// ------------ v2i ----------- //
static vec2i_t v2i(i32 x, i32 y)                { return { x, y }; }
static vec2i_t v2i(i32 n)                       { return { n, n }; }
static vec2i_t v2i(vec2_t u)                    { return v2i(u.x, u.y); }
static vec2i_t v2i(vec3_t u)                    { return v2i(u.x, u.y); }

// ------------ v3i ----------- //
static vec3i_t v3i(i32 x, i32 y, i32 z)         { return { x, y, z }; }
static vec3i_t v3i(i32 n)                       { return { n, n, n }; }
static vec3i_t v3i(vec2_t u, f32 z = 0)         { return v3i(u.x, u.y, z); }
static vec3i_t v3i(vec3_t u)                    { return v3i(u.x, u.y, u.z); }
static vec3i_t v3i(vec4_t u)                    { return v3i(u.x, u.y, u.z); }

// ------------ v4i ----------- //
static vec4i_t v4i(i32 x, i32 y, i32 z, i32 w)  { return { x, y, z, w }; }
static vec4i_t v4i(i32 n)                       { return { n, n, n, n }; }

// ------------ color ----------- //
static color_t color(u8 r, u8 g, u8 b, u8 a)    { return { r, g, b, a }; }
static color_t color(vec3_t u, u8 a = 255)      { return color(255 * u.r, 255 * u.g, 255 * u.b, a); }
static color_t color(vec4_t u)                  { return color(255 * u.r, 255 * u.g, 255 * u.b, 255 * u.a); }

#if 0
// ------------ quat ----------- //
static quat_t  quat(f32 x, f32 y, f32 z, f32 w) { return { x, y, z, w }; }

// ------------ m2 ----------- //
static mat2_t m2(f32 x0, f32 y0,
                 f32 x1, f32 y1) {
    return {
        x0, y0,
        x1, y1,
    };
}

static mat2_t m2(vec2_t x, vec2_t y) {
    return {
        x.x, x.y,
        y.x, y.y,
    };
}

// ------------ m3 ----------- //
static mat3_t m3(f32 x0, f32 y0, f32 z0,
                 f32 x1, f32 y1, f32 z1,
                 f32 x2, f32 y2, f32 z2) {
    return {
        x0, y0, z0,
        x1, y1, z1,
        x2, y2, z2,
    };
}

static mat3_t m3(vec3_t x, vec3_t y, vec3_t z) {
    return {
        x.x, x.y, x.z,
        y.x, y.y, y.z,
        z.x, z.y, z.z,
    };
}

// ------------ m4 ----------- //
static mat4_t m4(f32 x0, f32 y0, f32 z0, f32 w0,
                 f32 x1, f32 y1, f32 z1, f32 w1,
                 f32 x2, f32 y2, f32 z2, f32 w2,
                 f32 x3, f32 y3, f32 z3, f32 w3) {
    return {
        x0, y0, z0, w0,
        x1, y1, z1, w1,
        x2, y2, z2, w2,
        x3, y3, z3, w3,
    };
}

static mat4_t m4(vec4_t x, vec4_t y, vec4_t z, vec4_t w) {
    return {
        x.x, x.y, x.z, x.w,
        y.x, y.y, y.z, y.w,
        z.x, z.y, z.z, z.w,
        w.x, w.y, w.z, w.w,
    };
}
#endif

// ---------------------- operator overloading --------------------- //

// -------- add -------- //

static vec2_t operator+(vec2_t a, vec2_t b) { return v2_add(a, b); }
static vec3_t operator+(vec3_t a, vec3_t b) { return v3_add(a, b); }
static vec4_t operator+(vec4_t a, vec4_t b) { return v4_add(a, b); }

static vec2i_t operator+(vec2i_t a, vec2i_t b) { return v2i_add(a, b); }
static vec3i_t operator+(vec3i_t a, vec3i_t b) { return v3i_add(a, b); }
static vec4i_t operator+(vec4i_t a, vec4i_t b) { return v4i_add(a, b); }

static vec2_t operator+=(vec2_t& a, vec2_t b) { a = a + b; return a; }
static vec3_t operator+=(vec3_t& a, vec3_t b) { a = a + b; return a; }
static vec4_t operator+=(vec4_t& a, vec4_t b) { a = a + b; return a; }

static vec2i_t operator+=(vec2i_t& a, vec2i_t b) { a = a + b; return a; }
static vec3i_t operator+=(vec3i_t& a, vec3i_t b) { a = a + b; return a; }
static vec4i_t operator+=(vec4i_t& a, vec4i_t b) { a = a + b; return a; }

// -------- sub -------- //

static vec2_t operator-(vec2_t a) { return v2_neg(a); }
static vec3_t operator-(vec3_t a) { return v3_neg(a); }
static vec4_t operator-(vec4_t a) { return v4_neg(a); }

static vec2i_t operator-(vec2i_t a) { return v2i_neg(a); }
static vec3i_t operator-(vec3i_t a) { return v3i_neg(a); }
static vec4i_t operator-(vec4i_t a) { return v4i_neg(a); }

static vec2_t operator-(vec2_t a, vec2_t b) { return v2_sub(a, b); }
static vec3_t operator-(vec3_t a, vec3_t b) { return v3_sub(a, b); }
static vec4_t operator-(vec4_t a, vec4_t b) { return v4_sub(a, b); }

static vec2i_t operator-(vec2i_t a, vec2i_t b) { return v2i_sub(a, b); }
static vec3i_t operator-(vec3i_t a, vec3i_t b) { return v3i_sub(a, b); }
static vec4i_t operator-(vec4i_t a, vec4i_t b) { return v4i_sub(a, b); }

static vec2_t operator-=(vec2_t& a, vec2_t b) { a = a - b; return a; }
static vec3_t operator-=(vec3_t& a, vec3_t b) { a = a - b; return a; }
static vec4_t operator-=(vec4_t& a, vec4_t b) { a = a - b; return a; }

static vec2i_t operator-=(vec2i_t& a, vec2i_t b) { a = a - b; return a; }
static vec3i_t operator-=(vec3i_t& a, vec3i_t b) { a = a - b; return a; }
static vec4i_t operator-=(vec4i_t& a, vec4i_t b) { a = a - b; return a; }

// -------- mul -------- //

static vec2_t operator*(vec2_t a, vec2_t b) { return v2_mul(a, b); }
static vec3_t operator*(vec3_t a, vec3_t b) { return v3_mul(a, b); }
static vec4_t operator*(vec4_t a, vec4_t b) { return v4_mul(a, b); }

static vec2i_t operator*(vec2i_t a, vec2i_t b) { return v2i_mul(a, b); }
static vec3i_t operator*(vec3i_t a, vec3i_t b) { return v3i_mul(a, b); }
static vec4i_t operator*(vec4i_t a, vec4i_t b) { return v4i_mul(a, b); }

static mat2_t operator*(mat2_t a, mat2_t b) { return m2_mul(a, b); }
static mat3_t operator*(mat3_t a, mat3_t b) { return m3_mul(a, b); }
static mat4_t operator*(mat4_t a, mat4_t b) { return m4_mul(a, b); }

static vec2_t operator*(mat2_t a, vec2_t b) { return m2_mulv(a, b); }
static vec3_t operator*(mat3_t a, vec3_t b) { return m3_mulv(a, b); }
static vec4_t operator*(mat4_t a, vec4_t b) { return m4_mulv(a, b); }

static vec2_t operator*=(vec2_t& a, vec2_t b) { a = a * b; return a; }
static vec3_t operator*=(vec3_t& a, vec3_t b) { a = a * b; return a; }
static vec4_t operator*=(vec4_t& a, vec4_t b) { a = a * b; return a; }

static vec2i_t operator*=(vec2i_t& a, vec2i_t b) { a = a * b; return a; }
static vec3i_t operator*=(vec3i_t& a, vec3i_t b) { a = a * b; return a; }
static vec4i_t operator*=(vec4i_t& a, vec4i_t b) { a = a * b; return a; }

// -------- scale -------- //

static vec2_t operator*(vec2_t a, f32 s) { return v2_scale(a, s); }
static vec3_t operator*(vec3_t a, f32 s) { return v3_scale(a, s); }
static vec4_t operator*(vec4_t a, f32 s) { return v4_scale(a, s); }
static vec2_t operator*(f32 s, vec2_t a) { return v2_scale(a, s); }
static vec3_t operator*(f32 s, vec3_t a) { return v3_scale(a, s); }
static vec4_t operator*(f32 s, vec4_t a) { return v4_scale(a, s); }

static vec2i_t operator*(vec2i_t a, i32 s) { return v2i_scale(a, s); }
static vec3i_t operator*(vec3i_t a, i32 s) { return v3i_scale(a, s); }
static vec4i_t operator*(vec4i_t a, i32 s) { return v4i_scale(a, s); }
static vec2i_t operator*(i32 s, vec2i_t a) { return v2i_scale(a, s); }
static vec3i_t operator*(i32 s, vec3i_t a) { return v3i_scale(a, s); }
static vec4i_t operator*(i32 s, vec4i_t a) { return v4i_scale(a, s); }

static vec2_t operator*=(vec2_t& a, f32 b) { a = a * b; return a; }
static vec3_t operator*=(vec3_t& a, f32 b) { a = a * b; return a; }
static vec4_t operator*=(vec4_t& a, f32 b) { a = a * b; return a; }

static vec2i_t operator*=(vec2i_t& a, i32 b) { a = a * b; return a; }
static vec3i_t operator*=(vec3i_t& a, i32 b) { a = a * b; return a; }
static vec4i_t operator*=(vec4i_t& a, i32 b) { a = a * b; return a; }

// -------- div -------- //

static vec2_t operator/(vec2_t a, vec2_t b) { return v2_div(a, b); }
static vec3_t operator/(vec3_t a, vec3_t b) { return v3_div(a, b); }
static vec4_t operator/(vec4_t a, vec4_t b) { return v4_div(a, b); }

static vec2i_t operator/(vec2i_t a, vec2i_t b) { return v2i_div(a, b); }
static vec3i_t operator/(vec3i_t a, vec3i_t b) { return v3i_div(a, b); }
static vec4i_t operator/(vec4i_t a, vec4i_t b) { return v4i_div(a, b); }

static vec2_t operator/(vec2_t a, f32 s) { return v2_div(a, v2(s, s)); }
static vec3_t operator/(vec3_t a, f32 s) { return v3_div(a, v3(s, s, s)); }
static vec4_t operator/(vec4_t a, f32 s) { return v4_div(a, v4(s, s, s, s)); }
static vec2_t operator/(f32 s, vec2_t a) { return v2_div(a, v2(s, s)); }
static vec3_t operator/(f32 s, vec3_t a) { return v3_div(a, v3(s, s, s)); }
static vec4_t operator/(f32 s, vec4_t a) { return v4_div(a, v4(s, s, s, s)); }

static vec2i_t operator/(vec2i_t a, i32 s) { return v2i_div(a, v2i(s, s)); }
static vec3i_t operator/(vec3i_t a, i32 s) { return v3i_div(a, v3i(s, s, s)); }
static vec4i_t operator/(vec4i_t a, i32 s) { return v4i_div(a, v4i(s, s, s, s)); }
static vec2i_t operator/(i32 s, vec2i_t a) { return v2i_div(a, v2i(s, s)); }
static vec3i_t operator/(i32 s, vec3i_t a) { return v3i_div(a, v3i(s, s, s)); }
static vec4i_t operator/(i32 s, vec4i_t a) { return v4i_div(a, v4i(s, s, s, s)); }

static vec2_t operator/=(vec2_t& a, vec2_t b) { a = a / b; return a; }
static vec3_t operator/=(vec3_t& a, vec3_t b) { a = a / b; return a; }
static vec4_t operator/=(vec4_t& a, vec4_t b) { a = a / b; return a; }

static vec2i_t operator/=(vec2i_t& a, vec2i_t b) { a = a / b; return a; }
static vec3i_t operator/=(vec3i_t& a, vec3i_t b) { a = a / b; return a; }
static vec4i_t operator/=(vec4i_t& a, vec4i_t b) { a = a / b; return a; }

static vec2_t operator/=(vec2_t& a, f32 b) { a = a / b; return a; }
static vec3_t operator/=(vec3_t& a, f32 b) { a = a / b; return a; }
static vec4_t operator/=(vec4_t& a, f32 b) { a = a / b; return a; }

static vec2i_t operator/=(vec2i_t& a, i32 b) { a = a / b; return a; }
static vec3i_t operator/=(vec3i_t& a, i32 b) { a = a / b; return a; }
static vec4i_t operator/=(vec4i_t& a, i32 b) { a = a / b; return a; }

// ----------- equal ------------ //

static bool operator==(vec2i_t a, vec2i_t b) { return a.x == b.x && a.y == b.y; }
static bool operator==(vec3i_t a, vec3i_t b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static bool operator==(vec4i_t a, vec4i_t b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

static bool operator!=(vec2i_t a, vec2i_t b) { return a.x != b.x || a.y != b.y; }
static bool operator!=(vec3i_t a, vec3i_t b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
static bool operator!=(vec4i_t a, vec4i_t b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }

static bool operator!=(color_t a, color_t b) { return a.r != b.r || a.g != b.g || a.b != b.b || a.a != b.a; }

#endif // __cplusplus

// ======================================= STATIC FUNCTIONS ==================================== //

static f32 sqrt_f32(f32 n) {
    f32 x = n * 0.5f;
    f32 y = n;
    int i = *(int*)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));

    return n * y;
}

static f32 rsqrt_f32(f32 n) {
	f32 x2 = n * 0.5f;
	f32 y  = n;
	int i  = *(long*)&y;           // evil floating point bit level hacking

	i = 0x5f3759df - (i >> 1);     // what the fuck? 
	y = *(f32*) &i;
	y = y * (1.5f - (x2 * y * y)); // 1st iteration

	return y;
}

static f32 shortest_angle_distance_f32(f32 a, f32 b) {
    f32 max = 2.0f * PI;
    f32 da  = fmodf(b - a, max);

    return fmodf(2.0f * da, max) - da;
}

static f32 lerp_angle_f32(f32 a, f32 b, f32 t) {
    return a + shortest_angle_distance_f32(a, b) * t;
}

static f32 clamp_f32(f32 n, f32 min, f32 max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

static f32 lerp_f32(f32 a, f32 b, f32 t) {
    return a + t * (b - a);
}

// ---------- from array ---------- //

static vec2_t v2_from_array(const f32* a) {
    return v2(a[0], a[1]);
}

static vec3_t v3_from_array(const f32* a) {
    return v3(a[0], a[1], a[2]);
}

static vec4_t v4_from_array(const f32* a) {
    return v4(a[0], a[1], a[2], a[3]);
}

static vec2i_t v2i_from_array(const i32* a) {
    return v2i(a[0], a[1]);
}

static vec3i_t v3i_from_array(const i32* a) {
    return v3i(a[0], a[1], a[2]);
}

static vec4i_t v4i_from_array(const i32* a) {
    return v4i(a[0], a[1], a[2], a[3]);
}

// ---------- from color ------------ //

static vec3_t v3_from_packed_color(u32 color) {
    return v3(
        ((color & 0x000000ff) >> 0)  / 256.0f,
        ((color & 0x0000ff00) >> 8)  / 256.0f,
        ((color & 0x00ff0000) >> 16) / 256.0f);
}

static vec4_t v4_from_packed_color(u32 color) {
    return v4(
        ((color & 0x000000ff) >> 0)  / 256.0f,
        ((color & 0x0000ff00) >> 8)  / 256.0f,
        ((color & 0x00ff0000) >> 16) / 256.0f,
        ((color & 0xff000000) >> 24) / 256.0f);
}

// ---------- identity ----------- //

static mat2_t m2_identity(void) {
    return m2(1, 0,
              0, 1);
}

static mat3_t m3_identity(void) {
    return m3(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1);
}

static mat4_t m4_identity(void) {
    return m4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

static quat_t quat_identity(void) {
    return quat(0, 0, 0, 1);
}

// --------- negate ---------- //

static vec2_t v2_neg(vec2_t u) {
    return v2(-u.x, -u.y);
}

static vec3_t v3_neg(vec3_t u) {
    return v3(-u.x, -u.y, -u.z);
}

static vec4_t v4_neg(vec4_t u) {
    return v4(-u.x, -u.y, -u.z, -u.w);
}

static vec2i_t v2i_neg(vec2i_t u) {
    return v2i(-u.x, -u.y);
}

static vec3i_t v3i_neg(vec3i_t u) {
    return v3i(-u.x, -u.y, -u.z);
}

static vec4i_t v4i_neg(vec4i_t u) {
    return v4i(-u.x, -u.y, -u.z, -u.w);
}

// ---------- addition ---------- //

static vec2_t v2_add(vec2_t a, vec2_t b) {
    return v2(a.x + b.x, a.y + b.y);
}

static vec3_t v3_add(vec3_t a, vec3_t b) {
    return v3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static vec4_t v4_add(vec4_t a, vec4_t b) {
    return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

static vec2i_t v2i_add(vec2i_t a, vec2i_t b) {
    return v2i(a.x + b.x, a.y + b.y);
}

static vec3i_t v3i_add(vec3i_t a, vec3i_t b) {
    return v3i(a.x + b.x, a.y + b.y, a.z + b.z);
}

static vec4i_t v4i_add(vec4i_t a, vec4i_t b) {
    return v4i(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

// -------- subtraction ------- //

static vec2_t v2_sub(vec2_t a, vec2_t b) {
    return v2(a.x - b.x, a.y - b.y);
}

static vec3_t v3_sub(vec3_t a, vec3_t b) {
    return v3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static vec4_t v4_sub(vec4_t a, vec4_t b) {
    return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

static vec2i_t v2i_sub(vec2i_t a, vec2i_t b) {
    return v2i(a.x - b.x, a.y - b.y);
}

static vec3i_t v3i_sub(vec3i_t a, vec3i_t b) {
    return v3i(a.x - b.x, a.y - b.y, a.z - b.z);
}

static vec4i_t v4i_sub(vec4i_t a, vec4i_t b) {
    return v4i(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// -------- multiplication ------- //

static vec2_t v2_mul(vec2_t a, vec2_t b) {
    return v2(a.x * b.x, a.y * b.y);
}

static vec3_t v3_mul(vec3_t a, vec3_t b) {
    return v3(a.x * b.x, a.y * b.y, a.z * b.z);
}

static vec4_t v4_mul(vec4_t a, vec4_t b) {
    return v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * a.w);
}

static vec2i_t v2i_mul(vec2i_t a, vec2i_t b) {
    return v2i(a.x * b.x, a.y * b.y);
}

static vec3i_t v3i_mul(vec3i_t a, vec3i_t b) {
    return v3i(a.x * b.x, a.y * b.y, a.z * a.z);
}

static vec4i_t v4i_mul(vec4i_t a, vec4i_t b) {
    return v4i(a.x * b.x, a.y * b.y, a.z * a.z, a.w * a.w);
}

static vec2_t m2_mulv(mat2_t m, vec2_t u) {
    return v2(
        m.e[0] * u.x + m.e[2] * u.y,
        m.e[1] * u.x + m.e[3] * u.y);
}

static vec3_t m3_mulv(mat3_t m, vec3_t u) {
    return v3(
        m.e[0] * u.x + m.e[3] * u.y + m.e[6] * u.z,
        m.e[1] * u.x + m.e[4] * u.y + m.e[7] * u.z,
        m.e[2] * u.x + m.e[5] * u.y + m.e[8] * u.z);
}

static vec4_t m4_mulv(mat4_t m, vec4_t u) {
    return v4(
        m.e[0] * u.x + m.e[4] * u.y + m.e[8]  * u.z + m.e[12] * u.w,
        m.e[1] * u.x + m.e[5] * u.y + m.e[9]  * u.z + m.e[13] * u.w,
        m.e[2] * u.x + m.e[6] * u.y + m.e[10] * u.z + m.e[14] * u.w,
        m.e[3] * u.x + m.e[7] * u.y + m.e[11] * u.z + m.e[15] * u.w);
}

static mat2_t m2_mul(mat2_t a, mat2_t b) {
    return m2(
        a.e[0] * b.e[0] + a.e[2] * b.e[1],
        a.e[1] * b.e[0] + a.e[3] * b.e[1],
        a.e[0] * b.e[2] + a.e[2] * b.e[3],
        a.e[1] * b.e[2] + a.e[3] * b.e[3]);
}

static mat3_t m3_mul(mat3_t a, mat3_t b) {
    return m3(
        a.e[0] * b.e[0] + a.e[3] * b.e[1]  + a.e[6] * b.e[2],
        a.e[1] * b.e[0] + a.e[4] * b.e[1]  + a.e[7] * b.e[2],
        a.e[2] * b.e[0] + a.e[5] * b.e[1]  + a.e[8] * b.e[2],

        a.e[0] * b.e[3] + a.e[3] * b.e[4]  + a.e[6] * b.e[5],
        a.e[1] * b.e[3] + a.e[4] * b.e[4]  + a.e[7] * b.e[5],
        a.e[2] * b.e[3] + a.e[5] * b.e[4]  + a.e[8] * b.e[5],

        a.e[0] * b.e[6] + a.e[3] * b.e[7]  + a.e[6] * b.e[8],
        a.e[1] * b.e[6] + a.e[4] * b.e[7]  + a.e[7] * b.e[8],
        a.e[2] * b.e[6] + a.e[5] * b.e[7]  + a.e[8] * b.e[8]);
}

static mat4_t m4_mul(mat4_t a, mat4_t b) {
    return m4(
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
        a.e[3] * b.e[12] + a.e[7] * b.e[13] + a.e[11] * b.e[14] + a.e[15] * b.e[15]);
}

static quat_t quat_mul(quat_t a, quat_t b) {
    return quat(
        a.y * b.z - a.z * b.y + a.w * b.x + b.w * a.x,
        a.z * b.x - a.x * b.z + a.w * b.y + b.w * a.y,
        a.x * b.y - a.y * b.x + a.w * b.z + b.w * a.z,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

// ------------ divition ------------ //

static vec2_t v2_div(vec2_t a, vec2_t b) {
    return v2(a.x / b.x, a.y / b.y);
}

static vec3_t v3_div(vec3_t a, vec3_t b) {
    return v3(a.x / b.x, a.y / b.y, a.z / b.z);
}

static vec4_t v4_div(vec4_t a, vec4_t b) {
    return v4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

static vec2i_t v2i_div(vec2i_t a, vec2i_t b) {
    return v2i(a.x / b.x, a.y / b.y);
}

static vec3i_t v3i_div(vec3i_t a, vec3i_t b) {
    return v3i(a.x / b.x, a.y / b.y, a.z / b.z);
}

static vec4i_t v4i_div(vec4i_t a, vec4i_t b) {
    return v4i(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

// ------------- scaling ------------- //

static vec2_t v2_scale(vec2_t a, f32 s) {
    return v2(a.x * s, a.y * s);
}

static vec3_t v3_scale(vec3_t a, f32 s) {
    return v3(a.x * s, a.y * s, a.z * s);
}

static vec4_t v4_scale(vec4_t a, f32 s) {
    return v4(a.x * s, a.y * s, a.z * s, a.w * s);
}

static vec2i_t v2i_scale(vec2i_t a, i32 s) {
    return v2i(a.x * s, a.y * s);
}

static vec3i_t v3i_scale(vec3i_t a, i32 s) {
    return v3i(a.x * s, a.y * s, a.z * s);
}

static vec4i_t v4i_scale(vec4i_t a, i32 s) {
    return v4i(a.x * s, a.y * s, a.z * s, a.w * s);
}

// ----------- dot product ----------- //

static f32 v2_dot(vec2_t a, vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

static f32 v3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static f32 v4_dot(vec4_t a, vec4_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static i32 v2i_dot(vec2i_t a, vec2i_t b) {
    return a.x * b.x + a.y * b.y;
}

static i32 v3i_dot(vec3i_t a, vec3i_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static i32 v4i_dot(vec4i_t a, vec4i_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// ----------- length squared ----------- //

static f32 v2_len_sq(vec2_t u) {
    return v2_dot(u, u);
}

static f32 v3_len_sq(vec3_t u) {
    return v3_dot(u, u);
}

static f32 v4_len_sq(vec4_t u) {
    return v4_dot(u, u);
}

static i32 v2i_len_sq(vec2i_t u) {
    return v2i_dot(u, u);
}

static i32 v3i_len_sq(vec3i_t u) {
    return v3i_dot(u, u);
}

static i32 v4i_len_sq(vec4i_t u) {
    return v4i_dot(u, u);
}

// -------------- length -------------- //

static f32 v2_len(vec2_t u) {
    return sqrt_f32(v2_len_sq(u));
}

static f32 v3_len(vec3_t u) {
    return sqrt_f32(v3_len_sq(u));
}

static f32 v4_len(vec4_t u) {
    return sqrt_f32(v4_len_sq(u));
}

// -------------- distance squared -------------- //

static f32 v2_dist_sq(vec2_t a, vec2_t b) {
    return v2_len_sq(v2_sub(a, b));
}

static f32 v3_dist_sq(vec3_t a, vec3_t b) {
    return v3_len_sq(v3_sub(a, b));
}

static f32 v4_dist_sq(vec4_t a, vec4_t b) {
    return v4_len_sq(v4_sub(a, b));
}

static i32 v2i_dist_sq(vec2i_t a, vec2i_t b) {
    return v2i_len_sq(v2i_sub(a, b));
}

static i32 v3i_dist_sq(vec3i_t a, vec3i_t b) {
    return v3i_len_sq(v3i_sub(a, b));
}

static i32 v4i_dist_sq(vec4i_t a, vec4i_t b) {
    return v4i_len_sq(v4i_sub(a, b));
}

// ------------------ distance ------------------- //

static f32 v2_dist(vec2_t a, vec2_t b) {
    return sqrt_f32(v2_dist_sq(a, b));
}

static f32 v3_dist(vec3_t a, vec3_t b) {
    return sqrt_f32(v3_dist_sq(a, b));
}

static f32 v4_dist(vec4_t a, vec4_t b) {
    return sqrt_f32(v4_dist_sq(a, b));
}

// -------------- manhattan distance -------------- //

static i32 v2i_manhattan(vec2i_t a, vec2i_t b) {
    vec2i_t diff = v2i_sub(a, b);

    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y);
}

static i32 v3i_manhattan(vec3i_t a, vec3i_t b) {
    vec3i_t diff = v3i_sub(a, b);

    return (0x7ffffffff & diff.x) + (0x7ffffffff & diff.y) + (0x7ffffffff & diff.z);
}

// -------------- normalize --------------- //

static vec2_t v2_norm(vec2_t u) {
    return v2_scale(u, rsqrt_f32(v2_dot(u, u)));
}

static vec3_t v3_norm(vec3_t u) {
    return v3_scale(u, rsqrt_f32(v3_dot(u, u)));
}

static vec4_t v4_norm(vec4_t u) {
    return v4_scale(u, rsqrt_f32(v4_dot(u, u)));
}

// ---------------- min ----------------- //

static vec2_t v2_min(vec2_t a, vec2_t b) {
    return v2(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y);
}

static vec3_t v3_min(vec3_t a, vec3_t b) {
    return v3(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z);
}

static vec4_t v4_min(vec4_t a, vec4_t b) {
    return v4(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w);
}

static vec2i_t v2i_min(vec2i_t a, vec2i_t b) {
    return v2i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y);
}

static vec3i_t v3i_min(vec3i_t a, vec3i_t b) {
    return v3i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z);
}

static vec4i_t v4i_min(vec4i_t a, vec4i_t b) {
    return v4i(
        a.x < b.x? a.x : b.x,
        a.y < b.y? a.y : b.y,
        a.z < b.z? a.z : b.z,
        a.w < b.w? a.w : b.w);
}

// ----------- equal ------------ //

static b32 v2i_equal(vec2i_t a, vec2i_t b) {
    return a.x == b.x && a.y == b.y;
}

static b32 v3i_equal(vec3i_t a, vec3i_t b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static b32 v4i_equal(vec4i_t a, vec4i_t b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

// ---------------- max ----------------- //

static vec2_t v2_max(vec2_t a, vec2_t b) {
    return v2(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y);
}

static vec3_t v3_max(vec3_t a, vec3_t b) {
    return v3(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z);
}

static vec4_t v4_max(vec4_t a, vec4_t b) {
    return v4(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w);
}

static vec2i_t v2i_max(vec2i_t a, vec2i_t b) {
    return v2i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y);
}

static vec3i_t v3i_max(vec3i_t a, vec3i_t b) {
    return v3i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z);
}

static vec4i_t v4i_max(vec4i_t a, vec4i_t b) {
    return v4i(
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z,
        a.w > b.w? a.w : b.w);
}

// ---------------- lerp ----------------- //

static vec2_t v2_lerp(vec2_t a, vec2_t b, f32 t) {
    return v2(a.x + t * (b.x - a.x),
              a.y + t * (b.y - a.y));
}

static vec3_t v3_lerp(vec3_t a, vec3_t b, f32 t) {
    return v3(a.x + t * (b.x - a.x),
              a.y + t * (b.y - a.y),
              a.z + t * (b.z - a.z));
}

static vec4_t v4_lerp(vec4_t a, vec4_t b, f32 t) {
    return v4(a.x + t * (b.x - a.x),
              a.y + t * (b.y - a.y),
              a.z + t * (b.z - a.z),
              a.w + t * (b.w - a.w));
}

static color_t color_lerp(color_t a, color_t b, f32 t) {
    return color((u8)(a.r + t * (b.r - a.r)),
                 (u8)(a.g + t * (b.g - a.g)),
                 (u8)(a.b + t * (b.b - a.b)),
                 (u8)(a.a + t * (b.a - a.a)));
}

// --------------- cross ------------------- //

static vec3_t v3_cross(vec3_t a, vec3_t b) {
    return v3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

// --------------- get angle ------------------- //

static f32 v2_get_angle(vec2_t a, vec2_t b) {
    f32 det = a.x * b.y - b.x * a.y;
    f32 dot = a.x * b.x + a.y * b.y;

    return atan2f(det, dot);
}

// ------------------ transform/scale/rotate ------------------ //

static mat2_t m2_rotate(f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    return m2(c, s, -s, c);
}

static mat3_t m3_rotate(vec3_t axis, f32 angle) {
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    f32 k = 1.0f - c;

    vec3_t sa   = { s * axis.x, s * axis.y, s * axis.z };
    vec3_t omca = { k * axis.x, k * axis.y, k * axis.z };

    return m3(
        omca.x * axis.x + c,
        omca.x * axis.y - sa.z,
        omca.x * axis.z + sa.y,

        omca.y * axis.x + sa.z,
        omca.y * axis.y + c,
        omca.y * axis.z - sa.x,

        omca.z * axis.x - sa.y,
        omca.z * axis.y + sa.x,
        omca.z * axis.z + c);
}

static mat4_t m4_rotate(vec3_t axis, f32 angle) {
    f32 cosv = cosf(angle);
    f32 sinv = sinf(angle);
    f32 inv_cosv = 1.0f - cosv;

    vec3_t sa   = { axis.x * sinv,      axis.y * sinv,      axis.z * sinv };
    vec3_t omca = { axis.x * inv_cosv,  axis.y * inv_cosv,  axis.z * inv_cosv };

    return m4(
        omca.x * axis.x + cosv,  omca.x * axis.y - sa.x,  omca.x * axis.z + sa.y, 0,
        omca.y * axis.x + sa.z,  omca.y * axis.y + cosv,  omca.y * axis.z - sa.x, 0,
        omca.z * axis.x - sa.y,  omca.z * axis.y + sa.x,  omca.z * axis.z + cosv, 0,
        0,  0,  0,  1);
}

static quat_t quat_rotate(vec3_t axis, f32 angle) {
    f32    s = sinf(0.5f * angle);
    vec3_t v = { s * axis.x, s * axis.y, s * axis.z };

    return quat(v.x, v.y, v.z, cosf(0.5f * angle));
}

static mat4_t m4_translate(f32 x, f32 y, f32 z) {
    return m4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1);
}

static mat4_t m4_scale(f32 x, f32 y, f32 z) {
    return m4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1);
}

// --------------- view matricies --------------- //

static mat4_t m4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    return m4(
        2 / (r - l),            0,                      0,                      0,
        0,                      2 / (t - b),            0,                      0,
        0,                      0,                      -2 / (f - n),           0,
        -(r + l) / (r - l),     -(t + b) / (t - b),     -(f + n) / (f - n),     1);
}

static mat4_t m4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f) {
    f32 a = 1.0f / tanf(y_fov / 2.0f);

    return m4(
        a / aspect,     0,      0,                          0,
        0,              a,      0,                          0,
        0,              0,      -((f + n) / (f - n)),      -1,
        0,              0,      -((2 * f * n) / (f - n)),   0);
}

static mat4_t m4_look_at(vec3_t eye, vec3_t center, vec3_t up) {
    vec3_t f = v3_norm(v3(center.x - eye.x, center.y - eye.y, center.z - eye.z));
    vec3_t s = v3_norm(v3_cross(f, up));
    vec3_t t = v3_cross(s, f);

    mat4_t m = {0};

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

static plane_t plane_normalize(plane_t plane) {
    f32 mag = sqrt_f32(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;

    return plane;
}

static frustum_t frustum_create(mat4_t m, b32 normalize) {
    frustum_t frustum = {0};

    // left clipping plane
    frustum.plane[0].a = m.e[3]  + m.e[0];
    frustum.plane[0].b = m.e[7]  + m.e[4];
    frustum.plane[0].c = m.e[11] + m.e[8];
    frustum.plane[0].d = m.e[15] + m.e[12];

    // right clipping plane
    frustum.plane[1].a = m.e[3]  - m.e[0];
    frustum.plane[1].b = m.e[7]  - m.e[4];
    frustum.plane[1].c = m.e[11] - m.e[8];
    frustum.plane[1].d = m.e[15] - m.e[12];

    // top clipping plane
    frustum.plane[2].a = m.e[3]  - m.e[1];
    frustum.plane[2].b = m.e[7]  - m.e[5];
    frustum.plane[2].c = m.e[11] - m.e[9];
    frustum.plane[2].d = m.e[15] - m.e[13];

    // bottom clipping plane
    frustum.plane[3].a = m.e[3]  + m.e[1];
    frustum.plane[3].b = m.e[7]  + m.e[5];
    frustum.plane[3].c = m.e[11] + m.e[9];
    frustum.plane[3].d = m.e[15] + m.e[13];

    // near clipping plane
    frustum.plane[4].a = m.e[3]  + m.e[2];
    frustum.plane[4].b = m.e[7]  + m.e[6];
    frustum.plane[4].c = m.e[11] + m.e[10];
    frustum.plane[4].d = m.e[15] + m.e[14];

    // far clipping plane
    frustum.plane[5].a = m.e[3]  - m.e[2];
    frustum.plane[5].b = m.e[7]  - m.e[6];
    frustum.plane[5].c = m.e[11] - m.e[10];
    frustum.plane[5].d = m.e[15] - m.e[14];

    if (normalize) {
        frustum.plane[0] = plane_normalize(frustum.plane[0]);
        frustum.plane[1] = plane_normalize(frustum.plane[1]);
        frustum.plane[2] = plane_normalize(frustum.plane[2]);
        frustum.plane[3] = plane_normalize(frustum.plane[3]);
        frustum.plane[4] = plane_normalize(frustum.plane[4]);
        frustum.plane[5] = plane_normalize(frustum.plane[5]);
    }
    
    return frustum;
}

// ------------------ contains ------------------ //

static b32 circle_contains(circle_t circle, vec2_t pos) {
    // TODO:
    return false;
}

static b32 sphere_contains(sphere_t sphere, vec3_t pos) {
    // TODO:
    return false;
}

static b32 rect2_contains(rect2_t rect, vec2_t pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static b32 rect3_contains(rect3_t rect, vec3_t pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static b32 rect2i_contains(rect2i_t rect, vec2i_t pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;

    return true;
}

static b32 rect3i_contains(rect3i_t rect, vec3i_t pos) {
    if (pos.x < rect.min.x || pos.x > rect.max.x) return false;
    if (pos.y < rect.min.y || pos.y > rect.max.y) return false;
    if (pos.z < rect.min.z || pos.z > rect.max.z) return false;

    return true;
}

static b32 frustum_contains(frustum_t frustum, vec3_t pos) {
    for (i32 i = 0; i < 6; i++) {
		if (frustum.plane[i].a * pos.x + frustum.plane[i].b * pos.y + frustum.plane[i].c * pos.z + frustum.plane[i].d <= 0)
			return false;
	}

	return true;
}

// ------------------ intersect ------------------ //

static b32 circle_intersect(circle_t a, circle_t b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static b32 sphere_intersect(sphere_t a, sphere_t b) {
    f32 dx  = b.pos.x - a.pos.x;
    f32 dy  = b.pos.y - a.pos.y;
    f32 dz  = b.pos.z - a.pos.z;

    f32 rt  = a.rad + b.rad;

    return (dx * dx + dy * dy) < (rt * rt);
}

static b32 rect2_intersect(rect2_t a, rect2_t b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static b32 rect3_intersect(rect3_t a, rect3_t b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static b32 rect2i_intersect(rect2i_t a, rect2i_t b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

static b32 rect3i_intersect(rect3i_t a, rect3i_t b) {
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;
    if (a.min.z > b.max.z || a.max.z < b.min.z) return false;

    return true;
}

static b32 frustum_intersect_sphere(frustum_t frustum, sphere_t sphere) {
    for (i32 i = 0; i < 6; i++) {
		if(frustum.plane[i].a * sphere.pos.x + frustum.plane[i].b * sphere.pos.y + frustum.plane[i].c * sphere.pos.z + frustum.plane[i].d <= -sphere.rad) {
			return false;
		}
	}

	return true;
}

static b32 frustum_intersect_rect3(frustum_t frustum, rect3_t rect) {
    for (int i = 0; i < 6; i++) {
		if (frustum.plane[i].a * rect.min.x + frustum.plane[i].b * rect.min.y + frustum.plane[i].c * rect.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.max.x + frustum.plane[i].b * rect.min.y + frustum.plane[i].c * rect.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.min.x + frustum.plane[i].b * rect.max.y + frustum.plane[i].c * rect.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.max.x + frustum.plane[i].b * rect.max.y + frustum.plane[i].c * rect.min.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.min.x + frustum.plane[i].b * rect.min.y + frustum.plane[i].c * rect.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.max.x + frustum.plane[i].b * rect.min.y + frustum.plane[i].c * rect.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.min.x + frustum.plane[i].b * rect.max.y + frustum.plane[i].c * rect.max.z + frustum.plane[i].d > 0) continue;
		if (frustum.plane[i].a * rect.max.x + frustum.plane[i].b * rect.max.y + frustum.plane[i].c * rect.max.z + frustum.plane[i].d > 0) continue;

		return false;
	}

	return true;
}

// ------------------- get overlap --------------- //

static rect2_t rect2_get_overlap(rect2_t a, rect2_t b) {
    rect2_t result = {
        v2_max(a.min, b.min),
        v2_min(a.max, b.max),
    };

    return result;
}

static rect3_t rect3_get_overlap(rect3_t a, rect3_t b) {
    rect3_t result = {
        v3_max(a.min, b.min),
        v3_min(a.max, b.max),
    };

    return result;
}

static rect2i_t rect2i_get_overlap(rect2i_t a, rect2i_t b) {
    rect2i_t result = {
        v2i_max(a.min, b.min),
        v2i_min(a.max, b.max),
    };

    return result;
}

static rect3i_t rect3i_get_overlap(rect3i_t a, rect3i_t b) {
    rect3i_t result = {
        v3i_max(a.min, b.min),
        v3i_min(a.max, b.max),
    };

    return result;
}

// -------------- get intersect vector ---------- //

static vec2_t circle_get_intersect_vector(circle_t a, circle_t b) {
    vec2_t  delta = { a.pos.x - b.pos.x, a.pos.y - b.pos.y };
    f32     depth = v2_len(delta) - (a.rad + b.rad);
    
    return v2(-depth * delta.x, -depth * delta.y);
}

static vec3_t sphere_get_intersect_vector(sphere_t a, sphere_t b) {
    vec3_t  delta = { b.pos.x - a.pos.x, b.pos.y - a.pos.y, b.pos.z - a.pos.z };
    f32     depth = v3_len(delta) - (a.rad + b.rad);
    
    return v3(-depth * delta.x, -depth * delta.y, -depth * delta.z);
}

static vec2_t rect2_get_intersect_vector(rect2_t a, rect2_t b) {
    rect2_t o  = rect2_get_overlap(a, b);

    f32 dx = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32 dy = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);

    return v2(
        SIGN(dx) * (o.max.x - o.min.x),
        SIGN(dy) * (o.max.y - o.min.y));
}

static vec3_t rect3_get_intersect_vector(rect3_t a, rect3_t b) {
    rect3_t o = rect3_get_overlap(a, b);

    f32 dx = 0.5f * (a.min.x + a.max.x) - 0.5f * (b.min.x + b.max.x);
    f32 dy = 0.5f * (a.min.y + a.max.y) - 0.5f * (b.min.y + b.max.y);
    f32 dz = 0.5f * (a.min.z + a.max.z) - 0.5f * (b.min.z + b.max.z);

    return v3(
        SIGN(dx) * (o.max.x - o.min.x),
        SIGN(dy) * (o.max.y - o.min.y),
        SIGN(dz) * (o.max.z - o.min.z));
}

static vec2i_t rect2i_get_intersect_vector(rect2i_t a, rect2i_t b) {
    rect2i_t o = rect2i_get_overlap(a, b);

    i32  dx = (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2;
    i32  dy = (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2;

    return v2i(
        SIGN(dx) * (o.max.x - o.min.x),
        SIGN(dy) * (o.max.y - o.min.y));
}

static vec3i_t rect3i_get_intersect_vector(rect3i_t a, rect3i_t b) {
    rect3i_t o = rect3i_get_overlap(a, b);

    i32 dx = (a.min.x + a.max.x) / 2 - (b.min.x + b.max.x) / 2;
    i32 dy = (a.min.y + a.max.y) / 2 - (b.min.y + b.max.y) / 2;
    i32 dz = (a.min.z + a.max.z) / 2 - (b.min.z + b.max.z) / 2;

    return v3i(
        SIGN(dx) * (o.max.x - o.min.x),
        SIGN(dy) * (o.max.y - o.min.y),
        SIGN(dz) * (o.max.z - o.min.z));
}

// ---------------------- random ------------------------ //

static u32 rand_u32(u32* state) {
    u32 x = *state;

	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

	return *state = x;
}

static i32 rand_i32(u32* state, i32 min, i32 max) {
    return min + rand_u32(state) % (max - min);
}

static f32 rand_f32(u32* state, f32 min, f32 max) {
    return min + ((f32)rand_u32(state) / (f32)0xFFFFFFFF) * (max - min); 
}

static vec2_t rand_unit_v2(u32* state) {
    vec2_t out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1) };

    return v2_norm(out);
}

static vec3_t rand_unit_v3(u32* state) {
    vec3_t out = { rand_f32(state, -1, 1), rand_f32(state, -1, 1), rand_f32(state, -1, 1) };

    return v3_norm(out);
}

static vec2_t rand_v2(u32* state, f32 min, f32 max) {
    return v2_scale(rand_unit_v2(state), rand_f32(state, min, max));
}

static vec3_t rand_v3(u32* state, f32 min, f32 max) {
    return v3_scale(rand_unit_v3(state), rand_f32(state, min, max));
}

// ----------------------- hash ------------------------- //

static u32 hash_u32(u32 a) {
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 0x27d4eb2d;
    a = a ^ (a >> 15);

    return a;
}

static u32 hash_i32(i32 a) {
    union { u32 u; i32 i; } convert;
    convert.i = a;
    return hash_u32(convert.u);
}

static u32 hash_str(const char* str) {
    u32 hash = 0;

    while (*str) {
        hash = (hash << 7) + (hash >> 25) + *str++;
    }

    return hash + (hash >> 16);
}

static const u32 crc_table[] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

static u32 hash_mem(const void *data, u32 size) {
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

static u32 hash_v2i(vec2i_t k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1);
}

static u32 hash_v3i(vec3i_t k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2);
}

static u32 hash_v4i(vec4i_t k) {
    u32 a = hash_i32(k.x);
    u32 b = hash_i32(k.y);
    u32 c = hash_i32(k.z);
    u32 d = hash_i32(k.w);

    return (a * HASH_PRIME0) ^ (b * HASH_PRIME1) ^ (c * HASH_PRIME2) ^ (d * HASH_PRIME3);
}

// --------------------- packed color u32 -------------------- //

static u32 pack_color_u8(u8 r, u8 g, u8 b, u8 a) {
    u32 color = 0;

    color |= (u32)(r) << 0;
    color |= (u32)(g) << 8;
    color |= (u32)(b) << 16;
    color |= (u32)(a) << 24;

    return color;
}

static u32 pack_color_f32(f32 r, f32 g, f32 b, f32 a) {
    return pack_color_u8((u8)(255 * r), (u8)(255 * g), (u8)(255 * b), (u8)(255 * a));
}

static u32 pack_color_v4(vec4_t color) {
    return pack_color_f32(color.r, color.g, color.b, color.a);
}

static u32 pack_color_v3(vec3_t color) {
    return pack_color_f32(color.r, color.g, color.b, 1.0);
}

// ---------------------- priority queue --------------------- //

static b32 queue_empty(const priority_queue_t* queue) {
    return queue->len == 0;
}

static void queue_clear(priority_queue_t* queue) {
    queue->len = 0;
}

static void queue_push(priority_queue_t* queue, vec2i_t e, f32 weight) {
    queue_node_t node = { weight, e };

    int i = queue->len + 1;
    int j = i / 2;

    while (i > 1 && queue->array[j].weight > node.weight) {
        queue->array[i] = queue->array[j];

        i = j;
        j = j / 2;
    }

    queue->array[i] = node;
    queue->len++;
}

static f32 queue_pop(vec2i_t* out, priority_queue_t* queue) {
    queue_node_t data = queue->array[1];

    queue->array[1] = queue->array[queue->len];
    queue->len--;

    int i = 1;
    while (i != queue->len + 1)
    {
        int k = queue->len + 1;
        int j = 2 * i;

        if (j <= queue->len && queue->array[j].weight < queue->array[k].weight)
            k = j;

        if (j + 1 <= queue->len && queue->array[j + 1].weight < queue->array[k].weight)
            k = j + 1;

        queue->array[i] = queue->array[k];
        i = k;
    }

    *out = data.e;
    return data.weight;
}

// -------------------- f64 matrix funcs ------------------- //

static void f4x4_mul_64(f64 *R, const f64 *a, const f64 *b) {
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

static void f4x4_mulv_64(f64 *out, const f64 *M, const f64 *v) {
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

static void f4x4_invert_64(f64* T, const f64* M) {
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

static b32 f4x4_project_64(f64* result, f64 objx, f64 objy, f64 objz, f64* modelview, f64* projection, int* viewport) {
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

static b32 f4x4_unproject_64(f64* result, f64 winx, f64 winy, f64 winz, f64* modelview, f64* projection, int* viewport) {
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

// ============================================================================================ //
// ======================================= IMPLEMENTATION ===================================== //
// ============================================================================================ //
#ifdef ATS_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ---------------------- arena allocator ------------------------ //

extern memory_arena_t ma_create(u8* buffer, u32 size) {
    memory_arena_t ma = ATS_INIT_ZERO;

    ma.cap    = size;
    ma.buffer = buffer;

    return ma;
}

extern void* ma_alloc(memory_arena_t* ma, u32 byte_size) {
    byte_size = ALIGN_UP(byte_size, 16);

    assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

    void* memory  = ma->buffer + ma->index;
    ma->index     += byte_size;
    ma->max       = ma->max > ma->index? ma->max : ma->index;

    memset(memory, 0, byte_size);

    return memory;
}

extern void* ma_begin(memory_arena_t* ma) {
    ma->lock = true;
    return ma->buffer + ma->index;
}

extern void ma_end(memory_arena_t* ma, u32 byte_size) {
    ma->index += ALIGN_UP(byte_size, 16);
    ma->lock = false;
}

extern void ma_save(memory_arena_t* ma) {
    assert(ma->top < ma->cap);
    ma->stack[ma->top++] = ma->index;
}

extern void ma_restore(memory_arena_t* ma) {
    assert(ma->top > 0);
    ma->index = ma->stack[--ma->top];
}

extern void ma_validate(memory_arena_t* ma) {
    assert(ma->top == 0);
}

// --------------------- read/write files ------------------------- //

static size_t file_get_size(FILE* fp) {
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return size;
}

extern char* file_read_str(const char* file_name, memory_arena_t* ma) {
    FILE *fp      = NULL;
    char *buffer  = NULL;
    
    if (fopen_s(&fp, file_name, "rb") == 0) {
        size_t size = file_get_size(fp);

        memory_arena_t state = *ma;
        buffer = (char*)ma_alloc(ma, size + 1);

        if (buffer) {
            buffer[size] = 0;

            if (fread(buffer, 1, size, fp) == 0) {
                *ma = state;
                buffer = NULL;
            }
        }

        fclose(fp);
    }

    return buffer;
}

extern b32 file_write_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "w") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);

        fclose(fp);
        return n == size;
    }

    return false;
}

extern b32 file_append_str(const char* file_name, const char* buffer) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "a") == 0) {
        size_t size = strlen(buffer);
        size_t n = fwrite(buffer, 1, size, fp);

        fclose(fp);
        return n == size;
    }

    return false;
}

extern b32 file_read_bin(const char* file_name, void* buffer, u32 size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "rb") == 0) {
        fread(buffer, size, 1, fp);
        fclose(fp);

        return true;
    }

    return false;
} 

extern b32 file_write_bin(const char* file_name, const void* buffer, u32 size) {
    FILE *fp = NULL;

    if (fopen_s(&fp, file_name, "wb") == 0) {
        fwrite(buffer, size, 1, fp);
        fclose(fp);
        return 1;
    }

    return false;
}

// --------------------- image ----------------------- //

#ifndef ATS_NO_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h" 

extern image_t image_load_from_file(const char* path) {
    image_t image = {0};
    i32 channels = 0;

    image.pixels = (u32*)stbi_load(path, &image.width, &image.height, &channels, 4);
    assert(image.pixels);

    return image;
}

extern u32 image_get_pixel(image_t* img, i32 x, i32 y) {
    return img->pixels[y * img->width + x];
}

extern void image_set_pixel(image_t* img, i32 x, i32 y, u32 pixel) {
    img->pixels[y * img->width + x] = pixel;
}

#endif

#endif // ATS_IMPL

