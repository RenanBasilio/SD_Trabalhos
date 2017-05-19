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

#include <cstdlib>
#include <iostream>
#include "RPCServerWrapper.h"
#include "Type1RemoteMethods.h"
#include "Type2RemoteMethods.h"
#include "OtherRemoteMethods.h"

#define LISTEN_PORT 8080


int main (int const argc, const char ** const argv) {

    RPCServerWrapper* rpcServer = new RPCServerWrapper(LISTEN_PORT);
    
    // Register other methods
    rpcServer->RegisterMethod("simple.add", &simple_add);
    
    // Register type 1 methods
    rpcServer->RegisterMethod("array.increment", &array_increment);
    rpcServer->RegisterMethod("array.pow", &array_pow);
    rpcServer->RegisterMethod("array.logn", &array_logn);
    
    // Register type 2 methods
    rpcServer->RegisterMethod("array.sum", &array_sum);
    rpcServer->RegisterMethod("array.avg", &array_avg);
    rpcServer->RegisterMethod("array.count>", &array_countBigger);

    rpcServer->Start();
    
    return 0;
}

