/*
 * testsystem.hpp
 *
 *  Created on: Jun 4, 2020
 *      Author: User
 */

#ifndef INC_TESTSYSTEM_HPP_
#define INC_TESTSYSTEM_HPP_

class testsystem
{
private:
	// variales for test with digital pt1
	// y(k)
	float y;
	// y(k-1)
	float ylast;
	// u
	float u;
	// deltaT
	float deltaT;
	// T=3600
	float T;
	// Kpt1 = 200
	float Kpt1;
	// flag
	int flag;
	// return array
	float *output;



public:

	testsystem()
	:y(0.0),ylast(0.0),u(0.0),deltaT(1000),T(36),Kpt1(200), flag(0), output(new float[2])
	{};

	// test with digital pt1
	// y(k) = y(k-1) + [Kpt1 * u(k) - y(k-1)] * (deltaT/(T+deltaT))
	// deltaT = 1 s, T=3600, Kpt1 = 200

	float* testsystem_output(float u, int t)
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
	}

};




#endif /* INC_TESTSYSTEM_HPP_ */
