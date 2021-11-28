/* OZAN AKIN 2309599 */

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 40000000L
#include <xc.h>

#include <stdio.h>
#include <string.h>

#include "LCD.h"

/* The definitions for 7 segment letters */
#define LETTER_0 0x3F
#define LETTER_1 0x06
#define LETTER_2 0x5B
#define LETTER_3 0x4F
#define LETTER_4 0x66
#define LETTER_5 0x6D
#define LETTER_6 0x7D
#define LETTER_7 0x07
#define LETTER_8 0x7F
#define LETTER_9 0x6F
#define LETTER_NONE 0x00

/* The 7 segment characters for displaying remaining attempt */
#define ATTEMPT_3 0x49
#define ATTEMPT_2 0x48
#define ATTEMPT_1 0x08

/* 7 Segment select values for LATA */
#define SEGMENT_2 0x08
#define SEGMENT_3 0x10
#define SEGMENT_4 0x20

/* Reset values for TMR 0 and 1*/
#define TIMER0_RESET 0xB3B5 // 65535 - (10000*500/256 (Mhz*ms/prescale)) + 1 = 46005
#define TIMER1_RESET 0xD8F0 // 65535 - (10000*8/8 (Mhz*ms/prescale)) + 1 = 55536

/* TMR0 is used for keeping the track of countdown timer. TMR0 works every 0.5s
 * to use the same timer for counting down every second with also 500ms blink
 * in the fail stage. The system keeps a toggle variable called "timer_toggle"
 * to scale 500ms to 1s for the countdown timer.
 *
 * TMR1 is used for 7 segment display. It works every 8ms and lights up the 7
 * segment display with a round-robin pattern. Each time the timer overflows
 * the system switches to next 7segment display and lights it up. TMR1 is only
 * enabled in the password check stage.
 */

/* The default values for password check stage */
#define TIMEOUT_COUNTER_RESET 90
#define REMAINING_ATTEMPT_RESET 3

/* The stage values for keeping the track of current stage */
#define STAGE_INIT 0
#define STAGE_SET_PASSWORD 1
#define STAGE_CHECK_PASSWORD 2
#define STAGE_SUCCESS 3
#define STAGE_FAIL 4

/* An array for mapping numbers into 7segment display letters */
unsigned char number_letters[10] = {
    LETTER_0,
    LETTER_1,
    LETTER_2,
    LETTER_3,
    LETTER_4,
    LETTER_5,
    LETTER_6,
    LETTER_7,
    LETTER_8,
    LETTER_9,
};

/* An array for mapping remaining attempt into 7segment display letters */
unsigned char attempt_segments[4] = {
    LETTER_NONE,
    ATTEMPT_1,
    ATTEMPT_2,
    ATTEMPT_3
};

/* Segment selection mapper for the TMR1 timer interrupt, it only uses
 * the last 3 digit of 7 segment display
 */
unsigned char segment_selection[3] = {
    SEGMENT_4,
    SEGMENT_3,
    SEGMENT_2,
};

/* The current values of the 7 segment display letters (in the reversed order) */
unsigned char segments[3] = {
    LETTER_NONE,
    LETTER_NONE,
    LETTER_NONE
};

/* The values for the current stage, if the stage_will_change is not equal
 * current_stage, system switches to the stage of the value of stage_will_change
 */
int current_stage = 0;
int stage_will_change = 0;

/* The values for keeping the track of the input values. password_saved is for
 * the password set stage, password_input is for the password_check stage, and
 * last_pot_value is for keeping the track of last pot value written on the
 * LCD display.
 */
int password_saved[3] = {0};
int password_input[3] = {0};
int last_pot_value = 0;

/* The input cursor for the password input, it can be 0, 1, or 2 */
int input_cursor = 0;

/* For keeping the track of is countdown started or not in the password check
 * stage. The countdown starts with a user interaction with the system.
 */
int is_countdown_started = 0;

/* The current values of remaining time and attempt in the password check stages */
int remaining_time = TIMEOUT_COUNTER_RESET;
int remaining_attempt = REMAINING_ATTEMPT_RESET;

/* The current lighted up segment with the TMR1 interrupt */
int segment_display_state = 0;

/* For the keeping the track of PORTB blinking in the wrong password fail. The
 * first variable keeps the is the blinking started or not. The second variable keeps
 * the current state of the blinking.
 */
int portb_blink_enabled = 0;
int portb_blink_state = 0;

/* The adc toggle, the system checks the ADC values in each two loops, with this
 * feature the system make sure that both AN0 and AN2 ADC's can be detected
 */
int adc_toggle = 0;

/* The timer toggle for prescaling the TMR0 for 1s. */
int timer_toggle = 0;

/* The save values for LATA and LATD, like callee-saved registers. It is necessary
 * to edit the LCD display while displaying the 7segment display values.
 */
unsigned char lata_saved;
unsigned char latd_saved;

/* This methods locks the system for the display, i.e. it disables the TMR1
 * interrupt to make sure that the 7segment display does not actively working.
 * Moreover, it saves the values for LATA and LATD registers.
 */
inline void lock_for_lcd() {
    PIE1bits.TMR1IE = 0;

    lata_saved = LATA;
    latd_saved = LATD;

    LATA = 0x00;
    LATD = 0x00;
}

/* Unlocks the system from the display, i.e. it reenables the TMR1 interrupt
 * and restore the values of LATA and LATD registers, to continue lighting the
 * 7 segment display
 */
inline void unlock_for_lcd() {
    LATA = lata_saved;
    LATD = latd_saved;

    PIE1bits.TMR1IE = 1;
}

/* Blinks the PORTB for the password fail. It makes the LATB 0 if
 * the blink is not enabled.
 */
void blink_portb(void) {
    if (portb_blink_enabled == 1) {
        if (portb_blink_state == 1) {
            LATB = 0x00;
            portb_blink_state = 0;
        } else {
            LATB = 0xFF;
            portb_blink_state = 1;
        }
    } else {
        LATB = 0x00;
        portb_blink_state = 0;
    }
}

/* Sets and enables the ADC */
inline void enable_adc(void) {
    // RA0 = Analog, RA1= Analog, RA2 = Analog, the rest is digital.
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 0;
    ADCON1bits.PCFG0 = 0;

    ADCON1bits.VCFG0 = 0; // Vref = 5.0, Vref = 0
    ADCON1bits.VCFG1 = 0;

    // RA0, RA1, and RA2 are input.
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;

    // For 40MHZ -> Tosc = 1/40 us
    // Tad options (2xTosc, 4xTosc, 8xTosc, 16xTosc, 32xTosc, 64xTosc)
    // min Tad 0.7 us - max Tad 25 us (Keep as short as possible)
    // The closest one to min Tad (8xTosc) hence Tad = 32xTosc = 0.8 us (ADCS2:ADCS0=010)
    // Acquisition time options (0xTad, 2xTad, 4xTad, 6xTad, 8xTad, 12xTad, 16xTad, 20xTad)
    // Min acquisition time = 2.4 us (the closest acqusition time we can set 4xTad = 3.2us) (ACQT2:ACQT0=010)

    ADCON2bits.ADCS2 = 0; //Tad (32xTOSC) -> 0.8us
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS0 = 0;

    ADCON2bits.ACQT2 = 0; // Acqusition time (4xTad) = 3.2 us
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 0;

    ADCON2bits.ADFM = 1; // Right justified...

    // To remove some glitch related to the 7 segment display
    LATA = 0x00;

    // Set the channel to 0, for initial startup.
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;

    // Enable the ADC interrupt
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;

    ADCON0bits.ADON = 1; // ADC module is enabled....
}

/* Set the channel to 0 and start ADC process */
inline void start_adc0(void) {
    ADCON0bits.CHS1 = 0;

    ADCON0bits.GODONE = 1; // GODONE = 1 starts the process
}

/* Set the channel to 2 and start ADC process */
inline void start_adc2(void) {
    ADCON0bits.CHS1 = 1;

    ADCON0bits.GODONE = 1; // GODONE = 1 starts the process
}

/* Enables the related interrupts and button configurations with
 * RB0 is input, the rest is output.
 */
inline void enable_buttons_rb0(void) {
    // First disable all button interrupts
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 0;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    TRISBbits.RB7 = 0;

    PORTBbits.RB0 = 1;

    // Enable the related interrupts
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;
}

/* Enables the related interrupts and button configurations with
 * RB4 is input, the rest is output.
 */
inline void enable_buttons_rb4(void) {
    // First disable all button interrupts
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 0;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 1;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    TRISBbits.RB7 = 0;

    PORTBbits.RB4 = 1;

    // Enable the related interrupts
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 0;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
}

/* Disables all interrupts and makes all RB pins output */
inline void disable_buttons(void) {
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 0;

    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 0;

    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    TRISBbits.RB7 = 0;
}

/* Sets the pins for the 7segment display, configures TMR1, but not starts.
 * The start of TMR1 will happen in setup_password_check_stage function.
 */
inline void enable_segment_display(void) {
    // Make sure that RA3, RA4, and RA5 are output
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;

    // Set the timer
    TMR1 = TIMER1_RESET;
    T1CON = 0b01110000; // Configure the TMR1 with 1:8 prescale mode.

    // Enable TMR1 interrupt
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;

    // Notice that the TMR1 has not started yet.

    // Set TRISD to remove some glitches.
    TRISD = 0x00;
}

/* Enables the heather. */
inline void enable_heather(void) {
    TRISCbits.RC5 = 0;
    LATCbits.LATC5 = 1;
}

/* Disables the heather. */
inline void disable_heather(void) {
    LATCbits.LATC5 = 0;
}

/* Enables the TMR0 for countdown. */
inline void enable_timer(void) {
    TMR0 = TIMER0_RESET;
    T0CON = 0b00000111; // Configure TMR0 with 1:256 prescale

    // Enable the TMR0 overflow interrupt
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;

    // Start the TMR0
    T0CONbits.TMR0ON = 1;
}

/* Disables the TMR0 */
inline void disable_timer(void) {
    // Disable the TMR0 interrupt
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;

    // Remove configuration of TMR0 and stops the timer
    T0CON = 0;
    TMR0 = 0;
}

/* Updates the segment values with remaining time and attempt. */
inline void update_segment_values_with_remaining_time() {
    // The second digit of countdown
    segments[0] = number_letters[(int) (remaining_time % 10)];

    // The first digit of countdown
    segments[1] = number_letters[(int) (remaining_time / 10)];

    // The remaining attempt digit
    segments[2] = attempt_segments[remaining_attempt];

    // Notice that the values are in the reversed order.
}

/* Disables and resets the 7 segment display */
inline void reset_segments(void) {
    // Disable and remove configuration of TMR1
    T1CON = 0;
    TMR1 = 0;

    // Set the segments none
    segments[0] = LETTER_NONE;
    segments[1] = LETTER_NONE;
    segments[2] = LETTER_NONE;

    LATD = LETTER_NONE;

    LATA = segment_selection[0];
    LATA = segment_selection[1];
    LATA = segment_selection[2];
}

/* Syncs the LCD cursor with the variable input_cursor */
inline void lcd_sync_cursor(void) {
    switch (input_cursor) {
        case 0:
            LCDGoto(1, 2);
            break;

        case 1:
            LCDGoto(4, 2);
            break;

        case 2:
            LCDGoto(7, 2);
            break;
    }
}

/* Updates LCD with the pot_value and input_cursor */
inline void update_lcd_with_input(int pot_value) {
    // The function updates different arrays depending on the current stage.
    if (current_stage == STAGE_SET_PASSWORD) {
        password_saved[input_cursor] = pot_value;
    } else if (current_stage == STAGE_CHECK_PASSWORD) {
        password_input[input_cursor] = pot_value;
    }

    // Convert pot_value into string
    // Note that the pot_value is not ADC value, the pot value is in the range
    // [-1, 99]. -1 means invalid value.
    char s[3] = {0};

    if (pot_value == -1) {
        sprintf(s, "XX");
    } else {
        sprintf(s, "%02d", pot_value);
    }

    // Freeze 7segment displays
    lock_for_lcd();

    // Sync the cursor and print the value
    lcd_sync_cursor();
    LCDStr(s);

    // Enable 7 segment displays
    unlock_for_lcd();
}

/* The interrupt function, it runs in TMR0, TMR1, ADC, RB, and INT0 interrupts */
void __interrupt(high_priority) FNC() {
    if (INTCONbits.TMR0IF) { // The TMR0 interrupt
        // This interrupt only run in password check stage
        if (current_stage == STAGE_CHECK_PASSWORD) {
            // Check the timer toggle, for prescaling 0.5s to 1s
            if (timer_toggle) {
                timer_toggle = 0;
            } else {
                // 1 second passed, update remaining time
                remaining_time--;
                update_segment_values_with_remaining_time();

                // If remaining time hit 0, change stage to fail
                if (remaining_time == 0) {
                    stage_will_change = STAGE_FAIL;
                }

                timer_toggle = 1;
            }

            // Blink portb, if portb blinking is enabled
            blink_portb();
        }

        // Reset the TMR0 values
        TMR0 = TIMER0_RESET;

        INTCONbits.TMR0IF = 0;
    } else if (PIR1bits.TMR1IF) { // TMR1 interrupt
        if (current_stage == STAGE_CHECK_PASSWORD) { // This interrupt only run in password check stage
            segment_display_state++; // Change the digit for 7 segment

            if (segment_display_state == 3) segment_display_state = 0;

            // Update the related values
            LATA = segment_selection[segment_display_state];
            LATD = segments[segment_display_state];
        }

        // Reset the TMR1 values
        TMR1 = TIMER1_RESET;

        PIR1bits.TMR1IF = 0;
    } else if (INTCONbits.INT0IF) { // RB0 interrupt
        // RB0 interrupt could happen in multiple stages
        switch (current_stage) {
            case STAGE_SET_PASSWORD:
                // In password set stage, the RB0 interrupt shifts the cursor
                if (password_saved[input_cursor] != -1) {
                    input_cursor++;
                    if (input_cursor == 3) {
                        stage_will_change = STAGE_CHECK_PASSWORD;
                    }
                }

                break;

            case STAGE_CHECK_PASSWORD:
                // In password check stage, if the RB0 clicked, that means an
                // user interaction occurred, start the timer
                if (is_countdown_started == 0) {
                    is_countdown_started = 1;
                    enable_timer();
                }

                // If the current input value is valid, check the value.
                // If it is correct, shift the cursor
                // If it is not, then update the remaining attempt
                if (password_input[input_cursor] != -1) {
                    if (password_input[input_cursor] != password_saved[input_cursor]) {
                        remaining_attempt--;
                        update_segment_values_with_remaining_time();

                        switch (remaining_attempt) {
                            case 0:
                                // No attempt left, change stage
                                portb_blink_enabled = 0;
                                blink_portb();

                                disable_heather();

                                stage_will_change = STAGE_FAIL;
                                break;

                            case 1:
                                // Enable heather
                                enable_heather();
                                break;

                            case 2:
                                // Enable portb blinking
                                portb_blink_enabled = 1;
                                blink_portb();

                                break;
                        }
                    } else {
                        input_cursor++;

                        if (input_cursor == 3) {
                            stage_will_change = STAGE_SUCCESS;
                        }
                    }
                }

                break;
        }

        // Reset the RB0 interrupt
        INTCONbits.INT0IF = 0;
    } else if (INTCONbits.RBIF) { // RB4 will change stage from success to check password
        if (PORTBbits.RB4 == 1) {
            if (current_stage == STAGE_SUCCESS) {
                stage_will_change = STAGE_CHECK_PASSWORD;
            }
        }

        // Reset the RB button interrupt
        INTCONbits.RBIF = 0;
    } else if (PIR1bits.ADIF) { // ADC interrupt
        if (ADCON0bits.GODONE == 0) { // the ADC process finished
            int adc_value = (int)((ADRESH << 8)+ADRESL); // Fetch the adc value

            if (ADCON0bits.CHS1 == 0) { // If the channel is 0
                // Calculate the pot_value with currrent input_cursor
                int pot_value = adc_value;

                if (input_cursor == 1) pot_value = 1023 - pot_value;

                pot_value = (int) (pot_value / (int) 10);

                if (pot_value < 0 || pot_value > 100) {
                    pot_value = -1;
                }

                last_pot_value = pot_value;

                // Update the LCD with the input value depending on the stage
                if (current_stage == STAGE_SET_PASSWORD) {
                    if (password_saved[input_cursor] != pot_value) {
                        update_lcd_with_input(pot_value);
                    }
                } else if (current_stage == STAGE_CHECK_PASSWORD) {
                    if (password_input[input_cursor] != pot_value) {
                        update_lcd_with_input(pot_value);

                        if (is_countdown_started == 0) {
                            is_countdown_started = 1;
                            enable_timer();
                        }
                    }
                }
            } else { // AN2, the temperature value interrupt
                double temperature_value = ((adc_value*5)/1024.0) * 100.0;

                if (temperature_value >= 40.0) { // If the temperature is above 40, then change stage to fail
                    stage_will_change = STAGE_FAIL;
                }
            }
        }

        // Reset the ADC interrupt
        PIR1bits.ADIF = 0;
    }
}


/* The setup function for startup stage, it waits 3 seconds and exists */
void startup_stage(void) {
    lock_for_lcd(),
    LCDCmd(LCD_CLEAR);
    LCDGoto(1, 1);
    LCDStr("SuperSecureSafe!");
    unlock_for_lcd();
    LATD = 0x00;
    __delay_ms(3000);
}

/* The setup function for password set stage, it sets the variables and LCD display
 * The rest is configured in the setup function.
 */
void setup_set_password_stage(void) {
    reset_segments();

    lock_for_lcd();
    LCDCmd(LCD_CLEAR);
    LCDGoto(1, 1);
    LCDStr("Set Password:");

    LCDGoto(1, 2);
    LCDStr("__-__-__");
    unlock_for_lcd();

    LATD = 0x00;
    LATB = 0x00;

    input_cursor = 0;
}

/* The step function for password set stage. This function will run in a loop.
 * This function will check the value of AN0 if there is no ongoing ADC check.
 */
void step_set_password_stage(void) {
    if (ADCON0bits.GODONE == 0 && PIR1bits.ADIF == 0) {
        start_adc0();
    }
}

/* The setup function for password check stage, it sets the variables, enables TMR1,
 * and sets LCD display. The rest is configured in the setup function.
 */
void setup_check_password_stage(void) {
    password_input[0] = 0;
    password_input[1] = 0;
    password_input[2] = 0;

    reset_segments();

    lock_for_lcd();
    LCDCmd(LCD_CLEAR);
    LCDGoto(1, 1);
    LCDStr("Input Password:");

    LCDGoto(1, 2);
    LCDStr("__-__-__");
    unlock_for_lcd();

    LATD = 0x00;
    LATB = 0x00;

    timer_toggle = 0;
    input_cursor = 0;

    remaining_time = TIMEOUT_COUNTER_RESET;
    is_countdown_started = 0;
    remaining_attempt = REMAINING_ATTEMPT_RESET;

    // Update LCD with last pot value coming from the previous stage.
    update_lcd_with_input(last_pot_value);
    LATD = 0x00;

    update_segment_values_with_remaining_time();

    // Enable TMR1
    T1CONbits.TMR1ON = 1;
}

/* The step function for password change stage. This function will run in a loop.
 * This function will check the value of AN0 and AN2 if there is no ongoing ADC check.
 * The function works with a round robin manner, with checking AN2, then AN0, then AN2...
 */
void step_check_password_stage(void) {
    if (ADCON0bits.GODONE == 0 && PIR1bits.ADIF == 0) {
        if (adc_toggle == 0) {
            start_adc0();
            adc_toggle = 1;
        } else {
            start_adc2();
            adc_toggle = 0;
        }
    }
}

/* The step function for success stage */
void setup_success_stage(void) {
    reset_segments();

    lock_for_lcd();

    LCDCmd(LCD_CLEAR);
    LCDGoto(1, 1);
    LCDStr("Unlocked; Press RB4 to lock!");

    unlock_for_lcd();

    LATD = 0x00;
    LATB = 0x00;
}

/* The step function for fail stage */
void setup_fail_stage(void) {
    reset_segments();

    lock_for_lcd();

    LCDCmd(LCD_CLEAR);
    LCDGoto(1, 1);
    LCDStr("You Failed!");

    unlock_for_lcd();

    LATD = 0x00;
    LATB = 0xFF;
}

/* The main setup function. This function will run if current_stage != stage_will_change */
void setup(void) {
    switch (current_stage) {
        case STAGE_SET_PASSWORD:
            // Set buttons
            enable_buttons_rb0();

            // Setup stage
            setup_set_password_stage();
            break;

        case STAGE_CHECK_PASSWORD:
            // Set buttons
            enable_buttons_rb0();

            // Set the 7 segment display
            enable_segment_display();

            // Setup stage
            setup_check_password_stage();

            // The TMR0 will start if there is an user interaction.
            break;

        case STAGE_SUCCESS:
            // Set buttons
            enable_buttons_rb4();

            // Disable timer
            disable_timer();

            // Disable heather and PORTB blink
            portb_blink_enabled = 0;
            blink_portb();
            disable_heather();

            // Setup stage
            setup_success_stage();
            break;

        case STAGE_FAIL:
            // Set buttons
            disable_buttons();

            // Disable timer
            disable_timer();

            // Disable heather and PORTB blink
            portb_blink_enabled = 0;
            blink_portb();
            disable_heather();

            // Setup stage
            setup_fail_stage();
            break;
    }
}

/* The main step function. This function will run in a loop. It runs the related
 * step function and updates the stage if current_stage != stage_will_change.
 */
void step(void) {
    if (current_stage != stage_will_change) {
        current_stage = stage_will_change;
        setup();
    }

    switch (current_stage) {
        case STAGE_INIT:
            startup_stage();
            stage_will_change = STAGE_SET_PASSWORD;
            break;
        case STAGE_SET_PASSWORD:
            step_set_password_stage();
            break;
        case STAGE_CHECK_PASSWORD:
            step_check_password_stage();
            break;
    }
}

/* Entrypoint and the main function*/
void main(void) {
    PLLEN = 0;//PLL disabled...

    // At initial, make all ports output and make all 0
    TRISA = 0x00;
    LATA = 0x00;

    TRISB = 0x00;
    LATB = 0x00;

    TRISD = 0x00;
    LATD = 0x00;

    // Disable all buttons and interrupts, if any remained
    disable_buttons();

    // Set the ADC module on
    enable_adc();

    // Init the LCD
    InitLCD();

    // Enable the interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    // Step
    while(1) step();

    return;
}
