/* us_xfr_v2_sv.c

   An example UNIX stream socket server. Accepts incoming connections and
   copies data sent from clients to stdout. This program is similar to
   us_xfr_sv.c, except that it uses the functions in unix_sockets.c to
   simplify working with UNIX domain sockets.

   See also us_xfr_v2_cl.c.
*/
#include "us_xfr_v2.h"

int main(int argc, char *argv[])
{
    int sfd = unixBind(SV_SOCK_PATH, SOCK_STREAM);
    if (sfd == -1)
        errExit("unixBind");

    if (listen(sfd, 5) == -1)
        errExit("listen");

    for (;;)
    { /* Handle client connections iteratively */
        int cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
            errExit("accept");

        /* Transfer data from connected socket to stdout until EOF */

        ssize_t numRead;
        char buf[BUF_SIZE];

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                fatal("partial/failed write");

        if (numRead == -1)
            errExit("read");

        if (close(cfd) == -1)
            errMsg("close");
    }
}