#pragma once

static size_t hash_bytes(const void *p, size_t len, size_t seed);
static u32 hash_crc32(const void *data, size_t size);

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
        return hash_crc32(&key, sizeof (key_t));
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

// ================================================= CRC32 ============================================== //

static const uint32_t crc_table[] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

static u32 hash_crc32(const void *data, size_t size) {
    const u8 *d = (const u8*)data;
    u32 crc = 0xFFFFFFFF;

    while (size--) {
        u32 index = (crc ^ *(d++)) & 0xFF;
        crc = (crc >> 8) ^ crc_table[index];
    }

    return crc ^ 0xFFFFFFFF;
}

