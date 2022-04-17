/*
 *		File: bookings.c
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
	booking* new_booking = SecureMalloc(sizeof(booking));
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
void AddBooking(global_store* global, char* flight_code, clock date) {
	booking* new_booking = ReadBooking(flight_code, date);
	flight* flight_1;

	if (CheckAddBookingErrors(global, new_booking)) {
		ClearBooking(new_booking);
		return;
	}

	flight_1 = GetFlight(global, flight_code, new_booking->date_departure);
	flight_1->occupation += new_booking->res_num;
	global->bookingsTable = hashtable_insert(global->bookingsTable, new_booking,
											GetBookingKey(new_booking));
}

/**
 *
 */
void ListBookings(global_store* global, char* flight_code, clock date) {
	list_t* tmp_list;
	booking* tmp_booking;
	int i;
	node_t* p1;

	if (CheckListBookingsErrors(global, flight_code, date)) {
		return;
	}

	tmp_list = list_create();

	for (i = 0; i < global->bookingsTable->size; i++) {
		if (hash_elem_dead(global->bookingsTable->table[i])) {
			continue;
		}
		tmp_booking = (booking*)global->bookingsTable->table[i]->data;
		if (strcmp(tmp_booking->flight_code, flight_code) == 0
			&& CompareDates(tmp_booking->date_departure, date, 1) == 0) {
			list_insert(tmp_list, tmp_booking);
		}
	}

	p1 = list_mergesort(tmp_list->first, CompareBookings);

	PrintBookings(p1);

	free(tmp_list);
}

/**
 *
 */
int CheckAddBookingErrors(global_store* global, booking* new_booking) {
	flight* flight_1 =
		GetFlight(
			global, new_booking->flight_code, new_booking->date_departure);
	booking* booking_1 =
		GetBooking(global, new_booking->booking_code);

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

	if (booking_code_len < MIN_BOOKING_CODE_LENGTH) {
		return 1;
	}

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
int CheckListBookingsErrors(global_store* global, char* flight_code, clock date) {
	flight* flight_1 = GetFlight(global, flight_code, date);

	if (flight_1 == NULL) {
		printf(BOOKING_ERR_FLIGHT_CODE, flight_code);
	} else if (CheckDateErrors(global, date)) {
		printf(DATE_ERR_INVALID);
	} else {
		return 0;
	}

	return 1;
}

/**
 *
 */
booking* GetBooking(global_store* global, char* booking_code) {
	hash_elem* elem =
		hashtable_get(global->bookingsTable, booking_code, GetBookingKey);

	if (elem == NULL) {
		return NULL;
	}

	return (booking*)elem->data;
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
	free((booking*)booking_delete);
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
void PrintBookings(node_t* booking_head) {
	node_t *p, *aux;
	booking* tmp_booking;

	for (p = booking_head; p != NULL; p = aux) {
		aux = p->next;
		tmp_booking = (booking*)p->data;
		printf(BOOKING_FULL_PRINT,
		 		tmp_booking->booking_code, tmp_booking->res_num);
		free(p);
	}
}

/**
 *
 */
void RemoveBooking(global_store* global, char* booking_code) {
	booking* booking_1 = GetBooking(global, booking_code);
	flight* flight_1 = GetFlight(global, booking_1->flight_code,
							  	booking_1->date_departure);

	flight_1->occupation -= booking_1->res_num;
	hashtable_remove(global->bookingsTable, booking_code, GetBookingKey,
				  	ClearBooking);
}

/**
 *
 */
void RemoveAllBookings(global_store* global) {
	booking* tmp_booking;
	hash_elem* elem;
	int i;

	for (i = 0; i < global->bookingsTable->size; i++) {
		elem = global->bookingsTable->table[i];
		if (elem == NULL) {
			continue;
		} else if (elem->state == HASHTABLE_DELETED) {
			free(elem);
		} else {
			tmp_booking = (booking*)elem->data;
			ClearBooking(tmp_booking);
			free(elem);
		}
	}

	hashtable_destroy(global->bookingsTable);
}
