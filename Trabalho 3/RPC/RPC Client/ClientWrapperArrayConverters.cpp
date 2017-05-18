
#include "RPCClientWrapper.h"

std::vector<xmlrpc_value*> RPCClientWrapper::ConvertArray(const std::vector<int> & inVector)
{
    std::vector<xmlrpc_value*> values(inVector.size());
    for(int i = 0; i < inVector.size(); i++)
    {
        values[i] = xmlrpc_int_new(&env, inVector[i]);
        die_if_fault_occurred();
    }
    return values;
}
