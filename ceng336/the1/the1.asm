LIST P=18F4620
    
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
 
LINE_1 equ LATD
LINE_2 equ LATC
LINE_3 equ LATA

BUTTON_UP equ 2
BUTTON_DOWN equ 1
BUTTON_TOGGLE equ 0
BUTTON_CONFIRM equ 3
 
BUTTON_THRESHOLD equ 0FAh

STATE_RELEASED_BIT equ 0
STATE_JUST_PRESSED_BIT equ 1
STATE_PRESSED_BIT equ 2
STATE_JUST_RELEASED_BIT equ 3

STATE_RELEASED equ 001h
STATE_JUST_PRESSED equ 002h
STATE_PRESSED equ 004h
STATE_JUST_RELEASED equ 008h

COUNTER_1_INIT_RESET_VAL equ 0x01
COUNTER_2_INIT_RESET_VAL equ 0x01
COUNTER_3_INIT_RESET_VAL equ 0x34
 
COUNTER_1_PHASE_RESET_VAL equ 0x01
COUNTER_2_PHASE_RESET_VAL equ 0x5D
 
COUNTER_1_BUTTON_RESET_VAL equ 0x01
COUNTER_2_BUTTON_RESET_VAL equ 0x07

phase res 1
 
counter_1 res 1
counter_2 res 1
counter_3 res 1

button_up_state res 1
button_down_state res 1
button_toggle_state res 1
button_confirm_state res 1

button_up_counter_1 res 1
button_down_counter_1 res 1
button_toggle_counter_1 res 1
button_confirm_counter_1 res 1
 
button_up_counter_2 res 1
button_down_counter_2 res 1
button_toggle_counter_2 res 1
button_confirm_counter_2 res 1
 
blink_state res 1
 
selected_line res 1
line_1_drawing res 1
line_2_drawing res 1
line_3_drawing res 1
 
selected_line_drawing res 1

selected_edge res 1
 
left_part_drawing res 1
right_part_drawing res 1

selected_line_drawing_length res 1
	
org 0x0000
    GOTO init

org 0x0008
    GOTO $
    
phase_1_default:
    MOVLW 000h
    BTFSC blink_state, 0
    MOVLW 0FFh

    phase_1_default_blink_line_1:
	BTFSS selected_line, 1
	GOTO phase_1_default_blink_line_2

	MOVWF LINE_1

    phase_1_default_blink_line_2:
	BTFSS selected_line, 2
	GOTO phase_1_default_blink_line_3

	MOVWF LINE_2

    phase_1_default_blink_line_3:
	BTFSS selected_line, 3
	GOTO phase_1_default_blink_end

	MOVWF LINE_3
    phase_1_default_blink_end:
	
    phase_1_default_draw_line_1:
	BTFSC selected_line, 1
	GOTO phase_1_default_draw_line_2

	MOVFF line_1_drawing, LINE_1
    phase_1_default_draw_line_2:
	BTFSC selected_line, 2
	GOTO phase_1_default_draw_line_3

	MOVFF line_2_drawing, LINE_2
    phase_1_default_draw_line_3:
	BTFSC selected_line, 3
	GOTO phase_1_default_draw_line_end

	MOVFF line_3_drawing, LINE_3
    phase_1_default_draw_line_end:
    
    RETURN
    
phase_1_button_up:
    RLNCF selected_line, 1
    
    MOVLW 010h
    CPFSEQ selected_line
    RETURN

    MOVLW 008h
    MOVWF selected_line
    RETURN

phase_1_button_down:
    RRNCF selected_line, 1
    
    MOVLW 001h
    CPFSEQ selected_line
    RETURN

    MOVLW 002h
    MOVWF selected_line
	    
    return

phase_1_button_toggle:
    BTFSC selected_line, 1
    COMF line_1_drawing
    BTFSC selected_line, 2
    COMF line_2_drawing
    BTFSC selected_line, 3
    COMF line_3_drawing

    RETURN

phase_1_button_confirm:
    CLRF selected_edge
    
    CLRF LINE_1
    CLRF LINE_2
    CLRF LINE_3
    
    MOVLW 001h
    MOVWF selected_line_drawing_length
    MOVWF left_part_drawing

    MOVLW 0FFh
    MOVWF right_part_drawing
    
    RETURN

    
phase_1_pulse:
    COMF blink_state

    RETURN
    
    
phase_2_button_up:
    BTFSS selected_edge, 0
    GOTO phase_2_button_up_right_edge

    phase_2_button_up_left_edge:
	MOVLW 001h
	SUBWF selected_line_drawing_length, 0
	BZ phase_2_button_up_right_end

	RLNCF right_part_drawing, 1
	MOVLW 0FEh
	ANDWF right_part_drawing, 1

	DECF selected_line_drawing_length, 1

	GOTO phase_2_button_up_right_end
    phase_2_button_up_right_edge:
	MOVLW 0FFh
	SUBWF left_part_drawing, 0
	BZ phase_2_button_up_right_end
    
	RLNCF left_part_drawing, 1
	MOVLW 001h
	IORWF left_part_drawing, 1

	INCF selected_line_drawing_length, 1

    phase_2_button_up_right_end:	
        RETURN

phase_2_button_down:
    BTFSS selected_edge, 0
    GOTO phase_2_button_down_right_edge

    phase_2_button_down_left_edge:
	MOVLW 0FFh
	SUBWF right_part_drawing, 0
	BZ phase_2_button_up_right_end
	
	RRNCF right_part_drawing, 1
	MOVLW 080h
	IORWF right_part_drawing, 1

	INCF selected_line_drawing_length, 1

	GOTO phase_2_button_down_end
    phase_2_button_down_right_edge:
	MOVLW 001h
	SUBWF selected_line_drawing_length, 0
	BZ phase_2_button_down_end

	RRNCF left_part_drawing, 1
	MOVLW 07Fh
	ANDWF left_part_drawing, 1

	DECF selected_line_drawing_length, 1

    phase_2_button_down_end:
	RETURN

phase_2_button_toggle:
    COMF selected_edge
    
    RETURN

phase_2_button_confirm:
    BTFSC selected_line, 1
    MOVFF selected_line_drawing, line_1_drawing
    BTFSC selected_line, 2
    MOVFF selected_line_drawing, line_2_drawing
    BTFSC selected_line, 3
    MOVFF selected_line_drawing, line_3_drawing
    
    CLRF blink_state
    COMF blink_state
    
    MOVLW COUNTER_1_PHASE_RESET_VAL
    MOVWF counter_1
    MOVLW COUNTER_2_PHASE_RESET_VAL
    MOVWF counter_2
	    
    RETURN

    
phase_2_default:
    MOVF left_part_drawing, 0
    ANDWF right_part_drawing, 0
    MOVWF selected_line_drawing
    
    BTFSC selected_line, 1
    GOTO phase_2_default_draw_1
    BTFSC selected_line, 2
    GOTO phase_2_default_draw_2
    BTFSC selected_line, 3
    GOTO phase_2_default_draw_3
    
    phase_2_default_draw_1:
	MOVF line_1_drawing, 0
	IORWF selected_line_drawing, 1
	MOVFF selected_line_drawing, LINE_1
	
	MOVFF line_2_drawing, LINE_2
	MOVFF line_3_drawing, LINE_3
	RETURN
    
    phase_2_default_draw_2:
	MOVF line_2_drawing, 0
	IORWF selected_line_drawing, 1
	MOVFF selected_line_drawing, LINE_2
	
	MOVFF line_1_drawing, LINE_1
	MOVFF line_3_drawing, LINE_3
	RETURN
	
    phase_2_default_draw_3:
	MOVF line_3_drawing, 0
	IORWF selected_line_drawing, 1
	MOVFF selected_line_drawing, LINE_3
	
	MOVFF line_1_drawing, LINE_1
	MOVFF line_2_drawing, LINE_2
	RETURN
  
    
phase_2_pulse:
    RETURN
    


main:

    main_button_toggle_start:
	BTFSS PORTB, BUTTON_TOGGLE
	GOTO main_button_toggle_released
	
	main_button_toggle_pressed:
	    BTFSC button_toggle_state, STATE_RELEASED_BIT
	    GOTO main_button_toggle_pressed_R
	    BTFSC button_toggle_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_toggle_pressed_JP
	    BTFSC button_toggle_state, STATE_PRESSED_BIT
	    GOTO main_button_toggle_end
	    BTFSC button_toggle_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_toggle_pressed_JR
	    
	    main_button_toggle_pressed_R:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_2
		
		MOVLW STATE_JUST_PRESSED
		MOVWF button_toggle_state
		
		GOTO main_button_toggle_end
    
	    main_button_toggle_pressed_JP:		
		;DECFSZ button_toggle_counter_1
		;GOTO main_button_toggle_end
		;DECFSZ button_toggle_counter_2
		;GOTO main_button_toggle_end
		
		MOVLW STATE_PRESSED
		MOVWF button_toggle_state
		
		GOTO main_button_toggle_end
		
	    main_button_toggle_pressed_JR:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_2
		
		MOVLW STATE_PRESSED
		MOVWF button_toggle_state
		
		GOTO main_button_toggle_end

    
	main_button_toggle_released:
	    BTFSC button_toggle_state, STATE_RELEASED_BIT
	    GOTO main_button_toggle_end
	    BTFSC button_toggle_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_toggle_released_JP
	    BTFSC button_toggle_state, STATE_PRESSED_BIT
	    GOTO main_button_toggle_released_P
	    BTFSC button_toggle_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_toggle_released_JR

	    main_button_toggle_released_JP:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_toggle_state
		
		GOTO main_button_toggle_end

	    main_button_toggle_released_P:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_toggle_counter_2

		MOVLW STATE_JUST_RELEASED
		MOVWF button_toggle_state
		
		GOTO main_button_toggle_end
		
	    main_button_toggle_released_JR:
		DECFSZ button_toggle_counter_1
		GOTO main_button_toggle_end
		DECFSZ button_toggle_counter_2
		GOTO main_button_toggle_end
		
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_toggle_state

		rb0_released: ; debug label
		    BTFSS phase, 0
		    CALL phase_1_button_toggle
		    BTFSC phase, 0
		    CALL phase_2_button_toggle

		GOTO main_button_toggle_end
		
    main_button_toggle_end:
	
    
    
    
    
    
    main_button_down_start:
	BTFSS PORTB, BUTTON_DOWN
	GOTO main_button_down_released
	
	main_button_down_pressed:
	    BTFSC button_down_state, STATE_RELEASED_BIT
	    GOTO main_button_down_pressed_R
	    BTFSC button_down_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_down_pressed_JP
	    BTFSC button_down_state, STATE_PRESSED_BIT
	    GOTO main_button_down_end
	    BTFSC button_down_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_down_pressed_JR
	    
	    main_button_down_pressed_R:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_down_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_down_counter_2
		
		MOVLW STATE_JUST_PRESSED
		MOVWF button_down_state
		
		GOTO main_button_down_end
    
	    main_button_down_pressed_JP:		
		;DECFSZ button_down_counter_1
		;GOTO main_button_down_end
		;DECFSZ button_down_counter_2
		;GOTO main_button_down_end
		
		MOVLW STATE_PRESSED
		MOVWF button_down_state
		
		GOTO main_button_down_end
		
	    main_button_down_pressed_JR:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_down_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_down_counter_2
		
		MOVLW STATE_PRESSED
		MOVWF button_down_state
		
		GOTO main_button_down_end

    
	main_button_down_released:
	    BTFSC button_down_state, STATE_RELEASED_BIT
	    GOTO main_button_down_end
	    BTFSC button_down_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_down_released_JP
	    BTFSC button_down_state, STATE_PRESSED_BIT
	    GOTO main_button_down_released_P
	    BTFSC button_down_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_down_released_JR

	    main_button_down_released_JP:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_down_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_down_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_down_state
		
		GOTO main_button_down_end

	    main_button_down_released_P:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_down_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_down_counter_2

		MOVLW STATE_JUST_RELEASED
		MOVWF button_down_state
		
		GOTO main_button_down_end
		
	    main_button_down_released_JR:
		DECFSZ button_down_counter_1
		GOTO main_button_down_end
		DECFSZ button_down_counter_2
		GOTO main_button_down_end
		
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_down_state

		rb1_released: ; debug label
		    BTFSS phase, 0
		    CALL phase_1_button_down
		    BTFSC phase, 0
		    CALL phase_2_button_down

		GOTO main_button_down_end
		
    main_button_down_end:
    
    
    
    
    
    
    main_button_up_start:
	BTFSS PORTB, BUTTON_UP
	GOTO main_button_up_released
	
	main_button_up_pressed:
	    BTFSC button_up_state, STATE_RELEASED_BIT
	    GOTO main_button_up_pressed_R
	    BTFSC button_up_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_up_pressed_JP
	    BTFSC button_up_state, STATE_PRESSED_BIT
	    GOTO main_button_up_end
	    BTFSC button_up_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_up_pressed_JR
	    
	    main_button_up_pressed_R:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_up_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_up_counter_2
		
		MOVLW STATE_JUST_PRESSED
		MOVWF button_up_state
		
		GOTO main_button_up_end
    
	    main_button_up_pressed_JP:		
		;DECFSZ button_up_counter_1
		;GOTO main_button_up_end
		;DECFSZ button_up_counter_2
		;GOTO main_button_up_end
		
		MOVLW STATE_PRESSED
		MOVWF button_up_state
		
		GOTO main_button_up_end
		
	    main_button_up_pressed_JR:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_up_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_up_counter_2
		
		MOVLW STATE_PRESSED
		MOVWF button_up_state
		
		GOTO main_button_up_end

    
	main_button_up_released:
	    BTFSC button_up_state, STATE_RELEASED_BIT
	    GOTO main_button_up_end
	    BTFSC button_up_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_up_released_JP
	    BTFSC button_up_state, STATE_PRESSED_BIT
	    GOTO main_button_up_released_P
	    BTFSC button_up_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_up_released_JR

	    main_button_up_released_JP:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_up_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_up_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_up_state
		
		GOTO main_button_up_end

	    main_button_up_released_P:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_up_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_up_counter_2

		MOVLW STATE_JUST_RELEASED
		MOVWF button_up_state
		
		GOTO main_button_up_end
		
	    main_button_up_released_JR:
		DECFSZ button_up_counter_1
		GOTO main_button_up_end
		DECFSZ button_up_counter_2
		GOTO main_button_up_end
		
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_up_state

		rb2_released: ; debug label
		    BTFSS phase, 0
		    CALL phase_1_button_up
		    BTFSC phase, 0
		    CALL phase_2_button_up

		GOTO main_button_up_end
		
    main_button_up_end:
	
    
    
    
    main_button_confirm_start:
	BTFSS PORTB, BUTTON_CONFIRM
	GOTO main_button_confirm_released
	
	main_button_confirm_pressed:
	    BTFSC button_confirm_state, STATE_RELEASED_BIT
	    GOTO main_button_confirm_pressed_R
	    BTFSC button_confirm_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_confirm_pressed_JP
	    BTFSC button_confirm_state, STATE_PRESSED_BIT
	    GOTO main_button_confirm_end
	    BTFSC button_confirm_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_confirm_pressed_JR
	    
	    main_button_confirm_pressed_R:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_JUST_PRESSED
		MOVWF button_confirm_state
		
		GOTO main_button_confirm_end
    
	    main_button_confirm_pressed_JP:		
		;DECFSZ button_confirm_counter_1
		;GOTO main_button_confirm_end
		;DECFSZ button_confirm_counter_2
		;GOTO main_button_confirm_end
		
		MOVLW STATE_PRESSED
		MOVWF button_confirm_state
		
		GOTO main_button_confirm_end
		
	    main_button_confirm_pressed_JR:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_PRESSED
		MOVWF button_confirm_state
		
		GOTO main_button_confirm_end

    
	main_button_confirm_released:
	    BTFSC button_confirm_state, STATE_RELEASED_BIT
	    GOTO main_button_confirm_end
	    BTFSC button_confirm_state, STATE_JUST_PRESSED_BIT
	    GOTO main_button_confirm_released_JP
	    BTFSC button_confirm_state, STATE_PRESSED_BIT
	    GOTO main_button_confirm_released_P
	    BTFSC button_confirm_state, STATE_JUST_RELEASED_BIT
	    GOTO main_button_confirm_released_JR

	    main_button_confirm_released_JP:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_confirm_state
		
		GOTO main_button_confirm_end

	    main_button_confirm_released_P:
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2

		MOVLW STATE_JUST_RELEASED
		MOVWF button_confirm_state
		
		GOTO main_button_confirm_end
		
	    main_button_confirm_released_JR:
		DECFSZ button_confirm_counter_1
		GOTO main_button_confirm_end
		DECFSZ button_confirm_counter_2
		GOTO main_button_confirm_end
		
		MOVLW COUNTER_1_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_1
		MOVLW COUNTER_2_BUTTON_RESET_VAL
		MOVWF button_confirm_counter_2
		
		MOVLW STATE_RELEASED
		MOVWF button_confirm_state

		rb3_released: ; debug label
		    BTFSS phase, 0
		    CALL phase_1_button_confirm
		    BTFSC phase, 0
		    CALL phase_2_button_confirm
		    
		    COMF phase

		GOTO main_button_confirm_end
		
    main_button_confirm_end:
 
    
    DECFSZ counter_1
    GOTO main_skip_pulse
    DECFSZ counter_2
    GOTO main_skip_pulse

    msec200_passed: ; debug label
    main_pulse:
	BTFSS phase, 0
	CALL phase_1_pulse
	BTFSC phase, 0
	CALL phase_2_pulse

	MOVLW COUNTER_1_PHASE_RESET_VAL
	MOVWF counter_1
	MOVLW COUNTER_2_PHASE_RESET_VAL
	MOVWF counter_2

    main_skip_pulse:
    
	BTFSS phase, 0
	CALL phase_1_default
	BTFSC phase, 0
	CALL phase_2_default
	
    
    GOTO main
	   
init:
	MOVLW 00Fh
	MOVWF ADCON1
	
	CLRF TRISD ; down
	CLRF TRISC ; mid
	CLRF TRISA ; up

	CLRF line_1_drawing
	CLRF line_2_drawing
	CLRF line_3_drawing

	CLRF selected_edge

	CLRF phase
	
	CLRF blink_state
	
	MOVLW STATE_RELEASED
	MOVWF button_up_state
	MOVWF button_down_state
	MOVWF button_toggle_state
	MOVWF button_confirm_state
	
	MOVLW COUNTER_1_BUTTON_RESET_VAL
	MOVWF button_toggle_counter_1
	MOVWF button_up_counter_1
	MOVWF button_down_counter_1
	MOVWF button_confirm_counter_1

	MOVLW COUNTER_2_BUTTON_RESET_VAL
	MOVWF button_toggle_counter_2
	MOVWF button_up_counter_2
	MOVWF button_down_counter_2
	MOVWF button_confirm_counter_2
	
	MOVLW 008h
	MOVWF selected_line

	MOVLW 001h
	MOVWF selected_line_drawing_length
	MOVWF left_part_drawing

	MOVLW 0FFh
	MOVWF right_part_drawing
	
	MOVWF LINE_1
	MOVWF LINE_2
	MOVWF LINE_3

	; GOTO init_start_loop_end

	MOVLW COUNTER_1_INIT_RESET_VAL
	MOVWF counter_1
	MOVLW COUNTER_2_INIT_RESET_VAL
	MOVWF counter_2
	MOVLW COUNTER_3_INIT_RESET_VAL
	MOVWF counter_3

	init_complete:; debug label
	init_start_loop:
	    DECFSZ counter_1
	    GOTO init_start_loop
	    DECFSZ counter_2
	    GOTO init_start_loop
	    DECFSZ counter_3
	    GOTO init_start_loop

	sec_passed: ; debug label
	init_start_loop_end:
	    MOVLW COUNTER_1_PHASE_RESET_VAL
	    MOVWF counter_1
	    MOVLW COUNTER_2_PHASE_RESET_VAL
	    MOVWF counter_2
	    
	    CLRF LINE_1
	    CLRF LINE_2
	    CLRF LINE_3

	    GOTO main

end