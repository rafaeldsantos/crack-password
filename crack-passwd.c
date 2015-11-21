/*
Nome: Rafael Santos
Nome: Davy Souza
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TAM_HASH 256
#define TAM_SENHA 4
#define MAX 1000
/* Obtém o hash a partir de uma senha e coloca o resultado em hash.
   O vetor hash deve ter pelo menos 14 elementos. */
void calcula_hash_senha(const char *senha, char *hash);

char* incrementa_senha(char *senha);
void* testa_senha (void *arg);



typedef struct Thread_arg{
	char hash_alvo[50];
	char senha[MAX][30];
} thread_arg, *ptr_thread_arg;

int num_pthread;
pthread_mutex_t bloqueio;
pthread_attr_t ttr;
pthread_cond_t nao_vazio;

int main(int argc, char *argv[]) {
	int i;
	char senha[TAM_SENHA + 1];

	if (argc != 2) {
		printf("Uso: %s <hash>", argv[0]);
		return 1;
	}

	for (i = 0; i < TAM_SENHA; i++) {
		senha[i] = 'a';
	}

	senha[TAM_SENHA] = '\0';
	//int x=0;
	num_pthread = 0;
	pthread_mutex_init(&bloqueio, NULL);
	pthread_attr_init(&ttr);
	pthread_cond_init(&nao_vazio, NULL);
	pthread_attr_setdetachstate(&ttr, PTHREAD_CREATE_DETACHED);

	while (1) {
		ptr_thread_arg args = malloc(sizeof(thread_arg));

		pthread_mutex_lock(&bloqueio);

		while(num_pthread > 400){
			pthread_cond_wait(&nao_vazio, &bloqueio);
		}
		pthread_mutex_unlock(&bloqueio);
		strcpy(args->hash_alvo,argv[1]);


		int i;
		for(i = 0;i < MAX; i++){
			strcpy( args->senha[i],incrementa_senha(senha));

		}

		pthread_t *thr_testa_senha = malloc(sizeof(pthread_t));

	}
	return 0;
}

void* testa_senha (void *arg) {
char hash_calculado[TAM_HASH + 1];


	pthread_mutex_lock(&bloqueio);
	num_pthread++;
	pthread_mutex_unlock(&bloqueio);

	int i;
	for(i=0;i<MAX;i++){
		ptr_thread_arg testasenha = (ptr_thread_arg) arg;

		calcula_hash_senha(testasenha->senha[i], hash_calculado);
		if (!strcmp(testasenha->hash_alvo, hash_calculado)) {
			printf("Achou! %s\n", testasenha->senha[i]);
			exit(0);
		}
	}
	pthread_mutex_lock(&bloqueio);
	num_pthread--;
	pthread_cond_signal(&nao_vazio);
	pthread_mutex_unlock(&bloqueio);
	return NULL;
}

char* incrementa_senha(char *senha) {
	int i;

	i = TAM_SENHA - 1;

	while (i >= 0) {
		if (senha[i] != 'z') {
			senha[i]++;
			i = -2;
		} else {
			senha[i] = 'a';
			i--;
		}
	}
	if (i == -1 && num_pthread == 0) {
		printf("Não achou!\n");
		exit(1);
		return NULL;
	}
	return senha;
}


void calcula_hash_senha(const char *senha, char *hash) {
	struct crypt_data data;
	data.initialized = 0;
	strcpy(hash, crypt_r(senha, "aa", &data));
}
