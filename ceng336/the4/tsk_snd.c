#include "define.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char rcv_value;
extern char rcv_buffer[10];
extern char rcv_buffer_cursor;
extern char snd_buffer[10];
extern char snd_buffer_cursor;

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

/**********************************************************************
 * ------------------------------ TASK0 -------------------------------
 *
 * Task that sends '@' character every second.
 *
 **********************************************************************/
TASK(TASK0)
{
    while(1) {
        WaitEvent(SND_EVENT);
        ClearEvent(SND_EVENT);

        TXREG = snd_buffer[snd_buffer_cursor++];
        TXSTAbits.TXEN = 1;
    } 
    TerminateTask();
}


/* End of File : tsk_task0.c */
