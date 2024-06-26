
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils/utils.h"

#include "models/user.h"

int login(User *loginUser, User *foundUser)
{
  if (findUser(loginUser, foundUser) != 0)
  {
    println("Coundn't find user with %s username", loginUser->username);
    return 1;
  };
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
  fwrite(newUser, sizeof(User), 1, fp);
  memcpy(storedUser, newUser, sizeof(User));
  fclose(fp);
  return 0;
}

int findUser(User *searchUser, User *foundUser)
{
  FILE *fp;

  fp = fopen(USER_DATA_FILE, "rb");
  if (fp == NULL)
  {
    perror("Error opening file");
    return -1;
  }

  User tempUser;

  while (fread(&tempUser, sizeof(User), 1, fp) == 1)
  {
    if (
        strcmp(searchUser->username, tempUser.username) == 0)
    {
      memcpy(foundUser, &tempUser, sizeof(User));
      fclose(fp);
      return 0;
    }
  }

  fclose(fp);
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
