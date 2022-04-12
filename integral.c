#include "integral.h"
    
double Trapeze (double left, double right, size_t nsections, double (* Count_func) (double)) {
	double dx = (right - left) / nsections;
	double sum = Count_func (left) + Count_func (right);
	for (ssize_t i = 0; i < nsections - 1; i ++) {
		sum += 2 * Count_func (left + dx * i);
	}
	sum *= dx / 2;
	return sum;
}