/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RPCWrapper.cpp
 * Author: renan
 * 
 * Created on May 16, 2017, 10:45 PM
 */

#include "RPCServerWrapper.h"

RPCServerWrapper::RPCServerWrapper(int port) {
    std::cout << "Initializing server at port " << port << std::endl;
    methodCount = 0;
    xmlrpc_env_init(&env);
    registryP = xmlrpc_registry_new(&env);
    //serverparm.config_file_name = argv[1];
    serverparm.port_number = port;
}

RPCServerWrapper::~RPCServerWrapper()
{
    
}

void RPCServerWrapper::die_if_fault_occurred()
{
    /* Check our error-handling environment for an XML-RPC fault. */
    if (env.fault_occurred) {
        std::cerr << "XML-RPC Fault: " << env.fault_string << " " << env.fault_code << std::endl; 
        exit(1);
    }
}

int RPCServerWrapper::RegisterMethod(std::string mName, const xmlrpc_method2 method)
{
    struct xmlrpc_method_info3 const methodInfo = {
        .methodName     = mName.c_str(),
        .methodFunction = method,
    };
    xmlrpc_registry_add_method3(&env, registryP, &methodInfo);
    die_if_fault_occurred();
    
    methodCount++;
    std::cout << "Registered method " << mName << " with server." << std::endl;
    
    return 0;
}

int RPCServerWrapper::Start()
{
    if(methodCount == 0)
    {
        std::cerr << "Failed to start server. Cannot start with 0 registered methods." << std::endl;
        return -1;
    }
    
    std::cout << "Starting XML-RPC server..." << std::endl;
    
    serverparm.registryP = registryP;
    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(registryP));
    die_if_fault_occurred();
    
    std::cout << "Server Exited." << std::endl;
    
    return 0;
}

