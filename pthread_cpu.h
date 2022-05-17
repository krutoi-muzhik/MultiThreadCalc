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

typedef struct {
	double sum;

	double lower;
	double upper;

	int CPUnum;
} threadinfo_t;

#endif // PTHREAD_CPU_H