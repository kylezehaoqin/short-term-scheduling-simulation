/* ready-queue.h */
// #include <stdio.h>
#include <list>

#ifndef _MYPROCESS_
#define _MYPROCESS_

#define CPU_BURST -1
#define ARRIVAL 0
#define BACK_TO_Q 1
#define CPU_BURST_COMP 2



class Process
{

	public:

		Process();
		Process( char _pid, int _t_arrival, int _cpu_bursts, int _tau, int _t_cs );
		// Process( const Process &old );
		const char getID() const;
		const int getArrivalTime() const;
		const int getCPUBursts() const;
		const int getTau() const;
		const int getBurstsCompleted() const;
		const std::string nextBurst() const;
		void recalculateTau( double alpha );
		void logCPUBurstTime( int t );
		void logIOBurstTime( int t );
		void arrival();
		void cpuburst( int current_time );
		bool cpuburst_comp();
		void ioburst( int current_time );
		void backtoq();
		void terminate();

	// private:

		char pid;
		int t_cs;
		int t_arrival;
		int cpu_bursts;
		int tau;
		int next_op;
		int next_op_time;
		bool terminated;
		int cpu_bursts_completed;
		std::list<int> cpu_burst_times;
		std::list<int> io_burst_times;
};

Process::Process()
{
	
}

// Class constructor
Process::Process( char _pid, int _t_arrival, int _cpu_bursts, int _tau, int _t_cs )
{
	pid = _pid;
	t_arrival = _t_arrival;
	cpu_bursts = _cpu_bursts;
	tau = _tau;
	next_op = ARRIVAL;
	next_op_time = t_arrival;
	terminated = false;
	cpu_bursts_completed = 0;
	t_cs = _t_cs;
}

// // Class copy constructor
// Process::Process( const Process &old )
// {
// 	pid = old.pid;
// 	t_arrival = old.t_arrival;
// 	cpu_bursts = old.cpu_bursts;
// 	tau = old.tau;
// 	cpu_burst_times = old.cpu_burst_times;
// 	io_burst_times = old.io_burst_times;
// 	cpu_bursts_completed = old.cpu_bursts_completed;
// 	io_bursts_completed = old.io_bursts_completed;
// 	next_burst = old.next_burst;
// }

// add cpu burst time t into member vector
void Process::logCPUBurstTime( int t )
{
	cpu_burst_times.push_back( t );
}

// add io burst time t to member vector
void Process::logIOBurstTime( int t )
{
	io_burst_times.push_back( t );
}

// return arrival time of member (t_arrival)
const int Process::getArrivalTime() const
{
	return t_arrival;
}

// 
const int Process::getCPUBursts() const
{
	return cpu_bursts;
}


const char Process::getID() const
{
	return pid;
}

const int Process::getTau() const
{
	return tau;
}

// print process arrival event
// change the next_op to CPU_BURST
void Process::arrival()
{
	printf("Process %c arrived; added to ready queue ", pid);
	next_op = CPU_BURST;
}

// print cpu burst event
// remove the current burst time in cpu_burst_times
// change the next_op to CPU_BURST_COMP
void Process::cpuburst( int current_time )
{
	int burst_time = cpu_burst_times.front();
	printf("Process %c started using the CPU for %dms burst ", pid, burst_time);
	next_op = CPU_BURST_COMP;
	next_op_time = current_time + burst_time;
	cpu_burst_times.pop_front();
}

// if no bursts to go, process terminated
// return true if terminated, false otherwise
bool Process::cpuburst_comp()
{
	int cpu_burst_togo = cpu_burst_times.size();
	if ( cpu_burst_togo == 0 )
	{
		printf("Process %c terminated ", pid );
		terminated = true;
		return true;
	}
	else
	{
		printf("Process %c completed a CPU burst; %d burst%s to go ", pid, cpu_burst_togo, cpu_burst_togo == 1 ? "" : "s" );
		return false;
	}
}

// ioburst
void Process::ioburst( int current_time )
{
	int burst_time = io_burst_times.front();
	next_op = BACK_TO_Q;
	next_op_time = current_time + burst_time + t_cs/2;
	printf("time %dms: Process %c switching out of CPU; will block on I/O until time %dms ", current_time, pid, next_op_time );
	io_burst_times.pop_front();
}

void Process::backtoq()
{
	printf("Process %c completed I/O; added to ready queue ", pid);
	next_op = CPU_BURST;
}


// operator overloading for custom sorting by least arrival time
bool operator<( const Process & p1, const Process & p2 )
{
	return p1.next_op_time > p2.next_op_time;
}


#endif