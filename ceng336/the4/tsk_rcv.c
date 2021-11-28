#include "define.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
extern char mode;
extern char rcv_value;
extern char rcv_buffer[10];
extern char rcv_buffer_cursor;
extern char snd_buffer[10];
extern char snd_buffer_cursor;

extern char mine_x;
extern char mine_y;
extern char mine_type;
extern char mine_exist;

/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/

/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Typing a single char should reply with the same char. You will need buffers to echo strings.
 *
 **********************************************************************/
TASK(TASK1) 
{
	while(1) {
        WaitEvent(RCV_EVENT);
        ClearEvent(RCV_EVENT);
        
        if (rcv_value == '$') {
            rcv_buffer_cursor = 0;
        } else if (rcv_value == ':') {
            rcv_buffer[rcv_buffer_cursor++] = 0;
            
            if (rcv_buffer[0] == 'G' && rcv_buffer[1] == 'O') {
                mode = 1;
            } else if (rcv_buffer[0] == 'A') {
                if (!mine_exist && rcv_buffer[1] < 250 && rcv_buffer[2] < 250) {
                    mine_x = rcv_buffer[1];
                    mine_y = rcv_buffer[2];
                    mine_type = rcv_buffer[3];
                    mine_exist = 1;
                }
            }
            
            rcv_buffer_cursor = 0;
        } else {
            rcv_buffer[rcv_buffer_cursor++] = rcv_value;
        }
	}
	TerminateTask();
}

/* End of File : tsk_task1.c */