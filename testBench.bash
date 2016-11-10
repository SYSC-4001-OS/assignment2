#!/bin/bash

NONIOPCBDATA="1 22 0
2 11 9
3 12 12
4 13 11
5 14 17
6 70 40
10 2 15"

echo "writing PCB data to PCBdata.fcfs\n"

echo $NONIOPCBDATA >> PCBdata.fcfs

echo "Data written, starting ./fcfsScheduler\n\n"

./fcfsScheduler #run it, must be executable

IOPCBDATA="1 22 10 1 0
2 12 8 2 10
4 15 5 1 45
10 100 20 8 80
12 8 5 1 20"

echo "\n\nwriting PCB data to PCBdata.fcfsx\n"

echo $IOPCBDATA >> PCBdata.fcfsx

echo "Data written, starting ./fcfsSchedulerIO\n\n"

./fcfsSchedulerIO #run the IO one

IOPCBDATA="1 40 10 2 1 10
2 100 20 10 2 0
3 25 5 2 3 0
10 50 12 6 10 0
12 20 10 50 5 0
8 25 10 12 5 0"

echo "\n\nwriting PCB data to PCBdata.datx\n"

echo $IOPCBDATA >> PCBdata.fcfsx

echo "Data written, starting ./prioritySchedulerIO\n\n"

./priorityQueueSchedulerIO
