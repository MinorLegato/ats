#include "ats.h"

static mem_arena* mem_stack;

ATS_API void mem_init(void* data, usize size) {
  static mem_arena arena;
  arena = mem_create(data, size);
  mem_push(&arena);
}

ATS_API mem_arena mem_create(void* data, usize size) {
  mem_arena arena = {0};

  arena.cap = size;
  arena.buf = (u8*)data;

  return arena;
}

#define MEM_GET(arg) ((arg).arena? (arg).arena : (mem_stack))

typedef struct {
  usize size;
  usize count;
} mem_header;

ATS_API void* mem__alloc(mem__alloc_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  mem_header* header = (mem_header*)(arena->buf + arena->pos);

  arena->pos += sizeof (mem_header) + desc.size;
  header->size = desc.size;
  header->count = desc.count? desc.count : desc.size; 

  arena->max = max(arena->pos, arena->max);

  return memset(header + 1, 0, desc.size);
}

ATS_API void mem__save(mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  usize pos = arena->pos;
  mem_index* node = mem_type(mem_index, arena);

  node->pos = pos;
  node->next = arena->stack;

  arena->stack = node;
}

ATS_API void mem__restore(mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);

  arena->pos = arena->stack->pos;
  arena->stack = arena->stack->next;
}

ATS_API void* mem__begin(mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  void* ptr = arena->buf + arena->pos;
  return ptr;
}

ATS_API void mem__end(usize size, mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  arena->pos += size;
}

ATS_API void mem_push(mem_arena* arena) {
  arena->next = mem_stack;
  mem_stack = arena;
}

ATS_API void mem_pop(void) {
  mem_stack = mem_stack->next;
}

ATS_API usize mem_max(void) {
  return mem_stack? mem_stack->max : 0;
}

