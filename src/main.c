
/*
features left
  ~ LOGOUT
  ~ DISCARD if not save
  ~ SORT BY creation date, priority

*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// IMPORT CONTROLLERS
#include "controllers/user_controller.h"
#include "controllers/todo_controller.h"

// Start Application
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
  runApp();

  return 0;
}