/**
*File:            	fcfsScheduler.c
*Project:           SYSC4001 Assignment 2
*Authors:           Jason Van Kerkhoven
* 					Brydon Gibson
*                                            
*Date of Update:    03/11/2016                                             
*Version:           1.2.3                                                   
*                                                                                   
*Purpose:           Simulator for a frist-come first-serve scheduling algorithm. Takes in PCB inputs as a .fcfs file,
*					parses into PCB structs, and runs simulation based on FCFS. Records important data points during
*					execution using processMetrics struct.
* 
* 
*Update Log:		v1.2.3
*						- calculation for scheduler av wait time fixed 
*					v1.2.2
*						- now records process turnaround, throughput, burst#,  total waiting time, av waiting time
*						- processMetrics now holds endTime
*						- final print added
*						- final stats calculated (av throughput changed to show as processes/100ms so its not a tiny decimal
*						- print statments made to allign arrows to show state change
*					v1.2.1
*						- metrics[] and metricsSize made global
*						- printMetrics() function implimented for dumping array of processMetrics
*						- logic for processMetric array fixed (shoutout to Brydon "Bry-Guy" Gibson for the debugging help
*						- metrics[] now fixed size equal to max allowed processes (40)
*						- metricsSize now critical. Used for representation of effective size of metrics[]
*						- now records program ID and start time in metrics[]
*						- print statments cleaned up to be more human-readable
*					v1.2.0
*						- burned to the ground, lets start again from v1.1.0
*					v1.1.0
*						- removed print statments from file parse (now that we know it works)
*						- branched from main.c 	
*											\--> fcfsScheduler.c
*											 \--> fcfsSchedulerIO.c
*											  \--> priorityQueueScheduler.c
*											   \--> prorityQueueSchedulerIO.c
*						- removed I/O related values from struct (no longer needed)
*						- removed I/O related values from print functions
*						- removed I/O related values from process file
*						- SNAZZY acronyms made for file extentions ;)
*						- pretty much all of the fcfs logic implimented
*					v1.0.0
*						- all arrays implimented to simulate state diagram
* 						- print statments FINALLY stopped segfaulting
* 						- general print statments added
* 						- file is now read and parsed
* 						- patched that nasty bug with the last param in a file
* 						  being dropped
* 						- new struct introduced to hold arrival time and PCB
* 						- code easier to look at thanks to big-ass horizontal lines
*/


//import external libraries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PCB_PARAM_CHAR 10
#define MAX_PROCESSES 40
#define TRUE 1
#define FALSE 0


//Declaring data structures
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Process Control Block data struct
typedef struct PCB
{
	int PID;					//ID
	int requiredCPUTime;		//How many ticks the process has until completion
} PCB;

//temporary wrapper for newArr items
typedef struct PCBAndArrival
{
	PCB pcb;					//the PCB stored
	int arrival;				//when the PCB is schedualed to arrive
}PCBAndArrival;

//holds the running average for each processes waittime
typedef struct processMetrics
{
	int PID;					//process ID these metrics are for
	int endTime;				//when the process ended
	int arrivalTime;			//when process was added to ready queue
	int turnaroundTime;			//the process turnaround time (delta b/wn end and arival times)
	int waitingTime;			//how many ms the process has been waiting in the ready queue total
	int burstCount;				//running tally of how many bursts this process had to run to reach completion
	double meanWaitingTime;		//waitingTime/burstCount
}processMetrics;




//Declaring global variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
long int simTime = 0;			//simutated time, in ms
	
PCB readyArr[MAX_PROCESSES];			//the waiting pseudo-list for processes that need access to the CPU
int readyArrSize = 0;					//effective size of above
PCBAndArrival newArr[MAX_PROCESSES];	//the pseudo-list of all processes loaded in from .dat file
int newArrSize = 0;						//effective size of above
PCB waitingArr[MAX_PROCESSES];			//the pseudo-list of all processes currently waiting for I/O to complete
int waitingArrSize = 0;					//the effective size of above
PCB terminatedArr[MAX_PROCESSES];		//the pseudo-list of all completed processes
int terminatedArrSize = 0;				//the effective size of above
PCB running;							//simulated processor. Whatever PCB is in here is "running"

processMetrics metrics[MAX_PROCESSES];	//array of processMetrics structs for monitoring
int metricsSize;						//effective size of metrics Array



//Fucntions dealing with processMetrics struct
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//return the correct metric in an array
int findMetric(int id)
{
	//set metrics start time for process
	for (int i=0; i < metricsSize; i++)
	{
		//correct metrics found
		if (metrics[i].PID == id)
		{
			return i;
		}
	}
	//something wrong has occured
	printf("!ERROR - PID NOT FOUND IN METRICS - LINE 107!");
	exit(0);
}




//Functions to print misc. things
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//print array
void printMetrics()
{
	for(int i=0; i < metricsSize; i++)
	{
		printf("PID:        %d\narrive t:   %d\nend t:      %d\nturn t:     %d\nwait t:     %d\nburst c:    %d\nav. wait t: %lf\n\n",
				metrics[i].PID,
				metrics[i].arrivalTime,
				metrics[i].endTime,
				metrics[i].turnaroundTime,
				metrics[i].waitingTime,
				metrics[i].burstCount,
				metrics[i].meanWaitingTime);
	}
}


//print PCB
void printPCB(PCB proc)
{
	printf("PID:      %d\nCPU Time: %d\n",
			proc.PID,
			proc.requiredCPUTime);
}


//print PCB array
void printPCBArray(PCB arr[], int length)
{
	int i=0;
	for(i=0; i<length; i++)
	{
		//print PCB
		printPCB(arr[i]);
	}
}


//print state change
void printStateChange(int id, char msg[])
{
	printf("%dms   ID-%d   %s\n", simTime, id, msg);
}


//print PCBAndArrival array (AKA the most unwieldy name)
void printPCBAndArrivalArray(PCBAndArrival arr[], int length)
{
	int i=0;
	for(i=0; i<length; i++)
	{
		//print PCB
		printPCB(arr[i].pcb);
		//print arrival
		printf("ARV:      %d\n\n", arr[i].arrival); 
	}
}





//Reads a .dat file, fills readyArr with relevant data.
//returns number of processes read
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int getPCBData(char fileName[])
{
	//declaring local variables
	int numProc = 1;
	int charRead = 0;
	FILE *fp;
	char param[MAX_PCB_PARAM_CHAR];
	int i = 0;
	int tempNum = 0;
	int paramNum = 0;
	int processNum = 0;
	
	//open the .dat file
	fp = fopen(fileName, "r");
	//check fp for validity
	if (fp == NULL)
	{
		//error
		printf("!UNEXPECTED ERROR - FILE NOT FOUND!\n");
		exit(0);
	}
	
	//read new char
	charRead = fgetc(fp);
		//read .dat file until end
		while(charRead != EOF)
		{
			//new param, as a space was found
			if (charRead == ' ' || charRead == '\n')
			{
				//convert current param and save to current process
				tempNum = atoi(param);

				switch(paramNum)
				{
					case(0):
						newArr[processNum].pcb.PID = tempNum;
						break;
					case(1):
						newArr[processNum].pcb.requiredCPUTime = tempNum;
						break;
					case(2):
						newArr[processNum].arrival = tempNum;
						break;
					default:
						printf("ERROR - UNEXPECTED NUMBER OF PARAMETERS");
						exit(0);
						break;
				}
				
				//reset i and param, inc paramNum (temporarily hijacking i for a for loop)
				//(temporarily hijacking i for a for loop)
				for(i=0; i < MAX_PCB_PARAM_CHAR; i++)
				{
					param[i] = '\0';
				}
				i=0;
				paramNum++;
				
				//if new line, inc processNum
				if(charRead == '\n')
				{
					//reset i and paramNum, inc processNum and procNum
					paramNum = 0;
					processNum++;
					numProc++;
				}
			}
			//add to current param
			else
			{
				param[i] = charRead;
				i++;
			}

			//read new char
			charRead = fgetc(fp);	
		}
	return (numProc-1);
	
}




//main function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char const *argv[])
{
	//declaring local variables
	double avWait = 0.0;					//mean wait time for schedualer
	double avTurnaround = 0.0;				//mean turnaround time for schedualer
	int newArrEmpty = FALSE;				//pseudo-boolean, denotes if newArr is empty
	int cpuBusy = FALSE;					//pseudo-boolean, denotes if the CPU is busy
	int index;								//where metric is located. Used to make code readable. Constantly overwritten and rewritten

	//load up processes into newArr, print contents
	newArrSize = getPCBData("PCBdata.fcfs");
	printf("%d PCBs added to NEW array\n", newArrSize);
	printf("Dumping contents of NEW array...\n-----------------------------------------\n");
	printPCBAndArrivalArray(newArr, newArrSize);
	printf("\n");
	
	//set up struct array to hold data for post-mortum
	metricsSize = newArrSize;
	for (int i=0; i<newArrSize; i++)
	{
		metrics[i].PID = (newArr[i].pcb).PID;
		metrics[i].endTime = 0;
		metrics[i].meanWaitingTime = 0.0;
		metrics[i].arrivalTime = 0;
		metrics[i].turnaroundTime = 0;
		metrics[i].waitingTime = 0;
		metrics[i].burstCount = 0;
	}

	//dump metrics
	printf("%d processMetrics added to METRICS array\n", newArrSize);
	printf("Dumping contents of METRICS array...\n-----------------------------------------\n");
	printMetrics();
	printf("\n");

	//start simulation, continue until all processes are complete
	printf("Starting Simulation...\n-----------------------------------------\n");
	while(terminatedArrSize < metricsSize)
	{
		//check for processes that have just arrived in newArr
		if (newArrEmpty == FALSE)
		{
			for(int i=0; i < newArrSize; i++)
			{
				//process arrives to scheduler for dispatching, add to tail of readyArr
				if (newArr[i].arrival <= simTime)
				{
					//add to readyArr and print
					readyArr[readyArrSize] = newArr[i].pcb;
					printStateChange(readyArr[readyArrSize].PID, "new     --> ready");

					//shift array left
					//case 1: i is at start or middle
					if (i < newArrSize-1)
					{
						for (int a=i; a < newArrSize-1; a++)
						{
							newArr[a] = newArr[a+1];
						}
						//dec size of newArr, dec i so as we check the new value we shifted into i
						newArrSize--;
						i--;	
					}
					//case 2: i is at end
					else
					{
						newArrSize--;
						//check if empty
						if (newArrSize == 0)
						{
							newArrEmpty == TRUE;
						}
					}

<<<<<<< HEAD
					//set metrics start time for process
					for (int i=0, somethingSaved=FALSE; (i < metricsSize) && (somethingSaved == FALSE); i++)
					{
						//correct metrics found
						if (metrics[i].PID == readyArr[readyArrSize].PID)
						{	
						metrics[i].startTime = simTime;
							somethingSaved = TRUE ;
						}
						//check that we actually recorded something, we should never enter this statment
						if (i+1 == metricsSize && somethingSaved == FALSE)
						{
							printf("!ERROR - PID NOT FOUND - LINE 333!");
							exit(0);
						}
					}

=======
					//set metrics arrival time for process
					index = findMetric(readyArr[readyArrSize].PID);
					metrics[index].arrivalTime = simTime;
>>>>>>> 71babefb4f71c303cca2ea43feaf7614fed84ff7
					//inc readyArrSize
					readyArrSize++;
				}
			}
		}

		//simulate processor
		//cpu idle, transfer first file in readyArr
		if (cpuBusy == FALSE)
		{
			//check if there are processes waiting for CPU time
			if (readyArrSize > 0)
			{
				//add first process in array and shift left
				running = readyArr[0];
				for (int i=0; i < readyArrSize-1; i++)
				{
					readyArr[i] = readyArr[i+1];
				}
				readyArrSize--;

				//print and alter CPU flag
				printStateChange(running.PID, "ready   --> running");
				cpuBusy = TRUE;
			}
		}

		//step forward in time
		simTime++;

		//run cpu process
		if (cpuBusy == TRUE)
		{
			//run cpu
			running.requiredCPUTime--;
			//check if done
			if (running.requiredCPUTime <= 0)
			{
				//alter flags and metric
				index = findMetric(running.PID);
				cpuBusy = FALSE;
				metrics[index].burstCount++;
				metrics[index].endTime = simTime;
				metrics[index].turnaroundTime = (simTime - metrics[index].arrivalTime);
				metrics[index].meanWaitingTime = metrics[index].waitingTime / metrics[index].burstCount;

				//print
				printStateChange(running.PID, "running --> complete");

				//remove from CPU, DO NOT place back into ready list, add to terminatedArrSize
				cpuBusy = FALSE;
				terminatedArrSize++;
			}
		}

		//increase stats for processes in readyArr
		for(int i=0; i < readyArrSize; i++)
		{
			index = findMetric(readyArr[i].PID);
			metrics[index].waitingTime++;
		}

	}

	//print final values for each process run
	printf("%dms   Simulation Complete!\n\n", simTime);
	printf("Dumping contents of METRICS array...\n-----------------------------------------\n");
	printMetrics();

	//calculate final stats
	for (int i=0; i<metricsSize; i++)
	{
		avWait = avWait + metrics[i].meanWaitingTime;
		avTurnaround = avTurnaround + metrics[i].turnaroundTime;
	}
	avWait = avWait/metricsSize;
	avTurnaround = avTurnaround/metricsSize;

	//print final values for scheduler
	printf("Calculating Final Scheduler Statistics...\n-----------------------------------------\n");
	printf("Average Process Waiting Time:  %lfms\nAverage Turnaround Time:       %lfms\nAverage Throughput:            %lf processes per 100ms\n\n\n\n", 
			avWait, 
			avTurnaround,
			(100/avTurnaround));
	
}
