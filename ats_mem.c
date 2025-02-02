#include "ats.h"

static mem_arena* mem_stack;

ATS_API mem_arena mem_create(void* data, usize size) {
  mem_arena arena = {0};

  arena.cap = size;
  arena.buf = (u8*)data;

  return arena;
}

#define MEM_GET(arg) ((arg).arena? (arg).arena : (mem_stack))

typedef struct mem_header mem_header;
struct mem_header {
  usize size;
  usize count;
};

ATS_API void* mem__alloc(struct mem__alloc_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  mem_header* header = (mem_header*)(arena->buf + arena->pos);

  arena->pos += sizeof (mem_header) + desc.size;
  header->size = desc.size;
  header->count = desc.count? desc.count : desc.size; 

  return memset(header + 1, 0, desc.size);
}

ATS_API void mem__save(struct mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  usize pos = arena->pos;
  mem_index* node = mem_type(mem_index, arena);

  node->pos = pos;
  node->next = arena->stack;

  arena->stack = node;
}

ATS_API void mem__restore(struct mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);

  arena->pos = arena->stack->pos;
  arena->stack = arena->stack->next;
}

ATS_API void* mem__begin(struct mem__arena_desc desc) {
  mem_arena* arena = MEM_GET(desc);
  void* ptr = arena->buf + arena->pos;
  return ptr;
}

ATS_API void mem__end(usize size, struct mem__arena_desc desc) {
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

