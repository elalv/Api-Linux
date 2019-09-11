/* siginterrupt.c

   An implementation of the siginterrupt(3) library function.
*/
#include <stdio.h>
#include <signal.h>

int siginterrupt(int sig, int flag)
{
    int status;
    struct sigaction act;

    status = sigaction(sig, NULL, &act);
    if (status == -1)
        return -1;

    if (flag)
        act.sa_flags &= ~SA_RESTART;
    else
        act.sa_flags |= SA_RESTART;

    return sigaction(sig, &act, NULL);
}