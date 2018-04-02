/*
*  FILE          : cA3a.cpp
*  PROJECT       : PROG1347 - Assignment #3a
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2012-03-29
*  DESCRIPTION   :
*    The functions in this file are used to find the shortest route from a given airport to a second given airport using the flight times
*    provided in the cA3a_data.h file.
*/

#include "cA3a_data.h"
#include "stdio.h"
#include "string.h"
#include <iostream>

using namespace std;		// to prevent using std:: repeatedly

const int FULLDAY = 2400;	// since the times are stored in HHMM format, 2400 is equivalent to 24 hours or one full day
const int NOON = 1200;		// the half way point in the day when the time switches from a.m. to p.m.



enum cities
{	// the numeric representation of the cities
	TORONTO = 0,
	ATLANTA = 1,
	AUSTIN = 2,
	SANTA_FE = 3,
	DENVER = 4,
	CHICAGO = 5,
	BUFFALO = 6,
	SIZE_OF_CITY_NAME = 10
};



enum timezones
{	// the numeric representation of the timezones of the airports
	EASTERN = 0,
	CENTRAL = 1,
	MOUNTAIN = 2
};


// Prototyping
int getTimezoneModifier(int, int);
int getTimezone(int);
int toFullDay(int);
int convertToTime(int);
void localizedTime(int, int, bool);
char getDigit(int, int);
char intToChar(int);
void printCityHeader(void);
bool getUserInformation(int *, int *, int *);
int getNum(void);



struct node
{	// All parts stored in a flight unit node in the linked list
	int departingCity;
	int arrivingCity;
	int departingTime;
	int arrivingTime;
	node *next;
};



struct flightTime
{	// a struct containing both the takeoff and landing of any given flight
	int flightTakeoff;
	int flightLanding;
};



class list
{
	/*
	*	NAME	: list
	*	PURPOSE	: A singly linked list with the purpose of storing possible flight paths 
	*			  the members store all portions of all flights including takeoff and landing
	*			  as well as the cities leaving and arriving from. 
	*/
private:
	node *head, *tail;
public:
	list()	// initial initialization
	{
		head = NULL;
		tail = NULL;
	}

	void createNode(int departingCity, int arrivingCity, int departingTime, int arrivingTime)	
	{	// Adding another node onto the end of the linked list
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
		/*
		 * FUNCTION		: list.display
		 * DESCRIPTION	: Goes through the nodes in the singly-linked list and displays the information in the required format
		 * PARAMETERS	:	
		 *	int finalTime	: The total time it took for the passenger to reach the destination airport from start to end
		 * RETURNS		:
		 *	VOID
		 */
		char cities[NUM_CITIES][SIZE_OF_CITY_NAME] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
		node *temp = new node;
		temp = head;
		int days = 0;

		while (temp != NULL)
		{
			char out[10] = { 0 };
			cout << "Leaving " << cities[temp->departingCity] << " at ";
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
		cout << "Total travel time: " << finalTime / 100 << ":" << getDigit(finalTime, 2) << getDigit(finalTime, 1) << endl << endl;
	}

	bool notArrived(int checkCity)
	{	// checks the history of cities that the passenger has been and returns if they have previously been at this airport as a boolean.
		bool untouched = true;
		node *temp = new node;
		temp = head;
		while (temp != NULL)
		{
			if (temp->departingCity == checkCity)
			{
				untouched = false;
			}
			temp = temp->next;
		}
		return untouched;
	}

	void clearList()
	{	// Clears the list to prevent spilling into other routes when pathing
		head = NULL;
		tail = NULL;
	}

	void copyList(list ls)
	{	// takes a desired list reference and copies into a new list to create new path
		node *temp = new node;
		temp = ls.head;
		while (temp != NULL)
		{
			createNode(temp->departingCity, temp->arrivingCity, temp->departingTime, temp->arrivingTime);
			temp = temp->next;
		}
	}

	int getStartCity()
	{	// to remove the need to hold onto the original departing city.
		return head->departingCity;
	}

	int getEndTime()
	{	// to return the desired route's end time as the 'currTime' variable may not be the same
		return tail->arrivingTime;
	}
};



void branchFromAirport(list, int, int, int, int*, list*, int);
flightTime getBestFlightOut(int, int, int);



void branchFromAirport(list ls, int currCity, int currTime, int endingCity, int *pCurrFastest, list *pCurrFastestRoute, int departureTime)
{
	/*
	* FUNCTION		: branchFromAirport
	* DESCRIPTION	: 
	*
	* PARAMETERS	:
	*	list ls					: the currently active linked list containing the current route information 
	*	int currCity			: This city which the path is currently in 
	*	int currTime			: The current time in which the list is 
	*	int endingCity			: The destination airport which the path is attempting to go
	*	int *pCurrFastest		: A pointer to the current shortest route's total time
	*	list *pCurrFastestRoute	: A pointer to the list containing the current record for shortest route.
	*	int departureTime		: the original time of departure entered by the user
	* RETURNS		:
	*	VOID
	*/
	if (currCity != endingCity)
	{
		for (int i = TORONTO; i < NUM_CITIES; i++)
		{
			flightTime ft = getBestFlightOut(currTime, currCity, i);
			if (ft.flightLanding != 0 && ls.notArrived(i) && currTime - departureTime + getTimezoneModifier(ls.getStartCity(), currCity) < *pCurrFastest) 
			{   // making sure the current time is lower then the currently agreed fastest route as well as the route hasn't been at this airport previously
				list lsNew;
				lsNew.copyList(ls);
				lsNew.createNode(currCity, i, ft.flightTakeoff, ft.flightLanding + getTimezoneModifier(currCity, i));
				// go further down the route with the current time and airport as the newest node on the list
				branchFromAirport(lsNew, i, ft.flightLanding + getTimezoneModifier(currCity, i), endingCity, pCurrFastest, pCurrFastestRoute, departureTime);				
			}
		}
	}
	else
	{
		if (currTime - departureTime + getTimezoneModifier(ls.getStartCity(), currCity) <= *pCurrFastest || *pCurrFastest == 0)
		{
			list blankList;
			pCurrFastestRoute->clearList();
			*pCurrFastest = currTime - departureTime + getTimezoneModifier(ls.getStartCity(), currCity);
			pCurrFastestRoute->copyList(ls);
		}		
	}
}



flightTime getBestFlightOut(int currTime, int currCity, int destinationCity)
{
	/*
	* FUNCTION		: getBestFlightOut
	* DESCRIPTION	: takes in a start and end location airport as well as the current time which then 
	*				  tries to get the quickest possible arrival time to the destination city after the current time
	*				  which can be in the next calendar day
	* PARAMETERS	:
	*	int currTime		: The current time of the query
	*	int currCity		: The city which the path is currently in
	*	int destinationCity	: The city which the path wants to go to
	* RETURNS		:
	*	flightTime			: the flight which arrives at the destination the quickest
	*/
	int* pFlightOut = getFlights(currCity, destinationCity);
	int allFlightsOut[50] = { 0 };
	int selectedFlightIndex = 0;
	flightTime ft;
	ft.flightLanding = FULLDAY;
	ft.flightTakeoff = currTime;

	if (pFlightOut[0] != 0) // pFlightOut[0] is the number of flights between the two cities, with 0 meaning there are no flights between the cities
	{
		for (int i = pFlightOut[0] * 2 - 1; i > 0 ; i -= 2) 
		{	// the flights are stored in 'departure time' 'flight time' pairs which allows for arrival time calculations.
			allFlightsOut[i - 1] = pFlightOut[i];
			allFlightsOut[i] = pFlightOut[i + 1];
			if (allFlightsOut[i - 1] >= currTime % FULLDAY && convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) < ft.flightLanding)
			{
				ft.flightLanding = convertToTime(allFlightsOut[i - 1] + allFlightsOut[i]) + toFullDay(currTime);
				ft.flightTakeoff = convertToTime(allFlightsOut[i - 1]) + toFullDay(currTime);
				selectedFlightIndex = i - 1;
			}
		}
		if (ft.flightLanding == FULLDAY)
		{
			ft = getBestFlightOut(toFullDay(currTime + FULLDAY), currCity, destinationCity);
		}
	}
	else
	{	// this is for when there are no flights from one airport to another
		ft.flightLanding = 0;
		ft.flightTakeoff = 0;
	}
	return ft;
}



int toFullDay(int input)
{
	/*
	* FUNCTION		: toFullDay
	* DESCRIPTION	: rounds the value to the nearest 24h increment below the current time
	* PARAMETERS	:
	*	int input	: the number to be rounded
	* RETURNS		:
	*	int			: the number after it has been rounded down to the nearest 24h
	*/
	return input / FULLDAY * FULLDAY;
}



int convertToTime(int currTime)
{
	/*
	* FUNCTION		: convertToTime
	* DESCRIPTION	: takes a time value and confirms that it is in the correct format
	* PARAMETERS	:
	*	int currTime	: The time which is to be confirmed that it is in the correct format
	* RETURNS		:
	*	int				: The properly formatted integer
	*/
	int newTime = currTime;
	if (currTime % 100 >= 60)
	{
		newTime += 40;
	}
	return newTime;
}



int getTimezoneModifier(int currCity, int destinationCity)
{
	/*
	* FUNCTION		: getTimezoneModifier
	* DESCRIPTION	: returns the number of hours apart the timezones are between two locations
	* PARAMETERS	:
	*	int currCity		: The current city used as a comparison
	*	int destinationCity	: the destination city which is compared to 
	* RETURNS		:
	*
	*/
	int currTimezone = getTimezone(currCity);
	int destTimezone = getTimezone(destinationCity);
	return ((currTimezone - destTimezone) * 100);	// multiplying by 100 because timezones affect hours
}



int getTimezone(int city)
{
	/*
	* FUNCTION		: getTimezone
	* DESCRIPTION	: gets the timezone of the given city and returns it as in integer
	* PARAMETERS	:
	*	int city	: the index of the city in question
	* RETURNS		:
	*	int the numberic representation of the timezone
	*/
	int currTimezone = EASTERN;

	if (city == TORONTO || city == ATLANTA || city == BUFFALO)
	{
		currTimezone = EASTERN;
	}
	else if (city ==  AUSTIN || city == CHICAGO)
	{
		currTimezone = CENTRAL;
	}
	else if (city == DENVER || city == SANTA_FE)
	{
		currTimezone = MOUNTAIN;
	}

	return currTimezone;
}



void localizedTime(int displayTime, int currCity, bool printNewLine)
{
	/*
	* FUNCTION		: localizedTime
	* DESCRIPTION	: prints to the screen the time properly formatted with timezone and am/pm
	* PARAMETERS	:
	*	int displayTime		: The time to be displayed formatted correctly
	*	int currCity		: The current city for timezone purposes
	*	bool printNewLine	: for if this is to be printed in-line or not
	* RETURNS		:
	*	VOID
	*/
	char timezoneCharacter = ' ';
	char pmOrAm = 'a';
	bool isPM = false;
	int currTimezone = getTimezone(currCity);
	displayTime = displayTime % FULLDAY;
	if (displayTime > NOON)
	{
		pmOrAm = 'p';
		if (displayTime > NOON + 100)
		{
			displayTime -= NOON;
		}
	}
	if (currTimezone == EASTERN)
	{
		timezoneCharacter = 'E';
	}
	else if (currTimezone == CENTRAL)
	{
		timezoneCharacter = 'C';
	}
	else if (currTimezone == MOUNTAIN)
	{
		timezoneCharacter = 'M';
	}

	char tensHours = getDigit(displayTime, 4);
	char onesHours = getDigit(displayTime, 3);
	if (tensHours == '0' && onesHours == '0')
	{
		tensHours = '1';
		onesHours = '2';
	}
	else if (tensHours == '0')
	{
		tensHours = ' ';
	}

	cout << tensHours << onesHours << ':' << getDigit(displayTime, 2) << getDigit(displayTime, 1) << ' ' << pmOrAm << ".m. " << timezoneCharacter << "ST";
	if (printNewLine)
	{
		cout << endl;
	}
}



char getDigit(int inputNum, int numIndex)
{
	/*
	* FUNCTION		: getDigit
	* DESCRIPTION	: Gets the digit of the number at the index provided by numIndex
	* PARAMETERS	:
	*	int inputNum	: The number which is to be cut for a digit
	*	int numIndex	: The index of the digit to be returned
	* RETURNS		:
	*	int				: the single digit at the numIndex index of the inputNum number
	*/
	return intToChar(inputNum / (int)pow(10, numIndex - 1) % 10);
}



char intToChar(int num)
{
	/*
	* FUNCTION		: intToChar
	* DESCRIPTION	: converts an int to a char
	* PARAMETERS	:
	*	int num	: the number to be converted
	* RETURNS		:
	*	char	: the converted int as a char
	*/
	return (char)(num + 48);
}



void printCityHeader(void)
{
	/*
	* FUNCTION		: printCityHeader
	* DESCRIPTION	: 
	*	print the header showing the city names and their index
	* PARAMETERS	:
	*	VOID
	* RETURNS		:
	*	VOID
	*/
	char cities[NUM_CITIES][SIZE_OF_CITY_NAME] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	for (int i = 0; i < 7; i++)
	{
		cout << "City #"<< i << ": " << cities[i] << endl;
	}
	cout << "Enter 7 to exit the program" << endl << endl;
}



bool getUserInformation(int *pStartingCity, int *pEndingCity, int *pDepartureTime)
{
	/*
	* FUNCTION		: getUserInformation
	* DESCRIPTION	: Takes the user's information with regards to the start, end, and time of departure 
	* PARAMETERS	:
	*	int *pStartingCity	: A pointer to the index of the initial departure city
	*	int *pEndingCity	: A pointer to the eventual destination
	*	int *pDepartureTime	: A pointer to the value representing the start time of the journey
	* RETURNS		:
	*	boolean 
	*/
	bool exitProgram = false;
	cout << "Please enter your starting city index: ";
	while (*pStartingCity > BUFFALO || *pStartingCity < TORONTO)
	{
		*pStartingCity = getNum();
		if (*pStartingCity == 7)	// this is the value flag that exits the program
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



#pragma warning(disable: 4996)
int getNum(void)
{
	/*
	* FUNCTION		: getNum
	* DESCRIPTION	: Gets an input from the user
	* PARAMETERS	: 
	*	VOID
	* RETURNS		:
	*	int		: the number that the user inputs
	*/
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



int main(void)
{
	char cities[NUM_CITIES][SIZE_OF_CITY_NAME] = { "Toronto", "Atlanta", "Austin", "Santa Fe", "Denver", "Chicago", "Buffalo" };
	int startingCity = INT32_MAX;
	int endingCity = INT32_MAX; 
	int departureTime = INT32_MAX;
	int currTime;
	int currCity;
	int currFastestTime = INT32_MAX;
	list currFastestRoute;
	bool exitProgram = false;

	while (!exitProgram)
	{
		printCityHeader();		
		exitProgram = getUserInformation(&startingCity, &endingCity, &departureTime);

		// as a means of checking for an exit, set the exitProgram flag to if the code should be executed
		if (!exitProgram)	// repeat until the user requests to exit
		{
			currTime = departureTime;
			currCity = startingCity;

			cout << "Flying from " << cities[startingCity] << " to " << cities[endingCity] << endl;
			cout << "Starting from " << cities[startingCity] << " at ";
			localizedTime(departureTime, currCity, true);

			for (int i = 0; i < NUM_CITIES; i++)
			{
				flightTime ft = getBestFlightOut(currTime, currCity, i);
				if (ft.flightLanding != 0) // the only time that the landing will be at time 0 will be if there is no possible flight paths
				{
					list ls;
					ls.createNode(currCity, i, ft.flightTakeoff, ft.flightLanding + getTimezoneModifier(currCity, i));
					branchFromAirport(ls, i, ft.flightLanding + getTimezoneModifier(currCity, i), endingCity, &currFastestTime, &currFastestRoute, departureTime); 
				}
			}

			int totalTime = currFastestRoute.getEndTime() - departureTime + getTimezoneModifier(startingCity, endingCity);
			if (departureTime % 100 > currFastestRoute.getEndTime() % 100)
			{	// if the time goes down across an hour then subtract an additional 40 because hours is base 60 and int values are base 10 numbers
				totalTime -= 40;
			}
			currFastestRoute.display(totalTime);

			startingCity = INT32_MAX;
			endingCity = INT32_MAX;
			departureTime = INT32_MAX;
		}
	}
	return 0;
}