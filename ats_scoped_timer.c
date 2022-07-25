
typedef struct timer_entry {
    const char* name;

    f64 start;
    f64 stop;

    usize depth;
} timer_entry;

static usize timer_top;
static timer_entry timer_stack[512];

static usize timer_count;
static timer_entry timer_array[512];

#define timer_scope(name) defer(timer_start(name), timer_stop())

static void
timer_start(const char* name) {
    timer_entry* entry = timer_stack + (timer_top++);

    entry->name = name;
    entry->start = timer_get_current();
    entry->stop = 0;
    entry->depth = timer_top - 1;
}

static void
timer_stop(void) {
    timer_entry* entry = timer_stack + (--timer_top);
    entry->stop = timer_get_current();
    timer_array[timer_count++] = *entry;
}

static void
timer_reset_all(void) {
    timer_top = 0;
    timer_count = 0;
}

static void
timer_print_result(f32 px, f32 py, f32 sx, f32 sy) {
    glEnable(GL_TEXTURE_2D);
    i32 y = 0;
    for_range(i, 0, timer_count) {
        timer_entry e = timer_array[i];
        gl_string_format(px + 4 * e.depth, py + y * (sy + 1), 0, sx, sy, 0xff77ccff, "%s : %.2f", e.name, 1000.0 * (e.stop - e.start));
        y++;
    }
}

