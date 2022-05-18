#include <pthread.h>
#include <stdio.h>

int main () {
	printf ("%ld\n", sizeof (pthread_t));
}