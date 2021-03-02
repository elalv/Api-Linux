/* userns_functions.c

   Some useful auxiliary functions when working with user namespaces.
*/
#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/capability.h>
#include "userns_functions.h"
#include "tlpi_hdr.h"

/* Display calling process's (effective) credentials and capabilities */

void display_creds_and_caps(char *str)
{
    printf("%seUID = %ld; eGID = %ld;  ", str,
           (long)geteuid(), (long)getegid());

    cap_t caps = cap_get_proc();
    if (caps == NULL)
        errExit("cap_get_proc");

    char *s = cap_to_text(caps, NULL);
    if (s == NULL)
        errExit("cap_to_text");
    printf("capabilities: %s\n", s);

    cap_free(caps);
    cap_free(s);
}