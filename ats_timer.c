#include "ats.h"

typedef struct timer_entry timer_entry;
struct timer_entry {
    const char* name;

    f64 start;
    f64 stop;

    u32 depth;
};

typedef struct timer_node timer_node;
struct timer_node {
    const char* name;
    f32 current;
    f32 max;
};

static u32 timer_top;
static u32 timer_count;
static timer_entry timer_stack[512];
static timer_entry timer_array[512];
static timer_node timer_table[512];

#define timer_scope(name) scope_guard(timer_start(name), timer_stop())

static void timer_start(const char* name) {
    timer_entry* entry = timer_stack + timer_top++;
    entry->name = name;
    entry->start = platform_get_time();
    entry->stop = 0;
    entry->depth = timer_top - 1;
}

static timer_node* timer_node_next(timer_node* node)  {
    node++;
    if (node >= (timer_table + 512)) {
        node = timer_table;
    }
    return node;
}

static void timer_stop(void) {
    timer_entry* entry = timer_stack + (--timer_top);
    {
        entry->stop = platform_get_time();
        timer_array[timer_count++] = *entry;
    }
    {
        u32 hash = hash_str(entry->name);
        u32 index = hash & 511;
        timer_node* node = &timer_table[index];
        while (node->name && (strcmp(entry->name, node->name) != 0)) {
            node = timer_node_next(node);
        }
        if (!node->name) {
            node->name = entry->name;
        }
        node->current = entry->stop - entry->start;
        if (node->max < node->current) {
            node->max = node->current;
        }
    }
}

static void timer_reset_all(void) {
    timer_top = 0;
    timer_count = 0;
}

