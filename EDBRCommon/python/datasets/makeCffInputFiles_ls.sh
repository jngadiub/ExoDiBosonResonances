#!/bin/sh

### Output directory for the lists ###

OUTPUTDIR=$USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

MAINDIRMC=/store/cmst3/group/exovv/CMGtuple/productionv2c/Summer12
MAINDIRDATA=/store/cmst3/group/exovv/CMGtuple/productionv2c/Run2012

### Make lists for DATA ###

#DoubleMu_Run2012A_22Jan2013 DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013 Photon_Run2012A_22Jan2013 DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013
### List for XZZ v2
for sample in 
do
  echo $MAINDIRDATA/$sample
  cmsLs $MAINDIRDATA/$sample | grep .root | awk '{print $5}'  > tmp.txt
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Make lists for MC ###

### List for XWW
#for sample in TTBAR_xww TTBARpowheg_xww WZ_xww ZZ_xww  WW_xww WJetsPt50To70_xww WJetsPt70To100_xww WJetsPt100_xww WJetsPt180_xww DYJetsPt50To70_xww DYJetsPt70To100_xww  DYJetsPt100_xww BulkG_WW_lvjj_c1p0_M1000_xww BulkG_WW_lvjj_c1p0_M1500_xww BulkG_WW_lvjj_c1p0_M600_xww RSG_WW_lvjj_c0p2_M1000_xww RSG_WW_lvjj_c0p2_M1500_xww RSG_WW_lvjj_c0p2_M600_xww SingleTopBarSchannel_xww SingleTopBarTWchannel_xww SingleTopBarTchannel_xww SingleTopSchannel_xww SingleTopTWchannel_xww SingleTopTchannel_xww BulkG_WW_lvjj_c0p2_M1000_xww BulkG_WW_lvjj_c0p2_M1100_xww  BulkG_WW_lvjj_c0p2_M1200_xww BulkG_WW_lvjj_c0p2_M1300_xww BulkG_WW_lvjj_c0p2_M1400_xww BulkG_WW_lvjj_c0p2_M1500_xww BulkG_WW_lvjj_c0p2_M1600_xww BulkG_WW_lvjj_c0p2_M1700_xww BulkG_WW_lvjj_c0p2_M1800_xww BulkG_WW_lvjj_c0p2_M1900_xww BulkG_WW_lvjj_c0p2_M2000_xww BulkG_WW_lvjj_c0p2_M2100_xww BulkG_WW_lvjj_c0p2_M2200_xww BulkG_WW_lvjj_c0p2_M2300_xww BulkG_WW_lvjj_c0p2_M2400_xww BulkG_WW_lvjj_c0p2_M2500_xww BulkG_WW_lvjj_c0p2_M600_xww BulkG_WW_lvjj_c0p2_M700_xww BulkG_WW_lvjj_c0p2_M800_xww BulkG_WW_lvjj_c0p2_M900_xww
### List for XZZ
# TTBAR DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 WW WZ ZZ BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c1p0_M1000 BulkG_ZZ_lljj_c1p0_M1500 BulkG_ZZ_lljj_c1p0_M600 RSG_ZZ_lljj_c0p05_M1000 RSG_ZZ_lljj_c0p2_M1000 RSG_ZZ_lljj_c0p2_M1500
### List for XZZ_v2
# 
for sample in DYJetsPt70To100 DYJetsPt100  TTBARpowheg WW WZ ZZ BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100  BulkG_ZZ_lljj_c0p2_M1200  BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500  BulkG_ZZ_lljj_c0p2_M1600  BulkG_ZZ_lljj_c0p2_M1700  BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900  BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500 BulkG_ZZ_lljj_c0p2_M1100-JHU-herwig   BulkG_ZZ_lljj_c0p2_M1400-JHU-herwig BulkG_ZZ_lljj_c0p2_M1700-JHU-herwig BulkG_ZZ_lljj_c0p2_M800-JHU-herwig


#for sample in DYJetsPt100 DYJetsPt70To100 TTBARpowheg WW WZ ZZ
do
  echo $MAINDIRMC/$sample
  cmsLs $MAINDIRMC/$sample | grep .root | awk '{print $5}'  > tmp.txt
  python makeCffInputFiles_ls.py tmp.txt $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Remove tmp file ###

rm -f tmp.txt
