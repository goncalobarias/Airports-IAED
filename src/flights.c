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
	clock date_depart = ReadClock(date_departure, time);

	for (i = 0; i < 2; i++)
		if (flight_code[i] < 'A' || flight_code[i] > 'Z') {
			printf(FLIGHT_ERR_INVALID);
			return 1;
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
	clock date_depart = ReadClock(date_departure, time);

	if (CompareDates(global_date, date_depart, 1) > 0 ||
		CompareDates(max_date, date_depart, 0) < 0) {
		printf(FLIGHT_ERR_INVALID_DATE);
		return 1;
	}

	return 0;
}

/**
 *
 */
int CheckDurationErrors(char *timespan) {
	clock duration = ReadClock(NO_DATE, timespan);
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
clock UpdateDate(clock date_departure, clock duration) {
	int date_departure_mins = ConvertDatesToMins(date_departure);
	int dur = ConvertDatesToMins(duration);
	int updated_date_mins = date_departure_mins + dur;
	int i = 0;
	clock date_arrival;

	date_arrival.year = date_departure.year + updated_date_mins / MINS_YEAR;
	updated_date_mins = updated_date_mins % MINS_YEAR;

	while (updated_date_mins >= days_months[i++] * MINS_DAY) {
		updated_date_mins -= days_months[i - 1] * MINS_DAY;
	}
	date_arrival.month = i;

	date_arrival.day = updated_date_mins / MINS_DAY + 1;
	updated_date_mins = updated_date_mins % MINS_DAY;

	date_arrival.hours = updated_date_mins / 60;
	date_arrival.minutes = updated_date_mins % 60;

	return date_arrival;
}

/**
 *
 */
int ConvertDatesToMins(clock formatted_date) {
	int mins = 0, i, year_difference;

	if (formatted_date.year > 0)
		year_difference = formatted_date.year - 2022;
	else
		year_difference = 0;
	mins += (year_difference * MINS_YEAR);

	for (i = 0; i < formatted_date.month - 1; i++)
		mins += days_months[i] * 24 * 60;

	if (formatted_date.day > 0)
		mins += (formatted_date.day - 1) * 24 * 60;
	mins += formatted_date.hours * 60;
	mins += formatted_date.minutes;

	return mins;
}

/**
 *
 */
int CompareDates(clock date_1, clock date_2, int mode) {
	int date_1_mins = ConvertDatesToMins(date_1);
	int date_2_mins = ConvertDatesToMins(date_2);

	if (date_1_mins == date_2_mins)
		return 0;

	if (mode == 0 && (date_1_mins - date_2_mins < MINS_DAY
		|| date_1_mins - date_2_mins > -MINS_DAY) && date_1.day == date_2.day
		&& date_1.year == date_2.year)
		return 0;

	return (date_2_mins < date_1_mins ? 1 : -1);
}

/**
 *
 */
clock ReadClock(char *calendar_date, char *hours_mins) {
	clock date_departure;
	char hifen[1], double_dots[1];

	sscanf(calendar_date, "%d%c%d%c%d", &date_departure.day, hifen,
						&date_departure.month, hifen, &date_departure.year);
	sscanf(hours_mins, "%d%c%d", &date_departure.hours, double_dots,
						&date_departure.minutes);

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
							new_flight.date_departure, 1) < 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first;
		if (CompareDates(allFlights[sortedFlights[first]].date_departure,
						new_flight.date_departure, 1) < 0)
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
										allFlights[i].date_departure.hours,
										allFlights[i].date_departure.minutes);
	}
}
