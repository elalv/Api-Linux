/* make_orphans.c

   Demonstrate that when a child process becomes orphaned because its parent
   terminates, the child is reparented, and hence its parent PID changes.

   Classically, the orphaned child is reparented to PID 1, but on a system
   that runs Systemd as the init process, the child may instead be reparented
   to a "child subreaper" process that has a PID other than 1.

   See https://lwn.net/Articles/532748/. This program is a more flexible
   version of the orhan.c program described in that article: it permits the
   creation of multiple orphans that sleep for a specified period before
   terminating.

   Usage: make_orphans [num-orphans [orphan-sleep-secs]]
                        Default: 1     Default: 120
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int numOrphans = (argc > 1) ? atoi(argv[1]) : 1;
    int sleepTime = (argc > 2) ? atoi(argv[2]) : 120;

    pid_t ppidOrig = getpid();

    for (int j = 0; j < numOrphans; j++)
    {
        switch (fork())
        {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:                           /* Child */
            while (getppid() == ppidOrig) /* Am I an orphan yet? */
                usleep(100000);

            printf("Child  (PID=%ld) now an orphan (parent PID=%ld)\n",
                   (long)getpid(), (long)getppid());

            sleep(sleepTime);

            printf("Child  (PID=%ld) terminating\n", (long)getpid());
            exit(EXIT_SUCCESS);

        default: /* Parent */
            break;
        }
    }

    /* Parent falls through to here. */

    printf("Parent (PID=%ld) terminating\n", (long)getpid());
    exit(EXIT_SUCCESS);
}