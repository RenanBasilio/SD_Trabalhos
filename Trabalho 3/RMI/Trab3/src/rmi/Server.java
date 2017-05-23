/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static java.lang.Math.log;
import static java.lang.Math.pow;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author eduardo
 */
public class Server extends UnicastRemoteObject implements ArrayFunctions{
    public Server() throws RemoteException{
    }

    public static void main(String[] args) {

        try {
            Server server = new Server();
            String localizacao= "//localhost/servico";
            Naming.rebind(localizacao, server);
        } catch (RemoteException ex) {
            System.out.println("Error: " + ex.getMessage());
        } catch (MalformedURLException ex) {
            System.out.println("Error: " + ex.getMessage());;
        }


    }

    public List<Byte> arrayIncrement(List<Byte> listNumbers, int increment) throws RemoteException {
        for(int i=0; i<listNumbers.size(); i++){
            listNumbers.set(i, (byte) (listNumbers.get(i) + increment));
        }
        return listNumbers;
    }

    public List<Byte> arrayPow(int power, List<Byte> listNumbers) throws RemoteException {
        for(int i=0; i<listNumbers.size(); i++){
            listNumbers.set(i, (byte) pow((listNumbers.get(i)), (byte) power));
        }
        return listNumbers;
    }

    public List<Byte>  arrayMultiply( int factor, List<Byte> listNumbers) throws RemoteException {
        for(int i=0; i<listNumbers.size(); i++){
            listNumbers.set(i, (byte) (listNumbers.get(i) * factor));
        }
        return listNumbers;
    }

    public long  arraySum(List<Byte> listNumbers) throws RemoteException {
        long sum = 0;
        for(int i=0; i<listNumbers.size(); i++){
            sum = (long) (sum + listNumbers.get(i));
        }
        return sum;
    }

    public long arrayAvarage(List<Byte> listNumbers) throws RemoteException{
        long sum = 0;
        for(int i=0; i<listNumbers.size(); i++){
            sum = (long) (sum + listNumbers.get(i));
        }

        return sum/listNumbers.size();
    }

    public long arrayCountBigger(List<Byte> listNumbers, int bigger) throws RemoteException {
        long count = 0;
        for(int i=0; i<listNumbers.size(); i++){
            if (listNumbers.get(i) > bigger){
                count++;
            }
        }
        return count;
    }
}
