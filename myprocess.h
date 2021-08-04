/* ready-queue.h */
// #include <stdio.h>
#include <vector>

#ifndef _MYPROCESS_
#define _MYPROCESS_


class Process
{

	public:

		Process( char _pid, int _t_arrival, int _cpu_bursts, int _tau );
		Process( const Process &old );
		void logCPUBurstTime( int t );
		void logIOBurstTime( int t );
		const int getArrivalTime() const;
		const char getID() const;
		const int getBursts() const;
		const int getTau() const;

	// private:

		char pid;
		int t_arrival;
		int cpu_bursts;
		std::vector<int> cpu_burst_times;
		std::vector<int> io_burst_times;
		int tau;
};

// Class constructor
Process::Process( char _pid, int _t_arrival, int _cpu_bursts, int _tau )
{
	pid = _pid;
	t_arrival = _t_arrival;
	cpu_bursts = _cpu_bursts;
	tau = _tau;
}

// Class copy constructor
Process::Process( const Process &old )
{
	pid = old.pid;
	t_arrival = old.t_arrival;
	cpu_bursts = old.cpu_bursts;
	tau = old.tau;
	cpu_burst_times = old.cpu_burst_times;
	io_burst_times = old.io_burst_times;
}

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
const int Process::getBursts() const
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


bool operator<( const Process & p1, const Process & p2 )
{
	return p1.getArrivalTime() > p2.getArrivalTime();
}


#endif