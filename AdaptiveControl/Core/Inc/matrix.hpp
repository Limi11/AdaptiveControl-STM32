/*
 * matrix.hpp
 *
 *  Created on: Jun 4, 2020
 *      Author: User
 */




#ifndef INC_MATRIX_HPP_
#define INC_MATRIX_HPP_


#include <stdio.h>

// Forward declare the class template
template <typename T> class matrix;
template <typename T> class vector;

// Declare the function
//template <typename T> matrix<T> operator- (matrix<T> &m1, matrix<T> &m2);
//template <typename T> matrix<T> operator* (matrix<T> m1, T x);
//template <typename T> matrix<T> operator* (matrix<T> m1, T x);



template <typename T>
class matrix
{
private:

	const int order;
	const int m;
	T **element;

public:

	matrix(const int order, T a)
	:order(order),m(2*order)
	{
		element = new T*[m];
		for(int i = 0; i < m; i++)
		{
		    element[i] = new T[m];
		}

		for(int i=0; i<m; i++)
		{
			for(int o=0; o < m; o++)
			{
			element[i][o]=0;
			}
		}
	};


	~matrix()
	{
		for(int i = 0; i<m; i++)
		{
		    delete [] element[i];
		}
		delete [] element;
	}

	void setElement(int idx1, int idx2, T value)
	{
		element[idx1][idx2] = value;
	}

	T getElement(int idx1, int idx2) const
	{
		return element[idx1][idx2];
	}

	int getOrder()
	{
		return m;
	}

	void printMatrix(char Name[])
	{
		printf("%s: \n\r", Name);
		for(int i=0; i<m; i++)
			{
				printf("| ");
				for(int o=0; o<m; o++)
					{
							printf(" %.4f ", element[i][o]);
					}
				printf("| \n\r");
			}
		printf("\n\r");
	}

	void operator = (const matrix<T> &m1)
	{
		float temp2;
		for(int i=0; i<m; i++)
				{
					for(int o=0; o<m; o++)
					{
						temp2 = m1.getElement(i,o);
						element[i][o] = temp2;
					}
				}
	}


	matrix<T> operator * (T x)
	{
			matrix<T> temp1(order,0.0);
			for(int i=0; i<m; i++)
				{
					for(int o=0; o<m; o++)
					{
						 float temp2 = element[i][o]*x;
						 temp1.setElement(i,o,temp2);
					}
				}
			return temp1;
	}

	vector<T> operator * (const vector<T> &p1)
		{
			vector<T> p2(order,0.0);
			T x = 0;

			for(int i=0; i<m; i++)
				{
					for(int o=0; o<m; o++)
					{
						x += element[i][o]*p1.getElement(o);
					}
					p2.setElement(i,x);
					x = 0;
				}
			return p2;
		}

	matrix<T> operator * (const matrix<T> &p1)
		{
			matrix<T> p2(order,0.0);
			T x = 0;

			for(int i=0; i<m; i++)
				{
					for(int o=0; o<m; o++)
					{
						for(int p=0; p<m; p++)
						{
						x += element[i][p]*p1.getElement(p,o);
						}
					p2.setElement(i,o,x);
					x = 0;
					}
				}
			return p2;
		}



	matrix<T> operator - (const matrix<T> &m2)
	{
		matrix<T> temp1(order,0.0);
		float temp2;
		for(int i=0; i<m; i++)
			{
				for(int o=0; o<m; o++)
				{
					temp2 = (element[i][o]- m2.getElement(i,o));
					temp1.setElement(i,o,temp2);
					//printf(" %f \n\r", element[i][o]);
				}
			}

		return temp1;
	}

	matrix<T> operator - (T x)
	{
		matrix<T> temp1(order,0.0);
		float temp2;
		for(int i=0; i<m; i++)
			{
				for(int o=0; o<m; o++)
				{
					temp2 = (element[i][o]- x);
					temp1.setElement(i,o,temp2);
					//printf(" %f \n\r", element[i][o]);
				}
			}

		return temp1;
	}


};



#endif /* INC_MATRIX_HPP_ */
