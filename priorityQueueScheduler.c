/**
*Class:             priorityQueueScheduler.c
*Project:           SYSC4001 Assignment 2
*Authors:           Jason Van Kerkhoven
* 					Brydon Gibson                                            
*Date of Update:    01/11/2016                                              
*Version:           1.0.0                                                   
*                                                                                   
*Purpose:           Stuff
* 
* 
*Update Log:		v1.1.0
*						- removed print statments from file parse (now that we know it works)
*						- branched from main.c 	
											\--> fcfsScheduler.c
*											 \--> IOfcfsScheduler.c
*											  \--> priorityQueueScheduler.c
*											   \--> IOprorityQueueScheduler.c
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


//Declaring data structures
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Process Control Block data struct
typedef struct PCB
{
	int PID;					//ID
	int requiredCPUTime;		//How many ticks the process has until completion
	int ioFrequency;			//How often the process goes to do I/O
	int ioDuration;				//How long the process is gone to do I/O
	int ioRemaining;			//How many ticks left until I/O is complete
} PCB;

//temporary wrapper for newArr items
typedef struct PCBAndArrival
{
	PCB pcb;					//the PCB stored
	int arrival;				//when the PCB is schedualed to arrive
}PCBAndArrival;




//Declaring global variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
long unsigned int simTime = 0;			//simutated time, in ms
	
PCB readyArr[MAX_PROCESSES];			//the waiting pseudo-list for processes that need access to the CPU
int readyArrSize = 0;					//effective size of above
PCBAndArrival newArr[MAX_PROCESSES];	//the pseudo-list of all processes loaded in from .dat file
int newArrSize = 0;						//effective size of above
PCB waitingArr[MAX_PROCESSES];			//the pseudo-list of all processes currently waiting for I/O to complete
int waitingArrSize = 0;					//the effective size of above
PCB terminatedArr[MAX_PROCESSES];		//the pseudo-list of all completed processes
int terminatedArrSize = 0;				//the effective size of above
PCB running;							//simulated processor. Whatever PCB is in here is "running"




//Various print statments
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//print PCB
void printPCB(PCB proc)
{
	printf("PID:      %d\nCPU Time: %d\nIO Freq:  %d\nIO Dur:   %d\nIO Rem:   %d\n",
			proc.PID,
			proc.requiredCPUTime,
			proc.ioFrequency,
			proc.ioDuration,
			proc.ioRemaining);
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
						newArr[processNum].pcb.ioFrequency = tempNum;
						break;
					case(3):
						newArr[processNum].pcb.ioDuration = tempNum;
						newArr[processNum].pcb.ioRemaining = 0;
						break;
					case(4):
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
	newArrSize = getPCBData("PCBdata.dat");
	printf("%d PCBs added to NEW array\n", newArrSize);
	printf("Dumping contents of NEW array...\n");
	printf("-----------------------------------------\n");
	printPCBAndArrivalArray(newArr, newArrSize);
	printf("-----------------------------------------\n\n");
	printf("Starting Simulation...\n");
}
