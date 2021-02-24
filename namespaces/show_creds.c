/* show_creds.c

   Display process UIDs and GIDs.
*/
#define _GNU_SOURCE
#include <sys/capability.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define errExit(msg)        \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    for (;;)
    {
        cap_t caps = cap_get_proc();
        if (caps == NULL)
            errExit("cap_get_proc");

        printf("eUID = %ld;  eGID = %ld;  ",
               (long)geteuid(), (long)getegid());

        char *s = cap_to_text(caps, NULL);
        if (s == NULL)
            errExit("cap_to_text");
        printf("capabilities: %s\n", s);

        cap_free(caps);
        cap_free(s);

        if (argc == 1)
            break;

        sleep(5);
    }
    exit(EXIT_SUCCESS);
}