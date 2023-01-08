#pragma once

#include "ats_base.h"

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

#define mem_type(type)      (type*)mem_alloc(sizeof (type))
#define mem_array(type, n)  (type*)mem_alloc((n) * sizeof (type))
#define mem_scope           scope_guard(mem_save(), mem_restore())

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

