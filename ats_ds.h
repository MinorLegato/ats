#ifndef __ATS_DS_H__
#define __ATS_DS_H__

#include <stdlib.h>

#ifndef BUF_INIT_SIZE
#define BUF_INIT_SIZE (256)
#endif

struct buf_header {
    u32 len;
    u32 cap;
};

#define _buf_header(b) ((struct buf_header*)(b) - 1)

#define buf_create(type, n)     ((type*)_buf_create(sizeof (type), (n)))
#define buf_len(b)              ((b)? _buf_header(b)->len : 0)
#define buf_cap(b)              ((b)? _buf_header(b)->cap : 0)
#define buf_add(b, ...)         ((b) = _buf_grow((b), sizeof (*(b))), (b)[_buf_header(b)->len++] = (__VA_ARGS__))
#define buf_rem(b, i)           ((b)? ((b)[(i)] = (b)[--_buf_header(b)->len], 1) : 0)
#define buf_reserve(b, n)       ((b) = _buf_reserve((b), sizeof (*(b)), (n)))
#define buf_clear(b)            ((b)? _buf_header(b)->len = 0 : 0)
#define buf_sort(b, cmp_func)   qsort((b), buf_len(b), sizeof (*(b)), (cmp_func))

#define for_buf(index, b)       for (u32 index = 0; index < buf_len(b); ++index)

extern void buf_free(void* buffer);

extern void* _buf_create(u32 element_size, u32 cap);
extern void* _buf_grow(void* buffer, u32 element_size);
extern void* _buf_reserve(void* buffer, u32 element_size, u32 new_cap);

#endif // __ATS_DS_H__

// ============================================================================================ //
// ======================================= IMPLEMENTATION ===================================== //
// ============================================================================================ //
#ifdef ATS_IMPL

extern void
buf_free(void* buffer) {
    if (buffer) {
        free(_buf_header(buffer));
    }
}

extern void*
_buf_create(u32 element_size, u32 cap) {
    struct buf_header* header = malloc(sizeof (struct buf_header) + cap * element_size);
    header->len = 0;
    header->cap = cap;
    return header + 1;
}

extern void*
_buf_grow(void* buffer, u32 element_size) {
    if (buffer) {
        struct buf_header* header = _buf_header(buffer);
        if ((header->len + 1) >= header->cap) {
            header->cap = header->cap << 1;
            struct buf_header* new_header = realloc(header, sizeof (struct buf_header) + header->cap * element_size);
            header = new_header;
        }
        return header + 1;
    } else {
        struct buf_header* header = malloc(sizeof (struct buf_header) + BUF_INIT_SIZE * element_size);
        header->len = 0;
        header->cap = BUF_INIT_SIZE;
        return header + 1;
    }
}

extern void*
_buf_reserve(void* buffer, u32 element_size, u32 new_cap) {
    if (buffer) {
        struct buf_header* header = _buf_header(buffer);
        if (new_cap >= header->cap) {
            header->cap = new_cap;
            struct buf_header* new_header = realloc(header, sizeof (struct buf_header) + header->cap * element_size);
            header = new_header;
        }
        return header + 1;
    } else {
        struct buf_header* header = malloc(sizeof (struct buf_header) + new_cap * element_size);
        header->len = 0;
        header->cap = new_cap;
        return header + 1;
    }
}

#endif
