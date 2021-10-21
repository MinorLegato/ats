#pragma once

static size_t hash_bytes(const void *p, size_t len, size_t seed);

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

// ============================== hash map ========================= //

#if 0
// used, hash, key, value

#define ht_put(table, ...) \
    ((table) = _ht_put((table), &(__VA_ARGS__), sizeof *(table), sizeof ((table)->key), (u32)((u8*)(table) - (u8*)&(table)->key)))

static void* _ht_put(void* table, const void* new_entry, u32 entry_size, u32 key_size, u32 key_offset) {
    if (!table) _buf_reserve(table, entry_size, 256);
    return table;
}
#endif

#else //=============================================== C++ ============================================ //

template <typename type_t, u32 INIT_SIZE = 32>
struct array_t {
    u32     cap    = 0;
    u32     len    = 0;
    type_t* array  = nullptr;

    inline void add(const type_t& e) {
        if (len >= cap) reserve(cap? (2 * cap) : INIT_SIZE);
        array[len++] = e;
    }

    inline void rem(u32 i) {
        array[i] = array[--len];
    }

    inline void reserve(u32 new_cap) {
        if (new_cap > cap) {
            cap     = new_cap;
            array   = (type_t*)realloc(array, cap * sizeof (type_t));
        }
    }

    inline type_t& operator[](u32 i) {
        return array[i];
    }

    inline const type_t& operator[](u32 i) const {
        return array[i];
    }

    inline void clear(void) {
        len = 0;
    }

    inline void destroy(void) {
        free(array);
        cap     = 0;
        len     = 0;
        array   = nullptr;

        puts("array destroy");
    }

    ~array_t() { destroy(); }
};

template <typename key_t, typename value_t, u32 INIT_SIZE = 64>
struct hash_table_t {
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
   
    u32      cap    = 0;
    u32      count  = 0;
    entry_t* array  = nullptr;

    inline void put(const key_t& key, const value_t& value) {
        if (count + 1 >= cap) { _grow(); }

        u32 hash = hash_bytes(&key, sizeof (key_t), 0xdeadbeef);
        entry_t* e = _get_entry(hash, key);

        if (e) {
            if (e->state == NONE) { count++; }

            e->state    = USED;
            e->hash     = hash;
            e->key      = key;
            e->value    = value;
        }
    }

    inline value_t get(const key_t& key) const {
        u32 hash = hash_bytes(&key, sizeof (key_t), 0xdeadbeef);
        entry_t* e = _get_entry(hash, key);

        return (e && e->state == USED)? e->value : value_t {};
    }

    inline void del(const key_t& key) {
        u32 hash = hash_bytes(&key, sizeof (key_t), 0xdeadbeef);
        entry_t* e = _get_entry(hash, key);

        if (e && (e->state == USED)) { e->state = DELETED; }
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
    
    ~hash_table_t() { destroy(); }
};

template <typename type_t, typename weight_t = f32>
struct priority_queue_t {
    struct entry_t {
        weight_t w;
        type_t   e;
    };

    array_t<entry_t> array;

    inline b32 empty() {
        return array.len == 0;
    }

    inline void clear() {
        array.len = 0;
    }

    inline void push(const type_t& e, weight_t w) {
        array.reserve(array.len + 1);
        
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

// yanked from https://github.com/nothings/stb/blob/master/stb_ds.h

#define STBDS_SIZE_T_BITS           ((sizeof (size_t)) * 8)
#define STBDS_ROTATE_LEFT(val, n)   (((val) << (n)) | ((val) >> (STBDS_SIZE_T_BITS - (n))))
#define STBDS_ROTATE_RIGHT(val, n)  (((val) >> (n)) | ((val) << (STBDS_SIZE_T_BITS - (n))))

#ifndef STBDS_SIPHASH_C_ROUNDS
#define STBDS_SIPHASH_C_ROUNDS 1
#endif
#ifndef STBDS_SIPHASH_D_ROUNDS
#define STBDS_SIPHASH_D_ROUNDS 1
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant, for do..while(0) and sizeof()==
#endif

static size_t hash_bytes(const void *p, size_t len, size_t seed) {
    unsigned char *d = (unsigned char *) p;
    size_t i,j;
    size_t v0,v1,v2,v3, data;

    // hash that works on 32- or 64-bit registers without knowing which we have
    // (computes different results on 32-bit and 64-bit platform)
    // derived from siphash, but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
    v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^  seed;
    v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
    v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^  seed;
    v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

#ifdef STBDS_TEST_SIPHASH_2_4
    // hardcoded with key material in the siphash test vectors
    v0 ^= 0x0706050403020100ull ^  seed;
    v1 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
    v2 ^= 0x0706050403020100ull ^  seed;
    v3 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
#endif

#define STBDS_SIPROUND() \
    do {                   \
        v0 += v1; v1 = STBDS_ROTATE_LEFT(v1, 13);  v1 ^= v0; v0 = STBDS_ROTATE_LEFT(v0,STBDS_SIZE_T_BITS/2); \
        v2 += v3; v3 = STBDS_ROTATE_LEFT(v3, 16);  v3 ^= v2;                                                 \
        v2 += v1; v1 = STBDS_ROTATE_LEFT(v1, 17);  v1 ^= v2; v2 = STBDS_ROTATE_LEFT(v2,STBDS_SIZE_T_BITS/2); \
        v0 += v3; v3 = STBDS_ROTATE_LEFT(v3, 21);  v3 ^= v0;                                                 \
    } while (0)

    for (i=0; i+sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
        data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
        data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16; // discarded if size_t == 4

        v3 ^= data;
        for (j=0; j < STBDS_SIPHASH_C_ROUNDS; ++j)
            STBDS_SIPROUND();
        v0 ^= data;
    }
    data = len << (STBDS_SIZE_T_BITS-8);
    switch (len - i) {
        case 7: data |= ((size_t) d[6] << 24) << 24; // fall through
        case 6: data |= ((size_t) d[5] << 20) << 20; // fall through
        case 5: data |= ((size_t) d[4] << 16) << 16; // fall through
        case 4: data |= (d[3] << 24); // fall through
        case 3: data |= (d[2] << 16); // fall through
        case 2: data |= (d[1] << 8); // fall through
        case 1: data |= d[0]; // fall through
        case 0: break;
    }
    v3 ^= data;
    for (j=0; j < STBDS_SIPHASH_C_ROUNDS; ++j)
        STBDS_SIPROUND();
    v0 ^= data;
    v2 ^= 0xff;
    for (j=0; j < STBDS_SIPHASH_D_ROUNDS; ++j)
        STBDS_SIPROUND();

#ifdef STBDS_SIPHASH_2_4
    return v0^v1^v2^v3;
#else
    return v1^v2^v3; // slightly stronger since v0^v3 in above cancels out final round operation? I tweeted at the authors of SipHash about this but they didn't reply
#endif
}

