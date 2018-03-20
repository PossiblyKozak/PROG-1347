#pragma once



double* getTorontoFlights(int destination)
{
	static double flights[7][25] = { 
		//To Toronto
		{},
		//To Atlanta
		{ 0625.0845, 0910.1400, 1230.1645, 1610.2220, 2000.2215 },
		//To Autsin
		{},
		//To Santa Fe
		{},
		//To Denver
		{ 0730.1105, 1500.2100 },
		//To Chicago
		{ 0640.0800, 0740.0915, 0840.1015, 0940.1115, 1040.1215, 1140.1315, 1240.1415, 1340.1515, 1440.1615, 1530.1715 , 1630.1815 , 1730.1915 , 1830.2015 , 1930.2115 , 2100.2230, 2200.2315 },
		//To Buffalo
		{}
	};
	return flights[destination];
}