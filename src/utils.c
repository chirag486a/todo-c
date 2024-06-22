#include <stdio.h>
#include <stdarg.h>
#include "../include/utils.h"
// UTILITY print new line

void println(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args); 
}