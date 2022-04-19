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
 * create a flight. It transforms some of the arguments into their appropriate form.
 */
flight* ReadFlight() {
	flight* new_flight = SecureMalloc(sizeof(flight));
	char flight_code[MAX_FLIGHT_CODE_LENGTH], date[DATE_LENGTH];
	char time[TIME_LENGTH], duration[TIME_LENGTH], capacity[MAX_ARG_LENGTH];

	GetOneArgument(flight_code, 0);
	GetOneArgument(new_flight->departure_id, 0);
	GetOneArgument(new_flight->arrival_id, 0);
	GetOneArgument(date, 0);
	GetOneArgument(time, 0);
	GetOneArgument(duration, 0);
	GetOneArgument(capacity, 0);

	new_flight->flight_code =
		(char*)SecureMalloc(sizeof(char) * (strlen(flight_code) + 1));
	strcpy(new_flight->flight_code, flight_code);

	new_flight->date_departure = ReadClock(date, time);
	new_flight->flight_key = GetFlightKey(new_flight->flight_code,
											new_flight->date_departure);
	new_flight->duration = ReadDuration(duration);
	new_flight->reservations = list_create();
	new_flight->occupation = 0;
	new_flight->capacity = atoi(capacity);
	new_flight->date_arrival = UpdateDate(new_flight->date_departure,
							  	new_flight->duration); /* gets the arrival date and time */

	return new_flight;
}

/**
 *
 */
void AddFlight(global_store* global) {
	flight* new_flight = ReadFlight();
	node_t* flight_node;
	int departure_airport;

	if (CheckAddFlightErrors(global, new_flight)) {
		ClearFlight(new_flight);
		return;
	}

	/* actually add the flight to the system */
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

		PrintClock(flight_print->date_departure);
	}
}

/**
 * Checks if a new airport is valid.
 * If so, it returns 0, otherwise returns 1.
 * Auxiliary function of the 'v' command.
 */
int CheckAddFlightErrors(global_store* global, flight* new_flight) {
	flight* flight_check = GetFlight(global, new_flight->flight_code,
								  	new_flight->date_departure);

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
	} else if (CheckDateErrors(global, new_flight->date_departure)) {
		printf(DATE_ERR_INVALID);
	} else if (new_flight->duration > MAX_DURATION_MINS) {
		printf(FLIGHT_ERR_INVALID_DURATION);
	} else if (new_flight->capacity < MIN_PASSENGERS) {
		printf(FLIGHT_ERR_INVALID_CAPACITY);
	} else {
		return 0;
	}

	return 1;
}

/**
 * Checks if the flight code it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 * Auxiliary function of the 'v' command and the CheckAddFlightErrors function.
 */
int CheckFlightCodeErrors(char* flight_code) {
	int i;

	for (i = 0; i < 2; i++) {
		if (!IsUpperCase(flight_code[i])) {
			return 1; /* it doesn't start with two uppercase letters */
		}
	}

	if (flight_code[i] == '0') {
	 	return 1; /* the digits start with a zero */
	}
	while (flight_code[i] != '\0') {
		if (!IsDigit(flight_code[i++])) {
			return 1; /* the digits are invalid */
		}
	}

	if (i > 6) {
		return 1; /* length of flight code is too big */
	}

	return 0;
}

/**
 *
 */
flight* GetFlight(global_store* global, char* flight_code, clock* date_depart) {
	char* flight_key = GetFlightKey(flight_code, date_depart);
	node_t* flight_node;
	hash_elem* elem = hashtable_get(global->flightsTable, flight_code,
								 	flight_key, GetFlightCalendarDate);

	free(flight_key);

	if (elem == NULL) {
		return NULL;
	} else {
		flight_node = (node_t*)elem->data;
		return (flight*)flight_node->data;
	}
}

/**
 *
 */
char* GetFlightCode(void* node) {
	node_t* flight_node = node;
	flight* flight_get = flight_node->data;

	return flight_get->flight_code;
}

/**
 *
 */
char* GetFlightCalendarDate(void* node) {
	node_t* flight_node = node;
	flight* flight_get = flight_node->data;

	return flight_get->flight_key;
}

/**
 *
 */
char* GetFlightKey(char* flight_code, clock* date) {
	char* flight_key =
		SecureMalloc(sizeof(char) * (strlen(flight_code) + DATE_LENGTH + 1));

	sprintf(flight_key, "%s %02d-%02d-%04d", flight_code, date->day,
		 									date->month,
											date->year);

	return flight_key;
}

/**
 *
 */
int CheckFlightCodeExistence(global_store* global, char* flight_code) {
	hash_elem* elem = hashtable_get(global->flightsTable, flight_code,
								 	flight_code, GetFlightCode);

	return (elem == NULL ? 0 : 1);
}

/**
 *
 */
void ClearFlight(void* tmp_flight) {
	flight* flight_delete = tmp_flight;

	free(flight_delete->reservations);
	free(flight_delete->flight_code);
	free(flight_delete->flight_key);
	free(flight_delete->date_departure);
	free(flight_delete->date_arrival);
	free(flight_delete);
}

/**
 *
 */
void PrintFlights(node_t* flight_head, const int mode) {
	node_t *p, *aux;
	flight* tmp_flight;

	for (p = flight_head; p != NULL; p = aux) {
		aux = p->next;
		tmp_flight = (flight*)p->data;
		if (mode == 0) {
			printf(FLIGHT_PRINT,
		  			tmp_flight->flight_code, tmp_flight->arrival_id);
			PrintClock(tmp_flight->date_departure);
		} else {
			printf(FLIGHT_PRINT,
		  			tmp_flight->flight_code, tmp_flight->departure_id);
			PrintClock(tmp_flight->date_arrival);
		}
		free(p);
	}
}

/**
 *
 */
void RemoveFlights(global_store* global, char* flight_code) {
	node_t *p, *aux, *flight_node;
	flight* flight_delete;
	list_t* all_elems = hashtable_get_all(global->flightsTable, flight_code,
									   	flight_code, GetFlightCode);

	for (p = all_elems->first; p != NULL; p = aux) {
		aux = p->next;
		flight_node = (node_t*)p->data;
		flight_delete = (flight*)flight_node->data;
		if (strcmp(flight_delete->flight_code, flight_code) == 0) {
			RemoveFlight(global, flight_node);
		}
		free(p);
	}

	list_destroy(all_elems);
}

/**
 *
 */
void RemoveFlight(global_store* global, node_t* flight_node) {
	flight* flight_delete = flight_node->data;

	RemoveBookings(global, flight_delete->reservations);
	hashtable_remove(global->flightsTable, GetFlightCode(flight_node),
					GetFlightCalendarDate(flight_node), GetFlightCalendarDate);
	list_remove(global->allFlights, flight_node);
	ClearFlight(flight_delete);
}
