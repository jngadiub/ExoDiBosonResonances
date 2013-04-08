#!/bin/bash

####EDIT THIS PART#####

TREEDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv7_newMJ/fullallrange/

##use 1 to resubmit, 0 not to resubmit
RESUB=0 

QUEUE=2nd

##after everything is fine you can merge the output using MERGE=1
MERGE=0

##option for WW, to merege similar smaples using MERGESAMPLE=1. NOTE: check the last part for safety
MERGESAMPLE=0

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



if test $MERGESAMPLE -eq 1
	then
	echo merging similar samples for plotting:
	cd ${TREEDIR}
	hadd treeEDBR_SingleTop_xww.root treeEDBR_SingleTop*
	hadd treeEDBR_VV_xww.root treeEDBR_WW_xww.root treeEDBR_WZ_xww.root treeEDBR_ZZ_xww.root
	hadd treeEDBR_DYJets_xww.root treeEDBR_DYJetsP*
fi



