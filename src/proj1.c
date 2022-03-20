/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt

*--------------------------------------------------------------------*
*                       Airport Manager                              *
*                                                                    *
* 1st IAED Project - 21/22                                           *
* Computer Science and Engineering (Alameda)                         *
* Instituto Superior Técnico                                         *
*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "constants.h"
#include "prototypes.h"

/* Global variables */
int totalAirports = 0; 					/* tracks the total amount of airports added by the user */
int totalFlights = 0; 					/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 		/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 		/* stores all of the current flights */

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	char c[MAX_COMMAND_LENGTH];
	int run = 1;

	while (run) {
		scanf("%s", c);
		switch(c[0]) {
			case 'q':
				/* 'q' command stops the program */
				run = 0;
				break;
			case 'a':
				AddAirport();
				break;
			case 'l':
				ListAirports();
				break;
			case 'v':
				AddFlight();
				break;
			case 'p':
				FlightDeparturesInAirport();
				break;
			case 'c':
				FlightArrivalsInAirport();
				break;
			case 't':
				AdvanceSystemDate();
				break;
		}
	}
	return 0;
}

/**
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport() {
	char id[ID_LENGTH];
	int i;

	if (totalAirports > MAX_AIRPORTS) {
		printf(AIRPORT_ERR_TOO_MANY);
		return;
	}

	scanf("%s", id);
	for (i = 0; i < ID_LENGTH - 1; i++)
		if (id[i] < 'A' || id[i] > 'Z') {
			printf(AIRPORT_ERR_INVALID);
			return;
		}
	for (i = 0; i < totalAirports; i++)
	 	if (!strcmp(allAirports[i].id, id)) {
	 		printf(AIRPORT_ERR_DUPLICATE);
	 		return;
	  	}

	strcpy(allAirports[totalAirports].id, id);
	scanf("%s", allAirports[totalAirports].country);
	getchar(); /* removes the trailing white space before the city name */
	scanf("%[^\n]", allAirports[totalAirports].city);

	printf("airport %s\n", allAirports[totalAirports].id);

	totalAirports++;
}

/**
 * Handles the 'l' command.
 * If no arguments are provided it will print all of the airports in
 * alphabetical order.
 * Otherwise, the function prints the airports with the specified IDs.
 */
void ListAirports() {
	/**/
}

/**
 * Handles the 'v' command.
 * If no arguments are provided it will print all of the flights in alphabetical
 * order.
 * Otherwise, the function adds a new flight to the system with the specified
 * flight code, airport of departure id, airport of arrival id, date of
 * departure, time of departure, duration of flight and capacity of the flight.
 */
void AddFlight() {
	/**/
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID.
 */
void FlightDeparturesInAirport() {
	/**/
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID.
 */
void FlightArrivalsInAirport() {
	/**/
}

/**
 * Handles the 't' command.
 * Forwards the date of the system.
 */
void AdvanceSystemDate() {
	/**/
}
