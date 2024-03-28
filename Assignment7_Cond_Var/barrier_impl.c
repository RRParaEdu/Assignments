#include <stdio.h>
#include <pthread.h>

int pthread_hooks_init();

typedef struct _my_pthread_barrier{
  int max_count;
  int cur_count;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} my_pthread_barrier_t;

my_pthread_barrier_t barrier;

int my_pthread_barrier_init(my_pthread_barrier_t *barrier, int count)
{
  barrier->max_count = count;
  barrier->cur_count = 0;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->cond, NULL);

  return 0;
}

int my_pthread_barrier_wait(my_pthread_barrier_t *barrier)
{
  /*
   * Please implement my_pthread_barrier_wait here using a mutex and
   * a condition variable.
   */
  return 0;
}

void *thread_func(void *p)
{
	int id = *(int*)p;
	printf("Thread %d in 1st phase\n", id);
	my_pthread_barrier_wait(&barrier);
	printf("Thread %d in 2nd phase\n", id);
	my_pthread_barrier_wait(&barrier);
	printf("Thread %d in 3rd phase\n", id);
	
	return NULL;
}

int main()
{
	int ids[4]; 
	pthread_t threads[4];
	int i;

	pthread_hooks_init(); // logging initialization, do not remove.

	/*
	 * initialize the barrier
	 */
	my_pthread_barrier_init(&barrier, 4);
	
	/*
	 * create four threads and pass corresponding idx as parameter
	 */
	for(i = 0; i < 4; i++){
		ids[i] = i;
		pthread_create(&threads[i], NULL, thread_func, &ids[i]);
	}

	for(i = 0; i < 4; i++){
		pthread_join(threads[i], NULL);
	}

	return 0;
}
