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
#include <vector>
#include <typeinfo>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <xmlrpc-c/util.h>
#include <xmlrpc-c/config.h>

class RPCClientWrapper {
public:
    RPCClientWrapper(const char* serverURL);
    ~RPCClientWrapper();
    xmlrpc_value* RPCall(std::string methodName, std::vector<xmlrpc_value*> args);
    
    std::vector<xmlrpc_value*>  ConvertArray(std::vector<int> inVector);
    
    void Parse(xmlrpc_value * xmlValue, int* returnVar);
    void Parse(xmlrpc_value * xmlValue, double* returnVar);
    int ParseToInt(xmlrpc_value * xmlValue);
    
    template<typename T,typename U> void ExecRPC(std::string methodName, std::vector<T> args, U* returnVar)
    {
        std::vector<xmlrpc_value*> rpcArgs = ConvertArray(args);
        xmlrpc_value* retVal = RPCall(methodName, rpcArgs);
        Parse(retVal, returnVar);
    };
private:
    /* Initialize RPC Client*/
    xmlrpc_env env;
    xmlrpc_client * clientP;
    xmlrpc_server_info * serverInfoP;
    
    const char * url;
    
    void die_if_fault_occurred ();

};

#endif /* RPCCLIENTWRAPPER_H */
