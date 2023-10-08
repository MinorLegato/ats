#pragma once

#include "ats_base.h"

struct audio_id
{
   u16 index;
};

void audio_init(void* handle);
struct audio_id audio_get(const char* name);
void audio_pause(b32 pause);
void audio_kill_all(void);
void audio_play(struct audio_id id, f32 volume);
void* audio_play_looped(struct audio_id id, f32 volume);
void audio_play_music(struct audio_id id, f32 volume);
void audio_play_from_source(struct audio_id id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);

