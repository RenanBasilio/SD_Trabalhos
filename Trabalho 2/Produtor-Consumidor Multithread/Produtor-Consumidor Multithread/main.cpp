// Produtor-Consumidor Multithread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

// Prime verification
#include <isPrime.h>

// Random Number Generator
#include <random>

// Timing
#include <ctime>

// Threading
#include <thread>
#include <mutex>
#include <condition_variable>

// Shared memory
#include <vector>
std::vector<int> sharedMemory;
std::mutex mutex;
std::condition_variable notEmpty;
std::condition_variable notFull;
int elementCount;
int lastPosRead;
int lastPosWritten;


// Defines
#define SHARED_MEMORY_SIZE 32
#define PRODUCER_THREAD_COUNT 1
#define CONSUMER_THREAD_COUNT 1
#define STOP_AFTER 10000

// Globals
int runCount = 0;
bool stop = false;

int produce()
{
	while (!stop)
	{
		// Generate a random number
		int number = (rand() % 10000000) + 1; // Random numbers in range [1, 10^7]
											  // Create local lock manager
		std::unique_lock<std::mutex> lock(mutex);
		// Wait until shared memory isn't full
		while (elementCount == sharedMemory.size() && !stop) notFull.wait(lock);
		// If stop flag is raised, exit
		if (stop)
		{
			lock.unlock();
			break;
		}

		// Once mutex is locked, check last position written. The next position should be empty since the array isnt full
		int myPos = lastPosWritten + 1;
		// If myPos is bigger than array size, cycle over to zero
		if (myPos > sharedMemory.size()-1) myPos = 0;

		// Write random number to next position
		sharedMemory[myPos] = number;

		// Update trackers
		lastPosWritten = myPos;
		elementCount++;

		// If vector was empty, notify consumers
		if (elementCount == 1) notEmpty.notify_all();

		//std::cout << "Produced value " << number << std::endl;

		// Once done unlock the mutex
		lock.unlock();
	}
	return 0;
}


int consume()
{
	while (!stop && runCount <= STOP_AFTER)
	{
		// Create local lock manager
		std::unique_lock<std::mutex> lock(mutex);
		// Wait until shared memory isn't empty
		while (elementCount == 0) notEmpty.wait(lock);

		// Once mutex is locked, check last position read. The next position should be filled since the array isnt empty
		int myPos = lastPosRead + 1;
		// If myPos is bigger than array size, cycle over to zero
		if (myPos > sharedMemory.size()-1) myPos = 0;

		// Copy value from shared memory into local memory
		int value = sharedMemory[myPos];

		// Clear position on shared memory and update trackers
		sharedMemory[myPos] = 0;
		lastPosRead = myPos;
		elementCount--;

		// If vector was full, notify producers
		if (elementCount == sharedMemory.size()-1) notFull.notify_all();

		//std::cout << "Processing value " << value << std::endl;

		runCount++;

		// Once done unlock the mutex
		lock.unlock();

		// Process value retrieved from array
		bool prime = isPrime(value);
	}

	// Program is done, notify producers to exit
	stop = true;
	notFull.notify_all();

	return 0;
}

int main()
{
	// Record program start time
	std::clock_t startTime = clock();

	// Seed random number generator
	srand(time(NULL)); 

	// Initialize shared memory
	sharedMemory.resize(SHARED_MEMORY_SIZE, 0);
	elementCount = 0;
	lastPosRead = -1;
	lastPosWritten = -1;

	// Initialize producer and consumer threads
	std::vector<std::thread> producers(PRODUCER_THREAD_COUNT);
	std::vector<std::thread> consumers(CONSUMER_THREAD_COUNT);
	for (int i = 0; i < producers.size(); i++)
	{
		producers[i] = std::thread(produce);
	}
	for (int i = 0; i < consumers.size(); i++)
	{
		consumers[i] = std::thread(consume);
	}

	// Signal start production
	notFull.notify_all();

	// Wait for producers to finish and join them as they do
	for (int i = 0; i < producers.size(); i++)
	{
		producers[i].join();
	}
	// Wait for consumers to finish and join them as they do
	for (int i = 0; i < consumers.size(); i++)
	{
		consumers[i].join();
	}

	// Record program end time
	std::clock_t endTime = clock();

	std::cout << "Done! Run time: " << (double)(endTime - startTime)/CLOCKS_PER_SEC << " s" << std::endl;
	getchar();
    return 0;
}

