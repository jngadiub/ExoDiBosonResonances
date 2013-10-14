#! /bin/bash

### BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M1100  BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1800 

TYPE="fullsig"  ### presel fullsb fullsig fullrange ttbar
CHANNEL="ZZ" ###"WW" "ZZ"

SampleName=( RSG_ZZ_lljj_c0p2_M1500 RSG_ZZ_lljj_c0p2_M1000 RSG_ZZ_lljj_c0p05_M1000  ) ###DoubleMu_Run2012A_22Jan2013 DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013 Photon_Run2012A_22Jan2013 DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013  DYJetsPt70To100 DYJetsPt100 TTBARpowheg  DYJetsPt70To100 DYJetsPt100 WW WZ ZZ   BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500 ) 


#### DATA:
## DoubleMu_Run2012A_22Jan2013 DoubleMuParked_Run2012B_22Jan2013 DoubleMuParked_Run2012C_22Jan2013 DoubleMuParked_Run2012D_22Jan2013 Photon_Run2012A_22Jan2013 DoublePhotonHighPt_Run2012B_22Jan2013 DoublePhotonHighPt_Run2012C_22Jan2013 DoublePhotonHighPt_Run2012D_22Jan2013

#### SM bkgd MC:
#  TTBARpowheg  DYJetsPt70To100 DYJetsPt100 WW WZ ZZ   ### DYJetsPt50To70  WJetsPt100

#### Bulk G -> ZZ signal samples:
#  BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1200 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1600 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900 BulkG_ZZ_lljj_c0p2_M2000 BulkG_ZZ_lljj_c0p2_M2100 BulkG_ZZ_lljj_c0p2_M2200 BulkG_ZZ_lljj_c0p2_M2300 BulkG_ZZ_lljj_c0p2_M2400 BulkG_ZZ_lljj_c0p2_M2500 

#### alternative signal samples:
# BulkG_ZZ_lljj_c1p0_M600 BulkG_ZZ_lljj_c1p0_M1500 BulkG_ZZ_lljj_M700_G40 BulkG_ZZ_lljj_M700_G80 BulkG_ZZ_lljj_M700_G120  BulkG_ZZ_lljj_M2000_G40 BulkG_ZZ_lljj_M2000_G80 BulkG_ZZ_lljj_M2000_G120  BulkG_ZZ_lljj_M1000_G40 BulkG_ZZ_lljj_M1000_G80 BulkG_ZZ_lljj_M1000_G120  BulkG_ZZ_lljj_M1500_G40 BulkG_ZZ_lljj_M1500_G80 BulkG_ZZ_lljj_M1500_G120 BulkG_ZZ_lljj_c0p2_M1700-JHU-herwig BulkG_ZZ_lljj_c0p2_M1400-JHU-herwig BulkG_ZZ_lljj_c0p2_M800-JHU-herwig BulkG_ZZ_lljj_c0p2_M1100-JHU-herwig RSG_ZZ_lljj_c0p2_M1500 RSG_ZZ_lljj_c0p2_M1000 RSG_ZZ_lljj_c0p05_M1000



for sample in  "${SampleName[@]}"
do
echo "Submitting $sample"
bsub -q 8nh -J "treeEDBR_${sample}" run_AnalyzerEDBR.sh $sample $CHANNEL $TYPE
echo
done

#${CMSSW_BASE}/ExoDiBosonResonances/EDBRCommon/test/
