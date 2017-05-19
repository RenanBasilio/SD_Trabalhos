/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OtherRemoteMethods.h
 * Author: renan
 *
 * Created on May 18, 2017, 8:12 PM
 */

#ifndef OTHERREMOTEMETHODS_H
#define OTHERREMOTEMETHODS_H

#include <cstdlib>
#include <iostream>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/util.h>

static xmlrpc_value * simple_add(xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverContext, void * const callInfo) {
    
    xmlrpc_int32 x, y, z;

    /* Parse our argument array. */
    xmlrpc_parse_value(envP, paramArrayP, "(ii)", &x, &y);
    
    //std::cout << "Received values: " << x << " " << y << std::endl;
    
    if (envP->fault_occurred) return NULL;

    /* Add our two numbers. */
    z = x + y;

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", z);
}


#endif /* OTHERREMOTEMETHODS_H */

