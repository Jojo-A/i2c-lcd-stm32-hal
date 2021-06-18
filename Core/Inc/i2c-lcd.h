#include "stm32f1xx_hal.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHTON 0x01
#define LCD_BACKLIGHTOFF 0x00

#define Bl B00001000  // Backlight bit
#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

#define LCD_CTRL_EN B00000100  // Enable bit
#define LCD_CTRL_RW B00000010  // Read/Write bit
#define LCD_CTRL_RS B00000001  // Register select bit

#define LCD_MODE_CMD    0x00
#define LCD_MODE_DAT    0x01

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);
void lcd_home (void);
void lcd_show (void);
void lcd_hide (void);
void lcd_clear (void);
void lcd_cursor_show (void);
void lcd_cursor_hide (void);
void lcd_cursor_blink (void);
void lcd_cursor_noblink (void);

union _EXPANDER_DATA
{
    uint8_t All;
    struct
    {
        uint8_t Bit0:1;
        uint8_t Bit1:1;
        uint8_t Bit2:1;
        uint8_t Bit3:1;
        uint8_t Bit4:1;
        uint8_t Bit5:1;
        uint8_t Bit6:1;
        uint8_t Bit7:1;
    }Bits;
}
// LiquidCrystal stuff
typedef struct
{
    uint_fast8_t lcd_addr;
    uint_fast8_t lcd_cols;
    uint_fast8_t lcd_rows;

	uint_fast8_t backlightval;
}tLCD;

void LCD_Begin(void);


extern tLCD LCD;
