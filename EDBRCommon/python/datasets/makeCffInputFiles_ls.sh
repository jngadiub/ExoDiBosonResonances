#!/bin/sh


export eosCMD='/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select'

### Output directory for the lists ###

OUTPUTDIR=$USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

MAINDIRMC=/store/cmst3/group/exovv/CMGtuple/productionv2i/Summer12/
MAINDIRDATA=/store/cmst3/group/exovv/CMGtuple/shuai/production1011/Run2012/CA8/

### Make lists for DATA ###

for sample in  
do
  echo $MAINDIRDATA/$sample
####  cmsLs $MAINDIRDATA/$sample | grep .root | awk '{print $5}'  > tmp.txt
####  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
  for file in $( $eosCMD ls $MAINDIRDATA/$sample | grep .root)
    do 
    echo ${MAINDIRDATA}/${sample}/${file} >> tmp.txt
  done
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
  rm -f tmp.txt
done

### Make lists for MC ###

###WW ZZ WZ BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500 RSG_ZZ_lljj_c0p05_M750_PYTHIA6 RSG_ZZ_lljj_c0p05_M1000_PYTHIA6 RSG_ZZ_lljj_c0p05_M1250_PYTHIA6 RSG_ZZ_lljj_c0p05_M1500_PYTHIA6 RSG_ZZ_lljj_c0p05_M1750_PYTHIA6 RSG_ZZ_lljj_c0p05_M2000_PYTHIA6 WprimeWZ_inclusive_M750_PYTHIA6 WprimeWZ_inclusive_M1000_PYTHIA6 WprimeWZ_inclusive_M1500_PYTHIA6 WprimeWZ_inclusive_M2000_PYTHIA6 WprimeWZ_inclusive_M2200_PYTHIA6 WprimeWZ_inclusive_M2500_PYTHIA6 RSG_ZZ_lljj_c0p05_M1000 RSG_ZZ_lljj_c0p2_M1000 RSG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1000-JHU-HERWIGpp BulkG_ZZ_lljj_c0p2_M1800-JHU-HERWIGpp BulkG_ZZ_lljj_c0p2_M2500-JHU-HERWIGpp 

### BulkG_ZZ_lljj_M700_G40 BulkG_ZZ_lljj_M700_G80 BulkG_ZZ_lljj_M700_G120   BulkG_ZZ_lljj_M2000_G40 BulkG_ZZ_lljj_M2000_G80 BulkG_ZZ_lljj_M2000_G120  BulkG_ZZ_lljj_M2000_G160 BulkG_ZZ_lljj_M2000_G200 BulkG_ZZ_lljj_M1000_G40 BulkG_ZZ_lljj_M1000_G80  BulkG_ZZ_lljj_M1000_G120  BulkG_ZZ_lljj_M1500_G40 BulkG_ZZ_lljj_M1500_G80 BulkG_ZZ_lljj_M1500_G120

for sample in
  do
  echo $MAINDIRMC/$sample
#  cmsLs $MAINDIRMC/$sample | grep .root | awk '{print $5}'  > tmp.txt
  for file in $( $eosCMD ls $MAINDIRMC/$sample | grep .root)
    do 
    echo ${MAINDIRMC}/${sample}/${file} >> tmp.txt
  done
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py

### Remove tmp file ###
  rm -f tmp.txt

done

