#!/bin/sh

### Output directory for the lists ###

OUTPUTDIR=$USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

MAINDIRDATA=/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Run2012/CA8
MAINDIRMC=/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas/production06032013_edbr_vv_20130313/Summer12/CA8

### Make lists for DATA ###

for sample in   SingleMu_Run2012A_13Jul2012_xww  SingleMu_Run2012A_recover_xww SingleMu_Run2012B_13Jul2012_xww SingleMu_Run2012C_24Aug2012_xww SingleMu_Run2012C_PromptReco_xww SingleMu_Run2012D_PromptReco_xww   SingleElectron_Run2012A_13Jul2012_xww  SingleElectron_Run2012A_recover_xww SingleElectron_Run2012B_13Jul2012_xww SingleElectron_Run2012C_24Aug2012_xww SingleElectron_Run2012C_PromptReco_xww SingleElectron_Run2012D_PromptReco_xww
do
    echo $MAINDIRDATA/$sample
	cmsLs $MAINDIRDATA/$sample | grep .root | awk '{print $5}'  > tmp.txt
    python makeCffInputFiles_ls.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Make lists for MC ###

for sample in TTBAR_xww  WW_xww WZ_xww ZZ_xww WJetsPt50To70_xww WJetsPt70To100_xww WJetsPt100_xww DYJetsPt50To70_xww DYJetsPt70To100_xww  DYJetsPt100_xww  SingleTopBarSchannel_xww SingleTopBarTWchannel_xww SingleTopBarTchannel_xww SingleTopSchannel_xww SingleTopTWchannel_xww SingleTopTchannel_xww BulkG_WW_lvjj_c1p0_M1000_xww BulkG_WW_lvjj_c1p0_M600_xww BulkG_WW_lvjj_c1p0_M1500_xww  RSG_WW_lvjj_c0p2_M1000_xww  RSG_WW_lvjj_c0p2_M600_xww  RSG_WW_lvjj_c0p2_M1500_xww
do
echo $MAINDIRMC/$sample
cmsLs $MAINDIRMC/$sample | grep .root | awk '{print $5}'  > tmp.txt
python makeCffInputFiles_ls.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Remove tmp file ###

rm -f tmp.txt
