/* algo.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <utility>
#include "myprocess.h"

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


std::priority_queue<Process> processes_init( int n, int seed, double lambda, int upper_bound )
{

	std::priority_queue<Process> arrival_queue;

	srand48( (long)seed );

	double rand;
	int arrival_time;
	int cpu_bursts;
	int cpu_burst_time;
	int io_burst_time;
	// Initial guess for tau
	int tau = 1 / lambda;
	char pid;
	int i = 0;

	for ( i = 0; i < n; i++ )
	{
		// Get process id ( A-Z )
		pid = get_process_id( i );

		// Get process arrival time
		rand = next_exp( lambda, upper_bound );
		arrival_time = floor( rand );
		
		// Get process CPU bursts
		cpu_bursts = getCPU_Bursts( upper_bound );

		// Initialize process object
		Process tmp( pid, arrival_time, cpu_bursts, tau );
		
		// For each cpu burst, get CPU burst time and I/O burst time
		int j;
		for ( j = 0; j < cpu_bursts; j++ )
		{
			rand = next_exp( lambda, upper_bound );
			cpu_burst_time = ceil( rand );
			tmp.logCPUBurstTime( cpu_burst_time );

			// No I/O burst for the last CPU burst
			if ( j != cpu_bursts - 1)
			{
				rand = next_exp( lambda, upper_bound );
				io_burst_time = ceil( rand ) * 10;
				tmp.logIOBurstTime( io_burst_time );
			}
		}

		arrival_queue.push( tmp );

		printf("Process %c (arrival time %d ms) %d CPU bursts (tau %dms)\n", \
		get_process_id( i ), arrival_time, cpu_bursts, tau );
		
	}
	printf("\n");
	return arrival_queue;
}

void FCFS( int n, int seed, double lambda, int upper_bound )
{
	int time = 0;

	std::priority_queue<Process> arrival_queue = processes_init( n, seed, lambda, upper_bound );

	std::map<int, Process> processes;

	

	while( !arrival_queue.empty() )
	{
		Process tmp = arrival_queue.top();
		std::pair<int, Process> process( tmp.getArrivalTime(), tmp);
		processes.insert( process );
		arrival_queue.pop();
	}

	printf("time %dms: Simulator started for FCFS [Q empty]\n", time);

	for ( auto p : processes )
	{
		printf("%c arrives at %dms\n", p.second.getID(), p.first );
	}
	
}
