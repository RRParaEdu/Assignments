/*
 *   This program demonstrates a condition variable racing/synchronization 
 *   problem.
 *
 *   Please correct this program to fix the race condition.
 *
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


/* Define and scope what needs to be seen by everyone */
#define NUM_THREADS  3
int count = 0;
pthread_mutex_t mutex;
pthread_cond_t condvar;

int pthread_hooks_init();

/*
 * sub1 thread do some work and wait for the condition
 */
void *producer(void *t)
{
  int tid = *(int*)t;
 
  printf("producer thread %d start\n", tid);
  
  pthread_mutex_lock(&mutex);

  count++;
  count++;
  printf("Two Produced\n");
  pthread_cond_signal(&condvar);
  pthread_cond_signal(&condvar);

  pthread_mutex_unlock(&mutex);

  printf("producer thread %d end\n", tid);
  
  return NULL;
}

/*
 * sub2 thread do some work and signal the condition
 */
void *consumer(void *t) 
{
  int tid = *(int*)t;

  printf("consumer thread %d start\n", tid);

  pthread_mutex_lock(&mutex);

  pthread_cond_wait(&condvar, &mutex);
  count--;
  printf("consumed one\n");
  
  pthread_mutex_unlock(&mutex);

  printf("consumer thread %d end\n", tid);
  
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
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init (&condvar, NULL);

  /* creating two sub2 threads and one sub1 thread */
  pthread_create(&threads[0], NULL, producer, (void *)&t1);
  pthread_create(&threads[1], NULL, consumer, (void *)&t2);
  pthread_create(&threads[2], NULL, consumer, (void *)&t3);

  /* Wait for all threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Threads finished.\n",
          NUM_THREADS);

  /* Clean up and exit */
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&condvar);

  printf("Main(): process end\n");
  
  return 0;

}

