/* cap_functions.h

   Useful functions for working with capabilities.
*/
#ifndef CAP_FUNCTIONS_H /* Prevent double inclusion */
#define CAP_FUNCTIONS_H

/* Change the 'setting' of the specified 'capability' in the capability set
   specified by 'flag'.

   'flag' is one of CAP_PERMITTED, CAP_EFFECTIVE, or CAP_INHERITABLE.
   'setting' is one of CAP_SET or CAP_CLEAR.

   Returns: 0 on success or -1 on error. */

#include <sys/capability.h>

int modifyCapSetting(cap_flag_t flag, int capability, int setting);

#endif