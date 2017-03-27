/****************** SERVER CODE ****************/

#include <stdio.h>
#include "stdlib.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "unistd.h"


int main(int argc, char *argv[]){

	if(argc != 2){
		printf("Modo de utilizacao: Q3S qtdeItens");
		return 0;
	}	

	
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
  
	int i, delta, numeroEscrito, qtde_itens;
	numeroEscrito = 0;
	qtde_itens = atoi(argv[1]);
	for (i = 1 ; i <= qtde_itens+1; i++) {
		sleep(1);
		srand( (unsigned)time(NULL) );
		delta = 1+ rand()%100;
		numeroEscrito = numeroEscrito + delta;
		
		if(i == qtde_itens+1)
			numeroEscrito = 0;
		
		printf("Produtor criou o %d.\n", numeroEscrito);
		
		sprintf(buffer, "%d", numeroEscrito);
		send(newSocket,buffer,20,0);
		sleep(2);
		recv(newSocket, buffer, 1024, 0);
		printf("Mensagem recebida: %s",buffer);   

	}


  return 0;
}