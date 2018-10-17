
#include "read_nmea.h"

status_t read_nmea(char (*statement)[]) { //recibe la dirección de una cadena, en la cual carga la sentencia

	int c, i, cksm1, cksm2;
	checksum = 0;

	while((c = getchar()) != CHAR_INIT_NMEA && c != EOF);
	for(i = 0; (c = getchar()) != CHAR_END_NMEA && c != '\n' && c != EOF && c != '\0' && c != CHAR_INIT_NMEA && i < MAX_STR_NMEA; i++) {
		(*statement)[i] = c;
		checksum ^= c;
	}
	//cuando termina la información util del statement coloca '\0'
	(*statement)[i] = '\0';

	if(c != CHAR_END_NMEA) {
		if(c == EOF)
			return ST_EOF;
		if(c == CHAR_INIT_NMEA) { // si sale del for porque c = $, pudo haber tomado tal simbolo de la siguiente sentencia, se devuelve y sale.
			ungetc(CHAR_INIT_NMEA, stdin); // se puede? es feo pero bueno no queda otra.
			return ST_DATA_ERR;
		}

		return ST_DATA_ERR; //HOLA VAGO ################################################## HOLA PATO ME OLVIDE DE BORRAR
	}

	//llega al '*' lee los dos caracteres del checksum y si son "validos" los pasa a int con hexstring_2_integer
	if((cksm1 = getchar()) == EOF || cksm1 == '\0' || (cksm2 = getchar()) == EOF || cksm2 == '\0')
		return ST_DATA_ERR;
	if(hexstring_2_integer((char)cksm1,(char)cksm2) != checksum)
		return ST_DATA_ERR;

	return ST_OK;
}



int hexstring_2_integer(int d1, int d2) {

	size_t hex1, hex2;
	char hexa_dic[] = {STR_HEX}, *ptr;

	//busca el primer caracter en el dicc, si no lo encuentra --> error
	if((ptr = strrchr(hexa_dic, d1)) == NULL)
		return HEXSTRING_NULL_FIND_INT;
	//calcula la posicion del caracter respecto al comienzo de la cadena
	hex1 = (size_t)ptr - (size_t)hexa_dic; //consultar
	
	if((ptr = strrchr(hexa_dic, d2)) == NULL)
		return HEXSTRING_NULL_FIND_INT;
	hex2 = (size_t)ptr - (size_t)hexa_dic; //consultar
	
	if(hex1 >= HEX_DIGITS || hex2 >= HEX_DIGITS) // HC?? PARA MI SI, CAMBIAR (Kevin) // Asi esta bien? E.
		return HEXSTRING_NULL_FIND_INT;
	return (int)hex1 * HEX_DIGITS + hex2; // HC ?? PARA MI SI, CAMBIAR (Kevin) // Asi esta bien? E.

} // si está mal devuelve -1

status_t time_of_fix(const char *statement, char **pos_ptr, struct tm *trackpt_time) { // pos_ptr ya viene apuntado a *statement? para que usa *statement la func?
	
	char aux[MAX_STR]; // con 4 digitos alcanza E.
	char *aux_ptr;
	size_t hours, minutes, i;
	float seconds;


	for(i = 0; i < HOURS_DIGITS; i++, *pos_ptr++) //se podria poner el incremento del puntero en la sig. declaración aux[i] = *((*pos_ptr)++) E.
		aux[i] = **pos_ptr; // al final del ciclo cargar un '\0' en aux[i], de este modo cortas la cadena donde no hay mas info util. E.
	hours = strtoul(aux, &aux_ptr, 10);
	if(*aux_ptr != '\0' && *aux_ptr != '\n') // solo hace falta validar el '\0' (creo) en el caso que haya cargado el '\0' en la linea anterior E.
		return ST_NUMERICAL_ERROR;	

	for(i = 0; i < MINUTES_DIGITS; i++, *pos_ptr++) // IDEM HOURS
		aux[i] = **pos_ptr;
	minutes = strtoul(aux, &aux_ptr, 10);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;	

	for(i = 0; i < SECONDS_DIGITS + 1; i++, *pos_ptr++) //agrego un dígito por el punto // Esta funcion hay que separarla en dos: segundos y milisegundos. tm_sec es (int). Creo un nuevo campo en trkpt_t. E.
		aux[i] = **pos_ptr;
	seconds = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;	
	
	trackpt_time->tm_hour = hours;
	trackpt_time->tm_min = minutes;
	trackpt_time->tm_sec = seconds; //Algun problema con que tm_sec vaya de 0 a 59? // imagino que el tracker tira segundos en ese rango. E.

	/* Importante: falta meter en la estructura dia, mes y año */
	
	
}

status_t latitude(const char *statement, char **pos_ptr, trackpt_t *lat) { // pos_ptr quedo en una coma creo, en la func anterior habria que aumentarlo, o aca. No entiendo para que usa *statement E.
															// 	     ↑   *lat es la estructura? Creo que esta bien (seguro lo esta), pero el nombre es confuso. desde lat podes acceder a todo..time, long, nsat, etc. E. 
	size_t degrees, i;
	float minutes;
	char *aux_ptr;
	char aux[MAX_STR]; // con 4 digitos alcanza E.

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < NMEA_LATITUDE_DEGREES; i++, *pos_ptr++) // al final de estos ciclo recomiendo poner un '\0' E.
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

	*pos_ptr++;
	*pos_ptr++; // creo que deberia avanzar 1 solo, termino en la coma. se puede inicializar una variable signo = 1, if(South){signo = -1}, y asignas una sola vez multiplicando por signo E.

	if(**pos_ptr == SOUTH_CHAR)
		lat->latitude = (-1)*(degrees + minutes/CONVERSION_FACTOR_MINUTES); //TODO esto hay que ponerlo más lindo
	else 
		lat->latitude = (degrees + minutes/CONVERSION_FACTOR_MINUTES);
		

	return ST_OK;

}


status_t longitude(const char *statement, char **pos_ptr, trackpt_t *lon) { 
	
	size_t degrees, i;
	float minutes;
	char *aux_ptr;
	char aux[MAX_STR];

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

	*pos_ptr++;
	*pos_ptr++;

	if(**pos_ptr == WEST_CHAR)
		lon->longitude = (-1)*(degrees + minutes/CONVERSION_FACTOR_MINUTES); //TODO esto hay que ponerlo más lindo
	else 
		lon->longitude = (degrees + minutes/CONVERSION_FACTOR_MINUTES);

	return ST_OK;

}

status_t quality_of_fix(const char *statement, char **pos_ptr, trackpt_t *qual) { // creo que el puntero termino en una coma E.
																		//  ↑  como antes, me parece raro quality como trackpt_t E.
	fix_quality_t option; // hay una mejor manera?	// No se como lo quiere pato...el switch es horrible pero me parece que quiere esto. E.

	switch(**pos_ptr) { // cargar el dato de pos_ptr en una variable y avanzar pos_ptr para la prox func. E.
		
		case INVALID_FIX_CHAR:
			option = INVALID_FIX;
			break;
		case GPX_FIX_CHAR:
			option = GPX_FIX;
			break;
		case DGPS_FIX_CHAR:
			option = DGPS_FIX;
			break;
		case PPS_FIX_CHAR:
			option = PPS_FIX;
			break;
		case RTK_FIX_CHAR:
			option = RTK_FIX;
			break;
		case FLOAT_RTK_FIX_CHAR:
			option = FLOAT_RTK_FIX;
			break;
		case ESTIMATED_FIX_CHAR:
			option = ESTIMATED_FIX;
			break;
		case MANUAL_FIX_CHAR:
			option = MANUAL_FIX;
			break;
		case SIMULATION_FIX_CHAR:
			option = SIMULATION_FIX;
			break;
		default:
			return ST_INVALID_NUMBER_ERROR;
	}
		
	qual->quality = option;

	return ST_OK;
		
}


status_t num_of_satellites(const char *statement, char **pos_ptr, trackpt_t *satellites) {

	if(**pos_ptr < MIN_SAT || **pos_ptr > MAX_SAT) // ojo que el numero de satelites puede ser 12, es decir dos chars..hay que leer ambas y hacer strto- E.
		return ST_INVALID_NUMBER_ERROR; //A lo que apunta pos_ptr es un char y MIN_SAT y MAX_SAT son enteros, nose si se pueden comparar. - U.

	satellites->n_sat = **pos_ptr;

	return ST_OK;
		
}


status_t hdop(const char *statement, char **pos_ptr, trackpt_t *hd) {
	
	size_t i;
	float hdop;
	char *aux_ptr;
	char aux[MAX_STR]; // es muy grande esto.

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < HDOP_DIGITS + 1; i++, *pos_ptr++) // si despues del ciclo no agrega el '\0' (aux[] fue creado con 600 chars y puede tener basura) no hay chance de que lea bien hasta un '\0' E.
		aux[i] = **pos_ptr;
	hdop = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;	

	hd->hdop = hdop;
	

}

status_t elevation(const char *statement, char **pos_ptr, trackpt_t *elev) { // los comentarios se repiten para las funciones que restan E.
	
	size_t i;
	float elevation;
	char *aux_ptr;
	char aux[MAX_STR];

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < ELEVATION_DIGITS + 1; i++, *pos_ptr++) 
		aux[i] = **pos_ptr;
	elevation = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;	

	elev->elevation = elevation;
	

}

status_t undulation_of_geoid(const char *statement, char **pos_ptr, trackpt_t *und_of_geoid) {
	
	size_t i;
	float undulation;
	char *aux_ptr;
	char aux[MAX_STR];

	// validar punteros si fuera necesario. No lo pongo porque capaz ya está validado en otra parte

	for(i = 0; i < UNDULATION_OF_GEOID_DIGITS + 1; i++, *pos_ptr++) 
		aux[i] = **pos_ptr;
	undulation = strtof(aux, &aux_ptr);
	if(*aux_ptr != '\0' && *aux_ptr != '\n')
		return ST_NUMERICAL_ERROR;	

	und_of_geoid->undulation_of_geoid = undulation;
	

}
