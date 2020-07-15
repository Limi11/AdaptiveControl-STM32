/*
 * systemidentification.cpp
 *
 *  Created on: Jun 4, 2020
 *      Author: MilanLiebsch
 */


#include "systemidentification.hpp"
#include "debug.h"
#include <stdio.h>



systemidentification::systemidentification(int order, float expoForget)
:timeFactor(10000),order(order),m(2*order),flag(0),expoForget(expoForget),error(0.0),
 measuredOutputNew(0.0), measuredOutputOld(0.0),
 signalInput(new float[order]), signalOutput(new float[order]),
 estimatedValue(0.0),resultArray(new float[m]),
 parametersVector(new vector<float>(order,0.0)),
 signalVectornew(new vector<float>(order,0.0)),
 signalVector(new vector<float>(order,0.0)),
 correctionVector(new vector<float>(order,0.0)),
 lastcorrectionVector(new vector<float>(order,0.0)),
 helpVector(new vector<float>(order,0.0)),
 covarianceMatrix(new matrix<float>(order,0.0)),
 unitMatrix(new matrix<float>(order,0.0)),
 helpMatrix(new matrix<float>(order,0.0)),
 sysVerification(new verification(order))
{
	// add some special values to covariance- and unit- matrix
	//covarianceMatrix({1000.0,0.0,0.0,0.0},{0.0,1000.0,0.0,0.0},{0.0,0.0,1000.0,0.0},{0.0,0.0,0.0,1000.0})
	//, unitMatrix({1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,1.0,0.0},{0.0,0.0,0.0,1.0};
	for(int i=0; i<m; i++)
		{
		covarianceMatrix->setElement(i,i,100000.0);
		unitMatrix->setElement(i,i,1.0);
		signalInput[i]=0.0;
		signalOutput[i]=0.0;
		}
}


systemidentification::~systemidentification()
{
	delete signalInput;
	delete signalOutput;
	delete resultArray;
	delete parametersVector;
	delete signalVectornew;
	delete signalVector;
	delete correctionVector;
	delete lastcorrectionVector;
	delete helpVector;
	delete covarianceMatrix;
	delete unitMatrix;
	delete helpMatrix;
}


float* systemidentification::calculateSystem(float OutputNew,float InputNew)
{
	// Source: "Identifikation Dynamischer Systeme v. Klaus Schwebel"

	// definition of variables
	// P = Covariance Matrix
	// I = Unit Matrix
	// Y= Signal Vector
	// K = Correction Vector
	// O = Parameter Vector
	// y = Output

#ifdef _DEBUG
	printf("\r\n\r\n\r\n\r\n\r\n********************** Start System Identification **********************\n\r");
	printf("*************************************************************************\n\r\n");
	printf("Order of system: %d \r\n", order);
#endif

	// first round when flag = 0
	// the first two rounds without newCovarianceMatrix
	if(flag < 3)
	{
		// set old signalVector k+1 to new signalVector k
#ifdef _DEBUG
		printf("\r\nInitialization round %d from 2 \r\n", flag+1);
#endif
		// set signalVector k = k+1
		*signalVector = *signalVectornew;
		// set new signalVector k+1
		newSignalVector(OutputNew,InputNew);
		newCorrectionVector(OutputNew);
		flag++;
	}

	if(flag == 3)
	{
		newCovarianceMatrix();
		*signalVector = *signalVectornew;
		newSignalVector(OutputNew,InputNew);
		newCorrectionVector(OutputNew);
	}

	if(flag == 2)
	{
		flag = 3;
	}

	error = getError();

	return newParametersVector(OutputNew);
}

float systemidentification::getError()
{
	float yverif = sysVerification->verification_output(-signalVectornew->getElement(order),resultArray);
	float diff = -signalVectornew->getElement(0)-yverif;
 	printf("diff: %.2f  \r\n\r\n", diff);
}

void systemidentification::newSignalVector(float OutputNew,float InputNew)
{
	// Now we need to update the signalVector, the old k gets k-1, the old k-1 gets k-2 ...
		// The new k comes from the input values. The signal vector saves the output values from
		// 0 until order/2 and the input values from order/2 until order

	#ifdef _DEBUG
		printf("\r\n\r\n");
		printf("\r\n********* New signal Input ********* \r\n\r\n");
		printf("Signal vector: \r\n");
	#endif

		// we need to start at the end of the signal input or output otherwise we will lose information !
		for(int i=(order-1); i>0; i--)
		{
		signalVectornew->setElement(i,signalVectornew->getElement(i-1));
		}

		for(int i=m; i>order; i--)
		{
		signalVectornew->setElement(i,signalVectornew->getElement(i-1));
		}

		// we need negative y values! therefore -OutputNew ! see source of algorithm RLS
		signalVectornew->setElement(0,-OutputNew);
		signalVectornew->setElement(order,InputNew);



	#ifdef _DEBUG
		signalVector->printVector("Result: Signal Vector k");
		signalVectornew->printVector("Result: Signal Vector k+1");
		printf("\r\n");
	#endif
}

void systemidentification::newCovarianceMatrix()
{
	// ********* calculate new covariance matrix *********
	// formula: P(k+1) = [I-(K(k)*Y'(k))]*P(k)
	// help scalar = (K(k)*Y'(k))

#ifdef _DEBUG
	printf("\r\n\r\n\r\n");
	printf("\r\n********* Calculate Covarince Matrix: P(k+1) = (1/expoForget)*([I-(K(k)*Y'(k))]*P(k)) ********* \r\n\r\n");
	printf("Firstly calculate Help Matrix: K(k)*Y'(k) \r\n");
	printf("Calculation Input: \r\n");
	signalVector->printVector("Signal Vector");
	correctionVector->printVector("Correction Vector");
#endif

    *helpMatrix = (*correctionVector) * (*signalVector);

#ifdef _DEBUG
    helpMatrix->printMatrix("Result Help Matrix:");
	printf("Secondly calculate help matrix: I-helpMatrix \r\n");
	printf("Calculation Input: \r\n");
	unitMatrix->printMatrix("Unit Matrix:");
	helpMatrix->printMatrix("Help Matrix:");
#endif

	*helpMatrix = *unitMatrix-*helpMatrix;

#ifdef _DEBUG
	helpMatrix->printMatrix("Result Help Matrix:");
	printf("Thirdly calculate covarianceMatrix: P(k+1) = (1/expoForget)*([helpMatrix]*P(k)) \r\n");
	printf("Calculation Input: \r\n");
	helpMatrix->printMatrix("Help Matrix:");
	covarianceMatrix->printMatrix("Covariance Matrix:");
#endif

	*covarianceMatrix = (*helpMatrix) * (*covarianceMatrix);
	*covarianceMatrix = *covarianceMatrix * (1/expoForget);

#ifdef _DEBUG
	covarianceMatrix->printMatrix("Result covarianceMatrix:");
#endif
}

void systemidentification::newCorrectionVector(float OutputNew)
{
	float helpScalar = 0.0;

#ifdef _DEBUG
	printf("\r\n\r\n");
	printf("\r\n********* Calculate Correction Vector: K = P(k)*Y(k)/(Y'(k)*P(k)*Y(k)+expoForget) ********* \r\n\r\n");
	printf("Firstly calculate help vector: Y'(k)*P(k) \r\n");
	printf("Calculation Input: \r\n");
	signalVector->printVector("Signal Vector");
	covarianceMatrix->printMatrix("Covariance Matrix:");
#endif

	// ********* calculate correctionVector *********
	// formula: K = P(k)*Y(k)/(Y'(k)*P(k)*Y(k)+1)
	// help Vector1 == Y'(k)*P(k)
	// help Scalar == P(k)*Y(k)
	// help Vector2 == Y'(k)*P(k)
	// formula: K = help Vector/(help Scalar+1)

	*helpVector =(*signalVector)*(*covarianceMatrix);


#ifdef _DEBUG
	helpVector->printVector("Result Help Vector:");
	printf("Secondly calculate help scalar: helpScalar = helpVector*Y \r\n");
	printf("Calculation Input: \r\n");
	helpVector->printVector("Help Vector");
	signalVector->printVector("Signal Vector");
#endif

	helpScalar = helpVector->dot(*signalVector);

#ifdef _DEBUG
	printf("Result Help Scalar: %.2f \r\n" , helpScalar);
	printf("Thirdly calculate new help vector: P(k)*Y(k) \r\n");
	printf("Calculation Input: \r\n");
	covarianceMatrix->printMatrix("Covariance Matrix:");
	signalVector->printVector("Signal Vector");
#endif

	*helpVector = (*covarianceMatrix)*(*signalVector);

#ifdef _DEBUG
	helpVector->printVector("Result Help Vector:");
	printf("Fourthly calculate Correction Vector (help Vector/(help Scalar+1)): \r\n");
	printf("Calculation Input: \r\n");
	helpVector->printVector("Help Vector");
	printf("Help Scalar: %.2f  \r\n", helpScalar);
#endif


	// ********* calculate new correctionVector *********
	// formula: help Vector/(help Scalar+1)

	for(int i=0; i<m; i++)
	{
		float x = helpVector->getElement(i)/(helpScalar+expoForget);
		correctionVector->setElement(i, x);
	}



#ifdef _DEBUG
	correctionVector->printVector("Result Correction Vector:");
	printf("\r\n\r\n\r\n");
#endif

}

float* systemidentification::newParametersVector(float OutputNew)
{
	float helpScalar = 0.0;

	// ********* calculate new parameter vector *********
		// formula: O(k)=O(k-1)+K(k-1)*(y(k)-Y(k)'*O(k-1))

		// calculate helpScalar = (y(k)-Y'(k)*O(k-1))

		helpScalar = OutputNew - signalVector->dot(*parametersVector);

	#ifdef _DEBUG
		printf("\r\n********* Calculate Parameter Vector: O(k)=O(k-1)+K(k-1)*(y(k)-Y(k)'*O(k-1)) ********* \r\n");
		printf("Firstly calculate Help Scalar: helpScalar = y(k)-Y'(k)*O(k-1) \r\n");
		printf("Calculation Input: \r\n");
		printf("Output new: %.2f \r\n" , OutputNew);
		signalVector->printVector("Signal Vector");
		parametersVector->printVector("Parameters Vector");
		printf("Result Help Scalar: %.2f \r\n" , helpScalar);
		printf("Scondly calculate new parametersVector (parametersVector+correctionVector*helpScalar): \r\n");
		printf("Calculation Input: \r\n");
		parametersVector->printVector("old parametersVector");
		correctionVector->printVector("correctionVector");
		printf("helpScalar: %.2f \r\n" , helpScalar);
	#endif

		// calculate O(k)=O(k-1)+K(k-1)*helpScalar

		*parametersVector = (*parametersVector) + ((*correctionVector)*helpScalar);

	#ifdef _DEBUG
		parametersVector->printVector("Result Parameters Vector:");
		printf("\r\n\r\n\r\n");
	#endif


		for(int i=0; i<m; i++)
		{
			resultArray[i] = parametersVector->getElement(i);
		}


		return resultArray;
}
