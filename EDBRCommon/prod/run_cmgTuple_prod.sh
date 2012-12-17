#! /bin/bash


SAMPLE_Run2012A=() #Run2012A_DoubleElectron Run2012A_DoubleMu 
SAMPLE_Run2012B=() #Run2012B_DoubleElectron Run2012B_DoubleMu 


SAMPLE_MC1=( DYJetsPt70To100  )  # MC background
SAMPLE_MC2=( ) #MC signal: JHUGrav300 
OUTPATHBASE="/store/cmst3/user/bonato/cmgTuple/2012/"
OUTPATHDATA=${OUTPATHBASE}/Run2012/presel/
OUTPATHMC=${OUTPATHBASE}/Summer12/presel/
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
  OUTDIR=${OUTPATHMC}/${sample}
  NFILES=10
#  cmsMkdir  $OUTDIR
  QUEUE="8nh"

#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py  --notagCVS -o ${LOGDIR}/${sample} -r ${OUTDIR} -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC maxEvents=1000"

   let sub_ind=$sub_ind +1

done

sub_ind=0
for sample in "${SAMPLE_MC2[@]}"
  do
  LOGDIR=logs/Summer12/
  OUTDIR=${OUTPATHMC}/${sample}
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR


#for MC
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py 2  EDBR_main_cfg.py  -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer11_${sample}_cff lepton=both selection=full mcordata=MC"
   let sub_ind=$sub_ind +1
done

sub_ind=0
for sample in "${SAMPLE_Run2012A[@]}"
  do
  LOGDIR=logs/data12/
  OUTDIR=${OUTPATHDATA}/${sample}
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR

  NFILES=15

  JSONFILE="dummy.json"
  if [[ "$sample" =~ "DoubleElectron" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v2.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v2.txt"
  elif [[ "$sample" =~ "SingleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/myjsonforsinglemu.json"
  else
      echo "UNRECOGNIZED TYPE OF DATA SAMPLE ! "
      echo $sample
      echo"EXITING "
      exit 1
  fi

#for data
#  echo $JSONFILE
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py   -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data_${sample}_cff lepton=both selection=presel mcordata=DATA json=${JSONFILE}"

   let sub_ind=$sub_ind +1
done

sub_ind=0
for sample in "${SAMPLE_Run2012B[@]}"
  do
  LOGDIR=logs/data12/
  OUTDIR=${OUTPATHDATA}/${sample}
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR
  JSONFILE="dummy.json"
  NFILES=15

  if [[ "$sample" =~ "DoubleElectron" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v2.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v2.txt"
  elif [[ "$sample" =~ "SingleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/myjsonforsinglemu.json"
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
  ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py  -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data_${sample}_cff lepton=both selection=presel mcordata=DATA json=${JSONFILE}"  ##json=${JSONFILE}

   let sub_ind=$sub_ind +1

done