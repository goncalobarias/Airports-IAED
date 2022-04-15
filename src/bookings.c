/*
 *		File: dates.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All the functions used to manipulate bookings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "structures.h"

/**
 *
 */
booking* ReadBooking(char* flight_code, clock date) {
	booking* new_booking = (booking*)SecureMalloc(sizeof(booking));
	char booking_code[MAX_ARG_LENGTH], passangers_nums[MAX_ARG_LENGTH];

	GetOneArgument(booking_code, 0);
	GetOneArgument(passangers_nums, 0);


	new_booking->flight_code =
		(char*)SecureMalloc(sizeof(char) * (strlen(flight_code) + 1));
	strcpy(new_booking->flight_code, flight_code);

	new_booking->date_departure = date;

	new_booking->booking_code =
		(char*)SecureMalloc(sizeof(char) * (strlen(booking_code) + 1));
	strcpy(new_booking->booking_code, booking_code);

	new_booking->res_num = atoi(passangers_nums);

	return new_booking;
}

/**
 *
 */
int CheckAddBookingErrors(global_store* global, booking* new_booking) {
	flight* flight_1 =
		GetFlight(
			global, new_booking->flight_code, new_booking->date_departure);
	booking* booking_1 =
		hashtable_get(
			global->bookingsTable, new_booking->booking_code, GetBookingKey);

	if (CheckBookingCodeErrors(new_booking->booking_code)) {
		printf(BOOKING_ERR_INVALID);
	} else if (flight_1 == NULL) {
		printf(BOOKING_ERR_FLIGHT_CODE, new_booking->flight_code);
	} else if (booking_1 != NULL) {
		printf(BOOKING_ERR_DUPLICATE, new_booking->booking_code);
	} else if (flight_1->occupation+new_booking->res_num > flight_1->capacity) {
		printf(BOOKING_ERR_TOO_MANY);
	} else if (CheckDateErrors(global, new_booking->date_departure)) {
		printf(DATE_ERR_INVALID);
	} else if (new_booking->res_num <= 0) {
		printf(BOOKING_ERR_PASSENGER);
	} else {
		return 0;
	}

	return 1;
}

/**
 *
 */
int CheckBookingCodeErrors(char* booking_code) {
	int booking_code_len = strlen(booking_code), i;

	for (i = 0; i < booking_code_len; i++) {
		if (!IsUpperCase(booking_code[i]) && !IsDigit(booking_code[i])) {
			return 1;
		}
	}

	return 0;
}

/**
 *
 */
void ListBookings(global_store* global, char* flight_code, clock date) {
	list_t* tmp_list = list_create();
	booking* tmp_booking;
	int i;
	node_t *p1, *p2;

	for (i = 0; i < global->bookingsTable->size; i++) {
		if (global->bookingsTable->table[i] == NULL
			|| global->bookingsTable->table[i]->state == HASHTABLE_DELETED) {
			continue;
		}
		tmp_booking = (booking*)global->bookingsTable->table[i]->data;
		if (strcmp(tmp_booking->flight_code, flight_code) == 0
			&& CompareDates(tmp_booking->date_departure, date, 1) == 0) {
			list_insert(tmp_list, tmp_booking);
		}
	}

	p1 = list_mergesort(tmp_list->first, CompareBookings);

	for (; p1 != NULL; p1 = p2) {
		p2 = p1->next;
		tmp_booking = (booking*)p1->data;
		printf(BOOKING_FULL_PRINT, tmp_booking->booking_code, tmp_booking->res_num);
		free(p1);
	}

	free(tmp_list);
}

/**
 *
 */
int CompareBookings(void* booking_1, void* booking_2) {
	return (strcmp(((booking*)booking_1)->booking_code,
			((booking*)booking_2)->booking_code));
}

/**
 *
 */
char* GetBookingKey(void* booking_1) {
	return ((booking*)booking_1)->booking_code;
}

/**
 *
 */
void ClearBooking(void* booking_delete) {
	free(((booking*)booking_delete)->flight_code);
	free(((booking*)booking_delete)->booking_code);
}
