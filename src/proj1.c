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
#include "proj1.h"

/* Global variables */
int totalAirports; 									/* tracks the total amount of airports added by the user */
int totalFlights; 									/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 					/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 					/* stores all of the current flights */
int sortedAirports[MAX_AIRPORTS]; 					/* stores the indexes of all the airports, sorted by the alphabetical order of the IDs */
int sortedFlights_departure[MAX_FLIGHTS];			/* stores the indexes of all the flights, sorted by departure date and time */
int sortedFlights_arrival[MAX_FLIGHTS];				/* stores the indexes of all the flights, sorted by arrival date and time */
clock global_date = {1, 1, 2022, 0, 0};				/* stores the system date of the system */
clock max_date = {1, 1, 2023, 0, 0};				/* stores the date that is one year in future from the system date */
const int days_months[MONTHS] = {31, 28, 31, 30, 	/* stores the amount of days per month in a non leap year */
								31, 30, 31, 31,
								30, 31, 30, 31};
/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * Exits program if letter 'q' is inserted.
 */
int main() {
	char c;

	while ((c = getchar()) != EOF) {
		switch(c) {
			case 'q':
				/* 'q' command exits the program */
				exit(0);
			case 'a':
				AddAirport();
				break;
			case 'l':
				ListAirports();
				break;
			case 'v':
				AddFlight_ListFlights();
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
	airport new_airport;

	GetOneArgument(new_airport.id, 0);
	GetOneArgument(new_airport.country, 0);
	GetOneArgument(new_airport.city, 1);
	new_airport.departures = 0;

	if (CheckAddAirportErrors(new_airport.id)) {
		return;
	}

	/* actually add the aiport to the system */
	allAirports[totalAirports] = new_airport;
	AddSortedAirport(new_airport);

	printf(AIRPORT_ADD_PRINT, new_airport.id);

	totalAirports++;
}

/**
 * Handles the 'l' command.
 * If no arguments are provided it will print all of the airports in
 * alphabetical order of their ids.
 * Otherwise, the function prints the airports with the specified IDs.
 */
void ListAirports() {
	char id[ID_LENGTH];
	int i, state;

	/* if there is no arguments it lists all airports */
	if (getchar() == '\n') {
		for (i = 0; i < totalAirports; i++) {
			PrintAirport(allAirports[sortedAirports[i]]);
		}
		return;
	}

	do {
		state = GetOneArgument(id, 0);

		if (CheckAirportExistence(id)) {
			printf(AIRPORT_ERR_NO_ID, id);
			continue;
		}
		i = GetAirport(id);

		PrintAirport(allAirports[sortedAirports[i]]);
	} while (state != 1);
}

/**
 * Handles the 'v' command.
 * If no arguments are provided it will print all of the flights in the order
 * they were created by.
 * Otherwise, the function adds a new flight to the system with the specified
 * flight code, id of departure airport, id of arrival aiport, date of
 * departure, duration of flight, date of arrival and capacity of the flight.
 */
void AddFlight_ListFlights() {
	flight new_flight;
	char date[DATE_LENGTH], time[TIME_LENGTH];
	char duration[TIME_LENGTH], capacity[MAX_CAPACITY_LENGTH];
	int departure_airport;

	/* if there is no arguments it lists all flights */
	if (getchar() == '\n') {
		ListAllFlights();
		return;
	}

	GetOneArgument(new_flight.flight_code, 0);
	GetOneArgument(new_flight.departure_id, 0);
	GetOneArgument(new_flight.arrival_id, 0);
	GetOneArgument(date, 0);
	GetOneArgument(time, 0);
	GetOneArgument(duration, 0);
	GetOneArgument(capacity, 0);

	new_flight.date_departure = ReadClock(date, time);
	new_flight.duration = ReadDuration(duration);
	new_flight.capacity = atoi(capacity);
	new_flight.date_arrival = UpdateDate(new_flight.date_departure,
							  	new_flight.duration); /* gets the arrival date and time */

	if (CheckAddFlightErrors(new_flight)) {
		return;
	}

	/* actually add the flight to the system */
	allFlights[totalFlights] = new_flight;
	AddSortedFlight(sortedFlights_arrival, new_flight, 0);
	AddSortedFlight(sortedFlights_departure, new_flight, 1);

	/* updates the number of departures on the departure airport */
	departure_airport = GetAirport(new_flight.departure_id);
	allAirports[sortedAirports[departure_airport]].departures++;

	totalFlights++;
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID,
 * sorted by the date and time of departure.
 */
void FlightDeparturesInAirport() {
	char id[ID_LENGTH];
	int j, i;

	GetOneArgument(id, 0);
	if (CheckAirportExistence(id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		/* looks at the sorted flights to find the ones linked to the right */
		/* departure id. */
		i = sortedFlights_departure[j];
		if (strcmp(allFlights[i].departure_id, id) == 0) {
			printf(FLIGHT_PRINT, allFlights[i].flight_code,
		  						allFlights[i].arrival_id);

			PrintClock(allFlights[i].date_departure);
		}
	}
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID,
 * sorted by the date and time of arrival.
 */
void FlightArrivalsInAirport() {
	char id[ID_LENGTH];
	int j, i;

	GetOneArgument(id, 0);
	if (CheckAirportExistence(id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		/* looks at the sorted flights to find the ones linked to the right */
		/* arrival id. */
		i = sortedFlights_arrival[j];
		if (strcmp(allFlights[i].arrival_id, id) == 0) {
			printf(FLIGHT_PRINT, allFlights[i].flight_code,
		  						allFlights[i].departure_id);

			PrintClock(allFlights[i].date_arrival);
		}
	}
}

/**
 * Handles the 't' command.
 * Forwards the date of the system and sets the max date of the system.
 */
void AdvanceSystemDate() {
	char date[DATE_LENGTH];
	clock new_date;

	GetOneArgument(date, 0);
	new_date = ReadClock(date, START_DAY);
	if (CheckDateErrors(new_date)) {
		return;
	}

	global_date = new_date;
	max_date = new_date;
	max_date.year += 1; /* sets the max date one year in the future */

	printf(DATE_PRINT, global_date.day, global_date.month, global_date.year);
}

/**
 * Reads a single argument from stdin and stores it.
 * Fetches one argument (characters separated by spaces) from the standard
 * input and populates the given string with it.
 * Ignores any trailing white spaces and always stops at a newline or end of file.
 * If the mode is set to 0 it will fetch for an argument that contains whitespaces
 * and only end on a newline or end of file.
 */
char GetOneArgument(char *argument, const int mode) {
	int c, i = 0;

	while ((c = getchar()) != '\n' && c != EOF) {
		/* ignores trailing white space */
		if (i == 0 && isspace(c)) {
			continue;
		}
		/* finishes argument when a non trailing white space is read if the */
		/* mode is 0. */
		if (mode == 0 && isspace(c)) {
			break;
		}
		argument[i++] = c;
	}
	argument[i] = '\0';

	return (c != '\n' ? 0 : 1); /* returns a 0 when it reads the last argument */
}
