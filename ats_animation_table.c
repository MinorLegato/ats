
static at_frame_t* at_current_frame = NULL;
static at_animation_t* at_current_animation = NULL;
static at_entity_t* at_current_entity = NULL;
static at_entity_t* at_entity_list = NULL;
static mem_arena_t* at_arena = NULL;

extern void at_add_entity(const char* name) {
  assert(name);
  at_current_animation = NULL;
  at_current_frame = NULL;

  at_entity_t* entity = mem_type(at_arena, at_entity_t);
  entity->name = name;

  if (!at_entity_list) {
    at_entity_list = entity;
  } else {
    at_current_entity->next = entity;
  }

  at_current_entity = entity;
}

extern void at_add_animation(const char* name) {
  assert(name);
  at_current_frame = NULL;

  at_animation_t* animation = mem_type(at_arena, at_animation_t);
  animation->name = name;

  if (!at_current_entity->animation) {
    at_current_entity->animation = animation;
  } else {
    at_current_animation->next = animation;
  }

  at_current_animation = animation;
}

extern void at_add_frame(const char* name) {
  assert(name);

  at_frame_t* frame = mem_type(at_arena, at_frame_t);

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

extern void at_begin(mem_arena_t* ma) {
  at_arena = ma;
  at_entity_list = NULL;
  at_current_entity = NULL;
  at_current_frame = NULL;
  at_current_animation = NULL;
}

extern void at_end(void) {
  // @NOTE: do some cool shit here!
}

static inline b32 at_cstr_equal(const char* a, const char* b) {
  while (*a && *a == *b) {
    a++;
    b++;
  }
  return *a == *b;
}

extern void at_set(at_asset_t* asset, const char* name) {
  if (at_cstr_equal(asset->frame->animation->name, name)) return;

  at_animation_t* animation = asset->entity->animation;

  while (animation && !at_cstr_equal(animation->name, name)) {
    animation = animation->next;
  }

  if (animation) {
    asset->frame = animation->frame;
    asset->duration = 0;
  }
}

extern void at_update(at_asset_t* asset, f32 dt) {
  asset->duration += dt;
  if (asset->duration >= 1.0) {
    asset->frame = asset->frame->next;
    asset->duration = 0;
  }
}

static at_entity_t* at_get_entity(const char* name) {
  at_entity_t* entity = at_entity_list;
  while (entity && !at_cstr_equal(entity->name, name)) {
    entity = entity->next;
  }
  return entity? entity : NULL;
}

extern at_asset_t at_get(const char* name) {
  at_asset_t state = ATS_INIT;
  state.entity = at_get_entity(name);
  state.frame = state.entity->animation->frame;
  return state;
}

