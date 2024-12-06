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
    int temp;
    while (i > 0 && m->vector[padre] > m->vector[i]) {
        temp = m->vector[i];
        m->vector[i] = m->vector[padre];
        m->vector[padre] = temp;
        i = padre;
        padre = (i - 1) / 2;
    }
}

void hundir(pmonticulo m, int i) {
    int hijo_izq, hijo_der, menor, temp;
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
        temp = m->vector[i];
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

void imprimirMonticulo(pmonticulo m) {
    int i;
    printf("  [");
    for (i = 0; i <= m->ultimo; i++) {
        printf("%d", m->vector[i]);
        if (i < m->ultimo) {
            printf(", ");
        }
    }
    printf("]\n");
}

////////ALGORITMO DE ORDENACION////////
void ordenarPorMonticulos(int v[], int n) {
    int i;
    pmonticulo m = (pmonticulo)malloc(sizeof(struct monticulo));
    
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
////////FUNCIONES AUXILIARES TESTS////////
bool monticuloVacio(pmonticulo m){
    return m->ultimo == -1;
}

bool es_monticulo_valido(pmonticulo m) {
    for (int i = 0; i <= m->ultimo / 2; i++) {
        int hijo_izq = 2 * i + 1;
        int hijo_der = 2 * i + 2;

        if (hijo_izq <= m->ultimo && m->vector[i] > m->vector[hijo_izq]) {
            return false;
        }
        if (hijo_der <= m->ultimo && m->vector[i] > m->vector[hijo_der]) {
            return false;
        }
    }
    return true;
}

bool contiene_elementos(pmonticulo m, int v[], int n) {
    bool encontrado[n];
    int i, j;
    for (i = 0; i < n; i++) encontrado[i] = false;

    for (i = 0; i <= m->ultimo; i++) {
        for (j = 0; j < n; j++) {
            if (m->vector[i] == v[j] && !encontrado[j]) {
                encontrado[j] = true;
                break;
            }
        }
    }

    for (i = 0; i < n; i++) {
        if (!encontrado[i]) {
            return false;
        }
    }
    return true;
}

bool ordenado(int v[], int n) {
    int i;
    for (i = 0; i < (n - 1); ++i){
        if (v[i] > v[i + 1]){
            return false;
        }
    }
    return true;
}

int buscarRepetido(int n, pmonticulo m){
    int i, num = 0;
    for (i = 0; i <= m->ultimo; i++) {
        if (m->vector[i] == n) {
            num = num + 1;
        }
    }
    return num;
}

void buscarNumero(pmonticulo m, int x, int y) {
    int i, num = 0;
    for (i = 0; i <= m->ultimo; i++) {
        if (m->vector[i] == x) {
            num = num + 1;
            if (y == num){
                printf("Error: Se ha encontrado el menor que debió ser eliminado.\n");
                return;
            }
        }
    }
    printf("El menor ha sido eliminado correctamente.\n");
}

////////TESTS////////
void probarIni() {
    pmonticulo m = (pmonticulo)malloc(sizeof(struct monticulo));
    iniMonticulo(m);
    printf("TEST INICIALIZAR MONTÍCULO:\n");
    if (m->ultimo == -1) {
        printf("Montículo inicializado correctamente.\n");
        imprimirMonticulo(m);
    } else {
        printf("Error: Montículo no inicializado correctamente.\n");
    }
    free(m);
}

void probarCrear() {
    int v[17], n = 17;
    pmonticulo m = (pmonticulo) malloc(sizeof(struct monticulo));
    printf("TEST CREAR MONTÍCULO:\n");
    printf("Inicializacion aleatoria:\n");
    vector_aleatorio(v,n);
    imprimir_vector(v, n);
    crearMonticulo(m, v, n);
    printf("Vector del monticulo:\n");
    imprimirMonticulo(m);
    if(es_monticulo_valido(m))
        printf("El montículo se ha creado correctamente.\n");
    else{
        printf("El montículo NO se ha creado correctamente.\n");
    }
    free(m);
}

void probarInsertar(){
    int v[17];
    int i, n = 17, x;
    pmonticulo m = (pmonticulo) malloc(sizeof(struct monticulo));
    printf("TEST INSERTAR MONTÍCULO:\n");
    printf("Inicializacion aleatoria:\n");
    iniMonticulo(m);
    vector_aleatorio(v,n);
    imprimir_vector(v, n);
    for(i = 0; i < n; i ++){
        x = v[i];
        insertarMonticulo(m,x);
    }
    printf("Vector del monticulo:\n");
    imprimirMonticulo(m);
    if(es_monticulo_valido(m))
        printf("El monticulo se ha creado con las inserciones correctamente.\n");
    else{
        printf("El montículo NO se ha creado correctamente.\n");
    }
    free(m);
}

void probarMenoryBorrar(){
    int v[17], n = 17, x, y;
    //int vector[17] = {5, 2, 8, 12, 2, 7, 2, 14, 9, 6, 2, 10, 15, 11, 3, 4, 2};
    pmonticulo m = (pmonticulo) malloc(sizeof(struct monticulo));
    printf("TEST QUITAR MENOR MONTÍCULO:\n");
    vector_aleatorio(v,n);
    crearMonticulo(m, v, n);
    printf("Vector del monticulo:\n");
    imprimirMonticulo(m);
    printf("El menor del monticulo es : %d\n",consultarMenor(m));
    x = consultarMenor(m);
    y = buscarRepetido(x,m);
    quitarMenor(m);
    buscarNumero(m, x, y);
    imprimirMonticulo(m);
    free(m);
}

void probarBorrarVacio(){
    int v[1], n = 1, x, y;
    pmonticulo m = (pmonticulo) malloc(sizeof(struct monticulo));
    printf("TEST QUITAR ÚNICO ELEMENTO MONTÍCULO:\n");
    v[0] = 1;
    crearMonticulo(m, v, n);
    printf("Vector del monticulo:\n");
    imprimirMonticulo(m);
    x = consultarMenor(m);
    y = buscarRepetido(x,m);
    quitarMenor(m);
    buscarNumero(m, x, y);
    imprimirMonticulo(m);
    quitarMenor(m);
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

    printf("TEST DE ORDENACIÓN POR MONTÍCULOS:\n");

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
                double* y, double* z, int n){
    if(tipoVector == 1){ //ascendente
        *x=t/n;
        *y=t/n;
        *z=t/n;
    }else if(tipoVector == 2){ //descendente
        *x=t/n;
        *y=t/n;
        *z=t/n;
    }else{ //aleatorio
        *x=t/n;
        *y=t/n;
        *z=t/n;
    }
}

void imprimirCabeceras(int tipoVector) {
    const char* tipoVectorStr;
    const char* cotaSub;
    const char* cotaAjustada;
    const char* cotaSobre;

    switch(tipoVector) {
        case 1:
            tipoVectorStr = "ascendente";
            cotaSub = "n^0.72 * log(n)";
            cotaAjustada = "n^0.87 * log(n)";
            cotaSobre = "n^1.02 * log(n)";
            break;
        case 2:
            tipoVectorStr = "descendente";
            cotaSub = "n^0.84 * log(n)";
            cotaAjustada = "n^0.89 * log(n)";
            cotaSobre = "n^0.94 * log(n)";
            break;
        case 3:
            tipoVectorStr = "aleatorio";
            cotaSub = "n^0.81 * log(n)";
            cotaAjustada = "n^0.91 * log(n)";
            cotaSobre = "n^1.01 * log(n)";
            break;
        default:
            tipoVectorStr = "desconocido";
            cotaSub = "n^0.8 * log(n)";
            cotaAjustada = "n^0.9 * log(n)";
            cotaSobre = "n^1.0 * log(n)";
    }

    printf("\nMedición de tiempos para vector %s:\n", tipoVectorStr);
    printf("Cota subestimada: %s\n", cotaSub);
    printf("Cota ajustada: %s\n", cotaAjustada);
    printf("Cota sobrestimada: %s\n", cotaSobre);
    printf("\n");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", cotaSub, 
                                    cotaAjustada, cotaSobre);
}

void medirTiempoMonticulo(int tipoVector, int creac){
    double ta, tb, t, x; int n, i, ok; int *v;
    if(creac == 1){ // Imprimir cabeceras de crear
        printf("%8s\t%24s\t%24s\n", "n", "t(n)",
        "t(n)/n");
    }else{//Imprimir cabeceras de insertar
        printf("%8s\t%24s\t%24s\n", "n", "t(n)",
        "t(n)/n*log(n)");
    }
    for(n= 500; n <= 32000; n = n * 2){
        v = malloc(n * sizeof(int));
        elegirTipoDeVector(v,n,tipoVector);
        pmonticulo m = (pmonticulo) malloc(sizeof(struct monticulo));
        ta = microsegundos();
        if(creac == 1){ crearMonticulo(m,v,n);
        }else{ iniMonticulo(m);
            for(i = 0; i < n; i++){
                insertarMonticulo(m, v[i]);
            }
        }
        tb = microsegundos(); t = tb - ta; ok=0;
        if(t < 500){
            ta = microsegundos();
            for( i = 1; i <= K; i++ ){
                if(creac == 1){ crearMonticulo(m,v,n);
                }else{ iniMonticulo(m);
                    for(int j = 0; j < n; j++){
                        insertarMonticulo(m, v[j]);
                    }
                }
            }
            tb = microsegundos(); t = (tb - ta)/K; ok=1;
        }
        x = pow(n,1) * log(pow(n,1));
        if(ok == 0){ printf("%8d\t%24.7f\t%24.7f\n",n,t,t/x);
        }else{ printf("%8d\t%24.7f%s\t%24.7f\n",n,t,"*",t/n);
        }
        free(v); free(m);
    }
}

void medirTiempo(int tipoVector){
    double ta, tb, t, x, y, z; int n, i, ok; int *v;

    imprimirCabeceras(tipoVector);
    for(n= 500; n <= 32000; n = n * 2){
        v = malloc(n * sizeof(int));
        elegirTipoDeVector(v,n,tipoVector);
        ta = microsegundos();
        ordenarPorMonticulos(v,n);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            elegirTipoDeVector(v,n,tipoVector);
            ta = microsegundos();            
            for( i = 1; i <= K; i++ ){
                ordenarPorMonticulos(v,n);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        escogerCota(tipoVector,t,&x,&y,&z,n);
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",n,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",n,t,"*",x, y,z);
        }
        free(v);
    }    
}

////////MAIN////////

int main(){
    inicializar_semilla();

    probarIni();
    printf("\n");
    probarCrear();
    printf("\n");
    probarInsertar();
    printf("\n");
    probarMenoryBorrar();
    printf("\n");
    probarBorrarVacio();
    printf("\n");
    testOrdenarPorMonticulos();
    printf("\n");

    printf("Medición de tiempos para crearMonticulo:\n");
    medirTiempoMonticulo(3,1); // crear monticulo
    printf("Medición de tiempos para insertarMonticulo:\n");
    medirTiempoMonticulo(3,0); // insertar monticulo
    medirTiempo(1); // Vector ascendente
    medirTiempo(2); // Vector descendente
    medirTiempo(3); // Vector aleatorio
}
