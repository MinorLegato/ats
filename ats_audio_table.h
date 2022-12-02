#pragma once

#include "ats_base.h"

typedef struct audio_id {
  u16 index;
} audio_id_t;

extern void audio_init(void* handle);
extern audio_id_t audio_get(const char* name);
extern void audio_pause(b32 pause);
extern void audio_kill_all(void);
extern void audio_play(audio_id_t id, f32 volume);
extern void* audio_play_looped(audio_id_t id, f32 volume);
extern void audio_play_music(audio_id_t id, f32 volume);
extern void audio_play_from_source(audio_id_t id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);
