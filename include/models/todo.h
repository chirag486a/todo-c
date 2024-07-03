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

/*
 * Function: prepTodoDir
 * ---------------------
 * Prepares the directory path for storing user-specific todo data.
 *
 * Parameters:
 *  - user: Pointer to the User structure containing user information.
 */
void prepTodoDir(const User *user);

/*
 * Function: runSave
 * -----------------
 * Saves the current todo list to the opened file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if an error occurred during saving.
 */
int runSave();

/*
 * Function: runNew
 * ----------------
 * Creates a new todo file with the specified filename in the user's todo directory.
 *
 * Parameters:
 *  - filename: The name of the new todo file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if an error occurred during file creation.
 */
int runNew(char *filename);

/*
 * Function: runOpen
 * -----------------
 * Opens a todo file with the specified filename in the user's todo directory.
 *
 * Parameters:
 *  - filename: The name of the todo file to open.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if the file does not exist.
 *  - -1 if an error occurred during file opening.
 */
int runOpen(char *filename);

/*
 * Function: runDelete
 * -------------------
 * Deletes a todo file with the specified filename from the user's todo directory.
 *
 * Parameters:
 *  - filename: The name of the todo file to delete.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if an error occurred during file deletion.
 */
int runDelete(char *filename);

/*
 * Function: runClose
 * ------------------
 * Closes the currently opened todo file and frees associated memory.
 *
 * Parameters:
 *  - filename: A pointer to store the name of the closed file.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if no file is currently open.
 */
int runClose(char *filename);

/*
 * Function: runList
 * -----------------
 * Lists all todo files in the user's todo directory.
 *
 * Parameters:
 *  - files: Pointer to an array of strings to store file names.
 *  - allocSize: Pointer to an integer to store allocated size of the files array.
 *  - writeFiles: Pointer to an integer to store the number of files listed.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if an error occurred during listing.
 */
int runList(char **files, int *numFiles, int *writeFiles);

/*
 * Function: runLook
 * -----------------
 * Retrieves the currently opened todo list.
 *
 * Parameters:
 *  - workingFile: A pointer to store the name of the working file.
 *
 * Returns:
 *  - Pointer to the TodoArray structure containing the todo list.
 */
TodoArray *runLook(char *workingFile);

/*
 * Function: runEdit
 * -----------------
 * Edits a todo in the todo list by its ID.
 *
 * Parameters:
 *  - todo: Pointer to the Todo structure containing updated todo information.
 *
 * Returns:
 *  - 0 on success.
 */
int runEdit(Todo *todo);

/*
 * Function: runRemove
 * -------------------
 * Removes a todo from the todo list by its ID.
 *
 * Parameters:
 *  - id: The ID of the todo to remove.
 *
 * Returns:
 *  - 0 on success.
 */
int runRemove(int id);

/*
 * Function: runExit
 * -----------------
 * Closes the currently opened todo file and performs cleanup operations.
 *
 * Returns:
 *  - 1 if no file is open and cleanup is successful.
 *  - 0 if a file is open and cleanup is successful.
 */
int runExit();

/*
 * Function: checkTodoIdExist
 * --------------------------
 * Checks if a todo ID already exists in the todo list.
 *
 * Parameters:
 *  - id: The ID to check.
 *
 * Returns:
 *  - 0 if the ID exists.
 *  - 1 if the ID does not exist.
 */
int checkTodoIdExist(int id);

/*
 * Function: appendTodo
 * --------------------
 * Appends a todo to the todo list, generating a unique ID if necessary.
 *
 * Parameters:
 *  - todo: The todo to append.
 */
void appendTodo(Todo todo);

/*
 * Function: checkFileOpen
 * -----------------------
 * Checks if a file is currently open.
 *
 * Returns:
 *  - 0 if no file is open.
 *  - 1 if a file is open.
 */
int checkFileOpen();


#endif