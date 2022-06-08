#ifndef __ATS_ANIMATION_H__
#define __ATS_ANIMATION_H__

struct at_string {
    usize len;
    char* buf;
};

struct at_frame {
    struct at_string name;
    r2i rect;
    struct at_frame* next;
    struct at_animation* animation;
};

struct at_animation {
    struct at_string name;
    struct at_frame* frame;
    struct at_animation* next;
};

struct at_entity {
    struct at_string name;
    struct at_animation* animation;
    struct at_entity* next;
};

extern void at_begin(void);
extern void at_end(void);

extern void at_add_entity(const char* name);
extern void at_add_animation(const char* name);
extern void at_add_frame(const char* name);

struct at_asset {
    struct at_entity* entity;
    struct at_frame* frame;
    f32 duration;
};

extern struct at_asset at_get(const char* name);
extern void at_set(struct at_asset* asset, const char* name);
extern void at_update(struct at_asset* asset, f32 dt);

#endif // __ATS_ANIMATION__

// ============================================================================================ //
// ======================================= IMPLEMENTATION ===================================== //
// ============================================================================================ //
#ifdef ATS_IMPL

static struct at_frame* at_current_frame = NULL;
static struct at_animation* at_current_animation = NULL;
static struct at_entity* at_current_entity = NULL;

static struct at_entity* at_entity_list = NULL;
static usize at_mem_idx = 0;
static u8 at_mem_buf[8 * MB];

static void*
at_push_size(usize size) {
    assert(at_mem_idx < array_count(at_mem_buf));
    u8* ptr = at_mem_buf + at_mem_idx;
    memory_clear(ptr, size);
    at_mem_idx += size;
    return ptr;
}

#define at_push_type(type) (type*)at_push_size(sizeof (type))
#define at_push_array(type, size) (type*)at_push_size((size) * sizeof (type))

static struct at_string
at_string_create(const char* c_str) {
    struct at_string str = {0};
    str.len = strlen(c_str) + 1;
    str.buf = at_push_array(char, str.len);
    strcpy(str.buf, c_str);
    return str;
}

static b32
at_equal(struct at_string a, struct at_string b) {
    if (a.len != b.len) return false;
    return strcmp(a.buf, b.buf) == 0;
}

static b32
at_equal_cstr(struct at_string a, const char* b) {
    if (a.len == 0 || b == NULL) return false;
    return strcmp(a.buf, b) == 0;
}

extern void
at_add_entity(const char* name) {
    at_current_animation = NULL;
    at_current_frame = NULL;
    struct at_entity* entity = at_push_type(struct at_entity);
    entity->name = at_string_create(name);
    if (!at_entity_list) {
        at_entity_list = entity;
    } else {
        at_current_entity->next = entity;
    }
    at_current_entity = entity;
}

extern void
at_add_animation(const char* name) {
    at_current_frame = NULL;
    struct at_animation* animation = at_push_type(struct at_animation);
    animation->name = at_string_create(name);

    if (!at_current_entity->animation) {
        at_current_entity->animation = animation;
    } else {
        at_current_animation->next = animation;
    }
    at_current_animation = animation;
}

extern void
at_add_frame(const char* name) {
    struct at_frame* frame = at_push_type(struct at_frame);
    frame->name = at_string_create(name);
    frame->rect = tt_get(name);
    frame->animation = at_current_animation;
    if (!at_current_animation->frame) {
        at_current_animation->frame = frame;
    } else {
        at_current_frame->next = frame;
    }
    at_current_frame = frame;
    at_current_frame->next = at_current_animation->frame;
}

extern void
at_begin(void) {
    at_mem_idx = 0;
    at_entity_list = NULL;
    at_current_entity = NULL;
    at_current_frame = NULL;
    at_current_animation = NULL;
}

extern void
at_end(void) {
    // @NOTE: do some cool shit here!
}

extern void
at_set(struct at_asset* asset, const char* name) {
    if (at_equal_cstr(asset->frame->animation->name, name)) return;

    struct at_animation* animation = asset->entity->animation;
    while (animation && !at_equal_cstr(animation->name, name)) {
        animation = animation->next;
    }

    if (animation) {
        asset->frame = animation->frame;
        asset->duration = 0;
    }
}

extern void
at_update(struct at_asset* asset, f32 dt) {
    asset->duration += dt;
    if (asset->duration >= 1.0) {
        asset->frame = asset->frame->next;
        asset->duration = 0;
    }
}

static struct at_entity*
at_get_entity(const char* name) {
    struct at_entity* entity = at_entity_list;
    while (entity && !at_equal_cstr(entity->name, name)) {
        entity = entity->next;
    }
    return entity? entity : NULL;
}

extern struct at_asset
at_get(const char* name) {
    struct at_asset state = {0};
    state.entity = at_get_entity(name);
    state.frame = state.entity->animation->frame;
    return state;
}

#endif // ATS_IMPL
