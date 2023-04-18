/*
 *		File: airports.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All functions used to create and modify airports.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

/**
 * Uses the GetOneArgument function to read all of the arguments from stdin that
 * create an airport.
 * Auxiliary function to the 'a' command.
 */
void ReadAirport(airport *new_airport) {
    GetOneArgument(new_airport->id, UNTIL_WHITESPACE);
    GetOneArgument(new_airport->country, UNTIL_WHITESPACE);
    GetOneArgument(new_airport->city, UNTIL_EOL);
    new_airport->departures = 0;
}

/**
 * Checks if the airport id that it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 * Auxiliary function of the 'a' command.
 */
int CheckAddAirportErrors(global_store *global, const char id[]) {
    int i, id_len = strlen(id);

    for (i = 0; i < id_len; i++) {
        if (!IsUpperCase(id[i])) {
            printf(AIRPORT_ERR_INVALID); /* id contains unwanted characters */
            return TRUE;
        }
    }
    if (global->totalAirports >= MAX_AIRPORTS) {
        printf(AIRPORT_ERR_TOO_MANY); /* no more space to add new airports*/
    } else if (!CheckAirportExistence(global, id)) {
        printf(AIRPORT_ERR_DUPLICATE); /* duplicated airport */
    } else {
        return FALSE;
    }

    return TRUE;
}

/**
 * Searches the database and finds out if an airport has already been added
 * to the system.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckAirportExistence(global_store *global, const char id[]) {
    int i = GetAirport(global, id);

    /* if the index of the airport is out of bounds or the airport is not in the
     */
    /* system it returns 1. */
    if (i == global->totalAirports ||
        strcmp(global->allAirports[global->sortedAirports[i]].id, id) != 0) {
        return TRUE;
    }

    return FALSE;
}

/**
 * Uses binary search to get the index of an airport from the id it receives.
 * It uses the sortedAirports array to do the searching. If the airport is
 * found, it returns it's position on the sorted array, otherwise it returns the
 * position where the airport should be inserted.
 */
int GetAirport(global_store *global, const char id[]) {
    int left = 0, right = global->totalAirports - 1, middle, comp;

    while (left <= right) {
        middle = (left + right) / 2;
        comp =
            strcmp(global->allAirports[global->sortedAirports[middle]].id, id);
        if (comp == 0) {
            return middle; /* it finds the airport */
        } else if (comp < 0) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    return left;
}

/**
 * Adds a new airport to an array sorted by airport ids. It uses the
 * GetAirport function in order to find the place to insert the new airport.
 */
void AddSortedAirport(global_store *global, airport airport_add) {
    int index, i;

    index = GetAirport(global, airport_add.id);

    /* inserts the airport in the sorted array */
    for (i = global->totalAirports - 1; i >= index; i--) {
        global->sortedAirports[i + 1] = global->sortedAirports[i];
    }
    global->sortedAirports[index] = global->totalAirports;
}

/**
 * Lists all of the airports sorted by their id in alphabetical order.
 */
void ListAllAirports(global_store *global) {
    int i;

    for (i = 0; i < global->totalAirports; i++) {
        PrintAirport(global->allAirports[global->sortedAirports[i]]);
    }
}

/**
 * Receives an airport and prints it's id, city, country and number of
 * departures.
 * Auxiliary function of the 'l' command.
 */
void PrintAirport(airport airport_print) {
    printf(AIRPORT_PRINT, airport_print.id, airport_print.city,
           airport_print.country, airport_print.departures);
}
