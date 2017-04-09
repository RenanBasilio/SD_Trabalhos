// Produtor-Consumidor Multithread.cpp : Defines the entry point for the console application.
//

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
#include <atomic>

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
#define SHARED_MEMORY_SIZE 2
#define PRODUCER_THREAD_COUNT 16
#define CONSUMER_THREAD_COUNT 1
#define STOP_AFTER 10000

// Globals
std::atomic_int runCount = 0;
bool stop = false;

int produce()
{
	while (!stop)
	{
		// Generate a random number
		int number = (rand() % 10000000) + 1; // Random numbers in range [1, 10^7]
		
		// Create local lock manager. This also locks the mutex
		std::unique_lock<std::mutex> lock(mutex);
		// If memory is full, subscribe to condition variable
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

		// Notify consumers of data in memory
		notEmpty.notify_all();

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

		// Create local lock manager. This also locks the mutex.
		std::unique_lock<std::mutex> lock(mutex);
		// Wait until shared memory isn't empty
		while (elementCount == 0 && !stop) notEmpty.wait(lock);

		// If stop flag is raised, exit
		if (stop)
		{
			lock.unlock();
			break;
		}

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

		// Notify producers of free space
		notFull.notify_all();

		//std::cout << "Processing value " << value << std::endl;

		// Once done unlock the mutex
		lock.unlock();

		// Process value retrieved from array
		bool prime = isPrime(value);

		runCount++;
	}

	// Program is done, notify producers to exit
	stop = true;
	notFull.notify_all();
	notEmpty.notify_all();

	return 0;
}

int main()
{
	std::clock_t startTimes[10];
	std::clock_t endTimes[10];
	int leftovers[10];

	for (int i = 0; i < 10; i++)
	{
		// Record program start time
		startTimes[i] = clock();

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

		// Reset global variables
		runCount = 0;
		stop = false;

		// Record program end time
		endTimes[i] = clock();
		leftovers[i] = elementCount;
	}

	double timeAvg = 0;
	double leftoverAvg = 0;
	
	for (int i = 0; i < 10; i++)
	{
		timeAvg += (double)(endTimes[i] - startTimes[i]) / CLOCKS_PER_SEC;
		leftoverAvg += leftovers[i];
	}
	timeAvg = timeAvg / 10;
	leftoverAvg = leftoverAvg / 10;

	std::cout << "Done! Run time: " << timeAvg << " s" << std::endl;
	std::cout << leftoverAvg << " values were left in shared memory ( size: " << SHARED_MEMORY_SIZE << " )" << std::endl;
	getchar();
    return 0;
}

