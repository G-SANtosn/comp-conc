/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define VLENGTH   20 //tamanho maximo do vetor
#define VHALF 10 //tamanho da metade do vetor

typedef struct {
    int thread_id;
    int *vector;
} threArgs;

//--funcao executada pelas threads
void *PrintHello (void *arg) {

  printf("Hello World\n");

  pthread_exit(NULL);
}

void incrementaArray(void* args) {
    threArgs* argumentos = (threArgs*)args;
    int thread_id = argumentos->thread_id;

    for (int i = thread_id; i < VLENGTH; i++) {
        argumentos->vector[i]++;
    }

    free(argumentos);
    pthread_exit(NULL);
}

void printVector(int array[]) {
    printf("[");

    for (int i = 0; i < array.length; i++) {
        print(" %d ", array[i]);
    }

    printf("]\n");
}

//--funcao principal do programa
int main(void) {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  int* vector = malloc(sizeof(int) * VLENGTH);
  for (int i = 0; i < VLENGTH; i++) {
      *(vector + i) = i*4;
  }
  threArgs* argumentos;

  int tamanho;                  //tamanho das partes do vetor
  int resto;                    //resto da divisao do vetor

  printf("Vetor inicial de %d elementos:\n", VLENGTH);
  printVector(vector);

  tamanho = VLENGTH / NTHREADS;
  resto = VLENGTH % NTHREADS;

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Aloca e preenche os elementos para a thread %d\n", thread);
    argumentos = malloc(sizeof(*argumentos));
    if (argumentos == NULL) {
        printf("--ERRO: malloc()\n"); exit(-1);
    }

    argumentos->thread_id = thread;
    
    if ((thread == NTHREADS - 1) && (resto > 0)) {
        argumentos->vector = vector + (tamanho * thread) + resto;
    }
    else {
        argumentos->vector = vector + (tamanho * thread);
    }


    if (pthread_create(&tid_sistema[thread], NULL, incrementaArray, (void*) argumentos)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  for (thread = 0; thread < NTHREADS; thread++) {
      if (pthread_join(tid_sistema[thread], NULL)) {
          printf("--ERRO: pthread_join()\n");
          exit(-1);
      }

  }

  printf("Vetor final de %d elementos:\n", VLENGTH);
  printVector(vector);

  printf("--Thread principal terminou\n");

  pthread_exit(NULL);
}
