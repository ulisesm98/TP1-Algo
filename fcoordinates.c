/* Supongamos que latitude y longitude son campos de una estructura statement_t */

#define NMEA_LATITUDE_DEGREES 2
#define NMEA_LATITUDE_MINUTES 5
#define NMEA_LONGITUDE_DEGREES 3
#define NMEA_LONGITUDE_MINUTES 5
#define CONVERSION_FACTOR_MINUTES 60

status_t latitude(const char *statement, char **pos_ptr, stantement_t *lat) {
	
	size_t degrees;
	float minutes;
	char *aux_ptr;
	char aux[MAX_STR];
	size_t i;

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < NMEA_LATITUDE_DEGREES; i++, *pos_ptr++) 
		aux[i] = **pos_ptr;

	degrees = strtoul(aux, &aux_ptr, 10);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;

	for(i = 0; i < NMEA_LATITUDE_MINUTES + 1; i++, *pos_ptr++) // sumo uno por el punto
		aux[i] = **pos_ptr;

	/* Reutilizo cadena porque considero que la sentencia viene según indica el código NMEA. Sino tendría que considerar el caso en el que 
	CARECTERES_LATITUD_GRADOS sea mayor que CARACTERES_LATITUD_MINUTOS */

	minutes = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;

	lat -> latitude = degrees + minutes/CONVERSION_FACTOR_MINUTES; //falta multiplicar por -1 (o no) según si sea S o N

	return ST_OK;

}

status_t longitude(const char *statement, char **pos_ptr, stantement_t *lon) {
	
	size_t degrees;
	float minutes;
	char *aux_ptr;
	char aux[MAX_STR];
	size_t i;

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < NMEA_LONGITUDE_DEGREES; i++, *pos_ptr++) 
		aux[i] = **pos_ptr;

	degrees = strtoul(aux, &aux_ptr, 10);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;

	for(i = 0; i < NMEA_LONGITUDE_MINUTES + 1; i++, *pos_ptr++) // sumo uno por el punto
		aux[i] = **pos_ptr;

	/* Reutilizo cadena porque considero que la sentencia viene según indica el código NMEA. Sino tendría que considerar el caso en el que 
	CARECTERES_LATITUD_GRADOS sea mayor que CARACTERES_LATITUD_MINUTOS */

	minutes = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;

	lon -> longitude = degrees + minutes/CONVERSION_FACTOR_MINUTES; //falta multiplicar por -1 (o no) según si sea S o N

	return ST_OK;

}