#include <stdio.h>

#define HEADER_MAX_SIZE (1024 * 1024)
static char header_content[HEADER_MAX_SIZE];
static char* header = header_content;

#define emit_format(...) (header) += sprintf((header), __VA_ARGS__)

static void emit_file_to_disk(const char* path) {
  FILE* fp = 0;
  fopen_s(&fp, path, "w");
  fwrite(header_content, 1, header - header_content, fp);
  fclose(fp);
}


