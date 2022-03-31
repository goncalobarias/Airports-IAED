/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

/**
 * Defines
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
#define MAX_DURATION 720
#define MONTHS 12
#define MINS_YEAR 525600
#define MINS_DAY 1440
#define MINS_HOUR 60
#define START_DAY "00:00"
#define NO_DATE "00-00-0000"

#define DATE_PRINT "%02d-%02d-%04d\n"
#define DATE_FULL_PRINT " %02d-%02d-%04d %02d:%02d\n"

#define DATE_ERR_INVALID "invalid date\n"

/**
 * Structs
 */

/* used for dates and durations of time */
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
	clock date_arrival;
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
extern int sortedAirports[MAX_AIRPORTS];
extern int sortedFlights_departure[MAX_FLIGHTS];
extern int sortedFlights_arrival[MAX_FLIGHTS];
extern clock global_date;
extern clock max_date;
extern const int days_months[MONTHS];

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

int CheckAddAirportErrors(const char id[]);

int CheckAirportExistence(const char id[]);

int GetAirport(const char id[]);

void AddSortedAirport(airport airport_1);

void PrintAirport(airport airport_1);

/* flights.c */

int CheckFlightCodeErrors(const char flight_code[], clock date);

int CheckAddFlightErrors(flight new_flight);

void AddSortedFlight(int sort[], flight new_flight, const int mode);

void ListAllFlights();

/* dates.c */

int CheckDateErrors(clock date);

clock UpdateDate(clock date_departure, clock duration);

int ConvertDatesToMins(clock date);

int CompareDates(clock date_1, clock date_2, int mode);

int CompareFlightDates(flight flight_1, flight flight_2, const int mode);

clock ReadClock(char date[], char time[]);

void PrintClock(clock date);
