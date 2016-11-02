#!/bin/bash

$NONIOPCBDATA = "1 22 0
2 11 9
3 12 12
4 13 11
5 14 17
6 70 40
10 2 15"

echo "writing PCB data to PCBdata.fcfs"

echo $NONIOPCBDATA >> PCBdata.fcfs

echo "Data written, starting ./fcfsScheduler"

./fcfsScheduler #run it, must be executable

$IOPCBDATA = "1 22 10 1 0" #continue as neede

echo "writing PCB data to PCBdata.fcfsx"

echo $IOPCBDATA >> PCBdata.fcfsx

echo "Data written, starting ./fcfsSchedulerIO"

./fcfsSchedulerIO #run the IO one

