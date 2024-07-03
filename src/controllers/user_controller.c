
#include <stdio.h>
#include "controllers/user_controller.h"

/*
 * Function: userLogin
 * -------------------
 * Logs in a user by getting user details, calling the login function,
 * and returning the result.
 *
 * Parameters:
 *  - storedUser: A pointer to a User structure where the logged-in user's information will be copied.
 *
 * Returns:
 *  - Result of the login operation as returned by the login function.
 */
int userLogin(User *storedUser)
{
  User user;
  getUserDetails(&user);

  return login(&user, storedUser);
}

/*
 * Function: userSignUp
 * --------------------
 * Handles the user signup process by getting user details, signing up the user,
 * and returning status codes based on the outcome.
 *
 * Parameters:
 *  - storedUser: A pointer to a User structure where the stored user's information will be copied.
 *
 * Returns:
 *  - 1 if the username is already taken.
 *  - -1 if there was an error during the signup process.
 *  - 0 on successful signup.
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
