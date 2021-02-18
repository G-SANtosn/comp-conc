/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Trabalho de Implementação */
/* Codigo: Implementar quicksort usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "timer.h"

int* matriz;
int* vetor;
int nthreads;

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

typedef struct tElems {
    int low;
    int high;
    struct tElems* next;
} tElems;

typedef struct {
    tElems* top;
    tElems* bottom;
} tFila;

typedef struct {
    int id;
} tArgs;

tFila* fila;

//--funcao que adiciona elementos na fila
int push(tFila* fila, int low, int high) {
    printf("Debug: Entrou no metodo push.\n");
    pthread_mutex_lock(&x_mutex);
    printf("Debug: Teste.\n");

    tElems* novo = (tElems*)malloc(sizeof(tElems));
    if (novo == NULL) {
        printf("ERRO--malloc\n");
        return 3;
    }
    printf("Debug: Alocou elemento novo.\n");

    novo->low = low;
    printf("Debug: novo->low = low (%d).\n", (int)low);
    novo->high = high;
    printf("Debug: novo->high = high (%d).\n", (int)high);
    novo->next = NULL;
    printf("Debug: novo->next = NULL.\n");

    if (fila->bottom != NULL) {
        fila->bottom->next = novo;
        printf("Debug: Colocou.\n");
    }
    fila->bottom = novo;
    printf("Debug: Colocou elemento no final da fila.\n");

    //primeiro elemento
    if (fila->top == NULL) {
        fila->top = novo;
        printf("Debug: Colocou elemento no comeco da fila.\n");
    }

    free(novo);
    printf("Debug: push(): Liberou espaco da variavel auxiliar.\n");

    pthread_mutex_unlock(&x_mutex);

    return 4;
}

//--funcao que remove o primeiro elemento da fila
tElems* pop(tFila* fila) {
    printf("Debug: Entrou no metodo pop.\n");
    //pthread_mutex_lock(&x_mutex);
    printf("Debug: Teste.\n");

    if (fila->top == NULL) {
        pthread_mutex_lock(&x_mutex);
        //return NULL;
    }

    printf("Debug: Topo da fila: low = %d e high = %d.\n", (int)fila->top->low, (int)fila->top->high);

    tElems* top = (tElems*)fila->top;
    //tElems* aux = (tElems*)fila->top;
    fila->top = (tElems*)fila->top->next;
    printf("Debug: Pegou um elemento. low = %d e high = %d.\n", (int)top->low, (int)top->high);
    //free(aux);
    //printf("Debug: pop(): Liberou espaço da variavel auxiliar.\n");

    pthread_mutex_unlock(&x_mutex);

    //return (void*)top;
    printf("Saindo do metodo pop().\n");
    return top;
}

//troca elementos
void swap(int i, int j) {
    printf("Debug: swap(): Trocando os valores das posicoes %d e %d.\n", i, j);
    int aux = vetor[i];
    vetor[i] = vetor[j];
    vetor[j] = aux;
    printf("Debug: swap(): Valores de %d e %d trocados com sucesso.\n", i, j);
}

//particao
int partition(int* vetor, int low, int high) {
    int pivot = vetor[high];
    int i = (low - 1);
    printf("Entrou em partition(). low = %d e high = %d.\n", low, high);

    for (int j = low; j <= high - 1; j++) {
        if (vetor[j] <= pivot) {
            i++;
            swap(i, j);
        }
    }
    swap(i + 1, high);

    return (i + 1);
}

//quicksort
void quicksort(int* array, int low, int high) {
    //void* arg

    //tElems* elementos = (tEles*)arg;
    printf("Debug: Comecou quicksort. Elementos de %d a %d do vetor.\n", (int)low, (int)high);

    if (low < high) {
        int pi = partition(array, low, high);
        printf("Debug: Voltou ao quicksort. Particao na posicao %d.\n", pi);

        //tElems t1 = (tElems*)malloc(sizeof(tElems));
        //t1->low = low;
        //t1->high = (pi - 1);
        //quicksort(array, low, pi - 1);
        push(fila, low, pi - 1);

        //tElems t2 = (tElems*)malloc(sizeof(tElems));
        //t2->low = (pi + 2);
        //t2->high = high;
        //quicksort(array, pi + 1, high);
        push(fila, pi + 1, high);
    }
}

//--funcao executada pelas threads
void* executaTarefa(void* arg) {

    //tFila* fila = (tFila*)arg;
    tArgs* argumentos = (tArgs*)arg;
    printf("Thread %d iniciando.\n", argumentos->id);

    //while (fila->top != NULL) {
    while (1) {
        pthread_mutex_lock(&x_mutex);
        printf("Debug: Teste.\n");

        tElems* elemento = pop(fila);
        printf("Thread %d pegou a parte de %d a %d do vetor.\n", argumentos->id, (int)elemento->low, (int)elemento->high);
        quicksort(vetor, elemento->low, elemento->high);
        //quicksort(elemento);

        //free(elemento);

        pthread_mutex_unlock(&x_mutex);

    }

    printf("Thread %d encerrando.\n", argumentos->id);

    pthread_exit(NULL);
}

//--funcao de inicializacao
int inicializa(int tam) {
    vetor = (int*)malloc(sizeof(int) * tam);
    if (vetor == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    printf("Debug: Alocou vetor.\n");

    for (int i = 0; i < tam; i++) {
        vetor[i] = (rand() % 100);
    }
    printf("Debug: Preencheu o vetor com valores aleatórios.\n");

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    return 0;
}

//--imprime o vetor
int imprimeVetor(int tam) {
    printf("Vetor:\n");
    for (int i = 0; i < tam; i++) {
        printf("%d ", vetor[i]);

        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");

    return 0;
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
    int tam;            //tamanho do vetor
    pthread_t* tid;      //identificadores das threads
    //tFila* fila;
    tArgs* argumentos;  //identificadores locais das threads
    double inicio, fim, delta;

    //Inicializacao
    GET_TIME(inicio);
    if (argc < 3) {
        printf("Digite: %s <tamanho do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    tam = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    printf("Debug: Começou.\n");

    if (nthreads > tam) {
        nthreads = tam;
    }

    inicializa(tam);
    //Fim da inicializacao
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicialização: %lf\n", delta);

    imprimeVetor(tam);

    //Alocacao
    GET_TIME(inicio);
    fila = (tFila*)malloc(sizeof(tFila));
    if (fila == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    printf("Debug: Alocou a fila.\n");
    fila->top = NULL;
    fila->bottom = NULL;
    printf("Debug: Elemento vazio na fila.\n");
    push(fila, 0, tam - 1);


    tid = (pthread_t*)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    printf("Debug: Alocou threads.\n");
    argumentos = (tArgs*)malloc(sizeof(tArgs) * nthreads);
    if (argumentos == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    printf("Debug: Alocou argumentos.\n");

    for (int i = 0; i < nthreads; i++) {
        (argumentos + i)->id = i;
        //(argumentos + i)->tam = tam;
        if (pthread_create(tid + i, NULL, executaTarefa, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid + i), NULL);
    }
    //Fim da alocação
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de alocação: %lf\n", delta);


    //Finalizacao
    /* Desaloca variaveis e termina */
    GET_TIME(inicio);
    //free(matriz);
    free(vetor);
    free(fila);
    free(argumentos);
    free(tid);

    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);


    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %lf\n", delta);

    imprimeVetor(tam);

    return 0;
}