/*
 * DS18B20.h
 *
 *  Created on: 30.09.2020
 *      Author: User
 */



#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.hpp"
#include "cmsis_os.h"
#include "stm32f4xx_hal_def.h"

extern TIM_HandleTypeDef htim3;

uint8_t DS18B20_Start (void);
void DS18B20_Write (uint8_t data);
uint8_t DS18B20__Read (void);
void DS18B20_Delay (uint32_t us);


#endif DS18B20_H_
