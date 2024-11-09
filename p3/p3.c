/*
TITULO: Práctica 3 -> DICCIONARIO DE DATOS

AUTOR1: Ángela Fouz Suárez          LOGIN1: angela.fouz
AUTOR2: Angela Marlene Clemente López       LOGIN2: angela.clemente
GRUPO:2.3	FECHA:9/11/24
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

#define TAM_TEST 11
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 38197
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define K 1000

typedef struct entrada_ {
    int ocupada;
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;

typedef int pos;
typedef entrada *tabla_cerrada;

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

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
        valor = (valor<<5) + clave[i];
    return valor % tamTabla;
}

unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA") == 0) return 7;
    if (strcmp(clave, "JOSE") == 0) return 7;
    if (strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    *diccionario = (tabla_cerrada)malloc(tam * sizeof(entrada));
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }
}

unsigned int resolucion_lineal(int pos_ini, int num_intento) {
    return (pos_ini + num_intento);
}

unsigned int resolucion_cuadratica(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * num_intento);
}

unsigned int resolucion_doble(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * (10007 - pos_ini % 10007)) % TAM_TABLA;
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,
                   int *colisiones, unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)
                   (int pos_ini, int num_intento)) {
    int i = 0;
    pos inicial = dispersion(clave, tam);
    pos actual = inicial;

    *colisiones = 0;

    while (diccionario[actual].ocupada &&
        strcmp(diccionario[actual].clave, clave) != 0) {
        i++;
        actual = resol_colisiones(inicial, i) % tam;
        (*colisiones)++;
        if (i >= tam) return -1;
    }
    return actual;
}

int insertar_cerrada(char *clave, char *sinonimos,
                     tabla_cerrada diccionario, int tam,
                     unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)
                     (int pos_ini, int num_intento)) {

    unsigned int pos_ini = dispersion(clave, tam);
    unsigned int pos = pos_ini;
    int colisiones = 0;

    while (diccionario[pos].ocupada) {
        colisiones++;
        pos = resol_colisiones(pos_ini, colisiones) % tam;
    }

    diccionario[pos].ocupada = 1;
    strncpy(diccionario[pos].clave, clave, LONGITUD_CLAVE - 1);
    diccionario[pos].clave[LONGITUD_CLAVE - 1] = '\0';
    strncpy(diccionario[pos].sinonimos, sinonimos, LONGITUD_SINONIMOS - 1);
    diccionario[pos].sinonimos[LONGITUD_SINONIMOS - 1] = '\0';

    return colisiones;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s)\n", i, diccionario[i].clave);
        } else {
            printf("%d - \n", i);
        }
    }
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir 'sinonimos.txt'\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

void mostrar_resultados(tabla_cerrada diccionario,
                        int tam, int colisiones_totales,
                        char *claves[], int num_claves,
                        unsigned int (*dispersion)(char *, int),
                        unsigned int (*resol_colisiones)(int, int),
                        const char *tipo_tabla) {
    int colisiones, pos;
    
    printf("***TABLA CERRADA %s\n", tipo_tabla);
    mostrar_cerrada(diccionario, tam);
    printf("Numero total de colisiones al insertar los elementos: %d\n",
           colisiones_totales);

    for (int i = 0; i < num_claves; i++) {
        pos = buscar_cerrada(claves[i], diccionario, tam, &colisiones,
                             dispersion, resol_colisiones);
        if (pos != -1 && strcmp(diccionario[pos].clave, claves[i]) == 0) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n",
                   claves[i], claves[i], colisiones);
        } else {
            printf("Al buscar: %s, no encuentro, colisiones: %d\n",
                   claves[i], colisiones);
        }
    }
    
    pos = buscar_cerrada("CARLOS", diccionario, tam, &colisiones,
                         dispersion, resol_colisiones);
    if (pos != -1 && strcmp(diccionario[pos].clave, "CARLOS") == 0) {
        printf("Al buscar: CARLOS, encuentro: CARLOS,"
               " colisiones: %d\n", colisiones);
    } else {
        printf("No encuentro: CARLOS, colisiones: %d\n", colisiones);
    }
}

void realizar_test(tabla_cerrada *diccionario, int tam, char *claves[],
                   int num_claves, unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int, int),
                   const char *tipo_tabla) {
    int colisiones_totales = 0;
    
    inicializar_cerrada(diccionario, tam);
    for (int i = 0; i < num_claves; i++) {
        colisiones_totales += insertar_cerrada(claves[i], "",
            *diccionario,tam, dispersion, resol_colisiones);
    }
    
    mostrar_resultados(*diccionario, tam, colisiones_totales,
        claves, num_claves,dispersion, resol_colisiones, tipo_tabla);
}

void tests() {
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN"};
    int num_claves = 6;
    tabla_cerrada diccionario_lineal,diccionario_cuadratica,diccionario_doble;

    realizar_test(&diccionario_lineal, TAM_TEST, claves, num_claves,
                  ndispersion, resolucion_lineal,
                  "LINEAL");
    realizar_test(&diccionario_cuadratica, TAM_TEST, claves, num_claves,
                  ndispersion, resolucion_cuadratica,
                  "CUADRATICA");
    realizar_test(&diccionario_doble, TAM_TEST, claves, num_claves,
                  ndispersion, resolucion_doble,
                  "DOBLE");

    free(diccionario_lineal);
    free(diccionario_cuadratica);
    free(diccionario_doble);
}

void escogerCota(int tipoDispersion, int tipoResolucion, double t, double* x,
                 double* y, double* z, int i) {
    if(tipoDispersion == 1) { //dispersionA
        if(tipoResolucion == 1) {           // lineal
            *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        } else if(tipoResolucion == 2) {    // cuadrática
            *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        } else {                            // doble
            *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        }
    } else { //dispersionB
        if(tipoResolucion == 1) {           // lineal
            *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        } else if(tipoResolucion == 2) {    // cuadrática
           *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        } else {                            // doble
            *x = t / pow(i,0.8);        // subestimada
            *y = t / pow(i,1);     // ajustada
            *z = t / i * log(i);        // sobrestimada
        }
    }
}

double medirTiempoBuscarNClaves(int n, unsigned int (*dispersion)(char *, int),
                                unsigned int (*resol_colisiones)(int, int),
                                char claves[][LONGITUD_CLAVE],
                                tabla_cerrada diccionario) {
    int i, colisiones;
    double ta, tb;
    
    ta = microsegundos();
    for(i = 0; i < n; i++) {
        buscar_cerrada(claves[i], diccionario, TAM_TABLA, &colisiones, 
                       dispersion, resol_colisiones);
    }
    tb = microsegundos();            
    return tb - ta;                 
}

double medirTiempoBuscarNClavesTPequenos(int n,
    unsigned int (*dispersion)(char *, int),
    unsigned int (*resol_colisiones)(int, int),
    char claves[][LONGITUD_CLAVE], tabla_cerrada diccionario) {

    int i, j, colisiones;
    double ta, tb;
    
    ta = microsegundos();
    for(j = 0; j < K; j++) {
        for(i = 0; i < n; i++) {
            buscar_cerrada(claves[i], diccionario, TAM_TABLA, &colisiones, 
                           dispersion, resol_colisiones);
        }
    }
    tb = microsegundos();            
    return (tb - ta) / K;                 
}

void inicializar_y_llenar_diccionario(tabla_cerrada *diccionario, 
                                      char claves[][LONGITUD_CLAVE],
                                      unsigned int (*dispersion)(char *, int),
                                      unsigned int (*resol_colisiones)
                                      (int, int)) {
    inicializar_cerrada(diccionario, TAM_TABLA);
    for (int i = 0; i < 16000; i++) {
        sprintf(claves[i], "clave%d", rand());
        insertar_cerrada(claves[i], "", *diccionario,
            TAM_TABLA, dispersion, resol_colisiones);
    }
}

void medir_y_imprimir_tiempos(int tipoDispersion, int tipoResolucion,
                              unsigned int (*dispersion)(char *, int),
                              unsigned int (*resol_colisiones)(int, int),
                              char claves[][LONGITUD_CLAVE],
                              tabla_cerrada diccionario) {
    double t, x, y, z;
    int ok;
    for(int i = 125; i <= 16000; i *= 2) {
        t = medirTiempoBuscarNClaves(i, dispersion,
            resol_colisiones, claves, diccionario);
        ok = 0;
        if(t < 500) {        
            t = medirTiempoBuscarNClavesTPequenos(i, dispersion,
                resol_colisiones, claves, diccionario);
            ok = 1;
        }
        escogerCota(tipoDispersion, tipoResolucion, t, &x, &y, &z, i);
        if(ok == 0) {
            printf("%8d %15.4f %24.10f %24.10f %24.10f\n", i, t, x, y, z);
        } else {
            printf("%8d %15.4f* %23.10f %24.10f %24.10f\n", i, t, x, y, z);
        }
    }
}

void imprimir_encabezado(const char *dispersion_str,
    const char *resolucion_str){

    printf("\nMedición de tiempos para dispersión %s y resolución %s:\n", 
           dispersion_str, resolucion_str);
    printf("%8s %15s %24s %24s %24s\n", "n", "t(n)",
        "t(n)/f(n)", "t(n)/g(n)", "t(n)/h(n)");
}

void imprimir_cotas(int tipoDispersion, int tipoResolucion) {
    printf("Donde:\n");
    switch(tipoDispersion * 10 + tipoResolucion) {
        case 11:
            printf("f(n) = n^0.9, g(n) = n, h(n) = n * log(n)\n");
        break;
        case 12:
            printf("f(n) = n^0.8, g(n) = sqrt(n), h(n) = log(n)\n");
        break;
        case 13:
            printf("f(n) = log(n), g(n) = sqrt(n), h(n) = n^0.8\n");
        break;
        case 21:
            printf("f(n) = n^0.85, g(n) = n, h(n) = n * log(n)\n");
        break;
        case 22:
            printf("f(n) = n^0.75, g(n) = sqrt(n), h(n) = log(n)\n");
        break;
        case 23:
            printf("f(n) = log(n), g(n) = n^0.7, h(n) = sqrt(n)\n");
        break;
        default:
            printf("f(n) = n^0.8, g(n) = n, h(n) = n * log(n)\n");
    }
}

const char* obtener_nombre_resolucion(int tipoResolucion) {
    switch(tipoResolucion) {
        case 1: return "lineal";
        case 2: return "cuadrática";
        case 3: return "doble";
        default: return "desconocida";
    }
}

void medirTiempo(int tipoDispersion, int tipoResolucion,
                 unsigned int (*dispersion)(char *, int),
                 unsigned int (*resol_colisiones)(int, int)) {
    char claves[16000][LONGITUD_CLAVE];
    tabla_cerrada diccionario;
    const char *dispersion_str = (tipoDispersion == 1) ? "A" : "B";
    const char *resolucion_str = obtener_nombre_resolucion(tipoResolucion);
    
    inicializar_y_llenar_diccionario(&diccionario,
        claves, dispersion, resol_colisiones);
    
    imprimir_encabezado(dispersion_str, resolucion_str);
    
    medir_y_imprimir_tiempos(tipoDispersion, tipoResolucion, dispersion, 
                             resol_colisiones, claves, diccionario);
    
    imprimir_cotas(tipoDispersion, tipoResolucion);
    
    free(diccionario);
}

int main() {
    srand(time(NULL));  // Initialize random seed
    tests();
    
    int dispersiones[] = {1, 2}; // 1 para A, 2 para B
    int resoluciones[] = {1, 2, 3}; // 1 para lineal, 2 para cuadrática, 3 para doble
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int (*dispersion)(char *, int) =
                (dispersiones[i] == 1) ? dispersionA : dispersionB;
            unsigned int (*resolucion)(int, int);
            
            switch(resoluciones[j]) {
                case 1: resolucion = resolucion_lineal; break;
                case 2: resolucion = resolucion_cuadratica; break;
                case 3: resolucion = resolucion_doble; break;
            }
            
            medirTiempo(dispersiones[i], resoluciones[j],
                dispersion, resolucion);
        }
    }
    
    return 0;
}