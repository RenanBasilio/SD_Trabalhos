/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: renan
 *
 * Created on May 16, 2017, 9:09 PM
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include "../thread"
#include "RPCClientWrapper.h"

#define VECTOR_SIZE 1000


using namespace std;


int main()
{
    const char * SERVER_ADDR = "http://localhost:8080/RPC2";

    RPCClientWrapper* testClient = new RPCClientWrapper(SERVER_ADDR);

    std::vector<double> preArgs;
    preArgs.push_back(2.0);
    preArgs.push_back(1.0);
    preArgs.push_back(3.0);
    preArgs.push_back(5.0);
    preArgs.push_back(15.0);
    
    double resultSingle1;
    testClient->ExecRPC("array.sum", &resultSingle1, preArgs);
    std::cout << "Sum returned " << resultSingle1 << std::endl;
    double resultSingle2;
    testClient->ExecRPC("array.avg", &resultSingle2, preArgs);
    std::cout << "Average returned " << resultSingle2 << std::endl;
    int resultSingle3;
    testClient->ExecRPC("array.count>", &resultSingle3, preArgs);
    std::cout << "Count bigger returned " << resultSingle3 << std::endl;
    vector<double> resultArray1;
    testClient->ExecRPC("array.increment", &resultArray1, preArgs);
    std::cout << "Increment operation returned ";
    for(int i = 0; i < resultArray1.size(); i++)
    {
        std::cout << resultArray1[i] << "(was " << preArgs[i+1] << "), ";
    }
    std::cout << std::endl;
    vector<double> resultArray2;
    testClient->ExecRPC("array.pow", &resultArray2, preArgs);
    std::cout << "Pow operation returned ";
    for(int i = 0; i < resultArray2.size(); i++)
    {
        std::cout << resultArray2[i] << "(was " << preArgs[i+1] << "), ";
    }
    std::cout << std::endl;
    vector<double> resultArray3;
    testClient->ExecRPC("array.logn", &resultArray3, preArgs);
    std::cout << "Log operation returned ";
    for(int i = 0; i < resultArray3.size(); i++)
    {
        std::cout << resultArray3[i] << "(was " << preArgs[i+1] << "), ";
    }
    std::cout << std::endl;
    
    
    delete testClient;
    
    return 0;
}


int workingOnIt(int const argc, const char ** const argv) {
    
    	/*if (argc != 3) {
		printf("Modo de utilizacao: programa ip porta");
		return -1;
	}

	int port = atoi(argv[1]);*/
    
    // Set server ip and port
    string addrString = "localhost";
    int port = 8080;
    int maxThreads = 2;
    
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize test vector
    std::vector<int> randomVector(VECTOR_SIZE);
    
    // Populate test vector with random numbers between 1 and 1000
    for(int i = 0; i < VECTOR_SIZE; i++)
    {
        randomVector[i] = rand() % 1000 + 1;
    }
    
    // Build connection string
    std::ostringstream addrStringBuilder ;
    addrStringBuilder << "http://" << addrString << ":" << port << "/RPC2";
    string SERVER_ADDR = addrStringBuilder.str();
    
    // Instantiate RPC client
    RPCClientWrapper* rpcClient = new RPCClientWrapper(SERVER_ADDR.c_str());
    
    // Initialize threads
    std::vector<std::thread*> threadPool(maxThreads);
    std::vector<double> results(maxThreads);
    int blockSize = VECTOR_SIZE/maxThreads;
    
    // Divide vector between threads and assign rpc call operation
    for(int i = 0; i < maxThreads; i++)
    {
        threadPool[i] = new thread([&]{ rpcClient->ExecRPC("array.sum", &results[i], randomVector, i*(blockSize), blockSize); } );
    }
    
    // Join threads
    for(int i = 0; i < maxThreads; i++)
    {
        threadPool[i]->join();
    }
    
    //  Finish with rpc call to combine values
    double result;
    rpcClient->ExecRPC("array.sum", &result, results);
    
    std::cout << "Sum returned " << result << std::endl;

    return 0;
}

