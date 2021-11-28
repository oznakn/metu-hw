; OZAN AKIN - 2309599

; NAMING CONVENTION
; - VALUE: Value is the term that I use for variables that affect the output
;          directly.
;
; - COUNTER: Counters are scaling variables for timers to match the conditions.
;
; - STATE: State is the term that I use for variables that do not affect the
;          output directly.
;
; - STAGE: I generally use the term state for representing data that affects
;          other datas and does not used directly. Thus, I used the term "stage"
;          in the code to represent the write "stage", the review "stage", and
;	   the read "stage".
;
; - MODES:
;	 - Edit Mode: Alias for stage 1 (the write stage).
;        - Show Mode: Alias for stage 2 (the review stage) or 
;                     stage 3 (the read stage).
;

; configurations for PIC18F4620
#include <P18F4620.INC>

config OSC = HSPLL      ; Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
config FCMEN = OFF      ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
config IESO = OFF       ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
config PWRT = ON        ; Power-up Timer Enable bit (PWRT enabled)
config BOREN = OFF      ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config BORV = 3         ; Brown Out Reset Voltage bits (Minimum setting)

; CONFIG2H
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config WDTPS = 32768    ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
config CCP2MX = PORTC   ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
config PBADEN = OFF     ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
config LPT1OSC = OFF    ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config LVP = OFF        ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
config XINST = OFF      ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
config CP0 = OFF        ; Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
config CP1 = OFF        ; Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
config CP2 = OFF        ; Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
config CP3 = OFF        ; Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

; CONFIG5H
config CPB = OFF        ; Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
config CPD = OFF        ; Data EEPROM Code Protection bit (Data EEPROM not code-protected)

; CONFIG6L
config WRT0 = OFF       ; Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
config WRT1 = OFF       ; Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
config WRT2 = OFF       ; Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
config WRT3 = OFF       ; Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

; CONFIG6H
config WRTC = OFF       ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
config WRTB = OFF       ; Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
config WRTD = OFF       ; Data EEPROM Write Protection bit (Data EEPROM not write-protected)

; CONFIG7L
config EBTR0 = OFF      ; Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
config EBTR1 = OFF      ; Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
config EBTR2 = OFF      ; Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
config EBTR3 = OFF      ; Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

; CONFIG7H
config EBTRB = OFF      ; Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

variables udata_acs

; The constant values for 7 segment representations of letters (symbols)
 
LETTER_0 equ 03Fh
LETTER_1 equ 006h
LETTER_2 equ 05Bh
LETTER_3 equ 04Fh
LETTER_4 equ 066h
LETTER_5 equ 06Dh
LETTER_6 equ 07Dh
LETTER_7 equ 007h
LETTER_8 equ 07Fh
LETTER_9 equ 06Fh
 
LETTER_A equ 05Fh
LETTER_B equ 07Ch
LETTER_C equ 058h
LETTER_D equ 05Eh
LETTER_E equ 07Bh
LETTER_F equ 071h
LETTER_G equ 06Fh
LETTER_H equ 074h
LETTER_I equ 004h
LETTER_J equ 00Eh
LETTER_K equ 075h
LETTER_L equ 038h
LETTER_M equ 055h
LETTER_N equ 054h
LETTER_O equ 05Ch
LETTER_P equ 073h
LETTER_R equ 050h
LETTER_S equ 064h
LETTER_T equ 078h
LETTER_U equ 01Ch
LETTER_V equ 02Ah
LETTER_Y equ 06Eh
LETTER_Z equ 05Bh
LETTER_SPACE equ 000h
LETTER_NONE equ 008h

 
;;;;;;;;;;;;;;;;;;; KEY MAPPING ;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;; Column 1 ; Column 2  ; Column 3  ;;
;;   Row 1  ;;   1  (8) ;   2  (13) ;   3  (18) ;;
;;   Row 2  ;;   4  (4) ;   5  (9)  ;   6  (14) ;;
;;   Row 3  ;;   7  (2) ;   8  (7)  ;   9  (12) ;;
;;   Row 4  ;;   *  (1) ;   0  (6)  ;   #  (11) ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; The values written in paranthesis are ID's of keypad keys. These values are
; created for calculation purposes. Each cell follows the rule 5a + b, where
; b is 8 for row 1, 4 for row 2, 2 for row 3, and 1 for row 4. a is 0 
; for column 1, 1 for column 2, and 2 for column 3.
;
; With these calculation method, every key can be easily identified.
; Some examples:
; - Key 8, b = 2, a = 1, 5*1+2 = 7
; - Key 6, b = 4, a = 2, 5*2+4 = 14
;
 
; the values calculated using the method above.
KEY_0 equ 006h ; decimal value 6
KEY_1 equ 008h ; decimal value 8
KEY_2 equ 00Dh ; decimal value 13
KEY_3 equ 012h ; decimal value 18
KEY_4 equ 004h ; decimal value 4
KEY_5 equ 009h ; decimal value 9
KEY_6 equ 00Eh ; decimal value 14
KEY_7 equ 002h ; decimal value 2
KEY_8 equ 007h ; decimal value 7
KEY_9 equ 00Ch ; decimal value 12
KEY_STAR equ 001h ; decimal value 1
KEY_HASH equ 00Bh ; decimal value 11
KEY_NONE equ 0FFh ; decimal value 255, means no key

KEY_NONE_CHECK_FLAG equ 7 ; since the only key value that 7th bit is 1,
			  ; we can easily check the bit 7 to check a key
			  ; is empty or not
 
STAGE1 equ 002h ; represents stage 1, the write stage, binary:  0000 0010
STAGE2 equ 004h ; represents stage 2, the review stage, binary: 0000 0100
STAGE3 equ 008h ; represents stage 3, the read stage, binary:   0000 1000
 
STAGE1_CHECK_FLAG equ 1 ; bit to check the given value if stage 1 or not
STAGE2_CHECK_FLAG equ 2 ; bit to check the given value if stage 2 or not
STAGE3_CHECK_FLAG equ 3 ; bit to check the given value if stage 3 or not

SEGMENT1 equ 004h ; the value to write LATA to enable SEGMETN1, 0000 0100 
SEGMENT2 equ 008h ; the value to write LATA to enable SEGMETN1, 0000 1000 
SEGMENT3 equ 010h ; the value to write LATA to enable SEGMETN1, 0001 0000 
SEGMENT4 equ 020h ; the value to write LATA to enable SEGMETN1, 0010 0000 

; TIMER CALCULATION
; the timer set to 50 ms
; using the values 0FFh for LOWER byte, and 085h for HIGHER byte.
; 0xFFFF - 0x85FF = 31232 => 31232*16*5 = 2498560 cycle, approximately 49.97ms (16 is the prescaler)
;
; I've used only one timer for both timeout operation and countdown operation.
; necessary prescalers can be found below.
 
TMR0L_RESET equ 0FFh ; the LOWER byte of 16 bit timer value
TMR0H_RESET equ 085h ; the HIGHER byte of 16 bit timer value

; The default reset values for timers
 
SEGMENT_COUNTER_RESET equ 0FFh ; the loop count for waiting between segments
                               ; decimal value 255

KEYPAD_COUNTER_RESET equ 030h  ; the loop count for waiting after
                               ; setting the LATB register
			       ; decimal value 48
 
COUNTDOWN_RESET equ 014h ; 20 as decimal value, used for 20 seconds countdown

SHOW_SCROLL_COUNTER_RESET equ 00Ah ; decimal value 10, 10*50ms = 500 ms
				   ; used for scrolling effect in review stage
				   
ONE_SECOND_COUNTER_RESET equ 014h ; decimal value 20, 20*50ms = 1 second
                                  ; used for 20 seconds countdown
 
KEYPAD_CLICKED_KEY_COUNT_RESET equ 003h ; the default value for keypad count 
					; inputs to detect letters
					; the first value is 3, then 2, then 1,
					; and it loops between them
					; 3 means the first char, 2 means second,
					; and 1 means third.
					; Ex: for key 2, 3 => a, 2 => b, 1 => c

DEFAULT_LETTER_STATE equ 001h ; the default letter state, decimal value 1
                              ; letter state is current cursor in the 6 letters.
			      ; the letter state is shifted left to change it.
			      ; Thus the possible letter state values are 
			      ; 0x01, 0x02, 0x04, 0x08, 0x10, 0x20
			      
DEFAULT_SCROLL_STATE equ 001h ; the default scroll stage, decimal value 1
                              ; used during the read stage to create scroll
			      ; effect
			      ; 0x01 means scrolling to right, and 0x02 means
			      ; scrolling to left
 
MAX_SHOW_LETTER_STATE equ 004h ; the max value for letter state in show mode
                                

LETTER_STATE_LOCKED_FLAG equ 6 ; when the letter state reaches the value 0x40,
                               ; decimal value 64, that means the input process
			       ; is done, the system must switch to read stage
			       ; This is the bit to check in letter state to see
			       ; that the input process is done or not.
 
keypad_temp res 1 ; the temp register for keypad readings
rb4_temp res 1 ; the temp register for rb4 button readings
 
segment_counter res 1
keypad_counter res 1
 
stage_value res 1
 
letter1_value res 1
letter2_value res 1
letter3_value res 1
letter4_value res 1
letter5_value res 1
letter6_value res 1

segment1_value res 1
segment2_value res 1
segment3_value res 1
segment4_value res 1

countdown_counter res 1
timeout_counter res 1

countdown_value res 1

edit_letter_state res 1
edit_clicked_key_value res 1
edit_clicked_key_count_value res 1
edit_last_key_value res 1
 
show_letter_state res 1
show_clicked_key_value res 1
 
show_scroll_state res 1
show_scroll_counter res 1
 
org 0x0000
    GOTO init ; jump to init process

org 0x0008
    GOTO isr ; jump to interrupts section

calc_letter_from_key: ; calcs letter from key with given count
                      ; such as 2 times key '3' means the letter 'e'
		      ; this function is only used in write stage
		      
    ; This implementation uses jumpt tables for keys
    ; First, move the high byte of the jump table to
    ; PCLATH register to jump the correct place.
    MOVLW HIGH calc_letter_from_key_jtable
    MOVWF PCLATH
    
    ; Calculate the jumping point, the 'edit_clicked_key_value' variable holds
    ; the ID of the key using the key mapping table.
    ; Since GOTO is 2 word instruction, we need 4 times of the real key.
    MOVF edit_clicked_key_value, W
    ADDWF edit_clicked_key_value, W
    ADDWF edit_clicked_key_value, W
    ADDWF edit_clicked_key_value, W
    
    ADDWF PCL ; add value to PCL to jump the correct section
    
    calc_letter_from_key_jtable:
	GOTO calc_letter_from_key_none ; ID 0,  means nothing
	GOTO calc_letter_from_key_star ; ID 1,  the key star
	GOTO calc_letter_from_key_7    ; ID 2,  the key 7
	GOTO calc_letter_from_key_none ; ID 3,  means nothing
	GOTO calc_letter_from_key_4    ; ID 4,  the key 4
	GOTO calc_letter_from_key_none ; ID 5,  means nothing
	GOTO calc_letter_from_key_0    ; ID 6,  the key 0
	GOTO calc_letter_from_key_8    ; ID 7,  the key 8
	GOTO calc_letter_from_key_1    ; ID 8,  the key 1
	GOTO calc_letter_from_key_5    ; ID 9,  the key 5
	GOTO calc_letter_from_key_none ; ID 10, means nothing
	GOTO calc_letter_from_key_hash ; ID 11, the key hash
	GOTO calc_letter_from_key_9    ; ID 12, the key 9
	GOTO calc_letter_from_key_2    ; ID 13, the key 2
	GOTO calc_letter_from_key_6    ; ID 14, the key 6
	GOTO calc_letter_from_key_none ; ID 15, means nothing
	GOTO calc_letter_from_key_none ; ID 16, means nothing
	GOTO calc_letter_from_key_none ; ID 17, means nothing
	GOTO calc_letter_from_key_3    ; ID 18, the key 3

    ; These sections below finds the correct letters using the variable
    ; edit_clicked_key_count_value. The result is saved to WREG and returned
    ; using the RETLW instruction.

    ; These values do not affect the process in write stage
    ; put LETTER_NONE to WREG and return
    calc_letter_from_key_none:
    calc_letter_from_key_star:
    calc_letter_from_key_hash:
    calc_letter_from_key_0:
    calc_letter_from_key_1:
	RETLW LETTER_NONE

    calc_letter_from_key_2: ; For key 2, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011 
	RETLW LETTER_C
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011 
	RETLW LETTER_B
	RETLW LETTER_A

    calc_letter_from_key_3: ; For key 3, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_F
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_E
	RETLW LETTER_D

    calc_letter_from_key_4: ; For key 4, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_I
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_H
	RETLW LETTER_G

    calc_letter_from_key_5: ; For key 5, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_L
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_K
	RETLW LETTER_J

    calc_letter_from_key_6: ; For key 6, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_O
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_N
	RETLW LETTER_M

    calc_letter_from_key_7: ; For key 7, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_S
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_R
	RETLW LETTER_P

    calc_letter_from_key_8: ; For key 8, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_V
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_U
	RETLW LETTER_T

    calc_letter_from_key_9: ; For key 9, calculate and find the correct value, save it to WREG and return
	BTFSS edit_clicked_key_count_value, 1 ; checks the value is not 0000 0010 and 0000 0011
	RETLW LETTER_SPACE
	BTFSS edit_clicked_key_count_value, 0 ; checks the value is not 0000 0001 and 0000 0011
	RETLW LETTER_Z
	RETLW LETTER_Y

	
	
get_countdown_segment2: ; find the second digit of the countdown value using a
                        ; jump table, used only in write stage.
    
    ; This implementation uses jumpt tables for keys
    ; First, move the high byte of the jump table to
    ; PCLATH register to jump the correct place.    
    MOVLW HIGH get_countdown_segment2_jtable
    MOVWF PCLATH
    
    ; Calculate the correct jump point. Since the RETLW instruction is a one
    ; word instruction, 2 times the given values is enough to jump.
    MOVF countdown_value, W
    ADDWF countdown_value, W
    
    ADDWF PCL ; Add the offset to PCL and jump.
    
    ; This jump table finds the second digit of the given value, 
    ; the values are in the range 0-20 (inclusive).
    get_countdown_segment2_jtable:
	RETLW LETTER_0 ; second digit of 0
	RETLW LETTER_1 ; second digit of 1
	RETLW LETTER_2 ; second digit of 2
	RETLW LETTER_3 ; second digit of 3
	RETLW LETTER_4 ; second digit of 4
	RETLW LETTER_5 ; second digit of 5
	RETLW LETTER_6 ; second digit of 6
	RETLW LETTER_7 ; second digit of 7
	RETLW LETTER_8 ; second digit of 8
	RETLW LETTER_9 ; second digit of 9
	RETLW LETTER_0 ; second digit of 10
	RETLW LETTER_1 ; second digit of 11
	RETLW LETTER_2 ; second digit of 12
	RETLW LETTER_3 ; second digit of 13
	RETLW LETTER_4 ; second digit of 14
	RETLW LETTER_5 ; second digit of 15
	RETLW LETTER_6 ; second digit of 16
	RETLW LETTER_7 ; second digit of 17
	RETLW LETTER_8 ; second digit of 18
	RETLW LETTER_9 ; second digit of 19
	RETLW LETTER_0 ; second digit of 20


get_countdown_segment1: ; find the first digit of the countdown value using a
                        ; jump table, used only in write stage.

    ; Normally the common way of detecting the first digit is subtraction the
    ; value 10 from the variable to find which range is the value in. 
    ; Ranges are (0-9, 10-19, 20). However, picsimlab does not updates the N bit
    ; of status register after doing a SUBWL instruction.
    ; To accomplish the issue, I first multiplied the value with -1, and then
    ; added the value 10 to find the correct range. However, since 0 is a positive
    ; integer from the aspect of the N bit of STATUS register, first I have to add
    ; 1 to the countdown value to make sure that the selected range is the
    ; correct one.
  
    MOVF countdown_value, W
    INCF WREG       ; add 1 to the countdown value
    SUBLW 000h      ; simple way of multiplying a value with -1, subtracting the
                    ; the same value from 0 is much more easier.
    
    ADDLW 00Ah      ; add 10 to the counter value
    BTFSS STATUS, N ; if the value is postive, that means
    RETLW LETTER_0  ; the value is in range (0-9)
    
    ADDLW 00Ah      ; add 10 to the counter value again
    BTFSS STATUS, N ; if the value is postive, that means
    RETLW LETTER_1  ; the value is in range (10-19)
    
    RETLW LETTER_2  ; the value is still negative, that means our value was 20
                    ; adding 1 at the begging helped us to detect the first
		    ; digit as 2 correctly. If we hadn't add 1 at the begging,
		    ; the result would be 0. Since it is a positive integer,
		    ; the function would guess the first digit as 1.


isr: ; the high priority interrupt function.
    
    BTFSS INTCON, 0 ; check the interrupt is a timer interrupt or not.
    GOTO isr_timer  ; if it is a timer interrupt, then jump to the related section,
                    ; if not, continue with the button interrupt section.
    
    isr_button:
	    MOVFF PORTB, rb4_temp ; check the state of the RB4.

	    BTFSS rb4_temp, 4   ; we have to trigger the action on release of
	    GOTO isr_button_end ; the RB4, thus check the rb4 value

	    BTFSC stage_value, STAGE1_CHECK_FLAG ; RB4 is release, now find the
	    GOTO isr_button_stage1               ; correct section for stages.
	    BTFSC stage_value, STAGE2_CHECK_FLAG ; stage check flags are
	    GOTO isr_button_stage2               ; easy ways to check the stage.
	    GOTO isr_button_end                  ; stage 3 (the read stage) does
	                                         ; not have an interrupt.
	    
	isr_button_stage1:      ; interrupt occured during the stage 1
	    MOVLW STAGE2        ; let's switch to stage 2 (the review stage)
	    CALL switch_stage   ; using the 'switch_stage' function
	    
	    GOTO isr_button_end ; jump to the next section
	    
	isr_button_stage2:      ; interrupt occured during the stage 2
	    MOVLW STAGE1        ; let's switch to stage 2 (the review stage)
	    CALL switch_stage   ; using the 'switch_stage' function
	    
	    GOTO isr_button_end ; jump to the next section
	 
	isr_button_end:
	    MOVFF rb4_temp, LATB ; we have to write LATB to make sure that
	                         ; the interrupt is handled correctly.

	    BCF INTCON, 0        ; clear the rb interrupt bit of INTCON register
	                         ; to handle the interrupt correctly.

	    RETFIE 1             ; RETFIE is the instruction that we use for
	                         ; returning from interrupts. 1 as the paremter
				 ; means that the return operation must also
				 ; restore registers STATUS, WREG and PCLATH.
				 
    isr_timer: ; timer section of the interrupt function
    
	BTFSC stage_value, STAGE3_CHECK_FLAG ; since we do not have a countdown
	GOTO isr_timer_stage_3               ; in the read stage, jump directly.

	isr_timer_countdown: ; the interrupt section related to the countdown
    
	    DECFSZ countdown_counter       ; decrement countdown counter
	    GOTO isr_timer_countdown_end   ; countdown counter is not zero, skip
	    
	    MOVLW ONE_SECOND_COUNTER_RESET ; the countdown counter hit 0
	    MOVWF countdown_counter        ; reset the value

	    DECF countdown_value           ; since the countdown counter hit 0,
	                                   ; 1 second passed, decrease the
					   ; countdown value.
	isr_timer_countdown_end:
    
	isr_timer_stage_1: ; interrupt timer section for stage 1
    
	    BTFSS stage_value, STAGE1_CHECK_FLAG ; check that we are in stage 1
	    GOTO isr_timer_stage_1_end           ; otherwise jump to next section.

	    TSTFSZ timeout_counter         ; decrement the timeout counter
	    GOTO isr_timer_stage_1_timeout ; timeout counter is not zero,
	                                   ; that means there is an active timeout
					   ; jump to the related section.
	    GOTO isr_timer_stage_1_end     ; timeout counter is zero, jump to next section.
	    
	isr_timer_stage_1_timeout:
	    DECFSZ timeout_counter     ; decrease the timeout counter
	    GOTO isr_timer_stage_1_end ; timeout counter is not zero, jump to the next section
	    
	    ; actions for timeout counter happened
	    ; we do not reset the timeout counter here since we don't have a
	    ; pressed button yet.
	    
	    MOVLW KEY_NONE            ; set last key to none, to remove
	    MOVWF edit_last_key_value ; ambiguity.
	    
	    BTFSS edit_letter_state, LETTER_STATE_LOCKED_FLAG ; check we are in lock state, which means
	                                                      ; the stage will be changed to stage 3
							      ; at the end of the main loop.
	    RLNCF edit_letter_state, F ; since the stage is not in lock stage, shift letters to make space.

	isr_timer_stage_1_end:
	    GOTO isr_timer_end ; end timer section
	    
	isr_timer_stage_3: ; timer interrupt section for stage 3
    
	    DECFSZ show_scroll_counter      ; decrement the scroll counter
	    GOTO isr_timer_stage_3_end      ; if the scroll counter is not zero,
	                                    ; jump to next section.
	    
	    MOVLW SHOW_SCROLL_COUNTER_RESET ; reset the scroll
	    MOVWF show_scroll_counter       ; scroll counter

	    BTFSC show_scroll_state, 0      ; check the state using bit 0. 
	                                    ; if the value is 0x01, that means we are 
					    ; scrolling to right otherwise we are 
					    ; scrolling to left.
	    GOTO isr_timer_stage_3_right
	    
	isr_timer_stage_3_left:          ; shift to left code
	    BTFSC show_letter_state, 0   ; check the letter state, if we are in
	    GOTO isr_timer_stage_3_right ; the left edge, start scrolling to right.
	    
	    MOVLW 002h                   ; make sure that the show_scroll-state
	    MOVWF show_scroll_state      ; variable has the correct value.

	    RRNCF show_letter_state      ; shift the 'show_letter_state' in the
	                                 ; correct direction.

	    GOTO isr_timer_stage_3_end   ; jump to the next section.

	isr_timer_stage_3_right:         ; shift to right code
	    MOVF show_letter_state, W    ; check the letter state, if we 
	    SUBLW MAX_SHOW_LETTER_STATE  ; are in the right edge, start
	    BTFSC STATUS, Z              ; scrolling to right. MAX_SHOW_LETTER_STATE
	    GOTO isr_timer_stage_3_left  ; constant is used to determine that we
	                                 ; are in the right edge or not.
	    
	    MOVLW 001h                   ; make sure that the show_scroll-state
	    MOVWF show_scroll_state      ; variable has the correct value.

	    RLNCF show_letter_state      ; shift the 'show_letter_state' in the
                                         ; correct direction.
					 
	    GOTO isr_timer_stage_3_end   ; jump to the next section.
	    
	isr_timer_stage_3_end:
    
	isr_timer_end:
	    BCF INTCON, 2     ; clear the timer overflow bit of INTCON
	                      ; to handle the interrupt correctly.

	    MOVLW TMR0H_RESET ; reset the timer variables
	    MOVWF TMR0H
	    MOVLW TMR0L_RESET
	    MOVWF TMR0L

	    RETFIE 1          ; RETFIE is the instruction that we use for
	                      ; returning from interrupts. 1 as the paremter
		              ; means that the return operation must also
			      ; restore registers STATUS, WREG and PCLATH.

switch_stage: ; switch stage to desired one. 
              ; the stage value to switch is in the WREG register.
	      
    switch_stage_to_1:
	BTFSS WREG, STAGE1_CHECK_FLAG ; check the desired stage is 1 (write stage)
	GOTO switch_stage_to_2        ; otherwise, jump to next section.
	
	MOVLW STAGE1      ; set the stage value to 1
	MOVWF stage_value
	
	; Reset the variables for the stage 1
	MOVLW KEY_NONE
	MOVWF edit_clicked_key_value
	MOVWF edit_last_key_value

	MOVLW KEYPAD_CLICKED_KEY_COUNT_RESET
	MOVWF edit_clicked_key_count_value

	GOTO switch_stage_end ; jump to the next section.
	
    switch_stage_to_2:
	BTFSS WREG, STAGE2_CHECK_FLAG ; check the desired stage is 1 (write stage)
	GOTO switch_stage_to_3        ; otherwise, jump to next section.
	
	MOVLW STAGE2      ; set the stage value to 2
	MOVWF stage_value
	
	; Reset the variables for the stage 2
	
	CLRF timeout_counter ; clear timeout to remove ambiguity
	
	; Discard the last unsaved letter.
	MOVLW LETTER_NONE
	
	BTFSC edit_letter_state, 0
	MOVWF letter1_value
	BTFSC edit_letter_state, 1
	MOVWF letter2_value
	BTFSC edit_letter_state, 2
	MOVWF letter3_value
	BTFSC edit_letter_state, 3
	MOVWF letter4_value
	BTFSC edit_letter_state, 4
	MOVWF letter5_value
	BTFSC edit_letter_state, 5
	MOVWF letter6_value
	
	MOVLW DEFAULT_LETTER_STATE
	MOVWF show_letter_state

	GOTO switch_stage_end ; jump to the next section.
	
    switch_stage_to_3:
	BTFSS WREG, STAGE3_CHECK_FLAG ; check the desired stage is 1 (write stage)
	GOTO switch_stage_end         ; otherwise, jump to next section.
	
	MOVLW STAGE3      ; set the stage value to 3
	MOVWF stage_value
	
	CLRF timeout_counter ; clear timeout to remove ambiguity
	
	; Discard the last unsaved letter.
	MOVLW LETTER_NONE
	
	BTFSC edit_letter_state, 0
	MOVWF letter1_value
	BTFSC edit_letter_state, 1
	MOVWF letter2_value
	BTFSC edit_letter_state, 2
	MOVWF letter3_value
	BTFSC edit_letter_state, 3
	MOVWF letter4_value
	BTFSC edit_letter_state, 4
	MOVWF letter5_value
	BTFSC edit_letter_state, 5
	MOVWF letter6_value
	
	; Reset the variables for the stage 3
	
	MOVLW SHOW_SCROLL_COUNTER_RESET
	MOVWF show_scroll_counter
	
	MOVLW DEFAULT_SCROLL_STATE
	MOVWF show_scroll_state

	GOTO switch_stage_end ; jump to the next section.
	
    switch_stage_end:
	RETURN

	    
on_stage_timeout: ; when the counter hits 0, change stage to the 3 (read stage)
    ; clear these values to remove ambiguity.
    
    CLRF countdown_counter
    CLRF timeout_counter
    
    CLRF countdown_value
    
    MOVLW STAGE3      ; change stage to 3 (read stage)
    CALL switch_stage ; using the 'switch_stage' function.
    
    RETURN


set_edit_mode_segments: ; the segment values creation function using the 
                        ; state variables.
			; edit mode is an alias for stage 1.
			
    CALL get_countdown_segment1 ; set the segment 1 with the first
    MOVWF segment1_value        ; digit of the countdown.
    
    CALL get_countdown_segment2 ; set the segment 2 with the second
    MOVWF segment2_value        ; digit of the countdown.
    
    MOVLW LETTER_NONE           ; set the third digit using the 'edit_letter_state' cursor variable
    
    BTFSC edit_letter_state, 0
    MOVF letter1_value, W
    BTFSC edit_letter_state, 1
    MOVF letter2_value, W
    BTFSC edit_letter_state, 2
    MOVF letter3_value, W
    BTFSC edit_letter_state, 3
    MOVF letter4_value, W
    BTFSC edit_letter_state, 4
    MOVF letter5_value, W
    BTFSC edit_letter_state, 5
    MOVF letter6_value, W
    
    MOVWF segment4_value
        
    MOVLW LETTER_NONE            ; set the forth digit using the 'edit_letter_state' cursor variable
    
    BTFSC edit_letter_state, 1
    MOVF letter1_value, W
    BTFSC edit_letter_state, 2
    MOVF letter2_value, W
    BTFSC edit_letter_state, 3
    MOVF letter3_value, W
    BTFSC edit_letter_state, 4
    MOVF letter4_value, W
    BTFSC edit_letter_state, 5
    MOVF letter5_value, W
    
    MOVWF segment3_value
    
    RETURN
    
set_show_mode_segments: ; the segment values creation function using the 
                        ; state variables.
			; show mode is an alias for stage 2 and stage 3.
			
    MOVLW LETTER_NONE          ; set the first digit using the 'show_letter_state' cursor variable

    BTFSC show_letter_state, 0
    MOVF letter1_value, W
    BTFSC show_letter_state, 1
    MOVF letter2_value, W
    BTFSC show_letter_state, 2
    MOVF letter3_value, W
    
    MOVWF segment1_value
    
    MOVLW LETTER_NONE          ; set the second digit using the 'show_letter_state' cursor variable
    
    BTFSC show_letter_state, 0
    MOVF letter2_value, W
    BTFSC show_letter_state, 1
    MOVF letter3_value, W
    BTFSC show_letter_state, 2
    MOVF letter4_value, W
    
    MOVWF segment2_value
    

    MOVLW LETTER_NONE          ; set the third digit using the 'show_letter_state' cursor variable

    BTFSC show_letter_state, 0
    MOVF letter3_value, W
    BTFSC show_letter_state, 1
    MOVF letter4_value, W
    BTFSC show_letter_state, 2
    MOVF letter5_value, W
    
    MOVWF segment3_value
    
    
    MOVLW LETTER_NONE          ; set the fourth digit using the 'show_letter_state' cursor variable
    
    BTFSC show_letter_state, 0
    MOVF letter4_value, W
    BTFSC show_letter_state, 1
    MOVF letter5_value, W
    BTFSC show_letter_state, 2
    MOVF letter6_value, W
    
    MOVWF segment4_value
     
    RETURN
    

wait_keypad: ; wait after setting the LATB register to remove the ambiguity.
    DECFSZ keypad_counter
    GOTO wait_keypad

    MOVLW KEYPAD_COUNTER_RESET ; reset the keypad_counter
    MOVWF keypad_counter
    RETURN
    

on_letter_push: ; replaces the current cursor letter with the input.
                ; the letters are calculated using the 
		; 'edit_clicked_key_value' and 'edit_clicked_key_count_value' variables
		
    CALL calc_letter_from_key ; calculate the letter from key and count, the result
                              ; will be in WREG.
			      
    ; edit the correct variable
    BTFSC edit_letter_state, 0
    MOVWF letter1_value
    BTFSC edit_letter_state, 1
    MOVWF letter2_value
    BTFSC edit_letter_state, 2
    MOVWF letter3_value
    BTFSC edit_letter_state, 3
    MOVWF letter4_value
    BTFSC edit_letter_state, 4
    MOVWF letter5_value
    BTFSC edit_letter_state, 5
    MOVWF letter6_value
    
    MOVLW ONE_SECOND_COUNTER_RESET ; start and activate the timeout timer
    MOVWF timeout_counter
    
    RETURN
    
stage1_on_key_release: ; on key release listener function for stage 1 (write stage)
    
	BTFSC edit_last_key_value, KEY_NONE_CHECK_FLAG ; check the last key is none
	GOTO stage1_on_key_release_new                 ; if none, just replace the current cursor
	
	MOVF edit_last_key_value, W     ; check the last pressed key and current
	SUBWF edit_clicked_key_value, W ; key, if the keys are the same, jump
	BTFSC STATUS, Z                 ; to the related section.
	GOTO stage1_on_key_release_same

    stage1_on_key_release_shift: ; The current and last pressed keys are different,
                                 ; first shift the key, and then replace the current cursor.
				 
	CLRF timeout_counter     ; clear the timeout timer to remove the ambiguity.
    
	RLNCF edit_letter_state, F ; shift the cursor
	
	BTFSS edit_letter_state, LETTER_STATE_LOCKED_FLAG ; check we've locked
	GOTO stage1_on_key_release_new                    ; the letters or not.
	              
	MOVLW STAGE3     ; the letters are locked now, change stage to 3 (read stage).
	CALL switch_stage
	
	RETURN
    
    stage1_on_key_release_new: ; the last pressed key is none, thus we don't have to shift
	MOVLW KEYPAD_CLICKED_KEY_COUNT_RESET ; first reset the count variable
	MOVWF edit_clicked_key_count_value
	
	CALL on_letter_push ; replace the current cursor with given input
	
	MOVFF edit_clicked_key_value, edit_last_key_value ; set our new pressed key as last pressed one.
	
	RETURN
	
    stage1_on_key_release_same: ; the last pressed key and the current one are the same
	DECFSZ edit_clicked_key_count_value ; decrease the count variable
	GOTO stage1_on_key_release_same_end ; if the result is not zero, jump to the on_letter_push function.
	
	MOVLW KEYPAD_CLICKED_KEY_COUNT_RESET ; the count variable is zero, rollback
	MOVWF edit_clicked_key_count_value   ; to the first one.

    stage1_on_key_release_same_end:
	CALL on_letter_push ; replace the current cursor with given input
	
	RETURN


stage1_keypad_check: ; keypad check function for stage 1 (write stage)
    
    stage1_keypad_check_1:
	MOVLW 006h          ; set the LATB register, make the first column LOW
	MOVWF LATB
	
	CALL wait_keypad    ; wait some time to remove ambiguity.
	
	MOVF PORTD, W       ; run the keypad ID detect algorithm.
	COMF WREG, W        ; this part calculates the variable b.
	ANDLW 00Fh          ; first COMF the value since 1 means released in picsimlab,
	MOVWF keypad_temp   ; an then filter the result and calculate the variable b.
	
	MOVF keypad_temp, W ; check the b variable is it 8 or not,
	SUBLW 008h          ; if it 8, that means the key 1, which
	BTFSC STATUS, Z     ; is unresponsive in the write stage,
	RETURN              ; no action is necessary, return from the function.
	
	MOVF keypad_temp, W ; check the b variable is it 1 or not,
	SUBLW 001h          ; if it 1, that means the key star, which
	BTFSC STATUS, Z     ; is unresponsive in the write stage,
	RETURN              ; no action is necessary, return from the function.
	
	MOVF keypad_temp, W

	TSTFSZ WREG                        ; check there is an input or not, if there 
	GOTO stage1_keypad_check_success_1 ; is an input jump to the related section.

    stage1_keypad_check_2:
	MOVLW 005h          ; set the LATB register, make the second column LOW
	MOVWF LATB

	CALL wait_keypad    ; wait some time to remove ambiguity.
	
	MOVF PORTD, W       ; run the keypad ID detect algorithm.
	COMF WREG, W        ; this part calculates the variable b.
	ANDLW 00Fh          ; first COMF the value since 1 means released in picsimlab,
	MOVWF keypad_temp   ; an then filter the result and calculate the variable b.
	
	MOVF keypad_temp, W ; check the b variable is it 1 or not,
	SUBLW 001h          ; if it 1, that means the key 0, which
	BTFSC STATUS, Z     ; is unresponsive in the write stage,
	RETURN              ; no action is necessary, return from the function.
	
	MOVF keypad_temp, W

	TSTFSZ WREG                        ; check there is an input or not, if there 
	GOTO stage1_keypad_check_success_2 ; is an input jump to the related section.
	
    stage1_keypad_check_3:
	MOVLW 003h          ; set the LATB register, make the third column LOW
	MOVWF LATB

	CALL wait_keypad    ; wait some time to remove ambiguity.
	
	MOVF PORTD, W       ; run the keypad ID detect algorithm.
	COMF WREG, W        ; this part calculates the variable b.
	ANDLW 00Fh          ; first COMF the value since 1 means released in picsimlab,
	MOVWF keypad_temp   ; an then filter the result and calculate the variable b.
	
	MOVF keypad_temp, W ; check the b variable is it 1 or not,
	SUBLW 001h          ; if it 1, that means the key hash, which
	BTFSC STATUS, Z     ; is unresponsive in the write stage,
	RETURN              ; no action is necessary, return from the function.
	
	MOVF keypad_temp, W

	TSTFSZ WREG                        ; check there is an input or not, if there 
	GOTO stage1_keypad_check_success_3 ; is an input jump to the related section.
	
    stage1_keypad_check_noinput:
	BTFSS edit_clicked_key_value, KEY_NONE_CHECK_FLAG ; check a key released or not
	CALL stage1_on_key_release                        ; if a key released, run the related function.
    
	MOVLW KEY_NONE               ; set the current key as none
	MOVWF edit_clicked_key_value
	
	RETURN
    
    stage1_keypad_check_success_3:
	ADDLW 005h ; calculate the variable a and add
	
    stage1_keypad_check_success_2:
	ADDLW 005h ; calculate the variable a and add
	
    stage1_keypad_check_success_1:
	MOVWF edit_clicked_key_value ; move the calculated value to related variable
	
	RETURN
	
	
stage2_on_key_release:
	MOVF show_clicked_key_value, W  ; check the clicked value, since only star and hash
	SUBLW KEY_STAR                  ; keys are responsive in the stage 2,
	BTFSC STATUS, Z                 ; check them one by one.
	GOTO stage2_on_key_release_star

	MOVF show_clicked_key_value, W  ; check the clicked value, since only star and hash
	SUBLW KEY_HASH                  ; keys are responsive in the stage 2,
	BTFSC STATUS, Z                 ; check them one by one.
	GOTO stage2_on_key_release_hash

	RETURN

    stage2_on_key_release_star:
	BTFSC show_letter_state, 0  ; shift cursor by one if 
	RETURN                      ; we are not in the left edge

	RRNCF show_letter_state     ; shift cursor

	RETURN

    stage2_on_key_release_hash:
	MOVF show_letter_state, W   ; shift cursor by one if 
	SUBLW MAX_SHOW_LETTER_STATE ; we are not in the right edge
	BTFSC STATUS, Z
	RETURN

	RLNCF show_letter_state     ; shift cursor

	RETURN
    

stage2_keypad_check:
    stage2_keypad_check_1:
	MOVLW 006h          ; set the LATB register, make the first column LOW
	MOVWF LATB
	
	CALL wait_keypad    ; wait some time to remove ambiguity.
	
	MOVF PORTD, W       ; run the keypad ID detect algorithm.
	COMF WREG, W        ; this part calculates the variable b.
	ANDLW 00Fh          ; first COMF the value since 1 means released in picsimlab,
	MOVWF keypad_temp   ; an then filter the result and calculate the variable b.
	
	MOVF keypad_temp, W ; check the pressed key, if the key pressed is not star,
	SUBLW 001h          ; jump to the next section
	BTFSS STATUS, Z
	GOTO stage2_keypad_check_3
	
	MOVF keypad_temp, W

	TSTFSZ WREG
	GOTO stage2_keypad_check_success_1 ; the pressed is not empty, i.e. it is star
	                                   ; jump to the related section
	
    stage2_keypad_check_3:
	MOVLW 003h
	MOVWF LATB

	CALL wait_keypad

	MOVF PORTD, W       ; run the keypad ID detect algorithm.
	COMF WREG, W        ; this part calculates the variable b.
	ANDLW 00Fh          ; first COMF the value since 1 means released in picsimlab,
	MOVWF keypad_temp   ; an then filter the result and calculate the variable b.
	
	
	MOVF keypad_temp, W ; check the pressed key, if the key pressed is not hash,
	SUBLW 001h          ; jump to the next section
	BTFSS STATUS, Z
	GOTO stage2_keypad_check_noinput
	
	MOVF keypad_temp, W
 
	TSTFSZ WREG                        ; the pressed is not empty, i.e. it is hash
	GOTO stage2_keypad_check_success_3 ; jump to the related section

    stage2_keypad_check_noinput:
	BTFSS show_clicked_key_value, KEY_NONE_CHECK_FLAG ; check a key released or not.
	CALL stage2_on_key_release                        ; if a key released, call the necessary function.
    
	MOVLW KEY_NONE              ; set the current key as none
	MOVWF show_clicked_key_value
	
	RETURN
    
    stage2_keypad_check_success_3:
	ADDLW 005h ; calculate the variable a and add
	ADDLW 005h

    stage2_keypad_check_success_1:
	MOVWF show_clicked_key_value ; move the calculated value to related variable

	RETURN

wait_segment: ; waits between segments in order to see them properly.
    DECFSZ segment_counter
    GOTO wait_segment

    MOVLW SEGMENT_COUNTER_RESET
    MOVWF segment_counter
    RETURN

main: ; main loop of the program. 
    
    main_stage_1: ; stage 1 actions.
	BTFSS stage_value, STAGE1_CHECK_FLAG ; run only for stage 1
	GOTO main_stage_2                    ; otherwise jump to next section

	CALL stage1_keypad_check    ; check keypad inputs
	CALL set_edit_mode_segments ; set the segments properly
	
	GOTO main_stage_end
    main_stage_2: ; stage 2 actions.
	BTFSS stage_value, STAGE2_CHECK_FLAG ; run only for stage 2
	GOTO main_stage_3                    ; otherwise jump to next section

	CALL stage2_keypad_check    ; check keypad inputs
	CALL set_show_mode_segments ; set the segments properly
	
	GOTO main_stage_end
    main_stage_3: ; stage 3 actions.
	BTFSS stage_value, STAGE3_CHECK_FLAG ; run only for stage 3
	GOTO main_stage_end                  ; otherwise jump to next section

	CALL set_show_mode_segments ; set the segments properly

    main_stage_end:
    
    
    ; We use PORTD as both output and input, we keep PORTD as input for keypad.
    ; however, we also set it open to show the segment values.
    
    CLRF TRISD                 ; set the PORTD as output
    
    MOVLW SEGMENT1             ; set the LATA register to select the segment
    MOVWF LATA                 ; we select the first segment
    MOVFF segment1_value, LATD ; wrote the first segment value
    
    CALL wait_segment          ; wait some time to show the segment LEDS properly.
    
    MOVLW SEGMENT2             ; set the LATA register to select the segment
    MOVWF LATA                 ; we select the second segment
    MOVFF segment2_value, LATD ; wrote the second segment value
    
    CALL wait_segment          ; wait some time to show the segment LEDS properly.
    
    MOVLW SEGMENT3             ; set the LATA register to select the segment 
    MOVWF LATA                 ; we select the third segment
    MOVFF segment3_value, LATD ; wrote the third segment value
    
    CALL wait_segment          ; wait some time to show the segment LEDS properly.
    
    MOVLW SEGMENT4             ; set the LATA register to select the segment 
    MOVWF LATA                 ; we select the forth segment
    MOVFF segment4_value, LATD ; wrote the forth segment value
    
    CALL wait_segment          ; wait some time to show the segment LEDS properly.
    
    CLRF LATA                  ; clear LATA to remove ambiguity
    CLRF LATD                  ; clear LATD to remove ambiguity
    COMF TRISD, F              ; set PORTD as input again
    
    ; this function checks we are in the locked state, or the countdown
    ; hit 0 or not.
    ; if any of the conditions occur, we change the stage to the stage 3 (read stage)
    
    main_set_stage3_check_1:
	TSTFSZ countdown_value       ; check the countdown is 0 or not
	GOTO main_set_stage3_check_2 ; countdown is not 0, check the locked state
	GOTO main_set_stage3_op      ; countdown is 0, switch to stage 3.

    main_set_stage3_check_2:
	BTFSS edit_letter_state, LETTER_STATE_LOCKED_FLAG ; check the locked stage
	GOTO main_set_stage3_end                          ; the letters are not locked,
	                                                  ; jump to the next section.

    main_set_stage3_op:
	BTFSS stage_value, STAGE3_CHECK_FLAG ; check we are in stage 3 or not,
	CALL on_stage_timeout                ; if we are not in stage 3,
	                                     ; change to stage 3 using the 
					     ; 'on_stage_timeout' function.
	
    main_set_stage3_end:
    
    GOTO main ; rerun the main again, i.e. main loop
    

    
wait_for_rb3: ; wait for button rb3 to push, and then release.
    wait_for_rb3_wait_push:
	BTFSC PORTB, 3
	GOTO wait_for_rb3_wait_push
	
    wait_for_rb3_wait_release:
	BTFSS PORTB, 3
	GOTO wait_for_rb3_wait_release

	RETURN
    
init: ; init section, the enter point of the program
    MOVLW 00Fh
    MOVWF ADCON1
    
    CLRF INTCON  ; disable interrupts
    CLRF INTCON2 ; disable interrupts

    CLRF TRISA   ; set the PORTA as output, for seven segments.
    
    MOVLW 0FFh   ; set the PORTD as input, for keypad.
    MOVWF TRISD  ; later set this port to output for seven segments.
    
    MOVLW 018h   ; set bits 3 and 4 as input for RB3 and RB4.
    MOVWF TRISB
    
    MOVLW 000h   ; wrote 0 to LATB to remove ambiguity.
    MOVWF LATB
    
    BCF INTCON2, 7 ; enable pull-ups.
    CLRF PORTB     ; clear PORTB to remove ambiguity.
   
    MOVLW 028h ; 0010 1000, then will be 1110 1000, enable rb and timer0 interrupt
    MOVWF INTCON
    
    MOVLW 003h ; 0000 0011, then will be 1000 0011, 1:16 prescale, set to 50ms
    MOVWF T0CON

    BSF INTCON, 7 ; enable GIE
    BSF INTCON, 6 ; enable PIE
    
    ; general values
    
    CLRF segment1_value ; clear the segment1
    CLRF segment2_value ; clear the segment2
    CLRF segment3_value ; clear the segment3
    CLRF segment4_value ; clear the segment4
    
    ; set all letters to none
    MOVLW LETTER_NONE
    MOVWF letter1_value
    MOVWF letter2_value
    MOVWF letter3_value
    MOVWF letter4_value
    MOVWF letter5_value
    MOVWF letter6_value
    
    ; reset the counter related to the 20 countdown.
    MOVLW COUNTDOWN_RESET
    MOVWF countdown_value ; the real number 20

    MOVLW ONE_SECOND_COUNTER_RESET
    MOVWF countdown_counter ; one second counter for prescaling 

    ; reset the necessary counter for wait functions.
    MOVLW SEGMENT_COUNTER_RESET
    MOVWF segment_counter
    
    MOVLW KEYPAD_COUNTER_RESET
    MOVWF keypad_counter
       
    CALL wait_for_rb3 ; wait for RB3 to pushed and then released.
    
    MOVLW DEFAULT_LETTER_STATE ; set the letter state (cursor to default)
    MOVWF edit_letter_state
    	
    MOVLW DEFAULT_LETTER_STATE ; set the letter state (cursor to default)
    MOVWF show_letter_state
    
    MOVLW STAGE1      ; set stage to 1 (write stage)
    CALL switch_stage
    
    MOVLW TMR0H_RESET ; set the timer0 values
    MOVWF TMR0H       ; for 50 ms
    MOVLW TMR0L_RESET
    MOVWF TMR0L

    BSF T0CON, 7 ; start timer0

    GOTO main

end