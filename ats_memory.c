#include <stdlib.h>

// ==================================== HEAP ALLOCATOR ================================ //

static
M_ALLOCATOR_PROC(m_heap_allocator_proc) {
    switch (desc.tag) {
        case m_tag_alloc: {
            return malloc(desc.size);
        } break;

        case m_tag_resize: {
            void* new_pointer = realloc(desc.pointer, desc.size);
            if (new_pointer != desc.pointer) {
                free(desc.pointer);
            }
            return new_pointer;
        } break;

        case m_tag_free: {
            free(desc.pointer);
        } break;
    }
    return NULL;
}

extern m_allocator
m_heap_allocator(void) {
    return (m_allocator) { .proc = m_heap_allocator_proc };
}

// ====================================== LINEAR ALLOCATOR (using chunks) ================================ //

typedef struct m_chunk {
    struct m_chunk* prev;
    usize index;
    u8 data[];
} m_chunk;

typedef struct m_linear {
    usize chunk_size;
    usize chunk_count;
    usize memory_used;

    m_chunk* chunk;
} m_linear;

static void*
m_linear_alloc(m_linear* arena, usize size) {
    arena->memory_used += size;

    if (!arena->chunk || (arena->chunk->index + size > arena->chunk_size)) {
        m_chunk* chunk = malloc(sizeof (m_chunk) + max(arena->chunk_size, size));
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
M_ALLOCATOR_PROC(m_linear_allocator_proc) {
    m_linear* arena = desc.data;

    switch (desc.tag) {
        case m_tag_alloc: {
            return m_linear_alloc(arena, desc.size);
        } break;

        case m_tag_resize: {
            void* memory = m_linear_alloc(arena, desc.size);
            if (desc.pointer) {
                memcpy(memory, desc.pointer, desc.size);
            }
            return memory;
        } break;
    }
    return NULL;
}

extern m_allocator
m_linear_allocator(usize chunk_size) {
    m_linear* arena = calloc(1, sizeof (m_linear));
    assert(arena);

    arena->chunk_size = chunk_size;

    return (m_allocator) {
        .proc = m_linear_allocator_proc,
        .data = arena,
    };
}

