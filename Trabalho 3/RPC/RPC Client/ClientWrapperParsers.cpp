
#include "RPCClientWrapper.h"

void RPCClientWrapper::Parse(xmlrpc_value* xmlValue, int* returnVar)
{
    xmlrpc_read_int(&env, xmlValue, returnVar);
    die_if_fault_occurred();
}

void RPCClientWrapper::Parse(xmlrpc_value* xmlValue, double* returnVar)
{
    xmlrpc_read_double(&env, xmlValue, returnVar);
    die_if_fault_occurred();
}

int RPCClientWrapper::ParseToInt(xmlrpc_value* xmlValue)
{
    int value;
    
    /* Get our sum and print it out. */
    xmlrpc_read_int(&env, xmlValue, &value);
    die_if_fault_occurred();
    
    return value;
}

