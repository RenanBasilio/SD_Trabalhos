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

#define PORT "9001"

#include <cstdlib>
#include <string>
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
#include <string.h>
#include <string>

/*
 * 
 */
int main(int argc, char** argv) {

    int socketHandle;
    
    std::cout << "Initializing generic socket..." << std::endl;
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket initialized with handle " << socketHandle << std::endl;
    
    std::cout << "Configuring socket as client type." << std::endl;
    struct sockaddr_in remoteSocketInfo;
    const char * serverAddress = "127.0.0.1";
    int portNumber = 9006;
    
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_addr.s_addr = inet_addr(serverAddress);
    remoteSocketInfo.sin_port = htons(portNumber);
    
    std::cout << "Attempting to connect to server." << std::endl;
    if (connect(socketHandle, (struct sockaddr *) &remoteSocketInfo, sizeof(sockaddr_in)) < 0)
    {
        close(socketHandle);
        std::cout << "Could not connect to server at " << inet_ntoa(remoteSocketInfo.sin_addr) << ":" << portNumber << "(" << strerror(errno) << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server." << std::endl;
    
    int nrand = 0;
    std::cout << "Enter number of random numbers to generate: " << std::endl;
    std::cin >> nrand;
    
    int number = 0;
    for (int i = 0; i < nrand; i++)
    {
        int rc = 0;
        int buffSize = 20;
        char outBuffer[buffSize];
        char inBuffer[buffSize];
        
        number = rand()%100 + number;
        
        std::cout << "Generated " << number << ". ";
        
        std::string outstring = std::to_string(number);
        strcpy(outBuffer, outstring.c_str());
    
        send(socketHandle, outBuffer, buffSize, 0);
        
        rc = recv(socketHandle, inBuffer, buffSize, MSG_WAITALL);
        inBuffer[rc] = (char)NULL;
        
        std::cout << "Server response: " << inBuffer << std::endl;
    }
    char outBuffer[20];
    strcpy(outBuffer, "0");
    send(socketHandle, outBuffer, 20, 0);
    
    close(socketHandle);
    
    return 0;
}