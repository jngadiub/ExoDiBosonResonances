#!/bin/csh

### Output directory for the lists ###

set OUTPUTDIR = $USER
mkdir -p $OUTPUTDIR

### Location of CMGtuples ###

set MAINDIRDATA = /store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas_TEST__05_02_2013/Run2012/presel
set MAINDIRMC = /store/group/phys_exotica/leptonsPlusJets/ExoDiBosonResonances/CMGtuple/santanas_TEST__05_02_2013/Summer12/presel

### Create tmp file ###

touch tmp.txt

### Make lists for DATA ###

foreach sample (DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1)
echo $MAINDIRDATA/$sample
cmsLs $MAINDIRDATA/$sample | grep root | awk '{print $5}' > ! tmp.txt
python makeCffInputFiles.py $OUTPUTDIR/cmgTupleTest_${sample}_cff.py
end

### Make lists for MC ###

foreach sample (DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 TTBAR WW WZ ZZ)
echo $MAINDIRMC/$sample
cmsLs $MAINDIRMC/$sample | grep root | awk '{print $5}' > ! tmp.txt
python makeCffInputFiles.py $OUTPUTDIR/cmgTupleTest_${sample}_cff.py
end

### Remove tmp file ###

rm -f tmp.txt
