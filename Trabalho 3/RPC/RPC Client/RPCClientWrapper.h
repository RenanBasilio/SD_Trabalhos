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
    xmlrpc_value* RPCall(const std::string methodName, const std::vector<xmlrpc_value*> & args);
    
    std::vector<xmlrpc_value*>  ConvertArray(const std::vector<int> & inVector);
    
    void Parse(const xmlrpc_value * xmlValue, int* returnVar);
    void Parse(const xmlrpc_value * xmlValue, double* returnVar);
    int ParseToInt(const xmlrpc_value * xmlValue);
    
    template<typename T,typename U> void ExecRPC(std::string methodName, std::vector<T> args, U* returnVar)
    {
        std::vector<xmlrpc_value*> rpcArgs = ConvertArray(args);
        xmlrpc_value* callResult = RPCall(methodName, rpcArgs);
        Parse(callResult, returnVar);
        xmlrpc_DECREF(callResult);
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
