/*
 * verifikation.hpp
 *
 *  Created on: 06.07.2020
 *      Author: User
 */

#ifndef INC_VERIFICATION_HPP_
#define INC_VERIFICATION_HPP_

class verification
{
private:
	// variales for test with digital pt1
	// y(k)
	float y;
	// order
	int order;
	// m = order *2
	int m;
	// y vector
	float* yVec;
	// u(k)
	float u;
	// u vector
	float* uVec;




public:

	verification(int order)
	:y(0.0),order(order),m(order*2),yVec(new float[order]),u(0.0),uVec(new float[order])
	{
		for(int i=0; i<order; i++)
		{
		yVec[i] = 0;
		uVec[i] = 0;
		}
	};

	~verification()
	{
		delete yVec;
		delete uVec;
	};

	float verification_output(float x,const float* ab)
	{
		u = x;
		y = 0;

	// calculate new i
		for(int i=0; i<order; i++)
		{
		y += -ab[i]*yVec[i];
		}
		for(int i=order; i<m; i++)
		{
		y += ab[i]*uVec[i-order];
		}

	// reset signal Vectors
		for(int i=(order-1); i>0; i--)
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



};


#endif /* INC_VERIFICATION_HPP_ */
