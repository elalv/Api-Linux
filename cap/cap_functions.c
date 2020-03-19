#include <stdio.h>
#include "cap_functions.h"

/* Change the 'setting' of the specified 'capability' in the capability set
   specified by 'flag'.

   'flag' is one of CAP_PERMITTED, CAP_EFFECTIVE, or CAP_INHERITABLE.
   'setting' is one of CAP_SET or CAP_CLEAR.

   Returns: 0 on success or -1 on error. */

int modifyCapSetting(cap_flag_t flag, int capability, int setting)
{
    cap_t caps;
    cap_value_t capList[1];

    /* Retrieve caller's current capabilities */

    caps = cap_get_proc();
    if (caps == NULL)
        return -1;

    /* Change setting of 'capability' in the 'flag' capability set in 'caps'.
       The third argument, 1, is the number of items in the array 'capList'. */

    capList[0] = capability;
    if (cap_set_flag(caps, flag, 1, capList, setting) == -1)
    {
        cap_free(caps);
        return -1;
    }

    /* Push modified capability sets back to kernel, to change
       caller's capabilities */

    if (cap_set_proc(caps) == -1)
    {
        cap_free(caps);
        return -1;
    }

    /* Free the structure that was allocated by cap_get_proc() */

    if (cap_free(caps) == -1)
        return -1;

    return 0;
}