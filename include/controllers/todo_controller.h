#ifndef TODO_CONTROLLER_H
#define TODO_CONTROLLER_H

#include "models/user.h"

/*
 * Function: initTodo
 * ------------------
 * Initializes the todo application by greeting the user and preparing the todo directory.
 *
 * Parameters:
 *  - user: Pointer to the User structure containing user information.
 *
 * Returns:
 *  - 0 on success.
 */
int initTodo(User *user);


/*
 * Function: runApp
 * ----------------
 * Runs the main todo application loop, continuously accepting and executing commands.
 *
 * Returns:
 *  - 0 on success.
 */
int runApp();

#endif