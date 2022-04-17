/*
 *		File: flights.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All functions used to create and modify flights.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "structures.h"

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
	new_flight->flight_key = CreateFlightKey(flight_code,
							new_flight->date_departure);
	new_flight->duration = ReadDuration(duration);
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
	int departure_airport;
	node_t* new_node;

	if (CheckAddFlightErrors(global, new_flight)) {
		ClearFlight(new_flight);
		return;
	}

	/* actually add the flight to the system */
	new_node = list_insert(global->allFlights, new_flight);
	global->flightsTable = hashtable_insert(global->flightsTable, new_node,
										 GetFlightKey(new_node));

	/* updates the number of departures on the departure airport */
	departure_airport = GetAirport(global, new_flight->departure_id);
	++global->allAirports[global->sortedAirports[departure_airport]].departures;
}

/**
 * Lists all of the flights by the order they were created.
 * Auxiliary function of the 'v' command.
 */
void ListAllFlights(global_store* global) {
	node_t* p;
	flight* flight_print;

	for (p = global->allFlights->first; p != NULL; p = p->next) {
		flight_print = (flight*)p->data;

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
	if (CheckFlightCodeErrors(new_flight->flight_code)) {
		printf(FLIGHT_ERR_INVALID);
	} else if (GetFlight(global, new_flight->flight_code,
					  new_flight->date_departure) != NULL) {
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
flight* GetFlight(global_store* global, char* flight_code, clock date_depart) {
	char* flight_key = CreateFlightKey(flight_code, date_depart);
	hash_elem* elem =
		hashtable_get(global->flightsTable, flight_key, GetFlightKey);
	node_t* flight_node;
	flight* flight_return;

	free(flight_key);

	if (elem == NULL) {
		return NULL;
	}

	flight_node = (node_t*)elem->data;
	flight_return = (flight*)flight_node->data;

	return (flight_return);
}

/**
 *
 */
char* GetFlightKey(void* new_node) {
	node_t* flight_node = new_node;
	flight* flight_get_key = flight_node->data;

	return (flight_get_key->flight_key);
}

/**
 *
 */
char* CreateFlightKey(char* flight_code, clock date_depart) {
	char* flight_key =
		SecureMalloc(sizeof(char) * (strlen(flight_code) + DATE_LENGTH + 1));

	sprintf(flight_key, "%s %02d-%02d-%04d",
		 				flight_code,
		 				date_depart.day,
		 				date_depart.month,
		 				date_depart.year);

	return (flight_key);
}

/**
 *
 */
int CheckFlightCodeExistence(global_store* global, char* flight_code) {
	node_t* p;
	flight* tmp_flight;

	for (p = global->allFlights->first; p != NULL; p = p->next) {
		tmp_flight = (flight*)p->data;
		if (strcmp(tmp_flight->flight_code, flight_code) == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 *
 */
void ClearFlight(void* flight_delete) {
	free(((flight*)flight_delete)->flight_code);
	free(((flight*)flight_delete)->flight_key);
	free((flight*)flight_delete);
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
			printf(FLIGHT_PRINT, tmp_flight->flight_code, tmp_flight->arrival_id);
			PrintClock(tmp_flight->date_departure);
		} else {
			printf(FLIGHT_PRINT, tmp_flight->flight_code, tmp_flight->departure_id);
			PrintClock(tmp_flight->date_arrival);
		}
		free(p);
	}
}

/**
 *
 */
void RemoveFlights(global_store* global, char* flight_code) {
	int i;
	node_t* tmp_node;
	flight* tmp_flight;
	booking* tmp_booking;

	for (i = 0; i < global->flightsTable->size; i++) {
		if (hash_elem_dead(global->flightsTable->table[i])) {
			continue;
		}
		tmp_node = (node_t*)global->flightsTable->table[i]->data;
		tmp_flight = (flight*)tmp_node->data;
		if (strcmp(tmp_flight->flight_code, flight_code) == 0) {
			ClearFlight(tmp_flight);
			list_remove(global->allFlights, tmp_node);
			global->flightsTable->table[i]->state = HASHTABLE_DELETED;
		}
	}

	for (i = 0; i < global->bookingsTable->size; i++) {
		if (hash_elem_dead(global->bookingsTable->table[i])) {
			continue;
		}
		tmp_booking = (booking*)global->bookingsTable->table[i]->data;
		if (strcmp(tmp_booking->flight_code, flight_code) == 0) {
			hashtable_remove(global->bookingsTable, tmp_booking->booking_code,
							GetBookingKey, ClearBooking);
		}
	}
}

/**
 *
 */
void RemoveAllFlights(global_store* global) {
	flight* tmp_flight;
	node_t* tmp_node;
	hash_elem* elem;
	int i;

	for (i = 0; i < global->flightsTable->size; i++) {
		elem = global->flightsTable->table[i];
		if (elem == NULL) {
			continue;
		} else if (elem->state == HASHTABLE_DELETED) {
			free(elem);
		} else {
			tmp_node = (node_t*)elem->data;
			tmp_flight = (flight*)tmp_node->data;
			ClearFlight(tmp_flight);
			list_remove(global->allFlights, tmp_node);
			free(elem);
		}
	}

	free(global->allFlights);

	hashtable_destroy(global->flightsTable);
}
