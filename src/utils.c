#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../include/utils.h"
#include "../include/main.h"
// UTILITY print new line

void println(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

void removeNewLine(char *string)
{
  string[strcspn(string, "\n")] = '\0';
}

