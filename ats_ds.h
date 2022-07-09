#ifndef __ATS_DS_H__
#define __ATS_DS_H__

#include <stdlib.h>

typedef struct buf_header {
    m_allocator allocator;

    u32 len;
    u32 cap;
} buf_header;

#define _buf_header(b) ((buf_header*)(b) - 1)

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


#endif
