// mini4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

int main()
{
	static int myArray[10] = { 0 };
	int currentSmallest = -1;
	int smallestIndex = 0;

	for (int i = 0; i < 10; i++)
	{
		printf("Please enter a number %d/10: ", i + 1);
		myArray[i] = getNum();
		if (myArray[i] < currentSmallest || i == 0)
		{
			currentSmallest = myArray[i];
			smallestIndex = i;
		}
	}

	printf("The smallest value is %d at index %d\n", currentSmallest, smallestIndex);

    return 0;
}

