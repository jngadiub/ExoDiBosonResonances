#!/bin/sh

### Output directory for the lists ###

OUTPUTDIR=$USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

MAINDIRDATA=/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/shuai/production0215/Run2012/preselCA8
MAINDIRMC=/store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/shuai/production0215/Summer12/preselCA8

### Make lists for DATA ###

for sample in   SingleMu_Run2012A_13Jul2012_xww  SingleMu_Run2012A_recover_xww SingleMu_Run2012B_13Jul2012_xww SingleMu_Run2012C_24Aug2012_xww SingleMu_Run2012C_PromptReco_xww SingleMu_Run2012D_PromptReco_xww     
do
    echo $MAINDIRDATA/$sample
	cmsLs $MAINDIRDATA/$sample | grep .root | awk '{print $5}'  > tmp.txt
    python makeCffInputFiles_ls.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Make lists for MC ###

for sample in TTBAR_xww  WW_xww WZ_xww ZZ_xww WJetsPt50To70_xww WJetsPt70To100_xww WJetsPt100_xww DYJetsPt50To70_xww DYJetsPt70To100_xww  DYJetsPt100_xww  BulkG_WW_lvjj_c1p0_M1000_xww  BulkG_WW_lvjj_c1p0_M600_xww   BulkG_WW_lvjj_c1p0_M1500_xww   RSG_WW_lvjj_c0p2_M1000_xww  RSG_WW_lvjj_c0p2_M600_xww  RSG_WW_lvjj_c0p2_M1500_xww
do
echo $MAINDIRMC/$sample
cmsLs $MAINDIRMC/$sample | grep .root | awk '{print $5}'  > tmp.txt
python makeCffInputFiles_ls.py $OUTPUTDIR/cmgTuple_${sample}_cff.py
done

### Remove tmp file ###

rm -f tmp.txt