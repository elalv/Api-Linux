/* is_seqnum_v2.h

   Header file for is_seqnum_v2_sv.c and is_seqnum_v2_cl.c.
*/
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "inet_sockets.h" /* Declares our socket functions */
#include "read_line.h"    /* Declaration of readLine() */
#include "tlpi_hdr.h"

#define PORT_NUM_STR "50000" /* Port number for server */

#define INT_LEN 30 /* Size of string able to hold largest \
                      integer (including terminating '\n') */