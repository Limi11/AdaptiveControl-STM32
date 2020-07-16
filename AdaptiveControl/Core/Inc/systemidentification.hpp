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

	systemidentification(int, float, float);

	~systemidentification();

	float* calculateSystem(float OutputNew,float InputNew);
	void getError();
	void newSignalVector(float OutputNew,float InputNew);
	void newCovarianceMatrix();
	void newCorrectionVector(float OutputNew);
	float* newParametersVector(float OutputNew);

};



#endif /* INC_SYSTEMIDENTIFICATION_HPP_ */
