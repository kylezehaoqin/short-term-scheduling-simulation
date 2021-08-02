

typedef struct process
{
	char id;
	int arrival_time;
	int cpu_bursts;
	struct process* next;
	struct process* prev;
} Process;

Process* newProcess()