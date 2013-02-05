#! /bin/bash

SampleName=(TTBAR DYJetsPt50To70 DYJetsPt70To100 DYJetsPt100 WW WZ ZZ DoubleMu_Run2012A_13Jul2012 DoubleMu_Run2012A_recover DoubleMu_Run2012B_13Jul2012 DoubleMu_Run2012C_24Aug2012 DoubleMu_Run2012C_PRv2 DoubleMu_Run2012D_PRv1 )

for sample in  "${SampleName[@]}"
do
echo "Submitting $sample"
bsub -q 8nh -J "treeEDBR_${sample}" run_AnalyzerEDBR.sh $sample
echo
done

#${CMSSW_BASE}/ExoDiBosonResonances/EDBRCommon/test/
