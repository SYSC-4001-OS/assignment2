Assignment 2
SYSC 4001

Jason Van Kerkhoven		[100974276]
Brydon Gibson			[100xxxxxx]

01/11/2016


-----------------------------------------------------------
INPUT FORMAT:

	It should be noted that each line denotes a new proccess with the
	information per being shown as follows:
	
	---------------------------------------------------------------------------
	|  PID  |  requiredCPUTime  |  ioFrequency  |  ioDuration  |  ArivalTime  |
	---------------------------------------------------------------------------
	
	With the use of SINGLE SPACES (' ') to denote where one parameter ends,
	and the next one begins. The values entered are to be encoded with standard
	8 bit ASCII numerical values, 0-9. The values entered are assumed to be
	a decimal number during runtime. The values for "requiredCPUTime", 
	"ioFrequency", "ioDuration" and "ArivalTime" are assumed to be in milliseconds.
	An example of a process with a PID of 12, a required CPU time of 34ms,
	a I/O frequency of 56ms, and an I/O duration of 78ms  arriving at a time
	of 9ms after the simulation starts would be saved as:
	
	12 34 56 78 9
	
	Multiple processes are done in a similar fashion, with a new line between
	each process, such that:
	
	process_1	|	12 34 56 78 9
	process_2	|	5 7 10 100 0
	process_3	|	440 55 66 77 11
	process_4	|	45 69 87 10 50
	  ...		|		...
	process_40	|	112 28 2 25 12
	
	Additionally, please note that zero padding any number (ie writting
	5 as 05 or 0005) is not reccomended nor guarenteed to maintain functionality.
	There is a maximum number of allowed processes, with the max being 40.
	Exceding the maximun number of allowed processes will cause unexpected errors.
	
	It should also be noted each parameter field has a maximum number of characters
	of TEN (10) characters.
