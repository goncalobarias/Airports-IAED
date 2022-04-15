/*
 *		File: proj2.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Email: goncalo.barias@tecnico.ulisboa.pt
 *		Course: Computer Science and Engineering (Alameda) - Instituto Superior Técnico
 *		Description: Main project file for the 2nd IAED Project - 21/22 (Airport Manager).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "structures.h"

/**
 *
 */
void AddBooking_ListBookings(global_store* global) {
	char flight_code[MAX_FLIGHT_CODE_LENGTH], calendar_date[DATE_LENGTH];
	int state;
	booking* new_booking;
	flight* flight_1;
	clock date;

	GetOneArgument(flight_code, 0);
	state = GetOneArgument(calendar_date, 0);
	date = ReadClock(calendar_date, START_DAY);

	if (state == 1) {
		ListBookings(global, flight_code, date);
		return;
	}

	new_booking = ReadBooking(flight_code, date);

	if (CheckAddBookingErrors(global, new_booking)) {
		ClearBooking(new_booking);
		return;
	}

	flight_1 = GetFlight(global, flight_code, new_booking->date_departure);
	flight_1->occupation += new_booking->res_num;
	global->bookingsTable =
		hashtable_insert(global->bookingsTable, new_booking,
						GetBookingKey(new_booking), ClearBooking);
}

/**
 *
 */
void DeleteBooking_Flight(global_store* global) {
	char code[MAX_ARG_LENGTH];

	GetOneArgument(code, 0);

	if (strlen(code) > MIN_BOOKING_CODE_LENGTH) {
		if (hashtable_get(global->bookingsTable, code, GetBookingKey) == NULL) {
			printf(BOOKING_FLIGHT_NA);
			return;
		}
		hashtable_remove(global->bookingsTable, code, GetBookingKey,
						ClearBooking);
	} else {
		if (!CheckFlightExistence(global, code)) {
			printf(BOOKING_FLIGHT_NA);
			return;
		}
		list_remove(global->allFlights, code, GetFlightKey, ClearFlight);
	}
}

/**
 *
 */
void ExitProgram(global_store* global) {
	list_destroy(global->allFlights, ClearFlight);
	hashtable_destroy(global->bookingsTable, ClearBooking);
	free(global);
}

/*		Utilities		*/

/**
 *
 */
void* SecureMalloc(unsigned int allocation) {
	void* ptr = malloc(allocation);

	if (ptr != NULL) {
		return ptr;
	} else {
		printf(MEMORY_ERR);
		exit(MEMORY_ERR_CODE);
	}

	return NULL;
}

/**
 *
 */
int IsPrime(int x) {
	int i;

	for (i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			return 0;
		}
	}

	return 1;
}

/**
 *
 */
int GetPrime(int minNum) {
	int prime = minNum;

	while (!IsPrime(prime)) {
		prime++;
	}

	return prime;
}

/**
 *
 */
int IsUpperCase(char c) {
	return (c >= 'A' && c <= 'Z' ? 1 : 0);
}

/**
 *
 */
int IsDigit(char c) {
	return (c >= '0' && c <= '9' ? 1 : 0);
}
