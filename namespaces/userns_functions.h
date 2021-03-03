/* userns_functions.h

   Some useful auxiliary functions when working with user namespaces.
*/
#ifndef USERNS_FUNCTIONS_H /* Prevent double inclusion */
#define USERNS_FUNCTIONS_H

#include <unistd.h>

void display_creds_and_caps(char *str);

int update_map(char *mapping, char *map_file);

#endif