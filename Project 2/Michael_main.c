#include <pthread.h>  
#include <semaphore.h>
#include <stdio.h>    
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#define NUM_THREADS 5
//sem_t staircase;
pthread_mutex_t staircase;

//direction 1 is going up, 0 is going down
struct thread_args {
    int id;
    int direction;
};

const char* direction(int i) {
    if (i==0) {
        return "down";
    } else if (i == 1) {
        return "up";
    }
    return "";

}

void *perform_work(void *arguments){
  struct thread_args *thread_args = ((struct thread_args *)arguments);
  int sleep_time = 1 + rand() % NUM_THREADS;
  printf("THREAD %d: Wanting to go %s.\n", thread_args->id, direction(thread_args->direction));
  //sem_wait(&staircase); 
  pthread_mutex_lock(&staircase);
  printf("THREAD %d: Going %s in %d seconds.\n", thread_args->id, direction(thread_args->direction), sleep_time);
  sleep(sleep_time);
  pthread_mutex_unlock(&staircase);
  //sem_post(&staircase); 
  printf("THREAD %d: Ended.\n", thread_args->id);
  return NULL;
}

int main(void) {
  pthread_t threads[NUM_THREADS];
  struct thread_args *thread_args[NUM_THREADS];
  int i;
  for (i = 0; i < NUM_THREADS; i++) {
    thread_args[i] = (struct thread_args *)malloc(sizeof(struct thread_args));
  }
  
  int result_code;
  

  //sem_init(&staircase, 0, 1);
  pthread_mutex_init(&staircase, NULL);
  //create all threads one by one
  for (i = 0; i < NUM_THREADS; i++) {
    printf("IN MAIN: Creating thread %d.\n", i);
    thread_args[i]->id = i;
    thread_args[i]->direction = rand() % 2;
    result_code = pthread_create(&threads[i], NULL, perform_work, thread_args[i]);
    assert(!result_code);
  }

  printf("IN MAIN: All threads are created.\n");

  //wait for each thread to complete
  for (i = 0; i < NUM_THREADS; i++) {
    result_code = pthread_join(threads[i], NULL);
    assert(!result_code);
    printf("IN MAIN: Thread %d has ended.\n", i);
  }

  printf("MAIN program has ended.\n");
  return 0;
}