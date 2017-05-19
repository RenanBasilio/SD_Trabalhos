/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Type2RemoteMethods.h
 * Author: renan
 *
 * Created on May 18, 2017, 8:12 PM
 */

// Type 2 Functions (Array as Parameter, Value as Return)

#ifndef TYPE2REMOTEMETHODS_H
#define TYPE2REMOTEMETHODS_H

#include <cstdlib>
#include <iostream>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/util.h>

/*
 * This method adds together all the elements in the array and returns the result
 */
static xmlrpc_value * array_sum(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to add. Elements = [";
    
    double sum = 0.0;
    
    for(int i = 0; i < xmlrpc_array_size(envP, paramArrayP); i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        //std::cout << elementDouble << ", ";
        sum += elementDouble;
    }
    //std::cout << "] Sum = " << sum << std::endl;
    
    xmlrpc_value* retValue = xmlrpc_double_new(envP, sum);
    
    return retValue;
}

/*
 * This method calculates the average of all values in an array
 */
static xmlrpc_value * array_avg(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to average. Elements = [";
    
    double average = 0.0;
    int arraySize = xmlrpc_array_size(envP, paramArrayP);
    
    for(int i = 0; i < arraySize; i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        //std::cout << elementDouble << ", ";
        average += elementDouble;
    }
    average = average/arraySize;
    //std::cout << "] Average = " << average << std::endl;
    
    xmlrpc_value* retValue = xmlrpc_double_new(envP, average);
    
    return retValue;
}


/*
 * This method counts the number of elements in an array that are bigger than a reference value provided
 * Parameter 0 is the reference value, Parameters 1...n are the values in the array
 */
static xmlrpc_value * array_countBigger(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to count elements (bigger). Reference Value = ";
    xmlrpc_value * firstItem;
    xmlrpc_double refValue;
    xmlrpc_array_read_item(envP, paramArrayP, 0, &firstItem);
    xmlrpc_read_double(envP, firstItem, &refValue);
    //std::cout << refValue << "; Elements = [";
    int count = 0;
    
    for(int i = 1; i < xmlrpc_array_size(envP, paramArrayP); i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        //std::cout << elementDouble << ", ";
        if(elementDouble > refValue) count++;
    }
    //std::cout << "] Count = " << count << std::endl;
    
    xmlrpc_value* retValue = xmlrpc_int_new(envP, count);
    
    return retValue;
}


#endif /* TYPE2REMOTEMETHODS_H */

