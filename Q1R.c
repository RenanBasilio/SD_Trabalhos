#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// handler for common signals that terminate process
static void end_handler(int signal) {
    if (signal == SIGINT)
        printf("received SIGINT\n");
    else if (signal == SIGHUP)
        printf("received SIGHUP\n");
    else if (signal == SIGTERM){
        printf("received SIGTERM\n");
		exit(0);
	}

}

int main(int argc, char *argv[]) {
    // set handlers
	if(argc != 2){
		printf("Modo de utilizacao: Q1R modo(blockOUbusy)");
		return 0;
	}
	
    if ((signal(SIGINT, end_handler) == SIG_ERR) || (signal(SIGHUP, end_handler) ==  
            SIG_ERR) || (signal(SIGTERM, end_handler) == SIG_ERR)) {
        printf("Error while setting a signal handler\n");
        exit(EXIT_FAILURE);
    }
	
	
	
	if(!strcmp(argv[1], "block")){
		while (1) {
			pause();
		}
	}else if(!strcmp(argv[1], "busy")) {
		while (1);
	}else{
		printf("Modo inválido. Escolha entre o modo block ou busy.");
	}
	
     // infinite loop
    return 0;  
}