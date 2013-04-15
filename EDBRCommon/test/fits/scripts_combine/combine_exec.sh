#! /bin/bash

myrand=$1
mass=$2
OUTDIR=DataCards_XWW_V8_blind/
echo "Starting HiggsCombination with seed=$myrand at $( date +%c ) on $hostname."
export SCRAM_ARCH=slc5_amd64_gcc462
eval `scramv1 runtime -sh`

startdir=$( pwd )

#set CMSSW environment
RELEASEDIR=$CMSSW_BASE/src/

#algo="MarkovChainMC"
algo="Asymptotic"
#algo="ProfileLikelihood"
hint="ProfileLikelihood" # before the algo method, run the hint method for restricting integration field
label="EXOZZ"
ntoys=1000
#WORKDIR=${RELEASEDIR}/HiggsAna/HLLJJCommon/test/fits//${OUTDIR}/${mass}
WORKDIR=$CMSSW_BASE/src/ExoDiBosonResonances/EDBRCommon/test/fits/${OUTDIR}/${mass}
datacard="comb_xzz" 
OUTDIR="output_${label}_${algo}_"${datacard}

cd $RELEASEDIR
#cmsenv
cd $startdir

TMPDIR="/tmp/$(whoami)"
mkdir ${TMPDIR}/combine_${myrand}
cd $TMPDIR/combine_${myrand}
cp $WORKDIR/*input*root $WORKDIR/"${datacard}.txt" .
echo "I am in $( pwd ) (it should be: $TMPDIR/combine_${myrand} )"
echo


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
if [ $mass -gt 1500 ]
    then
    maxBoundary=1000
    minBoundary=1
    echo "High mass $mass > 1500: boundary of combine is $minBoundary - $maxBoundary "
elif [ $mass -gt 1000 ]
    then
    maxBoundary=100
    minBoundary=0.1
    echo "Medium mass $mass 1000 - 1500: boundary of combine is $minBoundary - $maxBoundary "
else
    maxBoundary=50
    minBoundary=0.1
    echo "Low mass $mass <1000: boundary of combine is $minBoundary - $maxBoundary "
 #   minBoundary=0.0001
   
fi

combine -M $algo -n ${label} -m $mass  -s $myrand -d ${datacard}.txt -H $hint --rMax $maxBoundary --rMin $minBoundary

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
mv $TMPDIR/combine_${myrand}/higgsCombine${label}*.root  ${WORKDIR}/$OUTDIR/

