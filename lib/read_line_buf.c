/* read_line_buf.c

   Implementation of readLineBuf(), a version of readLine() that is more
   efficient because it reads blocks of characters at a time.
*/
#include <unistd.h>
#include <errno.h>
#include "read_line_buf.h"

void /* Initialize a ReadLineBuf structure */
readLineBufInit(int fd, struct ReadLineBuf *rlbuf)
{
    rlbuf->fd = fd;
    rlbuf->len = 0;
    rlbuf->next = 0;
}

/* Return a line of input from the buffer 'rlbuf', placing the characters in
   'buffer'. The 'n' argument specifies the size of 'buffer'. If the line of
   input is larger than this, then the excess characters are discarded. */

ssize_t
readLineBuf(struct ReadLineBuf *rlbuf, char *buffer, size_t n)
{
    if (n <= 0 || buffer == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    size_t cnt = 0;

    /* Fetch characters from rlbuf->buf, up to the next new line. */

    for (;;)
    {

        /* If there are insufficient characters in 'tlbuf', then obtain
           further input from the associated file descriptor. */

        if (rlbuf->next >= rlbuf->len)
        {
            rlbuf->len = read(rlbuf->fd, rlbuf->buf, RL_MAX_BUF);
            if (rlbuf->len == -1)
                return -1;

            if (rlbuf->len == 0) /* End of file */
                break;

            rlbuf->next = 0;
        }

        char c = rlbuf->buf[rlbuf->next];
        rlbuf->next++;

        if (cnt < n)
            buffer[cnt++] = c;

        if (c == '\n')
            break;
    }

    return cnt;
}