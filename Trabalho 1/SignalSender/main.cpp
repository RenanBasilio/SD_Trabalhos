/* 
 * File:   main.cpp
 * Author: renan
 *
 * Created on March 25, 2017, 3:11 PM
 */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    pid_t thispid = getpid();
    pid_t pid;
    int signal;
    
    std::cout << "Signal Sender v0.1" << std::endl;
    std::cout << "Running with PID=" << thispid << std::endl << std::endl;
    
    std::cout << "Enter process ID to send signal to: ";
    std::cin >> pid;
    
    std::cout << "Enter signal to send: ";
    std::cin >> signal;
    
    std::cout << "Sending signal " << signal << " to process " << pid << "..." << std::endl;
    
    if( 0 != kill(pid, signal)) 
    {
        std::cout << "Could not send signal. Process does not exist.";
        return 1;
    }
    
    std::cout << "Signal sent successfully.";
    return 0;
}

