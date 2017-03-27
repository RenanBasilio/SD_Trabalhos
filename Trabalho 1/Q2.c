#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <time.h>


#define READ 0
#define WRITE 1
#define TRUE 1

/* protótipos */
void cria_item(int i);
void consome_item(int i);
void produtor(int fd[2], int qtde_itens);
void consumidor(int fd[2]);
int verifica_primo(int num);

int main (int argc, char *argv[]){
	
	if(argc != 2){
		printf("Modo de utilizacao: Q2 qtdeItens");
		return 0;
	}

	int fd[2];

	/* criando pipe */
	pipe (fd);

	/* criando novo processo */
	int pid = fork();

	if (pid == -1) {
		perror("Erro ao criar um novo processo!");
	} else if (pid == 0) {
	/* o novo processo funciona como produtor */
		produtor(fd, atoi(argv[1]));
	} else {
	/* o processo pai funciona como consumidor */
		consumidor(fd);
	}

	return 0;
}

void produtor(int fd[2], int qtde_itens) {
	close(fd[READ]);

	int i, bytesEscritos, delta, numeroEscrito;
	numeroEscrito = 0;
	for (i = 1 ; i <= qtde_itens+1; i++) {
		sleep(1 + rand() % 5);
		srand( (unsigned)time(NULL) );

		delta = 1+ rand()%100;
		numeroEscrito = numeroEscrito + delta;
		
		if(i == qtde_itens+1)
			numeroEscrito = 0;
		
		cria_item(numeroEscrito);
		
		/* escreve no pipe */
		bytesEscritos = write(fd[WRITE], &numeroEscrito, sizeof(int));

		if (bytesEscritos == -1) {
		   perror("Erro de escrita no pipe!");
		}
	}
	close (fd[WRITE]);
}


void consumidor(int fd[2]){
	close (fd[WRITE]);

	int i, bytesLidos;
	while (TRUE) {
		/* lê do pipe */
		bytesLidos = read (fd[READ], &i, sizeof(int));
		sleep(1);
		consome_item(i);
		
		if(i ==0)
			break;

		if (bytesLidos == -1) {
		   perror("Erro de leitura no pipe!");
		} else if (bytesLidos == 0) {
		   break;
		}
	}
	close (fd[READ]);
}

void cria_item(int i){
	printf("Produtor criou o %d.\n", i);
}

void consome_item(int i){
	printf("Consumidor consumiu o %d %s\n", i, verifica_primo(i)?"- primo":"");
}

int verifica_primo(int num){
  int i; 
  int div = 0;
  
  for (i = 1; i <= num; i++) {
    if (num % i == 0) { 
     div++;
    }
  }
  if (div == 2)
    return 1; // primo
  else
    return 0; // nao primo

}