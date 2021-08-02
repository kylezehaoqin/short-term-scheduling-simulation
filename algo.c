/* algo.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

// Exponential distribution pseudo random number generator function
double next_exp()
{
	return drand48();
}

// returns characters from A-Z by converting ASCII int value to char
// requires: n in range [1, 26]
char get_process_id( int n )
{
	int base = 64;
	return (char)(base + n);
}

// returns elapsed time in milliseconds
double timediff( struct timespec end, struct timespec start )
{
	double elapsed;
	elapsed = (double)(end.tv_sec - start.tv_sec) * 1000 + \
						(double)(end.tv_nsec - start.tv_nsec) / 1000000;
	return (int)elapsed;
}

void processes_init( int n, int seed, double lambda, int upper_bound, int t_cs,
									 double alpha, int time_slice )
{
	srand48( seed );

	double rand;
	int arrival_time;
	int cpu_bursts;
	int cpu_burst_time;
	int io_bursts;
	int io_burst_time;
	double tau;

	int i;
	for ( i = 0; i < n; i++ )
	{
		rand = next_exp();
		arrival_time = floor( rand );
		cpu_bursts = ceil( rand ) * 100;

		printf("Process %c");
		
	}
}