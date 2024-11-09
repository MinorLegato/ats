#include <ctype.h>

static char* scream(const char* s) {
  static char buffer[PATH_MAX];
  int i = 0;
  for (i = 0; s[i]; ++i) {
    buffer[i] = toupper(s[i]);
  }
  buffer[i] = '\0';
  return buffer;
}

