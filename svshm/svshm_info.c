/* svshm_info.c

   Demonstrate the use of the SHM_INFO operation to retrieve a 'shminfo'
   structure containing the current usage of System V shared memory resources.

   This program is Linux-specific.
*/
#define _GNU_SOURCE
#include <sys/shm.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    struct shm_info info;

    int s = shmctl(0, SHM_INFO, (struct shmid_ds *)&info);
    if (s == -1)
        errExit("shmctl");

    printf("Maximum ID index = %d\n", s);
    printf("shm_tot          = %ld\n", (long)info.shm_tot);
    printf("shm_rss          = %ld\n", (long)info.shm_rss);
    printf("shm_swp          = %ld\n", (long)info.shm_swp);
    printf("swap_attempts    = %ld\n", (long)info.swap_attempts);
    printf("swap_successes   = %ld\n", (long)info.swap_successes);

    exit(EXIT_SUCCESS);
}