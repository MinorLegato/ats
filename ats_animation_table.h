#pragma once

typedef struct at_frame at_frame;
typedef struct at_animation at_animation;
typedef struct at_entity at_entity;

struct at_frame {
  const char* name;
  r2i rect;
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

extern void at_begin(void);
extern void at_end(void);

extern void at_add_entity(const char* name);
extern void at_add_animation(const char* name);
extern void at_add_frame(const char* name);

typedef struct at_asset {
  at_entity* entity;
  at_frame* frame;
  f32 duration;
} at_asset;

extern at_asset at_get(const char* name);
extern void at_set(at_asset* asset, const char* name);
extern void at_update(at_asset* asset, f32 dt);


