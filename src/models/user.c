
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "utils/utils.h"
#include "models/user.h"

/*
 * Global variable: users
 * ----------------------
 * An array to store user data.
 *
 * Note:
 *  - The size of the array is defined by the constant `MAX_USERS`.
 */
User users[MAX_USERS];

/*
 * Global variable: loadedUserNum
 * ------------------------------
 * Stores the number of users loaded from the file.
 */
int loadedUserNum;

/*
 * Function: login
 * --------------------
 * Authenticates a user by checking their credentials against stored user data.
 *
 * Parameters:
 *  - loginUser: A pointer to a User structure containing the login credentials.
 *  - foundUser: A pointer to a User structure where the found user data will be stored.
 *
 * Returns:
 *  - 0 if login is successful.
 *  - 1 if the user is not found.
 *  - 2 if the password does not match.
 *
 * Notes:
 *  - This function uses the `findUser` function to locate the user.
 *  - The passwords are compared using `strcmp`.
 */
int login(User *loginUser, User *foundUser)
{
  if (findUser(loginUser, foundUser) != 0)
    return 1;
  if (strcmp(foundUser->userpassword, loginUser->userpassword) != 0)
    return 2;

  return 0;
}

/*
 * Function: generateUniqueId
 * ---------------------------
 * Generates a unique 10-digit ID and stores it in the provided character array.
 *
 * Parameters:
 *  - id: A pointer to a character array where the generated ID will be stored.
 */
void generateUniqueId(char *id)
{
  // Seed the random number generator with the current time
  srand(time(NULL)); // This initializes the random number generator with a seed based on the current time.

  // Generate a large random number by combining two calls to rand()
  unsigned long long random_number = (unsigned long long)rand() * (RAND_MAX) + rand();
  // This line creates a large random number by multiplying the result of rand() by RAND_MAX and adding another call to rand().

  // Format the random number as a zero-padded 10-digit string and store it in the provided character array
  sprintf(id, "%010llu", random_number % 10000000000ULL);
  // This line uses sprintf() to format the random number as a zero-padded 10-digit string and store it in the array pointed to by `id`.
}

int loadUser()
{
  FILE *fp;

  fp = fopen(USER_DATA_FILE, "rb");
  if (fp == NULL)
  {
    println("Error opening file");
    return -1;
  }
  int i = 0;
  while (fread(&users[i], sizeof(User), 1, fp) == 1)
  {
    i++;
  }
  loadedUserNum = i + 1;
  return i + 1;
}

/*
 * Function: idExist
 * -----------------
 * Checks if a given ID exists in the array of users.
 *
 * Parameters:
 *  - id: A pointer to a string representing the ID to check.
 *
 * Returns:
 *  - 1 if the ID exists.
 *  - 0 if the ID does not exist.
 */
int idExist(char *id)
{

  for (int i = 0; i < loadedUserNum; i++)
  {
    if (strcmp(users[i].userId, id) == 0)
    {
      return 1;
    }
  }

  return 0;
}

/*
 * Function: isUniqueUserName
 * --------------------------
 * Checks if a username is unique by comparing it with usernames in the array of users.
 *
 * Parameters:
 *  - user: A pointer to a User structure containing the username to check.
 *
 * Returns:
 *  - 1 if the username is not unique (already exists).
 *  - 0 if the username is unique (does not exist).
 */
int isUniqueUserName(User *user)
{
  for (int i = 0; i < loadedUserNum; i++)
  {
    if (strcmp(users[i].username, user->username) == 0)
    {
      return 1;
    }
  }
  return 0;
}

/*
 * Function: initUser
 * ------------------
 * Initializes a new user by generating a unique ID, checking username uniqueness,
 * creating a user directory, and setting up user data.
 *
 * Parameters:
 *  - user: A pointer to a User structure containing the new user's information.
 *
 * Returns:
 *  - 0 if user initialization is successful.
 *  - 1 if the username is already taken.
 *  - 2 if there was an error creating the user directory.
 */
int initUser(User *user)
{
  do
  {
    generateUniqueId(user->userId);
  } while (idExist(user->userId));

  if (isUniqueUserName(user))
  {
    println("Username already taken");
    return 1;
  }
  char path[64] = USER_TODO_DATA_DIR;
  strcat(path, user->userId);
  println(path);

  int status = mkdir(path, 0777);

  if (status == 0)
  {
    println("User Directory Created.");
  }
  else
  {
    println("Something went wrong while inintializing user");
    return 2;
  }
  return 0;
}

int signUp(User *newUser, User *storedUser)
{

  FILE *fp;
  fp = fopen(USER_DATA_FILE, "ab");
  if (fp == NULL)
  {
    println("Error opening file");
    return -1;
  }

  // Store User
  int initStatus = initUser(newUser);
  if (initStatus != 0)
  {
    fclose(fp);
    return initStatus;
  }
  fwrite(newUser, sizeof(User), 1, fp);
  memcpy(storedUser, newUser, sizeof(User));
  fclose(fp);
  return 0;
}

int findUser(User *searchUser, User *foundUser)
{
  for (int i = 0; i <= loadedUserNum; i++)
  {
    if (strcmp(users[i].username, searchUser->username) == 0)
    {
      memcpy(foundUser, &users[i], sizeof(User));
      return 0;
    }
  }

  return 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// STORE USER TO BUFFER
// int retriveUser(User **user, size_t userNum)
// {
//   FILE *fp;

//   fp = fopen(USER_DATA_FILE, "rb");
//   if (fp == NULL)
//   {
//     perror("Error opening file");
//     return 0;
//   }
//   int i = 0;
//   while (fread(user[i], sizeof(User), 1, fp) == 1)
//   {
//     i++;
//   }

//   fclose(fp);
//   return 0;
// }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GET TOTAL USERS IN DB
// size_t getUsersNumber(const char *filename)
// {
//   FILE *fp = fopen(filename, "rb");
//   if (fp == NULL)
//   {
//     perror("Error opening file");
//     return -1;
//   }

//   // Seek to the end of the file
//   if (fseek(fp, 0, SEEK_END) != 0)
//   {
//     perror("Error seeking to the end of file");
//     fclose(fp);
//     return -2;
//   }

//   long fileSize = ftell(fp);
//   if (fileSize == -1)
//   {
//     perror("Error getting file size");
//     fclose(fp);
//     return -3;
//   }

//   // Calculate the number of users
//   size_t numberOfUsers = fileSize / sizeof(User);

//   fclose(fp);
//   return numberOfUsers;
// }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
