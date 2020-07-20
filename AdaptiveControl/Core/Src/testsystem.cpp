/*
 * testsystem.cpp
 *
 *  Created on: 20.07.2020
 *      Author: User
 */


#include "testsystem.hpp"


testsystem::testsystem()
	:y(0.0),ylast(0.0),u(0.0),deltaT(1000),T(36),Kpt1(200), flag(0), output(new float[2])
	{};

float* testsystem::testsystem_output(float u, int t)
{
	if(flag == 0)
	{
		u = 0;
		flag = 1;
	}

	deltaT = t/1000;
	y = ylast + (Kpt1 * u - ylast) * deltaT/(deltaT+T);
	ylast = y;
	output[0] = y;
	output[1] = u;
	return output;
};
