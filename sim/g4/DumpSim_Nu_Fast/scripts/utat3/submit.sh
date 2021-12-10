#!/bin/bash

NRUNS=100
for (( a=1; a <= NRUNS; a++ ))
do
	/usr/bin/qsub -q normal -o log/qsub_run${a}_log.stdout -e err/qsub_run${a}_err.stderr -l walltime=24:00:00,cput=24:00:00 -N "run${a}" runscripts/run${a}.sh
done
