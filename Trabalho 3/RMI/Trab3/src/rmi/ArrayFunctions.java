/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicInteger;

/**
 *
 * @author eduardo
 */
public interface ArrayFunctions extends Remote {

    // Type 1 methods
    List<Byte>  arrayIncrement(List<Byte> listNumbers, int increment) throws RemoteException;
    List<Byte>  arrayPow( int power, List<Byte> list) throws RemoteException;
    List<Byte>  arrayMultiply( int factor, List<Byte> list) throws RemoteException;

    // Type 2 methods
    long arraySum(List<Byte> listNumbers) throws RemoteException;
    long arrayAvarage(List<Byte> listNumbers) throws RemoteException;
    long arrayCountBigger(List<Byte> listNumbers, int bigger) throws RemoteException;

}
