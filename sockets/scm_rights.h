/* scm_rights.h

   Header file used by scm_rights_send.c and scm_rights_recv.c.
*/
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdbool.h>
#include "unix_sockets.h" /* Declares our unix*() socket functions */
#include "tlpi_hdr.h"

#define SOCK_PATH "scm_rights"