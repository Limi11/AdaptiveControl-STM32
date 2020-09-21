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


deadbeat_controller::deadbeat_controller(int firstcontroloutput, int order,  int deadtimeMaxTimesteps)
:firstControlOutput(firstcontroloutput),deadTime(0),deadtimeVector(new float[deadtimeMaxTimesteps]),order(order),pArray(new float[order+1]), qArray(new float[order+1]),
 aArray(new float[order]), bArray(new float[order]), inputArray(new float[order+20]), outputArray(new float[order+1]), firstRound(0)
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
	delete deadtimeVector;
}


void deadbeat_controller::getInputs(float input)
{
		for(int i=(order+1); i>=1; i--)
		{
			inputArray[i] = inputArray[i-1];
		}
		inputArray[0] = input;
}

void deadbeat_controller::getNewSystem(float* system, int deadtime)
{
		for(int i=0; i<order; i++)
		{
			aArray[i] = -system[i];
			bArray[i] = system[i+order];
		}
		deadTime = deadtime;
}

void deadbeat_controller::calculateNewController()
{
	float bsum = 0;

	for(int i = 0; i<order; i++)
	{
		bsum += bArray[i];
	}

	qArray[0] = firstControlOutput;
	pArray[0] = 1;
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
	}



	if(output > firstControlOutput)
	{
		output = firstControlOutput;
	}
	if(output < 0)
	{
		output = 0;
	}


	if(deadTime == 0)
	{
		for(int i=(order+1); i>=1 ; i--)
		{
			outputArray[i] = outputArray[i-1];
		}
	outputArray[0] = output;
	}
	else
	{
		for(int i=(deadTime-1); i>=1; i--)
		{
			deadtimeVector[i] = deadtimeVector[i-1];
		}
		deadtimeVector[0] = output;
		outputArray[0] = deadtimeVector[deadTime];
		for(int i=(order+1); i>=1; i--)
		{
			outputArray[i] = outputArray[i-1];
		}
	}

	return output;
}




