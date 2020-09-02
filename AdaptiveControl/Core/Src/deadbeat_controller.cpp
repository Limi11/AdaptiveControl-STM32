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
:maxControlOutput(maxcontroloutput), firstControlOutput(firstcontroloutput), order(order),pArray(new float[order+1]), qArray(new float[order+1]),
 aArray(new float[order]), bArray(new float[order]),  inputArray(new float[order+1]), outputArray(new float[order+1])
{
	outputArray[0] = firstControlOutput;
}

deadbeat_controller::~deadbeat_controller()
{
	delete pArray;
	delete qArray;
	delete aArray;
	delete bArray;
	delete inputArray;
	delete outputArray;
}

void deadbeat_controller::getNewSystem(float* system)
{

	for(int i=(order-1); i>0; i--)
		{
		aArray[i] = system[i];
		}

		for(int i=(2*order); i>order; i--)
		{
		bArray[i] = system[i];
		}
}

void deadbeat_controller::calculateNewController()
{
	float bsum;

	for(int i = 0; i<order; i++)
	{
		bsum += bArray[i];
	}

	qArray[0] = firstControlOutput;
	qArray[1] = firstControlOutput*(aArray[0]-1)+1/bsum;

	pArray[0] =  firstControlOutput*bArray[0];

	for(int i = 2; i<order; i++)
	{
		qArray[i] = firstControlOutput*(aArray[i]-aArray[i-1])+aArray[i-1]/bsum;
		pArray[i] = firstControlOutput*(bArray[i]-bArray[i-1])+bArray[i-1]/bsum;
	}
	    qArray[order] = aArray[order]*(-firstControlOutput+1/bsum);
	    pArray[order] = bArray[order]*(-firstControlOutput+1/bsum);
}

float deadbeat_controller::controll(float input)
{
	float output;

	for(int i = order; i==1; i--)
		{
			outputArray[i] = outputArray[i-1];
		}


	for(int i = order; i==1; i--)
	{
		inputArray[i] = inputArray[i-1];
	}

	inputArray[0] = input;

	for(int i =0; i<order; i++)
	{
	output += (-1) * pArray[i] * inputArray[i] + qArray[i] * outputArray[i];
	}
	output += firstControlOutput;

	outputArray[0] = output;

	return output;
}




