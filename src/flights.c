/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj1.h"

/* stores the amount of days per month in a non leap year */
const int days_months[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 *
 */
int CheckFlightCodeErrors(char *flight_code, char *date_departure, char *time) {
	int i;
	date date_depart = ReadDate(date_departure, time);

	for (i = 0; i < 2; i++)
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}

	while (flight_code[i++] != '\0') {
		if (i == 2 && flight_code[i - 1] == '0') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		if (flight_code[i - 1] < '0' || flight_code[i - 1] > '9') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
		if (i >= 6) {
			printf(FLIGHT_ERR_INVALID);
			return 1;
		}
	}

	for (i = 0; i < totalFlights; i++) {

		if (strcmp(allFlights[i].flight_code, flight_code) == 0
			&& CompareDates(allFlights[i].date_departure, date_depart) == 0) {
			printf(FLIGHT_ERR_DUPLICATE);
			return 1;
		}
	}

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
int CheckDateErrors(char *date_departure, char *time) {
	date date_depart = ReadDate(date_departure, time);
	int one_year_future = ConvertDatesToMins(date_depart) + MINS_YEAR;

	if (CompareDates(date_depart, present_date) == 1
		|| one_year_future < ConvertDatesToMins(date_depart)) {
		printf(FLIGHT_ERR_INVALID_DATE);
		return 1;
	}

	return 0;
}

/**
 *
 */
int CheckDurationErrors(char *timespan) {
	time duration = ReadTime(timespan);
	int dur = duration.hours * 60 + duration.minutes;

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
date UpdateDate(date date_departure, time duration) {
	int date_departure_mins = ConvertDatesToMins(date_departure);
	int dur = duration.hours * 60 + duration.minutes;
	int updated_date_mins = date_departure_mins + dur;
	int i = 0;
	date date_arrival;

	date_arrival.year = date_departure.year + updated_date_mins / MINS_YEAR;
	updated_date_mins = updated_date_mins % MINS_YEAR;

	while (updated_date_mins >= days_months[i++] * 24 * 60) {
		updated_date_mins -= days_months[i];
		date_arrival.month = i;
	}

	date_arrival.day = updated_date_mins / (24 * 60);
	updated_date_mins = updated_date_mins % (24 * 60);

	date_arrival.clock.hours = updated_date_mins / 60;
	date_arrival.clock.minutes = updated_date_mins % 60;

	return date_arrival;
}

/**
 *
 */
int ConvertDatesToMins(date formatted_date) {
	int mins = 0, i;

	if (formatted_date.year == 2023)
		mins += 525600;

	for (i = 0; i < formatted_date.month - 1; i++)
		mins += days_months[i] * 24 * 60;

	mins += (formatted_date.day - 1) * 24 * 60;
	mins += formatted_date.clock.hours * 60;
	mins += formatted_date.clock.minutes;

	return mins;
}

/**
 *
 */
int CompareDates(date date_1, date date_2) {
	int date_1_mins = ConvertDatesToMins(date_1);
	int date_2_mins = ConvertDatesToMins(date_2);

	if (date_1_mins == date_2_mins) return 0;

	return (date_2_mins > date_1_mins ? 1 : -1);
}

/**
 *
 */
time ReadTime(char *hours_mins) {
	time clock;
	char double_dots[1];

	sscanf(hours_mins, "%d%c%d", &clock.hours, double_dots, &clock.minutes);

	return clock;
}

/**
 *
 */
date ReadDate(char *calendar_date, char *hours_mins) {
	date date_departure;
	char hifen[1];

	sscanf(calendar_date, "%d%c%d%c%d", &date_departure.day, hifen,
						&date_departure.month, hifen, &date_departure.year);

	date_departure.clock = ReadTime(hours_mins);

	return date_departure;
}

/**
 *
 */
void AddSortedFlight(flight new_flight) {
	int first, middle, last, index, i;

	if (totalFlights == 0)
		index = 0;
	else {
		first = 0;
		last = totalFlights - 1;
		middle = (first + last) / 2;
		while (first < last) {
			if (CompareDates(allFlights[sortedFlights[middle]].date_departure,
							new_flight.date_departure) > 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first;
		if (CompareDates(allFlights[sortedFlights[middle]].date_departure,
						new_flight.date_departure) > 0)
			index += 1;
	}

	for (i = totalFlights - 1; i >= index; i--) {
		sortedFlights[i + 1] = sortedFlights[i];
	}
	sortedFlights[index] = totalFlights;
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

		printf(" %02d-%02d-%04d %02d:%02d\n", allFlights[i].date_departure.day,
										allFlights[i].date_departure.month,
										allFlights[i].date_departure.year,
										allFlights[i].date_departure.clock.hours,
										allFlights[i].date_departure.clock.minutes);
	}
}
