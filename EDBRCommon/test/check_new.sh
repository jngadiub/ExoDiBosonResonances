#!/bin/bash

####EDIT THIS PART#####

TREEDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/test/testnewsh/fullsideband/

RESUB=0

QUEUE=2nd

######################

for sample in $(ls ${TREEDIR}/logs)
do
	#echo '##########################'
	#echo Checking $sample
#check if there is a root file of this sample
	if [ -f ${TREEDIR}/treeEDBR_${sample}.root ]
		then
		#echo Output file found!  ${TREEDIR}/treeEDBR_${sample}.root
		log=$(ls ${TREEDIR}/logs/${sample}/*.out)
		#echo $log
		egrep '1 fileAction           -s' -r $log &> /dev/null
		if [ $? != 0 ]
			then
			echo '##########################'
			echo Checking $sample
			echo $log bad log! Please check the log.
			if test $RESUB -eq 1
				then
				echo resub this job
				cd ${TREEDIR}/logs/$sample
				bsub -q $QUEUE  run_new.sh  ${TREEDIR}/logs/$sample
			fi
		fi
	else
		echo '##########################'
		echo Checking $sample
		echo Output root file not found!
		if test $RESUB -eq 1
			then
			echo resub this job
			cd ${TREEDIR}/logs/$sample
			bsub -q $QUEUE  run_new.sh  ${TREEDIR}/logs/$sample
		fi
	fi

done	
