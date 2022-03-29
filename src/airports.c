/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <string.h>
#include "proj1.h"

/**
 *
 */
int CheckAddAirportErrors(char *id) {
	int i, id_len = strlen(id), j = GetAirport(id);

	for (i = 0; i < id_len; i++)
		if (id[i] < 'A' || id[i] > 'Z') {
			printf(AIRPORT_ERR_INVALID);
			return 1;
		}
	if (totalAirports >= MAX_AIRPORTS) {
		printf(AIRPORT_ERR_TOO_MANY);
		return 1;
	}
	if (strcmp(allAirports[sortedAirports[j]].id, id) == 0)  {
		printf(AIRPORT_ERR_DUPLICATE);
	 	return 1;
	}

	return 0;
}

/**
 *
 */
int CheckAirportExistence(char *id) {
	int i = GetAirport(id);

	if (i == totalAirports ||
		strcmp(allAirports[sortedAirports[i]].id, id) != 0) {
		printf(AIRPORT_ERR_NO_ID, id);
		return 1;
	}

	return 0;
}

/**
 *
 */
int GetAirport(char *id) {
	int left, middle, right, comp;

	if (totalAirports == 0)
		return 0;
	else {
		left = 0;
		right = totalAirports - 1;
		middle = (left + right) / 2;
		while (left < right) {
			comp = strcmp(allAirports[sortedAirports[middle]].id, id);
			if (comp == 0)
				return middle;
			else if (comp < 0)
				left = middle + 1;
			else
				right = middle - 1;
			middle = (left + right) / 2;
		}
		comp = strcmp(allAirports[sortedAirports[left]].id, id);

		return (comp < 0 ? left + 1 : left);
	}
}

/**
 *
 */
void AddSortedAirport(airport airport_1) {
	int index, i;

	index = GetAirport(airport_1.id);

	for (i = totalAirports - 1; i >= index; i--)
		sortedAirports[i + 1] = sortedAirports[i];
	sortedAirports[index] = totalAirports;
}

/**
 *
 */
void PrintAirport(airport airport_1) {
	printf(AIRPORT_PRINT, airport_1.id, airport_1.city, airport_1.country,
 						airport_1.departures);
}
