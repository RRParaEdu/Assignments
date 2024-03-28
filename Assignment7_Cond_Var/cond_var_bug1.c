/*
 *   This program demonstrates a condition variable race/synchronization 
 *   problem.
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
pthread_mutex_t count_mutex;
pthread_cond_t count_condvar;

int pthread_hooks_init();

/*
 * sub1 thread do some work and wait for the condition
 */
void *producer(void *t)
{
  int tid = *(int*)t;
 
  printf("producer thread %d start\n", tid);
  
  pthread_mutex_lock(&count_mutex);printf("mutex locked\n");

  count++;
  count++;
  printf("Two Produced\n");
  pthread_cond_signal(&count_condvar);printf("cond signal\n");
  pthread_cond_signal(&count_condvar);printf("cond signal\n");  

  pthread_mutex_unlock(&count_mutex);printf("mutex unlocked\n");

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

  pthread_mutex_lock(&count_mutex);printf("mutex locked\n");

  printf("wait on cond\n");
  pthread_cond_wait(&count_condvar, &count_mutex);
  printf("cond wait end\n");
  count--;printf("consumed one\n");
  
  pthread_mutex_unlock(&count_mutex);printf("mutex_unlocked\n");

  printf("consumer thread %d end\n", tid);
  
  return NULL;
}

int main(int argc, char *argv[])
{
  int t1=1, t2=2, t3=3;
  int i;
  pthread_t threads[3];

  pthread_hooks_init();
  printf("Main(): process start\n");

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_condvar, NULL);

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
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_condvar);

  printf("Main(): process end");
  
  return 0;

}

