/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3 */
/* Codigo: Calcular aproximação de PI em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int nthreads;
double d15 = 0.000000000000001
double d16 = 0.0000000000000001

double serie(long long int n) {
    double x = pow(-1, double(n))*(1 / (2 * n + 1));

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

    if (argc < 2) {
        print("Digite: %s <tamanho da serie>\n", argv[0]);
        return 1;
    }
    n = atoll(argv[1]);

    GET_TIME(inicio);
    for (long long int i = 0; i < n; i++) {
        pi += serie(n);
    }
    GET_TIME(fim);
    delta = fim - inicio;

    printf("Aproximação de pi: %lf", n);
    printf("Valor exato de pi: %lf", M_PI);
    comparaPI(n);
    printf("Tempo decorrido: %lf", delta);

    return 0;
}
