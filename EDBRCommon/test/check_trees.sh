#!/bin/bash

TREEDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv4/test/fullsig

for line in $(ls $TREEDIR/logs)
do
	 log=${TREEDIR}/logs/${line}
	 echo checking $log
	 egrep '1 fileAction           -s' -r $log &> /dev/null
	 if [ $? != 0 ]
		then
		echo bad tree! plz check the log.
	fi

done	
