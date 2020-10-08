/* is_seqnum_v2_cl.c

   A simple Internet stream socket client. This server obtains a sequence
   number from the server.

   The program is the same as is_seqnum_cl.c, except that it uses the
   functions in our inet_sockets.c library to simplify the creation of a
   socket that connects to the server's socket.

   See also is_seqnum_v2_sv.c.
*/
#include "is_seqnum_v2.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s server-host [sequence-len]\n", argv[0]);

    int cfd = inetConnect(argv[1], PORT_NUM_STR, SOCK_STREAM);
    if (cfd == -1)
        fatal("inetConnect() failed");

    char *reqLenStr = (argc > 2) ? argv[2] : "1";
    if (write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
        fatal("Partial/failed write (reqLenStr)");
    if (write(cfd, "\n", 1) != 1)
        fatal("Partial/failed write (newline)");

    char seqNumStr[INT_LEN]; /* Start of granted sequence */
    ssize_t numRead = readLine(cfd, seqNumStr, INT_LEN);
    if (numRead == -1)
        errExit("readLine");
    if (numRead == 0)
        fatal("Unexpected EOF from server");

    printf("Sequence number: %s", seqNumStr); /* Includes '\n' */

    exit(EXIT_SUCCESS); /* Closes 'cfd' */
}