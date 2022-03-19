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


/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	int c, run = 1;

	while (run) {
		c = getchar();
		switch(c) {
			case 'q':
				/* stops the program */
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
	/**/
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
