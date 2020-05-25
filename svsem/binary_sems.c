/* binary_sems.c

   Implement a binary semaphore protocol using System V semaphores.
*/
#include <sys/types.h>
#include <sys/sem.h>
#include "semun.h" /* Definition of semun union */
#include "binary_sems.h"

int /* Initialize semaphore to 1 (i.e., "available") */
initSemAvailable(int semId, int semNum)
{
    union semun arg;

    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int /* Initialize semaphore to 0 (i.e., "in use") */
initSemInUse(int semId, int semNum)
{
    union semun arg;

    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}