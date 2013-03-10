#!/bin/bash

####EDIT THIS PART#####

TREEDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/batchtree/test/presel

RESUB=0 ##use 1 to resubmit, 0 not to resubmit

QUEUE=2nd

MERGE=0  ##after everything is fine you can merge the output

######################

eval `scram runtime -sh`


for sample in $(ls ${TREEDIR}/logs)
do
	#echo '##########################'
	#echo Checking $sample
	cd ${TREEDIR}/logs/${sample}
	cd   OutCmsBatch_*
	#loop over the jobs
	for job in $(ls |  grep Job)
	do
		#echo $job
		#check if there is a root file 
		cd $job
		if [ -f treeEDBR_${sample}.root ]
			then
			#echo Output file found!  
			#check the output log of this file
			log=LSFJOB_*/STDOUT
			egrep '1 fileAction           -s' -r $log &> /dev/null
			if [ $? != 0 ] 
            	then
				echo '##########################'
				echo Checking $sample
				echo $job
            	echo $log bad log! Please check the log.
				if test $RESUB -eq 1
					then
					echo Resubmit this job...
					rm -f treeEDBR_${sample}.root
					bsub -q $QUEUE -J "${sample}_${job}" batchScript.sh
				fi
        	#else 
        	   #echo good file!
        	fi
		else
			echo '##########################'
			echo Checking $sample
			echo $job
			echo Output root file not found!
			if test $RESUB -eq 1
				then
				echo Resubmit this job...
				bsub -q $QUEUE -J "${sample}_${job}" batchScript.sh
			fi 
		fi  ##end of if there is a root file
		cd ..
	done  ##end of loop over jobs

	#merge the outputs
	if test $MERGE -eq 1
		then
		echo merge the output of $sample
		cd ${TREEDIR}
		hadd treeEDBR_${sample}.root logs/${sample}/OutCmsBatch_*/Job_*/treeEDBR_${sample}.root
	fi
done  ##end of loop of sample




