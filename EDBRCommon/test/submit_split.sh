#! /bin/bash

########## EDIT THIS PART ################

SampleName=( TTBARpowheg_xww WW_xww WZ_xww ZZ_xww WJetsPt100_xww WJetsPt180_xww DYJetsPt100_xww SingleTopBarSchannel_xww SingleTopBarTWchannel_xww SingleTopBarTchannel_xww SingleTopSchannel_xww SingleTopTWchannel_xww SingleTopTchannel_xww BulkG_WW_inclusive_c0p2_M600_xww BulkG_WW_inclusive_c0p2_M700_xww BulkG_WW_inclusive_c0p2_M800_xww BulkG_WW_inclusive_c0p2_M900_xww BulkG_WW_inclusive_c0p2_M1000_xww BulkG_WW_inclusive_c0p2_M1100_xww BulkG_WW_inclusive_c0p2_M1200_xww BulkG_WW_inclusive_c0p2_M1300_xww BulkG_WW_inclusive_c0p2_M1400_xww BulkG_WW_inclusive_c0p2_M1500_xww BulkG_WW_inclusive_c0p2_M1600_xww BulkG_WW_inclusive_c0p2_M1700_xww BulkG_WW_inclusive_c0p2_M1800_xww BulkG_WW_inclusive_c0p2_M1900_xww BulkG_WW_inclusive_c0p2_M2000_xww BulkG_WW_inclusive_c0p2_M2100_xww BulkG_WW_inclusive_c0p2_M2200_xww BulkG_WW_inclusive_c0p2_M2300_xww BulkG_WW_inclusive_c0p2_M2400_xww BulkG_WW_inclusive_c0p2_M2500_xww BulkG_WW_inclusive_M1000_W50_xww BulkG_WW_inclusive_M1000_W150_xww BulkG_WW_inclusive_M1000_W300_xww BulkG_WW_inclusive_M1500_W75_xww BulkG_WW_inclusive_M1500_W225_xww BulkG_WW_inclusive_M1500_W450_xww BulkG_WW_inclusive_M2100_W105_xww BulkG_WW_inclusive_M2100_W315_xww BulkG_WW_inclusive_M2100_W630_xww SingleMu_Run2012A-22Jan2013_xww SingleMu_Run2012B-22Jan2013_xww SingleMu_Run2012C-22Jan2013_xww SingleMu_Run2012D-22Jan2013_xww SingleElectron_Run2012A-22Jan2013_xww SingleElectron_Run2012B-22Jan2013_xww SingleElectron_Run2012C-22Jan2013_xww SingleElectron_Run2012D-22Jan2013_xww WprimeWZ_inclusive_M1000_PYTHIA6_xww WprimeWZ_inclusive_M1500_PYTHIA6_xww WprimeWZ_inclusive_M1800_PYTHIA6_xww WprimeWZ_inclusive_M2000_PYTHIA6_xww WprimeWZ_inclusive_M2200_PYTHIA6_xww WprimeWZ_inclusive_M2500_PYTHIA6_xww WprimeWZ_inclusive_M3000_PYTHIA6_xww  WprimeWZ_inclusive_M4000_PYTHIA6_xww WprimeWZ_inclusive_M750_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1000_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M1750_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1250_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M2000_PYTHIA6_xww RSG_WW_lvjj_c0p05_M1500_PYTHIA6_xww  RSG_WW_lvjj_c0p05_M750_PYTHIA6_xww)

PROCESS=fullrange              ###processFullSel=="fullsb" or processFullSel=="fullsig" or processFullSel=="fullrange" or processFullSel=="ttbarcontrol" 

ANALYSIS=WW

OUTDIR=/afs/cern.ch/work/s/shuai/public/diboson/trees/productionv12/$PROCESS/

QUEUE=2nd

NFILES=20  #number of files to run in each job

##########################################

mkdir -p $OUTDIR
mkdir -p $OUTDIR/logs

echo OutDir is $OUTDIR

eval `scram runtime -sh`
MYWORKAREA=$CMSSW_BASE/src/
cd $MYWORKAREA
eval `scram runtime -sh`

for sample in  "${SampleName[@]}"
do
	echo '###################'
	echo Doing $sample
	 
	#create log dir for each sample
	mkdir -p $OUTDIR/logs/$sample
	echo $OUTDIR/logs/$sample created

	#create the cfg in the log dir of the sample
	cd $OUTDIR/logs/$sample
	cat ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/analyzerEDBR_zz2l2j_tpl.py  | sed -e s%"<SAMPLE>"%${sample}%g | sed -e s%"<ANALYSIS>"%${ANALYSIS}%g | sed -e s%"<PROCESS>"%${PROCESS}%g  > analyzerEDBR_${PROCESS}_${sample}_cfg.py
	echo Config file created

	#submit the job there, using cmsBatch
	${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/cmsBatch.py $NFILES analyzerEDBR_${PROCESS}_${sample}_cfg.py  -b "bsub -q "$QUEUE" -J "treeEDBR_${PROCESS}_${sample}" < batchScript.sh"
done

