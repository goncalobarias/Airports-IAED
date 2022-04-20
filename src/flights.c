/*
 *		File: flights.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All functions used to create and modify flights.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/**
 * Uses the GetOneArgument function to read all of the arguments from stdin that
 * create a flight and allocates the necessary memory to store them. It transforms
 * some of the arguments into their appropriate form.
 * Auxiliary function to the 'v' command.
 */
flight* ReadFlight() {
	flight* new_flight = SecureMalloc(sizeof(flight));
	char flight_code[MAX_FLIGHT_CODE_LENGTH], date[DATE_LENGTH];
	char time[TIME_LENGTH], duration[TIME_LENGTH], capacity[MAX_ARG_LENGTH];

	GetOneArgument(flight_code, UNTIL_WHITESPACE);
	GetOneArgument(new_flight->departure_id, UNTIL_WHITESPACE);
	GetOneArgument(new_flight->arrival_id, UNTIL_WHITESPACE);
	GetOneArgument(date, UNTIL_WHITESPACE);
	GetOneArgument(time, UNTIL_WHITESPACE);
	GetOneArgument(duration, UNTIL_WHITESPACE);
	GetOneArgument(capacity, UNTIL_WHITESPACE);

	new_flight->flight_code =
		(char*)SecureMalloc(sizeof(char) * (strlen(flight_code) + 1));
	strcpy(new_flight->flight_code, flight_code);

	new_flight->date_depart = ReadClock(date, time);
	new_flight->flight_key = CreateFlightKey(new_flight->flight_code,
										new_flight->date_depart);
	new_flight->duration = ReadDuration(duration);
	new_flight->bookings = list_create();
	new_flight->occupation = 0;
	new_flight->capacity = atoi(capacity);
	/* gets the arrival date and time */
	new_flight->date_arrival = UpdateDate(new_flight->date_depart,
							  			new_flight->duration);

	return new_flight;
}

/**
 * Adds a new flight to the system with the specified information.
 * Auxiliary function to the 'v' command.
 */
void AddFlight(global_store* global) {
	flight* new_flight = ReadFlight();
	node_t* flight_node;
	int departure_airport;

	if (CheckAddFlightErrors(global, new_flight)) {
		ClearFlight(new_flight);
		return;
	}

	/* stores the flight in the main list and saves the respective node in */
	/* flights hashtable */
	flight_node = list_insert(global->allFlights, new_flight);
	global->flightsTable = hashtable_insert(global->flightsTable, flight_node,
								 	GetFlightCode(flight_node), GetFlightCode);

	/* updates the number of departures on the departure airport */
	departure_airport = GetAirport(global, new_flight->departure_id);
	++global->allAirports[global->sortedAirports[departure_airport]].departures;
}

/**
 * Lists all of the flights by the order they were created.
 * Auxiliary function of the 'v' command.
 */
void ListAllFlights(global_store* global) {
	node_t* ptr;
	flight* flight_print;

	for (ptr = global->allFlights->first; ptr != NULL; ptr = ptr->next) {
		flight_print = (flight*)ptr->data;

		printf(FLIGHT_FULL_PRINT, flight_print->flight_code,
		 						flight_print->departure_id,
								flight_print->arrival_id);

		PrintClock(flight_print->date_depart);
	}
}

/**
 * Checks if the flight it receives is valid to be added to the system.
 * If so, it returns 0, otherwise returns 1.
 */
int CheckAddFlightErrors(global_store* global, flight* new_flight) {
	flight* flight_check = GetFlight(global, new_flight->flight_code,
								  	new_flight->date_depart);

	if (CheckFlightCodeErrors(new_flight->flight_code)) {
		printf(FLIGHT_ERR_INVALID);
	} else if (flight_check != NULL) {
		printf(FLIGHT_ERR_DUPLICATE);
	} else if (CheckAirportExistence(global, new_flight->arrival_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight->arrival_id); /* no arrival id exists */
	} else if (CheckAirportExistence(global, new_flight->departure_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight->departure_id); /* no departure id exists */
	} else if (global->flightsTable->elem_num >= MAX_FLIGHTS) {
		printf(FLIGHT_ERR_TOO_MANY); /* too many flights */
	} else if (CheckDateErrors(global, new_flight->date_depart)) {
		printf(DATE_ERR_INVALID);
	} else if (new_flight->duration > MAX_DURATION_MINS) {
		printf(FLIGHT_ERR_INVALID_DURATION);
	} else if (new_flight->capacity < MIN_PASSENGERS) {
		printf(FLIGHT_ERR_INVALID_CAPACITY);
	} else {
		return FALSE;
	}

	return TRUE;
}

/**
 * Checks if the flight code it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckFlightCodeErrors(char* flight_code) {
	int i;

	for (i = 0; i < 2; i++) {
		if (!IsUpperCase(flight_code[i])) {
			return TRUE; /* it doesn't start with two uppercase letters */
		}
	}

	if (flight_code[i] == '0') {
	 	return TRUE; /* the digits start with a zero */
	}
	while (flight_code[i] != '\0') {
		if (!IsDigit(flight_code[i++])) {
			return TRUE; /* the digits are invalid */
		}
	}

	if (i > 6) {
		return TRUE; /* length of flight code is too big */
	}

	return FALSE;
}

/**
 * Retrives the flight from the flights hashtable with the respective flight
 * key. It obtains the flight key from the GetFlightKey function. If it doesn't
 * find it, then it returns NULL.
 */
flight* GetFlight(global_store* global, char* flight_code, clock* date_depart) {
	char* flight_key = CreateFlightKey(flight_code, date_depart);
	node_t* flight_node;
	hash_elem* elem = hashtable_get(global->flightsTable, flight_code,
								 	flight_key, GetFlightKey);

	free(flight_key);

	if (elem == NULL) {
		return NULL;
	} else {
		flight_node = (node_t*)elem->data;
		return (flight*)flight_node->data;
	}
}

/**
 * Gets the flight code from the node it receives that is storing a flight.
 */
char* GetFlightCode(void* flight_node) {
	node_t* node = flight_node;
	flight* flight_get = node->data;

	return flight_get->flight_code;
}

/**
 * Gets the key of a given flight.
 */
char* GetFlightKey(void* flight_node) {
	node_t* node = flight_node;
	flight* flight_get = node->data;

	return flight_get->flight_key;
}

/**
 * Creates and allocates memory to a new flight key from the information it
 * receives.
 */
char* CreateFlightKey(char* flight_code, clock* date_depart) {
	char* flight_key =
		SecureMalloc(sizeof(char) * (strlen(flight_code) + DATE_LENGTH + 1));

	/* the format for a flight key is: <flight code> <calendar date> */
	sprintf(flight_key, "%s %02d-%02d-%04d", flight_code, date_depart->day,
		 									date_depart->month,
											date_depart->year);

	return flight_key;
}

/**
 * Checks if there are any flights with the given flight code already in the
 * system.
 */
int CheckFlightCodeExistence(global_store* global, char* flight_code) {
	hash_elem* elem = hashtable_get(global->flightsTable, flight_code,
								 	flight_code, GetFlightCode);

	return (elem == NULL ? 0 : 1);
}

/**
 * Clears all of the memory allocated to the flight it receives.
 */
void ClearFlight(void* tmp_flight) {
	flight* flight_delete = tmp_flight;

	free(flight_delete->bookings);
	free(flight_delete->flight_code);
	free(flight_delete->flight_key);
	free(flight_delete->date_depart);
	free(flight_delete->date_arrival);
	free(flight_delete);
}

/**
 * Receives the head of a flights list and prints the information of all the
 * flights in order. If the mode it receives is DEPARTURE_INFO, then it will
 * print the departure information, otherwise it will print the arrival
 * information.
 */
void PrintFlights(node_t* flight_head, const int mode) {
	node_t *ptr, *aux;
	flight* tmp_flight;

	for (ptr = flight_head; ptr != NULL; ptr = aux) {
		aux = ptr->next;
		tmp_flight = (flight*)ptr->data;
		if (mode == DEPARTURE_INFO) {
			printf(FLIGHT_PRINT,
		  			tmp_flight->flight_code, tmp_flight->arrival_id);
			PrintClock(tmp_flight->date_depart);
		} else {
			printf(FLIGHT_PRINT,
		  			tmp_flight->flight_code, tmp_flight->departure_id);
			PrintClock(tmp_flight->date_arrival);
		}
		free(ptr);
	}
}

/**
 * Retrives from the hashtable all of the flights with the given flight code
 * and removes each flight with the RemoveFlight function.
 */
void RemoveFlights(global_store* global, char* flight_code) {
	node_t *ptr, *aux, *flight_node;
	flight* flight_delete;
	list_t* all_elems = hashtable_get_all(global->flightsTable, flight_code,
									   	flight_code, GetFlightCode);

	for (ptr = all_elems->first; ptr != NULL; ptr = aux) {
		aux = ptr->next;
		flight_node = (node_t*)ptr->data;
		flight_delete = (flight*)flight_node->data;
		if (strcmp(flight_delete->flight_code, flight_code) == 0) {
			RemoveFlight(global, flight_node);
		}
		free(ptr);
	}

	list_destroy(all_elems);
}

/**
 * Receives the node of a flight to remove, clearing all of its memory and
 * removing all of its bookings. It removes it from the flights hashtable and
 * the main list.
 */
void RemoveFlight(global_store* global, node_t* flight_node) {
	flight* flight_delete = flight_node->data;

	RemoveBookings(global, flight_delete->bookings);
	hashtable_remove(global->flightsTable, GetFlightCode(flight_node),
					GetFlightKey(flight_node), GetFlightKey);
	list_remove(global->allFlights, flight_node);
	ClearFlight(flight_delete);
}
