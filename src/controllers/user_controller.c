
#include <stdio.h>
#include "controllers/user_controller.h"

int userLogin(User *storedUser)
{
  User user;
  getUserDetails(&user);

  return login(&user, storedUser);
}

/*
Returns 1: Username Already Taken
Returns 2: Unable to Initialize User Directory
Returns 0: On Success
*/
int userSignUp(User *storedUser)
{
  User user;
  getUserDetails(&user);
  int signUpStatus = signUp(&user, storedUser);
  if (signUpStatus == 1)
  {
    println("User name already taken");
    return 1;
  }
  if (signUpStatus != 0)
  {
    println("Something went wrong");
    return -1;
  }
  return 0;
}

int loginSignUp(User *user)
{
  int option = loginSignUpMenu();

  switch (option)
  {
  case 1:
    return userLogin(user);
    break;
  case 2:
    return userSignUp(user);
    break;

  default:
  {
    println("Please provide valid input");
    return 1;
  }
  break;
  }
  return 0;
}
