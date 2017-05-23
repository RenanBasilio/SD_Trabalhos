/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rmi;

import java.io.Serializable;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicLong;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 *
 * @author eduardo
 */
public class ClienteFunctions2 implements Runnable {

    private static AtomicLong total = new AtomicLong(0);
    ArrayFunctions arrayFunctions = null;
    int value;
    List<Byte> list = null;
    int typeFunction;
    long blockSize;

    public ClienteFunctions2(int value, int typeFunction, long blockSize) {
        this.value = value;
        this.typeFunction = typeFunction;
        this.blockSize = blockSize;

        list = Collections.synchronizedList(new ArrayList<Byte>());

        for (int i = 0; i< blockSize; i++){
            list.add((byte) ThreadLocalRandom.current().nextInt(1, 11));
        }

    }

    public String getTotal(){
        return this.total.toString();
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
                    sum = arrayFunctions.arraySum(list);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }

                break;

            case 2:
                try {
                    sum = arrayFunctions.arrayAvarage(list);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }

            case 3:
                try {
                    sum = arrayFunctions.arrayCountBigger(list,value);
                } catch (RemoteException ex) {
                    System.out.println("Error: "  + ex.getMessage());
                }
            default:
                break;
        }

        total.getAndAdd(sum);

    }

    public static void main(String[] args) {
        int vectorSize = 10000000;
        int numThreads = 64;
        int blockSize = vectorSize /numThreads;


        long[] elapsedTimeMillis = new long[20];

        for (int k=0; k<20; k++){

            Thread[] t = new Thread[numThreads];
            for (int i = 0; i < numThreads; i++) {
                t[i] = new Thread(new ClienteFunctions2(5, 1, blockSize));
            }

            long start;

            for (int i = 0; i < numThreads; i++) {
                t[i].start();
            }

            start = System.currentTimeMillis();

            for (int i = 0; i < numThreads; i++) {
                try {
                    t[i].join();
                } catch (InterruptedException ex) {
                    Logger.getLogger(ClienteFunctions2.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            elapsedTimeMillis[k] = System.currentTimeMillis()-start;
        }


        long totalTime = 0;
        for(int i = 0; i< elapsedTimeMillis.length; i++){
            totalTime += elapsedTimeMillis[i];
        }

        System.out.println("Tempo: "  + totalTime/20);
        //If operation == avarage, divide total by threads number
        System.out.println("Total: " + total);


    }
}
