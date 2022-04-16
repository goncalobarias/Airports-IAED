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
#include "structures.h"

int main() {
	/* initializes the global variables store */
	global_store* global = GlobalInit();

	/* executes the program until the user inserts the 'q' command */
	while (HandleCommands(global)) {
	}

	/* clear all of the memory and terminate program */
	ExitProgram(global);

	return 0;
}

/**
 * Handles command input.
 * Reads one letter inserted by the user and executes the right command.
 * Returns 1 if the program should continue, otherwise it returns 0 in
 * order to exit the program.
 */
int HandleCommands(global_store* global) {
	char command = getchar();
	switch(command) {
		case 'q': /* 'q' command exits the program */
			return 0;
		case 'a': AddAirport(global);
			return 1;
		case 'l': ListAirports(global);
			return 1;
		case 'v': AddFlight_ListFlights(global);
			return 1;
		case 'p': FlightDeparturesInAirport(global);
			return 1;
		case 'c': FlightArrivalsInAirport(global);
			return 1;
		case 't': AdvanceSystemDate(global);
			return 1;
		case 'r': AddBooking_ListBookings(global);
			return 1;
		case 'e': DeleteBooking_Flight(global);
			return 1;
		default: /* ignore all unknown commands */
			return 1;
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
		state = GetOneArgument(id, 0);

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
	flight* new_flight;
	int departure_airport;

	/* if there is no arguments it lists all flights */
	if (getchar() == '\n') {
		ListAllFlights(global);
		return;
	}

	new_flight = ReadFlight();

	if (CheckAddFlightErrors(global, new_flight)) {
		ClearFlight(new_flight);
		return;
	}

	/* actually add the flight to the system */
	list_insert(global->allFlights, new_flight);
	++global->totalFlights;

	/* updates the number of departures on the departure airport */
	departure_airport = GetAirport(global, new_flight->departure_id);
	++global->allAirports[global->sortedAirports[departure_airport]].departures;
}

/**
 * Handles the 'p' command.
 * Lists all of the flights that depart from the airport with the given ID,
 * sorted by the date and time of departure.
 */
void FlightDeparturesInAirport(global_store* global) {
	char id[ID_LENGTH];
	node_t* p1;
	list_t* tmp_list;
	flight* tmp_flight;

	GetOneArgument(id, 0);
	if (CheckAirportExistence(global, id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	tmp_list = list_create();

	for (p1 = global->allFlights->first; p1 != NULL; p1 = p1->next) {
		tmp_flight = (flight*)p1->data;
		/* looks at the sorted flights to find the ones linked to the right */
		/* departure id */
		if (strcmp(tmp_flight->departure_id, id) == 0)
			list_insert(tmp_list, tmp_flight);
	}

	p1 = list_mergesort(tmp_list->first, CompareFlightDatesDeparture);

	PrintFlights(p1, 0);

	free(tmp_list);
}

/**
 * Handles the 'c' command.
 * Lists all of the flights that arrive at the airport with the given ID,
 * sorted by the date and time of arrival.
 */
void FlightArrivalsInAirport(global_store* global) {
	char id[ID_LENGTH];
	node_t* p1;
	list_t* tmp_list;
	flight* tmp_flight;

	GetOneArgument(id, 0);
	if (CheckAirportExistence(global, id)) {
		printf(AIRPORT_ERR_NO_ID, id);
		return;
	}

	tmp_list = list_create();

	for (p1 = global->allFlights->first; p1 != NULL; p1 = p1->next) {
		tmp_flight = (flight*)p1->data;
		/* looks at the sorted flights to find the ones linked to the right */
		/* arrival id */
		if (strcmp(tmp_flight->arrival_id, id) == 0)
			list_insert(tmp_list, tmp_flight);
	}

	p1 = list_mergesort(tmp_list->first, CompareFlightDatesArrival);

	PrintFlights(p1, 1);

	free(tmp_list);
}

/**
 * Handles the 't' command.
 * Forwards the date of the system and sets the max date of the system.
 */
void AdvanceSystemDate(global_store* global) {
	char date[DATE_LENGTH];
	clock new_date;

	GetOneArgument(date, 0);
	new_date = ReadClock(date, START_DAY);

	if (CheckDateErrors(global, new_date)) {
		printf(DATE_ERR_INVALID);
		return;
	}

	global->date = new_date;
	global->max_date = new_date;
	++global->max_date.year; /* sets the max date one year in the future */

	printf(DATE_PRINT, global->date.day, global->date.month, global->date.year);
}

/**
 *
 */
global_store* GlobalInit() {
	global_store* global = SecureMalloc(sizeof(global_store));

	global->totalAirports = 0;
	global->totalFlights = 0;
	global->allFlights = list_create();
	global->bookingsTable = hashtable_create(HASHTABLE_START_SIZE);

	global->date.day = 1;
	global->date.month = 1;
	global->date.year = 2022;
	global->date.hours = 0;
	global->date.minutes = 0;

	global->max_date.day = 1;
	global->max_date.month = 1;
	global->max_date.year = 2023;
	global->max_date.hours = 0;
	global->max_date.minutes = 0;

	return global;
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
