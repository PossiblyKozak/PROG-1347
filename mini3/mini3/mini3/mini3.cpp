/*
*  FILE          : mini3.cpp
*  PROJECT       : PROG1347 - Mini-Assignment #3
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2018-02-08
*  DESCRIPTION   :
*    The functions in this file are used to take a number from the user and print if the number is even or odd.
*/

#include "stdafx.h"

bool isOdd(int inputNumber);
int getNum(void);

#pragma warning(disable: 4996)
int getNum(void)
{
	/* the array is 121 bytes in size; we'll see in a later lecture how we can improve this code */
	char record[121] = { 0 }; /* record stores the string */
	int number = 0;
	/* NOTE to student: indent and brace this function consistent with your others */
	/* use fgets() to get a string from the keyboard */
	fgets(record, 121, stdin);
	/* extract the number from the string; sscanf() returns a number
	* corresponding with the number of items it found in the string */
	if (sscanf(record, "%d", &number) != 1)
	{
		/* if the user did not enter a number recognizable by
		* the system, set number to -1 */
		number = -1;
	}
	return number;
}

bool isOdd(int inputNumber)
{
	/*
	FUNCTION    : isOdd
	DESCRIPTION : Takes in a number and outputs wether or not that number is odd
	PARAMETERS  :
	int inputNumber	: The number that the user had input
	RETURNS     :
	boolean : True if the number is odd, false if the number is even.
	*/
	return (inputNumber % 2 != 0);
}

int main()
{
	printf("Please enter a number: ");
	if (isOdd(getNum()))
	{
		printf("\nYour number is odd.\n");
	}
	else
	{
		printf("\nYour number is even.\n");
	}
	return 0;
}

