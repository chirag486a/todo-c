#ifndef TODO_H
#define TODO_H

#include "models/user.h"
#include <time.h>

#define TODO_DESCRIPTION_SIZE 256

typedef enum Priority
{
  CRITICAL = 1,
  HIGH,
  MEDIUM,
  LATER
} Priority;

typedef struct Todo
{

  int id;
  char description[TODO_DESCRIPTION_SIZE];
  Priority priority;
  time_t timestamp;

} Todo;

typedef struct TodoArray
{
  Todo *todos;
  size_t size;
  size_t capacity;
} TodoArray;

typedef enum
{
  HELP,
  NEW,
  OPEN,
  DELETE,
  CLEAR,
  CLOSE,
  LIST,
  APPEND,
  SAVE,

  REMOVE,
  EDIT,
  LOOK,
  EXIT,

  UNKNOWN
} Command;

void prepTodoDir(const User *user);
int runNew(char *filename);
int runOpen(char *filename);
int runDelete(char *filename);
int runClose(char *filename);
int runList(char **files, int *numFiles, int *writeFiles);
int runSave();
TodoArray *runLook();

void appendTodo(Todo todo);

#endif