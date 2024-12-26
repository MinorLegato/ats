#pragma once

#include "ats_base.h"

#include <stdlib.h>

#define DYN_INIT_SIZE (256)

typedef struct {
    u32 cap;
    u32 len;
    u8 buf[];
} dyn_hdr_t;

#define dyn(type_t) type_t*

#define dyn_hdr(xs)             ((dyn_hdr_t*)(xs) - 1)
#define dyn_cap(xs)             ((xs)? dyn_hdr(xs)->cap : 0)
#define dyn_len(xs)             ((xs)? dyn_hdr(xs)->len : 0)
#define dyn_add(xs, ...)        ((xs) = dyn__grow((xs), sizeof (xs)[0]), (xs)[dyn_hdr(xs)->len++] = (__VA_ARGS__))
#define dyn_new(xs)             ((xs) = dyn__grow((xs), sizeof (xs)[0]), memset(&(xs)[dyn_hdr(xs)->len++], 0, sizeof *(xs)))
#define dyn_rem(xs, i)          ((xs)[(i)] = (xs)[--dyn_hdr(xs)->len])
#define dyn_del(xs)             (free(xs))
#define dyn_sort(xs, cmp_func)  (qsort((xs), dyn_len(xs), sizeof (xs)[0], cmp_func))

static void* dyn__grow(void* xs, u32 element_size) {
    dyn_hdr_t* hdr = 0;
    u32 len = 0;
    u32 cap = 0;
    if (xs) {
        hdr = dyn_hdr(xs);
        len = hdr->len;
        cap = hdr->cap;
    }
    if (len >= cap) {
        cap = (cap << 1) + (!cap) * DYN_INIT_SIZE;
        hdr = realloc(hdr, sizeof (dyn_hdr_t) + element_size * cap);
    }
    hdr->len = len;
    hdr->cap = cap;
    return hdr->buf;
}

