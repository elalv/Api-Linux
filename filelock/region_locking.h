/* region_locking.h

   Header file for region_locking.c.
*/
#ifndef REGION_LOCKING_H
#define REGION_LOCKING_H

#include <sys/types.h>

int lockRegion(int fd, int type, int whence, int start, int len);

#endif