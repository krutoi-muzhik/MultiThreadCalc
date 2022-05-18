#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

double Trapeze (double left, double right, size_t nsections, double (* Count_func) (double));