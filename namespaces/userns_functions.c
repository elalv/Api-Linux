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

/* Update the mapping file 'map_file', with the value provided in
   'mapping', a string that defines a UID or GID mapping. A UID or
   GID mapping consists of one or more newline-delimited records
   of the form:

       ID_inside-ns    ID-outside-ns   length

   Requiring the user to supply a string that contains newlines is
   of course inconvenient for command-line use. Thus, we permit the
   use of commas to delimit records in this string, and replace them
   with newlines before writing the string to the file.

   Returns: 0 on success; -1 on error. */

int update_map(char *mapping, char *map_file)
{
    /* Replace commas in mapping string with newlines */

    size_t map_len = strlen(mapping);
    for (int j = 0; j < map_len; j++)
        if (mapping[j] == ',')
            mapping[j] = '\n';

    int fd = open(map_file, O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "ERROR: open %s: %s\n", map_file, strerror(errno));
        return -1;
    }

    int status = 0;

    if (write(fd, mapping, map_len) != map_len)
    {
        fprintf(stderr, "ERROR: writing to %s: %s\n",
                map_file, strerror(errno));
        status = -1;
    }

    close(fd);
    return status;
}