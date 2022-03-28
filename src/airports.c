/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

#include <stdio.h>
#include <string.h>
#include "proj1.h"

/**
 *
 */
int GetAirportFromID(char *id) {
	int i;

	for (i = 0; i < totalAirports; i++)
		if (strcmp(allAirports[i].id, id) == 0)
			return i;

	return -1;
}

/**
 *
 */
int CheckAddAirportErrors(char *id) {
	int i, id_len = strlen(id);

	for (i = 0; i < id_len; i++)
		if (id[i] < 'A' || id[i] > 'Z') {
			printf(AIRPORT_ERR_INVALID);
			return 1;
		}
	if (totalAirports >= MAX_AIRPORTS) {
		printf(AIRPORT_ERR_TOO_MANY);
		return 1;
	}
	if (GetAirportFromID(id) != -1)  {
		printf(AIRPORT_ERR_DUPLICATE);
	 	return 1;
	}

	return 0;
}

/**
 *
 */
void AddSortedAirportID(char *id) {
	int first, middle, last, index, i;

	if (totalAirports == 0)
		index = 0;
	else {
		first = 0;
		last = totalAirports - 1;
		middle = (first + last) / 2;
		while (first < last) {
			if (strcmp(sortedIDs[middle], id) < 0)
				first = middle + 1;
			else
				last = middle - 1;
			middle = (first + last) / 2;
		}

		index = first;
		if (strcmp(sortedIDs[first], id) < 0)
			index += 1;
	}

	for (i = totalAirports - 1; i >= index; i--)
		strcpy(sortedIDs[i + 1], sortedIDs[i]);
	strcpy(sortedIDs[index], id);
}

/**
 *
 */
void ListAllAirports() {
	int i, j;

	for (i = 0; i < totalAirports; i++) {
		j = GetAirportFromID(sortedIDs[i]);

		PrintAirport(allAirports[j]);
	}
}
