#! /bin/bash


SAMPLE_ARRAY1=()
OUTPATH=""
LOGDIR="./logs/"
if [ $# -lt 1 ]
then
    echo need at least one input, a string that is either \"DATA\" or \"MC\". 
    echo Second \(optional\) input is an integer that if it is \>0 resubmits the failed jobs.
    exit
fi

RESUB=0
if [ $# -ge 2 ]
    then
    RESUB=$2
fi


if [ $1 == "DATA" ]
    then
    echo "Checking data"
#    SAMPLE_ARRAY1=(DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1 DoublePhotonHighPt_Run2012B_13Jul2012  DoublePhotonHighPt_Run2012C_24Aug2012 DoublePhotonHighPt_Run2012C_PRv2 DoublePhotonHighPt_Run2012D_PRv1 Photon_Run2012A_13Jul2012 Photon_Run2012A_recover )  #ZjetsMadgraph WZPythia ZZPythia )
#    SAMPLE_ARRAY1=( DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1) 
    SAMPLE_ARRAY1=( DoubleMu_Run2012A_22Jan2013 DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013 Photon_Run2012A_22Jan2013 DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013)
    #OUTPATH="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Run2012/presel/"
    OUTPATH="/store/cmst3/group/exovv/CMGtuple/productionv2b/Run2012/"
    LOGDIR="./logs/data12/"
fi

if [ $1 == "MC" ]
    then
    echo "Checking MC"
#    SAMPLE_ARRAY1=(DYJetsPt70To100 DYJetsPt100 WW WZ ZZ TTBARpowheg)
#    SAMPLE_ARRAY1=( TTBARpowheg  DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 WW WZ ZZ WJetsPt100 BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c1p0_M1000 BulkG_ZZ_lljj_c1p0_M1500 BulkG_ZZ_lljj_c1p0_M600 RSG_ZZ_lljj_c0p05_M1000 RSG_ZZ_lljj_c0p2_M1000 RSG_ZZ_lljj_c0p2_M1500)
     SAMPLE_ARRAY1=(DYJetsPt70To100 DYJetsPt100 WW WZ ZZ TTBARpowheg BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500  BulkG_ZZ_lljj_M700_G40 BulkG_ZZ_lljj_M700_G80 BulkG_ZZ_lljj_M700_G120  BulkG_ZZ_lljj_M2000_G40 BulkG_ZZ_lljj_M2000_G80 BulkG_ZZ_lljj_M2000_G120  BulkG_ZZ_lljj_M1000_G40 BulkG_ZZ_lljj_M1000_G80 BulkG_ZZ_lljj_M1000_G120  BulkG_ZZ_lljj_M1500_G40 BulkG_ZZ_lljj_M1500_G80 BulkG_ZZ_lljj_M1500_G120 BulkG_ZZ_lljj_c0p2_M1700-JHU-herwig BulkG_ZZ_lljj_c0p2_M1400-JHU-herwig BulkG_ZZ_lljj_c0p2_M800-JHU-herwig BulkG_ZZ_lljj_c0p2_M1100-JHU-herwig)
    #OUTPATH="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Run2012/presel/"
    #OUTPATH="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1e/"
    OUTPATH="/store/cmst3/group/exovv/CMGtuple/productionv2b/Summer12/"
    LOGDIR="./logs/Summer12/"
fi




cd $LOGDIR

for sample in "${SAMPLE_ARRAY1[@]}"
do
  cd ./$sample
  if [ $? -ne 0 ]
      then
      echo "Directory $sample does not exist"
      continue
  fi

  echo I am in $(pwd):


  for job in $(/bin/ls -d Job*)
    do
    cd ./$job

    STATUS=0
    egrep exception -ir LSF*/* &> /dev/null
    if [ $? -eq 0 ]
	then 
	let STATUS=STATUS+1
    fi

    egrep segmentation -ir LSF*/* &> /dev/null
    if [ $? -eq 0 ]
	then 
	let STATUS=STATUS+1
    fi
 
   if [ $STATUS -gt 0 ]
	then
	echo 
	echo "+++"
	echo EXception in ./$sample/$job
	EXT=${job#"Job_"}
	if [ $RESUB -gt 0 ]
	    then
	    echo Removing ${OUTPATH}/${sample}/cmgTuple_${EXT}.root  
	    cmsRm ${OUTPATH}/${sample}/cmgTuple_${EXT}.root 
	    echo Resubmitting $job
	    bsub -q 8nh -J "${sample}_${job}" batchScript.sh
	    #rename dir with log for book-keeping
	    for lsfdir in $(/bin/ls -d LSF* )
	      do
	      mv $lsfdir old${lsfdir}
	    done
	fi

	echo "---"
    fi
    cd ../ &> /dev/null
  done

  cd ../

done
