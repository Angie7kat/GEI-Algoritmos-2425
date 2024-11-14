/*
TITULO: Práctica 4 -> MONTICULOS

AUTOR1: Ángela Fouz Suárez          LOGIN1: angela.fouz
AUTOR2: Angela Marlene Clemente López       LOGIN2: angela.clemente
GRUPO:2.3	FECHA:7/12/24
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

#define TAM 256000

struct monticulo {
int ultimo;
int vector[TAM];
};
typedef struct monticulo * pmonticulo;

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

//void iniMonticulo(pmonticulo m);
//void insertarMonticulo(pmonticulo m, int x);
//void quitarMenor(pmonticulo m);
//int consultarMenor(const pmonticulo m);
//void crearMonticulo(pmonticulo m, int v [], int n);

