#! /bin/bash

MYWORKAREA=$CMSSW_BASE/src/
cd $MYWORKAREA
eval `scram runtime -sh`
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



type="presel" #### could be fullsb or presel or fullsig, denpends on analyzerEDBR_zz2l2j_tpl.py

CFGAREA=${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/tmp_cfg_${type}/
mkdir -p $CFGAREA


OUTDIR=/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv1e/${type}
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
