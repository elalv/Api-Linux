/* hostname.c

   Display or change the system hostname.

   Usage: hostname [new-host-name]
*/
#define _BSD_SOURCE
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE (MAXHOSTNAMELEN + 1)

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (sethostname(argv[1], strlen(argv[1])) == -1)
        {
            perror("sethostname");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        char buf[BUF_SIZE];

        if (gethostname(buf, BUF_SIZE) == -1)
        {
            perror("gethostname");
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buf);
    }

    exit(EXIT_SUCCESS);
}