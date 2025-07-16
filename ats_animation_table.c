#pragma once

static at_frame_t* at_current_frame = 0;
static at_animation_t* at_current_animation = 0;
static at_entity_t* at_current_entity = 0;
static at_entity_t* at_entity_list = 0;

ATS_API void at_add_entity(const char* name)
{
  assert(name);
  at_current_animation = 0;
  at_current_frame = 0;
  at_entity_t* entity = mem_type(at_entity_t);
  entity->name = name;
  if (!at_entity_list)
  {
    at_entity_list = entity;
  }
  else
  {
    at_current_entity->next = entity;
  }
  at_current_entity = entity;
}

ATS_API void at_add_animation(const char* name)
{
  assert(name);
  at_current_frame = 0;
  at_animation_t* animation = mem_type(at_animation_t);
  animation->name = name;
  if (!at_current_entity->animation)
  {
    at_current_entity->animation = animation;
  }
  else
  {
    at_current_animation->next = animation;
  }
  at_current_animation = animation;
}

ATS_API void at_add_frame(const char* name)
{
  assert(name);
  at_frame_t* frame = mem_type(at_frame_t);
  frame->name = name;
  frame->rect = tex_get(name);
  frame->animation = at_current_animation;
  if (!at_current_animation->frame)
  {
    at_current_animation->frame = frame;
  }
  else
  {
    at_current_frame->next = frame;
  }
  at_current_frame = frame;
  at_current_frame->next = at_current_animation->frame;
}

ATS_API void at_begin(void)
{
  at_entity_list = 0;
  at_current_entity = 0;
  at_current_frame = 0;
  at_current_animation = 0;
}

ATS_API void at_end(void)
{
  // @NOTE: do some cool shit here!
}

ATS_API void at_set(at_state_t* state, const char* name)
{
  assert(state && name);
  if (strcmp(state->frame->animation->name, name) == 0) return;
  at_animation_t* animation = state->entity->animation;

  while (animation && (strcmp(animation->name, name) != 0))
    animation = animation->next;

  if (animation)
  {
    state->frame = animation->frame;
    state->duration = 0;
  }
}

ATS_API void at_update(at_state_t* state, f32 dt)
{
  assert(state);
  state->duration += dt;

  if (state->duration >= 1.0)
  {
    state->frame = state->frame->next;
    state->duration = 0;
  }
}

static at_entity_t* _at_get_entity(const char* name)
{
  assert(name);
  at_entity_t* entity = at_entity_list;

  while (entity && (strcmp(entity->name, name) != 0))
    entity = entity->next;

  return entity? entity : 0;
}

ATS_API at_state_t at_get(const char* name)
{
  assert(name);
  at_state_t state = {0};
  state.entity = _at_get_entity(name);
  state.frame = state.entity->animation->frame;
  return state;
}

