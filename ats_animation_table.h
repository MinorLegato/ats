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

void at_begin(void);
void at_end(void);
void at_add_entity(const char* name);
void at_add_animation(const char* name);
void at_add_frame(const char* name);

at_asset at_get(const char* name);
void at_set(at_asset* asset, const char* name);
void at_update(at_asset* asset, f32 dt);


