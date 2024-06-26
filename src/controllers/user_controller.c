
#include <stdio.h>
#include "controllers/user_controller.h"

int userLogin(User *user, User *foundUser)
{
  getUserDetails(user);
  return login(user, foundUser);
}

int userSignUp(User *user, User *storedUser)
{
  getUserDetails(user);
  return signUp(user, storedUser);
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
