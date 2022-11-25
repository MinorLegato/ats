#pragma once

typedef struct at_frame at_frame_t;
typedef struct at_animation at_animation_t;
typedef struct at_entity at_entity_t;

struct at_frame {
  const char* name;
  r2i rect;
  at_frame_t* next;
  at_animation_t* animation;
};

struct at_animation {
  const char* name;
  at_frame_t* frame;
  at_animation_t* next;
};

struct at_entity {
  const char* name;
  at_animation_t* animation;
  at_entity_t* next;
};

extern void at_begin(mem_arena_t* mem);
extern void at_end(void);

extern void at_add_entity(const char* name);
extern void at_add_animation(const char* name);
extern void at_add_frame(const char* name);

typedef struct at_asset {
  at_entity_t* entity;
  at_frame_t* frame;
  f32 duration;
} at_asset_t;

extern at_asset_t at_get(const char* name);
extern void at_set(at_asset_t* asset, const char* name);
extern void at_update(at_asset_t* asset, f32 dt);


