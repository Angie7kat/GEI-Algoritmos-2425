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
    int pivote, i, j;
    int x;
    if(iz < dr){
        srand(time(0));
        x = rand() % (dr - iz + 1) + iz;
        pivote=v[x];
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
            ord_rap_aux(v, i, dr);
    }
}

void ord_rap (int v [], int n){
  ord_rap_aux(v,0,n);
}

void test_insercion(){
    int n = 17, v[n];

    printf("\n- Ordenacion Inserción Ascendente:\n");
    vector_ascendente(v,n);
    print_vector(v,n);
    ord_ins(v,n);
    print_vector(v,n);
    printf("\n- Ordenacion Inserción Descendente:\n");
    vector_descendente(v,n);
    print_vector(v,n);
    ord_ins(v,n);
    print_vector(v,n);
    printf("\n- Ordenacion Inserción Aleatoria:\n");
    vector_aleatorio(v,n);
    print_vector(v,n);
    ord_ins(v,n);
    print_vector(v,n);
}

void test_ord_rap(){
    int n = 17, v[n];

    printf("\n- Ordenacion Rápida Ascendente:\n");
    vector_ascendente(v,n);
    print_vector(v,n);
    ord_rap(v,n);
    print_vector(v,n);
    printf("\n- Ordenacion Rápida Descendente:\n");
    vector_descendente(v,n);
    print_vector(v,n);
    ord_rap(v,n);
    print_vector(v,n);
    printf("\n- Ordenacion Rápida Aleatoria:\n");
    vector_aleatorio(v,n);
    print_vector(v,n);
    ord_rap(v,n);
    print_vector(v,n);

}

int main(){
    inicializar_semilla();
    
    printf("\nTest:\n");
    test_insercion();
    printf("\n-----------------------\n");
    test_ord_rap();
}