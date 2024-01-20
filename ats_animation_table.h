#pragma once

typedef struct at_frame at_frame;
typedef struct at_animation at_animation;
typedef struct at_entity at_entity;

struct at_frame {
  const char* name;
  tex_rect rect;
  at_frame* next;
  at_animation* animation;
};

struct at_animation {
  const char* name;
  at_frame* frame;
  at_animation* next;
};

struct at_entity {
  const char* name;
  at_animation* animation;
  at_entity* next;
};

typedef struct {
  at_entity* entity;
  at_frame* frame;
  f32 duration;
} at_asset;

static void at_begin(void);
static void at_end(void);
static void at_add_entity(const char* name);
static void at_add_animation(const char* name);
static void at_add_frame(const char* name);

static at_asset at_get(const char* name);
static void at_set(at_asset* asset, const char* name);
static void at_update(at_asset* asset, f32 dt);

// ============================ INTERNAL ================================ //

static at_frame* at_current_frame = NULL;
static at_animation* at_current_animation = NULL;
static at_entity* at_current_entity = NULL;
static at_entity* at_entity_list = NULL;

static void at_add_entity(const char* name)
{
  assert(name);
  at_current_animation = NULL;
  at_current_frame = NULL;

  at_entity* entity = mem_type(at_entity);
  entity->name = name;

  if (!at_entity_list) {
    at_entity_list = entity;
  } else {
    at_current_entity->next = entity;
  }

  at_current_entity = entity;
}

static void at_add_animation(const char* name)
{
  assert(name);
  at_current_frame = NULL;

  at_animation* animation = mem_type(at_animation);
  animation->name = name;

  if (!at_current_entity->animation) {
    at_current_entity->animation = animation;
  } else {
    at_current_animation->next = animation;
  }

  at_current_animation = animation;
}

static void at_add_frame(const char* name)
{
  assert(name);

  at_frame* frame = mem_type(at_frame);

  frame->name = name;
  frame->rect = tex_get(name);
  frame->animation = at_current_animation;

  if (!at_current_animation->frame) {
    at_current_animation->frame = frame;
  } else {
    at_current_frame->next = frame;
  }

  at_current_frame = frame;
  at_current_frame->next = at_current_animation->frame;
}

static void at_begin(void)
{
  at_entity_list = NULL;
  at_current_entity = NULL;
  at_current_frame = NULL;
  at_current_animation = NULL;
}

static void at_end(void)
{
  // @NOTE: do some cool shit here!
}

static b32 at_cstr_equal(const char* a, const char* b)
{
  while (*a && *a == *b) {
    a++;
    b++;
  }
  return *a == *b;
}

static void at_set(at_asset* asset, const char* name)
{
  if (at_cstr_equal(asset->frame->animation->name, name)) return;

  at_animation* animation = asset->entity->animation;

  while (animation && !at_cstr_equal(animation->name, name)) {
    animation = animation->next;
  }

  if (animation) {
    asset->frame = animation->frame;
    asset->duration = 0;
  }
}

static void at_update(at_asset* asset, f32 dt)
{
  asset->duration += dt;
  if (asset->duration >= 1.0) {
    asset->frame = asset->frame->next;
    asset->duration = 0;
  }
}

static at_entity* at_get_entity(const char* name)
{
  at_entity* entity = at_entity_list;
  while (entity && !at_cstr_equal(entity->name, name)) {
    entity = entity->next;
  }
  return entity? entity : NULL;
}

static at_asset at_get(const char* name)
{
  at_asset state = {0};
  state.entity = at_get_entity(name);
  state.frame = state.entity->animation->frame;
  return state;
}

