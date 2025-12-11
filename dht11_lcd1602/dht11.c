/*
 * dht11.c
 *
 *  Created on: Dec 8, 2025
 *      Author: Admin
 */

#include "stm32f1xx_hal.h"
#include <delay.h>
#include <dht11.h>
# include "math.h"
# include <lcd1602.h>


extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

//--------------------------------------------------------------------------------------
void GPIO_SET(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t MODE, uint32_t GPIO_PULL){
	if (GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
	    else if (GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
	    else if (GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_Pin;


  if (MODE == OUTPUT){
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set mode to input
	  GPIO_InitStruct.Pull = GPIO_PULL ; // Or GPIO_PULLUP/GPIO_PULLDOWN
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
  }

  else if (MODE == INPUT){
	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pull = GPIO_PULL; // Or GPIO_PULLUP/GPIO_PULLDOWN
	    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
  }
} // OK

//----------------------------------------------------------------------------
int DHT11_READ(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint8_t *hum, uint8_t *tem){

	// SEND START_SIGNAL
	GPIO_SET(GPIOx, GPIO_Pin, OUTPUT, GPIO_NOPULL);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, SET);
	delay_us(30);
	GPIO_SET(GPIOx, GPIO_Pin, INPUT, GPIO_PULLUP);


	// CHECK RESPONSE FROM DHT11
	uint32_t  t= 0;
	
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET){
		if(++t > 1000){
			return -1;
		}
		delay_us(1);
	}

	t = 0;
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET){
		if(++t > 1000){
			return -2;
		}
		delay_us(1);
	}

	t = 0;
	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET){
		if(++t > 1000){
			return -3;
		}
		delay_us(1);
	}


	// READ 40 DATA BIT
	uint8_t data[5] = {0};
	for (int i = 0; i < 40; i++){
		while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);

		__HAL_TIM_SET_COUNTER(&htim2, 0);
		while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET){
			if (__HAL_TIM_GET_COUNTER(&htim2) > 100) break;
		}

		uint16_t t_high = __HAL_TIM_GET_COUNTER(&htim2);

		int byte=i/8;
		data[byte] <<= 1;
		if (t_high > 40) data[byte] |= 1;
	}


	// CHECK_SUM
	uint8_t sum = data[0] + data[1] + data[2] + data[3];
	if ((sum & 0xFF) != data[4]){
		return -4;
	}

	// TAKE DATA FROM ARRAY
	*hum = data[0];
	*tem = data[2];

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	return READY;

}
 //---------------------------------------------------------------------------------------
void SEND_DATA(uint8_t hum, uint8_t tem){
	//--- SEND HUMIDITY ---
	    uint8_t percent = '%';
	    uint8_t hum_tens = hum / 10;
	    char hum_tens_char = hum_tens + '0';
	    uint8_t hum_units = hum % 10;
	    char hum_units_char = hum_units + '0';

//	    uint8_t new_line[] = "\n\r";
	    uint8_t hum_string[] = "DO AM: ";



	    lcd_put_cur(1,0);
	    lcd_send_string(hum_string);
	    lcd_send_char(hum_tens_char);
	    lcd_send_char(hum_units_char);
	    lcd_send_char(percent);
//	    HAL_UART_Transmit(&huart1, hum_string, sizeof(hum_string) -1, 100);
//	    HAL_UART_Transmit(&huart1, (uint8_t*)&hum_tens_char, 1,100);
//	    HAL_UART_Transmit(&huart1, (uint8_t*)&hum_units_char, 1, 100);
//	    HAL_UART_Transmit(&huart1, &percent,1,100);
//	    HAL_UART_Transmit(&huart1, new_line, 2, 100);


	//--- SEND TEMPERATURE ---
	    uint8_t celcius[] = {0xDF, 'C', '\0'};
	    uint8_t tem_tens = tem / 10;
	    char tem_tens_char = tem_tens + '0';
	    uint8_t tem_units = tem % 10;
	    char tem_units_char = tem_units + '0';
	    uint8_t tem_string[] = "NHIET DO: ";
	    lcd_put_cur(0, 0);
	    lcd_send_string(tem_string);
	    lcd_send_char(tem_tens_char);
	    lcd_send_char(tem_units_char);
	    lcd_send_string(celcius);
//	    HAL_UART_Transmit(&huart1, tem_string, sizeof(tem_string) -1, 100);
//	    HAL_UART_Transmit(&huart1, (uint8_t*)&tem_tens_char, 1,100);
//	    HAL_UART_Transmit(&huart1, (uint8_t*)&tem_units_char, 1, 100);
//	    HAL_UART_Transmit(&huart1, celcius, sizeof(celcius)-1 ,100);
//	    HAL_UART_Transmit(&huart1, new_line, 2, 100);
//	    HAL_UART_Transmit(&huart1, new_line, 2, 100);
}



