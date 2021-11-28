#include "define.h"

char mode = 0;
char rcv_value;
char rcv_buffer[10];
char rcv_buffer_cursor = 0;

char snd_buffer[10];
char snd_buffer_cursor = 0;

char mine_x = 0;
char mine_y = 0;
char mine_type = 0;
char mine_exist = 0;
char mine_counts[2] = {3,3};


/**********************************************************************
 * Function you want to call when an IT occurs.
 **********************************************************************/
extern void AddOneTick(void);
/*extern void MyOwnISR(void); */
void InterruptVectorL(void);
void InterruptVectorH(void);

/**********************************************************************
 * General interrupt vector. Do not modify.
 **********************************************************************/
#pragma code IT_vector_low=0x18
void Interrupt_low_vec(void)
{
   _asm goto InterruptVectorL  _endasm
}
#pragma code

#pragma code IT_vector_high=0x08
void Interrupt_high_vec(void)
{
   _asm goto InterruptVectorH  _endasm
}
#pragma code

/**********************************************************************
 * General ISR router. Complete the function core with the if or switch
 * case you need to jump to the function dedicated to the occuring IT.
 * .tmpdata and MATH_DATA are saved automaticaly with C18 v3.
 **********************************************************************/
#pragma	code _INTERRUPT_VECTORL = 0x003000
#pragma interruptlow InterruptVectorL
void InterruptVectorL(void) {
    EnterISR();

    if (INTCONbits.TMR0IF == 1) {
        AddOneTick();
    }

    /*Here is the next interrupts you want to manage */
    /*if (INTCONbits.RBIF == 1)                      */
    /*  MyOwnISR();                                  */

    if (PIR1bits.TXIF == 1) {
        PIR1bits.TXIF = 0; // You only need to do this on picsim. normally this would have no effect in PIC, this flag cannot be cleared by software. only way to clear the interrupt is load another char on TXREG or unset TXSTAbits.TXEN. this feature is probably not correctly implemented in picsim
        TXSTAbits.TXEN = 0; // you do this both for picsim and real life to terminate the transmission.

        if (snd_buffer[snd_buffer_cursor] != 0) {
            SetEvent(TASK_SND_ID, SND_EVENT);   
        } else {
            snd_buffer_cursor = 0;   
        }
    }

	if (PIR1bits.RCIF == 1) {
        rcv_value = RCREG;
        SetEvent(TASK_RCV_ID, RCV_EVENT);
		PIR1bits.RCIF = 0;	// clear RC1IF flag
	}

    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }

    LeaveISR();
}
#pragma	code

/* BE CARREFULL : ONLY BSR, WREG AND STATUS REGISTERS ARE SAVED  */
/* DO NOT CALL ANY FUNCTION AND USE PLEASE VERY SIMPLE CODE LIKE */
/* VARIABLE OR FLAG SETTINGS. CHECK THE ASM CODE PRODUCED BY C18 */
/* IN THE LST FILE.                                              */
#pragma	code _INTERRUPT_VECTORH = 0x003300
#pragma interrupt InterruptVectorH nosave=FSR0, TBLPTRL, TBLPTRH, TBLPTRU, TABLAT, PCLATH, PCLATU, PROD, section(".tmpdata"), section("MATH_DATA")
void InterruptVectorH(void)
{
    if (INTCONbits.INT0IF == 1)
        INTCONbits.INT0IF = 0;
}
#pragma	code



extern void _startup (void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x003400
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code


/* End of file : int.c */
