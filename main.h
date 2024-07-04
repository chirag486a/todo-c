#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>


// DECLARE UTILITIES
// ~~~~~~~~~~~UTILS~~~~~~~~~~~~~~~~~~~~~start
void println(const char *format, ...);
void removeNewLine(char *string);
void clearTerminal();
void flushStdin();
void displayPrompt();
// ~~~~~~~~~~~UTILS~~~~~~~~~~~~~~~~~~~~~end


// DECLARE MODELS
// ~~~~~~~~~~~USER MODEL~~~~~~~~~~~~~~~~~~~~~start

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

int findUser(User *searchUser, User *findUser);
int signUp(User *newUser, User *storedUser);
int login(User *loginUser, User *foundUser);
int loadUser();
// ~~~~~~~~~~~USER MODEL~~~~~~~~~~~~~~~~~~~~~end

// DECLARE MODELS
// ~~~~~~~~~~~TODO MODEL~~~~~~~~~~~~~~~~~~~~~start

// #include "models/user.h"
// #include <time.h>

#define TODO_DESCRIPTION_SIZE 256

typedef enum Priority
{
  CRITICAL = 1,
  HIGH,
  MEDIUM,
  LATER
} Priority;

typedef struct Todo
{

  int id;
  char description[TODO_DESCRIPTION_SIZE];
  Priority priority;
  time_t timestamp;

} Todo;

typedef struct TodoArray
{
  Todo *todos;
  size_t size;
  size_t capacity;
} TodoArray;

typedef enum
{
  HELP,
  NEW,
  OPEN,
  DELETE,
  CLEAR,
  CLOSE,
  LIST,
  APPEND,
  SAVE,

  REMOVE,
  EDIT,
  LOOK,
  EXIT,

  UNKNOWN
} Command;

void prepTodoDir(const User *user);
int runSave();
int runNew(char *filename);
int runOpen(char *filename);
int runDelete(char *filename);
int runClose(char *filename);
int runList(char **files, int *numFiles, int *writeFiles);
TodoArray *runLook(char *workingFile);
int runEdit(Todo *todo);
int runRemove(int id);
int runExit();
int checkTodoIdExist(int id);
void appendTodo(Todo todo);
int checkFileOpen();


// ~~~~~~~~~~~TODO MODEL~~~~~~~~~~~~~~~~~~~~~end


// DEFINE VIEWS
// ~~~~~~~~~~~USER VIEWS~~~~~~~~~~~~~~~~~~~~~start
// #include "models/user.h"

void getUserDetails(User *user);
int loginSignUpMenu();
// ~~~~~~~~~~~USER VIWES~~~~~~~~~~~~~~~~~~~~~end


// DEFINE VIEW
// ~~~~~~~~~~~TODO VIEWS~~~~~~~~~~~~~~~~~~~~~start
// #include "models/todo.h"
// #include "utils/utils.h"
// #include "views/todo_view.h"
// #include "models/todo.h"

// Says Hello to the user
void greetUser(const char *userName);
int giveOptions();
void showHelp();

void askFileName(char *filename);
void showError(int failedCmd, char *err);
void displayList(char **files, int writeFiles);
void askTodo(Todo *todo);

void displayTodos(TodoArray *todos, char *workingFile);
int askId();


// ~~~~~~~~~~~TODO VIEWS~~~~~~~~~~~~~~~~~~~~~end





// DEFINE CONTROLLER
// ~~~~~~~~~~~USER CONTROLLER~~~~~~~~~~~~~~~~~~~~~start
// #include "controllers/user_controller.h"
// #include "models/user.h"
// #include "views/user_view.h"
// #include "utils/utils.h"

int loginSignUp(User *user);
// ~~~~~~~~~~~USER CONTROLLER~~~~~~~~~~~~~~~~~~~~~end



// DEFINE CONTROLLER
// ~~~~~~~~~~~TODO CONTROLLER~~~~~~~~~~~~~~~~~~~~~start

// #include "controllers/todo_controller.h"
// #include "models/todo.h"
// #include "views/todo_view.h"
// #include "utils/utils.h"

int initTodo(User *user);
int runApp();

// ~~~~~~~~~~~TODO CONTROLLER~~~~~~~~~~~~~~~~~~~~~end

