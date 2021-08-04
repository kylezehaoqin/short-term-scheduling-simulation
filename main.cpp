#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <queue>
#include <vector>

// ======== FUNCTION DECLARATIONS ===================================================
double next_exp( double lambda, int upper_bound );
char get_process_id( int n );
int getCPU_Bursts( int upper_bound );
void FCFS( int n, int seed, double lambda, int upper_bound );


int main( int argc, char ** argv )
{
	// =================================================================================
	// Arguments validation and error handling
	// =================================================================================
  if ( argc != 8 )
	{
		fprintf( stderr, "ERROR: Invalid number of arguments, expected argv[0-7]\n");
		return EXIT_FAILURE;
	}

	// number of processes to simulate, from A-Z;
	const int n = atoi( argv[1] ); 
	if ( n <= 0 || n > 26 )
	{
		fprintf( stderr, "ERROR: Invalid n value ( argv[1] )\n");
		return EXIT_FAILURE;
	}

	// seed for pseudo random number generator;
	const int seed = atoi( argv[2] );
	if ( seed == 0 )
	{
		fprintf( stderr, "ERROR: Invalid seed value ( argv[2] )\n");
		return EXIT_FAILURE;
	}

	// lambda for exponential distribution;
	const double lambda = atof( argv[3] );
	if ( lambda == (double)0 )
	{
		fprintf( stderr, "ERROR: Invalid lambda value ( argv[3] )\n");
		return EXIT_FAILURE;
	}
	
	// upper bound for valid pseudo random numbers, random values exceeding upper_bound are skipped;
	const int upper_bound = atoi( argv[4] );
	if ( upper_bound <= 0 )
	{
		fprintf( stderr, "ERROR: Invalid upper_bound value ( argv[4] )\n");
		return EXIT_FAILURE;
	}

	// time to perform a context switch, expects a positive even int, measured in ms;
	const int t_cs = atoi( argv[5] );
	if ( t_cs <= 0 || t_cs % 2 != 0 )
	{
		fprintf( stderr, "ERROR: Invalid t-cs value ( argv[5] )\n");
		return EXIT_FAILURE;
	}

	// constant alpha for SJF and SRT algorithms
	const double alpha = atof( argv[6] );
	if ( alpha == (double)0 )
	{
		fprintf( stderr, "ERROR: Invalid alpha value ( argv[6] )\n");
		return EXIT_FAILURE;
	}
	
	// time slice value for RR algorithm
	const int time_slice = atoi( argv[7] );
	if ( time_slice <= 0 )
	{
		fprintf( stderr, "ERROR: Invalid timeslice value ( argv[7] )\n");
		return EXIT_FAILURE;
	}

	// ================================================================================
	// =========== Start of Simulation  ===============================================
	// ================================================================================


	FCFS( n, seed, lambda, upper_bound );


	// printf("For process %c\n", tmp.pid);
	// for ( long unsigned int i = 0; i < tmp.cpu_burst_times.size(); i++ ) {
	// 	printf("%d | ", tmp.cpu_burst_times[i]);
	// 	if ( (int)i != tmp.cpu_bursts - 1 ) {
	// 		printf("%d", tmp.io_burst_times[i]);
	// 	}
	// 	printf("\n");
	// }

	return EXIT_SUCCESS;
}

// ================================================================================================
// ===================================   FUNCTIONS  ===============================================
// ================================================================================================
