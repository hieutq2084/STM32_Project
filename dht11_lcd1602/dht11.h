/*
 * dht11.h
 *
 *  Created on: Dec 8, 2025
 *      Author: Admin
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#define  OUTPUT 0
#define  INPUT 1
#define READY 100

void GPIO_SET(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t MODE, uint32_t GPIO_PULL);
int DHT11_READ(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint8_t *hum, uint8_t *tem);
void SEND_DATA(uint8_t hum, uint8_t tem);
#endif /* INC_DHT11_H_ */
