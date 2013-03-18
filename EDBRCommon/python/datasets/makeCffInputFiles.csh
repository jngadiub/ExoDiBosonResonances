#!/bin/csh

### Output directory for the lists ###

set OUTPUTDIR = $USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

set MAINDIRDATA = /store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Run2012/CA8/
set MAINDIRMC =   /store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/tomei_edbr_vv_20130313/Summer12/CA8/

### Create tmp file ###

touch tmp.txt

### Make lists for DATA ###

#foreach sample (DoublePhotonHighPt_Run2012B_13Jul2012 DoublePhotonHighPt_Run2012C_24Aug2012 DoublePhotonHighPt_Run2012C_PRv2 DoublePhotonHighPt_Run2012D_PRv1 Photon_Run2012A_13Jul2012 Photon_Run2012A_recover DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1)
#echo $MAINDIRDATA/$sample
#cmsLs $MAINDIRDATA/$sample | grep root | awk '{print $5}' > ! tmp.txt
#python makeCffInputFiles.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
#end

### Make lists for MC ###

#foreach sample (DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 TTBAR WW WZ ZZ)
#echo $MAINDIRMC/$sample
#cmsLs $MAINDIRMC/$sample | grep root | awk '{print $5}' > ! tmp.txt
#python makeCffInputFiles.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
#end

### Make lists fors signal ###

foreach sample (BulkG_ZZ_lljj_c0p2_M600 BulkG_ZZ_lljj_c0p2_M700 BulkG_ZZ_lljj_c0p2_M800 BulkG_ZZ_lljj_c0p2_M900 BulkG_ZZ_lljj_c0p2_M1000 BulkG_ZZ_lljj_c0p2_M1100 BulkG_ZZ_lljj_c0p2_M1300 BulkG_ZZ_lljj_c0p2_M1400 BulkG_ZZ_lljj_c0p2_M1500 BulkG_ZZ_lljj_c0p2_M1700 BulkG_ZZ_lljj_c0p2_M1800 BulkG_ZZ_lljj_c0p2_M1900)
echo $MAINDIRMC/$sample
cmsLs $MAINDIRMC/$sample | grep root | awk '{print $5}' > ! tmp.txt
python makeCffInputFiles.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
end

### Remove tmp file ###

rm -f tmp.txt
