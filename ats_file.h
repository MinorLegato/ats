#pragma once

#include "ats_base.h"
#include "ats_mem.h"

usize file_get_size(const char* file_name);
char* file_read_str(const char* file_name);
b32   file_write_str(const char* file_name, const char* buffer);
b32   file_append_str(const char* file_name, const char* buffer);

b32   file_read_bin(const char* file_name, void* buffer, usize size);
b32   file_write_bin(const char* file_name, const void* buffer, usize size);

u32*  file_load_image(const char* path, u16* width, u16* height);
void  file_free_image(const u32* pixels);

