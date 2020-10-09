/* unix_sockets.c

   A package of useful routines for UNIX domain sockets.
*/
#include "unix_sockets.h" /* Declares functions defined here */
#include "tlpi_hdr.h"

/* Build a UNIX domain socket address structure for 'path', returning
   it in 'addr'. Returns -1 on success, or 0 on error. */

int unixBuildAddress(const char *path, struct sockaddr_un *addr)
{
    if (addr == NULL || path == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    memset(addr, 0, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;
    if (strlen(path) < sizeof(addr->sun_path))
    {
        strncpy(addr->sun_path, path, sizeof(addr->sun_path) - 1);
        return 0;
    }
    else
    {
        errno = ENAMETOOLONG;
        return -1;
    }
}