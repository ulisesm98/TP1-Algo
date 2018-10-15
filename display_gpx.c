#include "read_nmea.h"
#include "command_arg.h"

void print_header(void) {
	
	printf("%s\n", XML_HEADER);
	printf("%s\n", GPX_VERSION);

}


void print_closing_tags(void) {
	
	printf("\t\t%s\n", CLOSE_TRKSEG);
	printf("\t%s\n", CLOSE_TRK);
	printf("%s", CLOSE_GPX);

}

/* Está bien esta notación de flechitas? */

void display_metadata(void) {
	
	printf("\t%s\n", OPEN_METADATA);
	printf("\t\t%s%s%s\n", OPEN_NAME, metadata_t -> name, CLOSE_NAME);
	printf("\t\t%s%d-%d-%d ", OPEN_TIME, metadata_t -> meta_time -> tm_year, metadata_t -> meta_time -> tm_month, metadata_t -> meta_time -> tm_mday);
	printf("%c%d:%d:%d", OPEN_TIME_CHAR, metadata_t -> meta_time -> tm_hour, metadata_t -> meta_time -> tm_min, metadata_t -> meta_time -> tm_sec);
	printf("%c%s\n", CLOSE_TIME_CHAR, CLOSE_TIME);
	printf("\t%s\n", CLOSE_METADATA);

}

void print_opening_tags(void) {

	printf("\t%s\n", OPEN_TRK);	
	printf("\t\t%s\n", OPEN_TRKSEG);

}

void display_track_point(void) {

	printf("\t\t\t%s%s%f%s ", TRACKPT_LAT, INVERTED_COMMAS, trackpt_t -> latitude, INVERTED_COMMAS);
	printf("%s%s%f%s>\n", TRACKPT_LON, INVERTED_COMMAS, trackpt_t -> longitude, INVERTED_COMMAS);
	printf("\t\t\t\t%s%s%s\n", OPEN_ELEVATION, trackpt_t -> elevation, CLOSE_ELEVATION);
	printf("\t\t\t\t%s%d-%d-%d ", OPEN_TIME, trackpt_t -> trackpt_time -> tm_year, trackpt_t -> trackpt_time -> tm_month, trackpt_t -> trackpt_time -> tm_mday);
	printf("%c%d:%d:%d", OPEN_TIME_CHAR, trackpt_t -> trackpt_time -> tm_hour, trackpt_t -> trackpt_time -> tm_min, trackpt_t -> trackpt_time -> tm_sec);
	printf("%c%s\n", CLOSE_TIME_CHAR, CLOSE_TIME);
	printf("\t\t\t%s\n", CLOSE_TRACKPT);

}