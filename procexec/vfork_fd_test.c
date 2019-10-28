/* vfork_fd_test.c

   Demonstrate that a vfork()ed child has a separate set of file descriptors
   from its parent.
*/
#define _BSD_SOURCE /* To get vfork() declaration from <unistd.h> \
                       in case _XOPEN_SOURCE >= 700 */
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    switch (vfork())
    {
    case -1:
        errExit("vfork");

    case 0:
        if (close(STDOUT_FILENO) == -1)
            errMsg("close - child");
        _exit(EXIT_SUCCESS);

    default:
        break;
    }

    /* Now parent closes STDOUT_FILENO twice: only the second close
       should fail, indicating that the close(STDOUT_FILENO) by the
       child did not affect the parent. */

    if (close(STDOUT_FILENO) == -1)
        errMsg("close");
    if (close(STDOUT_FILENO) == -1)
        errMsg("close");

    exit(EXIT_SUCCESS);
}