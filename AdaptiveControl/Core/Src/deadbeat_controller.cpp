/*
 * optimal_controller.cpp
 *
 *  Created on: 20.07.2020
 *      Author: User
 */


#include "deadbeat_controller.hpp"
#include "debug.h"
#include <stdio.h>


deadbeat_controller::deadbeat_controller(int maxcontroloutput, int firstcontroloutput, int order)
:maxControlOutput(maxcontroloutput), firstControlOutput(firstcontroloutput), order(order),pArray(new float[order]), qArray(new float[order]),
 aArray(new float*[order]), bArray(new float*[order])
{
}

deadbeat_controller::~deadbeat_controller()
{
	delete pArray;
	delete qArray;
}

void getNewSystem(float* system)
{

	for(int i=(order-1); i>0; i--)
		{
		aArray[i] = system[i];
		}

		for(int i=m; i>order; i--)
		{
		bArray[i] = system[i];
		}
}

void calculateNewController()
{
	// to be defined
}

float controll(float input)
{
	// to be defined
}
}



