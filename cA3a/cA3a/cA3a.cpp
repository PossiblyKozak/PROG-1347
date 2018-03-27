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
void localizedTime(int, int, bool);
int getBestFlightOut(int, int, int);
int getTimezoneModifier(int, int);
int getTimezone(int);
int convertToTime(int);
char intToChar(int);
void printCityName(int);

struct node
{
	int departingCity;
	int arrivingCity;
	int departingTime;
	int arrivingTime;
	node *next;
};

class list
{
private:
	node *head, *tail;
public:
	list()
	{
		head = NULL;
		tail = NULL;
	}

	void createNode(int departingCity, int arrivingCity, int departingTime, int arrivingTime)
	{
		node *temp = new node;
		temp->departingCity = departingCity;
		temp->arrivingCity = arrivingCity;
		temp->departingTime = departingTime;
		temp->arrivingTime = arrivingTime;
		temp->next = NULL;
		if (head == NULL)
		{
			head = temp;
			tail = temp;
			temp = NULL;
		}
		else
		{
			tail->next = temp;
			tail = temp;
		}
	}

	void display()
	{
		node *temp = new node;
		temp = head;

		//cout << "Starting from" << temp->departingCity << " at ";

		while (temp != NULL)
		{
			cout << "Leaving ";
			printCityName(temp->departingCity);
			cout << " at ";
			localizedTime(temp->departingTime, temp->arrivingCity, false);
			if (temp->departingTime > temp->arrivingTime)
			{
				cout << " next day";
			}
			cout << " for ";
			printCityName(temp->arrivingCity);
			cout << "\n" << "Arriving in ";
			printCityName(temp->arrivingCity);
			cout << " at ";
			localizedTime(temp->arrivingTime, temp->arrivingCity, false);		
			cout << "\n";
			temp = temp->next;
		}
	}

	bool notArrived(int arrivingCity)
	{
		bool untouched = true;
		node *temp = new node;
		temp = head;
		while (temp != NULL)
		{
			if (temp->departingCity == arrivingCity)
			{
				untouched = false;
			}
			temp = temp->next;
		}
		return untouched;
	}

	void copyList(list ls)
	{
		node *temp = new node;
		temp = ls.head;
		while (temp != NULL)
		{
			createNode(temp->departingCity, temp->arrivingCity, temp->departingTime, temp->arrivingTime);
			temp = temp->next;
		}
	}
};

void branchFromAirport(list, int, int, int);

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
	int currentTime = departureTime;

	printf("Flying from %s to %s\n", cities[startingCity], cities[endingCity]);
	printf("Starting from %s at ", cities[startingCity]);
	localizedTime(departureTime, currentCity, true);	

	for (int i = 0; i < 7; i++)
	{
		int possibleDirection = getBestFlightOut(currentTime, currentCity, i);
		if (possibleDirection != 0)
		{
			list ls;
			ls.createNode(currentCity, i, currentTime, possibleDirection + getTimezoneModifier(currentCity, endingCity));
			branchFromAirport(ls, i, possibleDirection + getTimezoneModifier(currentCity, endingCity), endingCity);
		}
	}

	return 0;
}

void printCityName(int city)
{
	char cities[7][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	printf("%s", cities[city]);	
}

void branchFromAirport(list ls, int currentCity, int currentTime, int endingCity)
{
	if (currentCity != endingCity)
	{
		for (int i = 0; i < 7; i++)
		{
			int possibleDirection = getBestFlightOut(currentTime, currentCity, i);
			if (possibleDirection != 0 && ls.notArrived(i))
			{
				list lsNew;
				lsNew.copyList(ls);
				lsNew.createNode(currentCity, i, currentTime, possibleDirection + getTimezoneModifier(currentCity, endingCity));
				branchFromAirport(lsNew, i, possibleDirection + getTimezoneModifier(currentCity, endingCity), endingCity);
			}
		}
	}
	else
	{
		ls.display();
		printf("\n");
	}
}

int getBestFlightOut(int currentTime, int currentCity, int destinationCity)
{
	int* flightOut = getFlights(currentCity, destinationCity);
	int allFlightsOut[32] = { 0 };
	int selectedFlightIndex = 0;
	int selectedArrivalTime = 2400;

	if (flightOut[0] != 0)
	{
		for (int i = 1; i < flightOut[0] * 2; i += 2)
		{
			allFlightsOut[i - 1] = flightOut[i];
			allFlightsOut[i] = flightOut[i + 1];
			if (allFlightsOut[i - 1] >= currentTime % 2400 && convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) < selectedArrivalTime)
			{
				selectedArrivalTime = convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]);
				selectedFlightIndex = i - 1;
			}
		}
		if (selectedArrivalTime == 2400)
		{
			selectedArrivalTime = getBestFlightOut(2400, currentCity, destinationCity);
		}
	}
	else
	{
		selectedArrivalTime = 0;
	}
	return selectedArrivalTime;
}

int convertToTime(int time)
{
	int newTime = time;
	if (time % 100 >= 60)
	{
		newTime += 40;
	}
	return newTime;
}

int getTimezoneModifier(int currentCity, int destinationCity)
{
	int currTimezone = getTimezone(currentCity);
	int destTimezone = getTimezone(destinationCity);
	return (destTimezone - currTimezone * 100);
}

int getTimezone(int currentCity)
{
	int currTimezone = 0;
	if (currentCity == 0 || currentCity == 1 || currentCity == 6)
	{
		currTimezone = 2;
	}
	else if (currentCity == 2 || currentCity == 5)
	{
		currTimezone = 1;
	}
	else if (currentCity == 3 || currentCity == 4)
	{
		currTimezone = 0;
	}
	return currTimezone;
}

void localizedTime(int standardTime, int currentCity, bool newLine)
{
	char timezone = ' ';
	bool isPM = false;
	int currTimezone = getTimezone(currentCity);

	if (standardTime > 1200)
	{
		isPM = true;
		if (standardTime > 1300)
		{
			//standardTime -= 1200;
		}
	}		
	if (currTimezone == 0)
	{
		timezone = 'E';
	}
	else if (currTimezone == 1)
	{
		timezone = 'C';
	}
	else if (currTimezone == 2)
	{
		timezone = 'M';
	}

	int h10 = (char)(standardTime / 1000);

	char output[15] = { intToChar(standardTime / 1000), intToChar((standardTime / 100) % 10), ':', intToChar((standardTime / 10) % 10), intToChar(standardTime % 10), ' ', isPM ? 'p' : 'a', '.', 'm', '.', ' ', timezone, 'S', 'T', '\0' };
	printf("%s", output);	
	if (newLine)
	{
		printf("\n");
	}
}

char intToChar(int time)
{
	return (char)(time + 48);
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