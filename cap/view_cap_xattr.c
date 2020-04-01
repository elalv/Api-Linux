/* view_cap_xattr.c

   Display the contents of the "security.capability" extended attribute
   of a file. This attribute is used to store the capabilities associated
   with a file.
*/
#include <sys/xattr.h>
#include <sys/capability.h>
#include <linux/capability.h> /* Defines 'struct vfs_ns_cap_data' and
                                   VFS_CAP_* constants */
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s <file>\n", argv[0]);

    struct vfs_ns_cap_data cap_data;
    ssize_t valueLen = getxattr(argv[1], "security.capability",
                                (char *)&cap_data, sizeof(cap_data));
    if (valueLen == -1)
    {
        if (errno == ENODATA)
            fatal("\"%s\" has no \"security.capability\" attribute", argv[1]);
        else
            errExit("getxattr");
    }

    printf("Capability version: %d",
           cap_data.magic_etc >> VFS_CAP_REVISION_SHIFT);

    /* Only version 3 capabilities have the 'rootid' field. */

    if ((cap_data.magic_etc & VFS_CAP_REVISION_MASK) == VFS_CAP_REVISION_3)
        printf("   [root ID = %u]", cap_data.rootid);

    printf("\n");

    /* The size of the returned attribute value depends on the version of
       the 'security.capability' extended attribute */

    printf("Length of returned value = %zd\n", valueLen);

    /* Display file capabilities */

    printf("    Effective bit:   %d\n",
           cap_data.magic_etc & VFS_CAP_FLAGS_EFFECTIVE);
    printf("    Permitted set:   %08x %08x\n",
           cap_data.data[1].permitted, cap_data.data[0].permitted);
    printf("    Inheritable set: %08x %08x\n",
           cap_data.data[1].inheritable, cap_data.data[0].inheritable);
    exit(EXIT_SUCCESS);
}