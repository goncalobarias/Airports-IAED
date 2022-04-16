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
	new_flight->duration = ReadDuration(duration);
	new_flight->occupation = 0;
	new_flight->capacity = atoi(capacity);
	new_flight->date_arrival = UpdateDate(new_flight->date_departure,
							  	new_flight->duration); /* gets the arrival date and time */

	return new_flight;
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
	} else if (global->totalFlights >= MAX_FLIGHTS) {
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
flight* GetFlight(global_store* global, char* flight_code, clock date) {
	node_t* p;
	flight* tmp_flight;

	for (p = global->allFlights->first; p != NULL; p = p->next) {
		tmp_flight = (flight*)p->data;
		if (strcmp(tmp_flight->flight_code, flight_code) == 0
			&& CompareDates(tmp_flight->date_departure, date, 0) == 0) {
			return tmp_flight;
		}
	}

	return NULL;
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
char* GetFlightKey(void* flight_1) {
	return ((flight*)flight_1)->flight_code;
}

/**
 *
 */
void ClearFlight(void* flight_delete) {
	free(((flight*)flight_delete)->flight_code);
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
	booking* tmp_booking;
	int i;

	list_remove(global->allFlights, flight_code, GetFlightKey, ClearFlight);

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
