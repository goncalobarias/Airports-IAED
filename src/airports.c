/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <string.h>
#include "proj1.h"

/**
 * Checks if the airport id that it receives is valid.
 * If so, it returns 0, otherwise it returns 1.
 * Auxiliary function of the 'a' command.
 */
int CheckAddAirportErrors(const char id[]) {
	int i, id_len = strlen(id);

	for (i = 0; i < id_len; i++) {
		if (id[i] < 'A' || id[i] > 'Z') {
			printf(AIRPORT_ERR_INVALID); /* id contains unwanted characters */
			return 1;
		}
	}
	if (totalAirports >= MAX_AIRPORTS) {
		printf(AIRPORT_ERR_TOO_MANY); /* no more space to add new airports*/
		return 1;
	}
	if (!CheckAirportExistence(id)) {
		printf(AIRPORT_ERR_DUPLICATE); /* duplicated airport */
	 	return 1;
	}

	return 0;
}

/**
 * Searches the database and finds out if an airport has already been added
 * to the system.
 * If so, it returns 0, otherwise it returns 1.
 */
int CheckAirportExistence(const char id[]) {
	int i = GetAirport(id);

	/* if the index of the airport is out of bounds or the airport is not on the */
	/* system it returns 1. */
	if (i == totalAirports
		|| strcmp(allAirports[sortedAirports[i]].id, id) != 0) {
		return 1;
	}

	return 0;
}

/**
 * Uses binary search to get the index of an airport from the id it receives.
 * It uses the sortedAirports array to do the searching. It compares the id
 * of the middle position with the id it receives on each iteration. If the
 * airport is found, it returns it's position on the sorted array, otherwise
 * it returns the position where the airport should be inserted.
 */
int GetAirport(const char id[]) {
	int left = 0, right = totalAirports - 1, middle, comp;

	while (left <= right) {
		middle = (left + right) / 2;
		comp = strcmp(allAirports[sortedAirports[middle]].id, id);
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
void AddSortedAirport(airport airport_1) {
	int index, i;

	index = GetAirport(airport_1.id);

	/* inserts the airport in the sorted array */
	for (i = totalAirports - 1; i >= index; i--) {
		sortedAirports[i + 1] = sortedAirports[i];
	}
	sortedAirports[index] = totalAirports;
}

/**
 * Receives an airport and prints it's id, city, country and number of
 * departures.
 * Auxiliary function of the 'l' command.
 */
void PrintAirport(airport airport_1) {
	printf(AIRPORT_PRINT, airport_1.id, airport_1.city, airport_1.country,
 						airport_1.departures);
}
