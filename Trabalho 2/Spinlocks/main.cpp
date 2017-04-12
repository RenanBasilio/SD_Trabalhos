#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <vector> 
#include <iostream>
#include <sys/time.h>

using namespace std; 
 
bool test_and_set(bool *flag); 
void acquire(struct sum_blocking *v) ; 
void release(struct sum_blocking *v); 
void *sumThread(void *tmp); 
 
// Struct para armazenar a soma total e o booleano de bloqueio da regiao critica
struct sum_blocking{ 
	int sum; 
	bool held; 
}; 
 
//struct com os argumentos que serao passados para a thread
struct thread_args { 
	//tamanho do vector aleatorio
	long int size; 
	struct sum_blocking *sb; 
}; 

int main(int argc, char *argv[]){
	
	if(argc != 3){
		printf("Modo de utilizacao: spinlocks numThreads qtdNumbers");
		return -1;
	}
	
	//Medir tempo
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);

	//Alocar memoria para a struct
    struct sum_blocking *sb = (struct sum_blocking *) malloc(sizeof(struct sum_blocking *)); ; 
    struct thread_args *args = (struct thread_args *) malloc(sizeof (*args)); 
	
	
    int numThreads = atoi(argv[1]); 
    int numNumbers = atoi(argv[2]); 
    pthread_t parcels[numThreads];   
	
	int i; 

    args->sb = sb; 
    //cout << "numero de threads" << numThreads << endl; 
    args->size = numNumbers/numThreads; 
    

    for (i=0;i<numThreads;i++){ 
        //cout << "Criei a thread" << endl; 
        if(pthread_create(&parcels[i],NULL,sumThread,(void *) args)){ 
            cout << "Nao foi possivel criar a thread" << endl; 
            return -1; 
        } 
    }   
	
    for (i=0;i<numThreads;i++){ 
        pthread_join(parcels[i], NULL); 
    }

	//Obtem o tempo final e armazena a diferenca 
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);

	cout <<"Tempo gasto: " << tval_result.tv_sec << "." << tval_result.tv_usec << endl;     
	cout <<"Soma total: " << sb->sum << endl;; 
	
	//libera a memoria alocada para as struts
	free(sb);
	free(args);

	return 0;
} 
 
bool test_and_set(bool *flag) { 
    bool previous = *flag; 
    *flag = true; 
    return previous; 
} 

void acquire(struct sum_blocking *v) { 
    while(test_and_set(&v->held))
        ;
} 

void release(struct sum_blocking *v) { 
    v->held = 0; 
} 
 
void *sumThread(void *args){ 
    struct thread_args *threadArgs = (struct thread_args*) args; 
    long int numNumbers =  threadArgs->size; 
    long int sum = 0;
	
	
    vector<int8_t> randomNumbers;  
    randomNumbers.reserve(numNumbers);
     
	srand(time(NULL)); 

	//Gera os numeros aleatorios e os armazena no vector
    for( int i = 0; i<numNumbers; i++){ 
      randomNumbers[i] = (int8_t) (-100 + rand() % 201); 
      sum += randomNumbers[i];
    } 
	
    acquire(threadArgs->sb); 
	//Regiao critica - adiciona a somaparcial a soma total da struct
      threadArgs->sb->sum += sum; 
    release(threadArgs->sb); 
	
	//Termina a chamada da thread
    pthread_exit(NULL); 
}