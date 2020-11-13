/* scm_functions.c

   Functions to exchange ancillary data over UNIX domain sockets.
   These functions are simplistic, in that they ignore the "real" data
   content on the assumption that the sockets are being used only for
   the purposes of exchanging ancillary data. In many real-world
   applications, the application makes use of both the "real" data
   channel and the ancillary data, with some kind of protocol that
   determines how the "real" and ancillary data are used together.
*/
#include <string.h>
#include <errno.h>
#include "scm_functions.h"

/* Send the file descriptor 'fd' over the connected UNIX domain socket
   'sockfd'. Returns 0 on success, or -1 on error. */

int sendfd(int sockfd, int fd)
{
    /* Allocate a char array of suitable size to hold the ancillary data.
       However, since this buffer is in reality a 'struct cmsghdr', use a
       union to ensure that it is aligned as required for that structure.
       Alternatively, we could allocate the buffer using malloc(), which
       returns a buffer that satisfies the strictest alignment requirements
       of any type. However, if we employ that approach, we must ensure
       that we free() the buffer on all return paths from this function. */
    union
    {
        char buf[CMSG_SPACE(sizeof(int))];
        /* Space large enough to hold an 'int' */
        struct cmsghdr align;
    } controlMsg;

    /* The 'msg_name' field can be used to specify the address of the
       destination socket when sending a datagram. However, we do not
       need to use this field because we presume that 'sockfd' is a
       connected socket. */

    struct msghdr msgh;
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    /* On Linux, we must transmit at least one byte of real data in
       order to send ancillary data. We transmit an arbitrary integer
       whose value is ignored by recvfd(). */

    struct iovec iov;
    int data;

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(int);
    data = 12345;

    /* Set 'msghdr' fields that describe ancillary data */

    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);

    /* Set up ancillary data describing file descriptor to send */

    struct cmsghdr *cmsgp;
    cmsgp = CMSG_FIRSTHDR(&msgh);
    cmsgp->cmsg_level = SOL_SOCKET;
    cmsgp->cmsg_type = SCM_RIGHTS;
    cmsgp->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsgp), &fd, sizeof(int));

    /* Send real plus ancillary data */

    if (sendmsg(sockfd, &msgh, 0) == -1)
        return -1;

    return 0;
}