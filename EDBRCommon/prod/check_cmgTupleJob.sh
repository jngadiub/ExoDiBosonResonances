#! /bin/bash


SAMPLE_ARRAY1=()
OUTPATH=""
LOGDIR="./logs/"
if [ $# -lt 1 ]
then
    echo need one input, a string that is either \"DATA\" or \"MC\"
    exit
fi

if [ $1 == "DATA" ]
    then
    echo "Checking data"
    SAMPLE_ARRAY1=(DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012D_PRv1 DoublePhotonHighPt_Run2012B_13Jul2012 DoublePhotonHighPt_Run2012C_24Aug2012 DoublePhotonHighPt_Run2012C_PRv2 DoublePhotonHighPt_Run2012D_PRv1 Photon_Run2012A_13Jul2012 Photon_Run2012A_recover )  #ZjetsMadgraph WZPythia ZZPythia )
    OUTPATH="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Run2012/presel/"
    LOGDIR="./logs/data12/"
fi

if [ $1 == "MC" ]
    then
    echo "Checking MC"
    SAMPLE_ARRAY1=(DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 TTBAR WZ ZZ)
    OUTPATH="/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/productionV1/Run2012/presel/"
    LOGDIR="./logs/Summer12/"
fi




cd $LOGDIR

for sample in "${SAMPLE_ARRAY1[@]}"
do
  cd ./$sample
  echo I am in $(pwd):


  for job in $(/bin/ls -d Job*)
    do
    cd ./$job
    egrep exception -ir LSF*/* &> /dev/null
    if [ $? == 0 ]
	then
	echo 
	echo "+++"
	echo EXception in ./$sample/$job
	EXT=${job#"Job_"}
	echo Removing ${OUTPATH}/${sample}/cmgTuple_${EXT}.root 
#cmsRm ${OUTPATH}/${sample}/cmgTuple_${EXT}.root 
#echo Submitting $job
	#bsub -q cmscaf1nd -J "${sample}_${job}" batchScript.sh


       #  for lsfdir in $(/bin/ls -d LSF* )
	#  do
	  #  mv $lsfdir old${lsfdir}
	 # done
	echo "---"
    fi
    cd ../ &> /dev/null
  done

  cd ../

done
