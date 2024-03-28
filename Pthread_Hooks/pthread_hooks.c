/*
 * Wrapper functions for Pthread synchronizations. Logs with thread id and CPU
 * timestamp are added before calling the real Pthread functions.
 *
 * GCC's -wrap is used to intercept the calls to Pthread synchronization
 * functions.
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "pthread_hooks.h"

__thread int tid = -1; // thread id
int enable_log = 0; // flag to control if log is enabled or not

// pointer to real pthread functions
extern int __real_pthread_mutex_lock(pthread_mutex_t *mutex);
extern int __real_pthread_mutex_unlock(pthread_mutex_t *mutex);
extern int __real_pthread_cond_wait(pthread_cond_t *restrict cond,
				    pthread_mutex_t *restrict mutex);
extern int __real_pthread_cond_broadcast(pthread_cond_t *cond);
extern int __real_pthread_cond_signal(pthread_cond_t *cond);
extern int __real_pthread_create(pthread_t *restrict thread,
				 const pthread_attr_t *restrict attr,
				 void *(*start_routine)(void *),
				 void *restrict arg);

extern int __real_printf(const char*, ...);
     
int pthread_hooks_init()
{
	char *env_val;

	// set stdout to line buffer so bash pipeline will not buffer
	// so the outputs can be seen even when the program hangs.
	//setlinebuf(stdout);
	setvbuf(stdout, NULL, _IOLBF, 0); //posix standard
	
	// get the thread id for caller (usually main thread)
	tid = gettid();

	// check if logging is enabled.
        env_val = getenv(ENV_ENABLE_PTHREAD_HOOKS);

	if (env_val != NULL && strncmp(env_val, "1", 2) == 0){
		enable_log = 1;
	}

	return 0;
}
	
int __wrap_pthread_mutex_lock(pthread_mutex_t *mutex)
{
	phooks_dprintf(enable_log, "time %llu thread %d: lock mutex 0x%02x\n",
		       rdtsc(), tid, mutex);
	
	return __real_pthread_mutex_lock(mutex);
}

int __wrap_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	phooks_dprintf(enable_log, "time %llu thread %d: unlock mutex %04p\n",
		       rdtsc(), tid, mutex);
	
	return __real_pthread_mutex_unlock(mutex);
}

int __wrap_pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)
{
	phooks_dprintf(enable_log,
		       "time %llu thread %d: wait for cond %p with mutex %p\n",
		       rdtsc(), tid, cond, mutex);
	
	return __real_pthread_cond_wait(cond, mutex);
}


int __wrap_pthread_cond_broadcast(pthread_cond_t *cond)
{
	phooks_dprintf(enable_log,
		       "time %llu thread %d: broadcast for cond %p\n",
		       rdtsc(), tid, cond);
	
	return __real_pthread_cond_broadcast(cond);
}

int __wrap_pthread_cond_signal(pthread_cond_t *cond)
{
	phooks_dprintf(enable_log,
		       "time %llu thread %d: signal for cond %p\n",
		       rdtsc(), tid, cond);
	
	return __real_pthread_cond_signal(cond);
}

int __wrap_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    __real_printf("time %llu thread %d: ", rdtsc(), tid);
    int result = vprintf(format, args);
    va_end(args);
    return result;
}

struct pthread_hooks_thread_data{
	void* original_param;
	void *(*original_routine)(void *);
};

void* pthread_hooks_thread_func(void *param)
{
	struct pthread_hooks_thread_data * p = (struct pthread_hooks_thread_data *)param;
	void * ret_val;

	// get OS thread id
	tid = gettid();

	phooks_dprintf(enable_log,
		       "time %llu thread %d: pthread started\n",
		       rdtsc(), tid);

	ret_val =  p->original_routine(p->original_param);

	// clean up
	free(p);

	return ret_val;
}

int __wrap_pthread_create(pthread_t *restrict thread,
			  const pthread_attr_t *restrict attr,
			  void *(*start_routine)(void *),
			  void *restrict arg)
{
	struct pthread_hooks_thread_data * p = NULL;
	
	phooks_dprintf(enable_log,
		       "time %llu thread %d: pthread create\n",
		       rdtsc(), tid);

	// wrap the thread start routine
	p = malloc(sizeof(struct pthread_hooks_thread_data));
	p->original_param = arg;
	p->original_routine = start_routine;

	return __real_pthread_create(thread, attr, pthread_hooks_thread_func,
				     p);
}
