/* read_line_buf.h

   Header file for read_line_buf.c (implementation of readLineBuf()).
*/
#ifndef READ_LINE_BUF_H /* Prevent accidental double inclusion */
#define READ_LINE_BUF_H

#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define RL_MAX_BUF 10

struct ReadLineBuf
{
    int fd;               /* File descriptor from which to read */
    char buf[RL_MAX_BUF]; /* Current buffer from file */
    int next;             /* Index of next unread character in 'buf' */
    ssize_t len;          /* Number of characters in 'buf' */
};

void readLineBufInit(int fd, struct ReadLineBuf *rlbuf);

ssize_t readLineBuf(struct ReadLineBuf *rlbuf, char *buffer, size_t n);

#endif