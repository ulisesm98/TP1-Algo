#ifndef COMMAND_ARG__H
#define COMMAND_ARG__H 1

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
#define STR_VALID_COMMANDS "LIST OF COMMAND-LINE VALID COMMANDS "
#define STR_DESCRIPTION_ARG "DESCRIPTION OF ARGUMENTS"
#define STR_DEFAULT "DEFAULT"
#define STR_INVALID_INPUTS "INVALID INPUTS"
#define STR_IMPLEMENTATION "IMPLEMENTATION"
#define STR_VALID_EX "Valid Examples"
#define STR_INVALID_EX "Invalid Examples"
#define TXT_HELP "Show help."
#define TXT_NAME "Insert a name (alphanumeric characters only)."
#define TXT_DATE "Submit the date. It must be a sequence of 8 digits corresponding to year, month, and day (YYYYMMDD)."
#define TXT_YEAR "Insert a year. It must be a sequence of 4 digits (YYYY)."
#define TXT_MONTH "Insert a month. It must be a sequence of 1 or 2 digits (MM). Numbers from 1 to 12."
#define TXT_DAY "Insert a day. It must be a sequence of 1 or 2 digits (DD). Numbers from 1 to 31."
#define TXT_DEFAULT_1 "- If is received first a format command and then an year, month, or day command; format is modified (Ex: -f 20181010 -y 2019, it saves 20191010)."   
#define TXT_DEFAULT_2 "- If is received a date out of range, it prints help"
#define TXT_INV_INP_1 "- More than one command of the same type (Ex: -y 2019 --year 2004)."
#define TXT_INV_INP_2 "- Commands not listed (Ex: -p)."
#define TXT_INV_INP_3 "- Wrong input for a command (Ex: --name 2018)."
#define TXT_INV_INP_4 "- In case input for name is wrong, is set as 'Default'"
#define VALID_EX_HELP_1 "-h"
#define VALID_EX_HELP_2 "--help"
#define VALID_EX_NAME_1 "-n marcelo95"
#define VALID_EX_NAME_2 "--name marcelo95"
#define VALID_EX_DATE_1 "-f 20181011"
#define VALID_EX_DATE_2 "--format 20181011"
#define VALID_EX_YEAR_1 "-y 2014"
#define VALID_EX_YEAR_2 "--year 2014"
#define VALID_EX_MONTH_1 "-m 12"
#define VALID_EX_MONTH_2 "--month 12"
#define VALID_EX_DAY_1 "-d 18"
#define VALID_EX_DAY_2 "--day 18"
#define INVALID_EX_HELP_1 "-help"
#define INVALID_EX_HELP_2 "--h"
#define INVALID_EX_NAME_1 "-n marcelo$-2" 
#define INVALID_EX_NAME_2 "--name marcelo@95"
#define INVALID_EX_DATE_1 "-d 201812128472"
#define INVALID_EX_DATE_2 "--date a2005f"
#define INVALID_EX_YEAR_1 "-y a201"
#define INVALID_EX_YEAR_2 "--y 192"
#define INVALID_EX_MONTH_1 "-m 20"
#define INVALID_EX_MONTH_2 "--month q"
#define INVALID_EX_DAY_1 "-d 35"
#define INVALID_EX_DAY_2 "--day :1"

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
void print_help(void);

#endif
