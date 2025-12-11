/*
 * delay.c
 *
 *  Created on: Dec 7, 2025
 *      Author: Admin
 */
# include <delay.h>
extern TIM_HandleTypeDef htim2;   // TIM2 = 32-bit

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);  // Reset counter
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

