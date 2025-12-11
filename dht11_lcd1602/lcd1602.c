#include "stm32f1xx_hal.h"
#include <lcd1602.h>


extern I2C_HandleTypeDef hi2c1;

//----------------------------------------------------------
// Gửi lệnh
void lcd_send_cmd(uint8_t cmd)
{
  uint8_t data_u, data_l;
  uint8_t data[4];

  data_u = (cmd & 0xF0);
  data_l = ((cmd << 4) & 0xF0);

  data[0] = data_u | 0x0C;  // EN=1, RS=0, BL=1
  data[1] = data_u | 0x08;  // EN=0, RS=0, BL=1

  data[2] = data_l | 0x0C;
  data[3] = data_l | 0x08;

  HAL_I2C_Master_Transmit(&hi2c1, LCD1602_ADDRESS, data, 4, 100);
}



//------------------------------------------------------------------
// Gửi kí tự
void lcd_send_char(uint8_t data)
{
  uint8_t data_u, data_l;
  uint8_t data_arr[4];

  data_u = (data & 0xF0);
  data_l = ((data << 4) & 0xF0);

  data_arr[0] = data_u | 0x0D;   // EN=1, RS=1
  data_arr[1] = data_u | 0x09;   // EN=0, RS=1

  data_arr[2] = data_l | 0x0D;
  data_arr[3] = data_l | 0x09;

  HAL_I2C_Master_Transmit(&hi2c1, LCD1602_ADDRESS, data_arr, 4, 100);
}




//-------------------------------------------------------------------
void lcd_send_string (uint8_t *str){
	while (*str){
		lcd_send_char(*str);
		str++;
	}
}

//----------------------------------------------------------------------
void lcd_put_cur(int row, int col)           //Row 0 starts at DDRAM address 0x00 → Command = (0x80 | col)
		                                      //Row 1 starts at DDRAM address 0x40 → Command = (0x80 | 0x40 | col) → (0xC0 | col)
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


//-----------------------------------------------------------------------
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

  // display initialisation
  lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
  HAL_Delay(1);
  lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
  HAL_Delay(1);
  lcd_send_cmd (0x01);  // clear display
  HAL_Delay(2);
  lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
  HAL_Delay(1);
  lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}
