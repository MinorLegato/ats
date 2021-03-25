
#define TIMER_LOG_MAX (2048)

static u32  timer_log_count;
static char timer_log_array[TIMER_LOG_MAX][512];

static int timer_current_indent_level;
static int timer_indent_array[TIMER_LOG_MAX];

struct ScopedTimer {
    f64             time_point;
    const char*     timer_name;
    char*           entry;

    explicit ScopedTimer(const char* name) {
        timer_indent_array[timer_log_count] = timer_current_indent_level++;

        timer_name  = name;
        entry       = timer_log_array[timer_log_count++];
        time_point  = getCurrentTime();
    }

    ~ScopedTimer() {
        f64 dt = getCurrentTime() - time_point;

        sprintf(entry, "%s --- ms: %.2f, micro: %.2f", timer_name, 1000 * dt, (1000 * 1000) * dt);

        timer_current_indent_level--;
    }
};

void renderScopedTimerLog(v2 text_pos, v2 text_scale, b32 show = true) {
    if (show) {
        for (u32 i = 0; i < timer_log_count; ++i) {
            gl_renderStringFormat(V3(text_pos + V2(4 * text_scale.x * timer_indent_array[i], i * (text_scale.y + 8)), 0), text_scale, V4(1, 1, 1, 1), timer_log_array[i]);
        }
    }

    timer_log_count = 0;
}

#define SCOPED_TIMER(str, function, ...) { Scoped_Timer timer(str); function(__VA_ARGS__); }

