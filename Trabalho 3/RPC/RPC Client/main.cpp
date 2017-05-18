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
#include "RPCClientWrapper.h"


using namespace std;


int main(int const argc, const char ** const argv) {
    
    const char * SERVER_ADDR = "http://localhost:8080/RPC2";

    RPCClientWrapper* testClient = new RPCClientWrapper(SERVER_ADDR);

    std::string methodName = "sample.add";
    std::vector<int> preArgs;
    preArgs.push_back(5);
    preArgs.push_back(7);
    
    //xmlrpc_value * resultP = testClient->RPCall(methodName, testClient->ConvertArray(preArgs));
    //int sum;
    //testClient->Parse(resultP, &sum);
    
    int sum;
    testClient->ExecRPC(methodName, preArgs, &sum);
    
    std::cout << "The sum is " << sum << std::endl;
    
    /* Dispose of our result value. */
    //xmlrpc_DECREF(resultP);
    
    delete testClient;

    return 0;
}

