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
int totalAirports; 							/* tracks the total amount of airports added by the user */
int totalFlights; 							/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 			/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 			/* stores all of the current flights */
char sortedIDs[MAX_AIRPORTS][ID_LENGTH]; 	/* stores all of the IDs sorted by alphabetical order */

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	char c[MAX_COMMAND_LENGTH], lastchar;
	int run = 1;

	while (run) {
		lastchar = GetOneArgument(c, 0);
		switch(c[0]) {
			case 'q':
				/* 'q' command stops the program */
				run = 0;
				break;
			case 'a':
				AddAirport();
				break;
			case 'l':
				ListAirports(lastchar);
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
 *
 */
char GetOneArgument(char *argument, const int mode) {
	int c, i = 0;

	while ((!mode && (c = getchar()) != ' ' && c != '\t' && c != '\n')
			|| (mode && (c = getchar()) != '\n')) {
		argument[i++] = c;
	}
	argument[i] = '\0';

	return c; /* returns the last character read from stdin */
}

/**
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport() {
	char id[ID_LENGTH], country[MAX_COUNTRY_LENGTH], city[MAX_CITY_LENGTH];

	GetOneArgument(id, 0);
	GetOneArgument(country, 0);
	GetOneArgument(city, 1);

	if (CheckAddAirportErrors(id))
		return;

	/* actually add the aiport to the system */
	strcpy(allAirports[totalAirports].id, id);
	strcpy(allAirports[totalAirports].country, country);
	strcpy(allAirports[totalAirports].city, city);
	allAirports[totalAirports].num_flight_departures = 0;

	AddSortedAirportID(id);

	printf("airport %s\n", allAirports[totalAirports].id);

	totalAirports++;
}

/**
 *
 */
void AddSortedAirportID(char *id) {
	int first, middle, last, index;
	int i;

	if (!totalAirports)
		index = 0;
	else {
		first = 0;
		last = totalAirports - 1;
		while (first != last && last - first != 1) {
			middle = (first + last) / 2;
			if (strcmp(sortedIDs[middle], id) < 0)
				first = middle + 1;
			else
				last = middle - 1;
		}

		index = first;
		if (first != last || strcmp(sortedIDs[middle], id) < 0)
			index += 1;
	}

	for (i = totalAirports; i > index; i--)
		strcpy(sortedIDs[i - 1], sortedIDs[i]);
	strcpy(sortedIDs[index], id);
}

/**
 *
 */
int GetAirportFromID(char *id) {
	int i;

	for (i = 0; i < totalAirports; i++)
		if (!strcmp(allAirports[i].id, id))
			return i;

	return -1;
}

/**
 *
 */
int CheckAddAirportErrors(char *id) {
	int i;

	if (totalAirports >= MAX_AIRPORTS) {
		printf(AIRPORT_ERR_TOO_MANY);
		return 1;
	}
	for (i = 0; i < ID_LENGTH - 1; i++)
		if (id[i] < 'A' || id[i] > 'Z') {
			printf(AIRPORT_ERR_INVALID);
			return 1;
		}
	if (GetAirportFromID(id) != -1)  {
		printf(AIRPORT_ERR_DUPLICATE);
	 	return 1;
	}

	return 0;
}

/**
 * Handles the 'l' command.
 * If no arguments are provided it will print all of the airports in
 * alphabetical order.
 * Otherwise, the function prints the airports with the specified IDs.
 */
void ListAirports(char lastchar) {
	char id[ID_LENGTH];
	int i;

	if (lastchar == '\n') {
		ListAllAirports();
		return;
	}

	while (GetOneArgument(id, 0) != '\n') {
		if (CheckAirportValidity(id))
			return;

		i = GetAirportFromID(id);

		printf("%s %s %s %d\n", id, allAirports[i].city, allAirports[i].country,
		 					allAirports[i].num_flight_departures);
	}
}

/**
 *
 */
void ListAllAirports() {
	int i, j;

	for (i = 0; i < totalAirports; i++) {
		j = GetAirportFromID(sortedIDs[i]);
		printf("%s %s %s %d\n", sortedIDs[i], allAirports[j].city,
		 					allAirports[j].country,
		 					allAirports[j].num_flight_departures);
	}
}

/**
 *
 */
int CheckAirportValidity(char *id) {
	if (GetAirportFromID(id) != -1) {
		printf(AIRPORT_ERR_NO_ID);
		return 0;
	}

	return 1;
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
