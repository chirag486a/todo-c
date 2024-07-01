#include "models/todo.h"
#include "models/user.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

char userTodoDir[FILENAME_MAX] = USER_TODO_DATA_DIR;
char openedFile[FILENAME_MAX];
size_t todoListDefaultCapacity;

void initTodoArray(TodoArray *array, size_t capicity)
{
  array->todos = (Todo *)malloc(sizeof(Todo) * capicity);
  array->size = 0;
  array->capacity = capicity;
};

void appendTodo(TodoArray *array, Todo todo)
{
  if (array->size >= array->capacity)
  {
    array->capacity *= 2;
    array->todos = (Todo *)malloc(sizeof(Todo) * array->capacity);
  }
  array->todos[array->size++] = todo;
}
void removeTodoById(TodoArray *array, int id)
{
  for (size_t i = 0; i < array->size; i++)
  {
    if (array->todos[i].id == id)
    {
      for (size_t j = i; j < array->size - 1; j++)
      {
        array->todos[j] = array->todos[j + 1];
      }
      array->size--;
      return;
    }
  }
}
int checkTodoIdExist(TodoArray *array, int id)
{
  for (size_t i = 0; i < array->size; i++)
  {
    if (array->todos[i].id == id)
      return 0;
  }
  return 1;
}
void editTodoById(TodoArray *array, int id, const char *newDescription, Priority newPriority)
{
  for (size_t i = 0; i < array->size; i++)
  {
    if (array->todos[i].id == id)
    {
      strncpy(array->todos[i].description, newDescription, 256);
      array->todos[i].priority = newPriority;
      return;
    }
  }
}

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

int loadTodos(TodoArray *array, char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    return -1;
  }
  // Read the size of array

  TodoArray *todoList = malloc(sizeof(TodoArray));
  array = todoList;

  if (fread(&(todoList->size), sizeof(size_t), 1, fp) == 0)
  {
    fclose(fp);
    return 0;
  }

  // Allocate memory for the array
  todoList->capacity = todoList->size;
  todoList->todos = (Todo *)malloc(sizeof(Todo) * todoList->capacity);

  fread(todoList->todos, sizeof(Todo), todoList->size, fp);
  fclose(fp);
  return todoList->size;
}

int saveTodoArrayToFile(TodoArray *todoList, const char *filename)
{
  FILE *fp = fopen(openedFile, "rb");
  if (fp == NULL)
  {
    return 1;
  }
  // Write the size of the array
  fwrite(&todoList->size, sizeof(size_t), 1, fp);

  // Write the array data
  fwrite(&todoList->todos, sizeof(Todo), todoList->size, fp);

  fclose(fp);
  return 0;
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
    TodoArray todoList;
    if (loadTodos(&todoList, openedFile) > 0)
      return 0;
    initTodoArray(&todoList, todoListDefaultCapacity);
    return 0;
  }
  return 1;
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
  return 0;
}

int runList(char **files, int *numFiles, int *writeFiles)
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
    if (i == *numFiles)
    {
      *numFiles *= 2;
      files = realloc(files, sizeof(char *) * (*numFiles));
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
