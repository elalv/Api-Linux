/* bad_exclusive_open.c

   The following code shows why we need the open() O_EXCL flag.

   This program to tries ensure that it is the one that creates the file
   named in its command-line argument. It does this by trying to open()
   the filename once without the O_CREAT flag (if this open() succeeds
   then the program know it is not the creator of the file), and if
   that open() fails, it calls open() a second time, with the O_CREAT flag.

   If the first open() fails, the program assumes that it is the creator
   of the file. However this may not be true: some other process may have
   created the file between the two calls to open().

      Example:

        $ ./bad_exclusive_open tfile sleep &                # Seek to offset 100,000, write “abc”
        [PID 3317] File "tfile" doesn't exist yet
        [1] 3317
        $ ./bad_exclusive_open tfile                        # Seek to offset 10,000, read 5 bytes from hole
        [PID 3318] File "tfile" doesn't exist yet
        [PID 3318] Created file "tfile" exclusively
        $ [PID 3317] Done sleeping
        [PID 3317] Created file "tfile" exclusively         # Not true
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_WRONLY); /* Open 1: check if file exists */
    if (fd != -1)
    { /* Open succeeded */
        printf("[PID %ld] File \"%s\" already exists\n",
               (long)getpid(), argv[1]);
        close(fd);
    }
    else
    {
        if (errno != ENOENT)
        { /* Failed for unexpected reason */
            errExit("open");
        }
        else
        {
            printf("[PID %ld] File \"%s\" doesn't exist yet\n",
                   (long)getpid(), argv[1]);
            if (argc > 2)
            {             /* Delay between check and create */
                sleep(5); /* Suspend execution for 5 seconds */
                printf("[PID %ld] Done sleeping\n", (long)getpid());
            }
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1)
                errExit("open");

            printf("[PID %ld] Created file \"%s\" exclusively\n",
                   (long)getpid(), argv[1]); /* MAY NOT BE TRUE! */
        }
    }

    exit(EXIT_SUCCESS);
}