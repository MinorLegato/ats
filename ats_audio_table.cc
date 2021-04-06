#pragma once

#define CUTE_SOUND_IMPLEMENTATION
#include "../ats/dep/cute_sound.h"

#define CUTE_SOUND_PITCH_PLUGIN_IMPLEMENTATION
#include "../ats/dep/cute_sound_pitch_plugin.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

struct AudioEntry
{
    b32 not_empty;
    
    cs_loaded_sound_t loaded;
    cs_play_sound_def_t playing;
    
    char name[64];
};

static struct
{
    cs_context_t* context;
    cs_plugin_interface_t pitch_plugin;
    cs_plugin_id_t pitch_id;
} audio;

static AudioEntry audio_table[AUDIO_TABLE_SIZE];

static void InitAudio(void)
{
    audio.context = cs_make_context(platform.native, 44100, 16 * 4096, 1024, NULL);
    
    cs_spawn_mix_thread(audio.context);
    cs_thread_sleep_delay(audio.context, 16);
    
    audio.pitch_plugin  = csp_get_pitch_plugin();
    audio.pitch_id      = cs_add_plugin(audio.context, &audio.pitch_plugin);
}

#define GetAudio(name) GetAudioInternal(name, AUDIO_PATH name ".wav")

struct AudioID
{
    u16 index;
};

static AudioID GetAudioInternal(const char* name, const char* path)
{
    u32 hash    = HashCStr(name);
    u16 index   = hash & (AUDIO_TABLE_SIZE - 1);
    
    if (index == 0) index++;
    
    while (audio_table[index].not_empty)
    {
        if (strcmp(audio_table[index].name, name) == 0)
            return { .index = index };
        
        index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
        
        if (index == 0) index++;
    }
    
    AudioEntry* entry = &audio_table[index];
    
    entry->not_empty = true;
    strcpy(entry->name, name);
    
    entry->loaded  = cs_load_wav(path);
    entry->playing = cs_make_def(&entry->loaded);
    
    return { index };
}

static void PauseAudio(b32 pause)
{
    cs_lock(audio.context);
    
    cs_playing_sound_t* playing = cs_get_playing(audio.context);
    
    while (playing)
    {
        cs_pause_sound(playing, pause);
        playing = playing->next;
    }
    
    cs_unlock(audio.context);
}

static void KillAllAudio(void)
{
    cs_stop_all_sounds(audio.context);
}

static AudioEntry* GetAudioEntry(AudioID id)
{
    if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;

    return audio_table[id.index].not_empty? &audio_table[id.index] : NULL;
}

static void PlayAudio(AudioID id, f32 volume = 1.0)
{
    if (AudioEntry* entry = GetAudioEntry(id))
    {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static cs_playing_sound_t* PlayLoopedAudio(AudioID id, f32 volume = 1.0)
{
    if (AudioEntry* entry = GetAudioEntry(id))
    {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing)
            return NULL;
        
        cs_lock(audio.context);

        playing->looped = 1;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
        
        return playing;
    }
    
    return NULL;
}

static void PlayMusic(AudioID id, f32 volume = 1.0)
{
    static cs_playing_sound_t* playing = NULL;
    
    if (playing && cs_is_active(playing))
        cs_stop_sound(playing);
    
    if (AudioEntry* entry = GetAudioEntry(id))
    {
        playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);

        playing->looped = true;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static void PlayAudioFromSource(AudioID id, v3 pos, v3 dir, v3 source, f32 volume = 1.0, f32 max_distance = 16.0)
{
    f32 sound_distance  = Dist(pos, source);
    f32 final_volume    = volume * max(1 - sound_distance / max_distance, 0);

    if (final_volume <= 0) return;

    if (AudioEntry* entry = GetAudioEntry(id))
    {
        f32 pan = GetAngle(dir.xy, Norm(source.xy - pos.xy)) / PI;

        if (pan > 0.5f) pan = 1.0f - pan;
        if (pan <-0.5f) pan =-1.0f - pan;

        pan += 0.5f;
        pan = 1.0f - pan;
        
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        
        csp_set_pitch(playing, 1.0f + RandF32(-0.05f, 0.1f), audio.pitch_id);
        cs_set_pan(playing, pan);
        
        cs_set_volume(playing, final_volume, final_volume);
        
        cs_unlock(audio.context);
    }
}

