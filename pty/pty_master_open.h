/* pty_master_open.h

   Header file for pty_open.c (and pty_master_open_bsd.c).
*/
#ifndef PTY_MASTER_OPEN_H
#define PTY_MASTER_OPEN_H

#include <sys/types.h>

int ptyMasterOpen(char *slaveName, size_t snLen);

#endif