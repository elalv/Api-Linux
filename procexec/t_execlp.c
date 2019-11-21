/* t_execlp.c

   Demonstrate the use of execlp() to execute a program.
*/
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    /* Execute the program specified in argv[1] */

    execlp(argv[1], argv[1], "hello world", (char *)NULL);
    errExit("execlp"); /* If we get here, something went wrong */
}