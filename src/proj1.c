/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt

*--------------------------------------------------------------------*
*                       Airport Manager                              *
*                                                                    *
* 1st Project - 21/22                                                *
* Computer Science and Engineering (Alameda)                         *
* Instituto Superior Técnico                                         *
*--------------------------------------------------------------------*/

#include "constants.h"
#include "prototypes.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	int c, run = 1;

	while (run == 1) {
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

/*
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport() {
	/**/
}

/*
 * Handles the 'l' command.
 */
void ListAirports() {
	/**/
}

/*
 * Handles the 'v' command.
 */
void AddFlight() {
	/**/
}

/*
 * Handles the 'p' command.
 */
void FlightDeparturesInAirport() {
	/**/
}

/*
 * Handles the 'c' command.
 */
void FlightArrivalsInAirport() {
	/**/
}

/*
 * Handles the 't' command.
 */
void AdvanceSystemDate() {
	/**/
}
