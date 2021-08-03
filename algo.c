/* algo.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

// Obtain a random int as a number of cpu bursts within range [1,100]
int getCPU_Bursts( int upper_bound )
{
	int cpu_bursts;
	while ( 1 )
	{
		/* drand48() has range [0.0, 1.0)
		   increment by 1 after floor function to achieve inclusive range [1,100]
		*/
		cpu_bursts = floor( drand48() * 100 ) + 1;
		if ( cpu_bursts > upper_bound )
		{
			continue;
		}
		else
		{
			return cpu_bursts;
		}
	}
}

// Exponential distribution pseudo random number generator function
double next_exp( double lambda, int upper_bound )
{
	double rand, ret;
	while ( 1 )
	{
		rand = drand48();
		ret = -log( rand ) / lambda;

		if ( ret > upper_bound )
		{
			continue;
		}
		else
		{
			return ret;
		}
	}
}

// returns characters from A-Z by converting ASCII int value to char
// requires: n in range [0, 25]
char get_process_id( int n )
{
	int base = 65;
	return (char)(base + n);
}

// returns elapsed time in milliseconds
double timediff( struct timespec end, struct timespec start )
{
	double elapsed;
	elapsed = (double)(end.tv_sec - start.tv_sec) * 1000 + \
						(double)(end.tv_nsec - start.tv_nsec) / 1000000;

	return elapsed;
}

