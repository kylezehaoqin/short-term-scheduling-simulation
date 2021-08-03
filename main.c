#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "ready_queue.h"

// ======== FUNCTION DECLARATIONS ===================================================
double next_exp();
char get_process_id( int n );
double timediff( struct timespec end, struct timespec start );
int getCPU_Bursts( int upper_bound );
Process * processes_init_by_arrival( int n, int seed, double lambda, int upper_bound );

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


	Process * head = processes_init_by_arrival( n, seed, lambda, upper_bound );

	print_queue( &head );

	return EXIT_SUCCESS;
}

// ================================================================================================
// ===================================   FUNCTIONS  ===============================================
// ================================================================================================

Process * processes_init_by_arrival
( int n, int seed, double lambda, int upper_bound )
{
	srand48( (long)seed );

	double rand;
	int arrival_time;
	int cpu_bursts;
	int cpu_burst_time;
	int io_burst_time;

	int i = 0;
	// Initial guess for tau
	int tau = 1/lambda;

	// NEED TO INITIALIZE THE HEAD OF READY QUEUE 
	// ==================================================================================
	// Get process id ( A-Z )
	char pid = get_process_id( i );

	// Get process arrival time
	rand = next_exp( lambda, upper_bound );
	arrival_time = floor( rand );
	
	// Get process CPU bursts
	cpu_bursts = getCPU_Bursts( upper_bound );

	Process * head = newProcess( pid, arrival_time, cpu_bursts, tau );
	
	// For each cpu burst, get CPU burst time and I/O burst time
	int j;
	for ( j = 0; j < cpu_bursts; j++ )
	{
		rand = next_exp( lambda, upper_bound );
		cpu_burst_time = ceil( rand );

		head->cpu_burst_times[j] = cpu_burst_time;

		// No I/O burst for the last CPU burst
		if ( j != cpu_bursts - 1) {
			rand = next_exp( lambda, upper_bound );
			io_burst_time = ceil( rand ) * 10;
			
			head->io_burst_times[j] = io_burst_time;
		}
	}
	printf("Process %c (arrival time %d ms) %d CPU bursts (tau %dms)\n", \
				get_process_id( i ), arrival_time, cpu_bursts, (int)(1 / lambda));
	
	Process * test = copyProcess(head);
	// ==================================================================================

	for ( i = 1; i < n; i++ )
	{
		// Get process id ( A-Z )
		pid = get_process_id( i );

		// Get process arrival time
		rand = next_exp( lambda, upper_bound );
		arrival_time = floor( rand );
		
		// Get process CPU bursts
		cpu_bursts = getCPU_Bursts( upper_bound );

		Process * tmp = newProcess( pid, arrival_time, cpu_bursts, tau );
		
		// For each cpu burst, get CPU burst time and I/O burst time
		for ( j = 0; j < cpu_bursts; j++ )
		{
			rand = next_exp( lambda, upper_bound );
			cpu_burst_time = ceil( rand );
			tmp->cpu_burst_times[j] = cpu_burst_time;

			// No I/O burst for the last CPU burst
			if ( j != cpu_bursts - 1) {
				rand = next_exp( lambda, upper_bound );
				io_burst_time = ceil( rand ) * 10;
				tmp->io_burst_times[j] = io_burst_time;
			}
		}

		push_by_arrival( &head, tmp );

		printf("Process %c (arrival time %d ms) %d CPU bursts (tau %dms)\n", \
		get_process_id( i ), arrival_time, cpu_bursts, (int)(1 / lambda));
		
	}
	return head;
}