#! /bin/bash


SAMPLE_ARRAY1=()
OUTPATH=""
LOGDIR="./logs/"
if [ $# -lt 1 ]
then
    echo need at least one input, a string that is either \"DATA\" or \"MC\". 
    echo Second \(optional\) input is an integer that if it is \>0 resubmits the failed jobs.
    exit
fi

RESUB=0

if [ $# -ge 2 ]
    then
    RESUB=$2
fi


if [ $1 == "DATA" ]
    then
    echo "Checking data"
	SAMPLE_ARRAY1=(SingleMu_Run2012A-22Jan2013_xww SingleMu_Run2012B-22Jan2013_xww SingleMu_Run2012C-22Jan2013_xww SingleMu_Run2012D-22Jan2013_xww SingleElectron_Run2012A-22Jan2013_xww SingleElectron_Run2012B-22Jan2013_xww SingleElectron_Run2012C-22Jan2013_xww SingleElectron_Run2012D-22Jan2013_xww )
    OUTPATH="/store/cmst3/group/exovv/CMGtuple/shuai/production1011/Run2012/CA8/"
    LOGDIR="./logs/production1011/Run2012/CA8/"
fi

if [ $1 == "MC" ]
    then
    echo "Checking MC"
	SAMPLE_ARRAY1=(TTBARpowheg_xww WW_xww WZ_xww ZZ_xww WJetsPt100_xww WJetsPt180_xww DYJetsPt100_xww SingleTopBarSchannel_xww SingleTopBarTWchannel_xww SingleTopBarTchannel_xww SingleTopSchannel_xww SingleTopTWchannel_xww SingleTopTchannel_xww BulkG_WW_inclusive_c0p2_M600_xww BulkG_WW_inclusive_c0p2_M700_xww BulkG_WW_inclusive_c0p2_M800_xww BulkG_WW_inclusive_c0p2_M900_xww BulkG_WW_inclusive_c0p2_M1000_xww BulkG_WW_inclusive_c0p2_M1100_xww BulkG_WW_inclusive_c0p2_M1200_xww BulkG_WW_inclusive_c0p2_M1300_xww BulkG_WW_inclusive_c0p2_M1400_xww BulkG_WW_inclusive_c0p2_M1500_xww BulkG_WW_inclusive_c0p2_M1600_xww BulkG_WW_inclusive_c0p2_M1700_xww BulkG_WW_inclusive_c0p2_M1800_xww BulkG_WW_inclusive_c0p2_M1900_xww BulkG_WW_inclusive_c0p2_M2000_xww BulkG_WW_inclusive_c0p2_M2100_xww BulkG_WW_inclusive_c0p2_M2200_xww BulkG_WW_inclusive_c0p2_M2300_xww BulkG_WW_inclusive_c0p2_M2400_xww BulkG_WW_inclusive_c0p2_M2500_xww BulkG_WW_inclusive_M1000_W50_xww BulkG_WW_inclusive_M1000_W150_xww BulkG_WW_inclusive_M1000_W300_xww BulkG_WW_inclusive_M1500_W75_xww BulkG_WW_inclusive_M1500_W225_xww BulkG_WW_inclusive_M1500_W450_xww BulkG_WW_inclusive_M2100_W105_xww BulkG_WW_inclusive_M2100_W315_xww BulkG_WW_inclusive_M2100_W630_xww WprimeWZ_inclusive_M1000_PYTHIA6_xww WprimeWZ_inclusive_M1500_PYTHIA6_xww WprimeWZ_inclusive_M1800_PYTHIA6_xww WprimeWZ_inclusive_M2000_PYTHIA6_xww WprimeWZ_inclusive_M2200_PYTHIA6_xww WprimeWZ_inclusive_M2500_PYTHIA6_xww WprimeWZ_inclusive_M3000_PYTHIA6_xww  WprimeWZ_inclusive_M4000_PYTHIA6_xww WprimeWZ_inclusive_M750_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1000_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M1750_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1250_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M2000_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1500_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M750_PYTHIA6_xww)
    OUTPATH="/store/cmst3/group/exovv/CMGtuple/shuai/production1011/Summer12/CA8/"
    LOGDIR="./logs/production1011/Summer12/CA8/"
fi




cd $LOGDIR

for sample in "${SAMPLE_ARRAY1[@]}"
do
  cd ./$sample
  if [ $? -ne 0 ]
      then
      echo "Directory $sample does not exist"
      continue
  fi

  echo I am in $(pwd):


  for job in $(/bin/ls -d Job*)
    do
    cd ./$job

    STATUS=0
    egrep exception -ir LSF*/* &> /dev/null
    if [ $? -eq 0 ]
	then 
	let STATUS=STATUS+1
    fi

    egrep segmentation -ir LSF*/* &> /dev/null
    if [ $? -eq 0 ]
	then 
	let STATUS=STATUS+1
    fi
 
   if [ $STATUS -gt 0 ]
	then
	echo 
	echo "+++"
	echo EXception in ./$sample/$job
	EXT=${job#"Job_"}
	if [ $RESUB -gt 0 ]
	    then
	    echo Removing ${OUTPATH}/${sample}/cmgTuple_${EXT}.root  
	    cmsRm ${OUTPATH}/${sample}/cmgTuple_${EXT}.root 
	    echo Resubmitting $job
	    bsub -q 8nh -J "${sample}_${job}" batchScript.sh
	    #rename dir with log for book-keeping
	    for lsfdir in $(/bin/ls -d LSF* )
	      do
	      mv $lsfdir old${lsfdir}
	    done
	fi

	echo "---"
    fi
    cd ../ &> /dev/null
  done

  cd ../

done
