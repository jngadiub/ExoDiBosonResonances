#!/bin/bash

SIGNAL=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/control/fullsig
SIDEBAND=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/control/fullsideband
OUTDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv5/control/merged

mkdir -p $OUTDIR

for file in $(ls $SIGNAL | grep root)
do
	echo $line
	root -l -q mergetree.C\(\"${SIGNAL}/${file}\",\"${SIDEBAND}/${file}\",\"${OUTDIR}/${file}\"\)
done
