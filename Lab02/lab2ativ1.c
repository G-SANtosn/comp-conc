/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Codigo: Multiplicação de matrizes usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

float* matriz;
float* vetor;
float* saida;
int nthreads;

typedef struct {
    int id;
    int dim;
} tArgs;

//--funcao executada pelas threads
void multiplicaMatriz(void *arg) {
    tArgs* argumentos = (tArgs*)arg;
    printf("Thread %d\n", argumentos->id);
    for (int i = argumentos->id; i < argumentos->dim; i+=nthreads) {
        for (int j = 0; j < argumentos->dim; j++) {
            saida[i] += matriz[i * (argumentos->dim) + j] * vetor[j];
        }
    }
    
    pthread_exit(NULL);
}

//--funcao de inicializacao
int inicializa(int dim) {
    matriz = (float*)malloc(sizeof(float) * dim * dim);
    if (matriz == NULL) {
        print("ERRO--malloc\n");
        return 2;
    }

    vetor = (float*)malloc(sizeof(float) * dim);
    if (vetor == NULL) {
        print("ERRO--malloc\n");
        return 2;
    }

    saida = (float*)malloc(sizeof(float) * dim * dim);
    if (saida == NULL) {
        print("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matriz[i * dim + j] = i + j + 1;
        }
        vetor[i] = 1;
        saida[i] = 0;
    }
    
    return 0;
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
    int dim;            //dimensao da matriz de entrada
    pthread_t tid;      //identificadores das threads
    tArgs* argumentos;  //identificadores locais das threads e dimensao
    double inicio, fim, delta;

    //Inicializacao
    GET_TIME(inicio);
    if (argc < 3) {
        print("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
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


    //Alocacao
    GET_TIME(inicio);
    tid = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        print("ERRO--malloc\n");
        return 2;
    }
    argumentos = (tArgs*)malloc(sizeof(tArgs) * nthreads);
    if (argumentos == NULL) {
        print("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < nthreads; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->dim = dim;
        if (pthread_create(tid + i, NULL, multiplicaMatriz, (void*)(argumentos + i)) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    //Fim da alocacao
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de multiplicação: %lf\n", delta);

    printf("Vetor de saida:\n");
    for (int j = 0; j < dim; j++) {
        printf("%.lf", saida[j]);
    }
    printf("\n");

    //Finalizacao
    GET_TIME(inicio);
    free(matriz);
    free(vetor);
    free(saida);
    free(argumentos);
    free(tid);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf\n", delta);

    return 0;
}
