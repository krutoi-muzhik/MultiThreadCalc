#ifndef PTHREAD_CPU_H

#define PTHREAD_CPU_H

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <ctype.h>
#include <math.h>

#define MAX(a, b) (a) > (b) ? (a) : (b)

#define handle_error(msg)	\
	perror (msg);			\
	exit (EXIT_FAILURE)

typedef struct {
	double sum;

	double step;
	double lower;
	double upper;

	int core_id;
} threadmem_t;

typedef struct {
	int nthreads;
	int nhard;
	int nempty;

	void *ThreadMem;
	size_t MemSize;
} threadinfo_t;

double func1 (double x);
double func2 (double x); 
double func3 (double x);
double calc (int nthreads, double lower, double upper, double step);
threadinfo_t *ThreadInfoInit (int nthreads, double lower, double upper, double step);
void *ThreadFunc (void *arg);
void DumpInfo (const char *pathname, threadinfo_t *ThreadInfo);

#endif // PTHREAD_CPU_H