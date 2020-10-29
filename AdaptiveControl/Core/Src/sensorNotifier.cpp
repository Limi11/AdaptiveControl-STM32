/*
 * sensorNotifier.cpp
 *
 *  Created on: Oct 29, 2020
 *      Author: User
 */



#include "sensorNotifier.hpp";


sensorNotifier::sensorNotifier()
{};

sensorNotifier::~sensorNotifier()
{};

void sensorNotifier::setTemperature(float x)
{
	Temperature = x;
}

float sensorNotifier::getTemperature()
{
	return Temperature;
}
