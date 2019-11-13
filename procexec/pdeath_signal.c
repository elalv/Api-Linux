/* pdeath_signal.c

   On Linux, a child process can ask to get a signal when its parent dies.
   But there are various pieces of strangeness if the parent is multithreaded
   or if there are ancestor subreaper processes.

*/
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Structure defining parameters used by each thread */

struct threadParam
{
    int sleepTime;
    char **argv;
    int threadNum;
    bool createNextChild;
};

static int childPreSleep, childPostSleep;

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

/* Child process's handler for "parent death" signal */

static void
handler(int sig, siginfo_t *si, void *ucontext)
{
    static int cnt = 0;

    /* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(); see TLPI Section 21.1.2) */

    if (cnt == 0)
        printf("\n");

    cnt++;
    printf("*********** Child (%ld) got signal [%d]; "
           "si_pid = %d; si_uid = %d\n",
           (long)getpid(), cnt, si->si_pid, si->si_uid);
    printf("            Parent PID is now %ld\n", (long)getppid());
}

/* Create the child process. This step is performed after the chain
   of ancestors has been created. */

static void
createChild(void)
{
    struct sigaction sa;

    printf("TID %ld (PID %ld) about to call fork()\n",
           syscall(SYS_gettid), (long)getpid());

    switch (fork())
    {
    case -1:
        errExit("fork");

    case 0:
        printf("Final child %ld created; parent %ld\n",
               (long)getpid(), (long)getppid());

        /* Establish handler for "parent death" signal */

        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = handler;
        if (sigaction(SIGUSR1, &sa, NULL) == -1)
            errExit("sigaction");

        /* Perform a pre-sleep before requesting "parent death" signal;
           this allows us to see what happens if the parent terminates
           before the child requests the signal. */

        if (childPreSleep > 0)
        {
            printf("\tChild (PID %ld) about to sleep %d sec before setting "
                   "PR_SET_PDEATHSIG\n",
                   (long)getpid(), childPreSleep);
            sleep(childPreSleep);
        }

        /* Request death signal (SIGUSR1) when parent terminates */

        printf("\tChild about to set PR_SET_PDEATHSIG\n");
        if (prctl(PR_SET_PDEATHSIG, SIGUSR1) == -1)
            errExit("prctl");

        /* Now sleep, while ancestors terminate. Perform the sleep in
           1-second steps to allow for the fact that signal handler
           invocations will interrupt sleep() (and thus terminate
           a single long sleep of 'childPostSleep' seconds). */

        printf("\tChild (PID %ld) about to sleep %d seconds\n",
               (long)getpid(), childPostSleep);

        for (int j = 0; j < childPostSleep; j++)
            sleep(1);

        printf("Child about to exit\n");
        exit(EXIT_SUCCESS);

    default:
        return;
    }
}

/* Perform per-thread steps */

static void
performPerThreadSteps(struct threadParam *tparam)
{
    pid_t tid = syscall(SYS_gettid);

    usleep(tparam->threadNum * 1000);

    /* Is this the thread that is designated to create the next ancestor or
       child process? */

    if (tparam->createNextChild)
    {
        if (*(tparam->argv) != NULL)
            createAncestor(tparam->argv);
        else /* Last ancestor, so now we create the child */
            createChild();
    }

    /* Sleep for the specified interval, and then terminate */

    printf("\tTID %ld (PID: %ld) about to sleep %d sec\n",
           (long)tid, (long)getpid(), tparam->sleepTime);

    sleep(tparam->sleepTime);

    printf("TID %ld (PID: %ld) terminating (after %d sec sleep)\n",
           (long)tid, (long)getpid(), tparam->sleepTime);
}

/* Thread start function executed by each (noninitial) thread */

static void *
threadStartFunc(void *arg)
{
    struct threadParam *tparam = arg;

    performPerThreadSteps(tparam);

    free(tparam);
    pthread_exit(NULL);
}

/* Create a set of threads in the calling process, as specified in
   'ancestorArg'. The calling thread (which is the initial thread
   in the process) terminates in this function. */

static void
createThreads(char *ancestorArg, char **argv)
{
    struct threadParam *tparam;
    struct threadParam tparamInit;
    bool nextParentMarked = false;

    /* Split the argument into colon-separated tokens, and create an
       additional thread for each token from the second onward. (The first
       token will be handled by the initial thread in this process, which,
       by definition, already exists.) */

    for (int tnum = 0;; tnum++)
    {
        char *tokenp = strtok((tnum == 0) ? ancestorArg : NULL, ":");

        if (tokenp == NULL)
            break;

        /* Allocate and populate the structure that will be employed
           by the thread associated with this token. */

        tparam = malloc(sizeof(struct threadParam));
        if (tparam == NULL)
            errExit("malloc");

        /* If this token started with '+', remember that this thread should
           be the one to call fork() to create the next descendant */

        tparam->createNextChild = *tokenp == '+';

        if (tparam->createNextChild)
        {

            /* There should be at most one '+' in 'ancestorArg' */

            if (nextParentMarked)
            {
                fprintf(stderr, "Found '+' twice in one argument!\n");
                exit(EXIT_FAILURE);
            }

            tokenp++; /* Advance past '+' */
            nextParentMarked = true;
        }

        tparam->sleepTime = atoi(tokenp);
        tparam->argv = argv + 1;
        tparam->threadNum = tnum;

        if (tnum == 0)
        {

            /* No need to create a thread for the first token, which is
               handled in the initial thread */

            tparamInit = *tparam;
            free(tparam);
        }
        else
        {

            /* Create a new thread for this token; 'tparam' will be freed
               in threadStartFunc(). */

            pthread_t thr;

            int s = pthread_create(&thr, NULL, threadStartFunc, tparam);
            if (s != 0)
            {
                fprintf(stderr, "pthread_create() failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* The initial thread comes here... */

    /* If no token was marked with '+', then by default the initial thread
       will be the one to create the next descendant */

    if (!nextParentMarked)
        tparamInit.createNextChild = true;

    performPerThreadSteps(&tparamInit);

    pthread_exit(NULL);
}