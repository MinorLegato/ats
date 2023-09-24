#pragma once

#include "ats_base.h"

typedef struct {
   u16 index;
} audio_id;

void      audio_init(void* handle);
audio_id  audio_get(const char* name);
void      audio_pause(b32 pause);
void      audio_kill_all(void);
void      audio_play(audio_id id, f32 volume);
void*     audio_play_looped(audio_id id, f32 volume);
void      audio_play_music(audio_id id, f32 volume);
void      audio_play_from_source(audio_id id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);

