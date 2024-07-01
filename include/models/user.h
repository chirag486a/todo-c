
#ifndef USER_MODEL_H
#define USER_MODEL_H

#define USERNAME_LEN 32
#define USERPASSWORD_LEN 32

// User Data Storage
#define USER_DATA_FILE "./data/.userdata"
#define USER_TODO_DATA_DIR "./data/Users_Todo/"
#define ID_LEN 11
#define MAX_USERS 100

typedef char UserId[ID_LEN];

extern int loadedUserNum;

typedef struct User
{
  char username[USERNAME_LEN];
  char userpassword[USERPASSWORD_LEN];
  UserId userId;
} User;



int findUser(User *searchUser, User *findUser);
int signUp(User *newUser, User *storedUser);
int login(User *loginUser, User *foundUser);
int loadUser();

// size_t getUsersNumber(const char *filename);
// int retriveUser(User **user, size_t userNum);

#endif