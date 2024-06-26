#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils/utils.h"
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

void clearTerminal()
{
#ifdef _WIN32
  system("cls"); // Windows
#else
  printf("\033[H\033[J"); // Unix-like (Linux, macOS)
#endif
}

void flushStdin() {
  int c;
  while((c = getchar()) != '\n' && c != EOF) {}
}
