#! /bin/bash


SAMPLE_Run2012MU=( SingleMu_Run2012A_13Jul2012  SingleMu_Run2012A_recover SingleMu_Run2012B_13Jul2012 SingleMu_Run2012C_24Aug2012 SingleMu_Run2012C_PromptReco SingleMu_Run2012D_PromptReco  ) 
SAMPLE_Run2012ELE=( SingleElectron_Run2012A_13Jul2012 SingleElectron_Run2012A_recover SingleElectron_Run2012B_13Jul2012 SingleElectron_Run2012C_24Aug2012 SingleElectron_Run2012C_PromptReco SingleElectron_Run2012D_PromptReco  )


SAMPLE_MC1=( TTBAR WW WZ ZZ WJetsPt50To70 WJetsPt70To100 WJetsPt100 DYJetsPt50To70 DYJetsPt70To100  DYJetsPt100   )  # MC background
SAMPLE_MC2=( BulkG_WW_lvjj_c1p0_M1000  BulkG_WW_lvjj_c1p0_M600 BulkG_WW_lvjj_c1p0_M1500   RSG_WW_lvjj_c0p2_M1000  RSG_WW_lvjj_c0p2_M600 RSG_WW_lvjj_c0p2_M1500   ) #MC signal: JHUGrav300 
OUTPATHBASE="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/shuai/production0224/"
OUTPATHDATA=${OUTPATHBASE}/Run2012/preselCA8/
OUTPATHMC=${OUTPATHBASE}/Summer12/preselCA8/
OUTLOGPATH="$(pwd)/logs"

cmsMkdir $OUTPATHMC
cmsMkdir $OUTPATHDATA

MYCMSSW_AREA=${CMSSW_BASE}/src
cd $MYCMSSW_AREA
eval `scram runtime -sh`
cd -

sub_ind=0

for sample in "${SAMPLE_MC1[@]}"
  do

  LOGDIR=logs/Summer12/
  OUTDIR=${OUTPATHMC}/${sample}_xww
  NFILES=10
#  cmsMkdir  $OUTDIR
  QUEUE="8nh"

#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES XWW_main_cfg.py  --notagCVS -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"

   let sub_ind=$sub_ind +1

done

sub_ind=0
for sample in "${SAMPLE_MC2[@]}"
  do
  LOGDIR=logs/Summer12/
  OUTDIR=${OUTPATHMC}/${sample}_xww
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR


#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py 2  XWW_main_cfg.py  -o ${LOGDIR}/${sample}_xww -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"
   let sub_ind=$sub_ind +1
done

sub_ind=0
for sample in "${SAMPLE_Run2012ELE[@]}"
  do
  LOGDIR=logs/data12/
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

sub_ind=0
for sample in "${SAMPLE_Run2012MU[@]}"
  do
  LOGDIR=logs/data12/
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
