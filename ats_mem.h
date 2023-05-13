#pragma once

#include "ats_base.h"

#define MEM_KIB(n) (1024 * (n))
#define MEM_MIB(n) (1024 * MEM_KIB(n))
#define MEM_GIB(n) (1024 * MEM_MIB(n))

struct mem_index {
  usize pos;
  struct mem_index* next;
};

struct mem_arena {
  usize pos;
  usize cap;
  u8* buf;

  struct mem_index* stack;
  struct mem_arena* next;
};

static struct mem_arena* mem_stack = NULL;

#define mem_alloc(...) _mem_alloc((struct mem_alloc_desc) { __VA_ARGS__ })

#define mem_type(type_t, ...)         (type_t*)mem_alloc((sizeof (type_t)), 0, __VA_ARGS__)
#define mem_array(type_t, count, ...) (type_t*)mem_alloc(((count) * sizeof (type_t)), (count), __VA_ARGS__)

static void*
mem_clear(void* data, usize size) {
  u8* d = data;
  for (usize i = 0; i < size; ++i) {
    d[i] = 0;
  }
  return d;
}

static struct mem_arena
mem_create(void* data, usize size) {
  return (struct mem_arena) {
    .cap = size,
    .buf = data,
  };
}

#define MEM_GET(arg) ((arg).arena? (arg).arena : (mem_stack))

struct mem_alloc_desc {
  usize size;
  usize count;
  struct mem_arena* arena;
};

struct mem_header {
  usize size;
  usize count;
};

static void*
_mem_alloc(struct mem_alloc_desc desc) {
  struct mem_arena* arena = MEM_GET(desc);

  struct mem_header* header = (struct mem_header*)(arena->buf + arena->pos);
  arena->pos += sizeof (struct mem_header) + desc.size;

  header->size = desc.size;
  header->count = desc.count? desc.count : desc.size; 

  return mem_clear(header + 1, desc.size);
}

struct mem_arena_desc {
  struct mem_arena* arena;
};

#define mem_context(arena) scope_guard(mem_push(arena), mem_pop())

#define mem_save(...)     _mem_save((struct mem_arena_desc) { __VA_ARGS__ })
#define mem_restore(...)  _mem_restore((struct mem_arena_desc) { __VA_ARGS__ })
#define mem_scope(...)    scope_guard(_mem_save((struct mem_arena_desc) { __VA_ARGS__ }), _mem_restore((struct mem_arena_desc) { __VA_ARGS__ }))

#define mem_size(ptr)     ((struct mem_header*)(ptr) - 1)->size
#define mem_count(ptr)    ((struct mem_header*)(ptr) - 1)->count

static void
_mem_save(struct mem_arena_desc desc) {
  struct mem_arena* arena = MEM_GET(desc);

  usize pos = arena->pos;

  struct mem_index* node = mem_type(struct mem_index, arena);

  node->pos = pos;
  node->next = arena->stack;

  arena->stack = node;
}

static void
_mem_restore(struct mem_arena_desc desc) {
  struct mem_arena* arena = MEM_GET(desc);

  arena->pos = arena->stack->pos;
  arena->stack = arena->stack->next;
}

static void
mem_push(struct mem_arena* arena) {
  arena->next = mem_stack;
  mem_stack = arena;
}

static void 
mem_pop(void) {
  mem_stack = mem_stack->next;
}

#if 0

// ---------------------- arena ------------------------ //

typedef struct mem_index {
  struct mem_index* next;
  usize idx;
} mem_index;

typedef struct mem_arena {
  struct mem_arena* next;

  usize idx;
  usize cap;

  u8* buf;

  mem_index* stack;
} mem_arena;

static void mem_init(mem_arena* ma, u8* buffer, usize size) {
  memset(ma, 0, sizeof *ma);

  ma->cap = size;
  ma->buf = buffer;
}

static mem_arena* mem_ctx = NULL;

static void* mem_alloc(usize size) {
  u8* ptr = mem_ctx->buf + mem_ctx->idx;

  mem_ctx->idx += size;
  memset(ptr, 0, size);

  return ptr;
}

#define mem_type(type_t)      (type_t*)mem_alloc(sizeof (type_t))
#define mem_array(type_t, n)  (type_t*)mem_alloc((n) * sizeof (type_t))
#define mem_scope             scope_guard(mem_save(), mem_restore())

static void mem_push(mem_arena* ma) {
  assert(ma);
  ma->next = mem_ctx;
  mem_ctx = ma;
}

static void mem_pop(void) {
  assert(mem_ctx->next);
  mem_ctx = mem_ctx->next;
}

static void mem_save(void) {
  u32 idx = mem_ctx->idx;

  mem_index* index = mem_type(mem_index);

  index->idx = idx;
  index->next = mem_ctx->stack;

  mem_ctx->stack = index;
}

static void mem_restore(void) {
  mem_ctx->idx = mem_ctx->stack->idx;
  mem_ctx->stack = mem_ctx->stack->next;
}

static void* mem_begin(void) {
  return mem_ctx->buf + mem_ctx->idx;
}

static void mem_end(usize byte_size) {
  mem_ctx->idx += byte_size;
}

static void mem_validate(void) {
  assert(mem_ctx->stack == NULL);
}
#endif
