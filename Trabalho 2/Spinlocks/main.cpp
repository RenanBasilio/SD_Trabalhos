#include <stdio.h> 
#include <stdlib.h> 

#ifdef __linux__
#include <pthread.h>
#include <sys/time.h>
#elif _WIN32
#include <thread>
#include <ctime>
#endif

#include <vector> 
#include <iostream>

#include <atomic>

using namespace std; 
 
void acquire(struct sum_blocking *v) ; 
void release(struct sum_blocking *v); 
void *sumThread(void *tmp); 
 
// Struct para armazenar a soma total e o booleano de bloqueio da regiao critica
struct sum_blocking{ 
	int sum; 
	atomic_flag held; 
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
#ifdef __linux__
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
#elif _WIN32
	clock_t tval_before, tval_after;
	double tval_result;
	tval_before = clock();
#endif

	//Alocar memoria para a struct
    struct sum_blocking *sb = (struct sum_blocking *) malloc(sizeof(struct sum_blocking *)); ; 
    struct thread_args *args = (struct thread_args *) malloc(sizeof (*args)); 
	
	
    int numThreads = atoi(argv[1]); 
    int numNumbers = atoi(argv[2]); 

#ifdef __linux__
    pthread_t parcels[numThreads]; 
#elif _WIN32
	vector<thread> parcels(numThreads);
#endif
	
	int i; 

    args->sb = sb; 
    //cout << "numero de threads" << numThreads << endl; 
    args->size = numNumbers/numThreads; 
    

	for (i = 0; i < numThreads; i++) {
		//cout << "Criei a thread" << endl;
#ifdef __linux__
		if (pthread_create(&parcels[i], NULL, sumThread, (void *)args)) {
#elif _WIN32
		parcels[i] = thread(sumThread, (void *)args);
		if (!parcels[i].joinable()) {
#endif
            cout << "Nao foi possivel criar a thread" << endl; 
            return -1; 
        } 
    }   
	
    for (i=0;i<numThreads;i++){ 
#ifdef __linux__
        pthread_join(parcels[i], NULL); 
#elif _WIN32
		parcels[i].join();
#endif
    }

	//Obtem o tempo final e armazena a diferenca 
#ifdef __linux__
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	cout << "Tempo gasto: " << tval_result.tv_sec << "." << tval_result.tv_usec << endl;
#elif _WIN32
	tval_after = clock();
	tval_result = (double)(tval_after - tval_before) / CLOCKS_PER_SEC;
	cout << "Tempo gasto: " << tval_result << "s." << endl;
	cout << "Soma total: " << sb->sum << endl;
#endif
	
	//libera a memoria alocada para as struts
	free(sb);
	free(args);

	return 0;
} 
 

void acquire(struct sum_blocking *v) { 
    while(v->held.test_and_set())
        ;
} 

void release(struct sum_blocking *v) { 
    v->held.clear(); 
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
#ifdef __linux__
    pthread_exit(NULL);
#endif
	return 0;
}
