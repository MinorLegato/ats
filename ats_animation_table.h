#pragma once

struct at_frame;
struct at_animation;
struct at_entity;

struct at_frame {
  const char* name;
  struct tex_rect rect;
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

struct at_asset {
  struct at_entity* entity;
  struct at_frame* frame;
  f32 duration;
};

void at_begin(void);
void at_end(void);
void at_add_entity(const char* name);
void at_add_animation(const char* name);
void at_add_frame(const char* name);

struct at_asset at_get(const char* name);
void at_set(struct at_asset* asset, const char* name);
void at_update(struct at_asset* asset, f32 dt);


