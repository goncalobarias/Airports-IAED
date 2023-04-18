/*
 *		File: dates.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All the functions used to manipulate dates.
 */

#include <stdio.h>
#include "proj1.h"

/**
 * Checks if the date is not in the past or one year in the future.
 * If it's invalid it returns 1, otherwise it returns 0.
 * Auxiliary function of the 't' command.
 */
int CheckDateErrors(clock date_depart) {
	if (CompareDates(global_date, date_depart, 1) > 0
		|| CompareDates(max_date, date_depart, 0) < 0) {
		return 1;
	}

	return 0;
}

/**
 * Calculates the date it receives in minutes relative to the start of the
 * program (01-01-2022). If the date is before the start of the program the
 * number will be negative.
 */
int ConvertDatesToMins(clock formatted_date) {
	int mins = 0, i, year_difference;

	/* calculates the relative year in minutes */
	year_difference = formatted_date.year - 2022;
	mins += (year_difference * MINS_YEAR);

	/* calculates the relative month in minutes */
	for (i = 0; i < formatted_date.month - 1; i++) {
		mins += days_months[i] * MINS_DAY;
	}

	/* calculats the relative day in minutes */
	if (formatted_date.day > 0) {
		mins += (formatted_date.day - 1) * MINS_DAY;
	}

	/* calculates the relative hour in minutes and adds the rest into relative minutes */
	mins += formatted_date.hours * MINS_HOUR;
	mins += formatted_date.minutes;

	return mins;
}

/**
 * Updates a date it receives with a certain duration.
 * Transforms the date into minutes and adds the duration to it, then it
 * converts the new date in minutes into a proper formatted date.
 */
clock UpdateDate(clock date, int duration) {
	int date_departure_mins = ConvertDatesToMins(date);
	int updated_date_mins = date_departure_mins + duration, i = 0;

	/* updates the year */
	date.year += updated_date_mins / MINS_YEAR;
	updated_date_mins = updated_date_mins % MINS_YEAR;

	/* updates the month */
	while (updated_date_mins >= days_months[i++] * MINS_DAY) {
		updated_date_mins -= days_months[i - 1] * MINS_DAY;
	}
	date.month = i;

	/* updates the day */
	date.day = updated_date_mins / MINS_DAY + 1;
	updated_date_mins = updated_date_mins % MINS_DAY;

	/* updates the hours and minutes */
	date.hours = updated_date_mins / MINS_HOUR;
	date.minutes = updated_date_mins % MINS_HOUR;

	return date;
}

/**
 * Transforms two dates into minutes and compares them.
 * If the mode is set to 0, it will compare the dates by day of year instead of
 * minutes. Returns 1 if the second date is bigger then the first on, 0 if
 * both are equal dates or -1 if the first date is bigger then the second.
 */
int CompareDates(clock date_1, clock date_2, const int mode) {
	int date_1_mins = ConvertDatesToMins(date_1);
	int date_2_mins = ConvertDatesToMins(date_2);

	/* if the mode is 0, two dates will be equal if they are on the same day */
	if (mode == 0 && date_1.month == date_2.month
		&& date_1.day == date_2.day && date_1.year == date_2.year) {
		return 0;
	}

	if (date_1_mins == date_2_mins) {
		return 0;
	}

	return (date_2_mins < date_1_mins ? 1 : -1);
}

/**
 * Used as a proxy function to the CompareDates function. Receives two
 * flights and compares the dates of the two flights depending on the mode it
 * receives. If the mode is 0, it will compare the flights based on their
 * arrival date, otherwise it will compare them based on their departure date.
 */
int CompareFlightDates(flight flight_1, flight flight_2, const int mode) {
	if (mode == 0) {
		return CompareDates(flight_1.date_arrival, flight_2.date_arrival, 1);
	} else {
		return CompareDates(flight_1.date_departure, flight_2.date_departure,
							1);
	}
}

/**
 * Reads the clock and retrives a formatted date.
 */
clock ReadClock(char calendar_date[], char hours_mins[]) {
	clock date;
	char hifen[1], double_dots[1];

	/* reads the calendar date */
	sscanf(calendar_date, "%d%c%d%c%d", &date.day, hifen, &date.month, hifen,
										&date.year);

	/* reads the time */
	sscanf(hours_mins, "%d%c%d", &date.hours, double_dots, &date.minutes);

	return date;
}

/**
 * Reads the hours and time of a duration of time and converts it into minutes.
 */
int ReadDuration(char duration[]) {
	int hours, minutes, dur;
	char double_dots[1];

	/* reads the duration time */
	sscanf(duration, "%d%c%d", &hours, double_dots, &minutes);

	dur = hours * MINS_HOUR + minutes;

	return dur;
}

/**
 * Receives a date and prints it in a proper formatted way.
 */
void PrintClock(clock date) {
	printf(DATE_FULL_PRINT, date.day, date.month, date.year, date.hours,
							date.minutes);
}
