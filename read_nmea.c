
#define STR_HEX "0123456789ABCDEF"


status_t read_nmea(char *statement[]) {

	int c, checksum = 0;

	while((c = getchar()) != CHAR_INIT_NMEA && c != EOF);
	for(i = 0; (c = getchar()) != CHAR_INIT_NMEA && c != '\n' && c != '\0' && c != EOF && c != CHAR_END_NMEA; i++) {
		statement[i] = c;
		checksum ^= c;
	}

	if(c != CHAR_END_NMEA) {
		if(c == EOF)
			return ST_EOF;
		return ST_DATA_ERR; //HOLA VAGO ################################################## HOLA PATO ME OLVIDE DE BORRAR
	}

	i++;



	

	
	
}

int hexstring_2_integer(char d1, char d2) {

	int hex1, hex2;
	char hexa_dic[] = STR_HEX;
	size_t i;
	char *ptr;

	if((ptr = strrchr(hexa_dic, d1)) == NULL)
		return -1; //HC!!!

	hex1 = (int)ptr - hexa_dic; //consultar

	return hex1*16 + hex2;






}   // si está mal devuelve -1