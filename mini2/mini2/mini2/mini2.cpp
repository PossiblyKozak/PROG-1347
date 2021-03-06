/*
*  FILE          : mini2.cpp
*  PROJECT       : PROG1347 - Mini-Assignment #2
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2018-02-01
*  DESCRIPTION   :
*    The functions in this file are used to increment by one up to 100, and display the sum of all values.
*/

#include "stdafx.h"


int main()
{
	int counter = 0;
	int finalTotal = 0;

	while (counter <= 100)
	{
		finalTotal += counter;
		counter++;
	}

	printf("%d\n", finalTotal);

	return 0;
}

