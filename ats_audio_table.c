#pragma once

struct audio_id
{
  u16 index;
};

// ============================= INTERNAL ============================== //

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

static const char* audio_path = AUDIO_PATH;

struct audio_entry {
  b32 in_use;
  cs_audio_source_t* source;
  char name[64];
};

static struct audio_entry audio_table[AUDIO_TABLE_SIZE];

static void audio_init(void* handle) {
  cs_init(handle, 44100, 2 * 1024, NULL);
  cs_spawn_mix_thread();
  cs_mix_thread_sleep_delay(7);
}

static b32 audio_is_valid(struct audio_id id) {
  return id.index != 0;
}

static struct audio_id audio_get(const char* name) {
  u32 hash = hash_str(name);
  u16 index = hash & (AUDIO_TABLE_SIZE - 1);

  if (index == 0) index++;

  while (audio_table[index].in_use) {
    if (strcmp(audio_table[index].name, name) == 0) {
      struct audio_id id = { index };
      return id;
    }

    index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
    if (index == 0) index++;
  }

  char path[512] = {0};

  {
    int i = 0;
    for (i = 0; audio_path[i]; ++i) { path[i] = audio_path[i]; }
    for (int j = 0; name[j]; ++j, ++i) { path[i] = name[j]; }
    path[i++] = '.';
    path[i++] = 'w';
    path[i++] = 'a';
    path[i++] = 'v';
    path[i++] = '\0';
  }

  struct audio_entry* entry = &audio_table[index];

  entry->in_use = true;
  strcpy_s(entry->name, countof(entry->name), name);

  cs_error_t error = CUTE_SOUND_ERROR_NONE;
  entry->source = cs_load_wav(path, &error);

  if (error) {
    printf("%s ---- path: %s\n", cs_error_as_string(error), path);
  }

  struct audio_id id = { index };
  return id;
}

static void audio_pause(b32 pause) {
  cs_set_global_pause(pause);
}

static void audio_kill_all(void) {
  cs_stop_all_playing_sounds();
}

static struct audio_entry* audio_get_entry(struct audio_id id) {
  if (!id.index || id.index > AUDIO_TABLE_SIZE)
    return NULL;

  return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

static void audio_play(struct audio_id id, f32 volume) {
  struct audio_entry* entry = audio_get_entry(id);
  if (entry) {
    cs_sound_params_t params = {0};
    params.volume = volume;
    params.pan = 0.5;
    cs_play_sound(entry->source, params);
  }
}

static void audio_play_music(struct audio_id id, f32 volume) {
  struct audio_entry* entry = audio_get_entry(id);
  if (entry) {
    cs_music_stop(0);
    cs_music_play(entry->source, 0);
  }
}

