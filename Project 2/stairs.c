/*
Stairs crossing problem using pThreads and Semaphores
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#ifndef tNum
// TODO
// #define tNum: which is the maximum number of customers/threads in the system to test
#define tNum 20

// you can also define other constants for your "prevent deadlock" or "prevent starvation" algorithm
#endif

// #define maxStairs: how many customer can be on the stairs at the same time
#ifndef maxStairs
#define maxStairs 6
#endif

// #define maxBatch: how many customers of the same direction can be on a batch
#ifndef maxBatch
#define maxBatch 10
#endif

// TODO
//  create a struct as Thread's data. Contains the thread's ID.
typedef struct _tData_t
{
	int tID;
} tData_t;

// TODO
// define semaphores you will use
sem_t toA, toB, mutex;

/*
 *	the following variables are created to:
 *	1. prevent deadlock
 *	2. prevent starvation
 *	3. allow more than one customer to use the stairs in the same direction in an “efficient” way
	   that you determine
 */
int xingCnt, xedCnt, toAwaitCnt, toBwaitCnt;

/*
 *	flag the crossing direction (0 noDir, 2 upDir, 1 downDir)
 */
int xingDir;

// Customers going toward A (i.e. up) or toward B (i.e. down) respectively
void *a(void *arg);
void *b(void *arg);

// Semaphore wait and signal functions
void semWait(sem_t *sem);
void semSignal(sem_t *sem);

double turnaroundTime[tNum], avgTurnaroundTime;
double responseTime[tNum], avgResponseTime;

int main()
{

	printf("Project 2: Customer crossing problem using pThreads and Semaphores\n");

	printf("*****************\n\n");
	fflush(stdout);

	// TODO
	/*
	 *	initialize your pthread, semaphore and other variables you will need
	 *	please do error check if the program fail to pthread_create()
	 */

	tData_t tData[tNum];
	pthread_t thread[tNum];

	int resultCheck = 0;

	if (sem_init(&mutex, 0, (unsigned int)1) < 0 ||
		sem_init(&toA, 0, (unsigned int)1) < 0 ||
		sem_init(&toB, 0, (unsigned int)1) < 0)
	{
		printf("error! %d", resultCheck);
		exit(EXIT_FAILURE);
	}
	printf("Parent Process PID: %d \n", getpid()); // parent PID

	// TODO
	/*
	 *	do pthread_create() and call your upstair/downstair func
	 */

	for (int i = 0; i < tNum; ++i)
	{
		void *thread_func;	 // the function to call when creating pthreads
		tData[i].tID = i;	 // set thread id to current i value
		if (rand() % 2 == 1) // go up if it's even number
		{
			thread_func = a;
		}
		else
		{
			thread_func = b; // go down if it's odd number
		}

		if ((resultCheck = pthread_create(&thread[i], NULL, thread_func, &tData[i])))
		{
			printf("error in calling pthread_create, %d\n", resultCheck);
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < tNum; ++i)
	{
		if ((resultCheck = pthread_join(thread[i], NULL)))
		{
			printf("error in calling pthread_join, %d\n", resultCheck);
			return EXIT_FAILURE;
		}
	}

	double sumResponseTime = 0;
	double sumTurnaroundTime = 0;
	for (int i = 0; i < tNum; ++i)
	{
		sumResponseTime += responseTime[i];
		sumTurnaroundTime += turnaroundTime[i];
	}

	avgResponseTime = sumResponseTime / tNum;
	avgTurnaroundTime = sumTurnaroundTime / tNum;

	printf("\n The average Turnaroud time for %d customers is %f seconds \n", tNum, avgTurnaroundTime);
	printf("\n The average Response time for %d customers is %f seconds \n", tNum, avgResponseTime);
	return EXIT_SUCCESS;
}

/*
 * Function for the customer to B level
 *
 */
void *b(void *arg)
{

	// Setup timer
	struct timespec arriveTime, startTime, finishTime;
	tData_t *data = (tData_t *)arg;

	int tIDb = data->tID;

	clock_gettime(CLOCK_MONOTONIC_RAW, &arriveTime);
	semWait(&mutex);

	// if people on the stairs reach max, we constanlty release mutex to let customer
	// update xedCounter
	while (xingCnt > maxStairs)
	{
		semSignal(&mutex);
		sleep(1);
		printf("\n Thread: %d crossing count larger than maxStairs \n", tIDb);
		// if the xing direction is the same as the current thread,
		// we need to waken the other direction to unblock the update of global counters.
		if (xingDir == 1)
		{
			semSignal(&toA);
		}
	}
	// TODO
	/*
	 *	process the toB action.
	 *	print logs to show customers status (e.g., "customer is Crossing from A to B",
	 *  "customer to B should wait", “Finished Stairs")
	 */

	printf("****\nThread %d: About to across from A to B \n", tIDb);

	if ((xingDir == 0 || (xingDir == 1)) && (xingCnt < maxStairs) && ((xedCnt + xingCnt) < maxBatch))
	{

		printf("%d is Crossing from A to B\n", tIDb);

		xingDir = 1;
		xingCnt++;

		printf("%d Signaling Mutex\n", tIDb);

		semSignal(&mutex);
	}
	else
	{
		printf("%d is now supposed to wait\n", tIDb);
		toBwaitCnt++;

		// printf("%d is Signalling mutex \n", tIDb);
		// semWait(&mutex);
		semWait(&toB);

		printf("%d was waiting until signal call", tIDb);
		printf("Signal has been called \n");
		toBwaitCnt--;
		xingCnt++;
		xingDir = 1;

		printf("%d is about to cross \n", tIDb);
		printf("Signaling Mutex\n");
		// release the toB lock so that other queuing customer that already got the stair lock could go
		semSignal(&toB);
		semSignal(&mutex);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);

	sleep(1);
	printf("%d Crossing Finished. Waiting for mutex \n", tIDb);
	printf("\n xingCnt: %d, xedCnt: %d, toAwaitCnt: %d, toBwaitCnt: %d, xingDir: %d \n", xingCnt, xedCnt, toAwaitCnt, toBwaitCnt, xingDir);
	semWait(&mutex);
	printf("Mutex Passed\n");
	xedCnt++;
	xingCnt--;

	if (toBwaitCnt != 0 &&
		(xingCnt + xedCnt < maxStairs ||
		 (xedCnt + xingCnt >= maxBatch && toAwaitCnt == 0)))
	{

		printf("%d is Signaling customer Crossing to B\n", tIDb);

		semSignal(&toB);
	}

	else if (xingCnt == 0 &&
			 toAwaitCnt != 0 &&
			 (toBwaitCnt == 0 ||
			  xedCnt + toBwaitCnt >= maxBatch))
	{
		printf("%d is Changing Direction to turn toward A\n", tIDb);
		printf("%d is Signaling customer waiting to cross to A\n", tIDb);

		xingDir = 2;
		xedCnt = 0;
		semSignal(&toA);
	}

	else if (xingCnt == 0 &&
			 toBwaitCnt == 0 &&
			 toAwaitCnt == 0)
	{
		xingDir = 0;
		xedCnt = 0;

		printf("%d: Crossing Direction Reset \n", tIDb);
		semSignal(&mutex);
	}

	else
	{
		printf("%d: Signaling Mutex\n", tIDb);
		semSignal(&mutex);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &finishTime);

	turnaroundTime[tIDb] = (finishTime.tv_nsec - arriveTime.tv_nsec) / 1000000000.0 +
						   (finishTime.tv_sec - arriveTime.tv_sec);
	responseTime[tIDb] = (startTime.tv_nsec - arriveTime.tv_nsec) / 1000000000.0 +
						 (startTime.tv_sec - arriveTime.tv_sec);

	pthread_exit(NULL);
}

/*
 * Function for the customer to A level
 */
void *a(void *arg)
{
	// Setup timer
	struct timespec arriveTime, startTime, finishTime;
	tData_t *data = (tData_t *)arg;

	int tIDa = data->tID;
	clock_gettime(CLOCK_MONOTONIC_RAW, &arriveTime);
	semWait(&mutex);
	// if people on the stairs reach max, we constanlty release mutex to let customer
	// update xedCounter
	while (xingCnt > maxStairs)
	{
		semSignal(&mutex);
		sleep(1);
		printf("\n Thread: %d crossing count larger than maxStairs \n", tIDa);
		// if the xing direction is the same as the current thread,
		// we need to waken the other direction to unblock the update of global counters.
		if (xingDir == 2)
		{
			semSignal(&toB);
		}
	}
	// TODO
	/*
	 *	process the toA action.
	 *	print logs to show customers status (e.g., "customer is Crossing from B to A",
	 *  "customer to A should wait", “Finished Stairs")
	 */

	printf("****\nThread %d: About to across from B to A \n", tIDa);

	if ((xingDir == 0 || (xingDir == 2)) && (xingCnt < maxStairs) && (xedCnt + xingCnt) < maxBatch)
	{

		printf("%d is Crossing from B to A\n", tIDa);

		xingDir = 2;
		xingCnt++;

		printf("%d Signaling Mutex\n", tIDa);

		semSignal(&mutex);
	}
	else
	{
		printf("%d is now supposed to wait\n", tIDa);
		toAwaitCnt++;

		// printf("%d is Signalling mutex \n", tIDa);
		// semWait(&mutex);
		semWait(&toA);

		printf("%d was waiting until signal call.", tIDa);
		printf("Signal has been called \n");
		toAwaitCnt--;
		xingCnt++;
		xingDir = 2;

		printf("%d is about to cross \n", tIDa);
		printf("%d Signaling Mutex\n", tIDa);
		semSignal(&toA);
		semSignal(&mutex);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &startTime);

	sleep(1);
	printf("%d Crossing Finished. Waiting for mutex \n", tIDa);
	printf("\n xingCnt: %d, xedCnt: %d, toAwaitCnt: %d, toBwaitCnt: %d, xingDir: %d \n", xingCnt, xedCnt, toAwaitCnt, toBwaitCnt, xingDir);
	semWait(&mutex);
	printf("Mutex Passed\n");
	xedCnt++;
	xingCnt--;

	if (toAwaitCnt != 0 &&
		(xingCnt + xedCnt < maxStairs ||
		 (xedCnt + xingCnt >= maxBatch && toBwaitCnt == 0)))
	{

		printf("%d: Signaling customer Crossing to A\n", tIDa);

		semSignal(&toA);
	}

	else if (xingCnt == 0 &&
			 toBwaitCnt != 0 &&
			 (toAwaitCnt == 0 ||
			  xedCnt + toBwaitCnt >= maxBatch))
	{
		printf("%d: Changing Direction to turn toward B\n", tIDa);
		printf("Signaling customer waiting to cross to B\n");

		xingDir = 1;
		xedCnt = 0;
		semSignal(&toB);
	}

	else if (xingCnt == 0 &&
			 toBwaitCnt == 0 &&
			 toAwaitCnt == 0)
	{
		xingDir = 0;
		xedCnt = 0;

		printf("%d: Crossing Direction Reset \n", tIDa);
		semSignal(&mutex);
	}

	else
	{
		printf("%d: Signaling Mutex\n", tIDa);
		semSignal(&mutex);
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &finishTime);

	turnaroundTime[tIDa] = (finishTime.tv_nsec - arriveTime.tv_nsec) / 1000000000.0 +
						   (finishTime.tv_sec - arriveTime.tv_sec);
	responseTime[tIDa] = (startTime.tv_nsec - arriveTime.tv_nsec) / 1000000000.0 +
						 (startTime.tv_sec - arriveTime.tv_sec);

	pthread_exit(NULL);
}

// helper functions
/*
 *	Error-checked semaphore wait.
 */
void semWait(sem_t *sem)
{
	if (sem_wait(sem) < 0)
	{
		printf("sem_wait is smaller than 0!");
		exit(EXIT_FAILURE);
	}
}

/*
 *	Error-checked semaphore signal.
 */
void semSignal(sem_t *sem)
{
	if (sem_post(sem) < 0)
	{
		printf("sem_post is smaller than 0!");
		exit(EXIT_FAILURE);
	}
}

/*
 *	Expected test result:
 * Please print logs to show the customers' status for testing. Here is an example:

Project 2: Customer crossing problem using pThreads and Semaphores
*****************

Parent Process PID: 86672
****
Thread 0: About to cross
customer is Crossing from B to A
Crossing Finished
Signaling Mutex
Using Stairs
Finished Stairs . Waiting for mutex
Mutex Passed
Crossing Direction Reset
Signaling Mutex
****
Thread 1: About to cross
customer is Crossing from B to A
Crossing Finished
Signaling Mutex
Using Stairs
Finished Stairs . Waiting for mutex
Mutex Passed
Crossing Direction Reset
Signaling Mutex
****
Thread 2: customer Crossing from A to B
customer is Crossing from A to B
Signaling Mutex
There are 0 customers waiting
Using Stairs
The customer will finish crossing in 1 seconds
Finished Stairs. Waiting for mutex
Mutex Passed
Crossing Direction Reset
****

 */
