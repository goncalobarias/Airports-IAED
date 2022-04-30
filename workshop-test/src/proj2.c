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

/**
 * Handles the 'r' command.
 * Adds a new booking to the system with the specified information. If the
 * command only receives two arguments then it will list the bookings from
 * the flight with information it receives.
 */
void AddBooking_ListBookings(global_store* global) {
	char flight_code[MAX_FLIGHT_CODE_LENGTH], calendar_date[DATE_LENGTH];
	int state;
	clock* date;

	GetOneArgument(flight_code, UNTIL_WHITESPACE);
	state = GetOneArgument(calendar_date, UNTIL_WHITESPACE);
	date = ReadClock(calendar_date, START_DAY);

	if (state == 1) {
		ListBookings(global, flight_code, date);
		return;
	} else {
		AddBooking(global, flight_code, date);
		return;
	}
}

/**
 * Handles the 'e' command.
 * Checks if the code it receives is a flight code or a booking code and
 * removes the respective flights or booking associated with that code.
 */
void DeleteBooking_Flight(global_store* global) {
	char code[MAX_ARG_LENGTH];

	GetOneArgument(code, UNTIL_WHITESPACE);

	if (strlen(code) >= MIN_BOOKING_CODE_LENGTH) {
		if (GetBooking(global, code) == NULL) {
			printf(BOOKING_FLIGHT_NA);
			return;
		}
		RemoveBooking(global, code);
	} else {
		if (CheckFlightCodeExistence(global, code) == 0) {
			printf(BOOKING_FLIGHT_NA);
			return;
		}
		RemoveFlights(global, code);
	}
}

/**
 * Clears all of the memory still allocated before the program finishes.
 */
void ExitProgram(global_store* global) {
	node_t *ptr, *aux;

	for (ptr = global->allFlights->first; ptr != NULL; ptr = aux) {
		aux = ptr->next;
		RemoveFlight(global, ptr);
	}
	free(global->allFlights);
	hashtable_destroy(global->flightsTable);
	hashtable_destroy(global->bookingsTable);
	free(global->date);
	free(global);
}

/*		Utilities		*/

/**
 * A proxy function for malloc. Whenever it allocates memory using malloc, it
 * checks if there is spare memory. If the memory has gone out, it will print
 * an error to stdout and exit the program with the respective error code.
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
 * Checks if the number x is prime.
 */
int IsPrime(int x) {
	int i;

	for (i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * Receives a number and returns the smallest prime number bigger than it.
 */
int GetPrime(int minNum) {
	int prime = minNum;

	while (!IsPrime(prime)) {
		prime++;
	}

	return prime;
}

/**
 * Checks if the character c is an upper case letter.
 */
int IsUpperCase(char c) {
	return (c >= 'A' && c <= 'Z' ? TRUE : FALSE);
}

/**
 * Checks if the character c is an lower case letter.
 */
int IsLowerCase(char c) {
	return (c >= 'a' && c <= 'z' ? TRUE : FALSE);
}

/**
 * Checks if the character c is a digit.
 */
int IsDigit(char c) {
	return (c >= '0' && c <= '9' ? TRUE : FALSE);
}
