#include <string.h>
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

/*
unsigned int dispersionA(char *clave, int tamTabla) {
int i, n = MIN(8, strlen(clave));
unsigned int valor = clave[0];
for (i = 1; i < n; i++)
valor += clave[i];
return valor % tamTabla;
}
unsigned int dispersionB(char *clave, int tamTabla) {
int i, n = MIN(8, strlen(clave));
unsigned int valor = clave[0];
for (i = 1; i < n; i++)
valor = (valor<<5) + clave[i];  el desplazamiento de 5 bits equivale a 
return valor % tamTabla;  multipicar por 32 
}
*/


unsigned int dispersionA(char *clave, int tamTabla) {
    int i;
    int n = MIN(8, strlen(clave));  // Limita n al tamaño de la cadena o 8
    unsigned int valor = clave[0];  // Asigna el valor ASCII del primer carácter
    for (i = 1; i < n; i++) {
        valor += clave[i];  // Suma el valor ASCII de cada carácter
    }
    return valor % tamTabla;  // Devuelve el resultado de la suma % tamaño de la tabla
}