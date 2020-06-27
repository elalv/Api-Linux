/* svshm_rm.c

   Remove the System V shared memory segments identified by the
   command-line arguments
*/
#include <sys/types.h>
#include <sys/shm.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [shmid...]\n", argv[0]);

    for (int j = 1; j < argc; j++)
        if (shmctl(getInt(argv[j], 0, "shmid"), IPC_RMID, NULL) == -1)
            errExit("shmctl %s", argv[j]);

    exit(EXIT_SUCCESS);
}