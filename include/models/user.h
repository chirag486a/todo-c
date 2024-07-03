
#ifndef USER_MODEL_H
#define USER_MODEL_H

// Maximum length for username
#define USERNAME_LEN 32

// Maximum length for password
#define USERPASSWORD_LEN 32

// File paths for user crediential details. 
#define USER_DATA_FILE "./data/.userdata"

// Directory paths for user data storage
#define USER_TODO_DATA_DIR "./data/Users_Todo/"

// Length of user ID (including null terminator)
#define ID_LEN 11

// Maximum number of users that can be stored
#define MAX_USERS 100

// User id of type char
typedef char UserId[ID_LEN];

// Declaration of the global variable to keep track of loaded users
extern int loadedUserNum;

typedef struct User
{
  char username[USERNAME_LEN];
  char userpassword[USERPASSWORD_LEN];
  UserId userId;
} User;

/*
 * Function: findUser
 * ------------------
 * Finds a user by searching for their username in the array of users.
 *
 * Parameters:
 *  - searchUser: A pointer to a User structure containing the username to search for.
 *  - foundUser: A pointer to a User structure where the found user's information will be copied.
 *
 * Returns:
 *  - 0 if the user is found and copied to foundUser successfully.
 *  - 1 if the user is not found.
 */
int findUser(User *searchUser, User *findUser);

/*
 * Function: signUp
 * ----------------
 * Registers a new user by storing their information in a file.
 *
 * Parameters:
 *  - newUser: A pointer to a User structure containing the new user's information.
 *  - storedUser: A pointer to a User structure where the stored user's information will be copied.
 *
 * Returns:
 *  - 0 if the user registration is successful.
 *  - -1 if there was an error opening the file.
 *  - Any error code returned by `initUser` function if user initialization fails.
 */
int signUp(User *newUser, User *storedUser);

int login(User *loginUser, User *foundUser);

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
int loadUser();

#endif