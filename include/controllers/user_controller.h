#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include "models/user.h"
#include "views/user_view.h"
#include "utils/utils.h"

/*
 * Function: loginSignUp
 * ---------------------
 * Provides a menu for login or signup option selection and performs the selected action.
 *
 * Parameters:
 *  - user: A pointer to a User structure where the logged-in or signed-up user's information will be copied.
 *
 * Returns:
 *  - Result of the login or signup operation:
 *    - Result of userLogin function if option 1 (login) is chosen.
 *    - Result of userSignUp function if option 2 (signup) is chosen.
 *    - 1 if invalid input is provided.
 */
int loginSignUp(User *user);

#endif