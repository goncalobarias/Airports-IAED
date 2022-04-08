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
#include "proj1.h"
#include "proj2.h"

/**
 *
 */
void AddBooking_ListBookings() {
	booking new_booking;

	ReadBooking(&new_booking);

	if (CheckAddBookingErrors(new_booking)) {
		return;
	}
}

/**
 *
 */
void DeleteBooking_Flight() {
}

/**
 *
 */
void ReadBooking(booking *booking_1) {
	GetOneArgument(booking_1->flight_code, 0);
	GetOneArgument(booking_1->date, 0);
	GetOneArgument(, 0);
	GetOneArgument(, 0);
	booking_1->booking_code
	booking_1->passengers
}

/**
 *
 */
int CheckAddBookingErrors(booking booking_1) {
	if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else if () {
	} else {
		return 0;
	}

	return 1;
}
