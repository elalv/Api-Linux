/* event_flags.h

   Header file for event_flags.c.

   The event flags operations are:

        set a flag:              setEventFlag(semId, semNum)
        clear a flag:            clearEventFlag(semId, semNum)
        wait for flag to be set: waitForEventFlag(semId, semNum)
        read a flag's value:     getFlagState(semId, semNum, &isSet)

   NB: The semantics of System V semaphores require that the "set"
   value for a flag is 0 and the "clear" value is 1.
*/