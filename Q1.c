#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


int main(int argc, char *argv[]) {
    // set handlers
	if(argc != 2){
		printf("Modo de utilizacao: Q1 PIDdoProcesso");
		return 0;
	}
	if(kill(atoi(argv[1]),SIGHUP)){
		printf("Error: Processo não existe.");
	}
	printf(argv[1]);
    return 0;  
}