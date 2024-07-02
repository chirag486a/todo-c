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
  int wroteFileNum = 0;
  char **files = (char **)malloc(sizeof(char *) * allocFiles);
  int err = runList(files, &allocFiles, &wroteFileNum);
  if (err)

  {
    showError(LIST, "[Something went wrong]");
    return 1;
  }
  displayList(files, wroteFileNum);

  // clean memory
  for (int i = 0; i < wroteFileNum; i++)
  {
    free(files[i]);
  }
  free(files);

  return 0;
}

int appendCmdHandler()
{
  Todo task;
  askTodo(&task);
  appendTodo(task);
  return 0;
}

int saveCmdHandler()
{
  return runSave();
}

int lookCmdHanlder()
{
  char workingFile[FILENAME_MAX];
  TodoArray *todoList = runLook(workingFile);

  displayTodos(todoList, workingFile);

  return 0;
}

int editCmdHandler()
{
  if (!checkFileOpen())
  {
    showError(EDIT, "File not open.");
    return 1;
  }
  lookCmdHanlder();
  int id = askId();
  if (checkTodoIdExist(id))
  {
    showError(EDIT, "Id does not exist");
    return 1;
  }
  Todo todo;
  askTodo(&todo);
  todo.id = id;
  runEdit(&todo);
  return 0;
}

int removeCmdHandler()
{
  lookCmdHanlder();
  int id = askId();
  runRemove(id);
  return 0;
}

int exitCmdHandler()
{
  exit(EXIT_SUCCESS);
}

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
    return appendCmdHandler();
    break;
  case SAVE:
    return saveCmdHandler();
    break;
  case LOOK:
    return lookCmdHanlder();
    break;
  case EDIT:
    return editCmdHandler();
    break;
  case REMOVE:
    return removeCmdHandler();
    break;
  case EXIT:
    return exitCmdHandler();
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

  while (1)
  {
    int option = giveOptions(user->username);
    runCommand(option);
  }
  return 0;
}