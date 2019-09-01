/* ouch.c

   Catch the SIGINT signal, generated by typing control-C (^C).

   Note that although we use signal() to establish the signal handler in this
   program, the use of sigaction() is always preferable for this task.
*/
#include <signal.h>
#include "tlpi_hdr.h"

static void
sigHandler(int sig)
{
    printf("Ouch!\n"); /* UNSAFE (see Section 21.1.2) */
}

int main(int argc, char *argv[])
{
    int j;

    /* Establish handler for SIGINT. Here we use the simpler signal()
       API to establish a signal handler, but for the reasons described in
       Section 22.7 of TLPI, sigaction() is the (strongly) preferred API
       for this task. */

    if (signal(SIGINT, sigHandler) == SIG_ERR)
        errExit("signal");

    /* Loop continuously waiting for signals to be delivered */

    for (j = 0;; j++)
    {
        printf("%d\n", j);
        sleep(3); /* Loop slowly... */
    }
}