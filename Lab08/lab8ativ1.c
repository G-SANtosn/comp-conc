/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratorio 8 */
/* Codigo: Problema de escritores e leitores usando threads e semaforos em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
//#include "timer.h"

int* matriz;
int* vetor;
int nthreads;
int nleit, nescr;
int zeros;
sem_t em_e, em_l, escr, leit;       //semaforos
int e = 0, l = 0;                   //globais

typedef struct {
    int id;
    int tam;
    //int nthr;
} tArgs;

//--funcoes executada pelas threads


void* escritor(void* arg) {
    tArgs* argumentos = (tArgs*)arg;
    int inicio, fim;
    int tam_bloco = argumentos->tam / nleit;
    inicio = (argumentos->id) * tam_bloco;

    if (argumentos->id < nleit - 1) {
        fim = inicio + tam_bloco;
    }
    else {
        fim = argumentos->tam;
    }

    printf("Thread %d escritora.\n", argumentos->id);
    
    //while (1) {
        sem_wait(&em_e); e++;
        if (e == 1) {
            sem_wait(&leit);
        }
        sem_post(&em_e);
        sem_wait(&escr);
        //escreve...
        for (int i = inicio; i < fim; i++) {
            int num = (rand() % 100) + 1;      //Escreve um número aleatório de 1 a 100
            vetor[i] = num;
            printf("Thread %d escreveu %d na posição %d do vetor.\n", argumentos->id, num, i);
        }
        sem_post(&escr);
        sem_wait(&em_e); e--;
        if (e == 0) {
            sem_post(&leit);
        }
        sem_post(&em_e);
    //}

    pthread_exit(NULL);
}

void* leitor(void* arg) {
    tArgs* argumentos = (tArgs*)arg;
    int inicio, fim;
    int tam_bloco = argumentos->tam / nleit;
    inicio = (argumentos->id - nescr) * tam_bloco;

    if (argumentos->id < nleit - nescr - 1) {
        fim = inicio + tam_bloco;
    }
    else {
        fim = argumentos->tam;
    }

    printf("Thread %d leitora\n", argumentos->id);
    
    //while (1) {
        sem_wait(&leit);
        sem_wait(&em_l); l++;
        if (l == 1) {
            sem_wait(&escr);
        }
        sem_post(&em_l);
        sem_post(&leit);
        //le...
        for (int i = inicio; i < fim; i++) {
            int num = vetor[i];      //Le o numero
            printf("Thread %d leu o número %d na posição %d do vetor.\n", argumentos->id, num, i);
        }
        sem_wait(&em_l); l--;
        if (l == 0) {
            sem_post(&escr);
        }
        sem_post(&em_l);
    //}

    pthread_exit(NULL);
}



//--funcao de inicializacao
int inicializa(int tam) {
    
    vetor = (int*)malloc(sizeof(int) * tam);
    if (vetor == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < 100; i++) {
        vetor[i] = 0;
    }
    
    return 0;
}

//--imprime o vetor
int imprimeVetor(int tam) {
    printf("Vetor:\n");
    for (int i = 0; i < tam; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n\n");
    
    return 0;
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
    int tam = 100;      //tamanho do vetor
    pthread_t *tid;      //identificadores das threads
    tArgs* argumentos;  //identificadores locais das threads e tamanho
    //double inicio, fim, delta;
        
    //Inicializacao
    //GET_TIME(inicio);
    if (argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }
    nthreads = atoi(argv[1]);

    if (nthreads > tam) {
        nthreads = tam;
    }

    nleit = nthreads / 2;
    nescr = nthreads - nleit;

    inicializa(tam);
    //Fim da inicializacao
    //GET_TIME(fim);
    //delta = fim - inicio;
    //printf("Tempo de inicialização: %lf\n", delta);
    imprimeVetor(tam);


    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);
    //em_e, em_l, escr, leit;

    //Alocacao
    //GET_TIME(inicio);
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

    //for (int i = 0; i < nthreads; i++) {
    //    (argumentos + i)->id = i;
    //    (argumentos + i)->tam = tam;
    //    if (pthread_create(tid + i, NULL, escritor, (void*)(argumentos + i))) {
    //        printf("ERRO--pthread_create\n");
    //        return 3;
    //    }
    //}

    for (int i = 0; i < nescr; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->tam = tam;
        if (pthread_create(tid + i, NULL, escritor, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }
    for (int i = nescr; i < nleit + nescr; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->tam = tam;
        if (pthread_create(tid + i, NULL, leitor, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }

    imprimeVetor(tam);
    //Fim da alocação
    //GET_TIME(fim);
    //delta = fim - inicio;
    //printf("Tempo de alocação: %lf\n", delta);

    //printf("Número de zeros: %d\n", zeros);
    //printf("\n");

    //Finalizacao
    //GET_TIME(inicio);
    free(matriz);
    free(argumentos);
    free(tid);
    //GET_TIME(fim);
    //delta = fim - inicio;
    //printf("Tempo de finalização: %lf\n", delta);

    return 0;
}
