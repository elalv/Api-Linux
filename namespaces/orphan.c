/* orphan.c

   Demonstrate that a child becomes orphaned (and is adopted by init(1),
   whose PID is 1) when its parent exits.

   See https://lwn.net/Articles/532748/

   Changes to allow for the fact that on systems with a modern
   init(1) (e.g., systemd), an orphaned child may be adopted
   by a "child subreaper" process whose PID is not 1.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t ppidOrig = getpid();

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid != 0)
    { /* Parent */
        printf("Parent (PID=%ld) created child with PID %ld\n",
               (long)getpid(), (long)pid);
        printf("Parent (PID=%ld; PPID=%ld) terminating\n",
               (long)getpid(), (long)getppid());
        exit(EXIT_SUCCESS);
    }

    /* Child falls through to here */

    do
    {
        usleep(100000);
    } while (getppid() == ppidOrig); /* Am I an orphan yet? */

    printf("\nChild  (PID=%ld) now an orphan (parent PID=%ld)\n",
           (long)getpid(), (long)getppid());

    sleep(1);

    printf("Child  (PID=%ld) terminating\n", (long)getpid());
    exit(EXIT_SUCCESS);
}