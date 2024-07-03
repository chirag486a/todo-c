#include "models/todo.h"
#include "models/user.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

// Directory path for user todo data
char userTodoDir[FILENAME_MAX] = USER_TODO_DATA_DIR;

// Path of the current opened file
char openedFile[FILENAME_MAX];

// Dfault capacity for todo list array
size_t todoListDefaultCapacity = 10;

// Pointer to the todo list array
TodoArray *todoList;

int checkFileOpen()
{
  if (strcmp(openedFile, "") == 0)
    return 0;
  else
    return 1;
}

int checkTodoIdExist(int id)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == id)
      return 0;
  }
  return 1;
}
int generateUniqueTodoId()
{
  static int currentId = 0;
  return ++currentId;
}

void appendTodo(Todo todo)
{
  if (todoList->size >= todoList->capacity)
  {
    todoList->capacity *= 2;
    todoList->todos = (Todo *)realloc(todoList->todos, sizeof(Todo) * todoList->capacity);
  }
  int todoId;
  do
  {
    if (todo.id > 0)
    {
      todoId = todo.id;
      break;
    }

    todoId = generateUniqueTodoId();
    println("%d", todoId);
  } while (!checkTodoIdExist(todoId));

  todo.id = todoId;
  time(&(todo.timestamp));
  todoList->todos[todoList->size++] = todo;
}

/*
 * Function: removeTodoById
 * ------------------------
 * Removes a todo from the todo list by its ID.
 *
 * Parameters:
 *  - id: The ID of the todo to remove.
 */
void removeTodoById(int id)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == id)
    {
      for (size_t j = i; j < todoList->size - 1; j++)
      {
        todoList->todos[j] = todoList->todos[j + 1];
      }
      todoList->size--;
      return;
    }
  }
}

/*
 * Function: editTodoById
 * ----------------------
 * Edits a todo in the todo list by its ID.
 *
 * Parameters:
 *  - todo: Pointer to the todo structure containing the updated todo information.
 */
void editTodoById(Todo *todo)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == todo->id)
    {
      time(&(todo->timestamp));
      todoList->todos[i] = *todo;
      break;
    }
  }
}

/*
 * Function: freeTodoArray
 * -----------------------
 * Frees memory allocated for the todo list array.
 *
 * Parameters:
 *  - array: Pointer to the TodoArray structure to free.
 */
void freeTodoArray(TodoArray *array)
{
  free(array->todos);
  array->size = 0;
  array->capacity = 0;
}

void prepTodoDir(const User *user)
{
  strcat(userTodoDir, user->userId);
  println("Setting user directory: %s", userTodoDir);
}

/*
 * Function: loadTodos
 * -------------------
 * Loads todos from a specified file into the todo list.
 *
 * Parameters:
 *  - filename: The name of the file to load todos from.
 *
 * Returns:
 *  - 0 on success.
 *  - 1 if an error occurred during loading.
 *  - -1 if the file could not be opened.
 */
int loadTodos(char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    return -1;
  }

  todoList = malloc(sizeof(TodoArray));
  if (fread(&(todoList->size), sizeof(size_t), 1, fp) != 1)
  {
    free(todoList);
    fclose(fp);
    return 1;
  }

  // Allocate memory for the array
  println("LOADER SIZE: %d", todoList->size);
  todoList->capacity = todoList->size;
  todoList->todos = (Todo *)malloc(sizeof(Todo) * todoList->capacity);

  if (fread(todoList->todos, sizeof(Todo), todoList->size, fp) != todoList->size)
  {
    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }
  println("file loaded successfully");
  fclose(fp);
  return 0;
}

int runSave()
{
  FILE *fp = fopen(openedFile, "wb");
  if (fp == NULL)
  {
    return 1;
  }
  // Write the size of the array
  int err1 = fwrite(&todoList->size, sizeof(size_t), 1, fp);
  if (err1 != 1)
  {

    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }

  // Write the array data
  int err2 = fwrite(todoList->todos, sizeof(Todo), todoList->size, fp);

  if (err2 != todoList->size)
  {
    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return todoList->size;
}

int runNew(char *filename)
{
  char filefullAddress[FILENAME_MAX];
  strcpy(filefullAddress, userTodoDir);
  strcat(filefullAddress, "/");
  strcat(filefullAddress, filename);

  FILE *fp = fopen(filefullAddress, "w");

  if (fp == NULL)
  {
    return 1;
  }

  fclose(fp);
  println("Created: %s", filefullAddress);

  return 0;
}

int runOpen(char *filename)
{
  char temp[FILENAME_MAX];
  strcpy(temp, userTodoDir);
  strcat(temp, "/");
  strcat(temp, filename);
  if (access(temp, F_OK) != -1)
  {
    strcpy(openedFile, temp);
    println("File %s Opened", openedFile);

    // check if file already exist
    if (loadTodos(openedFile) == 0)
    {
      return 0;
    }

    todoList = malloc(sizeof(TodoArray));

    // Initialize todo list
    todoList->capacity = todoListDefaultCapacity;
    todoList->todos = (Todo *)malloc(sizeof(Todo) * todoList->capacity);
    todoList->size = 0;
    return 0;
  }
  return -1;
}

int runDelete(char *filename)
{
  char temp[FILENAME_MAX];
  strcpy(temp, userTodoDir);
  strcat(temp, "/");
  strcat(temp, filename);
  if (strcmp(temp, openedFile) == 0)
    strcpy(openedFile, "");
  if (remove(temp) == 0)
  {
    return 0;
  }
  return 1;
}

int runClose(char *filename)
{
  if (strcmp(openedFile, "") == 0)
  {
    return 1;
  };

  strcpy(filename, openedFile);
  strcpy(openedFile, "");
  freeTodoArray(todoList);
  free(todoList);
  return 0;
}

int runList(char **files, int *allocSize, int *writeFiles)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(userTodoDir);

  if (!d)
    return 1;

  int i = 0;
  while ((dir = readdir(d)) != NULL)
  {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    if (i >= *allocSize)
    {
      *allocSize *= 2;
      files = realloc(files, sizeof(char *) * (*allocSize));
      if (!files)
      {
        closedir(d);
        return 1;
      }
    }

    int fileNameLen = strlen(dir->d_name);
    // println("%d) %s", fileNameLen, dir->d_name);
    // println("%d", sizeof(fileNameLen));
    // println("------------------------------------------------");

    files[i] = (char *)malloc((sizeof(char) * fileNameLen) + 1);
    if (!files[i])
    {
      closedir(d);
      perror("Error allocating memory");
      return 1;
    }

    strcpy(files[i], dir->d_name);

    i++;
  }
  *writeFiles = i;
  closedir(d);

  return 0;
}

TodoArray *runLook(char *workingFile)
{
  strcpy(workingFile, openedFile);
  return todoList;
}

int runEdit(Todo *todo)
{
  editTodoById(todo);
  return 0;
}

int runRemove(int id)
{
  removeTodoById(id);
  return 0;
}

int runExit()
{
  if (!checkFileOpen())
  {
    runClose(openedFile);
    return 1;
  }
  return 0;
}