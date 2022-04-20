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
 * Uses the GetOneArgument function to read all of the arguments from stdin that
 * create a booking and allocates the necessary memory to store them. It also
 * gets the parent flight and stores it inside the booking for easy access.
 * Auxiliary function to the 'r' command.
 */
booking* ReadBooking(global_store* global, char* flight_code,
					 clock* date_depart) {
	booking* new_booking = SecureMalloc(sizeof(booking));
	char booking_code[MAX_ARG_LENGTH], pas_num[MAX_ARG_LENGTH];

	GetOneArgument(booking_code, UNTIL_WHITESPACE);
	GetOneArgument(pas_num, UNTIL_WHITESPACE);

	new_booking->parent_flight = GetFlight(global, flight_code, date_depart);
	free(date_depart);

	new_booking->booking_code =
		(char*)SecureMalloc(sizeof(char) * (strlen(booking_code) + 1));
	strcpy(new_booking->booking_code, booking_code);

	new_booking->pas_num = atoi(pas_num);

	return new_booking;
}

/**
 * Adds a new booking to the system with the specified information.
 * Auxiliary function to the 'r' command.
 */
void AddBooking(global_store* global, char* flight_code, clock* date_depart) {
	booking* new_booking = ReadBooking(global, flight_code, date_depart);
	flight* parent_flight = new_booking->parent_flight;
	node_t* booking_node;

	if (CheckAddBookingErrors(global, flight_code, new_booking)) {
		ClearBooking(new_booking);
		return;
	}

	/* updates the number of seats occupied on the flight */
	parent_flight->occupation += new_booking->pas_num;

	/* stores the booking in the parent flights booking list and saves */
	/* the respective node in the bookings hashtable */
	booking_node = list_insert(parent_flight->bookings, new_booking);
	global->bookingsTable = hashtable_insert(global->bookingsTable,
										  	booking_node,
										  	GetBookingKey(booking_node),
											GetBookingKey);
}

/**
 * Receives a the information of a flight and lists it's bookings sorted by
 * their booking code in lexicographic order.
 * Auxiliary function to the 'r' command.
 */
void ListBookings(global_store* global, char* flight_code, clock* date_depart) {
	flight* flight_list;

	if (CheckListBookingsErrors(global, flight_code, date_depart)) {
		free(date_depart);
		return;
	}

	flight_list = GetFlight(global, flight_code, date_depart);

	sort_list(flight_list->bookings, CompareBookings);

	PrintBookings(flight_list->bookings->first);

	free(date_depart);
}

/**
 * Checks if the booking it receives is valid to be added to the system.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckAddBookingErrors(global_store* global, char* flight_code,
						  booking* new_booking) {
	flight* parent_flight = new_booking->parent_flight;
	booking* booking_check = GetBooking(global, new_booking->booking_code);

	if (CheckBookingCodeErrors(new_booking->booking_code)) {
		printf(BOOKING_ERR_INVALID);
	} else if (parent_flight == NULL) {
		printf(BOOKING_ERR_FLIGHT_CODE, flight_code); /* no parent flight exists */
	} else if (booking_check != NULL) {
		printf(BOOKING_ERR_DUPLICATE, new_booking->booking_code);
	} else if (parent_flight->occupation + new_booking->pas_num
				> parent_flight->capacity) {
		printf(BOOKING_ERR_TOO_MANY); /* too many bookings for the flight */
	} else if (CheckDateErrors(global, parent_flight->date_depart)) {
		printf(DATE_ERR_INVALID);
	} else if (new_booking->pas_num <= 0) {
		printf(BOOKING_ERR_PASSENGER); /* the passenger number is an invalid number */
	} else {
		return FALSE;
	}

	return TRUE;
}

/**
 * Checks if the booking code it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckBookingCodeErrors(char* booking_code) {
	int booking_code_len = strlen(booking_code), i;

	if (booking_code_len < MIN_BOOKING_CODE_LENGTH) {
		return TRUE;
	}

	for (i = 0; i < booking_code_len; i++) {
		if (!IsUpperCase(booking_code[i]) && !IsDigit(booking_code[i])) {
			return TRUE;
		}
	}

	return FALSE;
}

/**
 * Checks if the flight information it receives corresponds to an actual flight
 * that exists.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckListBookingsErrors(global_store* global, char* flight_code,
							clock* date_depart) {
	flight* flight_list = GetFlight(global, flight_code, date_depart);

	if (flight_list == NULL) {
		printf(BOOKING_ERR_FLIGHT_CODE, flight_code);
	} else if (CheckDateErrors(global, date_depart)) {
		printf(DATE_ERR_INVALID);
	} else {
		return FALSE;
	}

	return TRUE;
}

/**
 * Retrives the booking from the bookings hashtable with the given key. If it
 * doesn't find it, then it returns NULL.
 */
booking* GetBooking(global_store* global, char* booking_code) {
	node_t* booking_node;
	hash_elem* elem = hashtable_get(global->bookingsTable, booking_code,
								 	booking_code, GetBookingKey);

	if (elem == NULL) {
		return NULL;
	} else {
		booking_node = (node_t*)elem->data;
		return (booking*)booking_node->data;
	}
}

/**
 * Gets the key of a given booking, which corresponds to its booking code.
 */
char* GetBookingKey(void* booking_node) {
	node_t* node = booking_node;
	booking* booking_get = node->data;

	return booking_get->booking_code;
}

/**
 * Clears all of the memory allocated to the booking it receives.
 */
void ClearBooking(void* tmp_booking) {
	booking* booking_delete = tmp_booking;

	free(booking_delete->booking_code);
	free(booking_delete);
}

/**
 * Compares the booking codes of the two bookings it receives. If the two
 * booking codes are identical then it returns 0. Returns 1 if the second  booking
 * code comes after in the lexicographic order,  0 if the bookings are  identical
 * or -1 if the first booking code comes first in the lexicographic order.
 */
int CompareBookings(void* booking_1, void* booking_2) {
	booking* booking_cmp_1 = booking_1;
	booking* booking_cmp_2 = booking_2;

	return strcmp(booking_cmp_1->booking_code, booking_cmp_2->booking_code);
}

/**
 * Receives the head of a bookings list and prints the information of all the
 * bookings in order.
 */
void PrintBookings(node_t* booking_head) {
	node_t* ptr;
	booking* tmp_booking;

	for (ptr = booking_head; ptr != NULL; ptr = ptr->next) {
		tmp_booking = (booking*)ptr->data;
		printf(BOOKING_FULL_PRINT,
		 		tmp_booking->booking_code, tmp_booking->pas_num);
	}
}

/**
 * Receives a list of bookings and removes each booking with the RemoveBooking
 * function.
 */
void RemoveBookings(global_store* global, list_t* bookings) {
	node_t *ptr, *aux;
	booking* booking_delete;

	for (ptr = bookings->first; ptr != NULL; ptr = aux) {
		aux = ptr->next;
		booking_delete = (booking*)ptr->data;
		RemoveBooking(global, booking_delete->booking_code);
	}
}

/**
 * Removes the booking associated with the given key from the system.
 * It removes it from the bookings hashtable and the parent flight list.
 */
void RemoveBooking(global_store* global, char* booking_code) {
	hash_elem* booking_elem = hashtable_get(global->bookingsTable,
							  booking_code, booking_code, GetBookingKey);
	node_t* booking_node = booking_elem->data;
	booking* booking_delete = booking_node->data;
	flight* parent_flight = booking_delete->parent_flight;

	/* updates the number of seats occupied on the flight */
	parent_flight->occupation -= booking_delete->pas_num;

	/* removes and clears the booking from the system */
	hashtable_remove(global->bookingsTable, booking_code, booking_code,
				  	GetBookingKey);
	list_remove(parent_flight->bookings, booking_node);
	ClearBooking(booking_delete);
}
