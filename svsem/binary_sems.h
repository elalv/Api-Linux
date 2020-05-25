/* binary_sems.h

   Header file for binary_sems.c.
*/
#ifndef BINARY_SEMS_H /* Prevent accidental double inclusion */
#define BINARY_SEMS_H

#include "tlpi_hdr.h"

int initSemAvailable(int semId, int semNum);

int initSemInUse(int semId, int semNum);

#endif