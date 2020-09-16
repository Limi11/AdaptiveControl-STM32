/*
 * testsystem.cpp
 *
 *  Created on: 20.07.2020
 *      Author: User
 */


#include "testsystem.hpp"


testsystem::testsystem(int deadtime)
	:y(0.0),ylast(0.0),u(0.0),deltaT(5),T(360),Kpt1(200), flag(0), output(new float[2]), deadTime(deadtime), deadtimeVector(new float[deadtime])
	{};

float* testsystem::testsystem_output(float u, int t)
{
	{
		deltaT = t/1000;
		y = ylast + (Kpt1 * u - ylast) * deltaT/(deltaT+T);
		ylast = y;

		if(deadTime != 0)
		{
			for(int i=(deadTime-1); i>0; i--)
			{
				deadtimeVector[i] = deadtimeVector[i-1];
			}
			deadtimeVector[0] = y;
			output[0] = deadtimeVector[deadTime-1];
			output[1] = u;
		}
		else
		{
			output[0] = y;
			output[1] = u;
		}

		return output;
	}
};
