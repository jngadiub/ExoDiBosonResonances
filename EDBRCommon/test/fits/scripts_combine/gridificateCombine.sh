#! /bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: ./gridificateCombine.sh <mass>"
    exit 1
fi

########## CHANGES GO HERE ##########
card="comb_xzz.txt"
channel="ZZ"
# To change number of toys and iterations, change run_fullCLs_TF.py
######### END CHANGES ##########

### Information
mass=$1
echo "Mass is $mass,   card is $card,   channel is $channel"

subdir=$mass

# copy the relevant files inside the mass subdirs
echo "Copying files inside subdir $subdir"
cp crab.cfg run_fullCLs_TF.py pset.py ${subdir}

# go inside the subdir and quickly run combine (asymptotic) 
# to get a handle of what the limit should be. In this way
# we avoid the big array of limits
echo "Running asymptotic limit to get a hint of where the limit should lie"
cd ${subdir}
ASYMPTOTICLIMIT=`combine ${card} -M Asymptotic 2> /dev/null | grep 'Observed Limit' | awk '{print $5}'`
cd -

echo "Preparing executable script for CRAB"
sed "s/sedMP/${mass}/g" run_fullCLs_TF.sh |\
sed "s/sedasympt/${ASYMPTOTICLIMIT}/g" |\
sed "s/sedcard/${card}/g"|\
sed "s/sedchannel/${channel}/g">\
$subdir/run_fullCLs_TF.sh

# go in the mass subdir, create and submit the job
echo "Submitting CRAB jobs"
cd ${subdir}
crab -create
crab -submit
cd -