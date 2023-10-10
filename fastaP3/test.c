#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

char* convertir_a_string(unsigned int x){
	// Pedimos la memoria suficiente para almacenar el string (32 bits + 0)
    int bits =64;
    char* binary_string = (char*) malloc(bits+1);
	// Creamos variables
    unsigned int aux;
    int bit;
    int i=0;
    while(i < bits){
		// Rescatamos el bit i-esimo
        aux = (x>>(bits - 1 -i)) & 1;
		// en bit recuperamos el bit i-esimo (0 o 1)
        if(aux) bit = 1;
        else bit = 0;
		// si es 1, el puntero binary_string+i apuntara a '1'
		// en caso contrario lo hara con el '0'
        *(binary_string+i) = bit + '0';
        i++;
    }
	// Finalizamos el string
    *(binary_string+i) = '\0';
    return binary_string;
}

