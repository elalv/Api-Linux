/* t_getenv.c

   Demonstrate the use of getenv() to retrieve the value of an
   environment variable.
*/
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *val;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s environ-var\n", argv[0]);

    val = getenv(argv[1]);
    printf("%s\n", (val != NULL) ? val : "No such variable");

    exit(EXIT_SUCCESS);
}