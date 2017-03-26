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

/*
 * 
 */
int main(int argc, char** argv) {

    int socketHandle;
    
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in remoteSocketInfo;
    struct hostent *hPtr;
    std::string serverAddress = "127.0.0.1"; // Loopback
    int portNumber = 9001;
    
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    remoteSocketInfo.sin_port = htons(portNumber);
    
    if (connect(socketHandle, (struct sockaddr *) &remoteSocketInfo, sizeof(remoteSocketInfo)) < 0);
    {
        close(socketHandle);
        std::cout << "Could not connect to server." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server.";
    
    close(socketHandle);
    
    return 0;
}