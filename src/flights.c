/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj1.h"

/**
 *
 */
int CheckFlightCodeErrors(char *flight_code, clock date_depart) {
	int i;

	for (i = 0; i < 2; i++) {
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
	}

	while (flight_code[i] != '\0') {
		if (i == 2 && flight_code[i] == '0') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		if (flight_code[i] < '0' || flight_code[i] > '9') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		if (i >= 6) {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		i++;
	}

	for (i = 0; i < totalFlights; i++) {
		if (strcmp(allFlights[i].flight_code, flight_code) == 0 &&
			CompareDates(allFlights[i].date_departure, date_depart, 0) == 0) {
			printf(FLIGHT_ERR_DUPLICATE);
			return 1;
		}
	}

	return 0;
}

/**
 *
 */
int CheckAddFlightErrors(flight new_flight) {
	int dur = ConvertDatesToMins(new_flight.duration);

	if (CheckFlightCodeErrors(new_flight.flight_code,
						   new_flight.date_departure))
		return 1;

	/* check arrival id existence */
	if (GetAirportFromID(new_flight.arrival_id) == -1) {
		printf(AIRPORT_ERR_NO_ID, new_flight.arrival_id);
		return 1;
	}

	/* check departure id existence */
	if (GetAirportFromID(new_flight.departure_id) == -1) {
		printf(AIRPORT_ERR_NO_ID, new_flight.departure_id);
		return 1;
	}

	if (totalFlights >= MAX_FLIGHTS) {
		printf(FLIGHT_ERR_TOO_MANY);
		return 1;
	}

	if (CheckDateErrors(new_flight.date_departure)) return 1;

	if (dur > MAX_DURATION) {
		printf(FLIGHT_ERR_INVALID_DURATION);
		return 1;
	}

	if (new_flight.capacity > MAX_PASSENGERS
		|| new_flight.capacity < MIN_PASSENGERS) {
		printf(FLIGHT_ERR_INVALID_CAPACITY);
		return 1;
	}

	return 0;
}

/**
 *
 */
void AddSortedFlight_departure(flight new_flight) {
	int first, middle, last, index, i;

	if (totalFlights == 0)
		index = 0;
	else {
		first = 0;
		last = totalFlights - 1;
		middle = (first + last) / 2;
		while (first < last) {
			if (CompareDates(allFlights[sortedFlights_departure[middle]].date_departure,
							new_flight.date_departure, 1) < 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first + 1;
		if (CompareDates(allFlights[sortedFlights_departure[first]].date_departure,
						new_flight.date_departure, 1) > 0)
			index -= 1;
	}

	for (i = totalFlights - 1; i >= index; i--) {
		sortedFlights_departure[i + 1] = sortedFlights_departure[i];
	}
	sortedFlights_departure[index] = totalFlights;
}

/**
 *
 */
void AddSortedFlight_arrival(flight new_flight) {
	int first, middle, last, index, i;

	if (totalFlights == 0)
		index = 0;
	else {
		first = 0;
		last = totalFlights - 1;
		middle = (first + last) / 2;
		while (first < last) {
			if (CompareDates(allFlights[sortedFlights_arrival[middle]].date_arrival,
							new_flight.date_arrival, 1) < 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first + 1;
		if (CompareDates(allFlights[sortedFlights_arrival[first]].date_arrival,
						new_flight.date_arrival, 1) > 0)
			index -= 1;
	}

	for (i = totalFlights - 1; i >= index; i--) {
		sortedFlights_arrival[i + 1] = sortedFlights_arrival[i];
	}
	sortedFlights_arrival[index] = totalFlights;
}

/**
 *
 */
void ListAllFlights() {
	int i;

	for (i = 0; i < totalFlights; i++) {
		printf("%s %s %s", allFlights[i].flight_code,
		 					allFlights[i].departure_id,
							allFlights[i].arrival_id);

		PrintClock(allFlights[i].date_departure);
	}
}

/**
 *
 */
void PrintAirport(airport airport_1) {
	printf(AIRPORT_PRINT, airport_1.id, airport_1.city, airport_1.country,
 						airport_1.departures);
}
