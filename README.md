# CPU Short Term Scheduling Simulation Using FCFS, SJF, SRT, and RR Algorithms

## Simulation Algorithms

### FCFS - First Come First Serve

Baseline algorithm, non-preemptive, processes line up in ready queue ordered by arrival
time. Could be implemented as RR with infinite "timeslice"

### SJF - Shortest Job First

Non-preemptive, processes are stored in ready queue ordered by "anticipated CPU burst times". 

### SRT - Shortest Remaining Time

Preemptive version of SJF. Before a process A enters the ready queue, if A has CPU burst time that is less than the remaining time of current running process, preemption occurs, current running process is added back in the ready queue.

### RR - Round Robin

Preemptive FCFS, with predefined time slice t_slice. Each process is given t_slice amount of time to complete its CPU burst. If expiration occurs (i.e. running process receives CPU burst time > t_slice), process is added back to the ready queue.

If process completes its CPU burst before expiration, head process of ready queue gets context-switched into the CPU.

## Preemption Skipping

When preemption occurs when no other processes reside in ready queue, keep executing without context switch. 


## Simulation Parameters

1. 
argv[1]: `n`, number of process to simulate. Max 26, A-Z. 

2. 
argv[2]: `seed`, seed serves as the seed for random number generator using `srand48()` before each algo in simulation.

3. 
argv[3]: `lambda`. Use exponential distribution for determining interarrival times. 1/lambda: average random value generated. See `exp-random.c` as example. 

4. 
argv[4]: `upper bound` for valid pseudo-random numbers. This threshold is used to avoid values far down the long tail of exponential distribution. 

5. 
argv[5]: `t_cs`, time it takes to perform a context switch, in milliseconds. First half `1/2 * t_cs` is time required to remove running process from CPU. Second half is the time required to bring in the next process in to use CPU. `t_cs` **positive even** integer. 

6. 
argv[6]: constant `alpha`, to produce an estimate of CPU burst times via exponential averaging. i.e. `tau_0 = 1/lambda`. Use "ceiling" function for `tau` calculations.

7. 
argv[7]: `t_slice` measured in milliseconds, for RR algorithm.


## Pseudo-random Numbers and Predictability

The following algorithm should be fully executed before application of any of the scheduling algorithms, to ensure identical initial conditions. 

**`processes_init()`**

Define the exponential distribution pseudo-random number generation function as `next_exp()`.

For each of the n processes, in alphabetical order:

1. 
Identify the initial process arrival time as the "floor" of the next number in the sequence given by `next_exp()`; note that we could have a **0** arrival time.

2. 
Identify the number of CPU bursts for the given process as the "ceiling" of the next random number generated form the uniform distribution (i.e. obtained via `drand48()`) multiplied by **100**; this should obtain a random integer in [1,100].

3. 
For *each* of these CPU bursts, identify the CPU burst time and the I/O burst time as the "ceiling" of the next two random numbers in the sequence given by `next_exp()`; multiply the I/O burst time by **10**; for the last CPU burst, do not generate an I/O burst time (each process ends with a final CPU burst).

#### After each algorithm simulation, reset simulation back to initial set of processes with elapsed time back to 0, re-seed random number generator.

##### During simulation, CPU being idle could occur when all processes are busy performing or blocked on I/O. When all processes terminate, simulation ends


## Handling "Ties"

If different types of events occur at the same time, sumulate these events using the following order:

a. CPU burst completion
b. I/O burst completions (back into ready queue)
c. new process arrivals 

If "ties" occur within one of the 3 scenarios, break tie using process ID order (alphabetically).

## Measurements

#### For each algorithm, count:

preemptions
context-switches
CPU usage
CPU idle time

#### For each CPU burst, measure:

CPU burst time (given)
turnaround time
wait time

##### Details
WAIT TIME: How much time does a process spend in the
           ready queue, waiting for time with the CPU?
           (Note that the process here is in the READY state.)

TURNAROUND TIME: How much time is required for a process to
                 complete its CPU burst, from the time it
                 first enters the ready queue through to when
                 it completes its CPU burst?


TURNAROUND TIME  =  WAIT TIME  +  CPU BURST TIME


TURNAROUND TIME  =  WAIT TIME  +  CPU BURST TIME  +  OVERHEAD
                                                    (context switches)



### CPU Burst Time
Randomly generated for each process. Does not include context-switch times.


### Turnaround Time
Measured for each process. Defined as the end-to-end time a process spends in executing **a single CPU burst**.
(i.e. from process arrival time through to when CPU burst completes and process switched out of CPU)
Therefore includes the second half of the initial context-switch-in and first half of the final context-switch-out, as well as other context switches that occur while CPU burst is being completed (due to preemptions).


### Wait Time
Measured for each process. Defined as the amount of time a process spends waiting to use the CPU, which equates to the amount of time the given process resides in the ready queue, and it does not include context switch times. 


A process leaves the ready queue when it is switched into the CPU, which takes a half of the context-switch time `t_cs`. A preempted process leaves the CPU and enters the ready queue after the first half of `t_cs`, likewise.


### CPU Utilization
Measured by CPU bursts times vs. total simulation time.


## Terminal Output
Keep track of elapsed time `t` in milliseconds, start at 0 for each algorithm. 
Format of "interesting" event: `time <t>ms: <event-details> [Q <queue-contents>]`

Events:
  Start of simulation
  Process arrival
  Process starts using the CPU
  Process finishes using the CPU (i.e., completes a CPU burst)
  Process has its `tau` value recalculated (i.e., after a CPU burst completion)
  Process preemption
  Process starts performing I/O
  Process finishes performing I/O
  Process terminates by finishing its last CPU burst
  End of simulation

When simulation ends: `time <t>ms: Simulator ended for <algorithm> [Q empty]`

**Include the process removal time (half of context switch time) for the last process**

## Output File
Generate output file `simout.txt`.

Format:

Algorithm FCFS
-- average CPU burst time: #.### ms
-- average wait time: #.### ms
-- average turnaround time: #.### ms
-- total number of context switches: #
-- total number of preemptions: #
-- CPU utilization: #.###%
Algorithm SJF
-- average CPU burst time: #.### ms
-- average wait time: #.### ms
-- average turnaround time: #.### ms
-- total number of context switches: #
-- total number of preemptions: #
-- CPU utilization: #.###%
Algorithm SRT
-- average CPU burst time: #.### ms
-- average wait time: #.### ms
-- average turnaround time: #.### ms
-- total number of context switches: #
-- total number of preemptions: #
-- CPU utilization: #.###%
Algorithm RR
-- average CPU burst time: #.### ms
-- average wait time: #.### ms
-- average turnaround time: #.### ms
-- total number of context switches: #
-- total number of preemptions: #
-- CPU utilization: #.###%

**Averages are averaged over all executed CPU bursts. To count the number of context switches, count the number of times a process starts using the CPU**

## Error Handling

Report error to `stderr`:

`ERROR: <error-text>`