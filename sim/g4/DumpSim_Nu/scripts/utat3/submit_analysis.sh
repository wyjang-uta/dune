#!/bin/bash
#
# script for submission of dump simulation analysis

QSUBEXEC=/usr/bin/qsub
QUEUENAME=normal
LOGDIR=analysis_log
LOGFPREFIX=qsub_run
LOGFSUFFIX=_log.stdout
ERRDIR=analysis_err
ERRFPREFIX=qsub_run
ERRFSUFFIX=_err.stderr
NRUNS=100

for (( i=1;i<=NRUNS;i++))
do
	${QSUBEXEC} -q ${QUEUENAME}\
		-o ${LOGDIR}/${LOGFPREFIX}${i}${LOGFSUFFIX}\
		-e ${ERRDIR}/${ERRFPREFIX}${i}${ERRFSUFFIX}\
		-l walltime=24:00:00,cput=24:00:00\
		-N "DUNE_DumpSim_run${i}"\
		analysis_scripts/run${i}.sh
done
