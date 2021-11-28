#pragma config OSC = HS, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include "define.h"

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE       0


/**********************************************************************
 * Function prototypes.
 **********************************************************************/
void main (void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);

extern union Timers Tmr0;

AppModeType SelectedMode;

/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * Setup the different alarms and start the kernel.
 *
 **********************************************************************/
void main(void)
{
    STKPTR = 0;
    SelectedMode = DEFAULT_MODE;
    Init();

    while(1)
    {
        StartOS(SelectedMode);
    }
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/
void Init(void)
{
    FSR0H = 0;
    FSR0L = 0;

    /* User setting : actual PIC frequency */
    Tmr0.lt = _10MHZ;

    /* Timer OFF - Enabled by Kernel */
    T0CON = 0x08;
    TMR0H = Tmr0.bt[1];
    TMR0L = Tmr0.bt[0];

    /* configure I/O ports */
    TRISCbits.RC7 = 1; // TX1 and RX1 pin configuration
    TRISCbits.RC6 = 0;

    /* configure USART transmitter/receiver */
    SPBRG = 64;    // for 10 MHz, to have 9600 baud rate, it should be 64
    TXSTA = 0x04;      // (= 00000100) 8-bit transmit, transmitter NOT enabled,
                            // asynchronous, high speed mode
    RCSTA = 0x90;      // (= 10010000) 8-bit receiver, receiver enabled,
                            // continuous receive, serial port enabled

    /* configure the interrupts */
    INTCON = 0;      // clear interrupt register completely
    PIE1bits.TXIE = 1;  // enable USART transmit interrupt
    PIE1bits.RCIE = 1;  // enable USART receive interrupt
    PIR1 = 0;      // clear all peripheral flags

    INTCONbits.PEIE = 1;// enable peripheral interrupts
    INTCONbits.GIE = 1;  // globally enable interrupts
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
