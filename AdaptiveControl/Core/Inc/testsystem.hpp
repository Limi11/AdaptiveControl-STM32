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
	// deadtime
	int deadTime;
	// timer for deadtime
	int deadTimeTimer;
	// return array
	float *output;
	//
	float *deadtimeVector;



public:

	testsystem(int deadtime);

	// test with digital pt1
	// y(k) = y(k-1) + [Kpt1 * u(k) - y(k-1)] * (deltaT/(T+deltaT))
	// deltaT = 1 s, T=3600, Kpt1 = 200

	float* testsystem_output(float u, int t);

};




#endif /* INC_TESTSYSTEM_HPP_ */
