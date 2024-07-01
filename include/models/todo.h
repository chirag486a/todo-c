#ifndef TODO_H
#define TODO_H

#include "models/user.h"
#include <time.h>

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
  char description[256];
  Priority priority;
  time_t timestamp;

} Todo;

typedef struct
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
  REMOVE,
  EDIT,
  SHOW,
  UNKNOWN
} Command;

void prepTodoDir(const User *user);
int runNew(char *filename);
int runOpen(char *filename);
int runDelete(char *filename);
int runClose(char *filename);
int runList(char **files, int *numFiles, int *writeFiles);

#endif