#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>

double decimalToTime(double dec)
{
	return (int)dec + ((dec - (int)dec) * 100 / 60);
}

int main()
{
	char buffer[80];
	char cityName[60];
	char layoverTime[10];
	char flightTime[10];

	int flightIndex = 0;
	int cityIndex = 0;
	int layoverIndex = 0;
	int itemIndex = 0;
	int len = 0;
	bool exitProg = false;

	while (!exitProg)
	{
		flightIndex = 0;
		cityIndex = 0;
		layoverIndex = 0;
		itemIndex = 0;
		itemIndex = 0;
		printf("Please enter the flight information data: ");
		fgets(buffer, 80, stdin);
		len = strlen(buffer);

		if (buffer[0] != '.' || buffer[1] != '\n')
		{
			for (int i = 0; i < len; i++)
			{
				if (buffer[i] == ',')
				{
					itemIndex++;
					i++;
				}
				switch (itemIndex)
				{
				case 0:
				{
					cityName[cityIndex] = buffer[i];
					cityIndex++;
					break;
				}
				case 1:
				{
					flightTime[flightIndex] = buffer[i];
					flightIndex++;
					break;
				}
				case 2:
				{
					layoverTime[layoverIndex] = buffer[i];
					layoverIndex++;
					break;
				}
				}
			}
			if (itemIndex == 2)
			{
				cityName[cityIndex] = '\0';
				flightTime[flightIndex] = '\0';
				layoverTime[--layoverIndex] = '\0';
				if ((atoi(flightTime) != 0 || flightTime[0] == '0') && (atoi(layoverTime) != 0 || layoverTime[0] == '0'))
				{
					double ft = atof(flightTime);
					double lt = atof(layoverTime);
					printf("%s has a flight time of %0.2f hours and a layover time of %0.2f hours\n", cityName, decimalToTime(ft), decimalToTime(lt));
				}				
				else
				{
					printf("The value entered for flight or layover time is not recognized as a valid number\n");
				}
			}
			else
			{
				printf("There was only %d value%c given. The program expects 3 values\n", ++itemIndex, itemIndex == 0 ? ' ' : 's');
			}
		}
		else
		{
			exitProg = true;
		}
	}
	return 1;
}