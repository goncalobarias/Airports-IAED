/*
 *		File: proj1.h
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: Header file for the whole project.
 *				Includes constants, prototypes and external variables.
 */

/**
 * Constants
 */

/* airports */
#define ID_LENGTH 4
#define MAX_COUNTRY_LENGTH 31
#define MAX_CITY_LENGTH 51
#define MAX_AIRPORTS 40
#define MAX_FLIGHT_CODE_LENGTH 7

#define AIRPORT_PRINT "%s %s %s %d\n"
#define AIRPORT_ADD_PRINT "airport %s\n"

#define AIRPORT_ERR_TOO_MANY "too many airports\n"
#define AIRPORT_ERR_INVALID "invalid airport ID\n"
#define AIRPORT_ERR_DUPLICATE "duplicate airport\n"
#define AIRPORT_ERR_NO_ID "%s: no such airport ID\n"

/* flights */
#define MAX_CAPACITY_LENGTH 3
#define MAX_FLIGHTS 30000
#define MAX_PASSENGERS 100
#define MIN_PASSENGERS 10

#define FLIGHT_PRINT "%s %s"
#define FLIGHT_FULL_PRINT "%s %s %s"

#define FLIGHT_ERR_INVALID "invalid flight code\n"
#define FLIGHT_ERR_DUPLICATE "flight already exists\n"
#define FLIGHT_ERR_TOO_MANY "too many flights\n"
#define FLIGHT_ERR_INVALID_DURATION "invalid duration\n"
#define FLIGHT_ERR_INVALID_CAPACITY "invalid capacity\n"

/* dates */
#define DATE_LENGTH 11
#define TIME_LENGTH 6
#define MAX_DURATION_MINS 720
#define MONTHS 12
#define MINS_YEAR 525600
#define MINS_DAY 1440
#define MINS_HOUR 60
#define START_DAY "00:00"

#define DATE_PRINT "%02d-%02d-%04d\n"
#define DATE_FULL_PRINT " %02d-%02d-%04d %02d:%02d\n"

#define DATE_ERR_INVALID "invalid date\n"

/* booking */
#define MAX_BOOKING_LENGTH 65535

#define BOOKING_FULL_PRINT "%s %d\n"

#define BOOKING_ERR_INVALID "invalid reservation code\n"
#define BOOKING_ERR_FLIGHT_CODE "%s: flight does not exist\n"
#define BOOKING_ERR_DUPLICATE "%s: flight reservation already used\n"
#define BOOKING_ERR_TOO_MANY "too many reservations\n"
#define BOOKING_ERR_PASSANGER "invalid passenger number\n"
#define BOOKING_FLIGHT_NA "not found\n"

/* structures */
#define MEMORY_ERR "No memory.\n"
#define MEMORY_ERR_CODE -1
#define MAX_LOAD 0.5
#define HASHTABLE_START_SIZE 7
#define HASHTABLE_DELETED -2
#define HASHTABLE_TAKEN -3
#define LIST_SORTED -4
#define LIST_UNSORTED -5

/**
 * Data structures
 */

typedef struct hashtable_t hashtable;

typedef struct hash_elem_t hash_elem;

typedef struct node node_t;

typedef struct list list_t;

typedef struct {
	int day;
	int month;
	int year;
	int hours;
	int minutes;
} clock;

typedef struct {
	char flight_code[MAX_FLIGHT_CODE_LENGTH];
	char departure_id[ID_LENGTH];
	char arrival_id[ID_LENGTH];
	clock date_departure;
	int duration;
	clock date_arrival;
	int capacity;
	list_t* reservations;
} flight;

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
	int departures;
} airport;

typedef struct {
} booking;

/**
 * External variables
 */

extern int totalAirports;
extern int totalFlights;
extern airport allAirports[MAX_AIRPORTS];
extern flight allFlights[MAX_FLIGHTS];
extern int sortedAirports[MAX_AIRPORTS];
extern clock global_date;
extern clock max_date;
extern const int days_months_ac[MONTHS];

/**
 * Prototypes
 */

/* proj1.c */

int handle_commands();

void AddAirport();

void ListAirports();

void AddFlight_ListFlights();

void FlightDeparturesInAirport();

void FlightArrivalsInAirport();

void AdvanceSystemDate();

char GetOneArgument(char *argument, const int mode);

/* airports.c */

void ReadAirport(airport *airport_1);

int CheckAddAirportErrors(const char id[]);

int CheckAirportExistence(const char id[]);

int GetAirport(const char id[]);

void AddSortedAirport(airport airport_1);

void ListAllAirports();

void PrintAirport(airport airport_1);

/* flights.c */

void ReadFlight(flight *new_flight);

int CheckAddFlightErrors(flight new_flight);

int CheckFlightCodeErrors(const char flight_code[]);

int CheckDuplicateFlight(const char flight_code[], clock date);

void AddSortedFlight(int sort[], int size, flight new_flight, int index,
					 const int mode);

void ListAllFlights();

/* dates.c */

int CheckDateErrors(clock date);

clock UpdateDate(clock date_departure, int duration);

int ConvertDatesToMins(clock date);

int CompareDates(clock date_1, clock date_2, int mode);

int CompareFlightDates(flight flight_1, flight flight_2, const int mode);

clock ReadClock(char date[], char time[]);

int ReadDuration(char duration[]);

void PrintClock(clock date);

/* proj2.c */

void AddBooking_ListBookings();

void DeleteBooking_Flight();

void ReadBooking(booking* booking_1);

int CheckAddBookingErrors(booking booking_1);

/* structures.c */

unsigned int calc_hash(char* key);

unsigned int calc_hash_step(char* key);

int* hashtable_calc_hashes(char* key, int size);

hashtable* hashtable_create(int size);

hashtable* hashtable_insert(hashtable* hash_t, void* data, char* key);

void* hashtable_get(hashtable* hash_t, char* key, char*(*get_key)(void*));

void hashtable_remove(hashtable* hash_t, char* key, char*(*get_key)(void*));

hashtable* hashtable_expand(hashtable* hash_t);

void hashtable_destroy(hashtable* hash_t);

list_t* list_create();

void list_insert(list_t* list, void* data);

void list_remove(list_t* list, hashtable* hash_t, void* key, char*(*get_key)(void*), void(*clear_data)(node_t*));

void list_destroy(list_t* list, void(*clear_data)(node_t*));

void* secure_malloc(unsigned int allocation);

int isPrime(int x);

int getPrime(int minNum);
