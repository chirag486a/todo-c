#ifndef MAINH

#define USERNAME_LEN 32
#define USERPASSWORD_LEN 32


typedef struct User
{
  char username[USERNAME_LEN];
  char userpasswd[USERPASSWORD_LEN];
} User;

#endif