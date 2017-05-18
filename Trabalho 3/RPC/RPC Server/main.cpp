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

#define LISTEN_PORT 8080


static xmlrpc_value * sample_add(xmlrpc_env *   const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo) {
    
    xmlrpc_int32 x, y, z;

    /* Parse our argument array. */
    xmlrpc_parse_value(envP, paramArrayP, "(ii)", &x, &y);
    
    std::cout << "Received values: " << x << " " << y << std::endl;
    
    if (envP->fault_occurred) return NULL;

    /* Add our two numbers. */
    z = x + y;

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", z);
}



int main (int const argc, const char ** const argv) {

    RPCServerWrapper* testServer = new RPCServerWrapper(LISTEN_PORT);
    
    testServer->RegisterMethod("sample.add", &sample_add);

    testServer->Start();
    
    return 0;
}

