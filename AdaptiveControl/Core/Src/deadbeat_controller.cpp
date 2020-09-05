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
	for(int i=0; i<order; i++)
	{
		outputArray[i] = 0;
	}

	for(int i=0; i<order; i++)
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
	pArray[0] = 0;
	qArray[1] = firstControlOutput*(aArray[0]-1)+(1/bsum);
	pArray[1] =  firstControlOutput*bArray[0];

	for(int i = 2; i<order; i++)
	{
		qArray[i] = firstControlOutput*(aArray[i]-aArray[i-1])+(aArray[i-1]/bsum);
		pArray[i] = firstControlOutput*(bArray[i]-bArray[i-1])+(bArray[i-1]/bsum);
	}
	    qArray[order] = aArray[order-1]*(-firstControlOutput+(1/bsum));
	    pArray[order] = -bArray[order-1]*(firstControlOutput-(1/bsum));

		printf("qArray[0]: %.2f  \r\n\r\n", qArray[0]);
		printf("qArray[1]: %.2f  \r\n\r\n", qArray[1]);
		printf("qArray[2]: %.2f  \r\n\r\n", qArray[2]);
		printf("pArray[0]: %.2f  \r\n\r\n", pArray[0]);
		printf("pArray[1]: %.2f  \r\n\r\n", pArray[1]);
		printf("pArray[2]: %.2f  \r\n\r\n", pArray[2]);
}

float deadbeat_controller::controll(float input)
{
	float output;

	for(int i=(order-1); i>=1 ; i--)
		{
			outputArray[i] = outputArray[i-1];
		}

	for(int i = (order-1); i>=1; i--)
		{
		inputArray[i] = inputArray[i-1];
		}

	inputArray[0] = input;

	for(int i =0; i<=order; i++)
	{
	output += (-1) * pArray[i] * outputArray[i] + qArray[i] * inputArray[i];
	}
//	output += firstControlOutput; I think that's wrong

	outputArray[0] = output;

	// check maximum and minimum output
	if(output < 0)
	{
		output = 0;
	}
	if(output > 2)
	{
		output = 2;
	}

	return output;
}




