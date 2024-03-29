/*
 *   Please implement a chained consumer program using one conditional variable
 *   and one mutex.
 *
 *   There will be one producer producing product1, which will be consumed by a
 *   intermediate consumer. This intermediate consumer produces product2, which
 *   will be consumed by the final consumer.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


/* Define and scope what needs to be seen by everyone */
#define NUM_THREADS  3
int product1 = 0;
int product2 = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_condvar;

int pthread_hooks_init();

/*
 * producer generate product 1
 */
void *producer(void *t)
{
  int tid = *(int*)t;
  sleep(2); // delay product1 generation, do not remove this line.
  
  printf("producer thread %d start\n", tid);
  
  /* TOOD: from here, please add code to use the condition variable to notify
     the intermediate consumer */
  printf("Generating product 1\n");
  product1++;

  printf("producer thread %d end\n", tid);
  
  return NULL;
}

/*
 * intermediate consumer consumes product 1 and produces product 2
 */
void *mid_consumer(void *t) 
{
  int tid = *(int*)t;

  sleep(1); // delay mid consumer, do not remove this line.

  printf("mid consumer thread %d start\n", tid);

  /* TOOD: from here, please add code to wait on the condition variable for
     product 1 and notify the final consumer for product 2*/
  printf("Consuming product 1\n");
  product1--;

  printf("Producing product 2\n");
  product2++;

  printf("mid consumer thread %d end\n", tid);
  
  return NULL;
}

/*
 * final consumer consumes product 2
 */
void *final_consumer(void *t) 
{
  int tid = *(int*)t;

  printf("final consumer thread %d start\n", tid);

  /* TOOD: from here, please add code to wait on the condition variable for
     product 2*/
  printf("Consuming product 2\n");
  product2--;

  printf("final consumer thread %d end\n", tid);
  
  return NULL;
}

int main(int argc, char *argv[])
{
  int t1=1, t2=2, t3=3;
  int i;
  pthread_t threads[3];

  pthread_hooks_init(); // logging initialization, do not remove.
  
  printf("Main(): process start\n");

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_condvar, NULL);

  /* creating consumer and producer threads, do not change this code */
  pthread_create(&threads[0], NULL, producer, (void *)&t1);
  pthread_create(&threads[1], NULL, mid_consumer, (void *)&t2);
  pthread_create(&threads[2], NULL, final_consumer, (void *)&t3);

  /* Wait for all threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Threads finished.\n",
          NUM_THREADS);

  /* Clean up and exit */
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_condvar);

  printf("Main(): process end\n");
  
  return 0;

}

