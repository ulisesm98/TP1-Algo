#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 666
#define MAX_ARGV 10
#define BASE 10
#define CHAR_NEW_LINE '\n'

#define TEN_POW_DIGITS_DAY 100
#define TEN_POW_DIGITS_MONTH 100
#define MAX_DAY 31
#define MAX_MONTH 12
#define MAX_YEAR 2666
#define MIN_YEAR 1900
#define MIN_YEAR_STRUCT_TM 1900

#define STR_ARG_HELP_1 "-h"
#define STR_ARG_HELP_2 "--help"
#define ARG_HELP_1_POS 0
#define ARG_HELP_2_POS 1
#define STR_ARG_NAME_1 "-n"
#define STR_ARG_NAME_2 "--name"
#define ARG_NAME_1_POS 2
#define ARG_NAME_2_POS 3
#define STR_ARG_DATE_1 "-f" 
#define STR_ARG_DATE_2 "--format"
#define STR_ARG_YEAR_1 "-Y"
#define STR_ARG_YEAR_2 "--year"
#define STR_ARG_MONTH_1 "-m"
#define STR_ARG_MONTH_2 "--month"
#define STR_ARG_DAY_1 "-d"
#define STR_ARG_DAY_2 "--day"
#define ARGV_DICCTIONARY_SIZE 12
#define ARG_TYPE 6
#define ARG_STR_PER_TYPE 2

#define DEFAULT_NAME "Default"
#define CHAR_NAME_SPACE '_'

typedef enum {ST_DATA_ERR, ST_OK, ST_HELP} status_t;

typedef struct metadata {
	char name[MAX_STR];
	struct tm meta_time;
} metadata_t;

const char argv_dicctionary[][MAX_ARGV] = {STR_ARG_HELP_1, STR_ARG_HELP_2, STR_ARG_NAME_1, STR_ARG_NAME_2, STR_ARG_DATE_1, STR_ARG_DATE_2, STR_ARG_YEAR_1, STR_ARG_YEAR_2, STR_ARG_MONTH_1, STR_ARG_MONTH_2, STR_ARG_DAY_1, STR_ARG_DAY_2};

int max_arr(int v[], int n);
status_t validate_argv(int argc, const char *argv[]);
status_t read_time_argv(int argc, const char *argv[], struct tm *meta_time); //funcion que carga la estructura time (utiliza la sig. funcion)
status_t date_2_ymd(const char *date_str, int *year, int *month, int *day); //funcion que convierte formatos de fecha YYYYMMDD
status_t read_name_argv(int argc, const char *argv[], metadata_t *chosen_name);


/*####################             MAIN            ######################*/


int main(int argc, const char *argv[]) {

	metadata_t metadata;
	status_t arg_validation;
	//char *read_ptr;
	//char statement[MAX_STATEMENT];

	if((arg_validation = validate_argv(argc, argv)) == ST_DATA_ERR) {
		puts("Error --> help");//print_help();
		return EXIT_FAILURE;
	}
	if(arg_validation == ST_HELP){
		puts("Help");//print_help();
		return EXIT_SUCCESS;
	}

	if(read_time_argv(argc, argv, &(metadata.meta_time)) != ST_OK) {
		puts("Error --> help");//print_help();
		return EXIT_FAILURE;
	}

	if(read_name_argv(argc, argv, &metadata) != ST_OK) {
		puts("Error --> help");//print_help();
		return EXIT_FAILURE;
	}
	//ya cargo todo el metadata
	//imprime si se cargo correctamente
	printf("%s\n%d/%d/%d\n%d:%d:%d\n", metadata.name, metadata.meta_time.tm_mday, metadata.meta_time.tm_mon + 1, metadata.meta_time.tm_year + MIN_YEAR_STRUCT_TM, metadata.meta_time.tm_hour, metadata.meta_time.tm_min, metadata.meta_time.tm_sec);

	return EXIT_SUCCESS;
}


/*####################         FUNCIONES          ######################*/


status_t validate_argv(int argc, const char *argv[]) {
	size_t i, j;
	int arg_call[ARGV_DICCTIONARY_SIZE] = {0}, arg_call_type[ARG_TYPE] = {0}, help_flag = 0;

	for(i = 1; i < argc; i++) {
		for(j = 0; j < ARGV_DICCTIONARY_SIZE; j++) {
			if(! strcmp(argv[i], argv_dicctionary[j])) {
				if(j == ARG_HELP_1_POS || j == ARG_HELP_2_POS) {
					help_flag = 1;
				} else if(j == ARG_NAME_1_POS || j == ARG_NAME_2_POS) {
					if(++i == argc)
						return ST_DATA_ERR;
					if(! isalpha(*argv[i]))
						return ST_DATA_ERR;
				} else {
					if(++i == argc)
						return ST_DATA_ERR;
					if(! isdigit(*argv[i]))
						return ST_DATA_ERR;
				}
				arg_call[j]++;
				arg_call_type[j / ARG_STR_PER_TYPE]++;
				break;
			}
		}
		if(j == ARGV_DICCTIONARY_SIZE)
			return ST_DATA_ERR;
	}
	if(max_arr(arg_call, ARGV_DICCTIONARY_SIZE) > 1 || max_arr(arg_call_type, ARG_TYPE) > 1)
		return ST_DATA_ERR;
	if(help_flag)
		return ST_HELP;
	return ST_OK;
}




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
			/*[4] Mediante la func. date2ymd se carga la fecha en la estructura.*/
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

	/* Normaliza la estructura de ser necesario (ver 1).*/
	if((time_fix = mktime(meta_time)) == -1)
		return ST_DATA_ERR;
	return ST_OK;
}






//convierte formato YYYYMMDD a año mes y dia enteros
status_t date_2_ymd(const char *date_str, int *year, int *month, int *day) {
	
	char *endptr;
	int date, aux;

	if(! (date = strtoul(date_str, &endptr, BASE)) || (*endptr && *endptr != CHAR_NEW_LINE)) //valída lectura
		return ST_DATA_ERR;
	
	aux = date % TEN_POW_DIGITS_DAY; // toma los ultimos 2 digitos
	if(aux > MAX_DAY || ! aux) // valída dia 1 ~ 31
		return ST_DATA_ERR;
	else {
		*day = aux;
		date = (date - aux) / TEN_POW_DIGITS_DAY; // elimina el dato usado de date
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





int max_arr(int v[], int n) {
	int i, max = 0;
	if(n > 0)
		for(max = v[0], i = 1; i < n; i++)
			if(v[i] > max)
				max = v[i];
	return max;
}





status_t read_name_argv(int argc, const char *argv[], metadata_t *chosen_name) {
	
	int i, j;

	for(i = 1; i < argc; i++) {
		if(! strcmp(STR_ARG_NAME_1, argv[i]) || ! strcmp(STR_ARG_NAME_2, argv[i])) {
			i++;
			for(j = 0; j < strlen(argv[i]); j++)
				if(! isalnum(*(argv[i] + j)) && *(argv[i] + j) != CHAR_NAME_SPACE)
					return ST_DATA_ERR;
			strcpy(chosen_name -> name, argv[i]);
			return ST_OK;
		}
	}
	strcpy(chosen_name -> name, DEFAULT_NAME);
	return ST_OK;
}
