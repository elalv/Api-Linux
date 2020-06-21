/* svshm_attach.c

   Experiment with the use of shmat() to attach previously created
   System V shared memory segments.

   Usage: svshm_attach [shmid:addr[rR]]...

        r = attach with SHM_RND flag
        R = attach with SHM_RDONLY flag
*/
#include <sys/types.h>
#include <sys/shm.h>
#include "tlpi_hdr.h"

static void
usageError(char *progName)
{
    fprintf(stderr, "Usage: %s [shmid:address[rR]]...\n", progName);
    fprintf(stderr, "            r=SHM_RND; R=SHM_RDONLY\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    printf("SHMLBA = %ld (%#lx), PID = %ld\n",
           (long)SHMLBA, (unsigned long)SHMLBA, (long)getpid());

    for (int j = 1; j < argc; j++)
    {
        char *p;
        int shmid = strtol(argv[j], &p, 0);
        if (*p != ':')
            usageError(argv[0]);

        void *addr = (void *)strtol(p + 1, NULL, 0);
        int flags = (strchr(p + 1, 'r') != NULL) ? SHM_RND : 0;
        if (strchr(p + 1, 'R') != NULL)
            flags |= SHM_RDONLY;

        char *retAddr = shmat(shmid, addr, flags);
        if (retAddr == (void *)-1)
            errExit("shmat: %s", argv[j]);

        printf("%d: %s ==> %p\n", j, argv[j], retAddr);
    }

    printf("Sleeping 5 seconds\n");
    sleep(5);

    exit(EXIT_SUCCESS);
}
