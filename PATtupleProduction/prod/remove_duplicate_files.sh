#! /bin/bash

#EOSDIR="/eos/cms//store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/bonato/EDBR_PATtuple_edbr_zz_20130113_Summer12MC_TTBAR_MADGRAPH_20130115_153214/bonato/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/EDBR_PATtuple_edbr_zz_20130113/9c7d4b9d75f0c2737d58f798bbc008e6/"

if [ $# -lt 1 ]
then
 echo "Error! Provide the name of the directory on EOS to be processed (it must start with /eos/cms... ). Exiting. "
    exit 1
fi

EOSDIR=$1

source /afs/cern.ch/project/eos/installation/cms/etc/setup.sh
export PATH="$PATH:/afs/cern.ch/project/eos/installation/0.2.5/bin/"

DESTDIR="${EOSDIR}/duplicates/"
eos.select mkdir -p $DESTDIR
echo "Created $DESTDIR"




FILELIST1="list_totfiles_tmp.txt"

eos.select ls $EOSDIR | grep .root &> $FILELIST1

TOTFILES=$( cat $FILELIST1 | wc -l )
echo "Found $TOTFILES files in the EOS directory"

IND=1


for FILENAME in $( cat $FILELIST1 )
  do
  FILENAMELENGTH=${#FILENAME}
  LEN1=0
  let "LEN1 = $FILENAMELENGTH - 10" #1_sTR.root
  STEM=`expr substr $FILENAME 1 $LEN1`

  nfiles=$( eos.select ls $EOSDIR | grep ${STEM} | wc -l )
 
  if [ $nfiles -gt 1 ]
      then
      echo
      echo "$nfiles copies for ${STEM}"
      IND2=0
      while [ $IND2 -lt 1 ]
	do 
#	sed -n "/${STEM}_${IND}_/ p" < $FILELIST1
	FILENAME2=$( sed -n "/${STEM}/{p;q;}" < $FILELIST1 )
	echo "Moving  $FILENAME2   --->   ${DESTDIR}/${FILENAME2}"
### there is no mv command for eos. Replaced with first copy and then remove
	eos.select cp ${EOSDIR}/${FILENAME2} ${DESTDIR}/${FILENAME2} &> /dev/null
	if [ $? -eq 0 ] 
	    then
	    eos.select rm ${EOSDIR}/${FILENAME2}
	else
	    echo "Copy of file $FILENAME2 failed with exit code $? . Skipping to next file."
	fi

	let IND2=IND2+1
      done
      
  fi
  
  let IND=IND+1
  
done


echo "Finished"
