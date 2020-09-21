/*
 * systemidentification.cpp
 *
 *  Created on: Jun 4, 2020
 *      Author: MilanLiebsch
 */


#include "systemidentification.hpp"
#include "debug.h"
#include <stdio.h>



systemidentification::systemidentification(int order, float expoForget, float errorTolerance, bool deadtime, float deadtimeTolerance, int deadtimeMaxTimesteps)
:timeFactor(10000),order(order),m(2*order),state(0),expoForget(expoForget),error(0.0),errorTolerance(errorTolerance),
 measuredOutputNew(0.0), measuredOutputOld(0.0),deadTimeFlag(deadtime), deadTimeTolerance(deadtimeTolerance), deadTimeMaxTimesteps(deadtimeMaxTimesteps), oldDeadTime(0), deadTime(0),
 signalInput(new float[order]), signalOutput(new float[order]),deadTimeVector(new float[deadtimeMaxTimesteps]),
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
 sysVerification(new verification(order,deadtimeMaxTimesteps))
{
	// add some special values to covariance- and unit- matrix
	//covarianceMatrix({1000.0,0.0,0.0,0.0},{0.0,1000.0,0.0,0.0},{0.0,0.0,1000.0,0.0},{0.0,0.0,0.0,1000.0})
	//, unitMatrix({1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,1.0,0.0},{0.0,0.0,0.0,1.0};
	for(int i=0; i<m; i++)
	{
		covarianceMatrix->setElement(i,i,100.0);
		unitMatrix->setElement(i,i,1.0);
	}
	for(int i=0; i<order; i++)
	{
		signalInput[i]=0.0;
		signalOutput[i]=0.0;
	}
	for(int i=0; i<deadtimeMaxTimesteps; i++)
	{
		deadTimeVector[i]=0.0;
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
	delete deadTimeVector;
	delete sysVerification;
}

float* systemidentification::calculateSystem(float OutputNew,float InputNew)
{

#ifdef _DEBUG
	printf("\r\n\r\n\r\n\r\n\r\n********************** Start System Identification **********************\n\r");
	printf("*************************************************************************\n\r\n");
	printf("Order of system: %d \r\n", order);
#endif

	// Source: "Identifikation Dynamischer Systeme v. Klaus Schwebel"

	// definition of variables
	// P = Covariance Matrix
	// I = Unit Matrix
	// Y= Signal Vector
	// K = Correction Vector
	// O = Parameter Vector
	// y = Output

	*signalVector = *signalVectornew;
	newSignalVector(OutputNew,InputNew);

	// the first two rounds without newCovarianceMatrix
	if(abs(error)>=errorTolerance)
	{
		newCorrectionVector();
		newParametersVector(OutputNew);
		newCovarianceMatrix();
	}

	// we need the new calculation error to decide if we want to start or stop identification

	getError(OutputNew);

	return resultParametersVector();
}

void systemidentification::getError(float OutputNew)
{
	float yverif = sysVerification->verification_output(signalVectornew->getElement(order),resultArray);
	error = abs(OutputNew-yverif);
 	printf("OutputNew: %.2f  \r\n\r\n", OutputNew);
 	printf("yverif: %.2f  \r\n\r\n", yverif);
 	printf("diff: %.2f  \r\n\r\n", error);
}

void systemidentification::newSignalVector(float OutputNew,float InputNew)
{
	// Now we need to update the signalVector, the old k gets k-1, the old k-1 gets k-2 ...
	// The new k comes from the input values. The signal vector saves the output values from
	// 0 until order/2 and the input values from order/2 until order
	// if there is a deadtime the new signalVectors input will be delayed k will be k+d


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

		for(int i=(m-1); i>order; i--)
		{
			signalVectornew->setElement(i,signalVectornew->getElement(i-1));
		}

		if(deadTime != 0)
		{
			printf("deadtimeVector:");
			for(int i=deadTime-1; i>0; i--)
			{
				deadTimeVector[i]=deadTimeVector[i-1];
			}
			deadTimeVector[0]=InputNew;
			signalVectornew->setElement(order,deadTimeVector[deadTime-1]);
			for(int i=0; i<deadTime; i++)
			{
				printf(", %2f", deadTimeVector[i]);
			}
			printf("\r\n\r\n");
			signalVectornew->printVector("signalVector");
		}
		else
		{
			// we need negative y values! therefore -OutputNew ! see source of algorithm RLS
			signalVectornew->setElement(order,InputNew);
		}

	signalVectornew->setElement(0,-OutputNew);



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

	// same calculation in another way
	//(*helpMatrix) = (*helpMatrix) * (*covarianceMatrix);
	//*covarianceMatrix = (*covarianceMatrix)-(*helpMatrix);

	*covarianceMatrix = *covarianceMatrix * (1/expoForget);

#ifdef _DEBUG
	covarianceMatrix->printMatrix("Result covarianceMatrix:");
#endif
}

void systemidentification::newCorrectionVector()
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
	// https://de.wikipedia.org/wiki/RLS-Algorithmus

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

void systemidentification::calculateDeadtime(float OutputNew,float InputNew)
{

#ifdef _DEBUG
	printf("\r\n\r\n\r\n\r\n\r\n********************** Start deadtime Identification **********************\n\r");
	printf("*************************************************************************\n\r\n");
#endif

		for(int i=(order-1); i>0; i--)
		{
			signalInput[i] = signalInput[i-1];
		}

		for(int i=(order-1); i>0; i--)
		{
			signalOutput[i] = signalOutput[i-1];
		}

		signalInput[0] = InputNew;
		signalOutput[0] = OutputNew;


			if(InputNew != 0)
			{
				state = 1;
			}
			if(OutputNew < deadTimeTolerance && state == 1)
			{
					deadTime++;
			}
			else
			{
				state = 0;
			}

#ifdef _DEBUG
			printf("DeadTime: %d  \r\n\r\n", oldDeadTime);
#endif

}

void systemidentification::newParametersVector(float OutputNew)
{
	float helpScalar = 0.0;

	// ********* calculate new parameter vector *********
		// formula: O(k)=O(k-1)+K(k-1)*(y(k)-Y(k)'*O(k-1))

		// calculate helpScalar = (y(k+1)-Y'(k)*O(k-1))

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


/*		this part is now in method resultparametersVector
 	 	for(int i=0; i<m; i++)
		{
			resultArray[i] = parametersVector->getElement(i);
		}

		parametersVector->printVector("Result Parameters Vector:");
		return resultArray;
*/
}

float systemidentification::abs(float x)
{
	if(x<0)
	{
		x = -x;
	}
	return x;
}

int systemidentification::newDeadTime()
{
	return deadTime;
}

float* systemidentification::resultParametersVector()
{

	for(int i=0; i<m; i++)
	{
		resultArray[i] = parametersVector->getElement(i);
	}

	parametersVector->printVector("Result Parameters Vector:");
	return resultArray;
}


