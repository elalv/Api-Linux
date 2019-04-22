/* ugid_functions.c

   Implements a set of functions that convert user/group names to user/group IDs
   and vice versa.
*/
#include <pwd.h>
#include "ugid_functions.h" /* Declares functions defined here */

char * /* Return name corresponding to 'uid', or NULL on error */
userNameFromId(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}