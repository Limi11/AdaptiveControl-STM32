/*
 * systemidentification.hpp
 *
 *  Created on: Jun 4, 2020
 *      Author: User
 */

#ifndef INC_SYSTEMIDENTIFICATION_HPP_
#define INC_SYSTEMIDENTIFICATION_HPP_


#include <main.hpp>
#include <verification.hpp>
#include "vector.hpp"
#include "matrix.hpp"

/* Define to prevent recursive inclusion -------------------------------------*/

class systemidentification
{
private:
	int timeFactor;
	int order;
	int m;
	int state;
	int deadTime;
	int oldDeadTime;
	bool deadTimeFlag;
	float deadTimeTolerance;
	int deadTimeMaxTimesteps;
	float expoForget;
	float errorTolerance;
	float error;
	float measuredOutputNew;
	float measuredOutputOld;
	float* signalInput;
	float* signalOutput;
	float* deadTimeVector;
	float estimatedValue;
	float* resultArray;
	vector<float>* parametersVector;
	vector<float>* signalVectornew;
	vector<float>* signalVector;
	vector<float>* correctionVector;
	vector<float>* lastcorrectionVector;
	vector<float>* helpVector;
	matrix<float>* covarianceMatrix;
	matrix<float>* unitMatrix;
	matrix<float>* helpMatrix;
	verification* sysVerification;



public:

	systemidentification(int order, float expoForget, float errorTolerance, bool deadtime, float deadtimeTolerance, int deadtimeMaxTimesteps);
	~systemidentification();


	void getError(float OutputNew);
	void newSignalVector(float OutputNew,float InputNew);
	void newCovarianceMatrix();
	void newCorrectionVector();
	void calculateDeadtime(float OutputNew,float InputNew);
	int newDeadTime();
	float abs(float x);
	float* calculateSystem(float OutputNew,float InputNew);
	void newParametersVector(float OutputNew);
	float* resultParametersVector();
};



#endif /* INC_SYSTEMIDENTIFICATION_HPP_ */
