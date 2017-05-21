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
#include <ctime>
#include <thread>
#include <random>
#include <future>
#include <chrono>
#include "RPCClientWrapper.h"

#define VECTOR_SIZE 1000
#define RUN_COUNT 20


std::string addrString;
int port;
int maxThreads;

std::string remoteMethodName;

RPCClientWrapper* rpcClient;

std::vector<int> randomVector;


int test()
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
    std::vector<double> resultArray1;
    testClient->ExecRPC("array.increment", &resultArray1, preArgs);
    std::cout << "Increment operation returned ";
    for(int i = 0; i < resultArray1.size(); i++)
    {
        std::cout << resultArray1[i] << "(was " << preArgs[i+1] << "), ";
    }
    std::cout << std::endl;
    std::vector<double> resultArray2;
    testClient->ExecRPC("array.pow", &resultArray2, preArgs);
    std::cout << "Pow operation returned ";
    for(int i = 0; i < resultArray2.size(); i++)
    {
        std::cout << resultArray2[i] << "(was " << preArgs[i+1] << "), ";
    }
    std::cout << std::endl;
    std::vector<double> resultArray3;
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


std::vector<double> type1RPCThread(int threadId)
{
    std::vector<double> result;
    int blockSize = VECTOR_SIZE/maxThreads;
    rpcClient->ExecRPC(remoteMethodName, &result, randomVector, threadId*blockSize, blockSize);
    return result;

}

double type2RPCThread(int threadId)
{
    double result;
    int blockSize = VECTOR_SIZE/maxThreads;
    rpcClient->ExecRPC(remoteMethodName, &result, randomVector, threadId*blockSize, blockSize);
    return result;
}

int type2RPCThreadParametered(int threadId, int arg)
{
    double result;
    int blockSize = VECTOR_SIZE/maxThreads;
    rpcClient->ExecRPC(remoteMethodName, &result, randomVector, threadId*blockSize, blockSize, arg);
    return (int)result;
}


int main(int const argc, const char ** const argv) {
    
    	/*if (argc != 3) {
		printf("Modo de utilizacao: programa ip porta");
		return -1;
	}

	int port = atoi(argv[1]);*/
    
    // Set server ip and port
    addrString = "localhost";
    port = 8080;
    maxThreads = 2;
    
    // Build connection string
    std::ostringstream addrStringBuilder ;
    addrStringBuilder << "http://" << addrString << ":" << port << "/RPC2";
    std::string SERVER_ADDR = addrStringBuilder.str();
    
    // Seed random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_int_distribution<int> distribution(1,1000);
    
    // Initialize test vector
    randomVector = std::vector<int>(VECTOR_SIZE);
    
    rpcClient = new RPCClientWrapper(SERVER_ADDR.c_str());
    
    std::cout << "Initialized RPC test suite." << std::endl;
    std::cout << "Time is set to average of " << RUN_COUNT << " times." << std::endl;
    std::cout << "Vector size is set to " << VECTOR_SIZE << std::endl;
    
    while(true)
    {
        std::cout << "Choose method to test:" << std::endl;
        std::string userInput;
        std::cout << "1. simple.add" << std::endl;
        std::cout << "2. array.increment" << std::endl;
        std::cout << "3. array.pow" << std::endl;
        std::cout << "4. array.logn" << std::endl;
        std::cout << "5. array.sum" << std::endl;
        std::cout << "6. array.avg" << std::endl;
        std::cout << "7. array.countBigger" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cin >> userInput;
        int selection = std::stoi(userInput);
        
        if (selection == 0) break;
        switch(selection)
        {
            case 1:
            {
                remoteMethodName = "simple.add";
                std::cout << "Not implemented." << std::endl;
                break;
            }
            case 2:
            {
                remoteMethodName = "array.increment";
                break;
            }
            case 3:
            {
                remoteMethodName = "array.pow";
                break;
            }
            case 4:
            {
                remoteMethodName = "array.logn";
                break;
            }
            case 5:
            {
                remoteMethodName = "array.sum";
                break;
            }
            case 6:
            {
                remoteMethodName = "array.avg";
                break;
            }
            case 7:
            {
                remoteMethodName = "array.count>";
                break;
            }
        }
        
        for(int i = 0; i < RUN_COUNT; i++)
        {
            randomVector.resize(VECTOR_SIZE);
            
            // Populate test vector with random numbers between 1 and 1000
            
            for(int j = 0; j < VECTOR_SIZE; j++)
            {
                randomVector[j] = distribution(generator);
            }
            
            if(selection == 2 || selection == 3 || selection == 4)
            {   
                // Initialize threads
                std::vector<std::future<std::vector<double>>> threadPool(maxThreads);
                std::vector<std::vector<double>> results(maxThreads);
    
                // Divide vector between threads and assign rpc call operation
                for(int k = 0; k < maxThreads; k++)
                {
                    threadPool[k] = std::async(&type1RPCThread, k);
                }
    
                // Join threads
                for(int k = 0; k < maxThreads; k++)
                {
                    results[k] = threadPool[k].get();
                }
            }
            
            if(selection == 5 || selection == 6)
            {   
                // Initialize threads
                std::vector<std::future<double>> threadPool(maxThreads);
                std::vector<double> results(maxThreads);
    
                // Divide vector between threads and assign rpc call operation
                for(int k = 0; k < maxThreads; k++)
                {
                    threadPool[k] = std::async(&type2RPCThread, k);
                }
    
                // Join threads
                for(int k = 0; k < maxThreads; k++)
                {
                    results[k] = threadPool[k].get();
                }
    
                //  Finish with rpc call to combine values
                double result = 0;
                rpcClient->ExecRPC(remoteMethodName, &result, results);
    
                //std::cout << "Returned " << result << std::endl;
            }
            
            if (selection == 7)
            {
                // Initialize threads
                std::vector<std::future<int>> threadPool(maxThreads);
                std::vector<int> results(maxThreads);
                
                int param = distribution(generator);
    
                // Divide vector between threads and assign rpc call operation
                for(int k = 0; k < maxThreads; k++)
                {
                    threadPool[k] = std::async(&type2RPCThreadParametered, k, param);
                }
    
                // Join threads
                for(int k = 0; k < maxThreads; k++)
                {
                    results[k] = threadPool[k].get();
                }
    
                //  Finish with rpc call to combine values
                int result = 0;
                for (int k = 0; k < maxThreads; k++)
                {
                    result += results[k];
                }

                //std::cout << "Returned " << result << std::endl;
            }
            
            // Clean up for next run
            randomVector.clear();
        }    
        
        
    }

    return 0;
}

