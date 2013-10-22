#! /bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: ./gridificateCombine.sh <mass> [width]"
    exit 1
fi

mass=$1
subdir=$mass

# copy the relevant files inside the mass subdirs
cp crab.cfg run_fullCLs_TF.py pset.py ${subdir}
sed "s/sedMP/${mass}/g" run_fullCLs_TF.sh > $subdir/run_fullCLs_TF.sh

# go in the mass subdir, create and submit the job
cd ${subdir}
crab -create
crab -submit
cd -