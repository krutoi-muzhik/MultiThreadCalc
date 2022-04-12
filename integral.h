#include <math.h>
#include <stdio.h>
#include <pthread.h>

double Trapeze (double left, double right, size_t nsections, double (* Count_func) (double));