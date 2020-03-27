/* show_secbits.c
 */
#include <sys/prctl.h>
#include "cap_functions.h"
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int secbits = prctl(PR_GET_SECUREBITS, 0, 0, 0, 0);
    if (secbits == -1)
        errExit("prctl");

    printf("secbits = 0x%x => ", secbits);

    printSecbits(secbits, argc == 1, stdout);
    printf("\n");

    exit(EXIT_SUCCESS);
}