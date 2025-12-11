/*
 * lcd1602.h
 *
 *  Created on: Dec 10, 2025
 *      Author: Admin
 */
# ifndef LCD1602_H
# define LCD1602_H

#include "stm32f1xx_hal.h"   // hoặc dòng MCU bạn dùng
#include "stdint.h"          // để dùng uint8_t

# define LCD1602_ADDRESS 0x4E



void lcd_send_cmd(uint8_t cmd);
void lcd_send_char(uint8_t data);
void lcd_init(void);
void lcd_send_string(uint8_t *string);
void lcd_put_cur(int row, int col);

#endif



    //*BIT MAP
// P7  P6  P5  P4  P3  P2  P1  P0
// D7  D6  D5  D4  BL  EN  RW  RS

//
