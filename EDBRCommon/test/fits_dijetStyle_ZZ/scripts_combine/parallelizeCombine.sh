#! /bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: ./parallelizeCombine.sh <mass> [LXB queue]"
    exit 1
fi

#-------------------------------------------------------------------
#EDIT HERE

RUN_LOCALLY=1 #if 1 -> do not submit, run locally

#-------------------------------------------------------------------

startdir=$( pwd )
NMAXJOBS=1
mass=$1
queue="dummyqueue"
OUTDIR=$startdir
LOGDIRNAME="logs"
mkdir -p ${OUTDIR}/${mass}/${LOGDIRNAME}
rm -f ${OUTDIR}/${mass}/${LOGDIRNAME}/*

if [ $# -gt 1 ]
then
    queue=$2
else
    queue=1nh
fi

ijob=1
chmod 744 combine_exec.sh

echo "Running combine for mass: " ${mass}

while [ $ijob -le $NMAXJOBS ]
do
  myrand=$RANDOM #random number generator (short integer: [0-32767])
  JOBNAME="combine_${myrand}"
  LOGFILE="${LOGDIRNAME}/log_batch_combine_${myrand}.out"

  if [ $RUN_LOCALLY -eq 1 ]
      then
      ${startdir}/combine_exec.sh $myrand $mass >& ${OUTDIR}/${mass}/$LOGFILE
      #${startdir}/combine_exec.sh $myrand $mass
 
  else
      bsub -q $queue -J $JOBNAME -oo ${OUTDIR}/${mass}/$LOGFILE ${startdir}/combine_exec.sh $myrand $mass
  fi
  let ijob=ijob+1
done


