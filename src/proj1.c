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
int totalAirports; 							/* tracks the total amount of airports added by the user */
int totalFlights; 							/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 			/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 			/* stores all of the current flights */
char sortedIDs[MAX_AIRPORTS][ID_LENGTH]; 	/* stores all of the IDs sorted by alphabetical order */
int sortedFlights[MAX_FLIGHTS];				/* stores the indexes of all the flights, sorted by date and time */
clock global_date = {1, 1, 2022, 0, 0};		/* stores the current date of the system */
clock max_date = {1, 1, 2023, 0, 0};		/* stores the date that is one year in future from the current date */

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * If the "run" variable has a value of 0 the program stops.
 */
int main() {
	char c;
	int run = 1;

	while (run) {
		c = getchar();
		switch(c) {
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
	char id[ID_LENGTH], country[MAX_COUNTRY_LENGTH], city[MAX_CITY_LENGTH];

	GetOneArgument(id, 0);
	GetOneArgument(country, 0);
	GetOneArgument(city, 1);

	if (CheckAddAirportErrors(id)) return;

	/* actually add the aiport to the system */
	strcpy(allAirports[totalAirports].id, id);
	strcpy(allAirports[totalAirports].country, country);
	strcpy(allAirports[totalAirports].city, city);
	allAirports[totalAirports].departures = 0;

	AddSortedAirportID(id);

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
	char id[ID_LENGTH];
	int i, state;

	if (getchar() == '\n') {
		ListAllAirports();
		return;
	}

	do {
		state = GetOneArgument(id, 0);

		if ((i = GetAirportFromID(id)) == -1) {
			printf(AIRPORT_ERR_NO_ID, id);
			continue;
		}

		printf("%s %s %s %d\n", id, allAirports[i].city, allAirports[i].country,
		 						allAirports[i].departures);
	} while (state != 1);
}

/**
 * Handles the 'v' command.
 * If no arguments are provided it will print all of the flights in alphabetical
 * order.
 * Otherwise, the function adds a new flight to the system with the specified
 * flight code, airport of departure id, airport of arrival id, date of
 * departure, time of departure, duration of flight and capacity of the flight.
 */
void AddFlight_ListFlights() {
	char flight_code[MAX_FLIGHT_CODE_LENGTH], arrival_id[ID_LENGTH];
	char departure_id[ID_LENGTH], date[DATE_LENGTH], time[TIME_LENGTH];
	char duration[TIME_LENGTH], capacity[MAX_CAPACITY_LENGTH];
	int i;

	if (getchar() == '\n') {
		ListAllFlights();
		return;
	}

	GetOneArgument(flight_code, 0);
	GetOneArgument(departure_id, 0);
	GetOneArgument(arrival_id, 0);
	GetOneArgument(date, 0);
	GetOneArgument(time, 0);
	GetOneArgument(duration, 0);
	GetOneArgument(capacity, 0);

	/* flight code */
	if (CheckFlightCodeErrors(flight_code, date, time)) return;
	strcpy(allFlights[totalFlights].flight_code, flight_code);

	/* arrival id */
	if (GetAirportFromID(arrival_id) == -1) {
		printf(AIRPORT_ERR_NO_ID, arrival_id);
		return;
	}
	strcpy(allFlights[totalFlights].arrival_id, arrival_id);

	/* departure id */
	if (GetAirportFromID(departure_id) == -1) {
		printf(AIRPORT_ERR_NO_ID, departure_id);
		return;
	}
	strcpy(allFlights[totalFlights].departure_id, departure_id);

 	/* date & time */
	if (CheckTooManyFlights() || CheckDateErrors(date, time)) return;
	allFlights[totalFlights].date_departure = ReadClock(date, time);

	/* duration */
	if (CheckDurationErrors(duration)) return;
	allFlights[totalFlights].duration = ReadClock(NO_DATE, duration);

	/* capacity */
	if (CheckCapacityErrors(capacity)) return;
	allFlights[totalFlights].capacity = atoi(capacity);

	AddSortedFlight(allFlights[totalFlights]);

	i = GetAirportFromID(departure_id);
	allAirports[i].departures += 1;

	totalFlights++;
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID.
 */
void FlightDeparturesInAirport() {
	char id[ID_LENGTH];
	int j, i;

	GetOneArgument(id, 0);
	if (GetAirportFromID(id) == -1) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		i = sortedFlights[j];
		if (strcmp(allFlights[i].departure_id, id) == 0) {
			printf("%s %s", allFlights[i].flight_code, allFlights[i].arrival_id);

			printf(" %02d-%02d-%04d %02d:%02d\n", allFlights[i].date_departure.day,
										allFlights[i].date_departure.month,
										allFlights[i].date_departure.year,
										allFlights[i].date_departure.hours,
										allFlights[i].date_departure.minutes);
		}
	}
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID.
 */
void FlightArrivalsInAirport() {
	char id[ID_LENGTH];
	clock new_date;
	int j, i;

	GetOneArgument(id, 0);
	if (GetAirportFromID(id) == -1) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		i = sortedFlights[j];
		if (strcmp(allFlights[i].arrival_id, id) == 0) {
			new_date = UpdateDate(allFlights[i].date_departure,
								allFlights[i].duration);

			printf("%s %s", allFlights[i].flight_code, allFlights[i].departure_id);

			printf(" %02d-%02d-%04d %02d:%02d\n", new_date.day, new_date.month,
											new_date.year, new_date.hours,
											new_date.minutes);
		}
	}
}

/**
 * Handles the 't' command.
 * Forwards the date of the system.
 */
void AdvanceSystemDate() {
	char new_date[DATE_LENGTH];

	GetOneArgument(new_date, 0);
	if (CheckDateErrors(new_date, START_DAY)) return;

	global_date = ReadClock(new_date, START_DAY);
	max_date = global_date;
	max_date.year += 1;

	printf("%02d-%02d-%04d\n", global_date.day, global_date.month,
							global_date.year);
}

/**
 *
 */
char GetOneArgument(char *argument, const int state) {
	int c, i = 0;

	while ((c = getchar()) != '\n' && c != EOF) {
		if (i == 0 && isspace(c))
			continue;
		if (state == 0 && isspace(c))
			break;
		argument[i++] = c;
	}
	argument[i] = '\0';

	return (c != '\n' ? 0 : 1);
}
