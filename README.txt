#########################################################
#	    ____  _________    ____     __  _________	#
#	   / __ \/ ____/   |  / __ \   /  |/  / ____/	#
#	  / /_/ / __/ / /| | / / / /  / /|_/ / __/   	#
#	 / _, _/ /___/ ___ |/ /_/ /  / /  / / /___   	#
#	/_/ |_/_____/_/  |_/_____/  /_/  /_/_____/   	#
#							#
#########################################################                                       

Assignment 2
SYSC 4001

Jason Van Kerkhoven		[100974276]
Brydon Gibson			[100xxxxxx]

04/11/2016


-----------------------------------------------------------
INPUT FORMAT:
FIRST-COME FIRST-SERVE WITH IO 		(.fcfsx)


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
	of TEN (10) characters. Exceding this maximum will result in unexpected errors.


-----------------------------------------------------------
INPUT FORMAT:
FIRST-COME FIRST-SERVE WITHOUT IO 	(.fcfs)


	It should be noted that each line denotes a new proccess with the
	information per being shown as follows:
	
	--------------------------------------------
	|  PID  |  requiredCPUTime  |  ArivalTime  |
	--------------------------------------------
	
	With the use of SINGLE SPACES (' ') to denote where one parameter ends,
	and the next one begins. The values entered are to be encoded with standard
	8 bit ASCII numerical values, 0-9. The values entered are assumed to be
	a decimal number during runtime. The values for "requiredCPUTime", 
	"ioFrequency", "ioDuration" and "ArivalTime" are assumed to be in milliseconds.
	An example of a process with a PID of 98 and a required CPU time of 76ms,
	arriving at a time of 5ms after the simulation starts would be saved as:
	
	98 76 5
	
	Multiple processes are done in a similar fashion, with a new line between
	each process, such that:
	
	process_1	|	98 76 5
	process_2	|	5 73 2 
	process_3	|	4 505 0 
	process_4	|	9 6 80
	  ...		|	 ...
	process_40	|	6 28 2 
	
	Additionally, please note that zero padding any number (ie writting
	5 as 05 or 0005) is not reccomended nor guarenteed to maintain functionality.
	There is a maximum number of allowed processes, with the max being 40.
	Exceding the maximun number of allowed processes will cause unexpected errors.
	
	It should also be noted each parameter field has a maximum number of characters
	of TEN (10) characters. Exceding this maximum will result in unexpected errors.


-----------------------------------------------------------
INPUT FORMAT:
PRIORITY QUEUES WITHOUT IO 			(.pq)


	It should be noted that each line denotes a new proccess with the
	information per being shown as follows:
	
	----------------------------------------------------------
	|  PID  |  requiredCPUTime  |  Priority  |  ArrivalTime  |
	----------------------------------------------------------
	
	With the use of SINGLE SPACES (' ') to denote where one parameter ends,
	and the next one begins. The values entered are to be encoded with standard
	8 bit ASCII numerical values, 0-9. The values entered are assumed to be
	a decimal number during runtime. The values for "requiredCPUTime", 
	"ioFrequency", "ioDuration" and "ArivalTime" are assumed to be in milliseconds.
	An example of a process with a PID of 98 and a required CPU time of 76ms,
	arriving at a time of 5ms after the simulation starts and a priority of 12 would be saved as:
	
	98 76 12 5
	
	Multiple processes are done in a similar fashion, with a new line between
	each process, such that:
	
	process_1	|	98 76 5 2
	process_2	|	5 73 2 10
	process_3	|	4 505 0 3
	process_4	|	9 6 80 5
	  ...		|	 ...
	process_40	|	6 28 2 6
	
	Additionally, please note that zero padding any number (ie writting
	5 as 05 or 0005) is not reccomended nor guarenteed to maintain functionality.
	There is a maximum number of allowed processes, with the max being 40.
	Exceding the maximun number of allowed processes will cause unexpected errors.
	
	It should also be noted each parameter field has a maximum number of characters
	of TEN (10) characters. Exceding this maximum will result in unexpected errors.

-----------------------------------------------------------
INPUT FORMAT:
PRIORITY QUEUES WITH IO 			(.pqx)


	It should be noted that each line denotes a new proccess with the
	information per being shown as follows:
	
	-----------------------------------------------------------------------------------------
	|  PID  |  requiredCPUTime  |  IOFrequency  |  IODuration  |  Priority  |  ArrivalTime  |
	-----------------------------------------------------------------------------------------
	
	With the use of SINGLE SPACES (' ') to denote where one parameter ends,
	and the next one begins. The values entered are to be encoded with standard
	8 bit ASCII numerical values, 0-9. The values entered are assumed to be
	a decimal number during runtime. The values for "requiredCPUTime", 
	"ioFrequency", "ioDuration" and "ArivalTime" are assumed to be in milliseconds.
	An example of a process with a PID of 98 and a required CPU time of 76ms,
	arriving at a time of 5ms after the simulation starts and a priority of 12 would be saved as:
	
	98 76 12 5
	
	IO duration is assumed to be in milliseconds, and the frequency is the number of milliseconds
	between each IO read. The IO reads are done at multiples of IO frequency, ie when time %
	IOFrequency == 0.

	Multiple processes are done in a similar fashion, with a new line between
	each process, such that:
	
	process_1	|	98 76 5 2
	process_2	|	5 73 2 10
	process_3	|	4 505 0 3
	process_4	|	9 6 80 5
	  ...		|	 ...
	process_40	|	6 28 2 6
	
	Additionally, please note that zero padding any number (ie writting
	5 as 05 or 0005) is not reccomended nor guarenteed to maintain functionality.
	There is a maximum number of allowed processes, with the max being 40.
	Exceding the maximun number of allowed processes will cause unexpected errors.
	
	It should also be noted each parameter field has a maximum number of characters
	of TEN (10) characters. Exceding this maximum will result in unexpected errors.

-----------------------------------------------------------
testBench.bash:
	
	testBench.bash has the capability to test all four of the programs just by being run
	
		It will echo into the appropriate files to run all of the programs, and run
		them one by one. It is not interactive and if the programs are not compiled
		and executable, it will not run correctly

