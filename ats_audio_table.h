
#define CUTE_SOUND_IMPLEMENTATION
#include "dep/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

typedef struct AudioEntry AudioEntry;
struct AudioEntry {
    b32 in_use;
    
    cs_loaded_sound_t loaded;
    cs_play_sound_def_t playing;
    
    char name[64];
};

static struct {
    cs_context_t* context;
} audio;

static AudioEntry audio_table[AUDIO_TABLE_SIZE];

static void audio_init(void) {
    audio.context = cs_make_context(platform.native, 44100, 16 * 4096, 1024, NULL);
    
    cs_spawn_mix_thread(audio.context);
    cs_thread_sleep_delay(audio.context, 16);
}

#define audio_get(name) audio_get_internal(name, AUDIO_PATH name ".wav")

typedef struct AudioID AudioID;
struct AudioID {
    u16 index;
};

static b32 audio_is_valid(AudioID id) {
    return id.index != 0;
}

static AudioID audio_get_internal(const char* name, const char* path) {
    u32 hash    = hash_str(name);
    u16 index   = hash & (AUDIO_TABLE_SIZE - 1);
    
    if (index == 0) index++;
    
    while (audio_table[index].in_use) {
        if (strcmp(audio_table[index].name, name) == 0)
            return (AudioID) {index };
        
        index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
        
        if (index == 0) index++;
    }
    
    AudioEntry* entry = &audio_table[index];
    
    entry->in_use = true;
    strcpy_s(entry->name, ARRAY_COUNT(entry->name), name);
    
    entry->loaded  = cs_load_wav(path);
    entry->playing = cs_make_def(&entry->loaded);
    
    return (AudioID) { index };
}

static void audio_pause(b32 pause) {
    cs_lock(audio.context);
    
    cs_playing_sound_t* playing = cs_get_playing(audio.context);
    
    while (playing) {
        cs_pause_sound(playing, pause);
        playing = playing->next;
    }
    
    cs_unlock(audio.context);
}

static void audio_kill_all(void) {
    cs_stop_all_sounds(audio.context);
}

static AudioEntry* audio_get_entry(AudioID id) {
    if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;

    return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

static void audio_play(AudioID id, f32 volume) {
    AudioEntry* entry = audio_get_entry(id);

    if (entry) {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static cs_playing_sound_t* audio_play_looped(AudioID id, f32 volume) {
    AudioEntry* entry = audio_get_entry(id);

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

static void audio_play_music(AudioID id, f32 volume) {
    static cs_playing_sound_t* playing = NULL;
    
    if (playing && cs_is_active(playing))
        cs_stop_sound(playing);
    
    AudioEntry* entry = audio_get_entry(id);

    if (entry) {
        playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);

        playing->looped = true;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static void audio_play_from_source(AudioID id, Vec3 pos, Vec3 dir, Vec3 source, f32 volume, f32 max_distance) {
    f32 sound_distance  = v3_dist(pos, source);
    f32 final_volume    = volume * max(1 - sound_distance / max_distance, 0);

    if (final_volume <= 0) return;

    AudioEntry* entry = audio_get_entry(id);

    if (entry) {
        Vec2 source_dir = {
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

