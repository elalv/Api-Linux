/* sched_view.c

   Display the scheduling policy and priority for the processes whose PID
   are provided on command line.

   See also sched_set.c.
*/
#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    for (j = 1; j < argc; j++)
    {
        pol = sched_getscheduler(getLong(argv[j], 0, "pid"));
        if (pol == -1)
            errExit("sched_getscheduler");

        if (sched_getparam(getLong(argv[j], 0, "pid"), &sp) == -1)
            errExit("sched_getparam");

        printf("%s: %-5s ", argv[j],
               (pol == SCHED_OTHER) ? "OTHER" : (pol == SCHED_RR) ? "RR"
                                            : (pol == SCHED_FIFO) ? "FIFO"
                                            :
#ifdef SCHED_BATCH /* Linux-specific */
                                            (pol == SCHED_BATCH) ? "BATCH"
                                            :
#endif
#ifdef SCHED_IDLE /* Linux-specific */
                                            (pol == SCHED_IDLE) ? "IDLE"
                                                                :
#endif
                                                                "???");
        printf("%2d\n", sp.sched_priority);
    }

    exit(EXIT_SUCCESS);
}
