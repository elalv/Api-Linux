/* svsem_rm.c

   Remove the System V semaphore sets whose IDs are specified by the
   command-line arguments.
*/
#include <sys/types.h>
#include <sys/sem.h>
#include "semun.h" /* Definition of semun union */
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [semid...]\n", argv[0]);

    union semun dummy;
    for (int j = 1; j < argc; j++)
        if (semctl(getInt(argv[j], 0, "semid"), 0, IPC_RMID, dummy) == -1)
            errExit("semctl %s", argv[j]);

    exit(EXIT_SUCCESS);
}