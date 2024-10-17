/*
TITULO: Práctica 2 -> Ordenación por inserción y ordenación rápida
AUTOR1: Ángela Fouz Suárez          LOGIN1: angela.fouz
AUTOR2: Angela Marlene Clemente López       LOGIN2: angela.clemente
GRUPO:2.3	FECHA:19/10/24
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#define K  1000

double microsegundos() {
 struct timeval t;
 if(gettimeofday(&t, NULL) < 0)
  return 0.0;
 return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializar_semilla() {
    srand(time(NULL));
}

void intercambiar(int v[], int x, int y) {
    int aux;
    aux = v[x];
    v[x] = v[y];
    v[y] = aux;
}

void vector_descendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void vector_ascendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = i + 1;
    }
}

void vector_aleatorio(int v[],int n){
    int m=2*n+1, i;
    for (i = 0; i<n; i++){
        v[i]= (rand() % m) - n;
    }
}

void print_vector(int v[], int n) {
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

bool ordenado(int v[], int length) {
	int i;
	for (i = 0; i < length - 1; i++) {
		if (v[i] > v[i + 1])
			return false;
	}
	return true;
}

void ord_ins(int v[], int n) {
    int i, x, j;
    for (i = 0; i < n; i++) {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}

void ord_rap_aux (int v [], int iz, int dr){
    int pivote, i, j, x;
    if(iz < dr){
        srand(time(0));
        x = (rand() % (dr - iz + 1)) + iz;
        pivote = v[x];
        intercambiar(v,iz, x);
        i = iz +1;
        j = dr;
        while( i <= j){
            while(i <= dr && v[i] < pivote){
                i++;
            }
            while(v[j] > pivote){
                j--;
            }
        
            if(i <= j){
                intercambiar(v,i, j);
                i++;
                j--;
            }
        }
        intercambiar(v,iz, j);
        ord_rap_aux(v, iz, j - 1);
        ord_rap_aux(v, j+1, dr);
    }
}

void ord_rap (int v [], int n){
  ord_rap_aux(v,0,n - 1);
}

void test_insercion(){
    int n = 17, v[n];

    printf("\n- Ordenacion Inserción Ascendente:\n");
    printf("Vector ya ordenado ascendentemente:\n");
    vector_ascendente(v,n);
    print_vector(v,n);
    printf("Llamamos a inserción\n");
    ord_ins(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d", ordenado(v,n));

    printf("\n- Ordenacion Inserción Descendente:\n");
    printf("Vector ya ordenado descendentemente:\n");
    vector_descendente(v,n);
    print_vector(v,n);
    printf("Llamamos a inserción\n");
    ord_ins(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d", ordenado(v,n));

    printf("\n- Ordenacion Inserción Aleatoria:\n");
    printf("Vector aleatorio:\n");
    vector_aleatorio(v,n);
    print_vector(v,n);
    printf("Llamamos a inserción\n");
    ord_ins(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d\n", ordenado(v,n));
}

void test_ord_rap(){
    int n = 17, v[n];

    printf("\n- Ordenacion Rápida Ascendente:\n");
    printf("Vector ya ordenado ascendentemente:\n");
    vector_ascendente(v,n);
    print_vector(v,n);
    printf("Llamamos a ordenación rápida\n");
    ord_rap(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d", ordenado(v,n));

    printf("\n- Ordenacion Rápida Descendente:\n");
    printf("Vector ya ordenado descendentemente:\n");
    vector_descendente(v,n);
    print_vector(v,n);
    printf("Llamamos a ordenación rápida\n");
    ord_rap(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d", ordenado(v,n));

    printf("\n- Ordenacion Rápida Aleatoria:\n");
    printf("Vector aleatorio:\n");
    vector_aleatorio(v,n);
    print_vector(v,n);
    printf("Llamamos a ordenación rápida\n");
    ord_rap(v,n);
    print_vector(v,n);
    printf("Está ordenado? %d\n", ordenado(v,n));
}

void escogerAlgoritmo(int v[],int n,int algorithm){
    switch(algorithm){
        case 1: 
            ord_ins(v,n);
            break;
        case 2:
            ord_rap(v,n);
        default:
            break;
    }
}

void escogerVector(int n,int v[], int vector){
    switch (vector){
    case 1:
        vector_ascendente(v,n);
        break;
    case 2:
        vector_descendente(v,n);
        break;
    case 3:
        vector_aleatorio(v,n);
    default:
        break;
    }
}

void medirTiempo(int algorithm, int vect, int v[], int n){
   double ta, tb, t, x, y, z; int i, j, ok;
    for(i = 500; i <= 32000; i = i * 2){
        ta = microsegundos();
        escogerAlgoritmo(v,n,algorithm);
        escogerVector(n,v,vect);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            ta = microsegundos();            
            for( j=1; j<= K; j++ ){
                escogerAlgoritmo(v,n,algorithm);
                escogerVector(n,v,vect);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        switch (algorithm) {
        case 1:
            x = t / (pow(1.1 ,i)); y = t / (pow(((1 + sqrt(5)) / 2), i));
            z = t / (pow(2 ,i));
            break;
        case 2:
            x = t / (pow(i, 0.8)); y = t / i; z = t / (i *log(i));
            break;
        case 3:
            x = t / (sqrt(log(i))); y = t / (log(i)); z = t / (pow(i ,0.5));
            break;
        default:
            break;
        }
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",i,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",i,t,"*",x, y,z);
        }
    }    
}

int main(){
    int n = 17, v[n];
    inicializar_semilla();
    printf("\nTESTS:\n");
    test_insercion();
    printf("\n-----------------------\n");
    test_ord_rap();

    printf("\n");
    printf("%s\n", "Resultados para vector ascencente con inserción:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(1,1,v,n);
    printf("\n");
    printf("%s\n", "Resultados para vector descendente con inserción:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(1,2,v,n);
    printf("\n");
    printf("%s\n", "Resultados para vector aleatorio con inserción:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(1,3,v,n);

    printf("\n");
    printf("%s\n", "Resultados para vector ascendente con ordenación rápida:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(2,1,v,n);
    printf("\n");
    printf("%s\n","Resultados para vector descendente con ordenación rápida:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(2,2,v,n);
    printf("\n");
    printf("%s\n", "Resultados para vector aleatorio con ordenación rápida:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempo(2,3,v,n);
}