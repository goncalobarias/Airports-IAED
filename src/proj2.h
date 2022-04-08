/*
 *		File: proj2.h
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: Header file for the second project.
 *				Includes constants, structures and prototypes.
 */

/**
 * Constants
 */

#define MAX_BOOKING_LENGTH 65535
#define MEMORY_ERR "No memory.\n"

#define BOOKING_FULL_PRINT "%s %d\n"

#define BOOKING_ERR_INVALID "invalid reservation code\n"
#define BOOKING_ERR_FLIGHT_CODE "%s: flight does not exist\n"
#define BOOKING_ERR_DUPLICATE "%s: flight reservation already used\n"
#define BOOKING_ERR_TOO_MANY "too many reservations\n"
#define BOOKING_ERR_PASSANGER "invalid passanger number\n"
#define BOOKING_FLIGHT_NA "not found\n"

/**
 * Structures
 */

typedef struct {
} booking;

/**
 * Prototypes
 */

/* proj2.c */

void AddBooking_ListBookings();

void DeleteBooking_Flight();

void ReadBooking(booking *booking_1);

int CheckAddBookingErrors(booking booking_1);

/* structures.c */
