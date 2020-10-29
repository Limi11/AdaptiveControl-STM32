/*
 * SensorNotifier.hpp
 *
 *  Created on: Oct 29, 2020
 *      Author: User
 */

#ifndef INC_SENSORNOTIFIER_HPP_
#define INC_SENSORNOTIFIER_HPP_

class sensorNotifier
{
private:
	float Temperature;

public:
	sensorNotifier();
	~sensorNotifier();
	void setTemperature(float x);
	float getTemperature();
};



#endif /* INC_SENSORNOTIFIER_HPP_ */
