/* rdwrn.c

   Implementations of readn() and writen().
*/
#include <unistd.h>
#include <errno.h>
#include "rdwrn.h" /* Declares readn() and writen() */

/* Read 'n' bytes from 'fd' into 'buf', restarting after partial
   reads or interruptions by a signal handlers */

ssize_t
readn(int fd, void *buffer, size_t n)
{
    ssize_t numRead; /* # of bytes fetched by last read() */
    size_t totRead;  /* Total # of bytes read so far */
    char *buf;

    buf = buffer; /* No pointer arithmetic on "void *" */
    for (totRead = 0; totRead < n;)
    {
        numRead = read(fd, buf, n - totRead);

        if (numRead == 0)   /* EOF */
            return totRead; /* May be 0 if this is first read() */
        if (numRead == -1)
        {
            if (errno == EINTR)
                continue; /* Interrupted --> restart read() */
            else
                return -1; /* Some other error */
        }
        totRead += numRead;
        buf += numRead;
    }
    return totRead; /* Must be 'n' bytes if we get here */
}