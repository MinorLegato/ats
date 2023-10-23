#pragma once

#include "ats_base.h"
#include "ats_mem.h"

static usize file_get_size(const char* file_name);
static char* file_read_str(const char* file_name);
static b32   file_write_str(const char* file_name, const char* buffer);
static b32   file_append_str(const char* file_name, const char* buffer);

static b32   file_read_bin(const char* file_name, void* buffer, usize size);
static b32   file_write_bin(const char* file_name, const void* buffer, usize size);

static u32*  file_load_image(const char* path, u16* width, u16* height);
static void  file_free_image(const u32* pixels);

