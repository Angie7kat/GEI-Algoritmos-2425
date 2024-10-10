/*
TITULO: Práctica 1 -> SUCESION DE FIBONACCI
AUTOR1: Ángela Fouz Suárez          LOGIN1: angela.fouz
AUTOR2: Angela Clemente López       LOGIN2: angela.clemente
GRUPO:2.3	FECHA:28/09/24
*/

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#define K  100000
#define K2   1000

//obtiene la hora actual en microsegundos
double microsegundos() {
 struct timeval t;
 if(gettimeofday(&t, NULL) < 0)
  return 0.0;
 return (t.tv_usec + t.tv_sec * 1000000.0);
}

//fib 1
int fib1(int n){
    if (n < 2) {
      return n;
    } else
      return (fib1(n-1) + fib1(n-2));
}   
   
//fib2
int fib2(int n){
    int i, j;
     i = 1;
    j = 0;
    for(int k = 1; k <= n; k++) {   
        j = i + j;
        i = j - i;
    }
    return j;
}

//fib3
int fib3(int n) {
    int i, j, k, h, t;
    i = 1; j = 0; k = 0; h = 1; t=0;
    while ( n > 0 ) {
        if ((n % 2) != 0) {
            t = j * h;
            j = (i * h) + (j * k) + t;
            i = (i * k) + t;
        }
        t = h * h;
        h = (2 * k * h) + t;
        k = (k * k) + t;
        n = n / 2;
    }
    return j;
}

//Tabla con la validación de los algortimos
void test(){  
    printf("%s\n", "Validacion de algoritmos:");
    printf("%6s\t%6s\t%6s\t%6s\n", "n", "fib1", "fib2", "fib3");
    for(int i = 0; i <= 8; i++){        
        printf("%6d\t%6d\t%6d\t%6d\n", i, fib1(i), fib2(i), fib3(i));
    }
}

void escogerFib(int n,int fibType){
    switch(fibType){
        case 1: 
            fib1(n);
            break;
        case 2:
            fib2(n);
            break;
        case 3:
            fib3(n);
            break;
        default:
            break;
    }
}

void medirTiempoFib(int fibType){
   double ta, tb, t, x, y, z; int n, nFib,nFibMax,nFibP, i, ok;
   (fibType == 1) ? (nFib = 2, nFibMax = 32, nFibP = 2) 
   : (nFib = 1000, nFibMax = 10000000, nFibP = 10);
    for(n=nFib; n <= nFibMax; n = n * nFibP){
        ta = microsegundos();
        escogerFib(n,fibType);
        tb = microsegundos();
        t = tb - ta;
        ok=0;
        if(t < 500){
            ta = microsegundos();            
            for( i=1; i<=K; i++ ){
                escogerFib(n,fibType);
            }
            tb = microsegundos();            
            t = (tb - ta)/K;
            ok=1;
        }
        switch (fibType) {
        case 1:
            x = t / (pow(1.1 ,n)); y = t / (pow(((1 + sqrt(5)) / 2), n));
            z = t / (pow(2 ,n));
            break;
        case 2:
            x = t / (pow(n, 0.8)); y = t / n; z = t / (n *log(n));
            break;
        case 3:
            x = t / (sqrt(log(n))); y = t / (log(n)); z = t / (pow(n ,0.5));
            break;
        default:
            break;
        }
        if(ok == 0){
            printf("%8d\t%24.7f\t%24.7f\t%24.7f\t%24.7f\n",n,t,x,y,z);    
        }else{
            printf("%8d\t%24.7f%s\t%24.7f\t%24.7f\t%24.7f\n",n,t,"*",x, y,z);
        }
    }    
}

int main(){

    test();
    
    printf("\n");
    printf("%s\n", "Resultados para fib1:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(1.1^n)", 
                                    "t(n)/((1 + sqrt(5))/2)", "t(n)/(2^n)");
    medirTiempoFib(1);
    
    printf("\n");
    printf("%s\n", "Resultados para fib2:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n", "n", "t(n)", "t(n)/(n^0.8)",
                                    "t(n)/n", "t(n)/(n *log(n))");
    medirTiempoFib(2);
    
    printf("\n");
    printf("%s\n", "Resultados para fib3:");
    printf("%8s\t%24s\t%24s\t%24s\t%24s\n\n","n","t(n)","t(n)/(sqrt(log(n)))",
                                    "t(n)/(log(n))", "t(n)/(n^0.5)");
    medirTiempoFib(3);
        
    return 0;
}         
