
/** Put this in the src folder **/

#include "i2c-lcd.h"
tLCD LCD{0x27, 20, 4, 1};

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

void LCD_Begin()
{
    uint8_t l_command = LCD_MODE_CMD;
    uint8_t l_data = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (LCD.lcd_rows > 1) {
		l_data |= LCD_2LINE;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands.
	HAL_Delay(50);

	// Now we pull both RS and R/W low to begin commands
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) l_command, 4, 100);(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	HAL_Delay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms (5 ms?)

	// second try
	write4bits(0x03 << 4);
	HAL_Delay(5); // wait min 4.1ms (5 ms?)

	// third go!
	write4bits(0x03 << 4);
	HAL_Delay(1);   // wait 150 µs (1 ms?)

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	lcd_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	lcd_home();
    HAL_I2C_Master_Transmit (&hi2c1, LCD.lcd_addr,(uint8_t *) data_t, 4, 100);
}

void lcd_send_cmd (char cmd)
{

    // NEW
    lcd_send(cmd, LCD_MODE_CMD);

  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=1
	data_t[1] = data_u|0x08;  //en=0, rs=1
	data_t[2] = data_l|0x0C;  //en=1, rs=1
	data_t[3] = data_l|0x08;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
    // NEW
    lcd_send(data, LCD_MODE_DAT);

	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
	lcd_send_cmd (0x80);
	for (int i=0; i<70; i++)
	{
		lcd_send_data (' ');
	}
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear (void)
{
    lcd_send_cmd(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
}

void lcd_home (void)
{
    lcd_send_cmd(LCD_RETURNHOME);  // set cursor position to zero
	HAL_Delay(2);  // this command takes a long time!
}

void lcd_show (void)
{

}

void lcd_hide (void)
{

}

void lcd_cursor_show (void)
{

}

void lcd_cursor_hide (void)
{

}

void lcd_cursor_blink (void)
{

}

void lcd_cursor_noblink (void)
{

}

void lcd_send (uint8_t value, uint8_t mode)
{
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}
