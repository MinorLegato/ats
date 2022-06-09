#ifndef __ATS_ANIMATION_H__
#define __ATS_ANIMATION_H__

struct at_frame {
    const char* name;
    r2i rect;
    struct at_frame* next;
    struct at_animation* animation;
};

struct at_animation {
    const char* name;
    struct at_frame* frame;
    struct at_animation* next;
};

struct at_entity {
    const char* name;
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

extern void
at_add_entity(const char* name) {
    assert(name);
    at_current_animation = NULL;
    at_current_frame = NULL;
    struct at_entity* entity = at_push_type(struct at_entity);
    entity->name = name;
    if (!at_entity_list) {
        at_entity_list = entity;
    } else {
        at_current_entity->next = entity;
    }
    at_current_entity = entity;
}

extern void
at_add_animation(const char* name) {
    assert(name);
    at_current_frame = NULL;
    struct at_animation* animation = at_push_type(struct at_animation);
    animation->name = name;

    if (!at_current_entity->animation) {
        at_current_entity->animation = animation;
    } else {
        at_current_animation->next = animation;
    }
    at_current_animation = animation;
}

extern void
at_add_frame(const char* name) {
    assert(name);
    struct at_frame* frame = at_push_type(struct at_frame);
    frame->name = name;
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

static inline b32
at_cstr_equal(const char* a, const char* b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

extern void
at_set(struct at_asset* asset, const char* name) {
    if (at_cstr_equal(asset->frame->animation->name, name)) return;

    struct at_animation* animation = asset->entity->animation;
    while (animation && !at_cstr_equal(animation->name, name)) {
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
    while (entity && !at_cstr_equal(entity->name, name)) {
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
