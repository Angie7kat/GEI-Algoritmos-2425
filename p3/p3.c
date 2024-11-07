#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#define K  1000

#define TAM_TEST 10
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define TAM_TABLA 38197
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

typedef struct entrada_ {
    int ocupada;
    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
} entrada;

typedef int pos;
typedef entrada *tabla_cerrada;

double microsegundos() {
 struct timeval t;
 if(gettimeofday(&t, NULL) < 0)
  return 0.0;
 return (t.tv_usec + t.tv_sec * 1000000.0);
}

// Función de dispersión A
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

// Función de dispersión B
unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor<<5) + clave[i];
    return valor % tamTabla;
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    *diccionario = (tabla_cerrada)malloc(tam * sizeof(entrada));
    for (int i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
    }
}

// Función de resolución de colisiones lineal
unsigned int resolucion_lineal(int pos_ini, int num_intento) {
    return (pos_ini + num_intento) % TAM_TABLA;
}

unsigned int resolucion_cuadratica(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * num_intento) % TAM_TABLA;
}

unsigned int resolucion_doble(int pos_ini, int num_intento) {
    return (pos_ini + num_intento * (10007 - pos_ini % 10007)) % TAM_TABLA;
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,
                   int *colisiones, unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int i = 0;
    pos inicial = dispersion(clave, tam);
    pos actual = inicial;

    *colisiones = 0;

    while (diccionario[actual].ocupada && strcmp(diccionario[actual].clave, clave) != 0) {
        i++;
        actual = resol_colisiones(inicial, i);
        (*colisiones)++;
        if (actual == inicial) return -1; // Tabla llena
    }

    return actual;
}

int insertar_cerrada(char *clave, char *sinonimos,
                     tabla_cerrada *diccionario, int tam,
                     unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int colisiones;
    pos lugar = buscar_cerrada(clave, *diccionario, tam, &colisiones, dispersion, resol_colisiones);

    if (lugar == -1) return -1; // Error: tabla llena

    if (!(*diccionario)[lugar].ocupada) {
        strcpy((*diccionario)[lugar].clave, clave);
        strcpy((*diccionario)[lugar].sinonimos, sinonimos);
        (*diccionario)[lugar].ocupada = 1;
    }

    return colisiones;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    for (int i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s: %s)\n", i, diccionario[i].clave, diccionario[i].sinonimos);
        } else {
            printf("%d - \n", i);
        }
    }
}

int leer_sinonimos(tabla_cerrada *diccionario,
                   unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    FILE *file = fopen("sinonimos.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir sinonimos.txt\n");
        return -1;
    }

    char clave[LONGITUD_CLAVE];
    char sinonimos[LONGITUD_SINONIMOS];
    int total_colisiones = 0;
    int colisiones;

    while (fscanf(file, "%s\t%[^\n]", clave, sinonimos) == 2) {
        colisiones = insertar_cerrada(clave, sinonimos, diccionario, TAM_TABLA, dispersion, resol_colisiones);
        if (colisiones == -1) {
            printf("Error: Tabla llena al insertar %s\n", clave);
            fclose(file);
            return -1;
        }
        total_colisiones += colisiones;
    }

    fclose(file);
    return total_colisiones;
}

void tests() {
    // Claves y configuraciones iniciales
    char *claves[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN"};
    int num_claves = 6;
    tabla_cerrada diccionario_lineal, diccionario_cuadratica;
    int colisiones_totales = 0, colisiones;

    // ***TABLA CERRADA LINEAL
    printf("***TABLA CERRADA LINEAL\n");
    inicializar_cerrada(&diccionario_lineal, TAM_TEST);
    for (int i = 0; i < num_claves; i++) {
        colisiones_totales += insertar_cerrada(claves[i], "", &diccionario_lineal, TAM_TEST, dispersionA, resolucion_lineal);
    }
    mostrar_cerrada(diccionario_lineal, TAM_TEST);
    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones_totales);

    // Búsquedas en la tabla lineal
    for (int i = 0; i < num_claves; i++) {
        int pos = buscar_cerrada(claves[i], diccionario_lineal, TAM_TEST, &colisiones, dispersionA, resolucion_lineal);
        printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], (pos != -1 ? claves[i] : "NO ENCONTRADO"), colisiones);
    }
    // Búsqueda de una clave no insertada
    buscar_cerrada("CARLOS", diccionario_lineal, TAM_TEST, &colisiones, dispersionA, resolucion_lineal);
    printf("No encuentro: CARLOS, colisiones: %d\n\n", colisiones);

    // ***TABLA CERRADA CUADRATICA
    printf("***TABLA CERRADA CUADRATICA\n");
    inicializar_cerrada(&diccionario_cuadratica, TAM_TEST);
    colisiones_totales = 0;
    for (int i = 0; i < num_claves; i++) {
        colisiones_totales += insertar_cerrada(claves[i], "", &diccionario_cuadratica, TAM_TEST, dispersionA, resolucion_cuadratica);
    }
    mostrar_cerrada(diccionario_cuadratica, TAM_TEST);
    printf("Numero total de colisiones al insertar los elementos: %d\n", colisiones_totales);

    // Búsquedas en la tabla cuadrática
    for (int i = 0; i < num_claves; i++) {
        int pos = buscar_cerrada(claves[i], diccionario_cuadratica, TAM_TABLA, &colisiones, dispersionA, resolucion_cuadratica);
        printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", claves[i], (pos != -1 ? claves[i] : "NO ENCONTRADO"), colisiones);
    }
    // Búsqueda de una clave no insertada
    buscar_cerrada("CARLOS", diccionario_cuadratica, TAM_TABLA, &colisiones, dispersionA, resolucion_cuadratica);
    printf("No encuentro: CARLOS, colisiones: %d\n", colisiones);

    // Liberación de memoria
    free(diccionario_lineal);
    free(diccionario_cuadratica);
}
/*
//COMPLETAR AQUI CON TU COTA
void escogerCota(int tipoDispersion, int tipoResolucion, double t, double* x, 
double* y, double* z, int i){

    if(tipoDispersion == 1){ //dispersionA
        if(tipoResolucion == 1){
            *x=t/
            *y=t/
            *z=t/
        }else if(tipoResolucion == 2){
            *x=t/
            *y=t/
            *z=t/
        }else{
            *x=t/
            *y=t/
            *z=t/
        }
    }else{ //dispersionB
         if(tipoResolucion == 1){ 
            *x=t/
            *y=t/
            *z=t/
        }else if(tipoResolucion == 2){
            *x=t/
            *y=t/
            *z=t/
        }else{ 
            *x=t/
            *y=t/
            *z=t/
        }
    }
}

double medirTiempoBuscarNClaves(int n,
                 unsigned int (*dispersion)(char *, int),
                 unsigned int (*resol_colisiones)(int, int),
                 char[][MAX_CLAVE] claves){
    int i;
    ta = microsegundos();
    //COMO TIENES QUE BUSCAR N CLAVES (N = A LA I DEL FOR DE MEDIR TIEMPO) VAS A TENER QUE DE ALGUNA MANERA GENERAR
    //UN ARRAY DE CLAVES A BUSCAR YO VOY A PONER QUE ES DE TIPO char[][MAX_CLAVE]
    for(i = 0;i<n;i++){
              buscar_cerrada(claves[i], diccionario, n, &colisiones, 
                                      dispersion, resol_colisiones);
    }
    tb = microsegundos();            
    return tb - ta;                 
}

double medirTiempoBuscarNClavesTPequenos(int n,
                 unsigned int (*dispersion)(char *, int),
                 unsigned int (*resol_colisiones)(int, int),
                 char[][MAX_CLAVE] claves){
    int i,j;
    ta = microsegundos();
    //COMO TIENES QUE BUSCAR N CLAVES (N = A LA I DEL FOR DE MEDIR TIEMPO) VAS A TENER QUE DE ALGUNA MANERA GENERAR
    //UN ARRAY DE CLAVES A BUSCAR YO VOY A PONER QUE ES DE TIPO char[][MAX_CLAVE]
    for(j=0;j<K;j++){
            for(i = 0;i<n;i++){
                buscar_cerrada(claves[i], diccionario, n, &colisiones, 
                                        dispersion, resol_colisiones);
            }
    }
    
    tb = microsegundos();            
    return (tb - ta)/K;                 
}

void medirTiempo(int tipoDispersion,int tipoResolucion,
                 unsigned int (*dispersion)(char *, int),
                 unsigned int (*resol_colisiones)(int, int)){
    double ta, tb, t, x = 0, y = 0, z = 0; int i, ok;
    char[][MAX_CLAVE] claves; //Leer abajo
    for(i = 250; i <= 16000; i = i * 2){
        //GENERAR AQUI ARRAY CLAVES ALEATORIAS (ASUMO QUE ES DE TIPO char[][MAX_CLAVES] Y SE LLAMA CLAVES
        t = medirTiempoBuscarNClaves(i,dispersion,resol_colisiones,claves);
        ok=0;
        if(t < 500){        
            t = medirTiempoBuscarNClavesTPequenos(i,dispersion,
                                      resol_colisiones,claves);
            ok=1;
        }
        escogerCota(tipoDispersion,tipoResolucion,t,&x,&y,&z,i);
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7lf\t%24.7lf\t%24.7lf\n",i,t,x,y,z);   
        }else{
            printf("%8d\t%24.7f%s\t%24.7lf\t%24.7lf\t%24.7lf\n",i,t,"*",x,y,z);
        }
    }    
}
*/

int main() {
    tests();
    return 0;
}