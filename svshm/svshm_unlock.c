/* svshm_unlock.c

   Unlock the System V shared memory segments identified by the
   command-line arguments.

   See also svshm_lock.c.
*/
#include <sys/types.h>
#include <sys/shm.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    for (int j = 1; j < argc; j++)
        if (shmctl(getInt(argv[j], 0, "shmid"), SHM_UNLOCK, NULL) == -1)
            errExit("shmctl");

    exit(EXIT_SUCCESS);
}