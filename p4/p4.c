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
#define K  1000

////////MONTICULO Y SUS FUNCIONES////////
struct monticulo {
    int ultimo;
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

void iniMonticulo(pmonticulo m) {
    m->ultimo = -1;
}

void flotar(pmonticulo m, int i) {
    int padre = (i - 1) / 2;
    while (i > 0 && m->vector[padre] > m->vector[i]) {
        int temp = m->vector[i];
        m->vector[i] = m->vector[padre];
        m->vector[padre] = temp;
        i = padre;
        padre = (i - 1) / 2;
    }
}

void hundir(pmonticulo m, int i) {
    int hijo_izq, hijo_der, menor;
    while (2 * i + 1 <= m->ultimo) {
        hijo_izq = 2 * i + 1;
        hijo_der = 2 * i + 2;
        if (hijo_der <= m->ultimo && m->vector[hijo_der] < m->vector[hijo_izq]) {
            menor = hijo_der;
        } else {
            menor = hijo_izq;
        }
        if (m->vector[i] <= m->vector[menor]) {
            break;
        }
        int temp = m->vector[i];
        m->vector[i] = m->vector[menor];
        m->vector[menor] = temp;
        i = menor;
    }
}

void insertarMonticulo(pmonticulo m, int x) {
    if (m->ultimo == TAM - 1) {
        printf("Error: Montículo lleno\n");
        return;
    }
    m->ultimo++;
    m->vector[m->ultimo] = x;
    flotar(m, m->ultimo);
}

void quitarMenor(pmonticulo m) {
    if (m->ultimo == -1) {
        printf("Error: Montículo vacío\n");
        return;
    }
    m->vector[0] = m->vector[m->ultimo];
    m->ultimo--;
    hundir(m, 0);
}

int consultarMenor(const pmonticulo m) {
    if (m->ultimo == -1) {
        printf("Error: Montículo vacío\n");
        return -1;
    }
    return m->vector[0];
}

void crearMonticulo(pmonticulo m, int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        m->vector[i] = v[i];
    }
    m->ultimo = n - 1;
    for (i = (n / 2) - 1; i >= 0; i--) {
        hundir(m, i);
    }
}

////////ALGORITMO DE ORDENACION////////
void ordenarPorMonticulos(int v[], int n) {
    int i;
    pmonticulo m = malloc(sizeof(struct monticulo));
    
    crearMonticulo(m, v, n);
    
    for (i = 0; i < n; i++) {
        v[i] = consultarMenor(m);
        quitarMenor(m);
    }
    
    free(m);
}

////////FUNCIONES DE VECTORES////////

void inicializar_semilla() {
    srand(time(NULL));
}

void vector_aleatorio(int v[], int n){
    int i, m=2*n+1;
    for (i = 0; i < n; i++) v[i] = (rand() % m) - n;
}


void vector_ascendente(int v[], int n){
    int i;
    for (i = 0; i < n; i++) v[i] = i;
}


void vector_descendente(int v[], int n){
    int i,j;
    for (i = n-1, j = 0; i >=0; i--, j++){
        v[j] = i;
    }
}

void imprimir_vector(int v[], int n) {
    int i;
    printf("  [");
    for (i = 0; i < n; i++) {
        printf("%3d", v[i]);
        if (i != n - 1) {
            printf(",");
        }
    }
    printf("]\n");
}

void elegirTipoDeVector(int v[], int n, int tipo) {
    switch (tipo) {
        case 1: // Ascendente
            vector_ascendente(v, n);
            break;
        case 2: // Descendente
            vector_descendente(v, n);
            break;
        case 3: // Aleatorio
            vector_aleatorio(v, n);
            break;
        default:
            printf("Tipo de vector no válido. Se generará un vector ascendente por defecto.\n");
            vector_ascendente(v, n); // Por defecto, ascendente
            break;
    }
}

////////TESTS////////

bool monticuloVacio(pmonticulo m){
    return m->ultimo == -1;
}

bool ordenado(int v[], int n) {
    for (int i = 0; i < (n - 1); ++i){
        if (v[i] > v[i + 1]){
            return false;
        }
    }
    return true;
}

void probarVector(int v[], int n, const char* tipo) {
    printf("\nVector %s inicial: ", tipo);
    imprimir_vector(v, n);

    ordenarPorMonticulos(v, n);
    printf("Vector %s ordenado: ", tipo);
    imprimir_vector(v, n);

    printf("Vector %s -> Ordenado correctamente: %s\n",
           tipo, ordenado(v, n) ? "Sí" : "No");
}

void testOrdenarPorMonticulos() {
    int n = 17;
    int ascendente[n], descendente[n], aleatorio[n];

    // Generar vectores
    vector_ascendente(ascendente, n);
    vector_descendente(descendente, n);
    vector_aleatorio(aleatorio, n);

    // Probar cada vector
    probarVector(ascendente, n, "ascendente");
    probarVector(descendente, n, "descendente");
    probarVector(aleatorio, n, "aleatorio");
}

////////MEDIR TIEMPOS////////

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void escogerCota(int tipoVector, double t, double* x, 
                double* y, double* z, int i){
    if(tipoVector == 1){ //ascendente
        *x=t/(pow(i,0.72) * log(pow(i,0.72)));
        *y=t/(pow(i,0.87) * log(pow(i,0.87)));
        *z=t/(pow(i,1.02) * log(pow(i,1.02)));
    }else if(tipoVector == 2){ //descendente
        *x=t/(pow(i,0.84) * log(pow(i,0.84)));
        *y=t/(pow(i, 0.89) * log(pow(i,0.89)));
        *z=t/(pow(i,0.94) * log(pow(i,0.94)));
    }else{ //aleatorio
        *x=t/(pow(i,0.81) * log(pow(i,0.81)));
        *y=t/(pow(i,0.91) * log(pow(i,0.91)));
        *z=t/(pow(i,1.01) * log(pow(i,1.01)));
    }
}

void imprimirCabecera(int tipoVector) {
    const char* tipoVectorStr;
    switch(tipoVector) {
        case 1:
            tipoVectorStr = "ascendente";
            break;
        case 2:
            tipoVectorStr = "descendente";
            break;
        case 3:
            tipoVectorStr = "aleatorio";
            break;
        default:
            tipoVectorStr = "desconocido";
    }

    printf("\nMedición de tiempos para vector %s:\n", tipoVectorStr);
    printf("Cota subestimada: n^0.8 * log(n)\n");
    printf("Cota ajustada: n^0.9 * log(n)\n");
    printf("Cota sobrestimada: n^1.0 * log(n)\n");
    printf("\n");
    printf("   n\t\t   t(n)\t\t   t(n)/n^0.8*log(n)   t(n)/n^0.9*log(n)   t(n)/n^1.0*log(n)\n");
}


void medirTiempo(int tipoVector){
    double ta, tb, t, x, y, z; int n, i, ok; int v[n];
    imprimirCabeceras(tipoVector);
    for(n= 500; n <= 32000; n = n * 2){
        elegirTipoDeVector(v,n,tipoVector);
        pmonticulo m = malloc(sizeof(struct monticulo));
        crearMonticulo(m,v,n);
        ta = microsegundos();
        ordenarPorMonticulos(m,n);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            elegirTipoDeVector(v,n,tipoVector);
            pmonticulo m = malloc(sizeof(struct monticulo));
            crearMonticulo(m,v,n);
            ta = microsegundos();            
            for( i = 1; i <= K; i++ ){
                ordenarPorMonticulos(m,n);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        escogerCota(v,t,&x,&y,&z,n);
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",n,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",n,t,"*",x, y,z);
        }
        free(m);
    }    
}

////////MAIN////////

int main(){
    inicializar_semilla();
    printf("Test de ordenación por montículos:\n");
    testOrdenarPorMonticulos();

    medirTiempo(1); // Vector ascendente
    medirTiempo(2); // Vector descendente
    medirTiempo(3); // Vector aleatorio
}
