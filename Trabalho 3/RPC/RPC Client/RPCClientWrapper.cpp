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
    serverInfoP = xmlrpc_server_info_new(&env, serverURL);
    
    xmlrpc_env_init(&env);

    xmlrpc_client_setup_global_const(&env);

    xmlrpc_client_create(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0, &clientP);
    die_if_fault_occurred();
}

RPCClientWrapper::~RPCClientWrapper() {
    /* Clean up our error-handling environment. */
    xmlrpc_env_clean(&env);
    
    xmlrpc_client_destroy(clientP);
    
    xmlrpc_server_info_free(serverInfoP);

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

// Executes RPC call on the current server and returns the unparsed xml string
xmlrpc_value* RPCClientWrapper::RPCall(const std::string methodName, const std::vector<xmlrpc_value*> & data)
{
    xmlrpc_value * vars = xmlrpc_array_new(&env);
    xmlrpc_value * resultP;
    
    /* Parse Variables into Vars Array*/
    for(int i = 0; i < data.size(); i++) xmlrpc_array_append_item(&env, vars, data[i]);
    
    /* Make the remote procedure call */
    xmlrpc_client_call2(&env, clientP, serverInfoP, methodName.c_str(), vars, &resultP);
    die_if_fault_occurred();
    
    xmlrpc_DECREF(vars);
    
    return resultP;

}

// Executes RPC call on the current server and returns the unparsed xml string
xmlrpc_value* RPCClientWrapper::RPCall(
    const std::string methodName, 
    const std::vector<xmlrpc_value*> & data, 
    const std::vector<double> &args)
{
    xmlrpc_value * vars = xmlrpc_array_new(&env);
    xmlrpc_value * resultP;
    
    /* Parse Variables into Vars Array*/
    for(int i = 0; i < args.size(); i++) 
    {
        xmlrpc_array_append_item(&env, vars, xmlrpc_double_new(&env, args[i]));
    }
    for(int i = 0; i < data.size(); i++) xmlrpc_array_append_item(&env, vars, data[i]);
    
    /* Make the remote procedure call */
    xmlrpc_client_call2(&env, clientP, serverInfoP, methodName.c_str(), vars, &resultP);
    die_if_fault_occurred();
    
    xmlrpc_DECREF(vars);
    
    return resultP;

}
