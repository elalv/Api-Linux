/* t_unlink.c

    Demonstrate that, when a file is unlinked, it is not actually removed from
    the file system until after any open descriptors referring to it are closed.

    Usage: t_unlink file
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define CMD_SIZE 200
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int fd, j, numBlocks;
    char shellCmd[CMD_SIZE]; /* Command to be passed to system() */
    char buf[BUF_SIZE];      /* Random bytes to write to file */

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s temp-file [num-1kB-blocks] \n", argv[0]);

    numBlocks = (argc > 2) ? getInt(argv[2], GN_GT_0, "num-1kB-blocks")
                           : 100000;

    /* O_EXCL so that we ensure we create a new file */

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    if (unlink(argv[1]) == -1) /* Remove filename */
        errExit("unlink");

    for (j = 0; j < numBlocks; j++) /* Write lots of junk to file */
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
            fatal("partial/failed write");

    snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
    system(shellCmd); /* View space used in file system */

    if (close(fd) == -1) /* File is now destroyed */
        errExit("close");
    printf("********** Closed file descriptor\n");

    system(shellCmd); /* Review space used in file system */
    exit(EXIT_SUCCESS);
}