/* svsem_info.c

   Demonstrate the use of the SEM_INFO operation to retrieve a 'seminfo'
   structure containing the current usage of System V semaphore resources.

   This program is Linux-specific.
*/
#define _GNU_SOURCE
#include <sys/sem.h>
#include "semun.h" /* Definition of semun union */
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct seminfo info;
    union semun arg;

    arg.__buf = &info;

    int s = semctl(0, 0, SEM_INFO, arg);
    if (s == -1)
        errExit("semctl");

    printf("Maximum ID index = %d\n", s);
    printf("sets in_use      = %ld\n", (long)info.semusz);
    printf("used_sems        = %ld\n", (long)info.semaem);
    exit(EXIT_SUCCESS);
}