#! /bin/bash

SAMPLE_Run2012MU=( SingleMu_Run2012A-22Jan2013 SingleMu_Run2012B-22Jan2013 SingleMu_Run2012C-22Jan2013 SingleMu_Run2012D-22Jan2013  ) 
SAMPLE_Run2012ELE=( SingleElectron_Run2012A-22Jan2013 SingleElectron_Run2012B-22Jan2013 SingleElectron_Run2012C-22Jan2013 SingleElectron_Run2012D-22Jan2013   )

SAMPLE_MC1=( TTBARpowheg WW WZ ZZ WJetsPt100 WJetsPt180  DYJetsPt100 SingleTopBarSchannel SingleTopBarTWchannel SingleTopBarTchannel SingleTopSchannel SingleTopTWchannel SingleTopTchannel   )  # MC background
SAMPLE_MC2=( BulkG_WW_inclusive_c0p2_M600 BulkG_WW_inclusive_c0p2_M700 BulkG_WW_inclusive_c0p2_M800 BulkG_WW_inclusive_c0p2_M900 BulkG_WW_inclusive_c0p2_M1000 BulkG_WW_inclusive_c0p2_M1100 BulkG_WW_inclusive_c0p2_M1200 BulkG_WW_inclusive_c0p2_M1300 BulkG_WW_inclusive_c0p2_M1400 BulkG_WW_inclusive_c0p2_M1500 BulkG_WW_inclusive_c0p2_M1600 BulkG_WW_inclusive_c0p2_M1700 BulkG_WW_inclusive_c0p2_M1800 BulkG_WW_inclusive_c0p2_M1900 BulkG_WW_inclusive_c0p2_M2000 BulkG_WW_inclusive_c0p2_M2100 BulkG_WW_inclusive_c0p2_M2200 BulkG_WW_inclusive_c0p2_M2300 BulkG_WW_inclusive_c0p2_M2400 BulkG_WW_inclusive_c0p2_M2500 BulkG_WW_inclusive_M1000_W50 BulkG_WW_inclusive_M1000_W150 BulkG_WW_inclusive_M1000_W300 BulkG_WW_inclusive_M1500_W75 BulkG_WW_inclusive_M1500_W225 BulkG_WW_inclusive_M1500_W450 BulkG_WW_inclusive_M2100_W105 BulkG_WW_inclusive_M2100_W315 BulkG_WW_inclusive_M2100_W630 WprimeWZ_inclusive_M1000_PYTHIA6 WprimeWZ_inclusive_M1500_PYTHIA6 WprimeWZ_inclusive_M1800_PYTHIA6 WprimeWZ_inclusive_M2000_PYTHIA6 WprimeWZ_inclusive_M2200_PYTHIA6 WprimeWZ_inclusive_M2500_PYTHIA6 WprimeWZ_inclusive_M3000_PYTHIA6  WprimeWZ_inclusive_M4000_PYTHIA6 WprimeWZ_inclusive_M750_PYTHIA6 RSG_WW_lvjj_c0p05_M1000_PYTHIA6  RSG_WW_lvjj_c0p05_M1750_PYTHIA6 RSG_WW_lvjj_c0p05_M1250_PYTHIA6  RSG_WW_lvjj_c0p05_M2000_PYTHIA6 RSG_WW_lvjj_c0p05_M1500_PYTHIA6  RSG_WW_lvjj_c0p05_M750_PYTHIA6 )  # MC signal

OUTPATHBASE="/store/cmst3/group/exovv/CMGtuple/shuai/production1011/"
OUTPATHDATA=${OUTPATHBASE}/Run2012/CA8/
OUTLOGPATHDATA=production1011/Run2012/CA8/
OUTPATHMC=${OUTPATHBASE}/Summer12/CA8/
OUTLOGPATHMC=production1011/Summer12/CA8/

cmsMkdir $OUTPATHMC
cmsMkdir $OUTPATHDATA

MYCMSSW_AREA=${CMSSW_BASE}/src
cd $MYCMSSW_AREA
eval `scram runtime -sh`
cd -


##############################
############# MC #############
##############################


sub_ind=0
for sample in "${SAMPLE_MC1[@]}"
  do

  LOGDIR=logs/${OUTLOGPATHMC}
  OUTDIR=${OUTPATHMC}/${sample}_xww
  NFILES=10
#  cmsMkdir  $OUTDIR
  QUEUE="8nh"


#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES XWW_main_cfg.py  --notagCVS -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"

   let sub_ind=$sub_ind +1

done


#-------------------------------------------

sub_ind=0
for sample in "${SAMPLE_MC2[@]}"
  do
  LOGDIR=logs/${OUTLOGPATHMC}
  OUTDIR=${OUTPATHMC}/${sample}_xww
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR


#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py 2  XWW_main_cfg.py  -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"
   let sub_ind=$sub_ind +1
done


################################
############# DATA #############
################################

sub_ind=0
for sample in "${SAMPLE_Run2012ELE[@]}"
  do
  LOGDIR=logs/${OUTLOGPATHDATA}
  OUTDIR=${OUTPATHDATA}/${sample}_xww
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR

  NFILES=15

  JSONFILE="dummy.json"
  if [[ "$sample" =~ "Photon" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "SingleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "SingleElectron" ]]
      then
	  JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  else
      echo "UNRECOGNIZED TYPE OF DATA SAMPLE ! "
      echo $sample
      echo"EXITING "
      exit 1
  fi

#for data
#  echo $JSONFILE
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES XWW_main_cfg.py   -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data12_${sample}_cff lepton=both selection=presel mcordata=DATASE json=${JSONFILE}"

   let sub_ind=$sub_ind +1
done


#-------------------------------------------

sub_ind=0
for sample in "${SAMPLE_Run2012MU[@]}"
  do
  LOGDIR=logs/${OUTLOGPATHDATA}
  OUTDIR=${OUTPATHDATA}/${sample}_xww
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR
  JSONFILE="dummy.json"
  NFILES=15


  JSONFILE="dummy.json"
  if [[ "$sample" =~ "Photon" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "SingleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "SingleElectron" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  else
      echo "UNRECOGNIZED TYPE OF DATA SAMPLE ! "
      echo $sample
      echo"EXITING "
      exit 1
  fi

#for MC
#  /afs/cern.ch/user/b/bonato/scratch0/PhysAnalysis/CMGTools/CMSSW_4_2_3/src/CMGTools/Common/scripts/cmsBatch2L2Q_MM2.py 15 HLLJJTemplateBoth_cff.py -o logs/${sample} -r ${OUTDIR} -b "bsub -q 1nd -J "cmg${sample}" < batchScript.sh" -c "infile=summer11_${sample}_cff lepton=both selection=full mcordata=MC"

#for data
###  echo $JSONFILE
  ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES XWW_main_cfg.py  -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data12_${sample}_cff lepton=both selection=presel mcordata=DATASM json=${JSONFILE}"  ##json=${JSONFILE}

   let sub_ind=$sub_ind +1

done
