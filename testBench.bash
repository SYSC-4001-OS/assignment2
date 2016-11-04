#!/bin/bash

NONIOPCBDATA="1 22 0\n2 11 9\n3 12 12\n4 13 11\n5 14 17\n6 70 40\n10 2 15"

echo -e "writing PCB data to PCBdata.fcfs\n"

echo -e $NONIOPCBDATA > PCBdata.fcfs

echo -e "Data written, starting ./fcfsScheduler\n\n"

./fcfsScheduler #run it, must be executable

IOPCBDATA="1 22 10 1 0\n2 12 8 2 10\n4 15 5 1 45\n10 100 20 8 80\n12 8 5 1 20\n"

echo -e "\n\nwriting PCB data to PCBdata.fcfsx\n"

echo -e $IOPCBDATA > PCBdata.fcfsx

echo -e "Data written, starting ./fcfsSchedulerIO\n\n"

./fcfsSchedulerIO #run the IO one

IOPCBDATA="1 40 10 2 1 10\n2 100 20 10 2 0\n3 25 5 2 3 0\n10 50 12 6 10 0\n12 20 10 50 5 0\n8 25 10 12 5 0"

echo -e "\n\nwriting PCB data to PCBdata.datx\n"

echo -e $IOPCBDATA > PCBdata.fcfsx

echo -e "Data written, starting ./prioritySchedulerIO\n\n"

./priorityQueueSchedulerIO
