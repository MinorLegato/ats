#include <stdlib.h>

// ==================================== HEAP ALLOCATOR ================================ //

static
M_ALLOCATOR_PROC(mem_heap_allocator_proc) {
    switch (desc.tag) {
        case mem_tag_alloc: {
            return malloc(desc.size);
        } break;

        case mem_tag_resize: {
            void* new_pointer = realloc(desc.pointer, desc.size);
            if (new_pointer != desc.pointer) {
                free(desc.pointer);
            }
            return new_pointer;
        } break;

        case mem_tag_free: {
            free(desc.pointer);
        } break;
    }
    return NULL;
}

extern mem_allocator
mem_heap_allocator(void) {
    return (mem_allocator) { .proc = mem_heap_allocator_proc };
}

// ====================================== LINEAR ALLOCATOR (using chunks) ================================ //

typedef struct mem_chunk {
    struct mem_chunk* prev;
    usize index;
    u8 data[];
} mem_chunk;

typedef struct mem_linear {
    usize chunk_size;
    usize chunk_count;
    usize memory_used;

    mem_chunk* chunk;
} mem_linear;

static void*
mem_linear_alloc(mem_linear* arena, usize size) {
    arena->memory_used += size;

    if (!arena->chunk || (arena->chunk->index + size > arena->chunk_size)) {
        mem_chunk* chunk = malloc(sizeof (mem_chunk) + max(arena->chunk_size, size));
        assert(chunk);

        chunk->index = 0;
        chunk->prev = arena->chunk;

        arena->chunk = chunk;
        arena->chunk_count++;
    }

    void* memory = arena->chunk->data + arena->chunk->index;
    arena->chunk->index += size;

    return memory;
}

static
M_ALLOCATOR_PROC(mem_linear_allocator_proc) {
    mem_linear* arena = desc.data;

    switch (desc.tag) {
        case mem_tag_alloc: {
            return mem_linear_alloc(arena, desc.size);
        } break;

        case mem_tag_resize: {
            void* memory = mem_linear_alloc(arena, desc.size);
            if (desc.pointer) {
                memcpy(memory, desc.pointer, desc.size);
            }
            return memory;
        } break;
    }
    return NULL;
}

extern mem_allocator
mem_linear_allocator(usize chunk_size) {
    mem_linear* arena = calloc(1, sizeof (mem_linear));
    assert(arena);

    arena->chunk_size = chunk_size;

    return (mem_allocator) {
        .proc = mem_linear_allocator_proc,
        .data = arena,
    };
}

