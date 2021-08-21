/* algo.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <map>
#include <list>
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


std::vector<Process> processes_init( int n, int seed, double lambda, int upper_bound )
{

	std::vector<Process> processes;

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

		processes.push_back( tmp );

		printf("Process %c (arrival time %d ms) %d CPU bursts (tau %dms)\n", \
		get_process_id( i ), arrival_time, cpu_bursts, tau );
		
	}
	printf("\n");
	return processes;
}

void printReadyQueue( std::list<char> ready_queue)
{
	printf("[Q ");

	if ( ready_queue.empty() )
	{
		printf("empty");
	}
	else
	{
		for ( auto itr : ready_queue )
		{
			printf("%c", itr);
		}
	}
	printf("]\n");
}

// Performs First Come First Serve algorithhm simulation
void FCFS( int n, int seed, double lambda, int upper_bound, int t_cs )
{
	
	std::vector<Process> processes = processes_init( n, seed, lambda, upper_bound );
	std::list<char> ready_queue;

	int time = 0;
	printf("time %dms: Simulator started for FCFS [Q empty]\n", time);


	bool cpu_busy = false;

	while( 1 )
	{
		// Use a multimap to keep future events in "time" increasing order
		std::multimap<int, Process> events;
		// Use a vector to collect all other processes that tie
		std::vector<Process> candidates;

		// Check on simulation termination
		// If no process left in processes, all processes terminated, break out of loop
		if ( processes.empty() ) break;

		for ( auto p : processes )
		{
			int next_operation = p.next_op;
			events.insert( {p.next_op_time, p} );
		}


		// Find the event expected to happen the soonest
		auto itr = events.begin();
		int tmp_time = itr->first;
		Process tmp = itr->second;
		
		// We are using a multimap which allows key collision
		// Need to add "candidates" if they share the same next_op_time, then resolve ties
		itr++;
		while ( itr != events.end() && itr->first == tmp_time )
		{
			candidates.push_back( itr->second );
			itr++;
		}

		// Resolving ties
		// tie resolution order: 
		// 			// cpu burst completion > io burst completion > new process arrival
		// 			// we also defined macro:
		// 			// CPU_BURST_COMP == 2, BACK_TO_Q == 1, ARRIVAL == 0
		if ( !candidates.empty() )
		{
			for ( Process candidate : candidates )
			{
				// if next operation is the same, choose the one with lower alphabet
				if ( tmp.next_op == candidate.next_op )
				{
					if ( tmp.pid > candidate.pid )
					{	
						tmp = candidate;
					}
				}
				else
				{
					
					if ( tmp.next_op > candidate.next_op )
					{
						tmp = candidate;
					}
				}
			}
		}

		// See if cpu is busy and if next event's time is larger than current time
		if ( !cpu_busy && time < tmp_time )
		{
			// if ready queue isn't empty, choose the front pid to perform cpu burst
			if ( !ready_queue.empty() )
			{
				for ( auto p : processes )
				{
					// Find the process with such pid
					if ( p.pid == ready_queue.front() )
					{
						p.cpuburst();
						ready_queue.pop_front();
						cpu_busy = true;
						break;
					}
				}
			}
		}
		
		// printf("time %dms: ", time);
		// // decide what to do based on tmp Process's next_op
		// if ( tmp.next_op == ARRIVAL )
		// {
		// 	tmp.arrival();
		// 	ready_queue.push_back( tmp.pid );
		// }
		// else if ( tmp.next_op == CPU_BURST )
		// {
		// 	tmp.cpuburst();

		// }
		// else if ( tmp.next_op == IO_BURST )
		// {
		// 	tmp.ioburst( time );
		// }
		// else if ( tmp.next_op == BACK_TO_Q )
		// {
		// 	tmp.backtoq();
		// 	ready_queue.push_back( tmp.pid );
		// }
		// else if ( tmp.next_op == TERMINATION )
		// {
		// 	tmp.terminate();
		// }

		printReadyQueue( ready_queue );
		time += tmp_time;
	}
	printf("Simulator ended for FCFS [Q empty]\n\n");

}
