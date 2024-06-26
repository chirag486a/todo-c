
#include <stdio.h>
#include <string.h>

#include "views/user_view.h"
#include "utils/utils.h"

void getUserDetails(User *user)
{

  char username[USERNAME_LEN];
  char userpaswd[USERPASSWORD_LEN];

  // GET Username
  println("Enter your username");
  char *notErr = fgets(username, USERNAME_LEN, stdin);
  if (notErr)
    removeNewLine(username);
  strncpy(user->username, username, USERNAME_LEN - 1);

  // GET Userpasswd
  println("Enter your password");
  notErr = fgets(userpaswd, USERPASSWORD_LEN, stdin);
  if (notErr)
    removeNewLine(userpaswd);
  strncpy(user->userpassword, userpaswd, USERPASSWORD_LEN - 1);
}

int loginSignUpMenu()
{

  println("1. Login");
  println("2. Signup");

  int option;
  scanf("%d", &option);
  flushStdin();

  return option;
}