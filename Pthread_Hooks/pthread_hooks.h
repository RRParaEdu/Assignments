#ifndef __PTHREAD_HOOKS_PARALLEL_EDU__
#define __PTHREAD_HOOKS_PARALLEL_EDU__

/*
 * Runtime controlled debug output
 */
#define phooks_dprintf(debug,fmt, ...)					\
	do { if(debug) {fprintf(stderr, fmt ,				\
				## __VA_ARGS__);}} while (0);

/* 
 * read processor time stamp
 */
#if defined(__i386__)
static __inline__ unsigned long long rdtsc(void)
{
      unsigned long long int x;
           __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
                return x;
}
#elif defined(__x86_64__)
static __inline__ unsigned long long rdtsc(void)
{
      unsigned hi, lo;
        __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
          return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif

#define ENV_ENABLE_PTHREAD_HOOKS "ENABLE_PTHREAD_HOOKS"

#endif
