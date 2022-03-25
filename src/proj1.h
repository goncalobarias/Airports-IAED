/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

/* constants */
#define ID_LENGTH 4
#define MAX_COUNTRY_LENGTH 30
#define MAX_CITY_LENGTH 50
#define MAX_AIRPORTS 40
#define MAX_FLIGHT_CODE_LENGTH 6
#define DATE_LENGTH 10
#define TIME_LENGTH 5
#define MAX_FLIGHTS 30000
#define MAX_COMMAND_LENGTH 10
#define MAX_PASSENGERS 100
#define MIN_PASSENGERS 10
#define MAX_DURATION 12

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
	char flight_code[MAX_FLIGHT_CODE_LENGTH];
	char departure_id[ID_LENGTH];
	char arrival_id[ID_LENGTH];
	char date[DATE_LENGTH];
	char time[TIME_LENGTH];
	int duration;
	int capacity;
} flight;

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
	int departures;
} airport;

/**
 * Global variables
 */

extern int totalAirports;
extern int totalFlights;
extern airport allAirports[MAX_AIRPORTS];
extern flight allFlights[MAX_FLIGHTS];
extern char sortedIDs[MAX_AIRPORTS][ID_LENGTH];
extern char time[TIME_LENGTH];

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

int CheckFlightCodeErrors(char *flight_code);

int CheckTooManyFlights();

int CheckDateErrors(char *date);

int CheckDurationErrors(char *duration);

int CheckCapacityErrors(char *capacity);

int ConvertDateTimeToMinutes(char *date, char *time);

void ConvertMinutesToDateTime(char *date, char *time, int minutes);

void AddSortedFlight(char *id, int mode);

void ListAllFlights();
