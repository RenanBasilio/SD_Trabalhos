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
#include <thread>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include <xmlrpc-c/util.h>
#include <xmlrpc-c/config.h>

class RPCClientWrapper {
public:
    RPCClientWrapper(const char* serverURL);
    ~RPCClientWrapper();
    xmlrpc_value* RPCall(
        const std::string methodName, 
        const std::vector<xmlrpc_value*> &data);
    xmlrpc_value* RPCall(
        const std::string methodName, 
        const std::vector<xmlrpc_value*> &data,
        const std::vector<double> &args);
    
    void Parse(const xmlrpc_value * xmlValue, int* returnVar);
    void Parse(const xmlrpc_value * xmlValue, double* returnVar);
    
    int ParseToInt(const xmlrpc_value * xmlValue);
    
    template <typename T> 
    void Parse(
        const xmlrpc_value* xmlValue, 
        std::vector<T>* returnVar)
    {
        for(int i = 0; i < xmlrpc_array_size(&env, xmlValue); i++)
        {
            xmlrpc_value* element;
            T parsedElement;
            xmlrpc_array_read_item(&env, xmlValue, i, &element);
            die_if_fault_occurred();
            Parse(element, &parsedElement);
            returnVar->push_back(parsedElement);
            xmlrpc_DECREF(element);
        }
    };
    
    template<typename T> 
    std::vector<xmlrpc_value*>  ConvertArray(
        const std::vector<T> &inVector, 
        const int startPos = 0, 
        const int blockSize = -1)
    {
        int stopPos;
        if (blockSize == -1) stopPos = inVector.size();
        else stopPos = startPos + blockSize;
        
        std::vector<xmlrpc_value*> values(stopPos - startPos);
        for(int i = startPos; i < stopPos; i++)
        {
            values[i-startPos] = xmlrpc_double_new(&env, (double)inVector[i]);
            die_if_fault_occurred();
        }
        return values;
    }
    
    
    template<typename T,typename U> 
    int ExecRPC(
        std::string methodName,
        U* returnVar, 
        std::vector<T> &data,
        const int arrayStartPos = 0, 
        const int arrayBlockSize = -1,
        const int argCount = 0,
        ...)
    {
        
        std::vector<xmlrpc_value*> rpcArgs = ConvertArray(data, arrayStartPos, arrayBlockSize);
        
        xmlrpc_value* callResult;
        if(argCount > 0)
        {
            va_list exArgs;
            va_start(exArgs, argCount);
            std::vector<double> args(argCount);
            for(int i = 0; i < argCount; i++) args[i] = (double)va_arg(exArgs, int);
            va_end(exArgs);
            
            callResult = RPCall(methodName, rpcArgs, args);
        }
        else callResult = RPCall(methodName, rpcArgs);
        
        for(int i = 0; i < rpcArgs.size(); i++) xmlrpc_DECREF(rpcArgs[i]);
        
        Parse(callResult, returnVar);
        xmlrpc_DECREF(callResult);
        return 0;
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
