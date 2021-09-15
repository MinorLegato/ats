
typedef u32 timer_type_t;
enum {
#define TIMER_TYPE(name) TIMER_##name,
    TIMER_TYPES
#undef TIMER_TYPE
    TIMER_COUNT
};

typedef struct timer_entry_t timer_entry_t;
struct timer_entry_t {
    b32     in_use;

    f32     start;
    f32     stop;
};

static const char* timer_name[TIMER_COUNT] = {
#define TIMER_TYPE(name) #name,
    TIMER_TYPES
#undef TIMER_TYPE
};

static timer_entry_t timer_table[TIMER_COUNT];

#define timer_scope(name) defer(timer_start(TIMER_##name), timer_stop(TIMER_##name))

static void timer_start(timer_type_t type) {
    timer_table[type].in_use = true;
    timer_table[type].start  = timer_get_current();
}

static void timer_stop(timer_type_t type) {
    timer_table[type].stop = timer_get_current();
}

static void timer_print_result(f32 px, f32 py, f32 sx, f32 sy) {
    i32 y = 0;
    for (i32 i = 0; i < TIMER_COUNT; ++i) {
        timer_entry_t e = timer_table[i];

        if (e.in_use) {
            gl_render_string_format(px, py + y * (sy + 1), 0, sx, sy, 0xff77ccff, "%s : %.2f", timer_name[i], 1000.0f * (e.stop - e.start));
            y++;
        }
    }
    memset(timer_table, 0, sizeof *timer_table);
}

