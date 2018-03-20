#include "cA3a_data.h"
#include "stdio.h"
#include "string.h"
#include <iostream>

using namespace std;

const int TORONTO = 0;
const int ATLANTA = 1;
const int AUSTIN = 2;
const int SANTAFE = 3;
const int DENVER = 4;
const int CHICAGO = 5;
const int BUFFALO = 6;

int getNum(void);
void localizedTime(int, int);

int main()
{
	char cities[7][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	int startingCity = 999;
	int endingCity = 999;	

	printf("Please enter your starting city index: ");
	while (startingCity > 6 || startingCity < 0)
	{
		startingCity = getNum();
	}

	printf("Please enter your ending city index: ");
	while (endingCity > 6 || endingCity < 0)
	{
		endingCity = getNum();
	}

	int currentCity = startingCity;

	printf("Please enter your departure time index: ");
	int departureTime = getNum();

	/*while (currentCity != endingCity)
	{

	}*/

	printf("Flying from %s to %s\n", cities[startingCity], cities[endingCity]);

	char time[15] = { 0 };
	printf("Starting from %s at ", cities[startingCity]);
	localizedTime(departureTime, currentCity);	

	double* torontoFlights = getTorontoFlights(1);

	return 0;
}

void localizedTime(int standardTime, int currentCity)
{
	bool isPM = false;
	if (standardTime > 1200)
	{
		isPM = true;
		if (standardTime > 1300)
		{
			standardTime -= 1200;
		}
	}
	char timezone = ' ';
	if (currentCity == 0 || currentCity == 1 || currentCity == 6)
	{
		timezone = 'E';
	}
	else if (currentCity == 3 || currentCity == 4)
	{
		timezone = 'M';
	}
	else if (currentCity == 2 || currentCity == 5)
	{
		timezone = 'C';
	}
	int h10 = (char)(standardTime / 1000);

	char output[15] = { (char)(standardTime / 1000) + 48, (char)((standardTime / 100) % 10) + 48, ':', (char)((standardTime / 10) % 10) + 48 , (char)(standardTime % 10) + 48, ' ', isPM ? 'p' : 'a', '.', 'm', '.', ' ', timezone, 'S', 'T', '\0' };
	printf("%s\n", output);	
}

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