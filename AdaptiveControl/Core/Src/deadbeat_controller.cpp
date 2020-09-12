/*
 * optimal_controller.cpp
 *
 *  Created on: 20.07.2020
 *      Author: Milan Liebsch
 *      Comment: Deadbeat Controller extrem hard for parametration
 */


#include "deadbeat_controller.hpp"
#include "debug.h"
#include <stdio.h>


deadbeat_controller::deadbeat_controller(int maxcontroloutput, int firstcontroloutput, int order)
:maxControlOutput(maxcontroloutput), firstControlOutput(firstcontroloutput), order(order),pArray(new float[order+1]), qArray(new float[order+1]),
 aArray(new float[order]), bArray(new float[order]),  inputArray(new float[order+1]), outputArray(new float[order+1]), firstRound(0)
{
	for(int i=0; i<order+1; i++)
	{
		outputArray[i] = 0;
	}

	for(int i=0; i<order+1; i++)
	{
		inputArray[i] = 0;
	}
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


void deadbeat_controller::getInputs(float input)
{

	for(int i = (order); i>=1; i--)
	{
		inputArray[i] = inputArray[i-1];
	}

	inputArray[0] = input;
}

void deadbeat_controller::getNewSystem(float* system)
{
	for(int i=0; i<order; i++)
		{
		aArray[i] = system[i];
		bArray[i] = system[i+order];
		}
		printf("aArray[0]: %.2f  \r\n\r\n", aArray[0]);
		printf("aArray[1]: %.2f  \r\n\r\n", aArray[1]);
		printf("bArray[0]: %.2f  \r\n\r\n", bArray[0]);
		printf("bArray[1]: %.2f  \r\n\r\n", bArray[1]);
}

void deadbeat_controller::calculateNewController()
{
	float bsum = 0;

	for(int i = 0; i<order; i++)
	{
		bsum += bArray[i];
	}

	qArray[0] = firstControlOutput;
	pArray[0] = -1;
	qArray[1] = firstControlOutput*(aArray[0]-1)+(1/bsum);
	pArray[1] = firstControlOutput*bArray[0];

	for(int i = 2; i<(order+1); i++)
	{
		qArray[i] = firstControlOutput*(aArray[i-1]-aArray[i-2])+(aArray[i-2]/bsum);
		pArray[i] = firstControlOutput*(bArray[i-1]-bArray[i-2])+(bArray[i-2]/bsum);
	}
	    qArray[order+1] = aArray[order-1]*(-firstControlOutput+(1/bsum));
	    pArray[order+1] = -bArray[order-1]*(firstControlOutput-(1/bsum));
}


float deadbeat_controller::controll()
{
	float output = 0;

	for(int i=0; i<=order+2; i++)
	{
	output += - pArray[i] * outputArray[i] + qArray[i] * inputArray[i];
	/*
	printf("qArray[i]: %.2f  \r\n\r\n", qArray[i]);
	printf("pArray[i]: %.2f  \r\n\r\n", pArray[i]);
	printf("outputArray[i]: %.2f  \r\n\r\n", outputArray[i]);
	printf("inputArray[i]: %.2f  \r\n\r\n", inputArray[i]);
	*/
	}

	if(output > firstControlOutput)
	{
		output = firstControlOutput;
	}
	if(output < 0)
	{
		output = 0;
	}

	for(int i=(order); i>=1 ; i--)
	{
		outputArray[i] = outputArray[i-1];
	}

	outputArray[0] = output;

	return output;
}




