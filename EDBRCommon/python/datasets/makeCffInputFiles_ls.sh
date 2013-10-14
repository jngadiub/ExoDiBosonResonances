#!/bin/sh

### Output directory for the lists ###

OUTPUTDIR=$USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

MAINDIRMC=/store/cmst3/group/exovv/CMGtuple/shuai/production1011/Summer12/CA8/
MAINDIRDATA=/store/cmst3/group/exovv/CMGtuple/shuai/production1011/Run2012/CA8/

### Make lists for DATA ###

for sample in SingleMu_Run2012A-22Jan2013_xww SingleMu_Run2012B-22Jan2013_xww SingleMu_Run2012C-22Jan2013_xww SingleMu_Run2012D-22Jan2013_xww SingleElectron_Run2012A-22Jan2013_xww  SingleElectron_Run2012B-22Jan2013_xww SingleElectron_Run2012C-22Jan2013_xww SingleElectron_Run2012D-22Jan2013_xww 
do
  echo $MAINDIRDATA/$sample
  cmsLs $MAINDIRDATA/$sample | grep .root | awk '{print $5}'  > tmp.txt
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Make lists for MC ###

for sample in TTBARpowheg_xww WW_xww WZ_xww ZZ_xww WJetsPt100_xww WJetsPt180_xww DYJetsPt100_xww SingleTopBarSchannel_xww SingleTopBarTWchannel_xww SingleTopBarTchannel_xww SingleTopSchannel_xww SingleTopTWchannel_xww SingleTopTchannel_xww BulkG_WW_inclusive_c0p2_M600_xww BulkG_WW_inclusive_c0p2_M700_xww BulkG_WW_inclusive_c0p2_M800_xww BulkG_WW_inclusive_c0p2_M900_xww BulkG_WW_inclusive_c0p2_M1000_xww BulkG_WW_inclusive_c0p2_M1100_xww BulkG_WW_inclusive_c0p2_M1200_xww BulkG_WW_inclusive_c0p2_M1300_xww BulkG_WW_inclusive_c0p2_M1400_xww BulkG_WW_inclusive_c0p2_M1500_xww BulkG_WW_inclusive_c0p2_M1600_xww BulkG_WW_inclusive_c0p2_M1700_xww BulkG_WW_inclusive_c0p2_M1800_xww BulkG_WW_inclusive_c0p2_M1900_xww BulkG_WW_inclusive_c0p2_M2000_xww BulkG_WW_inclusive_c0p2_M2100_xww BulkG_WW_inclusive_c0p2_M2200_xww BulkG_WW_inclusive_c0p2_M2300_xww BulkG_WW_inclusive_c0p2_M2400_xww BulkG_WW_inclusive_c0p2_M2500_xww BulkG_WW_inclusive_M1000_W50_xww BulkG_WW_inclusive_M1000_W150_xww BulkG_WW_inclusive_M1000_W300_xww BulkG_WW_inclusive_M1500_W75_xww BulkG_WW_inclusive_M1500_W225_xww BulkG_WW_inclusive_M1500_W450_xww BulkG_WW_inclusive_M2100_W105_xww BulkG_WW_inclusive_M2100_W315_xww BulkG_WW_inclusive_M2100_W630_xww 
do
  echo $MAINDIRMC/$sample
  cmsLs $MAINDIRMC/$sample | grep .root | awk '{print $5}'  > tmp.txt
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Remove tmp file ###

rm -f tmp.txt
