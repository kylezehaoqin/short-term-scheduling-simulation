/* ready-queue.h */
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

// #ifndef _READY_QUEUE_
// #define _READY_QUEUE_

typedef struct process
{
	char pid;
	int arrival_time;
	int cpu_bursts;
	struct process * next;
	struct process * prev;
	int * cpu_burst_times;
	int * io_burst_times;
	int tau;
} Process;

// Initialize new process
Process * newProcess( char pid, int arrival_time, int cpu_bursts, int tau )
{
	Process * tmp = ( Process * )calloc( 1, sizeof( Process ) );
	tmp->pid = pid;
	tmp->arrival_time = arrival_time;
	tmp->cpu_bursts = cpu_bursts;
	tmp->tau = tau;
	tmp->cpu_burst_times = ( int * )calloc( cpu_bursts, sizeof( int ) );
	tmp->io_burst_times = ( int * )calloc( cpu_bursts - 1, sizeof( int ) );
	tmp->next = NULL;
	tmp->prev = NULL;

	return tmp;
}

// perform a deep copy of old process
Process * copyProcess( Process * old )
{
	Process * new_ = ( Process * )calloc( 1, sizeof( Process ) );
	new_->pid = old->pid;
	new_->arrival_time = old->arrival_time;
	new_->cpu_bursts = old->cpu_bursts;
	new_->tau = old->tau;
	new_->cpu_burst_times = ( int * )calloc( new_->cpu_bursts, sizeof( int ) );
	new_->io_burst_times = ( int * )calloc( new_->cpu_bursts - 1, sizeof( int ) );

	int i;
	for ( i = 0; i < new_->cpu_bursts; i++ )
	{
		new_->cpu_burst_times[i] = old->cpu_burst_times[i];
		if ( i != new_->cpu_bursts - 1 )
		{
			new_->io_burst_times[i] = old->io_burst_times[i];
		}
	}
	new_->next = NULL;
	new_->prev = NULL;

	return new_;
}

// peek the arrival time of Process in the beginning of the ready queue
// for FCFS and RR simulations;
// returns: int
int peek_arrival( Process ** head )
{
	return (*head)->arrival_time;
}

// peek the calculated tau of Process in the beginning of the ready queue
// for SJF and SRT simulations;
// returns: int
int peek_tau( Process ** head )
{
	return (*head)->cpu_bursts;
}

// Push new process p to the end of queue
void push( Process ** head, Process * p )
{
	Process * first = *head;

	while ( first->next != NULL )
	{
		first = first->next;
	}
	first->next = p;
	p->prev = first;
}

// Push new process p to the queue ordered by increasing arrival time
void push_by_arrival( Process ** head, Process * p )
{
	// obtain a address of first process in the queue
	Process * first = *head;
	
	// Push the process with less arrival time in front
	if ( p->arrival_time < (*head)->arrival_time )
	{
		(*head)->prev = p;
		p->next = *head;
		*head = p;
	}
	else
	{
		// keep going down the queue until we find a process with larger arrival time;
		while ( first->next != NULL && p->arrival_time > first->arrival_time )
		{
			first = first->next;
		}
		// We have two scenarios here:
		// Queue: J(0)->A(9)->K(43)
		//											^
		// if p(42)
		if ( first->arrival_time > p->arrival_time )
		{
			first->prev->next = p;
			p->next = first;
			p->prev = first->prev;
			first->prev = p;
		}
		// if p(44)
		else
		{
			first->next = p;
			p->prev = first;
		}

	}

}

// Push new process p to the queue ordered by increasing tau
void push_by_tau( Process ** head, Process * p )
{
	// obtain a address of first process in the queue
	Process * first = *head;
	
	// Push the process with less arrival time in front
	if ( p->tau < (*head)->tau )
	{
		(*head)->prev = p;
		p->next = *head;
		*head = p;
	}
	else
	{
		// keep going down the queue until we find a process with larger arrival time;
		while ( first->next != NULL && p->tau > first->tau )
		{
			first = first->next;
		}
		// We have two scenarios here:
		// Queue: J(0)->A(9)->K(43)
		//											^
		// if p(42)
		if ( first->tau > p->tau )
		{
			first->prev->next = p;
			p->next = first;
			p->prev = first->prev;
			first->prev = p;
		}
		// if p(44)
		else
		{
			first->next = p;
			p->prev = first;
		}

	}

}

// print the content of the queue
void print_queue( Process ** head )
{
	Process * first = *head;
	printf( "[Q " );
	if ( first == NULL )
	{
		printf("empty");
	}
	else
	{
		while ( first != NULL )
		{
			printf( "%c", first->pid );
			first = first->next;
		}
	}
	printf( "]\n" );
}










// #endif