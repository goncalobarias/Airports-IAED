/*
 *		File: proj1.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Email: goncalo.barias@tecnico.ulisboa.pt
 *		Course: Computer Science and Engineering (Alameda) - Instituto Superior Técnico
 *		Description: Main project file for the 1st IAED Project - 21/22 (Airport Manager).
 */

#include <stdio.h>
#include <string.h>
#include "main.h"

/* Global variables */
int totalAirports; 							/* tracks the total amount of airports added by the user */
int totalFlights; 							/* tracks the total amount of flights added by the user */
airport allAirports[MAX_AIRPORTS]; 			/* stores all of the current airports */
flight allFlights[MAX_FLIGHTS]; 			/* stores all of the current flights */
int sortedAirports[MAX_AIRPORTS]; 			/* stores the indexes of all the airports, sorted by the alphabetical order of the IDs */
clock global_date = {1, 1, 2022, 0, 0};		/* stores the system date of the system */
clock max_date = {1, 1, 2023, 0, 0};		/* stores the date that is one year in future from the system date */
const int days_months_ac[MONTHS] = 			/* stores the accumulated days per month in a non leap year (jan = 1) */
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

int main() {
	/* executes the program until the user inserts the 'q' command */
	while (handle_commands()) {
	}

	return 0;
}

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * Returns 1 if the program should continue, otherwise it returns 0 in
 * order to exit the program.
 */
int handle_commands() {
	char command = getchar();
	switch(command) {
		case 'q': /* 'q' command exits the program */
			return 0;
		case 'a': AddAirport();
			return 1;
		case 'l': ListAirports();
			return 1;
		case 'v': AddFlight_ListFlights();
			return 1;
		case 'p': FlightDeparturesInAirport();
			return 1;
		case 'c': FlightArrivalsInAirport();
			return 1;
		case 't': AdvanceSystemDate();
			return 1;
		case 'r': AddBooking_ListBookings();
			return 1;
		case 'e': DeleteBooking_Flight();
			return 1;
		default: /* ignore all unknown commands */
			return 1;
	}
}

/**
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport() {
	airport new_airport;

	ReadAirport(&new_airport);

	if (CheckAddAirportErrors(new_airport.id)) {
		return;
	}

	/* actually adds the airport to the system */
	allAirports[totalAirports] = new_airport;
	AddSortedAirport(allAirports[totalAirports]);
	printf(AIRPORT_ADD_PRINT, allAirports[totalAirports].id);

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
		ListAllAirports();
		return;
	}

	do {
		/* when the state is 1 it means it just read the last argument */
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
 * information.
 */
void AddFlight_ListFlights() {
	flight new_flight;
	int departure_airport;

	/* if there is no arguments it lists all flights */
	if (getchar() == '\n') {
		ListAllFlights();
		return;
	}

	ReadFlight(&new_flight);

	if (CheckAddFlightErrors(new_flight)) {
		return;
	}

	/* actually add the flight to the system */
	allFlights[totalFlights] = new_flight;

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
	int j, size = 0, indexes[MAX_FLIGHTS];

	GetOneArgument(id, 0);
	if (CheckAirportExistence(id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		/* looks at the sorted flights to find the ones linked to the right */
		/* departure id */
		if (strcmp(allFlights[j].departure_id, id) == 0) {
			AddSortedFlight(indexes, size++, allFlights[j], j, 1);
		}
	}

	for (j = 0; j < size; j++) {
		printf(FLIGHT_PRINT, allFlights[indexes[j]].flight_code,
	  						allFlights[indexes[j]].arrival_id);
		PrintClock(allFlights[indexes[j]].date_departure);
	}
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID,
 * sorted by the date and time of arrival.
 */
void FlightArrivalsInAirport() {
	char id[ID_LENGTH];
	int j, size = 0, indexes[MAX_FLIGHTS];

	GetOneArgument(id, 0);
	if (CheckAirportExistence(id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	for (j = 0; j < totalFlights; j++) {
		/* looks at the sorted flights to find the ones linked to the right */
		/* departure id */
		if (strcmp(allFlights[j].arrival_id, id) == 0) {
			AddSortedFlight(indexes, size++, allFlights[j], j, 0);
		}
	}

	for (j = 0; j < size; j++) {
		printf(FLIGHT_PRINT, allFlights[indexes[j]].flight_code,
	  						allFlights[indexes[j]].departure_id);
		PrintClock(allFlights[indexes[j]].date_arrival);
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
		printf(DATE_ERR_INVALID);
		return;
	}

	global_date = new_date;
	max_date = new_date;
	max_date.year++; /* sets the max date one year in the future */

	printf(DATE_PRINT, global_date.day, global_date.month, global_date.year);
}

/**
 * Fetches one argument (characters separated by spaces) from the standard
 * input and populates the given string with it.
 * Ignores any trailing white spaces and always stops at a newline or end of file.
 * If the mode is set to 1 it will fetch for an argument that contains whitespaces
 * and only end on a newline or end of file.
 */
char GetOneArgument(char *argument, const int mode) {
	int i = 0;
	char c = getchar();

	/* ignores trailing white spaces */
	while (c == ' ' || c == '\t') {
		c = getchar();
	}

	/* finishes argument when a non trailing white space is read if the */
	/* mode is 0 */
	while (c != '\n' && c != EOF) {
		if (mode == 0 && (c == ' ' || c == '\t')) {
			break;
		}
		argument[i++] = c;
		c = getchar();
	}
	argument[i] = '\0';

	return (c != '\n' ? 0 : 1); /* returns a 1 when it reads the last argument */
}
