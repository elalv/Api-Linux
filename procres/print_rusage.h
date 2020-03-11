/* print_rusage.h

   Header file for print_rusage.c.
*/
#ifndef PRINT_RUSAGE_H /* Prevent accidental double inclusion */
#define PRINT_RUSAGE_H

#include <sys/resource.h>

void printRusage(const char *leader, const struct rusage *ru);

#endif