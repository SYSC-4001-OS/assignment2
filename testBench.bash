#!/bin/bash

arg=$1

write=$false

case $arg in
	-h | --help)
echo -e "This test bench tests four scripts:\n
	fcfsScheduler
	fcfsSchedulerIO
	priorityQueueScheduler
	priorityQueueSchedulerIO\n\n
The -w or --write option specifies that whatever is in testBench.bash (this script) should be written to the test files, those are:\n
1 22 0\n2 11 9\n3 12 12\n4 13 11\n5 14 17\n6 70 40\n10 2 15\nFor fcfsScheduler\n\n
1 22 10 1 0\n2 12 8 2 10\n4 15 5 1 45\n10 100 20 8 80\n12 8 5 1 20\nFor fcfsSchedulerIO\n\n
1 22 4 0\n2 11 6 9\n3 12 3 12\n4 13 12 11\n5 14 8 17\n6 70 10 40\n10 2 45 15\nFor priorityQueueScheduler\n\n
1 40 10 2 1 10\n2 100 20 10 2 0\n3 25 5 2 3 0\n10 50 12 6 10 0\n12 20 10 50 5 0\n8 25 10 12 5 0\nFor priorityQueueSchedulerIO\n
See the README for more information on what these numbers mean\n\n
This can be useful for cleaning up files\nOtherwise, whatever is in files PCBData.fcfs, PCBData.fcfsx, PCBData.dat and PCBData.datx is used."
exit 0 ;;
	-w | --write)
		echo "*******************************
write enabled, writing to files
*******************************"
		write=$true
	;;
	*)
	;;
esac

if $write ; then
NONIOPCBDATA="1 22 0\n2 11 9\n3 12 12\n4 13 11\n5 14 17\n6 70 40\n10 2 15"

echo -e "writing PCB data to PCBdata.fcfs\n"

echo -e $NONIOPCBDATA > PCBdata.fcfs

echo -e "Data written, starting ./fcfsScheduler\n\n"
else
echo -e "Using data from PCBData.fcfs"
fi

./fcfsScheduler #run it, must be executable
if $write ; then
IOPCBDATA="1 22 10 1 0\n2 12 8 2 10\n4 15 5 1 45\n10 100 20 8 80\n12 8 5 1 20"

echo -e "\n\nwriting PCB data to PCBdata.fcfsx\n"

echo -e $IOPCBDATA > PCBdata.fcfsx

echo -e "Data written, starting ./fcfsSchedulerIO\n\n"
else
echo -e "Using data from PCBData.fcfsx, starting ./fcfsSchedulerIO\n\n"
fi

./fcfsSchedulerIO #run the IO one

if $write ; then
NONIOPCBDATA="1 22 4 0\n2 11 6 9\n3 12 3 12\n4 13 12 11\n5 14 8 17\n6 70 10 40\n10 2 45 15"

echo -e "\n\nwriting PCB data to PCBdata.dat\n"

echo -e $NONIOPCBDATA > PCBdata.dat

echo -e "Data written, starting ./priorityQueueScheduler\n\n"
else
echo -e "Using data from PCBData.dat, starting ./priorityQueueScheduler\n\n"
fi

./priorityQueueScheduler

if $write ; then
IOPCBDATA="1 40 10 2 1 10\n2 100 20 10 2 0\n3 25 5 2 3 0\n10 50 12 6 10 0\n12 20 10 50 5 0\n8 25 10 12 5 0"

echo -e "\n\nwriting PCB data to PCBdata.datx\n"

echo -e $IOPCBDATA > PCBdata.fcfsx

echo -e "Data written, starting ./prioritySchedulerIO\n\n"
else
echo -e "Using data from PCBData.datx"
fi

./priorityQueueSchedulerIO