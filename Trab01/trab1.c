/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Trabalho de Implementação */
/* Codigo: Conta o número de zeros em uma matriz usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int* matriz;
int nthreads;
int zeros;

typedef struct {
    int id;
    int dim;
} tArgs;

//--funcao executada pelas threads
void* contaZeros(void* arg) {
    tArgs* argumentos = (tArgs*)arg;
    printf("Thread %d\n", argumentos->id);
    for (int i = argumentos->id; i < argumentos->dim; i += nthreads) {
        for (int j = 0; j < argumentos->dim; j++) {
            if (matriz[i * (argumentos->dim) + j] == 0) {
                zeros += 1;
            }
        }
    }

    pthread_exit(NULL);
}

//--funcao de inicializacao
int inicializa(int dim) {
    zeros = 0;

    matriz = (int*)malloc(sizeof(int) * dim * dim);
    if (matriz == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matriz[i * dim + j] = (i + j) % 3;
        }
    }
    
    return 0;
}

//--imprime a matriz
int imprimeMatriz(int dim) {
    printf("Matriz:\n");
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matriz[i * dim + j] = (i + j) % 3;
            printf("%d ", matriz[i * dim + j]);
        }
        printf("\n");
    }
    printf("\n\n");
    
    return 0;
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
    int dim;            //dimensao da matriz de entrada
    pthread_t *tid;      //identificadores das threads
    tArgs* argumentos;  //identificadores locais das threads e dimensao
    double inicio, fim, delta;
        
    //Inicializacao
    GET_TIME(inicio);
    if (argc < 3) {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    if (nthreads > dim) {
        nthreads = dim;
    }

    inicializa(dim);
    //Fim da inicializacao
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicialização: %lf\n", delta);

    imprimeMatriz(dim);

    //Alocacao
    GET_TIME(inicio);
    tid = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    argumentos = (tArgs*)malloc(sizeof(tArgs) * nthreads);
    if (argumentos == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < nthreads; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->dim = dim;
        if (pthread_create(tid + i, NULL, contaZeros, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    //Fim da alocação
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de alocação: %lf\n", delta);

    printf("Número de zeros: %d\n", zeros);
    printf("\n");

    //Finalizacao
    GET_TIME(inicio);
    free(matriz);
    free(argumentos);
    free(tid);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf\n", delta);

    return 0;
}
