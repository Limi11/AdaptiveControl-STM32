/*
 * deadbeat_controller.hpp
 *
 *  Created on: 20.07.2020
 *      Author: Milan Liebsch
 */

#ifndef INC_DEADBEAT_CONTROLLER_HPP_
#define INC_DEADBEAT_CONTROLLER_HPP_

// this class implements an adaptive deadbeat controller source "Adaptive und Lernende Regelungen v. Karl Kleinmann"
// use deadbeat controller only for asymptotic stable systems !
// the deadbeat controller tries settle the system in n+1 steps (with n as order of the system to be controlled)
// we can choose the first control output


class deadbeat_controller
{
private:
	int firstControlOutput;
	int deadTime;
	float* deadtimeVector;
	int order;
	// this is the coefficient p array of the controller
	float* pArray;
	// this is the coefficient q array of the controller
	float* qArray;
	// this is the coefficient a array of the system
	float* aArray;
	// this ist the coefficient b array of the system
	float* bArray;
	float* inputArray;
	float* outputArray;
	int firstRound;
	bool negativeOutput;

public:
	deadbeat_controller(int firstControlOutput, int order, int deadtimeMaxTimesteps, bool negativeOutput);
	~deadbeat_controller();
	void getInputs(float input);
	void getNewSystem(float* system, int deadtime);
	void calculateNewController();
	float controll();
};



#endif /* INC_DEADBEAT_CONTROLLER_HPP_ */
