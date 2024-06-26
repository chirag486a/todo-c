#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "controllers/user_controller.h"


int main() {
  User user;
  if(loginSignUp(&user)) {
    println("Exiting...");
    return 1;
  };
  println(user.username);
  println("Welcome to our app");

  // 1. Assign user 10-digit Id
  // 2 Based on that immediately create directory for user
  // 3. Create view for Todo [Add, Delete, Edit]
  // 4. Create Data structure




  return 0;
}