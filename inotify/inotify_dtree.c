/* inotify_dtree.c

   This is an example application to demonstrate the robust use of the
   inotify API.

   The goal of the application is to maintain an internal representation
   ("a cache") of the directory trees named on its command line. To keep
   the application shorter, just the directories are represented, not the
   files, but dealing with the latter is simpler in any case.

   As directories are added, removed, and renamed in the subtrees, the
   resulting inotify events are used to maintain an internal representation
   of the directory trees that remains consistent with the filesystem.
   The program also provides a command-line interface that allows the user
   to perform tasks such as dumping the current state of the cache and
   running a consistency check of the cache against the current state of
   the directory tree(s).

   Testing of this program is ongoing, and bug reports (to mtk@man7.org)
   are welcome.

   The rand_dtree.c program can be used to stress test the operation
   of this program.

   See also the article
   "Filesystem notification, part 2: A deeper investigation of inotify"
   July 2014
   https://lwn.net/Articles/605128/
*/

/* Known limitations
   - Pathnames longer than PATH_MAX are not handled.
*/
#include <fcntl.h>

static char *stopFile;

/* Something went badly wrong. Create a 'stop' file to signal the
   'rand_dtree' processes to stop, dump a copy of the cache to the
   log file, and abort. */

__attribute__ ((__noreturn__))
static void
createStopFileAndAbort(void)
{
    open(stopFile, O_CREAT | O_RDWR, 0600);
    dumpCacheToLog();
    abort();
}