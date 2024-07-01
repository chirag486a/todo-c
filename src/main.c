// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1. Assign user 10-digit Id
// 2 Based on that immediately create directory for user
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1. Create view for Todo [Add, Delete, Edit]
// 2. Create TODO Data structure
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "controllers/user_controller.h"
#include "controllers/todo_controller.h"

int main()
{
  loadUser();
  User user;

  int statusLoginSignUp;
  do
  {
    statusLoginSignUp = loginSignUp(&user);
  } while (statusLoginSignUp > 0);

  if (statusLoginSignUp < 0)
  {
    println("Exiting...");
    return 1;
  };

  initTodo(&user);

  return 0;
}