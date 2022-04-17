#ifndef __ATS_AUDIO_TABLE_H__
#define __ATS_AUDIO_TABLE_H__

struct AudioID {
    u16 index;
};

extern void     AudioInit(void);
extern AudioID  AudioGet(const char* name);
extern void     AudioPause(b32 pause);
extern void     AudioKillAll(void);
extern void     AudioPlay(AudioID id, f32 volume);
extern void*    AudioPlayLooped(AudioID id, f32 volume);
extern void     AudioPlayMusic(AudioID id, f32 volume);
extern void     AudioPlayFromSource(AudioID id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance);

#endif // __ATS_AUDIO_TABLE_H__

#ifdef ATS_IMPL

#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

static const char* audio_path = AUDIO_PATH;

struct AudioEntry {
    b32                     in_use;

    cs_loaded_sound_t       loaded;
    cs_play_sound_def_t     playing;

    char                    name[64];
};

static struct {
    cs_context_t* context;
} audio;

static AudioEntry audio_table[AUDIO_TABLE_SIZE];

extern void AudioInit(void) {
    audio.context = cs_make_context(platform.native, 44100, 8 * 4096, 1024, NULL);
    
    cs_spawn_mix_thread(audio.context);
    cs_thread_sleep_delay(audio.context, 16);
}

static b32 AudioIsValid(AudioID id) {
    return id.index != 0;
}

extern AudioID AudioGet(const char* name) {
    u32 hash  = HashStr(name);
    u16 index = hash & (AUDIO_TABLE_SIZE - 1);
    
    if (index == 0) index++;
    
    while (audio_table[index].in_use) {
        if (strcmp(audio_table[index].name, name) == 0) {
            AudioID id = { index };
            return id;
        }
        
        index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
        
        if (index == 0) index++;
    }
    
    static char path[1024] = {};

    {
        int i = 0;
        for (i = 0; audio_path[i]; ++i) {
            path[i] = audio_path[i];
        }

        for (int j = 0; name[j]; ++j, ++i) {
            path[i] = name[j];
        }

        path[i++] = '.';
        path[i++] = 'w';
        path[i++] = 'a';
        path[i++] = 'v';
        path[i++] = '\0';
    }

    AudioEntry* entry = &audio_table[index];
    
    entry->in_use = true;
    strcpy_s(entry->name, ArrayCount(entry->name), name);
    
    entry->loaded  = cs_load_wav(path);
    entry->playing = cs_make_def(&entry->loaded);

    if (cs_error_reason) {
        printf("%s ---- path: %s\n", cs_error_reason, path);
    }

    AudioID id = { index };
    return id;
}

extern void AudioPause(b32 pause) {
    cs_lock(audio.context);
    
    cs_playing_sound_t* playing = cs_get_playing(audio.context);
    
    while (playing) {
        cs_pause_sound(playing, pause);
        playing = playing->next;
    }
    
    cs_unlock(audio.context);
}

extern void AudioKillAll(void) {
    cs_stop_all_sounds(audio.context);
}

static AudioEntry* AudioGetEntry(AudioID id) {
    if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;

    return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

extern void AudioPlay(AudioID id, f32 volume) {
    AudioEntry* entry = AudioGetEntry(id);

    if (entry) {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

extern void* AudioPlayLooped(AudioID id, f32 volume) {
    AudioEntry* entry = AudioGetEntry(id);

    if (entry) {
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

extern void AudioPlayMusic(AudioID id, f32 volume) {
    static cs_playing_sound_t* playing = NULL;
    
    if (playing && cs_is_active(playing))
        cs_stop_sound(playing);
    
    AudioEntry* entry = AudioGetEntry(id);

    if (entry) {
       playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);

        playing->looped = true;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

extern void AudioPlayFromSource(AudioID id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance) {
    f32 sound_distance  = Dist(pos, source);
    f32 final_volume    = volume * Max(1 - sound_distance / max_distance, 0);

    if (final_volume <= 0) return;

    AudioEntry* entry = AudioGetEntry(id);

    if (entry) {
        v2 source_dir = {
            source.x - pos.x,
            source.y - pos.y,
        };

        source_dir = Norm(source_dir);

        f32 pan = GetAngle(dir.xy, source_dir) / PI;

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

#endif // ATS_IMPL

