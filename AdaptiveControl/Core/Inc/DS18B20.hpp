/*
 * DS18B20.h
 *
 *  Created on: 30.09.2020
 *      Author: User
 */



#ifndef DS18B20_H_
#define DS18B20_H_

#include <main.hpp>


extern TIM_HandleTypeDef htim3;

// mainly internal functions
void delay (uint16_t us);
void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


// functions for external use
uint8_t DS18B20_Start (void);
void DS18B20_Write (uint8_t data);
uint8_t DS18B20_Read (void);

#endif
