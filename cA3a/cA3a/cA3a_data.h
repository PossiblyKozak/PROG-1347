#pragma once



int* getFlights(int startingLocation, int destination)
{
	static int flights[7][7][35] = {
		{	// from Toronto
			//To Toronto
			{ 0 },
			//To Atlanta
			{ 5, 625, 220, 910, 450, 1230, 415, 1610, 610, 2000, 215 },
			//To Austin
			{ 0 },
			//To Santa Fe
			{ 0 },
			//To Denver
			{ 2, 730, 335, 1500, 600 },
			//To Chicago
			{ 16, 640, 120, 740, 135, 840, 135, 940, 135, 1040, 135, 1140, 135, 1240, 135, 1340, 135, 1440, 135, 1530, 145, 1630, 145, 1730, 145, 1830, 145, 1930, 145, 2100, 130, 2200, 115},
			//To Buffalo
			{ 0 }
		},
		{	// from Atlanta
			//To Toronto
			{ 5, 710, 210, 1030, 410, 1500, 350, 1710, 610, 2100, 220},
				//To Atlanta
			{ 0 },
				//To Austin
			{ 3, 900, 210, 1530, 250, 2000, 230 },
				//To Santa Fe
			{ 0 },
				//To Denver
			{ 3, 600, 300, 1320, 500, 1710, 250 },
				//To Chicago
			{ 15, 650, 210, 750, 300, 850, 300, 950, 300, 1050, 300, 1150, 300, 1250, 300, 1350, 300, 1450, 300, 1550, 230, 1650, 230, 1750, 230, 1850, 230, 1950, 230, 2030, 210},
				//To Buffalo
			{ 0 }
		},
		{	// from Austin
			//To Toronto
			{ 0 },
			//To Atlanta
			{ 3, 910, 220, 1500, 220, 2130, 230 },
			//To Austin
			{ 0 },
			//To Santa Fe
			{ 1, 1700, 55 },
			//To Denver
			{ 2, 1030, 220, 1820, 220 },
			//To Chicago
			{ 0 },
			//To Buffalo
			{ 0 }
		},
		{	// from Santa Fe
			//To Toronto
			{ 0 },
			//To Atlanta
			{ 0 },
			//To Austin
			{ 1, 1500, 45 },
			//To Santa Fe
			{ 0 },
			//To Denver
			{ 0 },
			//To Chicago
			{ 0 },
			//To Buffalo
			{ 0 }
		}, 
		{	// from Denver
			//To Toronto
			{ 3, 630, 410, 1030, 520, 1400, 500 },
			//To Atlanta
			{ 3, 600, 310, 1300, 320, 1500, 350 },
			//To Austin
			{ 2, 1200, 200, 1500, 220 },
			//To Santa Fe
			{ 0 },
			//To Denver
			{ 0 },
			//To Chicago
			{ 7, 700, 220, 800, 250, 1000, 250, 1200, 250, 1400, 250, 1600, 250, 1830, 240 },
			//To Buffalo
			{ 0 }
		},
		{	// from Chicago
			//To Toronto
			{ 12, 740, 110, 910, 230, 1010, 230, 1110, 230, 1210, 230, 1310, 230, 1410, 230, 1510, 230, 1610, 230, 1710, 230, 1910, 200, 2110, 210},
			//To Atlanta
			{ 15, 650, 210, 800, 240, 900, 240, 1000, 240, 1100, 240, 1200, 240, 1300, 240, 1400, 240, 1500, 240, 1600, 240, 1700, 240, 1800, 240, 1900, 240, 2000, 240, 2150, 300},
			//To Austin
			{ 0 },
			//To Santa Fe
			{ 0 },
			//To Denver
			{ 6, 900, 210, 1130, 220, 1330, 220, 1530, 220, 1730, 220, 2100, 250},
			//To Chicago
			{ 0 },
			//To Buffalo
			{ 4, 1100, 200, 1310, 150, 1500, 230, 1800, 210 }
		},
		{	// from Buffalo
			//To Toronto
			{ 0 },
			//To Atlanta
			{ 0 },
			//To Austin
			{ 0 },
			//To Santa Fe
			{ 0 },
			//To Denver
			{ 4, 940, 140, 1110, 150, 1740, 240, 2010, 220},
			//To Chicago
			{ 0 },
			//To Buffalo
			{ 0 },
		}

	};
	return flights[startingLocation][destination];
}