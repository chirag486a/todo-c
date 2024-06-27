// 1. Assign user 10-digit Id
// 2 Based on that immediately create directory for user
// 3. Create view for Todo [Add, Delete, Edit]
// 4. Create Data structure
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "controllers/user_controller.h"

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
  println(user.username);
  println("Welcome to our app");

  return 0;
}