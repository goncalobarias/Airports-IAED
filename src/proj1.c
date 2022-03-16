/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt

*--------------------------------------------------------------------*
*                       Gestor de Aeroportos                         *
*                                                                    *
* Projeto 1 -  21/22                                                 *
* Licenciatura em Engenharia Informática e de Computadores (Alameda) *
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
 *
 */
void AddAirport() {
	/**/
}

/*
 *
 */
void ListAirports() {
	/**/
}

/*
 *
 */
void AddFlight() {
	/**/
}

/*
 *
 */
void FlightDeparturesInAirport() {
	/**/
}

/*
 *
 */
void FlightArrivalsInAirport() {
	/**/
}

/*
 *
 */
void AdvanceSystemDate() {
	/**/
}
