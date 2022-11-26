#pragma once

#include "ats_base.h"

// ---------------------- arena ------------------------ //

typedef struct mem_arena {
  usize index;
  usize cap;
  u8* buffer;

  usize top;
  usize stack[16];

  usize lock;
  usize max;
} mem_arena_t;

#define mem_type(ma, type_t) (type_t*)mem_zero(ma, sizeof (type_t))
#define mem_array(ma, type_t, count) (type_t*)mem_zero(ma, (count) * sizeof (type_t))

#define mem_scope(ma) scope_guard(mem_save(ma), mem_restore(ma))

static void mem_init(mem_arena_t* ma, u8* buffer, usize size) {
  memset(ma, 0, sizeof *ma);
  ma->cap = size;
  ma->buffer = buffer;
}

static void* mem_alloc(mem_arena_t* ma, usize byte_size) {
  byte_size = AlignUp(byte_size, 16);
  assert(((ma->index + byte_size) < ma->cap) && !ma->lock);

  void* memory = ma->buffer + ma->index;
  ma->index += byte_size;
  ma->max = ma->max > ma->index? ma->max : ma->index;

  return memory;
}

static void* mem_zero(mem_arena_t* ma, usize byte_size) {
  void* ptr = mem_alloc(ma, byte_size);;
  memset(ptr, 0, byte_size);
  return ptr;
}

static void* mem_begin(mem_arena_t* ma) {
  ma->lock = true;
  return ma->buffer + ma->index;
}

static void mem_end(mem_arena_t* ma, usize byte_size) {
  ma->index += AlignUp(byte_size, 16);
  ma->lock = false;
}

static void mem_save(mem_arena_t* ma) {
  assert(ma->top < ma->cap);
  ma->stack[ma->top++] = ma->index;
}

static void mem_restore(mem_arena_t* ma) {
  assert(ma->top > 0);
  ma->index = ma->stack[--ma->top];
}

static void mem_validate(mem_arena_t* ma) {
  assert(ma->top == 0);
}

