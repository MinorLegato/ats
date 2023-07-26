#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

static const char* audio_path = AUDIO_PATH;

typedef struct
{
   b32 in_use;
   cs_audio_source_t* source;
   char name[64];
} audio_entry;

static audio_entry audio_table[AUDIO_TABLE_SIZE];

extern void audio_init(void* handle)
{
   cs_init(handle, 44100, 2 * 1024, NULL);

   cs_spawn_mix_thread();
   cs_mix_thread_sleep_delay(7);
}

static b32 audio_is_valid(audio_id id)
{
   return id.index != 0;
}

extern audio_id audio_get(const char* name)
{
   u32 hash = hash_str(name);
   u16 index = hash & (AUDIO_TABLE_SIZE - 1);

   if (index == 0) index++;

   while (audio_table[index].in_use) {
      if (strcmp(audio_table[index].name, name) == 0) {
         audio_id id = { index };
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

   audio_entry* entry = &audio_table[index];

   entry->in_use = true;
   strcpy_s(entry->name, countof(entry->name), name);

   cs_error_t error = CUTE_SOUND_ERROR_NONE;
   entry->source = cs_load_wav(path, &error);
   //entry->playing = cs_make_def(&entry->source);

   if (error) {
      printf("%s ---- path: %s\n", cs_error_as_string(error), path);
   }

   audio_id id = { index };
   return id;
}

extern void audio_pause(b32 pause)
{
   cs_set_global_pause(pause);
}

extern void audio_kill_all(void)
{
   cs_stop_all_playing_sounds();
}

static audio_entry* audio_get_entry(audio_id id)
{
   if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;
   return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

extern void audio_play(audio_id id, f32 volume)
{
   audio_entry* entry = audio_get_entry(id);

   if (entry) {
      cs_sound_params_t params = ATS_INIT;

      params.volume = volume;
      params.pan = 0.5;

      cs_play_sound(entry->source, params);
   }
}

extern void audio_play_music(audio_id id, f32 volume)
{
   audio_entry* entry = audio_get_entry(id);
   if (entry) {
      cs_music_stop(0);
      cs_music_play(entry->source, 0);
   }
}

#if 0
extern void audio_play_from_source(audio_id id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance) {
   f32 sound_distance = v3_dist(pos, source);
   f32 final_volume = volume * Max(1 - sound_distance / max_distance, 0);

   if (final_volume <= 0) return;

   audio_entry* entry = audio_get_entry(id);

   if (entry) {
      v2 source_dir = {
         source.x - pos.x,
         source.y - pos.y,
      };

      source_dir = v2_norm(source_dir);

      f32 pan = v2_get_angle(dir.xy, source_dir) / PI;

      if (pan > +0.5f) pan = 1.0f - pan;
      if (pan < -0.5f) pan =-1.0f - pan;

      pan += 0.5f;
      pan = 1.0f - pan;

      cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);

      if (!playing) return;

      cs_lock(audio.context);

      cs_set_pan(playing, pan);
      cs_set_volume(playing, final_volume, final_volume);

      cs_unlock(audio.context);
   }
}
#endif
