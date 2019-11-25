/* t_execl.c

   Demonstrate the use of execl() to execute printenv(1).
*/
#include <stdlib.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    printf("Initial value of USER: %s\n", getenv("USER"));
    if (putenv("USER=britta") != 0)
        errExit("putenv");

    /* exec printenv to display the USER and SHELL environment vars */

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *)NULL);
    errExit("execl"); /* If we get here, something went wrong */
}