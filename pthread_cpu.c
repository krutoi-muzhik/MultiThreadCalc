#include "pthread_cpu.h"

double func1 (double x) {
	return sin (cos (x)) - 5 * x * x * x * x + 10 * exp (x * x) + x;
}

double func2 (double x) {
	return x;
}

double func3 (double x) {
	return 1;
}

double calc (int nthreads, double lower, double upper, double step) {
	if (upper < lower) {
		handle_error ("Invalid limits: upper must be >= lower\n");
	}

	threadinfo_t *ThreadInfo = ThreadInfoInit (nthreads, lower, upper, step);

	pthread_t *threads = (pthread_t *) calloc (nthreads, sizeof (pthread_t));
	if (threads == NULL) {
		handle_error  ("calloc error\n");
	}

	for (int i = 0; i < nthreads; i ++) {
		if (pthread_create (threads + i, NULL, ThreadFunc, ThreadInfo->ThreadMem + i * ThreadInfo->MemSize) != 0) {
			handle_error ("pthread_create ERROR\n");
		}
	}

	double sum = 0;

	for (size_t i = 0; i < nthreads; i ++) {
		if (pthread_join (threads[i], NULL) != 0) {
			handle_error ("pthread_join ERROR\n");
		}

		if (i < ThreadInfo->nhard) {
			threadmem_t *Mem = (threadmem_t *)(ThreadInfo->ThreadMem + i * ThreadInfo->MemSize);
			sum += Mem->sum; 
		}
	}

	printf ("sum \t%lf\n", sum);
}

threadinfo_t *ThreadInfoInit (int nthreads, double lower, double upper, double step) {
	if (nthreads < 1) {
		handle_error ("ThreadInfoConstruct ERROR: number of threads must be > 0\n");
	}

	threadinfo_t *ThreadInfo = (threadinfo_t *) calloc (1, sizeof (threadinfo_t));

	int CacheLineSize = sysconf (_SC_LEVEL1_DCACHE_LINESIZE);
	ThreadInfo->MemSize = (sizeof (threadmem_t) / CacheLineSize + 1) * CacheLineSize;

	printf ("memsize = %ld\n", ThreadInfo->MemSize);
	ThreadInfo->ThreadMem = calloc (nthreads, ThreadInfo->MemSize);

	ThreadInfo->nhard = get_nprocs ();
	if (ThreadInfo->nhard < 1) {
		handle_error ("get_nprocs ERROR: Got number of threads < 0\n");
	}

	ThreadInfo->nempty = (ThreadInfo->nhard < nthreads) ? (nthreads - ThreadInfo->nhard) : (0);

	double len = (upper - lower) / nthreads;

	for (size_t i = 0; i < nthreads; i ++) {
		threadmem_t *ThreadMem = (threadmem_t *)(ThreadInfo->ThreadMem + ThreadInfo->MemSize * i);
		ThreadMem->lower = lower + len * i;
		ThreadMem->upper = lower + len * (i + 1);
		ThreadMem->step = step;
		ThreadMem->core_id = i % ThreadInfo->nhard;
	}

	for (size_t i = nthreads; i < nthreads + ThreadInfo->nempty; i ++) {
		threadmem_t *ThreadMem = (threadmem_t *)(ThreadInfo->ThreadMem + ThreadInfo->MemSize * i);
		ThreadMem->lower = lower;
		ThreadMem->upper = upper;
		ThreadMem->step = step;
		ThreadMem->core_id = -1;		
	}

	return ThreadInfo;
}

void *ThreadFunc (void *arg) {
	threadmem_t *Mem = (threadmem_t *) arg;

	if (Mem->core_id < 0)
		return NULL;

	if (Mem->core_id >= 0) {
		cpu_set_t CPU;
		pthread_t threadid = pthread_self ();

		CPU_ZERO (&CPU);
		CPU_SET (Mem->core_id, &CPU);

		if (pthread_setaffinity_np (threadid, sizeof (cpu_set_t), &CPU) < 0) {
			handle_error ("pthread_setaffinity_np\n");
		}
	}

	for (double x = Mem->lower; x < Mem->upper; x += Mem->step) {
		Mem->sum += func1 (x) * Mem->step;
	}

	return NULL;
}

void DumpInfo (const char *pathname, threadinfo_t *ThreadInfo) {
	FILE *dumpfile = fopen (pathname, "w+");
	threadinfo_t *info = NULL;

	for (size_t i = 0; i < ThreadInfo->nthreads; i ++) {
		threadmem_t *Mem = (threadmem_t *) (ThreadInfo->ThreadMem + i * ThreadInfo->MemSize);
		fprintf (dumpfile, "threadID: \t%ld \tcoreID: \t%d \tlower: \t%.4lf \tupper: \t%.4lf \tsum: \t%.4lf\n",
					i, Mem->core_id, Mem->lower, Mem->upper, Mem->sum);
	}

	fclose (dumpfile);
	return;
}
