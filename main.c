#include "integral.h"

double func1 (double x) {
	return sin (cos (x)) - 5 * x * x * x * x + 10 * exp (x * x) + x;
}

double func2 (double x) {
	return x;
}

double func3 (double x) {
	return 1;
}

int main (int argc, char *argv[]) {
	double sum = 0.0;
	time_t dt = time (NULL);

	sum = Trapeze (-5.0, 5.0, 10000000, func3);
	printf ("sum = %lf\n", sum);

	dt = time (NULL) - dt;
	printf ("time taken to calculate = %d\n", dt);
	
	exit (EXIT_SUCCESS);
}