/* algo.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

// Exponential distribution pseudo random number generator function
double next_exp( double lambda, int upper_bound )
{

                      /* uniform to exponential distribution: */
                      /*                                      */
  double min = 0;     /*        -ln(r)                        */
  double max = 0;     /*  x = ----------                      */
  double sum = 0;     /*        lambda                        */

  int iterations = 10000000;    /* <== make this number very large */

  for ( int i = 0 ; i < iterations ; i++ )
  {
    /* generate the next pseudo-random value x */
    // double lambda = 0.001;  /* average should be 1/lambda ==> 1000 */

    double r = drand48();   /* uniform dist [0.00,1.00) -- also see random() */

    double x = -log( r ) / lambda;  /* log() is natural log */

    /* skip values that are far down the "long tail" of the distribution */
    if ( x > upper_bound ) { i--; continue; }

    /* TO DO: Since adding the above line of code will lower the
              resulting average, try to modify lamdba to get back to
              an average of say 1000 */

    /* display the first 20 pseudo-random values */
    // if ( i < 20 ) printf( "x is %lf\n", x );

    sum += x;
    if ( i == 0 || x < min ) { min = x; }
    if ( i == 0 || x > max ) { max = x; }
  }

  double avg = sum / iterations;
	return avg;
}

// returns characters from A-Z by converting ASCII int value to char
// requires: n in range [0, 26)
char get_process_id( int n )
{
	int base = 65;
	return (char)(base + n);
}

// returns elapsed time in milliseconds
int timediff( struct timespec end, struct timespec start )
{
	double elapsed;
	elapsed = (double)(end.tv_sec - start.tv_sec) * 1000 + (double)(end.tv_nsec - start.tv_nsec) / 1000000;
	return (int)elapsed;
}