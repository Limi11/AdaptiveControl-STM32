/*
 * vector.hpp
 *
 *  Created on: Jun 4, 2020
 *      Author: User
 */

#ifndef INC_VECTOR_HPP_
#define INC_VECTOR_HPP_

#include <stdio.h>
#include "debug.h"
//using namespace std;

template <typename T> class matrix;


template <typename T>
class vector
{
private:
	T *element;
	const int order;
	int m;
	char *Name;

public:

	vector(int order, T a)
	:order(order)
	{
		m = 2*order;
		element = (new T[m]);
		for(int i=0; i<m; i++)
		{
			element[i]=a;
		}
	};

	~vector()
	{
		delete element;
	};

	vector<T> operator+ (const vector<T> &p1)
	{
		vector<T> p2(order,0.0);
		for(int i=0; i<m; i++)
		{
			T x = element[i] + p1.getElement(i);
			p2.setElement(i,x);
		}
		return p2;
	}

 	T dot(const vector<T> &p1)
	{
		T skalarprodukt = 0;
		for(int i=0; i<m; i++)
		{
			skalarprodukt += (this->getElement(i) * p1.getElement(i));
		}
		return skalarprodukt;
	}

	matrix<T> operator* (const vector<T> &p1)
	{
		float temp2 = 0;
		matrix<T> temp1(order,0.0);
		for(int i=0; i<m; i++)
		{
			for(int o=0; o<m; o++)
			{
				temp2 = element[i] * p1.getElement(o);
				temp1.setElement(i,o,temp2);
			}
		}
		return temp1;
	}

	vector<T> operator* (const matrix<T> &m1)
	{
		float temp2;
		vector<T> temp1(order,0.0);
		for(int i=0; i<m; i++)
		{
			temp2 = 0;
			for(int o=0; o<m; o++)
			{
				temp2 = temp2 + (element[o]*m1.getElement(o,i));

			}
			temp1.setElement(i,temp2);
		}
		return temp1;
	}

	vector<T> operator* (T p1)
	{
		vector<T> p2(order,0.0);
		for(int i=0; i<m; i++)
		{
			p2.element[i] = element[i] * p1;
		}
		return p2;
	}

	vector<T> operator/ (T p1)
	{
		for(int i=0; i<m; i++)
		{
			element[i] = element[i] / (1/p1);
		}
		return *this;
	}

	void operator= (const vector<T> &p1) const
	{
		for(int i=0; i<m; i++)
		{
			element[i] = p1.getElement(i);
		}
	}

	void setElement(int idx, T value)
	{
		element[idx] = value;
	}

	T getElement(int idx) const
	{
		return element[idx];
	}

	void printVector(char Name[])
	{
		printf("%s: [", Name );
		for(int i=0; i<m; i++)
		{
			printf("%.4f, ", this->getElement(i));
		}
		printf("]\r\n");
	}


};




#endif /* INC_VECTOR_HPP_ */
