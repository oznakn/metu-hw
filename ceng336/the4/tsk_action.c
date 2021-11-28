#include "define.h"

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
extern char mine_counts[2];

char w = 3;
char w_step = 0;
char curr_x = 25;
char curr_y = 25;

char adapt_w(char new_w) {
    if (w == new_w) return 1;
    
    if (w < new_w) {
        w_step += 9;
        if (w_step == 90) {
            w_step = 0;
            w++;
        }

        snd_buffer[0] = '$';
        snd_buffer[1] = 'R';
        snd_buffer[2] = ':';
        snd_buffer[3] = 0;
        snd_buffer_cursor = 0;
        SetEvent(TASK_SND_ID, SND_EVENT);
    }

    if (w > new_w) {
        w_step += 9;
        if (w_step == 90) {
            w_step = 0;
            w--;
        }

        snd_buffer[0] = '$';
        snd_buffer[1] = 'L';
        snd_buffer[2] = ':';
        snd_buffer[3] = 0;
        snd_buffer_cursor = 0;
        SetEvent(TASK_SND_ID, SND_EVENT);
    }

    if (w == 0) w = 4;
    if (w == 5) w = 1;
    
    return 0;
}

void forward(void) {
    snd_buffer[0] = '$';
    snd_buffer[1] = 'F';
    snd_buffer[2] = ':';
    snd_buffer[3] = 0;
    snd_buffer_cursor = 0;
    SetEvent(TASK_SND_ID, SND_EVENT);
}

TASK(TASK2) 
{
    SetRelAlarm(ALARM_TASK_ACTION, 100, 150);
    
    while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        
        if (mode) {
            if (mine_exist) {
                if (mine_y > curr_y) {
                    if (adapt_w(1)) {
                        forward();
                        curr_y += 5;
                    }
                } else if (mine_y < curr_y) {
                    if (adapt_w(3)) {
                        forward();
                        curr_y -= 5;
                    }
                } else {
                    if (mine_x > curr_x) {
                        if (adapt_w(2)) {
                            forward();
                            curr_x += 5;
                        }
                    } else if (mine_x < curr_x) {
                        if (adapt_w(4)) {
                            forward();
                            curr_x -= 5;
                        }
                    } else {
                        snd_buffer[0] = '$';
                        snd_buffer[1] = 'P';
                        snd_buffer[2] = ':';
                        snd_buffer[3] = 0;
                        snd_buffer_cursor = 0;
                        SetEvent(TASK_SND_ID, SND_EVENT);
                        
                        mine_counts[mine_type]--;
                        mine_exist = 0;

                        if (mine_counts[0] == 0 && mine_counts[1] == 0) {
                            snd_buffer[0] = '$';
                            snd_buffer[1] = 'E';
                            snd_buffer[2] = 'N';
                            snd_buffer[3] = 'D';
                            snd_buffer[4] = ':';
                            snd_buffer[5] = 0;
                            snd_buffer_cursor = 0;
                            SetEvent(TASK_SND_ID, SND_EVENT);
                            mode = 0;
                        }
                    }
                }
            } else {
                if (w == 3 && curr_y == 160) {
                    if (adapt_w(1)) {
                        forward();
                        curr_y += 5;
                    }
                } else if (w == 1 && curr_y == 25) {
                    if (adapt_w(3)) {
                        forward();
                        curr_y -= 5;
                    }
                }
            }
        }
    }

	TerminateTask();
}