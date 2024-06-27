
#include <stdio.h>
#include "controllers/user_controller.h"

int userLogin(User *user, User *foundUser)
{
  getUserDetails(user);
  return login(user, foundUser);
}

/*
Returns 1: Username Already Taken
Returns 2: Unable to Initialize User Directory
Returns 0: On Success
*/
int userSignUp(User *user, User *storedUser)
{
  getUserDetails(user);
  int signUpStatus = signUp(user, storedUser);
  if (signUpStatus == 1)
  {

    println("User name already taken");
    return 1;
  }
  else if (signUpStatus != 0)
  {
    println("Something went wrong");
    return -1;
  }
}

int loginSignUp(User *user)
{
  int option = loginSignUpMenu();
  User storedUser;

  switch (option)
  {
  case 1:
    return userLogin(user, &storedUser);
    break;
  case 2:
    return userSignUp(user, &storedUser);
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
