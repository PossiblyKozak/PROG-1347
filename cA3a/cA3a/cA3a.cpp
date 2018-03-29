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

class flightTime
{
private:

public:
	int flightDeparture;
	int flightArrival;
};

class list
{
private:
	node * head, *tail;
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

	void display(int finalTime, int* currentFastest)
	{
		node *temp = new node;
		temp = head;
		int days = 0;

		//cout << "Starting from" << temp->departingCity << " at ";

		while (temp != NULL)
		{
			cout << "Leaving ";
			printCityName(temp->departingCity);
			cout << " at ";
			localizedTime(temp->departingTime, temp->departingCity, false);
			if (temp->departingTime / 2400 > days)
			{
				cout << " next day";
				days++;
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
		cout << "Total travel time: " << finalTime / 100 << ":" << finalTime % 100 / 10 << finalTime % 10 << endl;
		//cout << finalTime << " --> " << *currentFastest << endl;
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

	void clearList()
	{
		head = NULL;
		tail = NULL;
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

	int getStartCity()
	{
		return head->departingCity;
	}

	int getEndTime()
	{
		return tail->arrivingTime;
	}
};

flightTime getBestFlightOut(int, int, int);
void branchFromAirport(list, int, int, int, int*, list*, int);

int main()
{
	char cities[7][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	for (int i = 0; i < 7; i++)
	{
		printf("%d: %s \t--> -%d GMT\n", i, cities[i], getTimezone(i) + 5);
	}
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

	printf("Please enter your departure time (hhmm): ");
	int departureTime = getNum();
	int currentTime = departureTime;

	printf("Flying from %s to %s\n", cities[startingCity], cities[endingCity]);
	printf("Starting from %s at ", cities[startingCity]);
	localizedTime(departureTime, currentCity, true);

	int currentFastest = INT32_MAX;
	list currentFastestRoute;

	for (int i = 0; i < 7; i++)
	{
		flightTime ft = getBestFlightOut(currentTime, currentCity, i);
		if (ft.flightArrival != 0)
		{
			list ls;
			ls.createNode(currentCity, i, ft.flightDeparture, ft.flightArrival + getTimezoneModifier(currentCity, i));
			branchFromAirport(ls, i, ft.flightArrival + getTimezoneModifier(currentCity, i), endingCity, &currentFastest, &currentFastestRoute, departureTime);
		}
	}
	currentFastestRoute.display(currentFastestRoute.getEndTime() - departureTime + getTimezoneModifier(startingCity, endingCity), &currentFastest);
	return 0;
}

void printCityName(int city)
{
	char cities[7][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	printf("%s", cities[city]);
}

void branchFromAirport(list ls, int currentCity, int currentTime, int endingCity, int *currentFastest, list *currentFastestRoute, int departureTime)
{
	if (currentCity != endingCity)
	{
		for (int i = 0; i < 7; i++)
		{
			flightTime ft = getBestFlightOut(currentTime, currentCity, i);
			if (ft.flightArrival != 0 && ls.notArrived(i) && currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity) < *currentFastest)
			{
				list lsNew;
				lsNew.copyList(ls);
				lsNew.createNode(currentCity, i, ft.flightDeparture, ft.flightArrival + getTimezoneModifier(currentCity, i));
				branchFromAirport(lsNew, i, ft.flightArrival + getTimezoneModifier(currentCity, i), endingCity, currentFastest, currentFastestRoute, departureTime);
			}
		}
	}
	else
	{
		if (currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity) <= *currentFastest || *currentFastest == 0)
		{
			list blankList;
			currentFastestRoute->clearList();
			*currentFastest = currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity);
			currentFastestRoute->copyList(ls);
		}		
		//ls.display(currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), endingCity), currentFastest);
	}
}

flightTime getBestFlightOut(int currentTime, int currentCity, int destinationCity)
{
	int* flightOut = getFlights(currentCity, destinationCity);
	int allFlightsOut[32] = { 0 };
	int selectedFlightIndex = 0;
	flightTime ft;
	ft.flightArrival = 2400;
	ft.flightDeparture = currentTime;

	if (flightOut[0] != 0)
	{
		for (int i = flightOut[0] * 2 - 1; i > 0 ; i -= 2)
		{
			allFlightsOut[i - 1] = flightOut[i];
			allFlightsOut[i] = flightOut[i + 1];
			if (allFlightsOut[i - 1] >= currentTime % 2400 && convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) < ft.flightArrival)
			{
				ft.flightArrival = convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) + (currentTime / 2400 * 2400);
				ft.flightDeparture = convertToTime(allFlightsOut[i - 1]) + (currentTime / 2400 * 2400);
				selectedFlightIndex = i - 1;
			}
		}
		if (ft.flightArrival == 2400)
		{
			ft = getBestFlightOut(2400, currentCity, destinationCity);
		}
	}
	else
	{
		ft.flightArrival = 0;
		ft.flightDeparture = 0;
	}
	return ft;
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
	int x = ((currTimezone - destTimezone) * 100);
	return x;
}

int getTimezone(int currentCity)
{
	int currTimezone = 0;
	if (currentCity == 0 || currentCity == 1 || currentCity == 6)
	{
		currTimezone = 0;
	}
	else if (currentCity == 2 || currentCity == 5)
	{
		currTimezone = 1;
	}
	else if (currentCity == 3 || currentCity == 4)
	{
		currTimezone = 2;
	}
	return currTimezone;
}

void localizedTime(int standardTime, int currentCity, bool newLine)
{
	char timezone = ' ';
	bool isPM = false;
	int currTimezone = getTimezone(currentCity);
	standardTime = standardTime % 2400;
	if (standardTime > 1200)
	{
		isPM = true;
		if (standardTime > 1300)
		{
			standardTime -= 1200;
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

	char tensHours = intToChar(standardTime / 1000);
	char onesHours = intToChar((standardTime / 100) % 10);;
	if (tensHours == '0' && onesHours == '0')
	{
		tensHours = '1';
		onesHours = '2';
	}
	else if (tensHours == '0')
	{
		tensHours = ' ';
	}

	char output[15] = { tensHours, onesHours, ':', intToChar((standardTime / 10) % 10), intToChar(standardTime % 10), ' ', isPM ? 'p' : 'a', '.', 'm', '.', ' ', timezone, 'S', 'T', '\0' };
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