#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/main.h"

// User Data Storage
const char *fileUserData = "./data/userdata";
// Authorization Feature
void registerUser();
int storeUser(User *user);
size_t getUsersNumber(const char *filename);
int retriveUser(User **user, size_t userNum);
int findUser(User user);

void menu() {
  println("Welcome!...");
  println("Please Type");
  println("1. Sign Up");
  println("2. Login");
}

int main(int argc, char *argv[])
{
  println("Welcome to newbada... %d", 43);
}

void registerUser()
{
  User currentUser;

  char username[USERNAME_LEN];
  char userpaswd[USERPASSWORD_LEN];

  println("Enter your username");
  char *notErr = fgets(username, USERNAME_LEN, stdin);
  if (notErr)
    removeNewLine(username);
  strncpy(currentUser.username, username, USERNAME_LEN - 1);

  println("Enter your password");
  notErr = fgets(userpaswd, USERPASSWORD_LEN, stdin);
  if (notErr)
    removeNewLine(userpaswd);
  strncpy(currentUser.userpasswd, userpaswd, USERPASSWORD_LEN - 1);

  storeUser(&currentUser);
  int userCount = getUsersNumber(fileUserData);

  User *user = (User *)malloc(sizeof(User) * userCount);
  println("userCount %d", userCount);
  for (int i = 0; i < userCount; i++)
  {
    println("User Name: %s", user[i].username);
    println("User Name: %s", user[i].userpasswd);
  }
  free(user);
}

int storeUser(User *user)
{
  FILE *fp;
  fp = fopen(fileUserData, "ab");
  if (fp == NULL)
  {
    perror("Error opening file");
    return -1;
  }
  fwrite(user, sizeof(User), 1, fp);
  return fclose(fp);
}

int retriveUser(User **user, size_t userNum)
{
  FILE *fp;

  fp = fopen(fileUserData, "rb");
  if (fp == NULL)
  {
    perror("Error opening file");
    return 0;
  }
  int i = 0;
  while (fread(user[i], sizeof(User), 1, fp) == 1)
  {
    i++;
  }

  fclose(fp);
  return 0;
}

size_t getUsersNumber(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    perror("Error opening file");
    return -1;
  }

  // Seek to the end of the file
  if (fseek(fp, 0, SEEK_END) != 0)
  {
    perror("Error seeking to the end of file");
    fclose(fp);
    return -2;
  }

  long fileSize = ftell(fp);
  if (fileSize == -1)
  {
    perror("Error getting file size");
    fclose(fp);
    return -3;
  }

  // Calculate the number of users
  size_t numberOfUsers = fileSize / sizeof(User);

  fclose(fp);
  return numberOfUsers;
}

int findUser(User user)
{
  FILE *fp;

  fp = fopen(fileUserData, "rb");
  if (fp == NULL)
  {
    perror("Error opening file");
    return 0;
  }
  User fUser;
  int i = 0;
  while (fread(&fUser, sizeof(User), 1, fp) == 1)
  {
    if (
        strcmp(user.username, fUser.username) == 0 &&
        strcmp(user.userpasswd, fUser.userpasswd) == 0)
    {
      println("User Authenticated");
      return 0;
    }
    i++;
  }

  fclose(fp);
  return 1;
}