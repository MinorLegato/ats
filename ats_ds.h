#pragma once

#ifndef __cplusplus

// ========================================== DYNAMIC ARRAY ===================================== //

#ifndef BUF_INIT_SIZE
#define BUF_INIT_SIZE (256)
#endif

typedef struct buf_header_t {
    u32 len;
    u32 cap;
} buf_header_t;

#define _buf_header(b)  ((buf_header_t*)(b) - 1)

#define buf_create(type, n) ((type*)_buf_create(sizeof (type), (n)))

#define buf_len(b)          ((b)? _buf_header(b)->len : 0)
#define buf_cap(b)          ((b)? _buf_header(b)->cap : 0)
#define buf_add(b, ...)     ((b) = _buf_grow((b), sizeof (*(b))), (b)[_buf_header(b)->len++] = (__VA_ARGS__))
#define buf_rem(b, i)       ((b)? ((b)[(i)] = (b)[--_buf_header(b)->len], 1) : 0)
#define buf_reserve(b, n)   ((b) = _buf_reserve((b), sizeof (*(b)), (n)))
#define buf_clear(b)        ((b)? _buf_header(b)->len = 0 : 0)

static void buf_free(void* buffer) {
    if (buffer) {
        free(_buf_header(buffer));
    }
}

static void* _buf_create(u32 element_size, u32 cap) {
    buf_header_t* header = malloc(sizeof (buf_header_t) + cap * element_size);

    header->len = 0;
    header->cap = cap;

    return header + 1;
}

static void* _buf_grow(void* buffer, u32 element_size) {
    if (buffer) {
        buf_header_t* header = _buf_header(buffer);

        if ((header->len + 1) >= header->cap) {
            header->cap = header->cap << 1;
            buf_header_t* new_header = realloc(header, sizeof (buf_header_t) + header->cap * element_size);
            header = new_header;
        }

        return header + 1;
    } else {
        buf_header_t* header = malloc(sizeof (buf_header_t) + BUF_INIT_SIZE * element_size);

        header->len = 0;
        header->cap = BUF_INIT_SIZE;

        return header + 1;
    }
}

static void* _buf_reserve(void* buffer, u32 element_size, u32 new_cap) {
    if (buffer) {
        buf_header_t* header = _buf_header(buffer);

        if (new_cap >= header->cap) {
            header->cap = new_cap;
            buf_header_t* new_header = realloc(header, sizeof (buf_header_t) + header->cap * element_size);
            header = new_header;
        }

        return header + 1;
    } else {
        buf_header_t* header = malloc(sizeof (buf_header_t) + new_cap * element_size);

        header->len = 0;
        header->cap = new_cap;

        return header + 1;
    }
}

#else //=============================================== C++ ============================================ //

// =================================================== ARRAYS ================================================== //

// --------------- fixed array --------------- //

template <typename type_t, u32 cap>
struct fixed_array {
    u32     len;
    type_t  buf[cap];

    inline type_t& operator[](u32 i) {
        return buf[i];
    }

    inline const type_t& operator[](u32 i) const {
        return buf[i];
    }
};

template <typename type_t, u32 cap>
static bool array_add(fixed_array<type_t, cap>* a, const type_t& e) {
    if (a->len >= cap) return false;
    a->buf[a->len++] = e;
    return true;
}

template <typename type_t, u32 cap>
static type_t* array_new(fixed_array<type_t, cap>* a) {
    if (a->len >= cap) return nullptr;
    type_t* e = &a->buf[a->len++];
    *e = {};
    return e;
}

template <typename type_t, u32 cap>
static void array_rem(fixed_array<type_t, cap>* a, u32 i) {
    a->buf[i] = a->buf[--a->len];
}

template <typename type_t, u32 cap>
static type_t* array_ptr(fixed_array<type_t, cap>* a) {
    return a->buf;
}

template <typename type_t, u32 cap>
static const type_t* array_ptr(const fixed_array<type_t, cap>* a) {
    return a->buf;
}

template <typename type_t, u32 cap>
static type_t* array_get(fixed_array<type_t, cap>* a, u32 i) {
    return a->buf + i;
}

template <typename type_t, u32 cap>
static const type_t* array_get(const fixed_array<type_t, cap>* a, u32 i) {
    return a->buf + i;
}

template <typename type_t, u32 cap>
static u32 array_len(const fixed_array<type_t, cap>* a) {
    return a->len;
}

template <typename type_t, u32 cap>
static u32 array_cap(const fixed_array<type_t, cap>* a) {
    return a->cap;
}

template <typename type_t, u32 cap>
static void array_clear(fixed_array<type_t, cap>* a) {
    a->len = 0;
}

// --------------- dynamic array --------------- //

#ifndef DYN_ARRAY_INIT_SIZE
#define DYN_ARRAY_INIT_SIZE (32)
#endif

template <typename type_t>
struct dyn_array {
    u32         cap;
    u32         len;
    type_t*     buf;

    inline type_t& operator[](u32 i) {
        return buf[i];
    }

    inline const type_t& operator[](u32 i) const {
        return buf[i];
    }
};

template <typename type_t>
static void array_reserve(dyn_array<type_t>* a, u32 new_cap) {
    if (new_cap > a->cap) {
        a->cap = new_cap;
        a->buf = (type_t*)realloc(a->buf, a->cap * sizeof (type_t));
    }
}

template <typename type_t>
static void array_resize(dyn_array<type_t>* a, u32 new_size) {
    array_reserve(a, new_size);
    a->len = new_size;
}

template <typename type_t>
static bool array_add(dyn_array<type_t>* a, const type_t& e) {
    if (a->len >= a->cap) array_reserve(a, a->cap? (2 * a->cap) : DYN_ARRAY_INIT_SIZE);
    a->buf[a->len++] = e;
    return true;
}

template <typename type_t>
static void array_rem(dyn_array<type_t>* a, u32 i) {
    a->buf[i] = a->buf[--a->len];
}

template <typename type_t>
static type_t* array_new(dyn_array<type_t>* a) {
    array_reserve(a, a->len + 1);
    type_t* e = &a->buf[a->len++];
    *e = {};
    return e;
}

template <typename type_t>
static type_t* array_ptr(dyn_array<type_t>* a) {
    return a->buf;
}

template <typename type_t>
static const type_t* array_ptr(const dyn_array<type_t>* a) {
    return a->buf;
}

template <typename type_t>
static type_t* array_get(dyn_array<type_t>* a, u32 i) {
    return a->buf + i;
}

template <typename type_t>
static const type_t* array_get(const dyn_array<type_t>* a, u32 i) {
    return a->buf + i;
}

template <typename type_t>
static u32 array_len(const dyn_array<type_t>* a) {
    return a->len;
}

template <typename type_t>
static u32 array_cap(const dyn_array<type_t>* a) {
    return a->cap;
}

template <typename type_t>
static void array_clear(dyn_array<type_t>* a) {
    a->len = 0;
}

template <typename type_t>
static void array_free(dyn_array<type_t>* a) {
    free(a->buf);

    a->cap = 0;
    a->len = 0;
    a->buf = nullptr;
}

// ===================================================== SLICES ================================================== //

template <typename type_t>
struct slice {
    u32         len;
    type_t*     buf;

    inline type_t& operator[](u32 i) {
        return buf[i];
    }

    inline const type_t& operator[](u32 i) const {
        return buf[i];
    }
};

// ===================================================== TABLES ================================================== //

template <typename key_t, typename value_t, u32 INIT_SIZE = 64>
struct hash_table {
    static_assert(IS_POWER_OF_TWO(INIT_SIZE), "init size has to be a power of 2");

    enum : u32 { NONE, USED, DELETED, };

    struct entry_t {
        u32     state;
        u32     hash;
        key_t   key;
        value_t value;

        inline b32 is_entry(u32 h, const key_t& k) const {
            return hash == h && (memcmp(&key, &k, sizeof (key_t)) == 0);
        }
    };
   
    u32 cap = 0;
    u32 count = 0;
    entry_t* array = nullptr;

    inline void put(const key_t& key, const value_t& value) {
        if (count + 1 >= cap) { _grow(); }

        u32 hash = _hash(key);
        entry_t* e = _get_entry(hash, key);

        if (e->state == NONE) { count++; }

        e->state    = USED;
        e->hash     = hash;
        e->key      = key;
        e->value    = value;
    }

    inline value_t* put_ptr(const key_t& key) {
        if (count + 1 >= cap) { _grow(); }

        u32 hash = _hash(key);
        entry_t* e = _get_entry(hash, key);

        if (e->state == NONE)       { count++; }
        if (e->state == DELETED)    { e->value = {}; }

        e->state    = USED;
        e->hash     = hash;
        e->key      = key;

        return &e->value;
    }

    inline value_t get(const key_t& key) const {
        u32 hash = _hash(key);
        entry_t* e = _get_entry(hash, key);

        return (e && e->state == USED)? e->value : value_t {};
    }

    inline value_t* get_ptr(const key_t& key) const {
        u32 hash = _hash(key);
        entry_t* e = _get_entry(hash, key);

        return (e && e->state == USED)? &e->value : nullptr;
    }

    inline void del(const key_t& key) {
        u32 hash = _hash(key);
        entry_t* e = _get_entry(hash, key);

        if (e && (e->state == USED)) { e->state = DELETED; }
    }

    inline void clear(void) {
        count = 0;
        memset(array, 0, cap * sizeof (entry_t));
    }

    inline void destroy(void) {
        free(array);
    }

    inline entry_t* _get_entry(u32 hash, const key_t& key) const {
        u32 index = hash & (cap - 1);

        for (u32 i = index; ((i + 1) & (cap - 1)) != index; i = ((i + 1) & (cap - 1))) {
            entry_t* e = &array[i];

            if ((e->state == NONE) ||
                (e->state == DELETED) && (hash == e->hash) ||
                (e->state == USED) && e->is_entry(hash, key)) {
                return e;
            }
        }

        return nullptr;
    }

    inline void _grow(void) {
        u32      old_cap    = cap;
        entry_t* old_array  = array;

        cap   = cap? (cap << 1) : INIT_SIZE;
        count = 0;
        array = (entry_t*)calloc(1, cap * sizeof (entry_t));

        if (old_array) {
            for (u32 i = 0; i < old_cap; ++i) {
                entry_t* e = &old_array[i];

                if (e->state == USED) {
                    put(e->key, e->value);
                }
            }

            free(old_array);
        }
    }

    inline u32 _hash(const key_t& key) const {
        return hash_mem(&key, sizeof (key_t));
    }
};

// ===================================================== QUEUES ================================================ //

template <typename type_t, typename weight_t = f32>
struct priority_queue {
    struct entry_t {
        weight_t w;
        type_t   e;
    };

    dyn_array<entry_t> array;

    inline b32 empty() {
        return array.len == 0;
    }

    inline void clear() {
        array.len = 0;
    }

    inline void push(const type_t& e, weight_t w) {
        array_reserve(&array, array.len + 1);
        
        entry_t node = { w, e };

        int i = array.len + 1;
        int j = i / 2;

        while (i > 1 && array[j].w > node.w) {
            array[i] = array[j];

            i = j;
            j = j / 2;
        }

        array[i] = node;

        array.len++;
    }

    inline f32 pop(type_t* out) {
        entry_t data = array[1];

        printf("%d\n", array.len);

        array[1] = array[array.len - 1];
        array.len--;

        int i = 1;
        while (i != array.len + 1) {
            int k = array.len + 1;
            int j = 2 * i;

            if (j <= array.len && array[j].w < array[k].w) {
                k = j;
            }

            if (j + 1 <= array.len && array[j + 1].w < array[k].w) {
                k = j + 1;
            }

            array[i] = array[k];
            i = k;
        }

        *out = data.e;
        return data.w;
    }
};

#endif

