/* pdeath_signal.c

   On Linux, a child process can ask to get a signal when its parent dies.
   But there are various pieces of strangeness if the parent is multithreaded
   or if there are ancestor subreaper processes.

*/
#include <stdio.h>
#include <stdlib.h>

static void
usageError(char *pname)
{
    fprintf(stderr, "Usage: %s child-pre-sleep "
                    "child-post-sleep [ancestor-arg...]\n",
            pname);
    fprintf(stderr,
            "Create a series of processes with the parental relationship:\n\
\n\
        ancestor1 -> ancestor2 -> ... ancestorN -> child\n\
\n\
in order to explore the behavior of the prctl() PR_SET_PDEATHSIG setting\n\
\n\
'child-pre-sleep' is the number of seconds that the child should sleep\n\
        before employing PR_SET_PDEATHSIG.\n\
'child-post-sleep' is the number of seconds that the child should sleep\n\
        after employing PR_SET_PDEATHSIG; in this time, we can observe what\n\
        happens when ancestors of this process terminate.\n\
'ancestor-arg...' defines attributes for an ancestor process.\n\
        One ancestor process is created for each of these arguments, with\n\
        the first of these being the most distant ancestor and the last\n\
        being the immediate ancestor of the 'child' process.\n\
        Each of these arguments consists a list of one or more\n\
        colon-separated integers. One thread is created for each integer\n\
        (except for the first integer, which is represented by the initial\n\
        thread), with each thread sleeping for the corresponding number of\n\
        seconds before terminating. At most one of the integers may be\n\
        preceded by a plus ('+') character; that thread will call fork()\n\
        to create the next ancestor process; if no integer is preceded with\n\
        a '+', then the initial thread will create the next ancestor.\n\
        If 'ancestor-arg' begins with an at sign ('@'), then the initial\n\
        thread marks the process as a subreaper before creating any\n\
        additional threads.\n");
    exit(EXIT_FAILURE);
}