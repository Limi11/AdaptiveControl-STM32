/*
 * DS18B20.c
 *
 *  Created on: 30.09.2020
 *      Author: User
 */


#include "DS18B20.hpp"


#define DS18B20_PORT 		GPIOA
#define DS18B20_PIN 		9



uint8_t DS18B20_Start (void)
{
	uint8_t Response = 0;
	GPIOA->MODER |= GPIO_MODER_MODER9_0;   // set the pin as output via GPIO register
	HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // pull the pin low
	DS18B20_Delay(480);   // delay according to datasheet

	GPIOA->MODER &= ~(GPIO_MODER_MODER9);    // set the pin as input
	DS18B20_Delay(80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (DS18B20_PORT, DS18B20_PIN))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	DS18B20_Delay(400); // 480 us delay totally.

	return Response;
}

void DS18B20_Write (uint8_t data)
{
	GPIOA->MODER |= GPIO_MODER_MODER9_0;  // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			GPIOA->MODER |= GPIO_MODER_MODER9_0;  // set as output
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // pull the pin LOW
			DS18B20_Delay(1);  // wait for 1 us

			GPIOA->MODER &= ~(GPIO_MODER_MODER9);  // set as input
			DS18B20_Delay(50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			GPIOA->MODER |= GPIO_MODER_MODER9_0;
			HAL_GPIO_WritePin (DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);  // pull the pin LOW
			DS18B20_Delay(50);  // wait for 60 us

			GPIOA->MODER &= ~(GPIO_MODER_MODER9);
		}
	}
}

uint8_t DS18B20__Read (void)
{
	uint8_t value=0;
	GPIOA->MODER &= ~(GPIO_MODER_MODER9);

	for (int i=0;i<8;i++)
	{
		GPIOA->MODER |= GPIO_MODER_MODER9_0;   // set as output

		HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  // pull the data pin LOW
		DS18B20_Delay(2);  // wait for 2 us

		GPIOA->MODER &= ~(GPIO_MODER_MODER9);  // set as input
		if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_1))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		DS18B20_Delay(60);  // wait for 60 us
	}
	return value;
}

void DS18B20_Delay (uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim3,0);
    while ((__HAL_TIM_GET_COUNTER(&htim3))<us);
}


