
#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

typedef struct audio_entry_t {
    b32 in_use;
    
    cs_loaded_sound_t loaded;
    cs_play_sound_def_t playing;
    
    char name[64];
} audio_entry_t;

static struct {
    cs_context_t* context;
} audio;

typedef struct audio_id_t {
    u16 index;
} audio_id_t;

static audio_entry_t audio_table[AUDIO_TABLE_SIZE];

static void audio_init(void) {
    audio.context = cs_make_context(platform.native, 44100, 8 * 4096, 1024, NULL);
    
    cs_spawn_mix_thread(audio.context);
    cs_thread_sleep_delay(audio.context, 16);
}

static b32 audio_is_valid(audio_id_t id) {
    return id.index != 0;
}

static audio_id_t audio_get(const char* name) {
    u32 hash  = hash_str(name);
    u16 index = hash & (AUDIO_TABLE_SIZE - 1);
    
    if (index == 0) index++;
    
    while (audio_table[index].in_use) {
        if (strcmp(audio_table[index].name, name) == 0) {
            audio_id_t id = { index };
            return id;
        }
        
        index = (index + 1) & (AUDIO_TABLE_SIZE - 1);
        
        if (index == 0) index++;
    }
    
    static char path[1024];

    {
        u32 path_index = 0;
        u32 name_index = 0;

        while (AUDIO_PATH[path_index])  path[path_index]    = AUDIO_PATH[path_index++];
        while (name[name_index])        path[path_index++]  = name[name_index++];

        path[path_index++] = '.';
        path[path_index++] = 'w';
        path[path_index++] = 'a';
        path[path_index++] = 'v';
        path[path_index++] = '\0';
    }

    audio_entry_t* entry = &audio_table[index];
    
    entry->in_use = true;
    strcpy_s(entry->name, ARRAY_COUNT(entry->name), name);
    
    entry->loaded  = cs_load_wav(path);
    entry->playing = cs_make_def(&entry->loaded);

    if (cs_error_reason) {
        printf("%s ---- path: %s\n", cs_error_reason, path);
    }

    audio_id_t id = { index };
    return id;
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

static audio_entry_t* audio_get_entry(audio_id_t id) {
    if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;

    return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

static void audio_play(audio_id_t id, f32 volume) {
    audio_entry_t* entry = audio_get_entry(id);

    if (entry) {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static cs_playing_sound_t* audio_play_looped(audio_id_t id, f32 volume) {
    audio_entry_t* entry = audio_get_entry(id);

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

static void audio_play_music(audio_id_t id, f32 volume) {
    static cs_playing_sound_t* playing = NULL;
    
    if (playing && cs_is_active(playing))
        cs_stop_sound(playing);
    
    audio_entry_t* entry = audio_get_entry(id);

    if (entry) {
       playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);

        playing->looped = true;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

static void audio_play_from_source(audio_id_t id, vec3_t pos, vec3_t dir, vec3_t source, f32 volume, f32 max_distance) {
    f32 sound_distance  = v3_dist(pos, source);
    f32 final_volume    = volume * max(1 - sound_distance / max_distance, 0);

    if (final_volume <= 0) return;

    audio_entry_t* entry = audio_get_entry(id);

    if (entry) {
        vec2_t source_dir = {
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

