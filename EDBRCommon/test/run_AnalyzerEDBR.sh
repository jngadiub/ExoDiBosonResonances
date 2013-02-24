#! /bin/bash

MYWORKAREA=$CMSSW_BASE/src/
cd $MYWORKAREA
#eval `scram runtime -sh`
#cd /tmp/${USER}/


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

type="presel" #### could be full or presel or fullsig, denpends on analyzerEDBR_zz2l2j_tpl.py
OUTDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/test/$type/
mkdir -p $OUTDIR/logs

# ok, now do the job:
sed -e "s/<SAMPLE>/${TAG}/g" < ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/analyzerEDBR_zz2l2j_tpl.py >  ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_cfg.py
cd $OUTDIR
cmsRun  ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_cfg.py &> "logs/log_AnalyzerPresel_${type}_${TAG}.out"

echo
echo
echo "--------------" 
ls -lhrt
echo


#echo
#echo "Copying to $OUTDIR"
#echo
#cp *root $OUTDIR/
#cp *.out $OUTDIR/logs
