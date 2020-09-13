/*
 * verification.cpp
 *
 *  Created on: 20.07.2020
 *      Author: User
 */


#include "verification.hpp"


verification::verification(int order)
:y(0.0),order(order),m(order*2),yVec(new float[order]+20),u(0.0),uVec(new float[order]), deadtime(0)
{
		for(int i=0; i<order; i++)
		{
		uVec[i] = 0;
		}
		for(int i=0; i<order+20; i++)
		{
		yVec[i] = 0;
		}

};

verification::~verification()
{
	delete yVec;
	delete uVec;
};

float verification::verification_output(float x,const float* ab, int deadtime)
{
	u = x;
	y = 0;

// calculate new i
	for(int i=0; i<order; i++)
	{
	y += -ab[i]*yVec[i+deadtime];
	}
	for(int i=order; i<m; i++)
	{
	y += ab[i]*uVec[i-order];
	}

// reset signal Vectors
	for(int i=(order-1+deadtime); i>0; i--)
	{
		yVec[i] = yVec[i-1];
	}
	for(int i=(order-1); i>0; i--)
	{
		uVec[i] = uVec[i-1];
	}

	yVec[0] = y;
	uVec[0] = u;

	return y;
};
