/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   remoteMethod.h
 * Author: renan
 *
 * Created on May 18, 2017, 6:43 PM
 */

#ifndef TYPE1REMOTEMETHODS_H
#define TYPE1REMOTEMETHODS_H


#include <cstdlib>
#include <iostream>
#include "math.h"
#include <cmath>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/util.h>


// Type 1 Functions (Array as Parameter, Array as Return)
/* 
 * This method incrementes every value in an array by the provided amount.
 * Parameter 0 is the amount to increment by, Parameters 1...n are the values in the array
 */
static xmlrpc_value * array_increment(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to increment. Increment Value = ";
    xmlrpc_value * firstItem;
    xmlrpc_double incrementAmount;
    xmlrpc_array_read_item(envP, paramArrayP, 0, &firstItem);
    xmlrpc_read_double(envP, firstItem, &incrementAmount);
    xmlrpc_DECREF(firstItem);
    //std::cout << incrementAmount << "; Elements = [";
    
    xmlrpc_value * returnArray = xmlrpc_array_new(envP);
    
    for(int i = 1; i < xmlrpc_array_size(envP, paramArrayP); i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        xmlrpc_DECREF(element);
        //std::cout << elementDouble << " (";
        elementDouble = elementDouble + incrementAmount;
        
        element = xmlrpc_double_new(envP, elementDouble);
        xmlrpc_array_append_item(envP, returnArray, element);
        xmlrpc_DECREF(element);
        
        //std::cout << elementDouble << "), ";
        
    }
    
    //std::cout << "]" << std::endl;
    return returnArray;
}

/* 
 * This method elevates every value in an array to a power provided
 * Parameter 0 is the power, Parameters 1...n are the values in the array
 */
static xmlrpc_value * array_pow(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to elevate to power. Power Value = ";
    xmlrpc_value * firstItem;
    xmlrpc_double power;
    xmlrpc_array_read_item(envP, paramArrayP, 0, &firstItem);
    xmlrpc_read_double(envP, firstItem, &power);
    xmlrpc_DECREF(firstItem);
    //std::cout << power << "; Elements = [";
    
    xmlrpc_value * returnArray = xmlrpc_array_new(envP);
    
    for(int i = 1; i < xmlrpc_array_size(envP, paramArrayP); i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        xmlrpc_DECREF(element);
        //std::cout << elementDouble << " (";
        elementDouble = pow(elementDouble, power);
        
        if (std::isinf(elementDouble)) elementDouble = 0.0;
        element = xmlrpc_double_new(envP, elementDouble);
        xmlrpc_array_append_item(envP, returnArray, element);
        xmlrpc_DECREF(element);
        
        //std::cout << elementDouble << "), ";
        
    }
    //std::cout << "]" << std::endl;
    return returnArray;
}

/* 
 * This method calculates the logarithm of base provided of every element in the array
 * Parameter 0 is the base, Parameters 1...n are the values in the array
 */
static xmlrpc_value * array_logn(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo)
{
    //std::cout << "Received an array to calculate log. Base = ";
    xmlrpc_value * firstItem;
    xmlrpc_double base;
    xmlrpc_array_read_item(envP, paramArrayP, 0, &firstItem);
    xmlrpc_read_double(envP, firstItem, &base);
    xmlrpc_DECREF(firstItem);
    //std::cout << base << "; Elements = [";
    
    xmlrpc_value * returnArray = xmlrpc_array_new(envP);
    
    for(int i = 1; i < xmlrpc_array_size(envP, paramArrayP); i++)
    {
        xmlrpc_value* element;
        xmlrpc_double elementDouble;
        xmlrpc_array_read_item(envP, paramArrayP, i, &element);
        xmlrpc_read_double(envP, element, &elementDouble);
        xmlrpc_DECREF(element);
        //std::cout << elementDouble << " (";
        elementDouble = log(elementDouble)/log(base);
        
        element = xmlrpc_double_new(envP, elementDouble);
        xmlrpc_array_append_item(envP, returnArray, element);
        xmlrpc_DECREF(element);
        
        //std::cout << elementDouble << "), ";
        
    }
    //std::cout << "]" << std::endl;
    return returnArray;
}


#endif /* TYPE1REMOTEMETHODS_H */

