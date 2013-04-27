#! /bin/bash

myrand=$1
mass=$2
echo "Starting HiggsCombination with seed=$myrand at $( date +%c ) on $hostname."
startdir=$( pwd )

#--------------------------------------------------------------------------------------------------
# EDIT HERE

#set CMSSW environment
RELEASEDIR=/afs/cern.ch/work/s/santanas/Releases/CMSSW_5_3_7_StatisticsTools_ExoDiBosonResonances/src/

#algo="MarkovChainMC"
algo="Asymptotic"
#algo="ProfileLikelihood"
hint="ProfileLikelihood" # before the algo method, run the hint method for restricting integration field

label="EXOZZ"
ntoys=1000

WORKDIR=${RELEASEDIR}/CMGTools/StatTools/MacrosCombine-VV/Test_mWWFit_Sideband_23_04_2013/datacards/${mass}
BKGFILE=${RELEASEDIR}/CMGTools/StatTools/MacrosCombine-VV/Test_mWWFit_Sideband_23_04_2013/workspaces/Xvv.inputbkg_8TeV.root
SIGNALFILE=${RELEASEDIR}/CMGTools/StatTools/MacrosCombine-VV/Test_mWWFit_Sideband_23_04_2013/workspaces/Xvv.mX${mass}_WW_8TeV.inputsig.root

# choose datacard, can be on of those : 
# comb_xzz , comb_xzz_2l1JHP , comb_xzz_2l1JLP , comb_xzz_ee1J , comb_xzz_mm1J
# comb_xzz_2l1JELEHP, comb_xzz_2l1JELELP, comb_xzz_2l1JMUHP, comb_xzz_2l1JMULP
#--
#datacard="comb_xzz" 
datacard="comb_xzz_2l1JELELP" 

#--------------------------------------------------------------------------------------------------

OUTDIR="output_${label}_${algo}_"${datacard}

cd $RELEASEDIR
export SCRAM_ARCH=slc5_amd64_gcc462
#cmsenv
eval `scramv1 runtime -sh`
cd $startdir

TMPDIR="/tmp/$(whoami)"
mkdir ${TMPDIR}/combine_${myrand}
mkdir -p ${TMPDIR}/combine_${myrand}/workspaces
mkdir -p ${TMPDIR}/combine_${myrand}/datacards
cp $WORKDIR/"${datacard}.txt" ${TMPDIR}/combine_${myrand}/datacards/
cp $BKGFILE $SIGNALFILE ${TMPDIR}/combine_${myrand}/workspaces/  
cd $TMPDIR/combine_${myrand}/datacards
echo "I am in $( pwd ) (it should be: $TMPDIR/combine_${myrand}/datacards )"
echo

rm -f ${WORKDIR}/$OUTDIR/*

if [ ! -d ${WORKDIR}/$OUTDIR/ ]
    then
    mkdir -p ${WORKDIR}/$OUTDIR/
fi
echo "Datacard: $datacard"
# if algo=HybridNew
#combine -M $algo -n $label -m 400 -s $myrand -t $ntoys -U  -d $WORKDIR/$datacard --freq --singlePoint 1

#if algo="Asymptotic"  ###-t $ntoys
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
 #   minBoundary=0.0001

fi

## calc limit

# - with systematics
#combine -M $algo -n ${label} -m $mass  -s $myrand -d ${datacard}.txt -H $hint --rMax $maxBoundary --rMin $minBoundary
# - without systematics
combine -M $algo -n ${label} -m $mass  -s $myrand -d ${datacard}.txt -H $hint --rMax $maxBoundary --rMin $minBoundary -S 0
# - without systematics + pseudo data
#combine -M $algo -n ${label} -m $mass  -s $myrand -d ${datacard}.txt -H $hint --rMax $maxBoundary --rMin $minBoundary -S 0 -t 1

echo "Calculating the significances."

## calc exp signif
echo
echo "==== Expected Significance ====="
combine -M ProfileLikelihood -n ${label}ExpSignif -m $mass  -s $myrand --signif --pvalue --expectSignal=1 -t -1 --toysFreq -d $WORKDIR/"${datacard}.txt" 

## calc obs signif
echo
echo "==== Observed Significance ====="
combine -M ProfileLikelihood -n ${label}ObsSignif -m $mass  --signif --pvalue -d $WORKDIR/"${datacard}.txt" 

echo "List of files in $( pwd ):"
ls -lh
echo "Moving files into ${WORKDIR}/$OUTDIR/"
mv $TMPDIR/combine_${myrand}/datacards/higgsCombine${label}*.root  ${WORKDIR}/$OUTDIR/

