/*
 *		File: dates.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All the functions used to manipulate dates.
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"

const int days_months_ac[MONTHS] = 			/* stores the accumulated days per month in a non leap year (jan = 1) */
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/**
 * Checks if the date is not in the past or one year in the future.
 * If it's invalid it returns 1, otherwise it returns 0.
 * Auxiliary function of the 't' command.
 */
int CheckDateErrors(global_store* global, clock* date) {
	clock* max_date = SecureMalloc(sizeof(clock));

	max_date->year = global->date->year + 1;
	max_date->month = global->date->month;
	max_date->day = global->date->day;
	max_date->hours = global->date->hours;
	max_date->minutes = global->date->minutes;

	if (CompareDates(global->date, date, CMP_DATES_MIN) > 0
		|| CompareDates(max_date, date, CMP_DATES_DAY) < 0) {
		free(max_date);
		return TRUE;
	}

	free(max_date);
	return FALSE;
}

/**
 * Calculates the date it receives in minutes relative to the start of the
 * program (01-01-2022). If the date is before the start of the program the
 * number will be negative.
 */
int ConvertDatesToMins(clock* formatted_date) {
	int mins = 0, year_difference;

	/* calculates the relative year in minutes */
	year_difference = formatted_date->year - 2022;
	mins += year_difference * MINS_YEAR;

	/* calculates the relative month in minutes */
	mins += days_months_ac[formatted_date->month - 1] * MINS_DAY;

	/* calculats the relative day in minutes */
	mins += (formatted_date->day - 1) * MINS_DAY;

	/* calculates the relative hour in minutes and adds the rest into relative minutes */
	mins += formatted_date->hours * MINS_HOUR;
	mins += formatted_date->minutes;

	return mins;
}

/**
 * Updates a date it receives with a certain duration.
 * Transforms the date into minutes and adds the duration to it, then it
 * converts the new date in minutes into a proper formatted date.
 */
clock* UpdateDate(clock* date, int duration) {
	int date_depart_mins = ConvertDatesToMins(date);
	int updated_date_mins = date_depart_mins + duration, i = 1;
	clock* new_date = SecureMalloc(sizeof(clock));

	/* updates the year */
	new_date->year = date->year + updated_date_mins / MINS_YEAR;
	updated_date_mins = updated_date_mins % MINS_YEAR;

	/* updates the month */
	while (i <= 11 && updated_date_mins >= days_months_ac[i] * MINS_DAY) {
		i++;
	}
	new_date->month = i;
	updated_date_mins -= days_months_ac[i - 1] * MINS_DAY;

	/* updates the day */
	new_date->day = updated_date_mins / MINS_DAY + 1;
	updated_date_mins = updated_date_mins % MINS_DAY;

	/* updates the hours and minutes */
	new_date->hours = updated_date_mins / MINS_HOUR;
	new_date->minutes = updated_date_mins % MINS_HOUR;

	return new_date;
}

/**
 * Transforms two dates into minutes and compares them.
 * If the mode is set to CMP_DATES_DAY, it will compare the dates by day of year
 * instead of minutes. Returns 1 if the second date is bigger then the first
 * one, 0 if both are equal dates or -1 if the first date is bigger then the second.
 */
int CompareDates(clock* date_1, clock* date_2, const int mode) {
	int date_1_mins = ConvertDatesToMins(date_1);
	int date_2_mins = ConvertDatesToMins(date_2);

	/* if the mode is 0, two dates will be equal if they are on the same day */
	if (mode == CMP_DATES_DAY && date_1->month == date_2->month
		&& date_1->day == date_2->day && date_1->year == date_2->year) {
		return 0;
	}

	if (date_1_mins == date_2_mins) {
		return 0;
	}

	return (date_2_mins < date_1_mins ? 1 : -1);
}

/**
 * Receives two flights and compares the departure dates of the two flights.
 */
int CompareFlightDatesDeparture(void* flight_1, void* flight_2) {
	flight* flight_cmp_1 = flight_1;
	flight* flight_cmp_2 = flight_2;

	return CompareDates(flight_cmp_1->date_depart,
					 	flight_cmp_2->date_depart, CMP_DATES_MIN);
}

/**
 * Receives two flights and compares the arrival dates of the two flights.
 */
int CompareFlightDatesArrival(void* flight_1, void* flight_2) {
	flight* flight_cmp_1 = flight_1;
	flight* flight_cmp_2 = flight_2;

	return CompareDates(flight_cmp_1->date_arrival,
					 	flight_cmp_2->date_arrival, CMP_DATES_MIN);
}

/**
 * Reads the clock and retrives a formatted date, allocating the necessary
 * memory to store it.
 */
clock* ReadClock(char calendar_date[], char hours_mins[]) {
	clock* date = SecureMalloc(sizeof(clock));
	char hifen[1], double_dots[1];

	/* reads the calendar date */
	sscanf(calendar_date, "%d%c%d%c%d", &date->day, hifen, &date->month, hifen,
										&date->year);

	/* reads the time */
	sscanf(hours_mins, "%d%c%d", &date->hours, double_dots, &date->minutes);

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
void PrintClock(clock* date) {
	printf(DATE_FULL_PRINT, date->day, date->month, date->year, date->hours,
							date->minutes);
}
