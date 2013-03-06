#!/bin/bash

####EDIT THIS PART#####

TREEDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/test/testnewsh_ca8/fullsideband/

RESUB=1 ##use 1 to resubmit, 0 not to resubmit

QUEUE=2nd

######################

eval `scram runtime -sh`

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
				echo Resub this job
				cd ${TREEDIR}/logs/$sample
				bsub -q $QUEUE  run_new.sh  ${TREEDIR}
			fi	
		#else 
		#	echo good file!
		fi
	else
		echo '##########################'
		echo Checking $sample
		echo Output root file not found!
		if test $RESUB -eq 1
			then
			echo Resub this job
			cd ${TREEDIR}/logs/$sample
			bsub -q $QUEUE  run_new.sh  ${TREEDIR}
		fi
	fi

done	
