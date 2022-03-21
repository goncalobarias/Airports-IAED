/* 103124 - Gonçalo Sampaio Bárias - goncalo.barias@tecnico.ulisboa.pt */

typedef struct {
	char id[ID_LENGTH];
	char country[MAX_COUNTRY_LENGTH];
	char city[MAX_CITY_LENGTH];
	int num_flight_departures;
} airport;

typedef struct {
	char flight_code[MAX_CODE_LENGTH];
	airport departure;
	airport arrival;
	char date[DATE_LENGTH];
	char time[TIME_LENGTH];
	int duration;
	int max_passengers;
} flight;

void AddAirport();

void ListAirports(char lastchar);

void AddFlight();

void FlightDeparturesInAirport();

void FlightArrivalsInAirport();

void AdvanceSystemDate();

char GetOneArgument(char *argument, const int mode);

int CheckAddAirportErrors(char *id);

void ListAllAirports();

void AddSortedAirportID(char *id);

int GetAirportFromID(char *id);

int CheckAirportValidity(char *id);
