/*
* FILE : cA2.cpp
* PROJECT : PROG1347 - Assignment #2
* PROGRAMMER : Alex Kozak
* FIRST VERSION : 02-21-2018
* DESCRIPTION :
* The functions in this file are used to create a limited base-exponent calculator
*/



#include "stdafx.h"
#include <math.h>
#include <windows.h>   // WinApi header



#define TOP_LEFT_CORNER 214			// ╓
#define BOTTOM_LEFT_CORNER 211		// ╙  
#define TOP_RIGHT_CORNER 183		// ╖
#define BOTTOM_RIGHT_CORNER 189		// ╜
#define HORIZONTAL_LINE 196			// ─
#define VERTICAL_LINE 186			// ║
#define VERTICAL_RIGHT 199			// ╟
#define VERTICAL_LEFT 182			// ╢



enum errorCode{noError, invalidBase, invalidExponent};	// enum containing the possible errors 



#pragma warning(disable: 4996)



// prototype
errorCode printCurrentErrors(errorCode, int, int, int, int);
COORD GetConsoleCursorPosition(void);
void printInternal(const char*, bool, double, double, int, int);
void printBorderJustified(const char*, bool, double, double, char);
void printLine(char);
void printInputWindow(void);
void printCurrentBaseExponent(int, int);
void gotoxy(int, int, bool);
void clearScreen(void);
int getNum(void);
bool checkRange(int, int, int);
int getColumns(void);
int getRows(void);
int calculatePower(int, int);



errorCode printCurrentErrors(errorCode currErrorCode, int minBase, int maxBase, int maxExponent, int minExponent)
{
	/*
	FUNCTION	: printCurrentErrors
	DESCRIPTION	:
	Receives the current error code, displays the appropriate error message to the user, and clears the error code
	PARAMETERS	:
	int currErrorCode		: the current
	int minBase				: the minimum valid base
	int maxBase				: the maximum valid base
	int maxExponent			: the minimum valid exponent
	int minExponent			: the maximum valid exponent
	RETURNS		:
	int	0	: Returns a new currErrorValue, which says that there is no longer an error, as it has been displayed to the user
	*/
	switch (currErrorCode)
	{
	case noError:	// 
	{
		break;
	}
	case invalidBase:	// this is when the base entered was outside of the range between minBase and maxBase
	{
		printLine('m');
		printBorderJustified("Your base was outside the range", false, 0, 0, 'c');
		printBorderJustified("of acceptable values (%.0f to %.0f)", true, minBase, maxBase, 'c');
		break;
	}
	case invalidExponent: // this is when the base entered was outside of the range between minExponent and maxExponent
	{
		printLine('m');
		printBorderJustified("Your exponent was outside the range of", false, 0, 0, 'c');
		printBorderJustified("of acceptable values (%.0f to %.0f)", true, minExponent, maxExponent, 'c');
		break;
	}
	}
	return noError;	// set the current error back to 0, which is no error
}



COORD GetConsoleCursorPosition(void)
{
	/*
	FUNCTION	: GetCurrentCursorPosition
	DESCRIPTION	:
		return a coordinate which is the current location of the console cursor. Used for relative cursor location
	PARAMETERS	:
		VOID
	RETURNS		:
		COORD currentCursorPosition	: the x/y position of the cursor in the currnt console
	*/
	COORD currentCursorPosition = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// if the program is unsuccessful in getting the screen buffer, it returns the default value of {0, 0}
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))	
	{
		// if the program IS successful, return the current value of the cursor
		currentCursorPosition = csbi.dwCursorPosition;
	}
	return currentCursorPosition;
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
		if (formatNum2 == 0) // formatNum2 is only 0 if there is no content to display for the second value.
		{
			printf(input, formatNum);
		}
		else
		{
			printf(input, formatNum, formatNum2);
		}
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



void printBorderJustified(const char* input, bool containsNumber, double formatNum, double formatNum2, char justification)
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
	int totalWidth = getColumns();

	for (inputSize = 0; input[inputSize] != '\0'; ++inputSize);	// iterate over the input char*, set InputSize to the character count

	if (containsNumber)
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



void printLine(char sides)
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
	int totalWidth = getColumns();
	// setting the default line to be ╟───╢ while declaring the left edge, the centers, and the right edge connector type
	int leftEdgeConnectorType = VERTICAL_RIGHT;
	int rightEdgeConnectorType = VERTICAL_LEFT;
	int columnLineConnectorType = HORIZONTAL_LINE;

	switch (sides)
	{
	case 'd': // corners facing 'd'own. ╓ ╖
		leftEdgeConnectorType = TOP_LEFT_CORNER;
		rightEdgeConnectorType = TOP_RIGHT_CORNER;
		break;
	case 'u': // corners facing 'u'p. ╙ ╜
		leftEdgeConnectorType = BOTTOM_LEFT_CORNER;
		rightEdgeConnectorType = BOTTOM_RIGHT_CORNER;
		break;
	case 'm': // 'm'iddle of a line sides ╟ ╢
		leftEdgeConnectorType = VERTICAL_RIGHT;
		rightEdgeConnectorType = VERTICAL_LEFT;
		break;
	}

	int numberOfPrintedLines = 0;	// a counter to make sure the line draws the entire line from edge to edge
	printf("%c", leftEdgeConnectorType);	// print the left edge connector
	numberOfPrintedLines++;
	for (int i = 1; i < totalWidth; i++)
	{
		printf("%c", HORIZONTAL_LINE);
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
	printLine('m');
	printBorderJustified("--->", false, 0, 0, 'l');
	printLine('u');
	gotoxy(6, -2, true); // goes back to the end of the arrow for where the user can then input their response
}



void printCurrentBaseExponent(int base, int exponent)
{
	/*
	FUNCTION	: printCurrentBaseExponent
	DESCRIPTION	:
		Prints the box displaying the current base/exponent pair as well as the calculated result.
	PARAMETERS	:
		int base		: the current base which will be displayed and calculated with
		int exponent	: the current exponent which will be displayed and calculated with
	RETURNS		:
		VOID
	*/
	printLine('d');
	printBorderJustified("%.0f ^ %.0f", true, base, exponent, 'c');
	printBorderJustified("= %.0f", true, calculatePower(base, exponent), 0, 'c');
	printLine('m');
}



void gotoxy(int x, int y, bool isRelative)
{
	/*
	FUNCTION	: gotoxy
	DESCRIPTION	:
		Changes the cursor position to either the absolute or relative position of the given x, y coordinates
	PARAMETERS	:
		int x			: the position horizontally in spaces from the left edge of the console
		int y			: the position vertically in spaces from the top edge of the console
		bool isRelative	: flag to set the position relative to the current position or as an absolute position
	RETURNS		:
		VOID
	*/
	COORD currPosition = { 0, 0 };
	if (isRelative)	// if a relative position is requested
	{	
		currPosition = GetConsoleCursorPosition();	// get the current position of the cursor
		currPosition.X += x;	// add the given x to the current x co-ordinate
		currPosition.Y += y;	// add the given y to the current y co-ordinate
	}
	else
	{
		currPosition = { (SHORT)x, (SHORT)y };	// set the current location coordinate to the absolute x/y given
	}
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, currPosition);		// either route, the location is set to the currPosition COORD
}



void clearScreen(void)
{
	/*
	FUNCTION	: clearScreen
	DESCRIPTION	:
		prints spaces in all visible places on the screen, before returning the cursor to the top left corner, where the program can then reprint from
	PARAMETERS	:
		VOID
	RETURNS		:
		VOID
	*/
	gotoxy(0, 0, false);	// go to the top left corner so all printed characters are encapsulated by the clear
	for (int i = 0; i < getRows(); i++)	// repeat for the number of horizontal rows that the console contains
	{
		// print the same number of spaces as columns in the console, essentially replacing all visible characters with a blank character.
		printf("%*c \n", getColumns(), ' ');	
	}
	gotoxy(0, 0, false);	// go back to the top to maintain consistency in printed location
}



bool checkRange(int value, int maxValue, int minValue)
{
	/*
	FUNCTION	: checkRange
	DESCRIPTION	:
	takes the max and min values for a given range and return a boolean declaring wether the value is within the given range
	PARAMETERS	:
	int value		: the given value to check if it falls within the range
	int maxValue	: the maximum value that the given value can be while reamining in the range
	int minValue	: the minimum value that the given value can be while reamining in the range
	RETURNS		:
	bool	: returns true if the input value is between the max and min values, and false otherwise.
	*/
	return (minValue <= value && value <= maxValue);
}



int getNum(void)
{
	/*
	FUNCTION	: getNum
	DESCRIPTION	:
		getNum waits for the user to enter a string of characters and checks to see if the user had entered a number, and returns that number
	PARAMETERS	:
		VOID
	RETURNS		:
		if the user input a number, the function then returns said number. if invalid, the function returns '-1'
	*/

	/* the array is 121 bytes in size; we'll see in a later lecture how we can improve this code */
	char record[121] = { 0 }; // record stores the string
	int number = 0;
	/* NOTE to student: indent and brace this function consistent with your others
	   use fgets() to get a string from the keyboard */
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



int getColumns(void)
{
	/*
	FUNCTION	: getColumns
	DESCRIPTION	:
		Get the current number of columns displayed in the console
	PARAMETERS	:
		VOID
	RETURNS		:
		int columns		: the number of displayed columns in the console
	*/
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns = 0;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// get the difference in the console left and right coordinates, and remove one as a buffer to prevent wrapping
	columns = csbi.srWindow.Right - csbi.srWindow.Left - 1;	
	return columns;
}



int getRows(void)
{
	/*
	FUNCTION	: getRows
	DESCRIPTION	:
		Get the current number of rows displayed in the console
	PARAMETERS	:
		VOID
	RETURNS		:
		int rows		: the number of displayed rows in the console
	*/
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int rows = 0;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	// get the difference in the console bottom and top coordinates, and remove one as a buffer to prevent wrapping
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top - 1;
	return rows;
}



int calculatePower(int base, int exponent)
{
	/*
	FUNCTION	: calculatePower
	DESCRIPTION	:
		Takes in a intereger base and exponent, and multiplies the base by itself the exponent number of times
	PARAMETERS	:
		int base		: the base of the given number going to be raised to a given exponent
		int exponent	: the number that the base is raised to before being returned
	RETURNS		:
		int finalValue	: the result of raising the base to the exponent
	*/
	int finalValue = base;	// declare a second variable which will be multiplied by the base to retain the value of the base
	for (int i = 1; i < exponent; i++)
	{
		finalValue *= base;	// for each value above 1, the final value is multiplied by the base, simputaing a power
	}
	return finalValue;
}



int main(void)
{
	errorCode currErrorCode = noError;	// errorCode enum value of noError
	bool continueAsking = true;	
	// default base/exponent pair
	int base = 1;
	int exponent = 1;	
	// range of valid values for the base
	int minBase = 1;
	int maxBase = 25;
	// range of valid values for the exponent
	int minExponent = 1;
	int maxExponent = 5;
	
	while (continueAsking)
	{
		// print the base/exponent/output as well as the menu for the user to change their desired value
		printCurrentBaseExponent(base, exponent);
		printBorderJustified("Power Menu:", false, 0, 0, 'l');
		printBorderJustified("1. Change Base", false, 0, 0, 'c');
		printBorderJustified("2. Change Exponent", false, 0, 0, 'c');
		printBorderJustified("3. Exit Program", false, 0, 0, 'c');
		printBorderJustified("Option?", false, 0, 0, 'l');
		currErrorCode = printCurrentErrors(currErrorCode, minBase, maxBase, maxExponent, minExponent);
		
		printInputWindow();
		int userChosenPath = getNum();
		clearScreen();

		switch (userChosenPath)
		{
		case 1:	// the user has selected to change the base
		{
			// prompt the user to enter a new base, while telling them what the current base is
			printCurrentBaseExponent(base, exponent);
			printBorderJustified("Please enter a new base.", false, 0, 0, 'c');
			printBorderJustified("The current base is %.0f", true, base, 0, 'c');
			printInputWindow();
			int possibleBase = getNum();

			if (checkRange(possibleBase, maxBase, minBase))
			{
				base = possibleBase;	// if the base is within the valid range, change it to the new value
			}
			else
			{
				currErrorCode = invalidBase;		// otherwise leave the base as the same number and set the error code to invalidBase
			}
			break;
		}
		case 2:	// the user has selected to change the exponent
		{
			// prompt the user to enter a new exponent, while telling them what the current exponent is
			printCurrentBaseExponent(base, exponent);
			printBorderJustified("Please enter a new exponent.", false, 0, 0, 'c');
			printBorderJustified("The current exponent is %.0f", true, exponent, 0, 'c');
			printInputWindow();
			int possibleExponent = getNum();

			if (checkRange(possibleExponent, maxExponent, minExponent))
			{
				exponent = possibleExponent;	// if the exponent is within the valid range, change it to the new value
			}
			else
			{
				currErrorCode = invalidExponent;	// otherwise leave the base as the same number and set the error code to invalidExponent
			}
			break;
		}
		case 3:
		{
			continueAsking = false;	// change the continuation flag to off and thus exit the loop.
			break;
		}
		}
		clearScreen();
	}
    return 0;
}