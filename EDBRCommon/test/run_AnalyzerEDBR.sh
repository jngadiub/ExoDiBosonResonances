#! /bin/bash

MYWORKAREA=/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/CMGTools/CMSSW_5_3_3_patch3/src/
cd $MYWORKAREA
eval `scram runtime -sh`
cd /tmp/${USER}/


TAG=""
if [ $# -gt 0 ]
    then
    TAG=$1
else
    echo "No input provided ! Script run_AnalyzerEDBR.sh requires one input argument, the label of the sample! Exiting."
    exit 1
fi

echo
echo "My working area is $MYWORKAREA"
echo

CFGAREA=${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/tmp_cfg/
mkdir -p $CFGAREA

OUTDIR=/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1/preselCA8/
mkdir -p $OUTDIR/logs

# ok, now do the job:
sed -e "s/<SAMPLE>/${TAG}/g" < ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/analyzerEDBR_zz2l2j_tpl.py >  ${CFGAREA}/analyzerEDBR_zz2l2j_${TAG}_cfg.py

cmsRun  ${CFGAREA}/analyzerEDBR_zz2l2j_${TAG}_cfg.py &> "log_AnalyzerPresel_${TAG}.out"

echo
echo
echo "--------------" 
ls -lhrt
echo


echo
echo "Copying to $OUTDIR"
echo
cp *root $OUTDIR/
cp *.out $OUTDIR/logs