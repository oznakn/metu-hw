#ifndef _DEFINE_H
#define _DEFINE_H

#include "device.h"

#define _XTAL_FREQ   10000000

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _10MHZ	63055
#define _16MHZ	61768
#define _20MHZ	60768
#define _32MHZ	57768
#define _40MHZ 	55768

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define LCD_EVENT         0x10
#define RCV_EVENT         0x40
#define SND_EVENT         0x20

/***********************************************************************
 * ----------------------------- Alarms --------------------------------
 **********************************************************************/
#define LCD_ALARM_ID        0       /* Alarm ID (index) in tascdesc.c */
#define ALARM_TASK_ACTION    1       /* Alarm ID (index) in tascdesc.c */

/***********************************************************************
 * ---------------------- Task ID & Priorities -------------------------
 **********************************************************************/
#define TASK_SND_ID         1
#define TASK_RCV_ID         2
#define TASK_ACTION_ID      3
#define LCD_ID              5

#define TASK_SND_PRIO       10
#define TASK_RCV_PRIO       9
#define TASK_ACTION_PRIO    5
#define LCD_PRIO            5

#endif /* _DEFINE_H */


/* End of File : define.h */
