#ifndef __LCD
#define __LCD

// LCD module connections
#define LCD_RS              PORTEbits.RE2;      // RS pin for LCD
#define LCD_E               PORTEbits.RE1;      // Enable pin for LCD
#define LCD_Data_Bus_D4     LATDbits.LATD4;
#define LCD_Data_Bus_D5     LATDbits.LATD5;
#define LCD_Data_Bus_D6     LATDbits.LATD6;
#define LCD_Data_Bus_D7     LATDbits.LATD7;

#define LCD_RS_Dir          TRISE2_bit;
#define LCD_E_Dir           TRISE1_bit;
#define LCD_Data_Bus_Dir_D4 TRISDbits.TRISD4;     // Data bus bit 4
#define LCD_Data_Bus_Dir_D5 TRISDbits.TRISD5;     // Data bus bit 5
#define LCD_Data_Bus_Dir_D6 TRISDbits.TRISD6;     // Data bus bit 6
#define LCD_Data_Bus_Dir_D7 TRISDbits.TRISD7;     // Data bus bit 7
// End LCD module connections

#define LCD_FS_1LINE 0x30
#define LCD_FS_2LINE 0x38
#define ROW1 0x80
#define ROW2 0xC0
#define LCD_OFF 0x08
#define LCD_ON  0x0C
#define LCD_CURSOR_ON 0x0A
#define LCD_CURSOR_BLINK 0x09
#define LCD_ENTRY_FORWARD 0x06
#define LCD_ENTRY_REVERSE 0x04
#define LCD_ENTRY_SLIDE 0x05
#define LCD_CLEAR 0x01
#define LCD_CURSOR_MOVE_RIGHT 0x14
#define LCD_CURSOR_MOVE_LEFT 0x10
#define LCD_CONTENT_MOVE_RIGHT 0x1C
#define LCD_CONTENT_MOVE_LEFT 0x18


// Constants
#define E_Delay       1


// Function Declarations
void WriteCommandToLCD(unsigned char);
void WriteDataToLCD(char);
void InitLCD(void);
void WriteStringToLCD(const char*);
void ClearLCDScreen(void);
void LcdPrintString(const char *,  unsigned char, unsigned char);


#endif

/* End of file : lcd.h */