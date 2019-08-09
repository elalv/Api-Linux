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
#include <sys/stat.h>
#include <limits.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* logMessage() flags */

#define VB_BASIC 1 /* Basic messages */
#define VB_NOISY 2 /* Verbose messages */

static int verboseMask;
static char *stopFile;
static int abortOnCacheProblem;

static FILE *logfp = NULL;

/* Something went badly wrong. Create a 'stop' file to signal the
   'rand_dtree' processes to stop, dump a copy of the cache to the
   log file, and abort. */

__attribute__((__noreturn__)) static void
createStopFileAndAbort(void)
{
    open(stopFile, O_CREAT | O_RDWR, 0600);
    dumpCacheToLog();
    abort();
}

/* Write a log message. The message is sent to none, either, or both of
   stderr and the log file, depending on 'vb_mask' and whether a log file
   has been specified via command-line options . */

static void
logMessage(int vb_mask, const char *format, ...)
{
    va_list argList;

    /* Write message to stderr if 'vb_mask' is zero, or matches one
       of the bits in 'verboseMask' */

    if ((vb_mask == 0) || (vb_mask & verboseMask))
    {
        va_start(argList, format);
        vfprintf(stderr, format, argList);
        va_end(argList);
    }

    /* If we have a log file, write the message there */

    if (logfp != NULL)
    {
        va_start(argList, format);
        vfprintf(logfp, format, argList);
        va_end(argList);
    }
}

/***********************************************************************/

/* Display some information about an inotify event. (Used when
   when we are doing verbose logging.) */

static void
displayInotifyEvent(struct inotify_event *ev)
{
    logMessage(VB_NOISY, "==> wd = %d; ", ev->wd);
    if (ev->cookie > 0)
        logMessage(VB_NOISY, "cookie = %4d; ", ev->cookie);

    logMessage(VB_NOISY, "mask = ");

    if (ev->mask & IN_ISDIR)
        logMessage(VB_NOISY, "IN_ISDIR ");

    if (ev->mask & IN_CREATE)
        logMessage(VB_NOISY, "IN_CREATE ");

    if (ev->mask & IN_DELETE_SELF)
        logMessage(VB_NOISY, "IN_DELETE_SELF ");

    if (ev->mask & IN_MOVE_SELF)
        logMessage(VB_NOISY, "IN_MOVE_SELF ");
    if (ev->mask & IN_MOVED_FROM)
        logMessage(VB_NOISY, "IN_MOVED_FROM ");
    if (ev->mask & IN_MOVED_TO)
        logMessage(VB_NOISY, "IN_MOVED_TO ");

    if (ev->mask & IN_IGNORED)
        logMessage(VB_NOISY, "IN_IGNORED ");
    if (ev->mask & IN_Q_OVERFLOW)
        logMessage(VB_NOISY, "IN_Q_OVERFLOW ");
    if (ev->mask & IN_UNMOUNT)
        logMessage(VB_NOISY, "IN_UNMOUNT ");

    logMessage(VB_NOISY, "\n");

    if (ev->len > 0)
        logMessage(VB_NOISY, "        name = %s\n", ev->name);
}

/***********************************************************************/

/* Data structures and functions for the watch list cache */

/* We use a very simple data structure for caching watched directory
   paths: a dynamically sized array that is searched linearly. Not
   efficient, but our main goal is to demonstrate the use of inotify. */

struct watch
{
    int wd;              /* Watch descriptor (-1 if slot unused) */
    char path[PATH_MAX]; /* Cached pathname */
};

struct watch *wlCache = NULL; /* Array of cached items */

static int cacheSize = 0; /* Current size of the array */

/* Deallocate the watch cache */

static void
freeCache(void)
{
    free(wlCache);
    cacheSize = 0;
    wlCache = NULL;
}

/* Check that all pathnames in the cache are valid, and refer
   to directories */

static void
checkCacheConsistency(void)
{
    int failures;
    struct stat sb;

    failures = 0;
    for (int j = 0; j < cacheSize; j++)
    {
        if (wlCache[j].wd >= 0)
        {
            if (lstat(wlCache[j].path, &sb) == -1)
            {
                logMessage(0,
                           "checkCacheConsistency: stat: "
                           "[slot = %d; wd = %d] %s: %s\n",
                           j, wlCache[j].wd, wlCache[j].path, strerror(errno));
                failures++;
            }
            else if (!S_ISDIR(sb.st_mode))
            {
                logMessage(0, "checkCacheConsistency: %s is not a directory\n",
                           wlCache[j].path);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (failures > 0)
        logMessage(VB_NOISY, "checkCacheConsistency: %d failures\n",
                   failures);
}

/* Check whether the cache contains the watch descriptor 'wd'.
   If found, return the slot number, otherwise return -1. */

static int
findWatch(int wd)
{
    for (int j = 0; j < cacheSize; j++)
        if (wlCache[j].wd == wd)
            return j;

    return -1;
}

/* Find and return the cache slot for the watch descriptor 'wd'.
   The caller expects this watch descriptor to exist.  If it does not,
   there is a problem, which is signaled by the -1 return. */

static int
findWatchChecked(int wd)
{
    int slot;

    slot = findWatch(wd);

    if (slot >= 0)
        return slot;

    logMessage(0, "Could not find watch %d\n", wd);

    /* With multiple renamers there are still rare cases where
       the cache is missing entries after a 'Could not find watch'
       event. It looks as though this is because of races with nftw(),
       since the cache is (occasionally) re-created with fewer
       entries than there are objects in the tree(s). Returning
       -1 to our caller identifies that there's a problem, and the
       caller should probably trigger a cache rebuild. */

    if (abortOnCacheProblem)
    {
        createStopFileAndAbort();
    }
    else
    {
        return -1;
    }
}

/* Mark a cache entry as unused */

static void
markCacheSlotEmpty(int slot)
{
    logMessage(VB_NOISY,
               "        markCacheSlotEmpty: slot = %d;  wd = %d; path = %s\n",
               slot, wlCache[slot].wd, wlCache[slot].path);

    wlCache[slot].wd = -1;
    wlCache[slot].path[0] = '\0';
}

/* Find a free slot in the cache */

static int
findEmptyCacheSlot(void)
{
    const int ALLOC_INCR = 200;

    for (int j = 0; j < cacheSize; j++)
        if (wlCache[j].wd == -1)
            return j;

    /* No free slot found; resize cache */

    cacheSize += ALLOC_INCR;

    wlCache = realloc(wlCache, cacheSize * sizeof(struct watch));
    if (wlCache == NULL)
        errExit("realloc");

    for (int j = cacheSize - ALLOC_INCR; j < cacheSize; j++)
        markCacheSlotEmpty(j);

    return cacheSize - ALLOC_INCR; /* Return first slot in
                                      newly allocated space */
}

/* Add an item to the cache */

static int
addWatchToCache(int wd, const char *pathname)
{
    int slot;

    slot = findEmptyCacheSlot();

    wlCache[slot].wd = wd;
    strncpy(wlCache[slot].path, pathname, PATH_MAX);

    return slot;
}

/* Return the cache slot that corresponds to a particular pathname,
   or -1 if the pathname is not in the cache */

static int
pathnameToCacheSlot(const char *pathname)
{
    for (int j = 0; j < cacheSize; j++)
        if (wlCache[j].wd >= 0 && strcmp(wlCache[j].path, pathname) == 0)
            return j;

    return -1;
}

/* Is 'pathname' in the watch cache? */

static int
pathnameInCache(const char *pathname)
{
    return pathnameToCacheSlot(pathname) >= 0;
}

/* Dump contents of watch cache to the log file */

static void
dumpCacheToLog(void)
{
    int cnt;

    cnt = 0;

    for (int j = 0; j < cacheSize; j++)
    {
        if (wlCache[j].wd >= 0)
        {
            fprintf(logfp, "%d: wd = %d; %s\n", j,
                    wlCache[j].wd, wlCache[j].path);
            cnt++;
        }
    }

    fprintf(logfp, "Total entries: %d\n", cnt);
}

/***********************************************************************/

/* Data structures and functions for dealing with the directory pathnames
   provided as command-line arguments. These directories form the roots of
   the trees that we will monitor */

static char **rootDirPaths; /* List of pathnames supplied on command line */
static int numRootDirs;     /* Number of pathnames supplied on command line */
static int ignoreRootDirs;  /* Number of command-line pathnames that
                               we've ceased to monitor */
static struct stat *rootDirStat;
/* stat(2) structures for root directories */

/* Duplicate the pathnames supplied on the command line, perform
   some sanity checking along the way */

static void
copyRootDirPaths(char *argv[])
{
    struct stat sb;

    numRootDirs = 0;

    /* Count the number of root paths, and check that the paths are valid */

    for (char **p = argv; *p != NULL; p++)
    {

        /* Check that command-line arguments are directories */

        if (lstat(*p, &sb) == -1)
        {
            fprintf(stderr, "lstat() failed on '%s'\n", *p);
            exit(EXIT_FAILURE);
        }

        if (!S_ISDIR(sb.st_mode))
        {
            fprintf(stderr, "'%s' is not a directory\n", *p);
            exit(EXIT_FAILURE);
        }

        numRootDirs++;
    }

    /* Create a copy of the root directory pathnames */

    rootDirPaths = calloc(numRootDirs, sizeof(char *));
    if (rootDirPaths == NULL)
        errExit("calloc");

    rootDirStat = calloc(numRootDirs, sizeof(struct stat));
    if (rootDirPaths == NULL)
        errExit("calloc");

    for (int j = 0; j < numRootDirs; j++)
    {
        rootDirPaths[j] = strdup(argv[j]);
        if (rootDirPaths[j] == NULL)
            errExit("strdup");

        /* If the same filesystem object appears more than once in the
           command line, this will cause confusion if we later try to zap
           an object from the set of root paths. So, reject such
           duplicates now. Note that we can't just do simple string
           comparisons of the arguments, since different pathname strings
           may refer to the same filesystem object (e.g., "mydir" and
           "./mydir"). So, we use stat() to compare i-node numbers and
           containing device IDs. */

        if (lstat(argv[j], &rootDirStat[j]) == -1)
            errExit("lstat");

        for (int k = 0; k < j; k++)
        {
            if ((rootDirStat[j].st_ino == rootDirStat[k].st_ino) &&
                (rootDirStat[j].st_dev == rootDirStat[k].st_dev))
            {

                fprintf(stderr, "Duplicate filesystem objects: %s, %s\n",
                        argv[j], argv[k]);
                exit(EXIT_FAILURE);
            }
        }
    }

    ignoreRootDirs = 0;
}

/* Return the address of the element in 'rootDirPaths' that points
   to a string matching 'path', or NULL if there is no match */

static char **
findRootDirPath(const char *path)
{
    for (int j = 0; j < numRootDirs; j++)
        if (rootDirPaths[j] != NULL && strcmp(path, rootDirPaths[j]) == 0)
            return &rootDirPaths[j];

    return NULL;
}

/* Is 'path' one of the pathnames that was listed on the command line? */

static int
isRootDirPath(const char *path)
{
    return findRootDirPath(path) != NULL;
}

/* We've ceased to monitor a root directory pathname (probably because it
   was renamed), so zap this pathname from the root path list */

static void
zapRootDirPath(const char *path)
{
    char **p;

    printf("zapRootDirPath: %s\n", path);

    p = findRootDirPath(path);
    if (p == NULL)
    {
        fprintf(stderr, "zapRootDirPath(): path not found!\n");
        exit(EXIT_FAILURE);
    }

    *p = NULL;
    ignoreRootDirs++;
    if (ignoreRootDirs == numRootDirs)
    {
        fprintf(stderr, "No more root paths left to monitor; bye!\n");
        exit(EXIT_SUCCESS);
    }
}