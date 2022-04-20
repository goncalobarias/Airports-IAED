/*
 *		File: proj1.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Email: goncalo.barias@tecnico.ulisboa.pt
 *		Course: Computer Science and Engineering (Alameda) - Instituto Superior Técnico
 *		Description: Main project file for the 1st IAED Project - 21/22 (Airport Manager).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main() {
	char command;

	/* initializes the global variables store */
	global_store* global = GlobalInit();

	/* executes the program until the user inserts the 'q' command or reaches the EOF */
	while ((command = getchar()) != 'q' && command != EOF) {
		HandleCommands(global, command);
	}

	/* clears all of the memory and terminates the program */
	ExitProgram(global);

	return 0;
}

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 */
void HandleCommands(global_store* global, char command) {
	switch(command) {
		case 'a': AddAirport(global);
			return;
		case 'l': ListAirports(global);
			return;
		case 'v': AddFlight_ListFlights(global);
			return;
		case 'p': FlightDeparturesInAirport(global);
			return;
		case 'c': FlightArrivalsInAirport(global);
			return;
		case 't': AdvanceSystemDate(global);
			return;
		case 'r': AddBooking_ListBookings(global);
			return;
		case 'e': DeleteBooking_Flight(global);
			return;
		default: /* ignore all unknown commands */
			return;
	}
}

/**
 * Handles the 'a' command.
 * Adds a new airport to the system with the specified ID, country and city.
 */
void AddAirport(global_store* global) {
	airport new_airport;

	ReadAirport(&new_airport);

	if (CheckAddAirportErrors(global, new_airport.id)) {
		return;
	}

	/* actually adds the airport to the system */
	global->allAirports[global->totalAirports] = new_airport;
	AddSortedAirport(global, global->allAirports[global->totalAirports]);
	printf(AIRPORT_ADD_PRINT, global->allAirports[global->totalAirports].id);

	++global->totalAirports;
}

/**
 * Handles the 'l' command.
 * If no arguments are provided it will print all of the airports in
 * alphabetical order of their ids.
 * Otherwise, the function prints the airports with the specified IDs.
 */
void ListAirports(global_store* global) {
	char id[ID_LENGTH];
	int i, state;

	/* if there is no arguments it lists all airports */
	if (getchar() == '\n') {
		ListAllAirports(global);
		return;
	}

	do {
		/* when the state is 1 it means it just read the last argument */
		state = GetOneArgument(id, UNTIL_WHITESPACE);

		if (CheckAirportExistence(global, id)) {
			printf(AIRPORT_ERR_NO_ID, id);
			continue;
		}
		i = GetAirport(global, id);

		PrintAirport(global->allAirports[global->sortedAirports[i]]);
	} while (state != 1);
}

/**
 * Handles the 'v' command.
 * If no arguments are provided it will print all of the flights in the order
 * they were created by.
 * Otherwise, the function adds a new flight to the system with the specified
 * information.
 */
void AddFlight_ListFlights(global_store* global) {
	/* if there is no arguments it lists all flights */
	if (getchar() == '\n') {
		ListAllFlights(global);
		return;
	} else {
		AddFlight(global);
		return;
	}
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID,
 * sorted by the date and time of departure.
 */
void FlightDeparturesInAirport(global_store* global) {
	char id[ID_LENGTH];
	node_t* ptr;
	list_t* tmp_list;
	flight* tmp_flight;

	GetOneArgument(id, UNTIL_WHITESPACE);
	if (CheckAirportExistence(global, id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	tmp_list = list_create();

	for (ptr = global->allFlights->first; ptr != NULL; ptr = ptr->next) {
		tmp_flight = (flight*)ptr->data;
		/* looks at the sorted flights to find the ones linked to the right */
		/* departure id */
		if (strcmp(tmp_flight->departure_id, id) == 0) {
			list_insert(tmp_list, tmp_flight);
		}
	}

	sort_list(tmp_list, CompareFlightDatesDeparture);

	PrintFlights(tmp_list->first, DEPARTURE_INFO);

	list_destroy(tmp_list);
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID,
 * sorted by the date and time of arrival.
 */
void FlightArrivalsInAirport(global_store* global) {
	char id[ID_LENGTH];
	node_t* ptr;
	list_t* tmp_list;
	flight* tmp_flight;

	GetOneArgument(id, UNTIL_WHITESPACE);
	if (CheckAirportExistence(global, id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	tmp_list = list_create();

	for (ptr = global->allFlights->first; ptr != NULL; ptr = ptr->next) {
		tmp_flight = (flight*)ptr->data;
		/* looks at the sorted flights to find the ones linked to the right */
		/* arrival id */
		if (strcmp(tmp_flight->arrival_id, id) == 0) {
			list_insert(tmp_list, tmp_flight);
		}
	}

	sort_list(tmp_list, CompareFlightDatesArrival);

	PrintFlights(tmp_list->first, ARRIVAL_INFO);

	list_destroy(tmp_list);
}

/**
 * Handles the 't' command.
 * Forwards the date of the system.
 */
void AdvanceSystemDate(global_store* global) {
	char date[DATE_LENGTH];
	clock* new_date;

	GetOneArgument(date, UNTIL_WHITESPACE);
	new_date = ReadClock(date, START_DAY);

	if (CheckDateErrors(global, new_date)) {
		printf(DATE_ERR_INVALID);
		free(new_date);
		return;
	}

	free(global->date);
	global->date = new_date;

	printf(DATE_PRINT,
			global->date->day, global->date->month, global->date->year);
}

/**
 * Initializes all of the global variables used throughout the project.
 */
global_store* GlobalInit() {
	global_store* global = SecureMalloc(sizeof(global_store));
	global->date = SecureMalloc(sizeof(clock));

	global->totalAirports = 0;
	global->allFlights = list_create();
	global->flightsTable = hashtable_create(HASHTABLE_START_SIZE);
	global->bookingsTable = hashtable_create(HASHTABLE_START_SIZE);

	global->date->day = 1;
	global->date->month = 1;
	global->date->year = 2022;
	global->date->hours = 0;
	global->date->minutes = 0;

	return global;
}

/**
 * Fetches one argument (characters separated by spaces) from the standard
 * input and populates the given string with it.
 * Ignores any trailing white spaces and always stops at a newline or end of file.
 * If the mode is set to UNTIL_EOL it will fetch for an argument that contains
 * whitespaces and only end on a newline or end of file.
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
		if (mode == UNTIL_WHITESPACE && (c == ' ' || c == '\t')) {
			break;
		}
		argument[i++] = c;
		c = getchar();
	}
	argument[i] = '\0';

	return (c != '\n' ? 0 : 1); /* returns a 1 when it reads the last argument */
}
