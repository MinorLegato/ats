
typedef u32 TimerType;
enum {
#define TIMER_TYPE(name) TIMER_##name,
    TIMER_TYPES
#undef TIMER_TYPE
    TIMER_COUNT
};

struct TimerEntry {
    b32     in_use;

    f32     start;
    f32     stop;
};

static const char* timer_name[TIMER_COUNT] = {
#define TIMER_TYPE(name) #name,
    TIMER_TYPES
#undef TIMER_TYPE
};

static TimerEntry timer_table[TIMER_COUNT];

#define TimerScope(name) Defer(TimerStart(TIMER_##name), TimerStop(TIMER_##name))

static void TimerStart(TimerType type) {
    timer_table[type].in_use = true;
    timer_table[type].start  = TimerGetCurrent();
}

static void TimerStop(TimerType type) {
    timer_table[type].stop = TimerGetCurrent();
}

static void TimerPrintResult(f32 px, f32 py, f32 sx, f32 sy) {
    i32 y = 0;
    for (i32 i = 0; i < TIMER_COUNT; ++i) {
        TimerEntry e = timer_table[i];

        if (e.in_use) {
            GL_RenderStringFormat(px, py + y * (sy + 1), 0, sx, sy, 0xff77ccff, "%s : %.2f", timer_name[i], 1000.0f * (e.stop - e.start));
            y++;
        }
    }

    MemoryClear(timer_table, sizeof *timer_table);
}

