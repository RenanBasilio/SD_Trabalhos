/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi;

import static com.sun.org.apache.xalan.internal.lib.ExsltDynamic.map;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.ConcurrentHashMap;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author eduardo
 */
public class ClienteFunctions1 implements Runnable {
    static ConcurrentHashMap<Integer, List<Byte>> mapList = new ConcurrentHashMap<Integer, List<Byte>>();

    ArrayFunctions arrayFunctions = null;
    int value;
    List<Byte> list = null;
    int typeFunction;
    int subListIndex;
    int endArray;

    public ClienteFunctions1(List<Byte> list, int value, int typeFunction,
                             int subListIndex) {
        this.list=  list;
        this.value = value;
        this.typeFunction = typeFunction;
        this.subListIndex = subListIndex;
    }

    @Override
    public void run() {

        long sum = 0;
        String localization = "//localhost/service";

        try {
            arrayFunctions = (ArrayFunctions) Naming.lookup(localization);
        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }
        switch(typeFunction){
            case 1:

                try {
                    list = arrayFunctions.arrayIncrement(list, value);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }
                break;

            case 2:
                try {
                    list = arrayFunctions.arrayMultiply(value, list);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }

            case 3:
                try {
                    list = arrayFunctions.arrayPow(value, list);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }
            default:
                break;
        }
        mapList.put(subListIndex, list);
    }


    public static void main(String[] args) {
        int vectorSize = 1000000;
        int numThreads = 1;
        int blockSize = vectorSize /numThreads;

        List<Byte> listNumbers = Collections.synchronizedList(new ArrayList<Byte>());
        for (int i = 0; i< vectorSize; i++){
            listNumbers.add((byte) ThreadLocalRandom.current().nextInt(1, 11));
        }

        ClienteFunctions1[] clienteFunctions1s= new ClienteFunctions1[numThreads];
        int i;
        int j = 0;
        List<Byte> subList = null;

        long[] elapsedTimeMillis = new long[20];
        for (int k=0; k<20; k++) {

            Thread[] t = new Thread[numThreads];
            for (i = 0; i < numThreads; i++) {
                subList = new ArrayList<Byte>(listNumbers.subList(j, j + blockSize));
                t[i] = new Thread(new ClienteFunctions1(subList, 2, 3, i));
                j += blockSize;
            }

            long start = System.currentTimeMillis();

            for (i = 0; i < numThreads; i++) {
                t[i].start();
            }

            for (i = 0; i < numThreads; i++) {
                try {
                    t[i].join();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ClienteFunctions1.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

            List<Byte> listB = Collections.synchronizedList(new ArrayList<Byte>());
            for (int m = 0; m < numThreads; m++) {
                listB.addAll(mapList.get(m));
            }

            elapsedTimeMillis[k] = System.currentTimeMillis() - start;
        }

        long totalTime = 0;
        for(int m = 0; m< elapsedTimeMillis.length; m++){
            totalTime += elapsedTimeMillis[m];
        }

        System.out.println("Tempo: "  + totalTime/20);
    }

}
