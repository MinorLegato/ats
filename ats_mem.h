#pragma once

#define MEM_KIB(n) (1024 * (n))
#define MEM_MIB(n) (1024 * MEM_KIB(n))
#define MEM_GIB(n) (1024 * MEM_MIB(n))

#define mem_alloc(...) _mem_alloc(make(mem_alloc_desc) { __VA_ARGS__ })
#define mem_type(type_t, ...)         (type_t*)mem_alloc((sizeof (type_t)), 0, __VA_ARGS__)
#define mem_array(type_t, count, ...) (type_t*)mem_alloc(((count) * sizeof (type_t)), (usize)(count), __VA_ARGS__)

#define mem_context(arena) scope_guard(mem_push(arena), mem_pop())

#define mem_save(...)       _mem_save(make(mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_restore(...)    _mem_restore(make(mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_begin(...)      _mem_begin(make(mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_end(size, ...)  _mem_end((size), make(mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_temp mem_begin
#define mem_scope(...)      scope_guard(_mem_save(make(mem_arena_desc) { 0, __VA_ARGS__ }), _mem_restore(make(mem_arena_desc) { 0, __VA_ARGS__ }))

#define mem_size(ptr)     ((mem_header*)(ptr) - 1)->size
#define mem_count(ptr)    ((mem_header*)(ptr) - 1)->count

typedef struct mem_index mem_index;
struct mem_index {
    usize pos;
    mem_index* next;
};

typedef struct mem_arena mem_arena;
struct mem_arena {
    usize pos;
    usize cap;
    u8* buf;

    mem_index* stack;
    mem_arena* next;
};

static mem_arena* mem_stack;

static void* mem_clear(void* data, usize size) {
    u8* d = (u8*)data;
    for (usize i = 0; i < size; ++i) {
        d[i] = 0;
    }
    return d;
}

static mem_arena mem_create(void* data, usize size) {
    mem_arena arena = {0};

    arena.cap = size;
    arena.buf = (u8*)data;

    return arena;
}

#define MEM_GET(arg) ((arg).arena? (arg).arena : (mem_stack))

typedef struct mem_alloc_desc mem_alloc_desc;
struct mem_alloc_desc {
    usize size;
    usize count;

    mem_arena* arena;
};

typedef struct mem_header mem_header;
struct mem_header {
    usize size;
    usize count;
};

static void* _mem_alloc(mem_alloc_desc desc) {
    mem_arena* arena = MEM_GET(desc);
    mem_header* header = (mem_header*)(arena->buf + arena->pos);

    arena->pos += sizeof (mem_header) + desc.size;
    header->size = desc.size;
    header->count = desc.count? desc.count : desc.size; 

    return mem_clear(header + 1, desc.size);
}

typedef struct mem_arena_desc mem_arena_desc;
struct mem_arena_desc {
    usize pad;
    mem_arena* arena;
};

static void _mem_save(mem_arena_desc desc) {
    mem_arena* arena = MEM_GET(desc);
    usize pos = arena->pos;
    mem_index* node = mem_type(mem_index, arena);

    node->pos = pos;
    node->next = arena->stack;

    arena->stack = node;
}

static void _mem_restore(mem_arena_desc desc) {
    mem_arena* arena = MEM_GET(desc);

    arena->pos = arena->stack->pos;
    arena->stack = arena->stack->next;
}

static void* _mem_begin(mem_arena_desc desc) {
    mem_arena* arena = MEM_GET(desc);
    void* ptr = arena->buf + arena->pos;
    return ptr;
}

static void _mem_end(usize size, mem_arena_desc desc) {
    mem_arena* arena = MEM_GET(desc);
    arena->pos += size;
}


static void mem_push(mem_arena* arena) {
    arena->next = mem_stack;
    mem_stack = arena;
}

static void mem_pop(void) {
    mem_stack = mem_stack->next;
}

