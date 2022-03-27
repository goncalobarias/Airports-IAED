/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

/* constants */
#define ID_LENGTH 4
#define MAX_COUNTRY_LENGTH 31
#define MAX_CITY_LENGTH 51
#define MAX_AIRPORTS 40
#define MAX_FLIGHT_CODE_LENGTH 7
#define DATE_LENGTH 11
#define TIME_LENGTH 6
#define MAX_CAPACITY_LENGTH 3
#define MAX_FLIGHTS 30000
#define MAX_PASSENGERS 100
#define MIN_PASSENGERS 10
#define MAX_DURATION 720
#define MONTHS 12
#define MINS_YEAR 525600
#define MINS_DAY 1440
#define START_DAY "00:00"
#define NO_DATE "00-00-0000"

/* error messages */
#define AIRPORT_ERR_TOO_MANY "too many airports\n"
#define AIRPORT_ERR_INVALID "invalid airport ID\n"
#define AIRPORT_ERR_DUPLICATE "duplicate airport\n"
#define AIRPORT_ERR_NO_ID "%s: no such airport ID\n"
#define FLIGHT_ERR_INVALID "invalid flight code\n"
#define FLIGHT_ERR_DUPLICATE "flight already exists\n"
#define FLIGHT_ERR_TOO_MANY "too many flights\n"
#define FLIGHT_ERR_INVALID_DATE "invalid date\n"
#define FLIGHT_ERR_INVALID_DURATION "invalid duration\n"
#define FLIGHT_ERR_INVALID_CAPACITY "invalid capacity\n"

/**
 * Structs
 */

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
	clock duration;
	int capacity;
} flight;

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
	int departures;
} airport;

/**
 * Shared variables
 */

extern int totalAirports;
extern int totalFlights;
extern airport allAirports[MAX_AIRPORTS];
extern flight allFlights[MAX_FLIGHTS];
extern char sortedIDs[MAX_AIRPORTS][ID_LENGTH];
extern int sortedFlights[MAX_FLIGHTS];
extern clock global_date;
extern clock max_date;
extern clock one_year;

/**
 * Prototypes
 */

/* proj1.c */

void AddAirport();

void ListAirports();

void AddFlight_ListFlights();

void FlightDeparturesInAirport();

void FlightArrivalsInAirport();

void AdvanceSystemDate();

char GetOneArgument(char *argument, const int mode);

/* airports.c */

int CheckAddAirportErrors(char *id);

void ListAllAirports();

void AddSortedAirportID(char *id);

int GetAirportFromID(char *id);

/* flights.c */

int CheckFlightCodeErrors(char *flight_code, char *date, char *time);

int CheckTooManyFlights();

int CheckDateErrors(char *date, char *time);

int CheckDurationErrors(char *duration);

int CheckCapacityErrors(char *capacity);

clock UpdateDate(clock date_departure, clock duration);

int ConvertDatesToMins(clock date);

int CompareDates(clock date_1, clock date_2, int mode);

clock ReadClock(char *date, char *time);

void AddSortedFlight(flight new_flight);

void ListAllFlights();
