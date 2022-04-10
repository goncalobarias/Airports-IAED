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
void ReadFlight(flight *new_flight) {
	char date[DATE_LENGTH], time[TIME_LENGTH];
	char duration[TIME_LENGTH], capacity[MAX_CAPACITY_LENGTH];

	GetOneArgument(new_flight->flight_code, 0);
	GetOneArgument(new_flight->departure_id, 0);
	GetOneArgument(new_flight->arrival_id, 0);
	GetOneArgument(date, 0);
	GetOneArgument(time, 0);
	GetOneArgument(duration, 0);
	GetOneArgument(capacity, 0);

	new_flight->date_departure = ReadClock(date, time);
	new_flight->duration = ReadDuration(duration);
	new_flight->capacity = atoi(capacity);
	new_flight->date_arrival = UpdateDate(new_flight->date_departure,
							  	new_flight->duration); /* gets the arrival date and time */
}

/**
 * Checks if a new airport is valid.
 * If so, it returns 0, otherwise returns 1.
 * Auxiliary function of the 'v' command.
 */
int CheckAddFlightErrors(flight new_flight) {
	if (CheckFlightCodeErrors(new_flight.flight_code)) {
		printf(FLIGHT_ERR_INVALID);
	} else if (CheckDuplicateFlight(new_flight.flight_code,
								 new_flight.date_departure)) {
		printf(FLIGHT_ERR_DUPLICATE);
	} else if (CheckAirportExistence(new_flight.arrival_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight.arrival_id); /* no arrival id exists */
	} else if (CheckAirportExistence(new_flight.departure_id)) {
		printf(AIRPORT_ERR_NO_ID, new_flight.departure_id); /* no departure id exists */
	} else if (totalFlights >= MAX_FLIGHTS) {
		printf(FLIGHT_ERR_TOO_MANY); /* too many flights */
	} else if (CheckDateErrors(new_flight.date_departure)) {
		printf(DATE_ERR_INVALID);
	} else if (new_flight.duration > MAX_DURATION_MINS) {
		printf(FLIGHT_ERR_INVALID_DURATION);
	} else if (new_flight.capacity > MAX_PASSENGERS
		|| new_flight.capacity < MIN_PASSENGERS) {
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
int CheckFlightCodeErrors(const char flight_code[]) {
	int i;

	for (i = 0; i < 2; i++) {
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			return 1; /* it doesn't start with two uppercase letters */
		}
	}

	if (flight_code[i] == '0') {
	 	return 1; /* the digits start with a zero */
	}
	while (flight_code[i] != '\0') {
		if (flight_code[i] < '0' || flight_code[i] > '9') {
			return 1; /* the digits are invalid */
		}
		i++;
	}

	if (i > 6) {
		return 1; /* length of flight code is too big */
	}

	return 0;
}

/**
 *
 */
int CheckDuplicateFlight(const char flight_code[], clock date_depart) {
	int i;

	for (i = 0; i < totalFlights; i++) {
		if (strcmp(allFlights[i].flight_code, flight_code) == 0 &&
			CompareDates(allFlights[i].date_departure, date_depart, 0) == 0) {
			return 1; /* another flight exists on the same day with the same code */
		}
	}

	return 0;
}

/**
 * Uses binary search to insert a flight into an array sorted by date.
 * It uses the sort array to insert the index of the new flight on the right position.
 * If the mode is 0 it will insert sorted by arrival date, otherwise it
 * will be sorted by departure date.
 */
void AddSortedFlight(int sort[], int size, flight new_flight, int index,
					 const int mode) {
	int left = 0, right = size - 1, middle, comp, i;

	while (left <= right) {
		middle = (left + right) / 2;
		comp = CompareFlightDates(allFlights[sort[middle]], new_flight, mode);
		if (comp < 0) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	for (i = size - 1; i >= left; i--) {
		sort[i + 1] = sort[i];
	}
	sort[left] = index;
}

/**
 * Lists all of the flights by the order they were created.
 * Auxiliary function of the 'v' command.
 */
void ListAllFlights() {
	int i;

	for (i = 0; i < totalFlights; i++) {
		printf(FLIGHT_FULL_PRINT, allFlights[i].flight_code,
		 						allFlights[i].departure_id,
								allFlights[i].arrival_id);

		PrintClock(allFlights[i].date_departure);
	}
}
