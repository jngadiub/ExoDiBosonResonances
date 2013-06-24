#! /bin/bash

########## EDIT THIS PART ################

#SampleName=(DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013 DoubleMu_Run2012A_22Jan2013 DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013 Photon_Run2012A_22Jan2013)
#SampleName=(DYJetsPt100_v2 DYJetsPt70To100_v2 TTBARpowheg_v2 WW_v2 WZ_v2 ZZ_v2)
#SampleName=(BulkG_ZZ_lljj_c0p2_M600_v2 BulkG_ZZ_lljj_c0p2_M700_v2 BulkG_ZZ_lljj_c0p2_M800_v2 BulkG_ZZ_lljj_c0p2_M800_v2 BulkG_ZZ_lljj_c0p2_M1100_v2 BulkG_ZZ_lljj_c0p2_M1200_v2 BulkG_ZZ_lljj_c0p2_M1300_v2 BulkG_ZZ_lljj_c0p2_M1400_v2 BulkG_ZZ_lljj_c0p2_M1500_v2 BulkG_ZZ_lljj_c0p2_M1600_v2 BulkG_ZZ_lljj_c0p2_M1700_v2 BulkG_ZZ_lljj_c0p2_M1800_v2 BulkG_ZZ_lljj_c0p2_M1900_v2 BulkG_ZZ_lljj_c0p2_M2000_v2 BulkG_ZZ_lljj_c0p2_M2100_v2 BulkG_ZZ_lljj_c0p2_M2200_v2 BulkG_ZZ_lljj_c0p2_M2300_v2 BulkG_ZZ_lljj_c0p2_M2400_v2 BulkG_ZZ_lljj_c0p2_M2500_v2) 
SampleName=(BulkG_ZZ_lljj_c0p2_M1000_v2)
#SampleName=(WW_v2 WZ_v2 ZZ_v2)
type="presel" #### 'type' could be fullsideband or fullsig or presel, denpending on analyzerEDBR_zz2l2j_tpl.py
OUTDIR=/afs/cern.ch/user/t/tomei/work/public/EXOVV_2012/analyzer_trees/productionv2a/$type/

QUEUE=2nd

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
	sed -e "s/<SAMPLE>/${sample}/g" < ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/analyzerEDBR_zz2l2j_tpl.py >  analyzerEDBR_${type}_${sample}_cfg.py
	echo Config file created

	#submit the job there, and put the log also there
	sed -e s%'<sample>'%${sample}%g  < ${MYWORKAREA}/ExoDiBosonResonances/EDBRCommon/test/run_new.sh > run_new_temp.sh
	sed -e s%'<type>'%${type}%g  < run_new_temp.sh > run_new.sh
	rm run_new_temp.sh
	chmod 755 run_new.sh	
	bsub -q $QUEUE  -J "treeEDBR_${type}_${sample}"  run_new.sh  $OUTDIR
	#./run_new.sh
done

