#include "controllers/todo_controller.h"
#include "views/todo_view.h"
#include "utils/utils.h"
#include "models/todo.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO options:
 * 
 * h/help - help
 * n/new - create new file
 * o/open - open file
 * d/delete - delete file
 * c/close - close file
 * 
 * a/add/append - add/append to do file {OPEN}
 * r/remove - remove todo from file {OPEN}
 * e/edit - edit todo from file {OPEN}
 * 
 * show
 * show 1 {priority} [1,2,3,4] [Critical, High, Medium, Later]
 * 
 */

/*
 * Function: newCmdHandler
 * -----------------------
 * Handles the 'new' command to create a new todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
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

/*
 * Function: openCmdHandler
 * ------------------------
 * Handles the 'open' command to open an existing todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
 */
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

/*
 * Function: deleteCmdHandler
 * --------------------------
 * Handles the 'delete' command to delete an existing todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
 */
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

/*
 * Function: clearCmdHandler
 * -------------------------
 * Handles the 'clear' command to clear the terminal screen.
 *
 * Returns:
 *  - 0 on success.
 */
int clearCmdHandler()
{
  clearTerminal();
  return 0;
}

/*
 * Function: closeCmdHandler
 * -------------------------
 * Handles the 'close' command to close the currently opened todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if no file is currently open.
 */
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

/*
 * Function: listCmdHandler
 * ------------------------
 * Handles the 'list' command to list all todo files in the user's directory.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
 */
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

/*
 * Function: appendCmdHandler
 * --------------------------
 * Handles the 'append' command to add a new todo to the currently opened file.
 *
 * Returns:
 *  - 0 on success.
 */
int appendCmdHandler()
{
  Todo task;
  askTodo(&task);
  appendTodo(task);
  return 0;
}

/*
 * Function: saveCmdHandler
 * ------------------------
 * Handles the 'save' command to save changes to the currently opened todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
 */
int saveCmdHandler()
{
  return runSave();
}

/*
 * Function: lookCmdHanlder
 * ------------------------
 * Handles the 'look' command to display the todos in the currently opened file.
 *
 * Returns:
 *  - 0 on success.
 */
int lookCmdHanlder()
{
  char workingFile[FILENAME_MAX];
  TodoArray *todoList = runLook(workingFile);

  displayTodos(todoList, workingFile);

  return 0;
}

/*
 * Function: editCmdHandler
 * ------------------------
 * Handles the 'edit' command to edit a todo in the currently opened file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure.
 */
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

/*
 * Function: removeCmdHandler
 * --------------------------
 * Handles the 'remove' command to remove a todo from the currently opened file.
 *
 * Returns:
 *  - 0 on success.
 */
int removeCmdHandler()
{
  lookCmdHanlder();
  int id = askId();
  runRemove(id);
  return 0;
}

/*
 * Function: exitCmdHandler
 * ------------------------
 * Handles the 'exit' command to exit the application, saving changes if necessary.
 *
 * Returns:
 *  - 0 on success.
 */
int exitCmdHandler()
{
  if (runExit() == 1)
  {
    showError(SAVE, "Changes were not saved.");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/*
 * Function: runCommand
 * --------------------
 * Executes a command based on the given option.
 *
 * Parameters:
 *  - option: The command option to execute.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 on failure or unknown command.
 */
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
    showError(UNKNOWN, "Unknown command");
    return 0;
    break;
  }
  return 0;
}

int initTodo(User *user)
{
  clearTerminal();
  greetUser(user->username);
  prepTodoDir(user);
  return 0;
}

int runApp()
{
  while (1)
  {
    int option = giveOptions();
    runCommand(option);
  }
  return 0;
}