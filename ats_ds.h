
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
    }

    ~array_t() { destroy(); }
};

template <typename key_t, typename value_t>
struct hash_table_t {
    struct entry_t {
        key_t   key;
        value_t value;
    };
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


