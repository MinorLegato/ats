#pragma once

#include <stdlib.h>

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

template <typename type_t, u32 max_cap>
struct Fixed_Array {
    u32     m_len = 0;
    type_t  m_buf[max_cap];

    inline type_t&          operator[](u32 i)       { return m_buf[i]; }
    inline const type_t&    operator[](u32 i) const { return m_buf[i]; }

    inline bool add(type_t e) {
        if (m_len >= max_cap) return false;
        m_buf[m_len++] = e;
        return true;
    }

    inline type_t* add() {
        if (m_len >= max_cap) return nullptr;
        type_t* e = &m_buf[m_len++];
        *e = {};
        return e;
    }

    inline void             rem(u32 i)  { m_buf[i] = m_buf[--m_len]; }

    inline type_t*          ptr()       { return m_buf; }
    inline const type_t*    ptr() const { return m_buf; }

    inline type_t*          get(u32 i)       { return m_buf + i; }
    inline const type_t*    get(u32 i) const { return m_buf + i; }

    inline u32              len() const { return m_len; }
    inline u32              cap() const { return max_cap; }

    inline void             clear() { m_len = 0; }

    inline type_t*          begin()         { return m_buf; }
    inline const type_t*    begin() const   { return m_buf; }

    inline type_t*          end()       { return m_buf + m_len; }
    inline const type_t*    end() const { return m_buf + m_len; }
};


// --------------- dynamic array --------------- //

#ifndef DYN_ARRAY_INIT_SIZE
#define DYN_ARRAY_INIT_SIZE (32)
#endif

template <typename type_t>
struct Array {
    u32     m_cap     = 0;
    u32     m_len     = 0;
    type_t* m_buf     = nullptr;

    inline type_t&          operator[](u32 i)       { return m_buf[i]; }
    inline const type_t&    operator[](u32 i) const { return m_buf[i]; }

    inline void reserve(u32 new_cap) {
        if (new_cap > m_cap) {
            m_cap = m_cap? 2 * m_cap : DYN_ARRAY_INIT_SIZE;
            m_cap = new_cap > m_cap? new_cap : m_cap;

            m_buf = (type_t*)realloc(m_buf, m_cap * sizeof (type_t));
        }
    }

    inline void resize(u32 new_size) {
        reserve(new_size);
        m_len = new_size;
    }

    inline bool add(const type_t& e) {
        reserve(m_len + 1);
        m_buf[m_len++] = e;
        return true;
    }

    inline void rem(u32 i) {
        m_buf[i] = m_buf[--m_len];
    }

    inline type_t* add() {
        reserve(m_len + 1);
        type_t* e = &m_buf[m_len++];
        *e = {};
        return e;
    }

    inline type_t*          ptr()       { return m_buf; }
    inline const type_t*    ptr() const { return m_buf; }

    inline type_t*          get()       { return m_buf + i; }
    inline const type_t*    get() const { return m_buf + i; }

    inline u32              len()       { return m_len; }
    inline u32              cap() const { return m_cap; }

    inline void             clear() { m_len = 0; }

    inline type_t*          begin()         { return m_buf; }
    inline const type_t*    begin() const   { return m_buf; }

    inline type_t*          end()       { return m_buf + m_len; }
    inline const type_t*    end() const { return m_buf + m_len; }

    inline void destroy() {
        free(m_buf);

        m_cap = 0;
        m_len = 0;
        m_buf = nullptr;
    }
};

// ===================================================== SLICES ================================================== //

template <typename type_t>
struct Slice {
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

enum entry_state_t : u32 {
    ENTRY_NONE,
    ENTRY_USED,
    ENTRY_DELETED,
};

template <typename key_t, typename value_t>
struct table_entry {
    u32         state;
    u32         hash;
    key_t       key;
    value_t     value;

    inline b32 is_entry(u32 h, const key_t& k) const {
        return hash == h && (memcmp(&key, &k, sizeof (key_t)) == 0);
    }
};

template <typename key_t, typename value_t, u32 INIT_SIZE = 64>
struct dyn_table {
    static_assert(IS_POWER_OF_TWO(INIT_SIZE), "init size has to be a power of 2");

    typedef table_entry<key_t, value_t> entry_t;
   
    u32         cap     = 0;
    u32         count   = 0;
    entry_t*    array   = nullptr;

    inline void put(const key_t& key, const value_t& value) {
        if (count + 1 >= cap) _grow();

        auto hash = _hash(key);
        auto e    = _get_entry(hash, key);

        if (e->state == ENTRY_NONE) { count++; }

        e->state    = ENTRY_USED;
        e->hash     = hash;
        e->key      = key;
        e->value    = value;
    }

    inline value_t* put(const key_t& key) {
        if (count + 1 >= cap) _grow();

        auto hash = _hash(key);
        auto e    = _get_entry(hash, key);

        if (e->state == NONE)       { count++; }
        if (e->state == DELETED)    { e->value = {}; }

        e->state    = ENTRY_USED;
        e->hash     = hash;
        e->key      = key;

        return &e->value;
    }

    inline value_t get(const key_t& key) {
        auto hash   = _hash(key);
        auto e      = _get_entry(hash, key);

        return (e && e->state == ENTRY_USED)? e->value : value_t {};
    }

    inline value_t* get_ptr(const key_t& key) {
        auto hash = _hash(key);
        auto e = _get_entry(hash, key);

        return (e && e->state == ENTRY_USED)? &e->value : nullptr;
    }

    inline void del(const key_t& key) {
        auto hash = _hash(key);
        auto e    = _get_entry(hash, key);

        if (e && (e->state == ENTRY_USED)) { e->state = ENTRY_DELETED; }
    }

    inline void clear(void) {
        count = 0;
        memset(array, 0, cap * sizeof (table_entry<key_t, value_t>));
    }

    inline void destroy(void) {
        free(array);

        cap     = 0;
        count   = 0;
        array   = nullptr;
    }

    inline table_entry<key_t, value_t>* _get_entry(u32 hash, const key_t& key) {
        if (!array) return nullptr;

        u32 index = hash & (cap - 1);

        for (u32 i = index; ((i + 1) & (cap - 1)) != index; i = ((i + 1) & (cap - 1))) {
            auto e = &array[i];

            if ((e->state == ENTRY_NONE) ||
                (e->state == ENTRY_DELETED) && (hash == e->hash) ||
                (e->state == ENTRY_USED) && e->is_entry(hash, key)) {
                return e;
            }
        }

        return nullptr;
    }

    inline void _grow(void) {
        auto old_cap   = cap;
        auto old_array = array;

        cap   = cap? (cap << 1) : INIT_SIZE;
        count = 0;
        array = (entry_t*)calloc(1, cap * sizeof (entry_t));

        if (old_array) {
            for (u32 i = 0; i < old_cap; ++i) {
                auto e = &old_array[i];

                if (e->state == ENTRY_USED) {
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

template <typename key_t, typename value_t, u32 SIZE>
struct fixed_table {
    typedef table_entry<key_t, value_t> entry_t;
       
    u32         count       = 0;
    entry_t     array[SIZE] = {};

    inline void put(const key_t& key, const value_t& value) {
        auto hash = _hash(key);
        auto e    = _get_entry(hash, key);

        if (e->state == ENTRY_NONE) { count++; }

        e->state    = ENTRY_USED;
        e->hash     = hash;
        e->key      = key;
        e->value    = value;
    }

    inline value_t* put(const key_t& key) {
        auto hash  = _hash(key);
        auto e     = _get_entry(hash, key);

        if (e->state == NONE)       { count++; }
        if (e->state == DELETED)    { e->value = {}; }

        e->state    = ENTRY_USED;
        e->hash     = hash;
        e->key      = key;

        return &e->value;
    }

    inline value_t get(const key_t& key) {
        auto hash   = _hash(key);
        auto e      = _get_entry(hash, key);

        return (e && e->state == ENTRY_USED)? e->value : value_t {};
    }

    inline value_t* get_ptr(const key_t& key) {
        auto hash   = _hash(key);
        auto e      = _get_entry(hash, key);

        return (e && e->state == ENTRY_USED)? &e->value : nullptr;
    }

    inline void del(const key_t& key) {
        auto hash   = _hash(key);
        auto e      = _get_entry(hash, key);

        if (e && (e->state == ENTRY_USED)) { e->state = DELETED; }
    }

    inline void clear(void) {
        count = 0;
        memset(array, 0, SIZE * sizeof (entry_t));
    }

    inline entry_t* _get_entry(u32 hash, const key_t& key) {
        u32 index = hash % SIZE;

        for (u32 i = index; ((i + 1) % SIZE) != index; i = ((i + 1) % SIZE)) {
            auto e = &array[i];

            if ((e->state == ENTRY_NONE) ||
                (e->state == ENTRY_DELETED) && (hash == e->hash) ||
                (e->state == ENTRY_USED) && e->is_entry(hash, key)) {
                return e;
            }
        }

        return nullptr;
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

