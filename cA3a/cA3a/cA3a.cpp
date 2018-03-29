#include "cA3a_data.h"
#include "stdio.h"
#include "string.h"
#include <iostream>

using namespace std;

const int FULLDAY = 2400;
const int NOON = 1200;



enum cities
{
	NUM_CITIES = 7,
	TORONTO = 0,
	ATLANTA = 1,
	AUSTIN = 2,
	SANTAFE = 3,
	DENVER = 4,
	CHICAGO = 5,
	BUFFALO = 6
};



enum timezones
{
	EASTERN = 0,
	CENTRAL = 1,
	MOUNTAIN = 2
};



int getNum(void);
void localizedTime(int, int, bool);
int getTimezoneModifier(int, int);
int getTimezone(int);
int convertToTime(int);
char intToChar(int);



struct node
{
	int departingCity;
	int arrivingCity;
	int departingTime;
	int arrivingTime;
	node *next;
};



struct flightTime
{
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

	void display(int finalTime)
	{
		char cities[NUM_CITIES][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
		node *temp = new node;
		temp = head;
		int days = 0;

		while (temp != NULL)
		{
			char out[10] = { 0 };
			cout << cities[temp->arrivingCity] << "Leaving " << cities[temp->departingCity] << " at ";
			localizedTime(temp->departingTime, temp->departingCity, false);
			if (temp->departingTime / FULLDAY > days)
			{
				cout << " next day";
				days++;
			}
			cout << " for " << cities[temp->arrivingCity] << "\n" << "Arriving in " << cities[temp->arrivingCity] << " at ";
			localizedTime(temp->arrivingTime, temp->arrivingCity, false);
			cout << "\n";
			temp = temp->next;
		}
		cout << "Total travel time: " << finalTime / 100 << ":" << finalTime % 100 / 10 << finalTime % 10 << endl << endl;
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



void branchFromAirport(list, int, int, int, int*, list*, int);
flightTime getBestFlightOut(int, int, int);



void branchFromAirport(list ls, int currentCity, int currentTime, int endingCity, int *pCurrentFastest, list *pCurrentFastestRoute, int departureTime)
{
	if (currentCity != endingCity)
	{
		for (int i = TORONTO; i < NUM_CITIES; i++)
		{
			flightTime ft = getBestFlightOut(currentTime, currentCity, i);
			if (ft.flightArrival != 0 && ls.notArrived(i) && currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity) < *pCurrentFastest)
			{
				list lsNew;
				lsNew.copyList(ls);
				lsNew.createNode(currentCity, i, ft.flightDeparture, ft.flightArrival + getTimezoneModifier(currentCity, i));
				branchFromAirport(lsNew, i, ft.flightArrival + getTimezoneModifier(currentCity, i), endingCity, pCurrentFastest, pCurrentFastestRoute, departureTime);
			}
		}
	}
	else
	{
		if (currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity) <= *pCurrentFastest || *pCurrentFastest == 0)
		{
			list blankList;
			pCurrentFastestRoute->clearList();
			*pCurrentFastest = currentTime - departureTime + getTimezoneModifier(ls.getStartCity(), currentCity);
			pCurrentFastestRoute->copyList(ls);
		}		
	}
}



flightTime getBestFlightOut(int currentTime, int currentCity, int destinationCity)
{
	int* flightOut = getFlights(currentCity, destinationCity);
	int allFlightsOut[32] = { 0 };
	int selectedFlightIndex = 0;
	flightTime ft;
	ft.flightArrival = FULLDAY;
	ft.flightDeparture = currentTime;

	if (flightOut[0] != 0)
	{
		for (int i = flightOut[0] * 2 - 1; i > 0 ; i -= 2)
		{
			allFlightsOut[i - 1] = flightOut[i];
			allFlightsOut[i] = flightOut[i + 1];
			if (allFlightsOut[i - 1] >= currentTime % FULLDAY && convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) < ft.flightArrival)
			{
				ft.flightArrival = convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) + (currentTime / FULLDAY * FULLDAY);
				ft.flightDeparture = convertToTime(allFlightsOut[i - 1]) + (currentTime / FULLDAY * FULLDAY);
				selectedFlightIndex = i - 1;
			}
		}
		if (ft.flightArrival == FULLDAY)
		{
			ft = getBestFlightOut((currentTime + FULLDAY) / FULLDAY * FULLDAY, currentCity, destinationCity);
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
	int currTimezone = EASTERN;
	if (currentCity == TORONTO || currentCity == ATLANTA || currentCity == BUFFALO)
	{
		currTimezone = EASTERN;
	}
	else if (currentCity ==  AUSTIN || currentCity == CHICAGO)
	{
		currTimezone = CENTRAL;
	}
	else if (currentCity == DENVER || currentCity == SANTAFE)
	{
		currTimezone = MOUNTAIN;
	}
	return currTimezone;
}



void localizedTime(int standardTime, int currentCity, bool newLine)
{
	char timezone = ' ';
	bool isPM = false;
	int currTimezone = getTimezone(currentCity);
	standardTime = standardTime % FULLDAY;
	if (standardTime > NOON)
	{
		isPM = true;
		if (standardTime > NOON + 100)
		{
			standardTime -= NOON;
		}
	}
	if (currTimezone == EASTERN)
	{
		timezone = 'E';
	}
	else if (currTimezone == CENTRAL)
	{
		timezone = 'C';
	}
	else if (currTimezone == MOUNTAIN)
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
	cout << output;
	if (newLine)
	{
		cout << endl;
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



void printCityHeader()
{
	char cities[NUM_CITIES][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	for (int i = 0; i < 7; i++)
	{
		cout << "City #"<< i << ": " << cities[i] << endl;
	}
	cout << "Enter 7 to exit the program" << endl << endl;
}



bool getUserInformation(int * pStartingCity, int * pEndingCity, int * pDepartureTime)
{
	bool exitProgram = false;
	cout << "Please enter your starting city index: ";
	while (*pStartingCity > BUFFALO || *pStartingCity < TORONTO)
	{
		*pStartingCity = getNum();
		if (*pStartingCity == 7)
		{
			exitProgram = true;
			break;
		}
	}

	if (!exitProgram)
	{
		cout << "Please enter your ending city index: ";
		while (*pEndingCity > BUFFALO || *pEndingCity < TORONTO)
		{
			*pEndingCity = getNum();
		}

		cout << "Please enter your departure time (hhmm): ";
		*pDepartureTime = getNum();
		cout << endl;
	}
	return exitProgram;
}



int main()
{
	char cities[NUM_CITIES][10] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	int startingCity = INT32_MAX;
	int endingCity = INT32_MAX; 
	int departureTime = INT32_MAX;
	int currentTime;
	int currentCity;
	int currentFastest = INT32_MAX;
	list currentFastestRoute;
	bool exitProgram = false;

	while (!exitProgram)
	{
		printCityHeader();
		exitProgram = getUserInformation(&startingCity, &endingCity, &departureTime);

		if (!exitProgram)
		{
			currentTime = departureTime;
			currentCity = startingCity;

			cout << "Flying from " << cities[startingCity] << " to " << cities[endingCity] << endl;
			cout << "Starting from " << cities[startingCity] << " at ";
			localizedTime(departureTime, currentCity, true);

			for (int i = TORONTO; i < NUM_CITIES; i++)
			{
				flightTime ft = getBestFlightOut(currentTime, currentCity, i);
				if (ft.flightArrival != 0)
				{
					list ls;
					ls.createNode(currentCity, i, ft.flightDeparture, ft.flightArrival + getTimezoneModifier(currentCity, i));
					branchFromAirport(ls, i, ft.flightArrival + getTimezoneModifier(currentCity, i), endingCity, &currentFastest, &currentFastestRoute, departureTime);
				}
			}
			int totalTime = currentFastestRoute.getEndTime() - departureTime + getTimezoneModifier(startingCity, endingCity);
			if (departureTime % 100 > currentFastestRoute.getEndTime() % 100)
			{
				totalTime -= 40;
			}
			currentFastestRoute.display(totalTime);

			startingCity = INT32_MAX;
			endingCity = INT32_MAX;
			departureTime = INT32_MAX;
		}
	}
	return 0;
}