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

CHANNEL="ZZ"
if [ $# -gt 1 ]
    then
    CHANNEL=$2

    if [[ ${CHANNEL} != "WW" && ${CHANNEL} != "ZZ" ]]
	then
	echo "Wrong analysis channel provided to run_AnalyzerEDBR.sh !!! You gave as input: ${CHANNEL}. Valid options are: WW ZZ"
	exit 2
    fi
fi

echo
echo "My working area is $MYWORKAREA"
echo

type="presel" #### could be: presel (default), fullsb, fullsig, fullrange, ttbar 
if [ $# -gt 2 ]
    then
    type=$3

    if [[ ${type} != "presel" && ${type} != "fullsb" && ${type} != "fullsig" && ${type} != "fullrange" && ${type} != "ttbar" ]]
	then
	echo "Wrong analysis channel provided to run_AnalyzerEDBR.sh !!! You gave as input: ${type}. Valid options are: presel fullsb fullsig fullrange ttbar"
	exit 3
    fi
fi

echo "Inputs to run_AnalyzerEDBR.sh: $TAG $CHANNEL $type"


CFGAREA=${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/tmp_cfg_${type}/
mkdir -p $CFGAREA


OUTDIR=/afs/cern.ch/user/b/bonato/work/PhysAnalysis/EXOVV_2012/analyzer_trees/productionv2g/${type}
mkdir -p $OUTDIR/logs

# ok, now do the job:
sed -e "s/<SAMPLE>/${TAG}/g" < ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/analyzerEDBR_zz2l2j_tpl.py >  ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_TMP.py
sed -e "s/<ANALYSIS>/${CHANNEL}/g" < ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_TMP.py >  ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_TMP2.py
sed -e "s/<PROCESS>/${type}/g" < ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_TMP2.py >  ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_cfg.py
rm -f ${CFGAREA}/analyzerEDBR_zz2l2j_${type}_${TAG}_TMP*.py
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
