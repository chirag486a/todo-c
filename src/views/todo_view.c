#include <string.h>
#include <stdio.h>
#include <time.h>
#include "utils/utils.h"
#include "views/todo_view.h"
#include "models/todo.h"

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
  println("a/append - Add Todo {OPEN}");
  println("s/save - Save Todo {OPEN}");
  println("r/remove - Remove todo from file {OPEN}");
  println("e/edit - Edit todo from file {OPEN}");
  println("l/look 1 {priority = 1} [1,2,3,4] [Critical, High, Medium, Later]");
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
  else if (strcmp(command, "s") == 0 || strcmp(command, "save") == 0)
  {
    return SAVE;
  }
  else if (strcmp(command, "r") == 0 || strcmp(command, "remove") == 0)
  {
    return REMOVE;
  }
  else if (strcmp(command, "e") == 0 || strcmp(command, "edit") == 0)
  {
    return EDIT;
  }
  else if (strcmp(command, "l") == 0 || strcmp(command, "look") == 0)
  {
    return LOOK;
  }
  else if (strcmp(command, "exit") == 0)
  {
    return EXIT;
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
  case SAVE:
    return "save";
  case REMOVE:
    return "remove";
  case EDIT:
    return "edit";
  case EXIT:
    return "exit";
  case LOOK:
    return "look";
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

char *stringifyTime(time_t raw_time, char *buffer, size_t bufferSize)
{
  struct tm *time_info;

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

  snprintf(buffer, bufferSize, "%04d-%02d-%02d %02d:%02d %s",
           time_info->tm_year + 1900,
           time_info->tm_mon + 1,
           time_info->tm_mday,
           hour,
           time_info->tm_min,
           am_pm);
  return buffer;
}

void askTodo(Todo *todo)
{
  println("Enter task Todo [NOTE: not long then %d character]", TODO_DESCRIPTION_SIZE);

  fgets(todo->description, TODO_DESCRIPTION_SIZE, stdin);
  removeNewLine(todo->description);

  while (1)
  {
    println("Select your priority");
    println("1) CRITICAL");
    println("2) HIGH");
    println("3) MEDIUM");
    println("4) LATER");

    int p;

    scanf("%d", &p);
    flushStdin();
    todo->priority = (Priority)p;

    if (
        !(todo->priority == CRITICAL ||
          todo->priority == HIGH ||
          todo->priority == MEDIUM ||
          todo->priority == LATER))
    {
      println("Please Input valid Priority");
      continue;
    }
    break;
  }
}
char *getPriorityString(Priority p) {
  switch (p)
  {
  case CRITICAL:
    return "critical";
  case HIGH:
    return "high";
  case MEDIUM:
    return "medium";
  case LATER:
    return "later";

  default:
    return "unknown";
  }
}
void displayTodos(TodoArray *todos, char *workingFile)
{
  // Print the table header
  println("%-11s | %-75s | %-8s | %s", "ID", "Description", "Priority", "Creation Time");

  for (size_t i = 0; i < 100; i++)
  {
    printf("-");
  }
  println("");

  char buffer[20];
  for (size_t i = 0; i < todos->size; i++)
  {
    Todo todo = todos->todos[i];
    stringifyTime(todo.timestamp, buffer, sizeof(buffer));

    println("%-11d | %-75s | %-8s | %s",
            todo.id,
            todo.description,
            getPriorityString(todo.priority),
            buffer);
  }
}

int askId()
{
  int i;
  println("Please provide the id for todo to be edited");
  scanf("%d", &i);
  flushStdin();
  return i;
}