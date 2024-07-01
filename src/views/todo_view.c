#include <string.h>
#include <stdio.h>
#include <time.h>
#include "utils/utils.h"
#include "views/todo_view.h"
#include "models/todo.h"

void get_current_time(char *buffer, size_t buffer_size)
{
  time_t raw_time;
  struct tm *time_info;

  time(&raw_time);
  time_info = localtime(&raw_time);

  char am_pm[3];
  int hour = time_info->tm_hour;

  if (hour >= 12)
  {
    strcpy(am_pm, "PM");
    if (hour > 12)
      hour -= 12;
  }
  else
  {
    strcpy(am_pm, "AM");
    if (hour == 0)
      hour = 12;
  }

  snprintf(buffer, buffer_size, "%04d-%02d-%02d %02d:%02d %s",
           time_info->tm_year + 1900,
           time_info->tm_mon + 1,
           time_info->tm_mday,
           hour,
           time_info->tm_min,
           am_pm);
}

void showHelp()
{

  println("TODO options");

  println("h/help - help");
  // OUTSIDE FILE OPERATION
  println("n/new - Create new file");
  println("o/open - Open file // Only 1 at a time");
  println("d/delete - Delete file");
  println("c/close - Close file");
  println("ls/list - List files");

  // INSIDE FILE OPERATION
  println("a/append - Append todo to file {OPEN}");
  println("r/remove - Remove todo from file {OPEN}");
  println("e/edit - Edit todo from file {OPEN}");
  println("s/show 1 {priority = 1} [1,2,3,4] [Critical, High, Medium, Later]");
}

int identifyCommand(const char *command)
{
  if (strcmp(command, "h") == 0 || strcmp(command, "help") == 0)
  {
    return HELP;
  }
  else if (strcmp(command, "n") == 0 || strcmp(command, "new") == 0)
  {
    return NEW;
  }
  else if (strcmp(command, "o") == 0 || strcmp(command, "open") == 0)
  {
    return OPEN;
  }
  else if (strcmp(command, "d") == 0 || strcmp(command, "delete") == 0)
  {
    return DELETE;
  }
  else if (strcmp(command, "cls") == 0 || strcmp(command, "clear") == 0)
  {
    return CLEAR;
  }
  else if (strcmp(command, "c") == 0 || strcmp(command, "close") == 0)
  {
    return CLOSE;
  }
  else if (strcmp(command, "ls") == 0 || strcmp(command, "list") == 0)
  {
    return LIST;
  }
  else if (strcmp(command, "a") == 0 || strcmp(command, "append") == 0)
  {
    return APPEND;
  }
  else if (strcmp(command, "r") == 0 || strcmp(command, "remove") == 0)
  {
    return REMOVE;
  }
  else if (strcmp(command, "e") == 0 || strcmp(command, "edit") == 0)
  {
    return EDIT;
  }
  else if (strcmp(command, "s") == 0 || strcmp(command, "show") == 0)
  {
    return SHOW;
  }
  else
  {
    return UNKNOWN;
  }
}
const char *getCommandString(int command)
{
  switch (command)
  {
  case HELP:
    return "help";
  case NEW:
    return "new";
  case OPEN:
    return "open";
  case DELETE:
    return "delete";
  case CLEAR:
    return "clear";
  case CLOSE:
    return "close";
  case LIST:
    return "list";
  case APPEND:
    return "append";
  case REMOVE:
    return "remove";
  case EDIT:
    return "edit";
  case SHOW:
    return "show";
  default:
    return "unknown";
  }
}

void greetUser(const char *userName)
{
  println("Hello %s", userName);
  println("Welcome! to our todo app");
  println("<usage>: Enter h for help or to use todo functions");
}

int giveOptions(char *username)
{

  const int maxOptionLen = 10;
  char options[maxOptionLen];
  displayPrompt();
  fgets(options, maxOptionLen, stdin);
  removeNewLine(options);

  return identifyCommand(options);

  return 0;
}

void askFileName(char *filename)
{
  println("Enter file name: ");
  displayPrompt();
  fgets(filename, FILENAME_MAX, stdin);
  removeNewLine(filename);
}

void showError(int failedCmd, char *filename)
{
  const char *cmd = getCommandString(failedCmd);
  println("Unable to %s on file %s", cmd, filename);
}

void displayList(char **files, int writeFiles)
{

  println("Total files %d", writeFiles);
  println("----------------------------");
  for (size_t i = 0; i < writeFiles; i++)
  {
    println(" %s", files[i]);
  }
}