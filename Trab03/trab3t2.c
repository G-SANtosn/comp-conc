#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// N e o tamanho do bloco lido.
// M e o tamanho do buffer onde armazeno os numeros do bloco
#define N 20
#define M 5

// Array que armazena o bloco
int* bloco;

// Array buffer
int** buffer;

// Variaveis que indicam se uma thread está lendo o buffer
// 0 -> Thread ainda está lendo o buffer
// 1 -> Thread já leu todo o buffer
int thread0Leu = 1;
int thread1Leu = 1;
int thread2Leu = 1;

// Proxima posicao que irei pegar o numero do bloco pra passar pro buffer
// Quando chega no final do array bloco[], vou ter que pegar outro bloco do arquivo
int posBloco = 0;

// Proxima posicao que irei inserir o numero sendo lido no momento
// Se chegar no final do array buffer[], volto pro inicio e escrevo a partir do inicio
int posBuffer = 0;

// Quantidade de numeros passados pelo arquivo.
// A cada numero lido decremento esse valor. Quando chega a 0 indica que ja li tudo
int tamSequencia;
pthread_cond_t produtor, consumidores;
pthread_mutex_t mutex;

bool terminouLeitura = false;


bool bufferMenosDois() {
	for (int i = 0; i < M; i++) {
		for (int j = 0; i < N; i++) {
			if (buffer[i][j] != -1) {
				return false;
			}
		}
	}

	return true;
}

bool bufferMenosUm() {
	for (int i = 0; i < M; i++) {
		for (int j = 0; i < N; i++) {
			if (buffer[i][j] != -1) {
				return false;
			}
		}
	}

	return true;
}


void* leituraArquivo(void* arg) {
    
	FILE* arquivo = (FILE*)arg;
    
	//printf("Entrou na leitura.\n");

	while (!terminouLeitura) {
		pthread_mutex_lock(&mutex);

        // Se algum desses valores for 0, ainda tem thread lendo o bloco atual, então travo
		while (thread0Leu == 0 || thread1Leu == 0 || thread2Leu == 0) {
			//printf("Travou leitura.\n");
			pthread_cond_wait(&produtor, &mutex);
		}

		pthread_mutex_unlock(&mutex);
		for (int i = 0; i < N; i++) {
            // Leio os números do arquivo e coloco no bloco
			//printf("Vai ler do arquivo e botar no bloco.\n");
			fread(&buffer[posBuffer][i], sizeof(int), 1, arquivo);
			tamSequencia--; // Decremento pois já li o número
			printf("Números restantes: %d\n", tamSequencia);
            
            // So passo o numero pro buffer se for diferente de -2 (explicado no if() logo abaixo)
            // Se eu ja tiver lido o bloco todo, mas ainda tiver "sobrando" posicao do buffer pra ler, buffer[posBuffer][i] recebe -2
            // Se buffer[posBuffer][i] == -2, ignoro
            buffer[posBuffer][i] = bloco[posBloco];
            posBloco = (posBloco + 1) % N;

            // Se voltei pro inicio do bloco, entao li o bloco todo
            // Marco a posicao do bloco como -2, se for -2 nao passo pro buffer, pra evitar elemento repetido
            if (posBloco == 0) {
                for (int j = 0; j < N; j++) {
                    bloco[j] = -2;
                }
            }

            // tamSequencia == 0 -> Li todos os numeros
			if (tamSequencia == 0) {
				terminouLeitura = true;
			}
		}
		pthread_mutex_lock(&mutex);
        // Li numeros novos -> coloco-os no array buffer, entao reseto as variaveis, pois as threads ainda nao leram os numeros novos
		thread0Leu = 0;
		thread1Leu = 0;
		thread2Leu = 0;
		pthread_mutex_unlock(&mutex);
		posBuffer = (posBuffer + 1) % M;
		pthread_cond_broadcast(&consumidores);
	}

	// Se nao tenho mais nada pra ler, tudo que vier depois do ultimo numero lido sera igual a -1, indicando que nao ha mais nada para ler
	pthread_mutex_lock(&mutex);
	int i = 0;
	while (i < N) {
		bloco[i] = -1;
		i++;
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);

}

void* seqMaior() {
    int i;
	int numAtual = -1;
    int seqAtual = 0;
    int posSeqAtual = 0;
    int seqMaior = 0;
    int numMaiorSeq = -1;
    int posMaiorSeq = 0;
    
	while (1) {
		pthread_mutex_lock(&mutex);
		while (thread0Leu == 1) {
            // Thread 0 já leu tudo, então não tem mais nada pra ler. Logo, travo a thread
			pthread_cond_wait(&consumidores, &mutex);
        }

		pthread_mutex_unlock(&mutex);
		//printf("Começou a ler sequencia de identicos.\n");
		for (i = 0; i < N; i++) {
			if ( (tamSequencia == 0) && bufferMenosUm() ) { //Ja li tudo			
				pthread_mutex_lock(&mutex);
				printf("Maior Sequencia de valores idênticos: %d %d %d\n", posMaiorSeq, seqMaior, numMaiorSeq);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

            // Se o elemento do buffer for -2, e porque eu li o bloco todo mas tinha posicao "sobrando" no buffer, entao ignoro pra nao ler repetido
            if (buffer[posBuffer][i] != -2) {
                if (buffer[posBuffer][i] == numAtual) {
                    seqAtual++;
                }
                else {
                    if (seqAtual > seqMaior) {
                        seqMaior = seqAtual;
                        numMaiorSeq = numAtual;
                        posMaiorSeq = posSeqAtual;
                    }
                    numAtual = buffer[posBuffer][i];
                    seqAtual = 1;
                    posSeqAtual = i;
                }
            }
		}

		pthread_mutex_lock(&mutex);
		//printf("Terminou de ler sequencia de identicos.\n");
		thread0Leu = 1; //assinala que leu o bloco do vetor
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	}
}

void* seqTripla() {
	int cont = 0;
    int sequencias = 0;
    int numAtual = -1;

	while (1) {
		pthread_mutex_lock(&mutex);
        // Thread 1 já leu tudo, então não tem mais nada pra ler. Logo, travo a thread
		while (thread1Leu == 1) {
			pthread_cond_wait(&consumidores, &mutex);
        }

		pthread_mutex_unlock(&mutex);
		//printf("Comecou a ler triplas.\n");
		for (int i = 0; i < N; i++) {
			if ( (tamSequencia == 0) && bufferMenosUm() ) { //Ja li tudo
				pthread_mutex_lock(&mutex);
				printf("Quantidade de triplas: %d\n", sequencias);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}

            // Se o elemento do buffer for -2, e porque eu li o bloco todo mas tinha posicao "sobrando" no buffer, entao ignoro pra nao ler repetido
            if (buffer[posBuffer][i] != 2) {
                if (buffer[posBuffer][i] == numAtual) {
                    cont++;
                    if (cont == 3) {
                        sequencias++;
                        cont = 0;
                    }
                }
                else {
                    numAtual = buffer[posBuffer][i];
                    cont = 1;
                }
            }
		}

		pthread_mutex_lock(&mutex);
		//printf("Terminou de ler triplas.\n");
		thread1Leu = 1; //assinala que leu o bloco do vetor
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	}

	pthread_exit(NULL);
}

void* seqCheia() {
	int numAtual = -1;
    int sequencias = 0;

	while (1) {
		pthread_mutex_lock(&mutex);
        // Thread 2 já leu tudo, então não tem mais nada pra ler. Logo, travo a thread
		while (thread2Leu == 1) {
			pthread_cond_wait(&consumidores, &mutex);
        }

		pthread_mutex_unlock(&mutex);
		//printf("Comecou a ler sequencia cheia.\n");
		for (int i = 0; i < N; i++) {
			if ( bufferMenosUm() ) { //Ja li tudo
				pthread_mutex_lock(&mutex);
				printf("Quantidade de ocorrências da sequência <012345>: %d\n", sequencias);
				pthread_mutex_unlock(&mutex);
				pthread_exit(NULL);
			}
            
            // Se o elemento do buffer for -2, e porque eu li o bloco todo mas tinha posicao "sobrando" no buffer, entao ignoro pra nao ler repetido
            if (buffer[posBuffer][i] != -2) {
                if (buffer[posBuffer][i] == numAtual + 1) {
                    numAtual++;
                    if (numAtual == 5)
                        sequencias++;
                }
                else numAtual = -1;
            }
		}

		pthread_mutex_lock(&mutex);
		thread2Leu = 1; //assinala que leu o bloco do vetor
		//printf("Terminou de ler sequencia cheia.\n");
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&produtor); //libera a thread leitora do arquivo caso esteja bloqueada
	}

	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	pthread_t threadsID[4];
	FILE* arquivo;

	//printf("Declaracoes iniciais\n");

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&produtor, NULL);
	pthread_cond_init(&consumidores, NULL);

	//printf("Vai começar\n");

	if (argc != 2)
		printf("Favor digitar o nome do arquivo a ser executado e o arquivo de entrada dos números");

	arquivo = fopen(argv[1], "rb");
	//printf("Passou de arquivo = fopen()\n");

	if (arquivo == NULL)
		printf("Erro ao abrir o arquivo\n");

	//printf("Arquivo nao é NULL\n");

	fread(&tamSequencia, sizeof(int), 1, arquivo);
	//printf("Passou de fread().\n");

    bloco = (int*) malloc(N * sizeof(int));
	buffer = (int**) malloc(M * N * sizeof(int));

	//printf("Alocou bloco e buffer\n");

	for (int i = 0; i < N; i++) {
        bloco[i] = -1;
	}

	for (int i = 0; i < M; i++) {
        buffer[i] = bloco;
	}

    //printf("Encheu bloco e buffer de 1.\n");

	pthread_create(&threadsID[0], NULL, leituraArquivo, (void*)arquivo);
	pthread_create(&threadsID[1], NULL, seqMaior, NULL);
	pthread_create(&threadsID[2], NULL, seqTripla, NULL);
	pthread_create(&threadsID[3], NULL, seqCheia, NULL);

	/* Espera todas as threads completarem */
	for (int i = 0; i < 4; i++) {
		pthread_join(threadsID[i], NULL);
	}
	//printf("\nThreads desligadas.\n");

	fclose(arquivo);
	free(bloco);
	free(buffer);

	return 0;
}