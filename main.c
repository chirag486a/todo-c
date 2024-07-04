
#include "main.h"

// Start Application
int main()
{
  
  loadUser();
  User user;

  int statusLoginSignUp;
  do
  {
    statusLoginSignUp = loginSignUp(&user);
  } while (statusLoginSignUp > 0);

  if (statusLoginSignUp < 0)
  {
    println("Exiting...");
    return 1;
  };

  initTodo(&user);
  runApp();

  return 0;
}

// ~~~~~~~~~~~~~~UTILITY~~~~~~~~~~~~~~~~~~~~~~~start

void println(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

void removeNewLine(char *string)
{
  string[strcspn(string, "\n")] = '\0';
}

void clearTerminal()
{
#ifdef _WIN32
  system("cls"); // Windows
#else
  printf("\033[H\033[J"); // Unix-like (Linux, macOS)
#endif
}

void flushStdin() {
  int c;
  while((c = getchar()) != '\n' && c != EOF) {}
}

void displayPrompt() {
  printf(" > ");
}
// ~~~~~~~~~~~~~~UTILITY~~~~~~~~~~~~~~~~~~~~~~~end

// ~~~~~~~~~~~~~~USER~~~~~~~~~~~~~~~~~~~~~~~~~~start

User users[MAX_USERS];

int loadedUserNum;

int login(User *loginUser, User *foundUser)
{
  if (findUser(loginUser, foundUser) != 0)
    return 1;
  if (strcmp(foundUser->userpassword, loginUser->userpassword) != 0)
    return 2;

  return 0;
}
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

// ~~~~~~~~~~~~~~USER~~~~~~~~~~~~~~~~~~~~~~~~~~end


// ~~~~~~~~~~~~~~TODO~~~~~~~~~~~~~~~~~~~~~~~~~~start

// Directory path for user todo data
char userTodoDir[FILENAME_MAX] = USER_TODO_DATA_DIR;

// Path of the current opened file
char openedFile[FILENAME_MAX];

// Dfault capacity for todo list array
size_t todoListDefaultCapacity = 10;

// Pointer to the todo list array
TodoArray *todoList;

int checkFileOpen()
{
  if (strcmp(openedFile, "") == 0)
    return 0;
  else
    return 1;
}

int checkTodoIdExist(int id)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == id)
      return 0;
  }
  return 1;
}
int generateUniqueTodoId()
{
  static int currentId = 0;
  return ++currentId;
}

void appendTodo(Todo todo)
{
  if (todoList->size >= todoList->capacity)
  {
    todoList->capacity *= 2;
    todoList->todos = (Todo *)realloc(todoList->todos, sizeof(Todo) * todoList->capacity);
  }
  int todoId;
  do
  {
    if (todo.id > 0)
    {
      todoId = todo.id;
      break;
    }

    todoId = generateUniqueTodoId();
    println("%d", todoId);
  } while (!checkTodoIdExist(todoId));

  todo.id = todoId;
  time(&(todo.timestamp));
  todoList->todos[todoList->size++] = todo;
}

void removeTodoById(int id)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == id)
    {
      for (size_t j = i; j < todoList->size - 1; j++)
      {
        todoList->todos[j] = todoList->todos[j + 1];
      }
      todoList->size--;
      return;
    }
  }
}

void editTodoById(Todo *todo)
{
  for (size_t i = 0; i < todoList->size; i++)
  {
    if (todoList->todos[i].id == todo->id)
    {
      time(&(todo->timestamp));
      todoList->todos[i] = *todo;
      break;
    }
  }
}

void freeTodoArray(TodoArray *array)
{
  free(array->todos);
  array->size = 0;
  array->capacity = 0;
}

void prepTodoDir(const User *user)
{
  strcat(userTodoDir, user->userId);
  println("Setting user directory: %s", userTodoDir);
}

int loadTodos(char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    return -1;
  }

  todoList = malloc(sizeof(TodoArray));
  if (fread(&(todoList->size), sizeof(size_t), 1, fp) != 1)
  {
    free(todoList);
    fclose(fp);
    return 1;
  }

  // Allocate memory for the array
  println("LOADER SIZE: %d", todoList->size);
  todoList->capacity = todoList->size;
  todoList->todos = (Todo *)malloc(sizeof(Todo) * todoList->capacity);

  if (fread(todoList->todos, sizeof(Todo), todoList->size, fp) != todoList->size)
  {
    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }
  println("file loaded successfully");
  fclose(fp);
  return 0;
}

int runSave()
{
  FILE *fp = fopen(openedFile, "wb");
  if (fp == NULL)
  {
    return 1;
  }
  // Write the size of the array
  int err1 = fwrite(&todoList->size, sizeof(size_t), 1, fp);
  if (err1 != 1)
  {

    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }

  // Write the array data
  int err2 = fwrite(todoList->todos, sizeof(Todo), todoList->size, fp);

  if (err2 != todoList->size)
  {
    freeTodoArray(todoList);
    free(todoList);
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return todoList->size;
}

int runNew(char *filename)
{
  char filefullAddress[FILENAME_MAX];
  strcpy(filefullAddress, userTodoDir);
  strcat(filefullAddress, "/");
  strcat(filefullAddress, filename);

  FILE *fp = fopen(filefullAddress, "w");

  if (fp == NULL)
  {
    return 1;
  }

  fclose(fp);
  println("Created: %s", filefullAddress);

  return 0;
}

int runOpen(char *filename)
{
  char temp[FILENAME_MAX];
  strcpy(temp, userTodoDir);
  strcat(temp, "/");
  strcat(temp, filename);
  if (access(temp, F_OK) != -1)
  {
    strcpy(openedFile, temp);
    println("File %s Opened", openedFile);

    // check if file already exist
    if (loadTodos(openedFile) == 0)
    {
      return 0;
    }

    todoList = malloc(sizeof(TodoArray));

    // Initialize todo list
    todoList->capacity = todoListDefaultCapacity;
    todoList->todos = (Todo *)malloc(sizeof(Todo) * todoList->capacity);
    todoList->size = 0;
    return 0;
  }
  return -1;
}

int runDelete(char *filename)
{
  char temp[FILENAME_MAX];
  strcpy(temp, userTodoDir);
  strcat(temp, "/");
  strcat(temp, filename);
  if (strcmp(temp, openedFile) == 0)
    strcpy(openedFile, "");
  if (remove(temp) == 0)
  {
    return 0;
  }
  return 1;
}

int runClose(char *filename)
{
  if (strcmp(openedFile, "") == 0)
  {
    return 1;
  };

  strcpy(filename, openedFile);
  strcpy(openedFile, "");
  freeTodoArray(todoList);
  free(todoList);
  return 0;
}

int runList(char **files, int *allocSize, int *writeFiles)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(userTodoDir);

  if (!d)
    return 1;

  int i = 0;
  while ((dir = readdir(d)) != NULL)
  {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    if (i >= *allocSize)
    {
      *allocSize *= 2;
      files = realloc(files, sizeof(char *) * (*allocSize));
      if (!files)
      {
        closedir(d);
        return 1;
      }
    }

    int fileNameLen = strlen(dir->d_name);
    // println("%d) %s", fileNameLen, dir->d_name);
    // println("%d", sizeof(fileNameLen));
    // println("------------------------------------------------");

    files[i] = (char *)malloc((sizeof(char) * fileNameLen) + 1);
    if (!files[i])
    {
      closedir(d);
      perror("Error allocating memory");
      return 1;
    }

    strcpy(files[i], dir->d_name);

    i++;
  }
  *writeFiles = i;
  closedir(d);

  return 0;
}

TodoArray *runLook(char *workingFile)
{
  strcpy(workingFile, openedFile);
  return todoList;
}

int runEdit(Todo *todo)
{
  editTodoById(todo);
  return 0;
}

int runRemove(int id)
{
  removeTodoById(id);
  return 0;
}

int runExit()
{
  if (!checkFileOpen())
  {
    runClose(openedFile);
    return 1;
  }
  return 0;
}

// ~~~~~~~~~~~~~~TODO~~~~~~~~~~~~~~~~~~~~~~~~~~end

// ~~~~~~~~~~~~~~USER VIEWS~~~~~~~~~~~~~~~~~~~~~~~~~~start

void getUserDetails(User *user)
{

  // clearTerminal();
  char username[USERNAME_LEN];
  char userpaswd[USERPASSWORD_LEN];

  println("Please Enter the following Details");
  // GET Username
  println("Enter your username");
  displayPrompt();
  char *notErr = fgets(username, USERNAME_LEN, stdin);
  if (notErr)
    removeNewLine(username);
  strncpy(user->username, username, USERNAME_LEN - 1);

  // GET Userpasswd
  println("Enter your password");
  displayPrompt();
  notErr = fgets(userpaswd, USERPASSWORD_LEN, stdin);
  if (notErr)
    removeNewLine(userpaswd);
  strncpy(user->userpassword, userpaswd, USERPASSWORD_LEN - 1);
  // clearTerminal();
}

int loginSignUpMenu()
{

  // clearTerminal();
  println("To Enter our app");
  println("Please Enter");
  println("1. Login");
  println("2. Signup");

  displayPrompt();
  int option;

  scanf("%d", &option);
  flushStdin();
  // clearTerminal();

  return option;
}

// ~~~~~~~~~~~~~~USER VIEWS~~~~~~~~~~~~~~~~~~~~~~~~~~end

// ~~~~~~~~~~~~~~TODO VIEWS~~~~~~~~~~~~~~~~~~~~~~~~~~end

void showHelp()
{

  println("TODO options");

  println("h/help - help");
  // OUTSIDE FILE OPERATION
  println("n/new - Create new file");
  println("o/open - Open file // Only 1 at a time");
  println("d/delete - Delete file");
  println("c/close - Close file");
  println("ls/list - List files");

  // INSIDE FILE OPERATION
  println("a/append - Add Todo {OPEN}");
  println("s/save - Save Todo {OPEN}");
  println("r/remove - Remove todo from file {OPEN}");
  println("e/edit - Edit todo from file {OPEN}");
  println("l/look 1 {priority = 1} [1,2,3,4] [Critical, High, Medium, Later]");
}

int identifyCommand(const char *command)
{
  if (strcmp(command, "h") == 0 || strcmp(command, "help") == 0)
  {
    return HELP;
  }
  else if (strcmp(command, "n") == 0 || strcmp(command, "new") == 0)
  {
    return NEW;
  }
  else if (strcmp(command, "o") == 0 || strcmp(command, "open") == 0)
  {
    return OPEN;
  }
  else if (strcmp(command, "d") == 0 || strcmp(command, "delete") == 0)
  {
    return DELETE;
  }
  else if (strcmp(command, "cls") == 0 || strcmp(command, "clear") == 0)
  {
    return CLEAR;
  }
  else if (strcmp(command, "c") == 0 || strcmp(command, "close") == 0)
  {
    return CLOSE;
  }
  else if (strcmp(command, "ls") == 0 || strcmp(command, "list") == 0)
  {
    return LIST;
  }
  else if (strcmp(command, "a") == 0 || strcmp(command, "append") == 0)
  {
    return APPEND;
  }
  else if (strcmp(command, "s") == 0 || strcmp(command, "save") == 0)
  {
    return SAVE;
  }
  else if (strcmp(command, "r") == 0 || strcmp(command, "remove") == 0)
  {
    return REMOVE;
  }
  else if (strcmp(command, "e") == 0 || strcmp(command, "edit") == 0)
  {
    return EDIT;
  }
  else if (strcmp(command, "l") == 0 || strcmp(command, "look") == 0)
  {
    return LOOK;
  }
  else if (strcmp(command, "exit") == 0)
  {
    return EXIT;
  }
  else
  {
    return UNKNOWN;
  }
}
const char *getCommandString(int command)
{
  switch (command)
  {
  case HELP:
    return "help";
  case NEW:
    return "new";
  case OPEN:
    return "open";
  case DELETE:
    return "delete";
  case CLEAR:
    return "clear";
  case CLOSE:
    return "close";
  case LIST:
    return "list";
  case APPEND:
    return "append";
  case SAVE:
    return "save";
  case REMOVE:
    return "remove";
  case EDIT:
    return "edit";
  case EXIT:
    return "exit";
  case LOOK:
    return "look";
  default:
    return "unknown";
  }
}

void greetUser(const char *userName)
{
  println("Hello %s", userName);
  println("Welcome! to our todo app");
  println("<usage>: Enter h for help or to use todo functions");
}

int giveOptions()
{

  const int maxOptionLen = 10;
  char options[maxOptionLen];
  displayPrompt();
  fgets(options, maxOptionLen, stdin);
  removeNewLine(options);

  return identifyCommand(options);

  return 0;
}

void askFileName(char *filename)
{
  println("Enter file name: ");
  displayPrompt();
  fgets(filename, FILENAME_MAX, stdin);
  removeNewLine(filename);
}

void showError(int failedCmd, char *err)
{
  const char *cmd = getCommandString(failedCmd);
  // println("Unable to %s on file %s", cmd, filename);
  println("Could not intepret %s. [ %s ]", cmd, err);
  println("Type h/help for help");
}

void displayList(char **files, int writeFiles)
{

  println("Total files %d", writeFiles);
  println("----------------------------");
  for (size_t i = 0; i < writeFiles; i++)
  {
    println(" %s", files[i]);
  }
}

char *stringifyTime(time_t raw_time, char *buffer, size_t bufferSize)
{
  struct tm *time_info;

  time_info = localtime(&raw_time);

  char am_pm[3];
  int hour = time_info->tm_hour;

  if (hour >= 12)
  {
    strcpy(am_pm, "PM");
    if (hour > 12)
      hour -= 12;
  }
  else
  {
    strcpy(am_pm, "AM");
    if (hour == 0)
      hour = 12;
  }

  snprintf(buffer, bufferSize, "%04d-%02d-%02d %02d:%02d %s",
           time_info->tm_year + 1900,
           time_info->tm_mon + 1,
           time_info->tm_mday,
           hour,
           time_info->tm_min,
           am_pm);
  return buffer;
}

void askTodo(Todo *todo)
{
  println("Enter task Todo [NOTE: not long then %d character]", TODO_DESCRIPTION_SIZE);

  fgets(todo->description, TODO_DESCRIPTION_SIZE, stdin);
  removeNewLine(todo->description);

  while (1)
  {
    println("Select your priority");
    println("1) CRITICAL");
    println("2) HIGH");
    println("3) MEDIUM");
    println("4) LATER");

    int p;

    scanf("%d", &p);
    flushStdin();
    todo->priority = (Priority)p;

    if (
        !(todo->priority == CRITICAL ||
          todo->priority == HIGH ||
          todo->priority == MEDIUM ||
          todo->priority == LATER))
    {
      println("Please Input valid Priority");
      continue;
    }
    break;
  }
}
char *getPriorityString(Priority p) {
  switch (p)
  {
  case CRITICAL:
    return "critical";
  case HIGH:
    return "high";
  case MEDIUM:
    return "medium";
  case LATER:
    return "later";

  default:
    return "unknown";
  }
}
void displayTodos(TodoArray *todos, char *workingFile)
{
  // Print the table header
  println("%-11s | %-75s | %-8s | %s", "ID", "Description", "Priority", "Creation Time");

  for (size_t i = 0; i < 100; i++)
  {
    printf("-");
  }
  println("");

  char buffer[20];
  for (size_t i = 0; i < todos->size; i++)
  {
    Todo todo = todos->todos[i];
    stringifyTime(todo.timestamp, buffer, sizeof(buffer));

    println("%-11d | %-75s | %-8s | %s",
            todo.id,
            todo.description,
            getPriorityString(todo.priority),
            buffer);
  }
}

int askId()
{
  int i;
  println("Please provide the id for todo to be edited");
  scanf("%d", &i);
  flushStdin();
  return i;
}

// ~~~~~~~~~~~~~~TODO VIEWS~~~~~~~~~~~~~~~~~~~~~~~~~~end

// ~~~~~~~~~~~~~~USER CONTROLLER~~~~~~~~~~~~~~~~~~~~~~~~~~end

int userLogin(User *storedUser)
{
  User user;
  getUserDetails(&user);

  return login(&user, storedUser);
}

int userSignUp(User *storedUser)
{
  User user;
  getUserDetails(&user);
  int signUpStatus = signUp(&user, storedUser);
  if (signUpStatus == 1)
  {
    println("User name already taken");
    return 1;
  }
  if (signUpStatus != 0)
  {
    println("Something went wrong");
    return -1;
  }
  return 0;
}

int loginSignUp(User *user)
{
  int option = loginSignUpMenu();

  switch (option)
  {
  case 1:
    return userLogin(user);
    break;
  case 2:
    return userSignUp(user);
    break;

  default:
  {
    println("Please provide valid input");
    return 1;
  }
  break;
  }
  return 0;
}


// ~~~~~~~~~~~~~~USER CONTROLLER~~~~~~~~~~~~~~~~~~~~~~~~~~end


// ~~~~~~~~~~~~~~TODO CONTROLLER~~~~~~~~~~~~~~~~~~~~~~~~~~end
/*
 * TODO options:
 * 
 * h/help - help
 * n/new - create new file
 * o/open - open file
 * d/delete - delete file
 * c/close - close file
 * 
 * a/add/append - add/append to do file {OPEN}
 * r/remove - remove todo from file {OPEN}
 * e/edit - edit todo from file {OPEN}
 * 
 * show
 * show 1 {priority} [1,2,3,4] [Critical, High, Medium, Later]
 * 
 */

int newCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runNew(filename);
  if (err)
  {
    showError(NEW, filename);
    return 1;
  }
  return 0;
}

int openCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runOpen(filename);
  if (err)
  {
    showError(OPEN, filename);
    return 1;
  }
  return 0;
}

int deleteCmdHandler()
{
  char filename[FILENAME_MAX];
  askFileName(filename);
  int err = runDelete(filename);
  if (err)
  {
    showError(DELETE, filename);
    return 1;
  }
  return 0;
}

int clearCmdHandler()
{
  clearTerminal();
  return 0;
}

int closeCmdHandler()
{
  char filename[FILENAME_MAX];
  int err = runClose(filename);
  if (err)
  {
    showError(CLOSE, "[file not opened]");
    return 1;
  }
  return 0;

  return 0;
}

int listCmdHandler()
{
  // Allocated space for file
  int allocFiles = 10;
  int wroteFileNum = 0;
  char **files = (char **)malloc(sizeof(char *) * allocFiles);
  int err = runList(files, &allocFiles, &wroteFileNum);
  if (err)

  {
    showError(LIST, "[Something went wrong]");
    return 1;
  }
  displayList(files, wroteFileNum);

  // clean memory
  for (int i = 0; i < wroteFileNum; i++)
  {
    free(files[i]);
  }
  free(files);

  return 0;
}

int appendCmdHandler()
{
  Todo task;
  askTodo(&task);
  appendTodo(task);
  return 0;
}

int saveCmdHandler()
{
  return runSave();
}

int lookCmdHanlder()
{
  char workingFile[FILENAME_MAX];
  TodoArray *todoList = runLook(workingFile);

  displayTodos(todoList, workingFile);

  return 0;
}

int editCmdHandler()
{
  if (!checkFileOpen())
  {
    showError(EDIT, "File not open.");
    return 1;
  }
  lookCmdHanlder();
  int id = askId();
  if (checkTodoIdExist(id))
  {
    showError(EDIT, "Id does not exist");
    return 1;
  }
  Todo todo;
  askTodo(&todo);
  todo.id = id;
  runEdit(&todo);
  return 0;
}

int removeCmdHandler()
{
  lookCmdHanlder();
  int id = askId();
  runRemove(id);
  return 0;
}

int exitCmdHandler()
{
  if (runExit() == 1)
  {
    showError(SAVE, "Changes were not saved.");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

int runCommand(int option)
{
  switch (option)
  {
  case HELP:
    showHelp();
    break;
  case NEW:
    return newCmdHandler();
  case OPEN:
    return openCmdHandler();
    break;
  case DELETE:
    return deleteCmdHandler();
  case CLEAR:
    return clearCmdHandler();
  case CLOSE:
    return closeCmdHandler();
  case LIST:
    return listCmdHandler();
    break;
  case APPEND:
    return appendCmdHandler();
    break;
  case SAVE:
    return saveCmdHandler();
    break;
  case LOOK:
    return lookCmdHanlder();
    break;
  case EDIT:
    return editCmdHandler();
    break;
  case REMOVE:
    return removeCmdHandler();
    break;
  case EXIT:
    return exitCmdHandler();
    break;

  default:
    showError(UNKNOWN, "Unknown command");
    return 0;
    break;
  }
  return 0;
}

int initTodo(User *user)
{
  clearTerminal();
  greetUser(user->username);
  prepTodoDir(user);
  return 0;
}

int runApp()
{
  while (1)
  {
    int option = giveOptions();
    runCommand(option);
  }
  return 0;
}

// ~~~~~~~~~~~~~~TODO CONTROLLER~~~~~~~~~~~~~~~~~~~~~~~~~~end
