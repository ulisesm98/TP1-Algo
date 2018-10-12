#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

status_t read_time_argv(int argc, const char *argv[], struct tm *meta_time) {

	char *endptr;
	int year, month, day, i;
	/*[1] La variable time_fix se utiliza para normalizar la estructura tm. Ej: 30/02/2014 ----> 02/03/2014.*/ 
	time_t actual_time = time(NULL), time_fix;
	/*[2] Se carga el tiempo de la computadora en la estructura tm.*/
	*meta_time = *gmtime(&actual_time);

	/*[3] Si se encuentran argumentos 'time' (-f, -Y, --day, etc.) se modifica la estructura tm.*/
	for(i = 1; i < argc; i++)
		if(! strcmp(STR_ARG_DATE_1, argv[i]) || ! strcmp(STR_ARG_DATE_2, argv[i])) {
			i++;
			/*[4] Mediante la func. date2ymd se carga la fecha en la estructura (ver 7).*/
			if(date_2_ymd(argv[i], &(meta_time -> tm_year), &(meta_time -> tm_mon), &(meta_time -> tm_mday)) != ST_OK)
				return ST_DATA_ERR;
			/*[5] Se realizan correcciones del año y mes por el formato que usa struct tm.*/
			(*meta_time).tm_year -= MIN_YEAR_STRUCT_TM;
			(*meta_time).tm_mon --;
			break;
		}

	for(i = 1; i < argc; i++)
		if(! strcmp(STR_ARG_YEAR_1, argv[i]) || ! strcmp(STR_ARG_YEAR_2, argv[i])) {
			i++;
			if(! (year = strtoul(argv[i], &endptr, BASE))|| (*endptr && *endptr != CHAR_NEW_LINE))
				return ST_DATA_ERR;
			if(year < MIN_YEAR || year > MAX_YEAR)
				return ST_DATA_ERR;
			meta_time -> tm_year = year - MIN_YEAR_STRUCT_TM;
			break;
		}

	for(i = 1; i < argc; i++)
		if(! strcmp(STR_ARG_MONTH_1, argv[i]) || ! strcmp(STR_ARG_MONTH_2, argv[i])) {
			i++;
			if(! (month = strtoul(argv[i], &endptr, BASE))|| (*endptr && *endptr != CHAR_NEW_LINE))
				return ST_DATA_ERR;
			if(! month || month > MAX_MONTH)
				return ST_DATA_ERR;
			meta_time -> tm_mon = --month;
			break;
		}

	for(i = 1; i < argc; i++)
		if(! strcmp(STR_ARG_DAY_1, argv[i]) || ! strcmp(STR_ARG_DAY_2, argv[i])) {
			i++;
			if(! (day = strtoul(argv[i], &endptr, BASE))|| (*endptr && *endptr != CHAR_NEW_LINE))
				return ST_DATA_ERR;
			if(! day || day > MAX_DAY)
				return ST_DATA_ERR;
			meta_time -> tm_mday = day;
			break;
		}

	/*[6] Normaliza la estructura de ser necesario (ver 1).*/
	if((time_fix = mktime(meta_time)) == -1)
		return ST_DATA_ERR;
	return ST_OK;
}


/*[7] Convierte formato YYYYMMDD a año mes y dia enteros.*/
status_t date_2_ymd(const char *date_str, int *year, int *month, int *day) {
	
	char *endptr;
	int date, aux;

	if(! (date = strtoul(date_str, &endptr, BASE)) || (*endptr && *endptr != CHAR_NEW_LINE)) //valída lectura
		return ST_DATA_ERR;

	/*[8] Toma los ultimos 2 digitos.*/
	aux = date % TEN_POW_DIGITS_DAY;
	/*[9] Valida el dia.*/
	if(aux > MAX_DAY || ! aux)
		return ST_DATA_ERR;
	else {
		*day = aux;
		/*[10] Descarta el dia ya leido: YYYYMMDD --> YYYYMM.*/
		date = (date - aux) / TEN_POW_DIGITS_DAY;
	}
	
	aux = date % TEN_POW_DIGITS_MONTH;
	if(aux > MAX_MONTH || ! aux)
		return ST_DATA_ERR;
	else {
		*month = aux;
		date = (date - aux) / TEN_POW_DIGITS_MONTH;
	}

	if(date > MAX_YEAR || date < MIN_YEAR)
		return ST_DATA_ERR;
	else
		*year = date;

	return ST_OK;
}

/*[11] Busca el maximo entero en un arreglo.*/
int max_arr(int v[], int n) {
	int i, max = 0;
	if(n > 0)
		for(max = v[0], i = 1; i < n; i++)
			if(v[i] > max)
				max = v[i];
	return max;
}

/*[12] Valida los argumentos en linea de comando.*/
status_t validate_argv(int argc, const char *argv[]) {
	size_t i, j;
	/*[13] Utiliza dos arreglos para contar las veces que aparecen los argumentos.*/
	int arg_call[ARGV_DICCTIONARY_SIZE] = {0}, arg_call_type[ARG_TYPE] = {0}, help_flag = 0;

	/*[14] Para cada argumento: busca el tipo y valida la correcta implementacion.*/
	for(i = 1; i < argc; i++) {
		for(j = 0; j < ARGV_DICCTIONARY_SIZE; j++) {
			if(! strcmp(argv[i], argv_dicctionary[j])) {
				/*[15] Los argumentos del tipo -h y --help no requieren validación.*/
				if(j == ARG_HELP_1_POS || j == ARG_HELP_2_POS) {
					help_flag = 1;
				/*[16] Los argumentos del tipo -n y --name requieren una cadena alfanumérica en el siguiente argumento.*/
				} else if(j == ARG_NAME_1_POS || j == ARG_NAME_2_POS){
					if(++i == argc)
						return ST_DATA_ERR;
					if(! isalnum(*argv[i]))
						return ST_DATA_ERR;
				/*[17] Los argumentos restantes requieren una cadena de numeros en el siguiente argumento.*/
				} else {
					if(++i == argc)
						return ST_DATA_ERR;
					if(! isdigit(*argv[i]))
						return ST_DATA_ERR;
				}
				arg_call[j]++;
				arg_call_type[j / 2]++;
				break;
			}
		}
		/*[18] Si el argumento no es compatible con ningun tipo en el diccionario devuelve error.*/
		if(j == ARGV_DICCTIONARY_SIZE)
			return ST_DATA_ERR;
	}
	/*[19] Si alguno de los argumentos (o tipo de argumentos) se repite, devuelve error.*/
	if(max_arr(arg_call, ARGV_DICCTIONARY_SIZE) > 1 || max_arr(arg_call_type, ARG_TYPE) > 1)
		return ST_DATA_ERR;
	/*[20] Si los argumentos son válidos y uno de ellos es del tipo 'ayuda', devuelve ayuda*/
	if(help_flag)
		return ST_HELP;
	return ST_OK;
}


status_t read_name_argv(int argc, const char *argv[], metadata_t *chosen_name) {
	
	int i;

	for(i = 1; i < argc; i++) {
		if(! strcmp(STR_ARG_NAME_1, argv[i]) || ! strcmp(STR_ARG_NAME_2, argv[i])) {
			i++;
			strcpy(chosen_name -> name, argv[i]);
			return ST_OK;
	}

	return ST_DATA_ERR;
}

/* TO DO:

			- funcion print_help( ... )

			- correr el programa (test2.c (¿test2? necesito git))
			  para ver si hay errores en las funciones ya implementadas.
			
			- se valido arc?

			φα.
*/
