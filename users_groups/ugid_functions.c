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

uid_t /* Return UID corresponding to 'name', or -1 on error */
userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') /* On NULL or empty string */
        return -1;                     /* return an error */

    u = strtol(name, &endptr, 10); /* As a convenience to caller */
    if (*endptr == '\0')           /* allow a numeric string */
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}