/*
*  FILE          : cA1.cpp
*  PROJECT       : PROG1347 - Assignment #1
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2018-02-08
*  DESCRIPTION   :
*    The functions in this file are used to print the letters of teh alphabet as well as a running average of the letter values
*/

#include "stdafx.h"


int main()
{
	char currentLetter = 'a';

	while (currentLetter <= 'z')
	{
		printf("%c\t", currentLetter);
		int avgLetter = currentLetter - ((currentLetter - 'a' + 1) / 2); // add one to compensate for the integer rounding
		if (currentLetter == 'a') // My name begins with 'a'
			printf("%d\n", avgLetter);
		else
			printf("%c\n", avgLetter);
		currentLetter++;	// iterate up one letter once all calculations are done with said letter
	}
    
	return 0;
}

