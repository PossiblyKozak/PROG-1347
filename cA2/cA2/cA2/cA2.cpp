// cA2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>   // WinApi header
#include <conio.h>

#define TOP_LEFT_CORNER 214			// ╓
#define BOTTOM_LEFT_CORNER 211		// ╙  
#define TOP_RIGHT_CORNER 183		// ╖
#define BOTTOM_RIGHT_CORNER 189		// ╜
#define HORIZONTAL_LINE 196			// ─
#define VERTICAL_LINE 186			// ║
#define VERTICAL_RIGHT 199			// ╟
#define VERTICAL_LEFT 182			// ╢

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define cursorforward(x) printf("\033[%dC", (x))		// Moves the cursor x characters forward
#define VIEW_WIDTH 100									// the width of the view box

#pragma warning(disable: 4996)

// prototype
COORD GetConsoleCursorPosition(HANDLE);
void printInternal(const char*, bool, double, double, int, int);
void printBorderJustified(const char*, bool, long double, long double, char);
void printLine(char);
void printInputWindow(void);
void printCurrentBaseExponent(int, int);
void gotoxy(int, int, bool);
void clearScreen(void);
int getNum(void);
int checkRange(int, int, int);
int getColumns(void);
int getRows(void);
int calculatePower(int, int);

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	COORD currentCursorPosition = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO cbsi;

	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		currentCursorPosition = cbsi.dwCursorPosition;
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

void printBorderJustified(const char* input, bool containsNumber, long double formatNum, long double formatNum2, char justification)
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
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int inputSize;						// The total number of characters in the input string
	int halfWidth;						// used for getting half of the required whitespace. For centering.
	double halfInputSize;				// used for getting half of the side of the input. For centering.
	int totalWidth = getColumns();

	for (inputSize = 0; input[inputSize] != '\0'; ++inputSize);	// iterate over the input char*, set inputsize to the character count

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
	gotoxy(6, -2, true);
}

void printCurrentBaseExponent(int base, int exponent)
{
	printLine('d');
	printBorderJustified("%.0f ^ %.0f", true, base, exponent, 'c');
	printBorderJustified("= %.0f", true, calculatePower(base, exponent), 0, 'c');
	printLine('m');
}

void gotoxy(int x, int y, bool isRelative)
{
	COORD currPosition;
	if (isRelative)
	{
		currPosition = GetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE));
		currPosition.X += x;
		currPosition.Y += y;
	}
	else
	{
		currPosition = { (SHORT)x, (SHORT)y };
	}
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, currPosition);
}

void clearScreen(void)
{
	gotoxy(0, 0, false);
	for (int i = 0; i < getRows(); i++)
	{
		printf("%*c \n", getColumns(), ' ');
	}
	gotoxy(0, 0, false);
}

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

int checkRange(int value, int maxValue, int minValue)
{
	return (minValue <= value && value <= maxValue);
}

int getColumns(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left;
	return columns - 1;
}

int getRows(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top;
	return rows - 1;
}

int calculatePower(int base, int exponent)
{
	int finalValue = base;
	for (int i = 1; i < exponent; i++)
	{
		finalValue = finalValue * base;
	}
	return finalValue;
}

int main(void)
{
	bool continueAsking = true;	
	int base = 1;
	int minBase = 1;
	int maxBase = 25;

	int exponent = 1;	
	int minExponent = 1;
	int maxExponent = 5;

	int currError = 0;	// 0 = no error, 1 = error in the base, 2 == error in the exponent
	while (continueAsking)
	{
		clearScreen();		
		printCurrentBaseExponent(base, exponent);
		printBorderJustified("Power Menu:", false, 0, 0, 'l');
		printBorderJustified("1. Change Base", false, 0, 0, 'c');
		printBorderJustified("2. Change Exponent", false, 0, 0, 'c');
		printBorderJustified("3. Exit Program", false, 0, 0, 'c');
		printBorderJustified("Option?", false, 0, 0, 'l');
		if (currError != 0)	
		{
			printLine('m');
		}
		switch (currError)
		{			
		case 1:
		{
			printBorderJustified("Your base was outside the range", false, 0, 0, 'c');
			printBorderJustified("of acceptable values (%.0f to %.0f)", true, minBase, maxBase, 'c');
			currError = 0;
			break;
		}
		case 2:
		{
			printBorderJustified("Your exponent was outside the range of", false, 0, 0, 'c');
			printBorderJustified("of acceptable values (%.0f to %.0f)", true, minExponent, maxExponent, 'c');
			currError = 0;
			break;
		}
		}		
		printInputWindow();
		int userChosenPath = getNum();
		clearScreen();

		switch (userChosenPath)
		{
		case 1:
		{
			printCurrentBaseExponent(base, exponent);
			printBorderJustified("Please enter a new base.", false, 0, 0, 'c');
			printBorderJustified("The current base is %.0f", true, base, 0, 'c');
			printInputWindow();
			int possibleBase = getNum();
			if (checkRange(possibleBase, maxBase, minBase) == 1)
				base = possibleBase;
			else
				currError = 1;
			break;
		}
		case 2:
		{
			printCurrentBaseExponent(base, exponent);
			printBorderJustified("Please enter a new exponent.", false, 0, 0, 'c');
			printBorderJustified("The current exponent is %.0f", true, base, 0, 'c');
			printInputWindow();
			int possibleExponent = getNum();
			if (checkRange(possibleExponent, maxExponent, minExponent) == 1)
				exponent = possibleExponent;
			else
				currError = 2;
			break;
		}
		case 3:
		{
			continueAsking = false;
		}
		}
	}
    return 0;
}

