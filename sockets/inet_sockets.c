/* inet_sockets.c

   A package of useful routines for Internet domain sockets.
*/
#define _BSD_SOURCE /* To get NI_MAXHOST and NI_MAXSERV \
                       definitions from <netdb.h> */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "inet_sockets.h" /* Declares functions defined here */
#include "tlpi_hdr.h"

/* The following arguments are common to several of the routines
   below:

        'host':         NULL for loopback IP address, or
                        a host name or numeric IP address
        'service':      either a name or a port number
        'type':         either SOCK_STREAM or SOCK_DGRAM
*/

/* Create socket and connect it to the address specified by
  'host' + 'service'/'type'. Return socket descriptor on success,
  or -1 on error */

int inetConnect(const char *host, const char *service, int type)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_socktype = type;

    s = getaddrinfo(host, service, &hints, &result);
    if (s != 0)
    {
        errno = ENOSYS;
        return -1;
    }

    /* Walk through returned list until we find an address structure
       that can be used to successfully connect a socket */

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue; /* On error, try next address */

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */

        /* Connect failed: close this socket and try next address */

        close(sfd);
    }

    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}