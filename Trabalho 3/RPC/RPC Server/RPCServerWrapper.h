/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RPCWrapper.h
 * Author: renan
 *
 * Created on May 16, 2017, 10:45 PM
 */

#ifndef RPCWRAPPER_H
#define RPCWRAPPER_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/util.h>
#include <xmlrpc-c/abyss.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

class RPCServerWrapper {
public:
    // Constructor
    RPCServerWrapper(int port);
    // Destructor
    ~RPCServerWrapper();
    // Register a method into RPC Server
    int RegisterMethod(std::string mName, const xmlrpc_method2 method);
    // Start RPC Server
    int Start();
private:
    // RPC Server Environment Variables
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;
    int methodCount;
    
    void die_if_fault_occurred ();

};

#endif /* RPCWRAPPER_H */

