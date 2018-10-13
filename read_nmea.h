#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define CHAR_INIT_NMEA '$'
#define CHAR_END_NMEA '*'
#define MAX_STR_NMEA 70
#define STR_HEX "0123456789ABCDEF"
#define HEXSTRING_NULL_FIND_INT -1

typedef enum {ST_DAT_ERR, ST_OK, ST_HELP, ST_EOF} status_t;
typedef enum {INVALID_FIX, GPS_FIX, DGPS_FIX, PPS_FIX, RTK_FIX, FLOAT_RTK_FIX, ESTIMATED_FIX, MANUAL_FIX, SIMULATION_FIX} fix_quality;

typedef struct trackpt {
	struct tm trackpt_time;
	double latitud;
	double longitud;
	fix_quality quality;
	size_t n_sat;
	double hdop;
	double elevation;
	double height_of_geoid;
} trackpt_t;


status_t read_nmea(char (*statement)[]);
int hexstring_2_integer(int d1, int d2);