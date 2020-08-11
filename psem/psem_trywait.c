/* psem_trywait.c

   Try to decrease the value of a POSIX named semaphore using the
   nonblocking sem_trywait() function.

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#include <semaphore.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s sem-name\n", argv[0]);

    sem_t *sem = sem_open(argv[1], 0);
    if (sem == SEM_FAILED)
        errExit("sem_open");

    if (sem_trywait(sem) == -1)
        errExit("sem_trywait");

    exit(EXIT_SUCCESS);
}