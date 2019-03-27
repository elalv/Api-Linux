/* necho.c

   A simple version of echo(1): echo - command-line arguments.
*/
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j;

    for (j = 0; j < argc; j++)
        printf("argv[%d] = %s\n", j, argv[j]);

    exit(EXIT_SUCCESS);
}