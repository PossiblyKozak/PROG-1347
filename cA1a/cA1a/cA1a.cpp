/*
*  FILE          : cA1a.cpp
*  PROJECT       : PROG1347 - Assignment #1A
*  PROGRAMMER    : Alex Kozak
*  FIRST VERSION : 2018-02-08
*  DESCRIPTION   :
*    The functions in this file are used to play a full game of deal or no deal, repeating until the user exits the program
*/

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm> 

using namespace std;				// used to simplify the shuffling of the array syntax

// each of these defines represents the extended askii for double line border characters
#define TOP_LEFT_CORNER 201			// ╔
#define BOTTOM_LEFT_CORNER 200		// ╚
#define TOP_RIGHT_CORNER 187		// ╗
#define BOTTOM_RIGHT_CORNER 188		// ╝
#define HORIZONTAL_LINE 205			// ═
#define VERTICAL_LINE 186			// ║
#define HORIZONTAL_UP 202			// ╩
#define HORIZONTAL_DOWN 203			// ╦
#define VERTICAL_RIGHT 204			// ╠
#define VERTICAL_LEFT 185			// ╣
#define ALL_WAY 206					// ╬

#define TOTAL_BOARD_WIDTH 119							// This is the total width of the game board in characters
#define NUMBER_OF_COLUMNS 4 							// The number of displayed columns. On a 1920x1080 screen, the maximum is 10 columns before it is consistently split
#define CASE_STRING "%c  Case %d: "						// The string which displays the case in the game board
#define cursorforward(x) printf("\033[%dC", (x))		// Moves the cursor x characters forward
#define cursorbackward(x) printf("\033[%dD", (x))		// Moves the cursor x characters backward
#define newLine() printf("\n")							// Prints a new line

#pragma warning(disable: 4996)
#pragma warning(disable: 6285)

// prototyping
int getNum(void);
int getTotalWidthOfGameBoard();
void printInternal(const char* input, bool containsNumber, double formatNum, double formatNum2, int spacesBefore, int spacesAfter);
void printBorderJustified(const char* input, bool containsNumber, double formatNum, double formatNum2, char justification, bool isDecimal);
void printLine(char sides, char centers, bool addColumns);
void printInputWindow(void);
void printLogo(void);
void clearPreviousLine(int repeatNumber);
void printAvailableCashValues(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase);
void printGameBoard(double caseValues[], int mixedCaseValueIndex[], int userSelectedCase, bool revealedCases[], bool showAllCashValues);
int selectCase(double caseValues[], int numberOfSelections, bool revealedCases[], int mixedCaseValueIndex[], int userSelectedCase);
bool yesOrNo(void);
double offerDeal(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase);
int finalCaseSwapSelection(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase);
void playGame(void);
int main(void);

int getNum(void)
{
	/*
	function limitation: only accepts 120 characters of input
	*/
	char record[121] = { 0 };
	int number = 0;
	/* NOTE to student: indent and brace this function consistent with your other functions
	   use  fgets() to get a string from the keyboard */
	fgets(record, 121, stdin);
	/* extract  the number from the string; sscanf() returns a number
	* corresponding with the number of items it found in the string */
	if (sscanf(record, "%d", &number) != 1)
	{
		/*  if the user did not enter a number recognizable by
		* the system, set number to	-1 */
		number = -1;
	}
	return  number;
}

int getTotalWidthOfGameBoard()
{
	/*
	FUNCTION	: getTotalWidthOfGameBoard
	DESCRIPTION	:
	This function prints a given input, justified in the center right or right,
	between two vertical bars on either side, for use in creating full borders.
	PARAMETERS	:
	VOID
	RETURNS		:
	The total width of the game board in number of characters
	*/
	int totalWidth = (TOTAL_BOARD_WIDTH / NUMBER_OF_COLUMNS) * NUMBER_OF_COLUMNS;	// To avoid rounding errors
	if (totalWidth < NUMBER_OF_COLUMNS * 21)
	{
		// 21 characters is the minimum characters where the width of a 2 digit case, 
		// one border wall, the case title, and $1000000 can fit without tearing. 
		totalWidth = NUMBER_OF_COLUMNS * 21;	// if it is below the minimum width, set to the minimum
	}
	return totalWidth;
}

void printInternal(const char* input, bool containsNumber, double formatNum, double formatNum2, int spacesBefore, int spacesAfter)
{
	/*
	FUNCTION	: printInternal
	DESCRIPTION	:
	This function prints a given input, justified in the center right or right,
	between two vertical bars on either side, for use in creating full borders.
	PARAMETERS	:
	const char* input		: string that is going to be printed
	bool containsNumber		: boolean if there is a number to be displayed or not
	double formatNum		: first of two possible numbers to display
	double formatNum2		: second possible number to display.
	int spacesBefore		: the character number of spaces required before the input is displayed
	int spacesAfter			: the character number of spaces required after the input is displayed
	RETURNS		:
	VOID
	*/
	if (spacesBefore != 0) // if spacesBefore == 0, you don't want to display any spaces, and if you let it run, it would still print one space, even though the number given is 0
	{
		printf("%*c", spacesBefore, ' ');
	}
	if (containsNumber)
	{
		if (formatNum2 == 0) // formatNum2 is only 0 if there is no content to display.
			printf(input, formatNum);
		else
			printf(input, formatNum, formatNum2);
	}
	else
	{
		printf(input);
	}
	if (spacesAfter != 0) // same as spacesBefore
	{
		printf("%*c", spacesAfter, ' ');
	}
}

void printBorderJustified(const char* input, bool containsNumber, double formatNum, double formatNum2, char justification, bool isDecimal)
{
	/*
	FUNCTION	: printBorderJustified
	DESCRIPTION	: 
		This function prints a given input, justified in the center right or right, 
		between two vertical bars on either side, for use in creating full borders.
	PARAMETERS	:
		const char* input		: string that is going to be printed
		bool containsNumber		: boolean if there is a number to be displayed or not
		double formatNum		: first of two possible numbers to display
		double formatNum2		: second possible number to display.
		char justification		: values 'c', 'l', or 'r' to set justification of input text
	RETURNS		:
		VOID
	*/
	int inputSize;						// The total number of characters in the input string
	int halfWidth;						// used for getting half of the required whitespace. For centering.
	double halfInputSize;				// used for getting half of the side of the input. For centering.
	int totalWidth = getTotalWidthOfGameBoard();	
		
	for (inputSize = 0; input[inputSize] != '\0'; ++inputSize);	// iterate over the input char*, set inputsize to the character count

	if (containsNumber)	
	{
		if (!isDecimal) // decimal inputs use %.2f instead of %.0f, and therefore have 3 extra digits to display '.00' 
		{
			if (formatNum2 == 0) // since there is no second number
			{
				inputSize -= 3;										// remove the '.00'
				inputSize += abs((int)(log10(formatNum) + 1));		// add the number of characters in the number 
			}
			else
			{
				inputSize -= 6;																	// remove '.00' x 2 
				inputSize += abs((int)(log10(formatNum)) + 1) + abs((int)(log10(formatNum2)));	// add the number of characters in both numbers
			}
		}
		else
		{
			if (formatNum2 == 0)	// same as above but without removing '.00' because the two numbers are not decimals
			{
				inputSize += abs((int)(log10(formatNum) + 1));	
			}
			else
			{
				inputSize += abs((int)(log10(formatNum)) + 1) + abs((int)(log10(formatNum2)));
			}
		}
	}
	else if (justification == 'c')
	{
		inputSize += 1; // Add one because if the content is centered, the whitespace rounds down, in order to round it up, 1 must be added
	}
	
	printf("%c", VERTICAL_LINE); // leftmost vertical line

	switch (justification)
	{
	case 'c':	// centered justification. 
		halfInputSize = inputSize / (double)2;							// divided by a double to preserve digits or precision before
		halfWidth = (int)(((totalWidth / (double)2) - halfInputSize));	// rounding down to the nearest whole number for number of displayed characters
		printInternal(input, containsNumber, formatNum, formatNum2, halfWidth, totalWidth - (halfWidth + inputSize));
		break;
	case 'r':
		printInternal(input, containsNumber, formatNum, formatNum2, totalWidth - (inputSize + 1), 0);
		break;
	case 'l':
		printInternal(input, containsNumber, formatNum, formatNum2, 0, totalWidth - (inputSize + 1));
		break;
	}	
	printf("%c\n", VERTICAL_LINE);
}

void printLine(char sides, char centers, bool addColumns)
{
	/*
	FUNCTION	: printLine
	DESCRIPTION	:
		The function prints a horizontal line with end caps and column caps defined by the parameters.
	PARAMETERS	:
		char sides				: 'd', 'u', or 'm' which sets the type of side character to display
		char centers			: 'd', 'u', or 'm' which sets the type of center character to display
		bool addColumns			: whether to ignore the centers to the line or not
	RETURNS		:
		VOID
	*/
	int totalWidth = getTotalWidthOfGameBoard();
	// setting the default line to be ╠════╣ while declaring the left edge, the centers, and the right edge connector type
	int leftEdgeConnectorType = VERTICAL_RIGHT;
	int rightEdgeConnectorType = VERTICAL_LEFT;
	int columnLineConnectorType = HORIZONTAL_LINE;	

	switch (sides)
	{
	case 'd': // corners facing 'd'own. ╔ ╗
		leftEdgeConnectorType = TOP_LEFT_CORNER;
		rightEdgeConnectorType = TOP_RIGHT_CORNER;
		break;
	case 'u': // corners facing 'u'p. ╚ ╝
		leftEdgeConnectorType = BOTTOM_LEFT_CORNER;
		rightEdgeConnectorType = BOTTOM_RIGHT_CORNER;
		break;
	case 'm': // 'm'iddle of a line sides ╠ ╣
		leftEdgeConnectorType = VERTICAL_RIGHT;
		rightEdgeConnectorType = VERTICAL_LEFT;
		break;
	}

	switch (centers)
	{
	case 'd': // center facing 'd'own ╦
		columnLineConnectorType = HORIZONTAL_DOWN;
		break;
	case 'u': // center facing 'u'p ╩
		columnLineConnectorType = HORIZONTAL_UP;
		break;
	case 'm': // 'm'iddle facing in all directions ╬
		columnLineConnectorType = ALL_WAY;
		break;
	}

	int numberOfPrintedLines = 0;	// a counter to make sure the line draws the entire line from edge to edge
	printf("%c", leftEdgeConnectorType);	// print the left edge connector
	numberOfPrintedLines++;
	for (int i = 1; i < totalWidth; i++)
	{
		numberOfPrintedLines++;
		if (i % (totalWidth / NUMBER_OF_COLUMNS) == 0 && i != totalWidth)	// every time that the the current 
		{
			if (addColumns)
			{
				printf("%c", columnLineConnectorType);
				numberOfPrintedLines++;
			}
			else
			{
				printf("%c", HORIZONTAL_LINE);
				numberOfPrintedLines++;
			}
		}
		else
		{
			printf("%c", HORIZONTAL_LINE);
		}
	}
	while (numberOfPrintedLines < totalWidth)	// print horizontal lines until the number of currently printed lines is one less than the total character spaces available
	{
		printf("%c", HORIZONTAL_LINE);
		numberOfPrintedLines++;
	}
	printf("%c\n", rightEdgeConnectorType);		// cap off the right edge of the line segment with the right edge connector
}

void printInputWindow(void)
{
	/*
	FUNCTION	: printInputWindow
	DESCRIPTION	:
		The function prints a box with an arrow for whenever the user needs to input a value.
	PARAMETERS	:
		VOID
	RETURNS		:
		VOID
	*/
	printLine('m', ' ', false);
	printBorderJustified("--->", false, 0, 0, 'l', false);
	printLine('u', ' ', false);

	printf("\033[2A\r");		// go up 2 lines and to the start of the line
	cursorforward(6);
}

void printLogo(void)
{
	/*
	FUNCTION	: printLogo
	DESCRIPTION	:
		The function prints the 'Deal or no Deal' text in either askii art or plain text based on how large the total game board area is
	PARAMETERS	:
		VOID
	RETURNS		:
		VOID
	*/
	printLine('d', ' ', false);
	if (getTotalWidthOfGameBoard() > 100) // if the total avaiable width is less than 100, than askii art is to big for the board
	{
		printBorderJustified("######                                           #     #           ######                      ", false, 0, 0, 'c', false);
		printBorderJustified("#     # ######   ##   #          ####  #####     ##    #  ####     #     # ######   ##   #     ", false, 0, 0, 'c', false);
		printBorderJustified("#     # #       #  #  #         #    # #    #    # #   # #    #    #     # #       #  #  #     ", false, 0, 0, 'c', false);
		printBorderJustified("#     # #####  #    # #         #    # #    #    #  #  # #    #    #     # #####  #    # #     ", false, 0, 0, 'c', false);
		printBorderJustified("#     # #      ###### #         #    # #####     #   # # #    #    #     # #      ###### #     ", false, 0, 0, 'c', false);
		printBorderJustified("#     # #      #    # #         #    # #   #     #    ## #    #    #     # #      #    # #     ", false, 0, 0, 'c', false);
		printBorderJustified("######  ###### #    # ######     ####  #    #    #     #  ####     ######  ###### #    # ######", false, 0, 0, 'c', false);
	}
	else	// backup header in case the game board is too small
	{
		printBorderJustified("DEAL OR NO DEAL!", false, 0, 0, 'c', false);
	}
	printLine('m', 'd', true);
}

void clearPreviousLine(int repeatNumber)
{
	/*
	FUNCTION	: clearPreviousLine
	DESCRIPTION	:
		The function clears a given number of lines above the currently selected line. 
		Used to prevent redrawing the entire game board and logo every time an input is asked for or is invalid.
	PARAMETERS	:
		int repeatNumber	: the number of lines to clear above the currently selected line
	RETURNS		:
		VOID
	*/
	for (int i = 0; i < repeatNumber; i++)
	{
		// go up one line, to the begining of that line, print a bunch of spaces to clear the line, then back to the begining
		printf("\033[A\r                                                                                                                       \r"); 
	}
}

void printAvailableCashValues(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase)
{
	/*
	FUNCTION	: printAvailableCashValues
	DESCRIPTION	:
		The function prints the values of the cases currently 
	PARAMETERS	:
		double caseValues[]			: case values in order from $0.01 to $1,000,000
		int mixedCaseValueIndex[]	: shuffled list of indexed case values
		bool revealedCases[]		: array of flags informing which cases have already been opened
		int userSelectedCase		: current case the player has in his posession		
	RETURNS		:
		VOID
	*/
	int TotalWidthOfGameBoard = getTotalWidthOfGameBoard();
	printBorderJustified("Cash Values Remaining:", false, 0, 0, 'l', false);
	printf("%c ", VERTICAL_LINE);		// prints opening vertical line
	int displayedCharacterCount = 2;	// starts with 2 characters already displayed
	bool firstPrintedNumber = true;
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (mixedCaseValueIndex[j] == i)	// mixedCaseValueIndex[j] is un-shuffling the mixed case index, and thus displays the case values in order from smallest to largest
			{
				double displayValue = caseValues[mixedCaseValueIndex[j]];
				if (!revealedCases[j] || j == userSelectedCase - 1)		// add to the list if case is unopened or is the selected case of the 
				{
					if (displayValue > 0 && displayValue < 1)
					{
						if (firstPrintedNumber)
						{
							firstPrintedNumber = false;
						}
						else
						{
							printf(",   ");
							displayedCharacterCount += 3;
						}
						printf("$%.2f", displayValue);
						displayedCharacterCount += 5;
					}
					else
					{
						if (firstPrintedNumber)
						{
							firstPrintedNumber = false;
						}
						else
						{
							printf(",   ");
							displayedCharacterCount += 4;
						}
						printf("$%.0f", displayValue);
						displayedCharacterCount += (int)(log10(displayValue) + 1) + 1;
					}
				}
				if (displayedCharacterCount > TotalWidthOfGameBoard - 15) 
				{	// 15 is approximately where putting $1000000 at the end of the list at maximum side postion would keep all numbers within the box.
					printf("%*c", (TotalWidthOfGameBoard - displayedCharacterCount), ' ');	// print spaces until the character limit is reached. where you can print the sides for a new line
					printf("%c\n%c ", VERTICAL_LINE, VERTICAL_LINE);
					displayedCharacterCount = 2;	// reset flags and values for a new line
					firstPrintedNumber = true;
				}
			}
		}
	}
	if (firstPrintedNumber)
	{
		// if there aren't any numbers printed in this line, go back to the begining of the line to overwrite the printed left bar
		printf("\r");	
	}
	else
	{
		printf("%*c", TotalWidthOfGameBoard - displayedCharacterCount, ' ');	// finish off all half complete printed rows by capping the right end of the line
		printf("%c\n", VERTICAL_LINE);
	}
	printLine('m', ' ', false);
}

void printGameBoard(double caseValues[], int mixedCaseValueIndex[], int userSelectedCase, bool revealedCases[], bool showAllCashValues)
{	
	/*
	FUNCTION	: printGameBoard
	DESCRIPTION	:
		The function prints the game board and logo
	PARAMETERS	:
		double caseValues[]			: case values in order from $0.01 to $1,000,000
		int mixedCaseValueIndex[]	: shuffled list of indexed case values
		int userSelectedCase		: current case the player has in his posession
		bool revealedCases[]		: array of flags informing which cases have already been opened
		bool showAllCashValues		: flag to display cash values, ignoring revealedCases[]
	RETURNS		:
		VOID
	*/
	int totalWidth = getTotalWidthOfGameBoard();
	totalWidth -= NUMBER_OF_COLUMNS;
	int columnWidth = totalWidth / NUMBER_OF_COLUMNS;	// the width of each individual column
	const char* numberDisplayString;					// set the display string for the cash values

	system("cls");
	printLogo();

	for (int i = 0; i < 26; i++)
	{		
		if (caseValues[mixedCaseValueIndex[i]] < 1 && caseValues[mixedCaseValueIndex[i]] > 0)
		{
			numberDisplayString = "$%.2f";	// display 2 decimal places only when the value is $0.01
		}
		else
		{
			numberDisplayString = "$%.0f";	// for all values other than $0.01 dont display the decimal places
		}
		printf(CASE_STRING, VERTICAL_LINE, i + 1);
		if ((revealedCases[i] && userSelectedCase != i + 1) || showAllCashValues)	// if the case is open or the flag to show all cases is true
		{
			printf(numberDisplayString, caseValues[mixedCaseValueIndex[i]]);
		}
		else if (userSelectedCase == i + 1)
		{
			printf("X");			// print 'X' to represent the user selected case
		}			
		else 
		{
			printf("-");			// print '-' to represent a closed unselected case
		}

		if ((revealedCases[i] && userSelectedCase != i + 1) || showAllCashValues)
		{
			if (caseValues[mixedCaseValueIndex[i]] < 1)
			{
				printf("%*c", (int)(columnWidth - (int)log10(i + 1) - sizeof(CASE_STRING) - 1), ' ');	// print whitespace needed to get to the next column border, specific to $0.01
			}
			else
			{
				int whitespaceRemaining = columnWidth - (int)log10(i + 1) - ((int)log10(caseValues[mixedCaseValueIndex[i]]) + 1) - (sizeof(CASE_STRING) - 3);	
				printf("%*c", whitespaceRemaining, ' ');	// print whitespace needed to get to the next column border for an open case
			}
		}
		else
		{
			int y = columnWidth - (int)log10(i + 1) - (sizeof(CASE_STRING) - 3);	
			printf("%*c", y, ' ');		// print the whitespace for a closed case
		}

		if ((i + 1) % NUMBER_OF_COLUMNS == 0)
		{
			// if the current column is the end cap of the play area, display the vertical line and advance to a new line
			printf("%c", VERTICAL_LINE);
			newLine();
		}
	}
	int repeat = NUMBER_OF_COLUMNS - (26 % NUMBER_OF_COLUMNS);	// get the number of blank case slots you need to print
	if (repeat != 0)	// print those slots to square off the table
	{
		for (int i = 0; i <= repeat; i++)
		{
			printf("%c", VERTICAL_LINE);
			if (i != repeat)
				printf("%*c", columnWidth, ' ');
		}
		newLine();
	}
	printLine('m', 'u', true);
	if (userSelectedCase != 0)	// display the user's case in a seperate box.
	{
		printBorderJustified("Your Case is: #%.0f", true, (double)userSelectedCase, 0, 'c', false);
		printLine('m', ' ', false);
	}
	if (!showAllCashValues)	// print the avaiable cash values if all of the values are not displayed
	{
		printAvailableCashValues(caseValues, mixedCaseValueIndex, revealedCases, userSelectedCase);
	}
}

int selectCase(double caseValues[], int numberOfSelections, bool revealedCases[], int mixedCaseValueIndex[], int userSelectedCase)
{
	/*
	FUNCTION	: selectCase
	DESCRIPTION	:
		The function gets and returns the user selected case, repeating until the user input is valid.
	PARAMETERS	:
		double caseValues[]			: case values in order from $0.01 to $1,000,000
		int numberOfSelections		: the number of cases to ask for before exiting the loop
		bool revealedCases[]		: array of flags informing which cases have already been opened
		int mixedCaseValueIndex[]	: shuffled list of indexed case values
		int userSelectedCase		: current case the player has in his posession		
	RETURNS		:
		int : only used for selecting an initial case, returns the user's input
	*/
	int caseSelection = 0;
	for (int i = 0; i < numberOfSelections; i++)	// repeat the case selection process for numberOfSelections times
	{
		if (numberOfSelections == 99)	// this is a flag for the initial selection. 
		{
			printBorderJustified("Select your case!", false, 0, 0, 'c', false);
		}
		else if (numberOfSelections - i > 1)	// porperly display either "case" or "cases" based on if there are multiple cases left to open
		{
			printBorderJustified("Select %.0f more cases to open ... ", true, numberOfSelections - i, 0, 'c', false);
		}
		else 
		{
			printBorderJustified("Select %.0f more case to open ... ", true, numberOfSelections - i, 0, 'c', false);
		}
		printInputWindow();
		bool isValidResponse = false;
		while (!isValidResponse)
		{
			caseSelection = getNum();
			if (caseSelection > 0 && caseSelection < 27 && !revealedCases[caseSelection - 1])
			{
				isValidResponse = true;				
			}
			else	// display a message if the user enters an invalid response
			{
				clearPreviousLine(3);
				printBorderJustified("Please enter a VALID AND UNPOENED case number.", false, 0, 0, 'l', false);
				printInputWindow();
			}
		}
		revealedCases[caseSelection - 1] = true;
		if (numberOfSelections == 99)
		{
			break;	// since this is only meant to repeat once, and it is a flag, break out of the loop
		}
		printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, false);
	}
	return caseSelection;	// only used for initial case selection
}

bool yesOrNo(void)
{
	/*
	FUNCTION	: yesOrNo
	DESCRIPTION	:
		The function gets a valid yes or no response from the user, only exiting if a valid response is given.
	PARAMETERS	:
		VOID
	RETURNS		:
		bool	:	returns true or false for yes or no
	*/
	printInputWindow();
	char response[121] = { 0 };
	fgets(response, 121, stdin);
	if (response[0] == 'y' || response[0] == 'Y')	
	{
		return true;	// only check the first letter of the response, and if it begins with 'y' or 'Y', assume the response to the question was yes / true
	}
	else if (response[0] == 'n' || response[0] == 'N')
	{
		return false;	// if the response begins with 'n' or 'N', assume the response was no / false
	}
	else
	{
		// if the response wasn't either 'y' or 'n', display a message to the user and call the yes or no function again
		// this will repeat until the response is valid and returns a true or false
		clearPreviousLine(3);
		printBorderJustified("Please enter a valid response (y/n)", false, 0, 0, 'c', false);
		return yesOrNo();
	}
}

double offerDeal(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase)
{
	/*
	FUNCTION	: offerDeal
	DESCRIPTION	:
		The function gets and returns the user selected case, repeating until the user input is valid.
	PARAMETERS	:
		double caseValues[]					: case values in order from $0.01 to $1,000,000
		int mixedCaseValueIndex[]			: shuffled list of indexed case values
		bool revealedCases[]				: array of flags informing which cases have already been opened
		int userSelectedCase				: current case the player has in his posession
	RETURNS		:
		double	:	returns the deal value, !0 if a deal is accepted and 0 if the deal is not accepted
	*/
	double isDealValue = 0;
	int caseCount = 1;
	double caseSum = caseValues[mixedCaseValueIndex[userSelectedCase - 1]];
	double maxValueLeft = caseValues[mixedCaseValueIndex[userSelectedCase - 1]];

	for (int i = 0; i < 26; i++)
	{
		// iterate over the cases, and calculate the average value of the remaining hidden cases
		if (!revealedCases[i])
		{
			caseCount++;
			caseSum += caseValues[mixedCaseValueIndex[i]];
			if (caseValues[mixedCaseValueIndex[i]] > maxValueLeft)
				maxValueLeft = caseValues[mixedCaseValueIndex[i]]; // used in the offer calculation
		}
	}

	double averageCaseValue = caseSum / (double)caseCount;
	double percent = (rand() % 35) + 75;
	percent = percent / (double)100;
	double simpleCalc = averageCaseValue * percent;
	/* 
	this blog is where I found this formula: http://commcognition.blogspot.ca/2007/06/deal-or-no-deal-bankers-formula.html
	unfortunately due to the nature of the game, there are some edge cases in which this calculation which break the formula
	hence the suggested average * random % between 70 and 115 value is calculated and kept as a back up in case of failure
	*/
	double complexOfferCalculation = 12275.30 + (0.748 * averageCaseValue) - (2714.74 * caseCount) - (0.040 * maxValueLeft) + (0.0000006986 * averageCaseValue * averageCaseValue) + (32.623 * caseCount * caseCount);
	if (complexOfferCalculation < 0 || maxValueLeft - complexOfferCalculation < 0)
	{
		complexOfferCalculation = simpleCalc;
	}
	if (caseCount == 2) // when only 2 cases are left yet to be opened
	{
		printBorderJustified("FINAL OFFER!!!", false, 0, 0, 'c', false);	// this is the last time which you can take a banker offer, thus 'FINAL OFFER'
	}
	printBorderJustified("The banker is offering you...", false, 0, 0, 'c', false);
	printBorderJustified("$%.2F!", true, complexOfferCalculation, 0, 'c', true);
	printBorderJustified("The biggest case still on the board is $%.2F", true, maxValueLeft, 0, 'c', true);
	printBorderJustified("You could loose $%.2F if you take this deal!", true, maxValueLeft - complexOfferCalculation, 0, 'c', true);
	printBorderJustified("Will you accept his offer? (y/n)", false, 0, 0, 'c', false);

	if (yesOrNo())	// if the user accepts the deal, isDealValue becomes a non-zero value
		isDealValue = complexOfferCalculation;	

	return isDealValue;		// if the user has not accepted a deal, this returns isDealValue's default value which is 0
}

int finalCaseSwapSelection(double caseValues[], int mixedCaseValueIndex[], bool revealedCases[], int userSelectedCase)
{
	/*
	FUNCTION	: offerDeal
	DESCRIPTION	:
		The function offers user a swap between their current case and the one other case left closed.
	PARAMETERS	:
		double caseValues[]					: case values in order from $0.01 to $1,000,000
		int mixedCaseValueIndex[]			: shuffled list of indexed case values
		bool revealedCases[]				: array of flags informing which cases have already been opened
		int userSelectedCase				: current case the player has in his posession
	RETURNS		:
		int		:	returns the final case the user has in his posession
	*/

	srand((int)time(NULL));
	for (int i = 0; i < 27; i++) 
	{
		// look through the revealed cases array to find the only case yet to be opened and is also not the user selected case
		if (!revealedCases[i])
		{
			printBorderJustified("There are 2 cases left.", false, 0, 0, 'c', false);
			printBorderJustified("Case #%.0f and Case #%.0f", true, userSelectedCase, i + 1, 'c', false);
			printBorderJustified("Would you like to swap from your case #%.0f to own #%.0f? (y/n)", true, userSelectedCase, i + 1, 'c', false);
			if (yesOrNo())
			{
				// if the user elects to swap cases, change the userSelectedCase value to the other non-opened case
				userSelectedCase = i + 1;				
			}
			break;	// no other items in this array are true
		}
	}
	return userSelectedCase;
}

void playGame(void)
{
	/*
	FUNCTION	: playGame
	DESCRIPTION	:
		The function works the main game logic, defining all default variables and running the case selection.
	PARAMETERS	:
		VOID
	RETURNS		:
		VOID
	*/

	double caseValues[27] = { 0.01, 1, 5, 10, 25, 50, 75, 100, 200, 300, 400, 500, 750, 1000, 5000, 10000, 25000, 50000, 75000, 100000, 200000, 300000, 400000, 500000, 750000, 1000000 };	// the cash values of all of the possible cases in the game
	int casesPerRound[10] = { 6, 5, 4, 3, 2, 1, 1, 1, 1 };	// The number of cases to open per round
	int mixedCaseValueIndex[27] = { };	// an indexing array for linking revealedCases and caseValues
	bool revealedCases[27] = { };		// a boolean array for keeping track of open (true) and closed (false) cases
	double isDealValue = 0;				// the number value of the deal if the player accepts a deal
	int userSelectedCase = 0;			// the case selected and thus posessed by the user
	fill_n(revealedCases, 27, false);	// populate the revealedCases array with all false values
	for (int i = 0; i < 27; i++)
	{
		mixedCaseValueIndex[i] = i;		// populate the mixedCaseValueIndex array with numbers 0 through 25
	}

	srand(time(NULL));
	random_shuffle(&mixedCaseValueIndex[0], &mixedCaseValueIndex[26]);										// shuffle the mixedCaseValueIndex array to keep the caseValue array in order

	printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, true);
	userSelectedCase = selectCase(caseValues, 99, revealedCases, mixedCaseValueIndex, userSelectedCase);	// get the user to select a case
	revealedCases[userSelectedCase - 1] = true;																// set the user's case to be revealed
	printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, false);				

	for (int i = 0; i <= 8; i++)	// iterate over the 8 selection rounds until there are 2 cases left
	{
		selectCase(caseValues, casesPerRound[i], revealedCases, mixedCaseValueIndex, userSelectedCase);
		isDealValue = offerDeal(caseValues, mixedCaseValueIndex, revealedCases, userSelectedCase);
		if (isDealValue != 0)	// isDealValue becomes a non-zero value only if a deal is accepted
		{
			break;	// in this case exit the case selection loop because a deal has been completed
		}
		printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, false);
	}

	if (isDealValue != 0)	
	{	// since a deal has been reached,
		system("cls");
		printLine('d', ' ', false);
		printBorderJustified("Congratulations!", false, 0, 0, 'c', false);
		printBorderJustified("You Won $%.2f!!!", true, isDealValue, 0, 'c', true);
		printBorderJustified("Would you like to open the remaining cases? (y/n)", false, 0, 0, 'c', false);
		if (yesOrNo())	// ask the user if they would like to have the contents of every case revealed to them
		{	
			double finalCaseValue = caseValues[mixedCaseValueIndex[userSelectedCase - 1]];	// get the value of the case the user had selected at the start
			printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, true);
			printBorderJustified("You took a deal for $%.2f and your case had $%.2f in it.", true, isDealValue, finalCaseValue, 'c', true);
			if (isDealValue > finalCaseValue)	//check the deal value compated to the in-case value
			{	// if the deal was better than the case, praise the player
				printBorderJustified("You made an excelent deal!", false, 0, 0, 'c', false);
				printBorderJustified("You won $%.2f more from getting out at the right time.", true, isDealValue - finalCaseValue, 0, 'c', true);
				printBorderJustified("Good Job!", false, 0, 0, 'c', false);
			}
			else
			{	// if the deal was worse than the case, let the user know that the deal they made was poor and they had more money in their case
				printBorderJustified("You made a poor deal!", false, 0, 0, 'c', false);
				printBorderJustified("You lost $%.2f more from getting out at the wrong time.", true, finalCaseValue - isDealValue, 0, 'c', true);
				printBorderJustified("Better luck next time!", false, 0, 0, 'c', false);
			}
		}
	}
	else
	{		
		// since no deal was reached, offer the final case swap to the player
		userSelectedCase = finalCaseSwapSelection(caseValues, mixedCaseValueIndex, revealedCases, userSelectedCase);
		double finalCaseValue = caseValues[mixedCaseValueIndex[userSelectedCase - 1]]; // get the final selected case
		printGameBoard(caseValues, mixedCaseValueIndex, userSelectedCase, revealedCases, true);
		printBorderJustified("You chose case #%.0f", true, userSelectedCase, 0, 'c', false);
		printBorderJustified("Your case contains...", false, 0, 0, 'c', false);
		printBorderJustified("$%.2f!!!", true, finalCaseValue, 0, 'c', true);
		if (finalCaseValue >= 10000)	// if the player's case is above $10,000 praise the player, otherwise offer condolences 
		{
			printBorderJustified("Congratulations, you struck a good deal!", false, 0, 0, 'c', false);
		}
		else
		{
			printBorderJustified("Too Bad! Try to make a better deal next time!", false, 0, 0, 'c', false);
		}
		printLine('m', ' ', false);
	}
}

int main(void)
{
	/*
	FUNCTION	: main
	DESCRIPTION	:
		The function works the main game loop, calling the game loop.
		This also asks the user if they would like to play again upon game completion.
	PARAMETERS	:
		VOID
	RETURNS		:
		int		: exit code --> 0
	*/
	bool continuePlaying = true;	// when true keep running through the loop, playing the game
	while (continuePlaying)
	{
		playGame();	// start the game
		printBorderJustified("Would you like to play again? (y/n)", false, 0, 0, 'c', false);
		continuePlaying = yesOrNo();	// if the user elects to say no to the offer to play again, exit the game loop and thereby the program.
		system("cls");
	}		
	return 0;
}