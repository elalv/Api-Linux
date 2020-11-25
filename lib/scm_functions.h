/* scm_functions.h

   Functions to exchange ancillary data over a UNIX domain socket.
*/
#ifndef SCM_FUNCTIONS_H
#define SCM_FUNCTIONS_H /* Prevent accidental double inclusion */

#include <sys/socket.h>
#include <sys/un.h>

int sendfd(int sockfd, int fd);

int recvfd(int sockfd);

#endif