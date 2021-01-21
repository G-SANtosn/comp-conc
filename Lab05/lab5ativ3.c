/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório 5 */
/* Codigo: Controla a ordem de execução das threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

int* vetor;
int nleit;
int nescr;
float media;

typedef struct {
    int id;
    int tam;
} tArgs;

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


/* Thread Leitora */
void* leitor(void* arg) {
    tArgs* argumentos = (tArgs*)arg;
    //printf("Thread %d\n", argumentos->id);

    int inicio, fim;
    int tam_bloco = argumentos->tam / nleit;
    inicio = (argumentos->id) * tam_bloco;
    
    if (argumentos->id < nleit - 1) {
        fim = inicio + tam_bloco;
    }
    else {
        fim = argumentos->tam;
    }

    for (int i = inicio; i < fim; i++) {
        media += vetor[i];
    }

    pthread_exit(NULL);
}

/* Thread Escritora */
void* escritor(void* arg) {
    tArgs* argumentos = (tArgs*)arg;
    //printf("Thread %d\n", argumentos->id);

    int inicio, fim;
    int tam_bloco = argumentos->tam / nleit;
    inicio = (argumentos->id - nleit) * tam_bloco;

    if (argumentos->id < nleit + nescr - 1) {
        fim = inicio + tam_bloco;
    }
    else {
        fim = argumentos->tam;
    }

    for (int j = inicio; j < fim; j++) {
        if ((j == 0) || (j == argumentos->tam - 1)) {
            vetor[j] = argumentos->id;
        }
        else {
            vetor[j] = argumentos->id * 2;
        }
    }

    pthread_exit(NULL);
}

int inicializa(int tam) {
    media = 0;

    vetor = (int*)malloc(sizeof(int) * tam);
    if (vetor == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < tam; i++) {
        vetor[i] = i;
    }

    return 0;
}

void printVetor(int tam) {
    for (int i = 0; i < tam; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

//--funcao principal do programa
int main(int argc, char* argv[]) {
    int i;
    int tam;              //tamanho do vetor
    pthread_t* tid;      //identificadores das threads
    tArgs* argumentos;  //identificadores locais das threads e tamanho
    
    if (argc < 4) {
        printf("Digite: %s <tamanho do vetor> <numero de threads leitoras> <numero de threads escritoras>\n", argv[0]);
        return 1;
    }
    tam = atoi(argv[1]);
    nleit = atoi(argv[2]);
    nescr = atoi(argv[3]);

    if (tam < 2) {
        tam = 2;
    }

    if (nleit < 2) {
        nleit = 2;
    }
    else if (nleit > tam) {
        nleit = tam;
    }

    if (nescr < 2) {
        nescr = 2;
    }
    else if (nescr > tam) {
        nescr = tam;
    }

    inicializa(tam);

    printf("Vetor inicial: ");
    printVetor(tam);

    
    tid = (pthread_t*)malloc(sizeof(pthread_t) * (nleit + nescr));
    if (tid == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }
    argumentos = (tArgs*)malloc(sizeof(tArgs) * (nleit + nescr));
    if (argumentos == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    for (int i = 0; i < nleit; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->tam = tam;
        if (pthread_create(tid + i, NULL, leitor, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }
    for (int i = nleit; i < nleit + nescr; i++) {
        (argumentos + i)->id = i;
        (argumentos + i)->tam = tam;
        if (pthread_create(tid + i, NULL, escritor, (void*)(argumentos + i))) {
            printf("ERRO--pthread_create\n");
            return 3;
        }
    }

    media = media / tam;
    printf("Média: %f\n", media);

    printVetor(tam);

    
    /* Espera todas as threads completarem */
    for (i = 0; i < nleit + nescr; i++) {
        pthread_join(*(tid + i), NULL);
    }



    /* Desaloca variaveis e termina */
    free(vetor);
    free(tid);
    free(argumentos);
    return 0;
}
