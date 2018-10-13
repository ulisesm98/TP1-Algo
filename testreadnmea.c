#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define CHAR_INIT_NMEA '$'
#define CHAR_END_NMEA '*'
#define MAX_STR_NMEA 100
#define STR_HEX "0123456789ABCDEF"
#define HEXSTRING_NULL_FIND_INT -1

typedef enum {ST_DATA_ERR, ST_OK, ST_HELP, ST_EOF} status_t;

status_t read_nmea(char (*statement)[], int *checksum);
int hexstring_2_integer(int d1, int d2);


//##################        MAIN         ####################
int main(void) {

	int checksum;
	char statement[MAX_STR_NMEA];
	status_t read_val;

	do {
		read_val = read_nmea(&statement, &checksum);
		if(read_val == ST_OK){
			printf("$%s", statement);
			printf("*%d\n", checksum);
		}
		else if(read_val == ST_DATA_ERR)
			puts("Sentencia ignorada (no silenciosamente)");
	} while(read_val != ST_EOF);

	return EXIT_SUCCESS;
}

//#################       FUNCIONES      ####################

status_t read_nmea(char (*statement)[],int *checksum) { // statement es un puntero a un string

	int c, i, cksm1, cksm2;
	*checksum = 0;

	while((c = getchar()) != CHAR_INIT_NMEA && c != EOF);
	for(i = 0; (c = getchar()) != CHAR_END_NMEA && c != '\n' && c != EOF && c != '\0' && c != CHAR_INIT_NMEA && i < MAX_STR_NMEA; i++) {
		(*statement)[i] = c; //es necesario el parentesis?
		*checksum ^= c;
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
	if(hexstring_2_integer((char)cksm1,(char)cksm2) != *checksum)
		return ST_DATA_ERR;

	return ST_OK;
}




int hexstring_2_integer(int d1, int d2) {

	size_t hex1, hex2;
	char hexa_dic[] = {STR_HEX}, *ptr;

	//busca el primer caracter en el dicc, si no lo encuentra --> error
	if((ptr = strrchr(hexa_dic, d1)) == NULL)
		return 2;//HEXSTRING_NULL_FIND_INT;
	//calcula la posicion del caracter respecto al comienzo de la cadena
	hex1 = (size_t)ptr - (size_t)hexa_dic; //consultar
	
	if((ptr = strrchr(hexa_dic, d2)) == NULL)
		return 3;//HEXSTRING_NULL_FIND_INT;
	hex2 = (size_t)ptr - (size_t)hexa_dic; //consultar
	
	if(hex1 >= 16 || hex2 >= 16) // HC??
		return 4;//HEXSTRING_NULL_FIND_INT;
	return (int)hex1 * 16 + hex2; // HC ??

}   // si está mal devuelve -1