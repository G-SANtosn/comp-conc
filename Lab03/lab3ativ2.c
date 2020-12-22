/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3 */
/* Codigo: Calcular aproximação de PI usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads;
double d15 = 0.000000000000001
double d16 = 0.0000000000000001

double serie(long long int n) {
    double x = pow(-1, double(n)) * (1 / (2 * n + 1));

    if ((x <= d15) and (x > d16)) {
        printf("%lf\n", x);
    }

    return x;
}

void comparaPi(double n) {
    printf("")
        if (n == M_PI) {
            printf("A aproximação é precisa.\n");
        }
        else if (n < M_PI) {
            printf("O número aproximado é menor.\n");
        }
        else {
            printf("O número aproximado é maior.\n");
        }
}

//--funcao principal do programa
int main(void) {
    long long int n;
    double pi = 0;
    double inicio, fim, delta;

    if (argc < 3) {
        print("Digite: %s <tamanho da serie> <numero de threads>\n", argv[0]);
        return 1;
    }
    n = atoll(argv[1]);
    nthreads = atoll(argv[2]);

    if (nthreads > n) {
        nthreads = n;
    }

    GET_TIME(inicio);
    long long int thread;
    for (thread i = 0; i < nthreads; i++) {
        if (pthread_create(double, NULL, serie, long long int)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (long long int i = 0; i < ntheads; i++) {
        pi += pthread_join(double, NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;

    printf("Aproximação de pi: %lf", n);
    printf("Valor exato de pi: %lf", M_PI);
    comparaPI(n);
    printf("Tempo decorrido: %lf", delta);

    return 0;
}
