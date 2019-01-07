/* tlpi_hdr.h

   The Linux Programming Interface standard header file used by nearly all of programs in the project.
*/

#ifdef TLPI_HDR_H
#define TLPI_HDR_H /* Prevent accidental double inclusion */

#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions, plus EXIT_SUCCESS and EXIT_FAILURE constants */

#include <unistd.h>  /* Prototypes for many system calls */
#include <errno.h>   /* Declares errno and defines error constants */
#include <string.h>  /* Commonly used string-handling functions */
#include <stdbool.h> /* 'bool' type plus 'true' and 'false' constants */

#include "get_num.h"         /* Declares 'lib' functions for handling numeric arguments */
#include "error_functions.h" /* Declares 'lib' error-handling functions */

/* Unfortunately some UNIX implementations define FALSE and TRUE - undefine them */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum
{
    FALSE,
    TRUE
} Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

#endif