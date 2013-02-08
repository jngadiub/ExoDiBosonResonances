#!/bin/bash
#source /afs/cern.ch/project/eos/installation/client/etc/setup.sh
source /afs/cern.ch/project/eos/installation/cms/etc/setup.sh
export PATH="$PATH:/afs/cern.ch/project/eos/installation/0.2.5/bin/"

INPUT=$1
echo mkdir $INPUT/duplicates
eos.select mkdir -p $INPUT/duplicates

echo "deleted files" > deletedfiles.txt

for file in $(eos.select ls $INPUT )
do
	echo "####################################"
	echo "check file:"
	folder=$INPUT/
	echo ${folder}${file}
	let "len2=${#file}-10"   ##..61_3_b14.root->...61_
	filetosearch=`expr substr $file 1 $len2`
	dufiles=$( eos.select ls $folder | grep ${filetosearch} )
	success=0
	nfiles=$( eos.select ls $folder | grep ${filetosearch} | wc -l )
	if test $nfiles -eq 1
		then
		continue
	fi
	echo duplicated files $nfiles
	for dufile in $dufiles
	do
		dufilepfn="root://eoscms//eos/cms"${folder}${dufile}
		dufilelfn=${folder}${dufile}
		echo $dufilepfn
		if test $success -eq 1
			then
			#echo cmsRm $dufilelfn
			#cmsRm $dufilelfn            ###################action####################
			echo cmsStage $dufilelfn ${folder}duplicates
			echo cmsRm $dufilelfn	
			cmsStage $dufilelfn ${folder}duplicates
			cmsRm $dufilelfn
			continue
		fi
		out=$( python testrootfile.py  $dufilepfn )
#		echo $out
		nline=0		
		for tmp in $out
		do
		    if test $nline -eq 4 && test $tmp -eq 1
		        then
		        echo This is a good file!
				success=1
		    fi  
		    if test $nline -eq 4 && test $tmp -eq 0
		        then
		        echo Bad file!
				#echo cmsRm $dufilelfn
				#cmsRm $dufilelfn             #####################action#################
				echo cmsStage $dufilelfn ${folder}duplicates
				echo cmsRm $dufilelfn
				cmsStage $dufilelfn ${folder}duplicates
            	cmsRm $dufilelfn
		    fi  
		    let "nline=$nline+1"
		done
	done
	if test $success -eq 0
		then
		echo $file  >> deletedfiles.txt
	fi
	echo ""

done
