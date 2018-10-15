#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

status_t read_nmea(char (*statement)[]) { //recive la dirección de una cadena, en la cual carga la sentencia

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
	
	if(hex1 >= 16 || hex2 >= 16) // HC??
		return HEXSTRING_NULL_FIND_INT;
	return (int)hex1 * 16 + hex2; // HC ??

}   // si está mal devuelve -1
