#pragma once

#include "ats_base.h"

#define MEM_KIB(n) (1024 * (n))
#define MEM_MIB(n) (1024 * MEM_KIB(n))
#define MEM_GIB(n) (1024 * MEM_MIB(n))

struct mem_index
{
  usize pos;
  struct mem_index* next;
};

struct mem_arena
{
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

static void* mem_clear(void* data, usize size)
{
  u8* d = (u8*)data;
  for (usize i = 0; i < size; ++i) {
    d[i] = 0;
  }
  return d;
}

static struct mem_arena mem_create(void* data, usize size)
{
  struct mem_arena arena = {0};

  arena.cap = size;
  arena.buf = (u8*)data;

  return arena;
}

#define MEM_GET(arg) ((arg).arena? (arg).arena : (mem_stack))

struct mem_alloc_desc
{
  usize size;
  usize count;
  struct mem_arena* arena;
};

struct mem_header
{
  usize size;
  usize count;
};

static void* _mem_alloc(struct mem_alloc_desc desc)
{
  struct mem_arena*  arena  = MEM_GET(desc);
  struct mem_header* header = (struct mem_header*)(arena->buf + arena->pos);

  arena->pos += sizeof (struct mem_header) + desc.size;
  header->size = desc.size;
  header->count = desc.count? desc.count : desc.size; 

  return mem_clear(header + 1, desc.size);
}

struct mem_arena_desc
{
  u64 pad;
  struct mem_arena* arena;
};

#define mem_context(arena) scope_guard(mem_push(arena), mem_pop())

#define mem_save(...)     _mem_save((struct mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_restore(...)  _mem_restore((struct mem_arena_desc) { 0, __VA_ARGS__ })
#define mem_scope(...)    scope_guard(_mem_save((struct mem_arena_desc) { 0, __VA_ARGS__ }), _mem_restore((struct mem_arena_desc) { 0, __VA_ARGS__ }))

#define mem_size(ptr)     ((mem_header*)(ptr) - 1)->size
#define mem_count(ptr)    ((mem_header*)(ptr) - 1)->count

static void _mem_save(struct mem_arena_desc desc)
{
  struct mem_arena* arena = MEM_GET(desc);
  usize pos = arena->pos;
  struct mem_index* node = mem_type(struct mem_index, arena);

  node->pos = pos;
  node->next = arena->stack;

  arena->stack = node;
}

static void _mem_restore(struct mem_arena_desc desc)
{
  struct mem_arena* arena = MEM_GET(desc);

  arena->pos = arena->stack->pos;
  arena->stack = arena->stack->next;
}

static void mem_push(struct mem_arena* arena)
{
  arena->next = mem_stack;
  mem_stack = arena;
}

static void mem_pop(void)
{
  mem_stack = mem_stack->next;
}

