/*
TITULO: EXAMEN DE PRÁCTICAS
AUTOR: Ángela Fouz Suárez          LOGIN: angela.fouz
FECHA:22/01/25
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
        if (hijo_der <= m->ultimo 
            && m->vector[hijo_der] < m->vector[hijo_izq]) {
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

////////OPERACIÓN PEDIDA K-ÉSIMO MAYOR////////
int kesimoMayor(int V[], int n, int k) {
    int resultado, i;
    pmonticulo M = (pmonticulo)malloc(sizeof(struct monticulo));
    if (k > n) {
        printf("Error: k no puede ser mayor que n\n");
        return -1;
    }
    if (M == NULL) {
        printf("Error: No se ha podido asignar memoria para el montículo\n");
        return -1;
    }
    crearMonticulo(M, V, k);
    for (i = k; i < n; i++) {
        if (V[i] > consultarMenor(M)) {
            quitarMenor(M);
            insertarMonticulo(M, V[i]);
        }
    }
    resultado = consultarMenor(M);
    free(M);
    return resultado;
}

////////FUNCIONES DE VECTORES////////
void vector_aleatorio(int v[], int n){
    int i, m=2*n+1;
    for (i = 0; i < n; i++) v[i] = (rand() % m) - n;
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

////////TESTS////////
void test(){
    int V[] =  {23, 45, 12, 67, 89, 34, 56, 78, 90, 11};
    int resultado, i;
    imprimir_vector(V,10);
    for(i = 1; i < 11; i ++) {
        resultado = kesimoMayor(V, 10, i);
        if (resultado != -1) {
        printf("%d, ", resultado);
        }
    }
    
}

////////MEDIR TIEMPOS////////
void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void imprimirCabeceras9() {
    const char* cotaSub;
    const char* cotaAjustada;
    const char* cotaSobre;
    cotaSub = "n^0.7";
    cotaAjustada = "n^0.85";
    cotaSobre = "n^0.9)";
    printf("Cota subestimada: %s\n", cotaSub);
    printf("Cota ajustada: %s\n", cotaAjustada);
    printf("Cota sobrestimada: %s\n", cotaSobre);
    printf("\n");
    printf("%8s\t%24s\tt(n)/%19s\tt(n)/%19s\tt(n)/%19s\n\n",
             "n", "t(n)", cotaSub, cotaAjustada, cotaSobre);
}

void imprimirCabeceras2() {
    const char* cotaSub;
    const char* cotaAjustada;
    const char* cotaSobre;
    cotaSub = "n^0.9 * log(n^0.9)";
    cotaAjustada = "n^1.03 * log(n^1.03)";
    cotaSobre = "n^1.16 * log(n^1.16)";
    printf("Cota subestimada: %s\n", cotaSub);
    printf("Cota ajustada: %s\n", cotaAjustada);
    printf("Cota sobrestimada: %s\n", cotaSobre);
    printf("\n");
    printf("%8s\t%24s\tt(n)/%19s\tt(n)/%19s\tt(n)/%19s\n\n",
             "n", "t(n)", cotaSub, cotaAjustada, cotaSobre);

}

void Cota9(double t, double* x,
                double* y, double* z, int n){
        *x=t/(pow(n,0.7));
        *y=t/(pow(n,0.85));
        *z=t/(pow(n,0.9));
}

void Cota2(double t, double* x,
                double* y, double* z, int n){
        *x=t/(pow(n,0.9) * log(pow(n,0.9)));
        *y=t/(pow(n,1.03) * log(pow(n,1.03)));
        *z=t/(pow(n,1.16) * log(pow(n,1.16)));
}

void medirTiempo9(){
    double ta, tb, t, x, y, z; int n, i, ok; int *v;

    imprimirCabeceras9();
    for(n= 500; n <= 64000; n = n * 2){
        v = malloc(sizeof (int) * n);
        vector_aleatorio(v,n);
        ta = microsegundos();
        kesimoMayor(v,n,9);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            v = malloc(sizeof (int) * n);
            vector_aleatorio(v,n);
            ta = microsegundos();            
            for( i = 1; i <= K; i++ ){
                kesimoMayor(v,n,9);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        Cota9(t,&x,&y,&z,n);
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",n,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",n,t,"*",x, y,z);
        }
        free(v);
    }    
}

void medirTiempo2(){
    double ta, tb, t, x, y, z; int n, i, ok; int *v;

    imprimirCabeceras2();
    for(n= 500; n <= 64000; n = n * 2){
        v = malloc(sizeof (int) * n);
        vector_aleatorio(v,n);
        ta = microsegundos();
        kesimoMayor(v,n,n/2);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            v = malloc(sizeof (int) * n);
            vector_aleatorio(v,n);
            ta = microsegundos();            
            for( i = 1; i <= K; i++ ){
                kesimoMayor(v,n,n/2);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        Cota2(t,&x,&y,&z,n);
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",n,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",n,t,"*",x, y,z);
        }
        free(v);
    }    
}

////////MAIN////////
int main() {
    printf("////////TEST////////\n");
    test();
    printf("\n");
    printf("////////MEDICIONES DE TIEMPOS////////\n");
    printf("////////Medición de tiempos para k = 9////////\n");
    medirTiempo9();
    printf("////////Medición de tiempos para k = n/2////////\n");
    medirTiempo2();
    return 0;
}

