#!/bin/bash

SIGNAL=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/fullsig
SIDEBAND=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/fullsideband
OUTDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/merged

mkdir -p $OUTDIR

for file in $(ls $SIGNAL | grep root)
do
	#echo $file
	root -l -q mergetree.C\(\"${SIGNAL}/${file}\",\"${SIDEBAND}/${file}\",\"${OUTDIR}/${file}\"\)
done
