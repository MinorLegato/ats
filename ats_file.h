#pragma once

#include "ats_base.h"
#include "ats_mem.h"

extern usize file_get_size(const char* file_name);
extern char* file_read_str(const char* file_name);
extern b32 file_write_str(const char* file_name, const char* buffer);
extern b32 file_append_str(const char* file_name, const char* buffer);

extern b32 file_read_bin(const char* file_name, void* buffer, usize size);
extern b32 file_write_bin(const char* file_name, const void* buffer, usize size);

extern u32* file_load_image(const char* path, u16* width, u16* height);
extern void file_free_image(const u32* pixels);

