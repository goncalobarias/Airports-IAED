/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <string.h>
#include "proj1.h"

/**
 *
 */
int CheckFlightCodeErrors(const char *flight_code, clock date_depart) {
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
						   new_flight.date_departure)) {
		return 1;
	}

	if (CheckAirportExistence(new_flight.arrival_id)) return 1;

	if (CheckAirportExistence(new_flight.departure_id)) return 1;

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
void AddSortedFlight(int sort[], flight new_flight, const int mode) {
	int first, middle, last, comp, index, i;

	if (totalFlights == 0) {
		index = 0;
	} else {
		first = 0;
		last = totalFlights - 1;
		middle = (first + last) / 2;
		while (first < last) {
			comp = CompareFlightDates(allFlights[sort[middle]], new_flight, mode);
			if (comp < 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first + 1;
		if (CompareFlightDates(allFlights[sort[first]], new_flight,
								mode) > 0)
			index -= 1;
	}

	for (i = totalFlights - 1; i >= index; i--) {
		sort[i + 1] = sort[i];
	}
	sort[index] = totalFlights;;
}

/**
 *
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
