
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "utils/utils.h"
#include "models/user.h"

User users[MAX_USERS];

int loadedUserNum;

int login(User *loginUser, User *foundUser)
{
  if (findUser(loginUser, foundUser) != 0) return 1;
  if (strcmp(foundUser->userpassword, loginUser->userpassword) != 0)
    return 2;

  return 0;
}

// Generates Unique Id
void generateUniqueId(char *id)
{
  // Seed the random number generator
  srand(time(NULL)); 

  // Generate a large random number
  unsigned long long random_number = (unsigned long long)rand() * (RAND_MAX ) + rand(); 

  // Format as a zero-padded 10-digit number
  sprintf(id, "%010llu", random_number % 10000000000ULL);
}

// Returns no. of users loaded
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
RETURNS 1 for true
RETURNS 2 for false
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
Returns 1: Username Already Taken
Returns 2: Unable to Initialize User Directory
Returns 0: On success
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

/*
Returns 1: Username Already Taken
Returns 2: Unable to Initialize User Directory
Returns 0: On Success

*/
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
