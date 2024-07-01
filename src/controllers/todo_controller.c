#include "controllers/todo_controller.h"
#include "views/todo_view.h"
#include "utils/utils.h"
#include "models/todo.h"
#include <stdio.h>
#include <stdlib.h>

/*

TODO options

h/help - help
n/new - create new file
o/open - open file
d/delete - delete file
c/close - close file

a/add/append - add/append to do file {OPEN}
r/remove - remove todo from file {OPEN}
e/edit - edit todo from file {OPEN}

show
show 1 {priority} [1,2,3,4] [Critical, High, Medium, Later]


todo
struct todo {


}

*/

int newCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runNew(filename);
  if (err)
  {
    showError(NEW, filename);
    return 1;
  }
  return 0;
}

int openCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runOpen(filename);
  if (err)
  {
    showError(OPEN, filename);
    return 1;
  }
  return 0;
}

int deleteCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runDelete(filename);
  if (err)
  {
    showError(DELETE, filename);
    return 1;
  }
  return 0;
}

int clearCmdHandler()
{
  clearTerminal();
  return 0;
}

int closeCmdHandler()
{
  char filename[FILENAME_MAX];
  int err = runClose(filename);
  if (err)
  {
    showError(CLOSE, "[file not opened]");
    return 1;
  }
  return 0;

  return 0;
}
int listCmdHandler()
{
  // Allocated space for file
  int allocFiles = 10;
  int writeFiles = 0;
  char **files = (char **)malloc(sizeof(char *) * allocFiles);
  int err = runList(files, &allocFiles, &writeFiles);
  if (err)
  {
    showError(LIST, "[Something went wrong]");
    return 1;
  }
  displayList(files, writeFiles);

  // clean memory
  for (int i = 0; i < allocFiles; i++)
  {
    free(files[i]);
  }
  free(files);

  return 0;
}

int appendCmdHandler() {}

int runCommand(int option)
{
  switch (option)
  {
  case HELP:
    showHelp();
    break;
  case NEW:
    return newCmdHandler();
  case OPEN:
    return openCmdHandler();
    break;
  case DELETE:
    return deleteCmdHandler();
  case CLEAR:
    return clearCmdHandler();
  case CLOSE:
    return closeCmdHandler();
  case LIST:
    return listCmdHandler();
    break;
  case APPEND:

    break;

  default:
    break;
  }
  return 0;
}

int initTodo(User *user)
{
  clearTerminal();
  greetUser(user->username);
  prepTodoDir(user);

  int exit = 0;

  while (!exit)
  {
    int option = giveOptions(user->username);
    println("Option: %d", option);
    runCommand(option);
  }
  return 0;
}