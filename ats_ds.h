#pragma once

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

template <typename key_t, typename value_t, u32 INIT_SIZE = 256>
struct hash_table_t {
    struct entry_t {
        u32 used;
        u32 hash;
        u32 next;

        key_t   key;
        value_t value;
    };
    
    u32         cap     = 0;
    u32         next   = 0;
    entry_t*    array   = nullptr;

    void put(const key_t& key, const value_t& value) {
        if (next >= cap) { _grow(); }

        u32 hash    = hash_mem(&key, sizeof (key));
        u32 index   = hash % (cap >> 1);

        entry_t* e  = &array[index];
        b32 has_key = false;

        while (e->used && e->next && (hash != e->hash) && (has_key = (memcmp(&e->key, &key, sizeof (key_t)) != 0))) {
            e = &array[e->next];
        }

        if (!e->used || !has_key) {
            e->used  = true;
            e->hash  = hash;
            e->key   = key;
            e->value = value;
        } else {
            entry_t* n = &array[next++];

            n->used  = true;
            n->hash  = hash;
            n->key   = key;
            n->value = value;

            e->next = next - 1;
        }
    }

    value_t get(const key_t& key) const {
        u32 hash    = hash_mem(&key, sizeof (key));
        u32 index   = hash % (cap >> 1);

        entry_t* e  = &array[index];
        b32 has_key = false;

        while (e->next && (hash != e->hash) && (has_key = (memcmp(&e->key, &key, sizeof (key_t)) != 0))) {
            e = &array[e->next];
        }

        return (has_key || memcmp(&e->key, &key, sizeof (key_t)) == 0)? e->value : (value_t {});
    }

    void destroy(void) {
        free(array);
    }

    void _grow(void) {
        u32      old_cap    = cap;
        entry_t* old_array  = array;

        cap    = cap? (2 * cap) : INIT_SIZE;
        array  = (entry_t*)calloc(1, cap * sizeof (entry_t));

        printf("%d\n", cap);

        if (old_array) {
            for (u32 i = 0; i < old_cap; ++i) {
                entry_t* e = &old_array[i];

                if (e->used) {
                    put(e->key, e->value);
                }
            }

            free(old_array);
        }

        next = cap >> 1;
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

