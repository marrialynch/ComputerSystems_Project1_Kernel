/*
Stairs crossing problem using pThreads and Semaphores
*/

#include <pthread.h>  
#include <semaphore.h> 
#include <stdio.h>    
#include <stdlib.h>
#include <unistd.h>	

#ifndef tNum
// TODO
// #define tNum: which is the maximum number of customers/threads in the system to test
// #define maxStairs: how many customer can be on the stairs at the same time
// you can also define other constants for your "prevent deadlock" or "prevent starvation" algorithm
#endif 

//TODO 
// create a struct as Thread's data. Contains the thread's ID.


// TODO
// define semaphores you will use


/* 
 *	the following variables are created to:  
 *	1. prevent deadlock
 *	2. prevent starvation
 *	3. allow more than one customer to use the stairs in the same direction in an “efficient” way
       that you determine
 */
int xingCnt, xedCnt, toAwaitCnt, toBwaitCnt; 

/* 
 *	flag the crossing direction (0 noDir, 1 upDir, 2 downDir)
 */
int xingDir; 

//Customers going toward A (i.e. up) or toward B (i.e. down) respectively
void *a(void *arg);
void *b(void *arg);

//Semaphore wait and signal functions
void semWait(sem_t *sem);
void semSignal(sem_t *sem);


int main(){

 
	printf("Project 2: Customer crossing problem using pThreads and Semaphores\n");
	printf("*****************\n\n");
	fflush(stdout);

	// TODO
	/*
	 *	initialize your pthread, semaphore and other variables you will need
	 *	please do error check if the program fail to pthread_create()
	 */


    printf("Parent Process PID: %d \n", getpid());//parent PID

    // TODO
	/*
	 *	do pthread_create() and call your upstair/downstair func
	 */


	return EXIT_SUCCESS;
}

/*
* Function for the customer to B level
* 
*/
void *b(void *arg){


	// TODO
	/*
	 *	process the toB action. 
	 *	print logs to show customers status (e.g., "customer is Crossing from A to B", 
	 *  "customer to B should wait", “Finished Stairs") 
	 */
	printf("****\nThread %d: customer would like to cross from A to B\n", tIDb);













	pthread_exit(NULL);
}


/*
* Function for the customer to A level
*/
void *a(void *arg){


	// TODO
	/*
	 *	process the toA action. 
	 *	print logs to show customers status (e.g., "customer is Crossing from B to A", 
	 *  "customer to A should wait", “Finished Stairs") 
	 */
	printf("****\nThread %d: customer would like to cross from B to A\n", tIDb);















	pthread_exit(NULL);
}

// helper functions
/*
 *	Error-checked semaphore wait.
 */
void semWait(sem_t *sem) {
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

/*
 *	Error-checked semaphore signal.
 */
void semSignal(sem_t *sem) {
    if (sem_post(sem) < 0) {
        perror("sem_post");
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

