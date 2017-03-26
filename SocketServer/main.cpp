/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: renan
 *
 * Created on March 25, 2017, 3:46 PM
 */

#include <cstdlib>
#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

/*
 * 
 */
int main(int argc, char** argv) {

    int socketHandle;
    
    std::cout << "Initializing generic socket..." << std::endl;
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket initialized with handle " << socketHandle << std::endl;
    
    std::cout << "Configuring socket as server type." << std::endl;
    struct sockaddr_in socketInfo;
    
    int portNumber = 9001;
    
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    socketInfo.sin_port = htons(portNumber);
    
    if( bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in)) < 0)
    {
        close(socketHandle);
        std::cout << "Failed to bind socket." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Configuration finished successfully." << std::endl << "Waiting for incoming connection..." << std::endl;
    listen(socketHandle, 1);
    
    int socketConnection;
    if((socketConnection = accept(socketHandle, NULL, NULL)) < 0)
    {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Client connected!";
    
    close(socketHandle);
    
    return 0;
}

