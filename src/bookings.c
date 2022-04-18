/*
 *		File: bookings.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All the functions used to manipulate bookings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/**
 *
 */
booking* ReadBooking(global_store* global, char* flight_code, clock date) {
	booking* new_booking = SecureMalloc(sizeof(booking));
	char booking_code[MAX_ARG_LENGTH], passangers_nums[MAX_ARG_LENGTH];

	GetOneArgument(booking_code, 0);
	GetOneArgument(passangers_nums, 0);

	new_booking->parent_flight = GetFlight(global, flight_code, date);

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
	booking* new_booking = ReadBooking(global, flight_code, date);
	node_t* booking_node;

	if (CheckAddBookingErrors(global, flight_code, new_booking)) {
		ClearBooking(new_booking);
		return;
	}

	new_booking->parent_flight->occupation += new_booking->res_num;
	booking_node = list_insert(new_booking->parent_flight->reservations,
								new_booking);
	global->bookingsTable = hashtable_insert(global->bookingsTable,
										  	booking_node,
											GetBookingKey(booking_node),
											GetBookingKey);
}

/**
 *
 */
void ListBookings(global_store* global, char* flight_code, clock date) {
	flight* flight_list;

	if (CheckListBookingsErrors(global, flight_code, date)) {
		return;
	}

	flight_list = GetFlight(global, flight_code, date);

	sort_list(flight_list->reservations, CompareBookings);

	PrintBookings(flight_list->reservations->first);
}

/**
 *
 */
int CheckAddBookingErrors(global_store* global, char* flight_code,
						  booking* new_booking) {
	flight* parent_flight = new_booking->parent_flight;
	booking* booking_1 =
		GetBooking(global, new_booking->booking_code);

	if (CheckBookingCodeErrors(new_booking->booking_code)) {
		printf(BOOKING_ERR_INVALID);
	} else if (parent_flight == NULL) {
		printf(BOOKING_ERR_FLIGHT_CODE, flight_code);
	} else if (booking_1 != NULL) {
		printf(BOOKING_ERR_DUPLICATE, new_booking->booking_code);
	} else if (parent_flight->occupation + new_booking->res_num
				> parent_flight->capacity) {
		printf(BOOKING_ERR_TOO_MANY);
	} else if (CheckDateErrors(global,
							parent_flight->date_departure)) {
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
	node_t* booking_node;
	hash_elem* elem =
		hashtable_get(global->bookingsTable, booking_code, GetBookingKey);

	if (elem == NULL) {
		return NULL;
	} else {
		booking_node = (node_t*)elem->data;
		return (booking*)booking_node->data;
	}
}

/**
 *
 */
char* GetBookingKey(void* booking_node) {
	booking* booking_get = ((node_t*)booking_node)->data;

	return booking_get->booking_code;
}

/**
 *
 */
void ClearBooking(void* tmp_booking) {
	booking* booking_delete = tmp_booking;

	free(booking_delete->booking_code);
	free(booking_delete);
}

/**
 *
 */
int CompareBookings(void* booking_1, void* booking_2) {
	booking* booking_cmp_1 = booking_1;
	booking* booking_cmp_2 = booking_2;

	return strcmp(booking_cmp_1->booking_code, booking_cmp_2->booking_code);
}

/**
 *
 */
void PrintBookings(node_t* booking_head) {
	node_t* p;
	booking* tmp_booking;

	for (p = booking_head; p != NULL; p = p->next) {
		tmp_booking = (booking*)p->data;
		printf(BOOKING_FULL_PRINT,
		 		tmp_booking->booking_code, tmp_booking->res_num);
	}
}

/**
 *
 */
void RemoveBooking(global_store* global, char* booking_code) {
	hash_elem* booking_elem = hashtable_get(global->bookingsTable,
							  booking_code, GetBookingKey);
	node_t* booking_node = booking_elem->data;
	booking* booking_delete = booking_node->data;

	booking_delete->parent_flight->occupation -= booking_delete->res_num;

	hashtable_remove(global->bookingsTable, booking_code, GetBookingKey);
	list_remove(booking_delete->parent_flight->reservations, booking_node);
	ClearBooking(booking_delete);
}

/**
 *
 */
void RemoveBookings(global_store* global, list_t* reservations) {
	node_t *p, *aux;
	booking* booking_delete;

	for (p = reservations->first; p != NULL; p = aux) {
		aux = p->next;
		booking_delete = (booking*)p->data;
		RemoveBooking(global, booking_delete->booking_code);
	}
}
