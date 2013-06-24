#! /bin/bash


#SAMPLE_Run2012MU=( DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1) 
#SAMPLE_Run2012ELE=( DoublePhotonHighPt_Run2012B_13Jul2012  DoublePhotonHighPt_Run2012C_24Aug2012 DoublePhotonHighPt_Run2012C_PRv2 DoublePhotonHighPt_Run2012D_PRv1 Photon_Run2012A_13Jul2012 Photon_Run2012A_recover)
#SAMPLE_Run2012MU=(DoubleMu_Run2012A_22Jan2013 DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013)
#SAMPLE_Run2012ELE=(DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013)
#SAMPLE_MC1=( TTBARpowheg  DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 WW WZ ZZ WJetsPt100) # MC Background
SAMPLE_MC2=( BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500)
#SAMPLE_MC1=(TTBARpowheg DYJetsPt70To100 DYJetsPt100 WW WZ ZZ)

#OUTPATHBASE="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/"
OUTPATHBASE="/store/cmst3/group/exovv/CMGtuple/productionv2a/"

OUTPATHDATA=${OUTPATHBASE}/Run2012/
OUTPATHMC=${OUTPATHBASE}/Summer12/
OUTLOGPATH="$(pwd)/logs"

cmsMkdir $OUTPATHMC
cmsMkdir $OUTPATHDATA

MYCMSSW_AREA=${CMSSW_BASE}/src
cd $MYCMSSW_AREA
eval `scram runtime -sh`
cd -

### MC BACKGROUND 
sub_ind=0
for sample in "${SAMPLE_MC1[@]}"
  do

  LOGDIR=logs/Summer12/
  OUTDIR=${OUTPATHMC}/${sample}
  NFILES=20
#  cmsMkdir  $OUTDIR
  QUEUE="8nh"

#for MC 
   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py  --notagCVS -o ${LOGDIR}/${sample} -r ${OUTDIR} -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"

   let sub_ind=$sub_ind +1

done

#### MC - SIGNAL 
sub_ind=0
for sample in "${SAMPLE_MC2[@]}"
  do
  LOGDIR=logs/Summer12/
  OUTDIR=${OUTPATHMC}/${sample}
  QUEUE="8nh"
  NFILES=15
 # cmsMkdir  $OUTDIR

   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py  $NFILES  EDBR_main_cfg.py  -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=summer12_${sample}_cff lepton=both selection=presel mcordata=MC"
   let sub_ind=$sub_ind +1
done



#### DATA (ele)
sub_ind=0
for sample in "${SAMPLE_Run2012ELE[@]}"
  do
  LOGDIR=logs/data12/
  OUTDIR=${OUTPATHDATA}/${sample}
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR

  NFILES=40

  JSONFILE="dummy.json"
  if [[ "$sample" =~ "Photon" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "SingleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/HiggsAna/HLLJJCommon/data/myjsonforsinglemu.json"
  else
      echo "UNRECOGNIZED TYPE OF DATA SAMPLE ! "
      echo $sample
      echo"EXITING "
      exit 1
  fi

   ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py   -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data12_${sample}_cff lepton=both selection=presel mcordata=DATAELE json=${JSONFILE}"

   let sub_ind=$sub_ind +1
done


#### DATA (mu)
sub_ind=0
for sample in "${SAMPLE_Run2012MU[@]}"
  do
  LOGDIR=logs/data12/
  OUTDIR=${OUTPATHDATA}/${sample}
  QUEUE="8nh"
 # cmsMkdir  $OUTDIR
  JSONFILE="dummy.json"
  NFILES=40


  JSONFILE="dummy.json"
  if [[ "$sample" =~ "Photon" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
  elif [[ "$sample" =~ "DoubleMu" ]]
      then
      JSONFILE="${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/data/goldenAnalysisJSON.txt"
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
###  echo $JSONFILE
  ${MYCMSSW_AREA}/ExoDiBosonResonances/EDBRCommon/prod/cmsBatch_EXOVV.py $NFILES EDBR_main_cfg.py  -o ${LOGDIR}/${sample} -r ${OUTDIR} --notagCVS -b "bsub -q "${QUEUE}" -J "cmg${sample}" < batchScript.sh" -c "infile=data12_${sample}_cff lepton=both selection=presel mcordata=DATAMU json=${JSONFILE}"  ##json=${JSONFILE}

   let sub_ind=$sub_ind +1

done