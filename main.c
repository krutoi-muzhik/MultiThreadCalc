#include "pthread_cpu.h"

int main (int argc, char *argv[]) {
	int nthreads = 0;

	if (argc < 2)
		nthreads = 1;

	else
		nthreads = atoi (argv[1]);

	if (nthreads < 1) {
		handle_error ("Invalid arg: Number of threads must be > 0\n");
	}

	double sum = calc (nthreads, -10.0, 10.0, 0.0000001);

	printf ("sum \t%lf\n", sum);
	
	exit (EXIT_SUCCESS);
}