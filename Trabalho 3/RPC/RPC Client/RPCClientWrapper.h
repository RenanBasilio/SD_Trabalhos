/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RPCClientWrapper.h
 * Author: renan
 *
 * Created on May 17, 2017, 12:44 AM
 */

#ifndef RPCCLIENTWRAPPER_H
#define RPCCLIENTWRAPPER_H

#define NAME "XML-RPC C Test Client"
#define VERSION "1.0"

#include <stdarg.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <xmlrpc-c/util.h>
#include <xmlrpc-c/config.h>

class RPCClientWrapper {
public:
    RPCClientWrapper(const char* serverURL);
    ~RPCClientWrapper();
    void ExecCall(std::string methodName);
    //int ExecCall(std::string methodName, ...);
private:
    /* Initialize RPC Client*/
    xmlrpc_env env;
    xmlrpc_client * clientP;
    const char * url;
    
    void die_if_fault_occurred ();

};

#endif /* RPCCLIENTWRAPPER_H */

