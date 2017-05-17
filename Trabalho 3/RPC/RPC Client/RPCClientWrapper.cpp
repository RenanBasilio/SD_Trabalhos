/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RPCClientWrapper.cpp
 * Author: renan
 * 
 * Created on May 17, 2017, 12:44 AM
 */

#include "RPCClientWrapper.h"


RPCClientWrapper::RPCClientWrapper(const char* serverURL) {
    /* Initialize our error-handling environment. */
    url = serverURL;
    
    xmlrpc_env_init(&env);

    xmlrpc_client_setup_global_const(&env);

    xmlrpc_client_create(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0, &clientP);
    die_if_fault_occurred();
}

RPCClientWrapper::~RPCClientWrapper() {
    /* Clean up our error-handling environment. */
    xmlrpc_env_clean(&env);
    
    xmlrpc_client_destroy(clientP);

    xmlrpc_client_teardown_global_const();
}

void RPCClientWrapper::die_if_fault_occurred()
{
    /* Check our error-handling environment for an XML-RPC fault. */
    if (env.fault_occurred) {
        std::cerr << "XML-RPC Fault: " << env.fault_string << " " << env.fault_code << std::endl; 
        exit(1);
    }
}

void RPCClientWrapper::ExecCall(std::string methodName)
{
    int sum;
    xmlrpc_value * resultP;
    
    /* Make the remote procedure call */
    xmlrpc_client_call2f(&env, clientP, url, methodName.c_str(), &resultP, "(ii)", (xmlrpc_int32) 5, (xmlrpc_int32) 7);
    die_if_fault_occurred();
    
    /* Get our sum and print it out. */
    xmlrpc_read_int(&env, resultP, &sum);
    die_if_fault_occurred();
    std::cout << "The sum is " << sum << std::endl;
    
    /* Dispose of our result value. */
    xmlrpc_DECREF(resultP);
}

