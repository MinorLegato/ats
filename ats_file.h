#pragma once

unsigned  file_get_size(const char* file_name);
char*     file_read_str(const char* file_name);
int       file_write_str(const char* file_name, const char* buffer);
int       file_append_str(const char* file_name, const char* buffer);
int       file_read_bin(const char* file_name, void* buffer, unsigned size);
int       file_write_bin(const char* file_name, const void* buffer, unsigned size);
unsigned* file_load_image(const char* path, unsigned short* width, unsigned short* height);
void      file_free_image(const unsigned* pixels);

