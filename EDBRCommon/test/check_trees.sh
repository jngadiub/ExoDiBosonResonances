#!/bin/bash

#TREEDIR=/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1b/preselCA8XCheck/

TREEDIR=/afs/cern.ch/work/s/santanas/Workspace/EXOVV_2012/ntuples/WW_04_03_2013/full/

for line in $(ls $TREEDIR/logs)
do
	 log=${TREEDIR}/logs/${line}
	 echo checking $log
	 egrep '1 fileAction           -s' -r $log &> /dev/null
	 if [ $? != 0 ]
		then
	     
		echo $log bad tree! plz check the log.
		echo
	fi

done	
