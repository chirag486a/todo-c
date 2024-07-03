#ifndef TODO_VIEW_H
#define TODO_VIEW_H

#include "models/todo.h"

// Says Hello to the user
void greetUser(const char *userName);
int giveOptions();
void showHelp();

void askFileName(char *filename);
void showError(int failedCmd, char *err);
void displayList(char **files, int writeFiles);
void askTodo(Todo *todo);

void displayTodos(TodoArray *todos, char *workingFile);
int askId();

#endif