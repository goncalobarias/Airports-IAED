/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include "proj1.h"

/* stores the amount of days per month in a non leap year */
const int days_months[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 *
 */
int CheckDateErrors(clock date_depart) {
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
clock UpdateDate(clock date_departure, clock duration) {
	int date_departure_mins = ConvertDatesToMins(date_departure);
	int dur = ConvertDatesToMins(duration);
	int updated_date_mins = date_departure_mins + dur;
	int i = 0;
	clock date_arrival;

	/* updates the year */
	date_arrival.year = date_departure.year + updated_date_mins / MINS_YEAR;
	updated_date_mins = updated_date_mins % MINS_YEAR;

	/* updates the month */
	while (updated_date_mins >= days_months[i++] * MINS_DAY) {
		updated_date_mins -= days_months[i - 1] * MINS_DAY;
	}
	date_arrival.month = i;

	/* updates the day */
	date_arrival.day = updated_date_mins / MINS_DAY + 1;
	updated_date_mins = updated_date_mins % MINS_DAY;

	/* updates the hours and minutes */
	date_arrival.hours = updated_date_mins / MINS_HOUR;
	date_arrival.minutes = updated_date_mins % MINS_HOUR;

	return date_arrival;
}

/**
 *
 */
int ConvertDatesToMins(clock formatted_date) {
	int mins = 0, i, year_difference;

	/* calculates the year in minutes */
	if (formatted_date.year > 0)
		year_difference = formatted_date.year - 2022;
	else
		year_difference = 0;
	mins += (year_difference * MINS_YEAR);

	/* calculates the month in minutes */
	for (i = 0; i < formatted_date.month - 1; i++)
		mins += days_months[i] * MINS_DAY;

	/* calculats the day in minutes */
	if (formatted_date.day > 0)
		mins += (formatted_date.day - 1) * MINS_DAY;

	/* calculates the hour in minutes and adds the rest into minutes */
	mins += formatted_date.hours * MINS_HOUR;
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
int CompareFlightDates(flight flight_1, flight flight_2, int mode) {
	if (mode == 0)
		return CompareDates(flight_1.date_arrival, flight_2.date_arrival, 1);
	else
		return CompareDates(flight_1.date_departure, flight_2.date_departure, 1);
}

/**
 *
 */
clock ReadClock(char *calendar_date, char *hours_mins) {
	clock date_departure;
	char hifen[1], double_dots[1];

	/* reads the calendar date */
	sscanf(calendar_date, "%d%c%d%c%d", &date_departure.day, hifen,
						&date_departure.month, hifen, &date_departure.year);

	/* reads the time */
	sscanf(hours_mins, "%d%c%d", &date_departure.hours, double_dots,
						&date_departure.minutes);

	return date_departure;
}

void PrintClock(clock date) {
	printf(FULL_DATE_PRINT, date.day, date.month, date.year, date.hours,
							date.minutes);
}
