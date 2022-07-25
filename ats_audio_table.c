
#define CUTE_SOUND_IMPLEMENTATION
#include "ext/cute_sound.h"

#ifndef AUDIO_TABLE_SIZE
#define AUDIO_TABLE_SIZE (256)
#endif

#ifndef AUDIO_PATH
#define AUDIO_PATH "assets/sounds/"
#endif

static const char* audio_path = AUDIO_PATH;

typedef struct audio_entry {
    b32 in_use;
    cs_loaded_sound_t loaded;
    cs_play_sound_def_t playing;
    char name[64];
} audio_entry;

static struct {
    cs_context_t* context;
} audio;

static audio_entry audio_table[AUDIO_TABLE_SIZE];

extern void
audio_init(void* handle) {
    audio.context = cs_make_context(handle, 44100, 8 * 4096, 1024, NULL);
    
    cs_spawn_mix_thread(audio.context);
    cs_thread_sleep_delay(audio.context, 16);
}

static b32
audio_is_valid(audio_id id) {
    return id.index != 0;
}

extern audio_id
audio_get(const char* name) {
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
    strcpy_s(entry->name, array_count(entry->name), name);
    
    entry->loaded  = cs_load_wav(path);
    entry->playing = cs_make_def(&entry->loaded);

    if (cs_error_reason) {
        printf("%s ---- path: %s\n", cs_error_reason, path);
    }

    audio_id id = { index };
    return id;
}

extern void
audio_pause(b32 pause) {
    cs_lock(audio.context);
    cs_playing_sound_t* playing = cs_get_playing(audio.context);
    while (playing) {
        cs_pause_sound(playing, pause);
        playing = playing->next;
    }
    cs_unlock(audio.context);
}

extern void
audio_kill_all(void) {
    cs_stop_all_sounds(audio.context);
}

static audio_entry*
audio_get_entry(audio_id id) {
    if (!id.index || id.index > AUDIO_TABLE_SIZE) return NULL;
    return audio_table[id.index].in_use? &audio_table[id.index] : NULL;
}

extern void
audio_play(audio_id id, f32 volume) {
    struct audio_entry* entry = audio_get_entry(id);

    if (entry) {
        cs_playing_sound_t* playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);
        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

extern void*
audio_play_looped(audio_id id, f32 volume) {
    struct audio_entry* entry = audio_get_entry(id);

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

extern void
audio_play_music(audio_id id, f32 volume) {
    static cs_playing_sound_t* playing = NULL;
    
    if (playing && cs_is_active(playing))
        cs_stop_sound(playing);
    
    struct audio_entry* entry = audio_get_entry(id);
    if (entry) {
       playing = cs_play_sound(audio.context, entry->playing);
        
        if (!playing) return;
        
        cs_lock(audio.context);

        playing->looped = true;

        cs_set_volume(playing, volume, volume);
        cs_unlock(audio.context);
    }
}

extern void
audio_play_from_source(audio_id id, v3 pos, v3 dir, v3 source, f32 volume, f32 max_distance) {
    f32 sound_distance = v3_dist(pos, source);
    f32 final_volume = volume * max(1 - sound_distance / max_distance, 0);

    if (final_volume <= 0) return;

    struct audio_entry* entry = audio_get_entry(id);

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

