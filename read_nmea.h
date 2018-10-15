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


/* macros de tiempo de fix */
#define HOURS_DIGITS 2
#define MINUTES_DIGITS 2
#define SECONDS_DIGITS 5

/* macros de latitud y longitud */
#define NMEA_LATITUDE_DEGREES 2
#define NMEA_LATITUDE_MINUTES 5
#define NMEA_LONGITUDE_DEGREES 3
#define NMEA_LONGITUDE_MINUTES 5
#define CONVERSION_FACTOR_MINUTES 60
#define SOUTH_CHAR 'S'
#define WEST_CHAR 'W'

/* macros de calidad de fix */

#define INVALID_FIX_CHAR '0'
#define GPX_FIX_CHAR '1'
#define DGPS_FIX_CHAR '2'
#define PPS_FIX_CHAR '3'
#define RTK_FIX_CHAR '4'
#define FLOAT_RTK_FIX_CHAR '5'
#define ESTIMATED_FIX_CHAR '6'
#define MANUAL_FIX_CHAR '7'
#define SIMULATION_FIX_CHAR '8'

/* macros de cantidad de satelites */
#define MAX_SAT 12
#define MIN_SAT 0

/* el resto */

#define HDOP_DIGITS 2
#define ELEVATION_DIGITS 2
#define UNDULATION_OF_GEOID_DIGITS 2

typedef enum {ST_DAT_ERR, ST_OK, ST_HELP, ST_EOF, ST_INVALID_NUMBER_ERROR, ST_NUMERICAL_ERROR} status_t;
typedef enum {INVALID_FIX, GPS_FIX, DGPS_FIX, PPS_FIX, RTK_FIX, FLOAT_RTK_FIX, ESTIMATED_FIX, MANUAL_FIX, SIMULATION_FIX} fix_quality_t;

/* La diferencia entre ST_NUMERICAL_ERROR y ST_INVALID_NUMBER_ERROR es que en el primer caso, por ej, se da un float en vez de un int (tipo invalido) 
En el segundo caso tiene que ver con que no respeta un rango pedido (ej: numero de satelites de 0 a 12) */

typedef struct trackpt {
	struct tm trackpt_time;
	double latitude;
	double longitude;
	fix_quality_t quality;
	size_t n_sat;
	double hdop;
	double elevation;
	double undulation_of_geoid;
} trackpt_t;


status_t read_nmea(char (*statement)[]);
int hexstring_2_integer(int d1, int d2);
status_t latitude(const char *statement, char **pos_ptr, statement_t *lat);
status_t longitude(const char *statement, char **pos_ptr, stantement_t *lon);
status_t time_of_fix(const char *statement, char **pos_ptr, struct tm *trackpt_time);
status_t quality_of_fix(const char *statement, char **pos_ptr, trackpt_t *qual);
status_t num_of_satellites(const char *statement, char **pos_ptr, trackpt_t *satellites);
status_t hdop(const char *statement, char **pos_ptr, trackpt_t *hd);
status_t elevation(const char *statement, char **pos_ptr, trackpt_t *elev);
status_t undulation_of_geoid(const char *statement, char **pos_ptr, trackpt_t *und_of_geoid);
