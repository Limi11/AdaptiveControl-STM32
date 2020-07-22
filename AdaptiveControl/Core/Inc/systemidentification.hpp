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
	int flag;
	int DeadTime;
	int oldDeadTime;
	bool deadTimeFlag;
	float deadTimeBorderHigh;
	float deadTimeBorderLow;
	float expoForget;
	float tolerance;
	float error;
	float measuredOutputNew;
	float measuredOutputOld;
	float* signalInput;
	float* signalOutput;
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

	systemidentification(int order, float expoForget, float tolerance, bool deadtime, float BorderHigh, float BorderLow);
	~systemidentification();


	void getError(float OutputNew);
	void newSignalVector(float OutputNew,float InputNew);
	void newCovarianceMatrix();
	void newCorrectionVector(float OutputNew);
	int newDeadTime();
	float* calculateSystem(float OutputNew,float InputNew);
	float* newParametersVector(float OutputNew);

};



#endif /* INC_SYSTEMIDENTIFICATION_HPP_ */
