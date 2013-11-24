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
echo
echo
echo "====================================================="
echo "Mass is $mass,   card is $card,   channel is $channel"



subdir=$mass

# create the crab.cfg file
rm crab.cfg
echo '[CRAB]' > crab.cfg
echo 'jobtype = cmssw' >> crab.cfg
echo 'scheduler = remoteGlidein' >> crab.cfg
echo '' >> crab.cfg
echo '[CMSSW]' >> crab.cfg
echo 'datasetpath=none' >> crab.cfg
echo 'pset=pset.py' >> crab.cfg
echo 'events_per_job = 5000' >> crab.cfg
echo 'number_of_jobs = 56' >> crab.cfg
echo 'output_file = output.root' >> crab.cfg
echo '' >> crab.cfg
echo '[USER]' >> crab.cfg
echo 'return_data = 1' >> crab.cfg
echo 'additional_input_files='${card}',xzz*root,run_fullCLs_TF.py' >> crab.cfg
echo 'script_exe=run_fullCLs_TF.sh' >> crab.cfg


# copy the relevant files inside the mass subdirs
echo "Copying files inside subdir $subdir"
cp crab.cfg run_fullCLs_TF.py pset.py ${subdir}

# go inside the subdir and quickly run combine (asymptotic) 
# to get a handle of what the limit should be. In this way
# we avoid the big array of limits


#set ad hoc boundaries for Asymptotic, otherwise it crashes 
maxBoundary=5
minBoundary=0.005

#change range of scan specifically for BulkG->ZZ with c=0.5
if [ $mass -gt 2000 ]
    then
    maxBoundary=100000
    minBoundary=1
    echo "High mass $mass > 2000: boundary of combine is $minBoundary - $maxBoundary "
elif [ $mass -gt 1500 ]
    then
    maxBoundary=10000
    minBoundary=1
    echo "High mass $mass 1500-2000: boundary of combine is $minBoundary - $maxBoundary "
elif [ $mass -gt 1000 ]
    then
    maxBoundary=200
    minBoundary=0.1
    echo "Medium mass $mass 1000 - 1500: boundary of combine is $minBoundary - $maxBoundary "
else
    maxBoundary=50
    minBoundary=0.1
    echo "Low mass $mass <1000: boundary of combine is $minBoundary - $maxBoundary "
fi

cd ${subdir}
echo "Running asymptotic limit to get a hint of where the limit should lie"
#combine ${card} -M Asymptotic -m $mass --rMin $minBoundary --rMax $maxBoundary
ASYMPTOTICLIMIT=`combine ${card} -M Asymptotic -m $mass --rMin $minBoundary --rMax $maxBoundary 2> /dev/null | grep 'Observed Limit' | awk '{print $5}'`

if [ $? != 0 ]
    then
    echo "Something went wrong when running the asymptotic limit... Exiting"
    exit 100
fi

ASYMPTBAD=$( echo "$ASYMPTOTICLIMIT <= 0" | bc ) #use bc for floating point comparison

if [ $ASYMPTBAD -eq 1  ]
    then #it shouldn't be. Something went wrong
    echo "Error in calculation of Asymptotic limit. Observed limit was $ASYMPTOTICLIMIT ( -> ASYMPTBAD=${ASYMPTBAD}). Exiting."
    exit 101
else
    echo "Observed limit is valid: $ASYMPTOTICLIMIT"
fi

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