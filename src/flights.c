/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj1.h"

/**
 *
 */
int CheckFlightCodeErrors(char *flight_code) {
	int i;

	for (i = 0; i < 2; i++)
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
	while (flight_code[i++] != '\0') {
		if (i == 2 && flight_code[i] == '0') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		else if (flight_code[i] < '0' || flight_code[i] > '9') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		if (i >= 6) {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
	}
	for (i = 0; i < totalFlights; i++)
		if (allFlights[i].date, )

	return 0;
}

/**
 *
 */
int CheckTooManyFlights() {
	if (totalFlights >= MAX_FLIGHTS) {
		printf(FLIGHT_ERR_TOO_MANY);
		return 1;
	}

	return 0;
}

/**
 *
 */
int CheckDateErrors(char *date) {
	printf("%s\n", date);
	return 0;
}

/**
 *
 */
int CheckDurationErrors(char *duration) {
	int dur = atoi(duration);

	if (dur > MAX_DURATION) {
		printf(FLIGHT_ERR_INVALID_DURATION);
		return 1;
	}

	return 0;
}

/**
 *
 */
int CheckCapacityErrors(char *capacity) {
	int cap = atoi(capacity);

	if (cap > MAX_PASSENGERS || cap < MIN_PASSENGERS) {
		printf(FLIGHT_ERR_INVALID_CAPACITY);
		return 1;
	}

	return 0;
}

/**
 *
 */
int ConvertDateTimeToMinutes(char *date, char *time) {
	int minutes = 0;
	printf("%s%s\n", date, time);
	return minutes;
}

/**
 *
  */
void ConvertMinutesToDateTime(char *date, char *time, int minutes) {
}

/**
 *
 */
void AddSortedFlight(char *id, int mode) {
	printf("%s%d\n", id, mode);
}

/**
 *
 */
void ListAllFlights() {
}
