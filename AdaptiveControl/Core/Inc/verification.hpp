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
	// deadtime
	int deadtime;
	//
	float* deadTimeVector;


public:

	verification(int order, int deadTimeMaxTimesteps);
	~verification();
	float verification_output(float x,const float* ab);

};


#endif /* INC_VERIFICATION_HPP_ */
