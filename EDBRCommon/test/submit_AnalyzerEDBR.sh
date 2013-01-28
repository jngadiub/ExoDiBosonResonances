#! /bin/bash

SampleName=(TTBAR DYJetsPt70To100 DYJetsPt50To70 DYJetsPt100) #
#SampleName=(   )

#SampleName=(data1Ele data1Mu data2Mu   data2Ele ) 



for sample in  "${SampleName[@]}"
do
echo "Submitting $sample"
bsub -q 8nh -J "treeEDBR_${sample}" run_AnalyzerEDBR.sh $sample
echo
done

#${CMSSW_BASE}/ExoDiBosonResonances/EDBRCommon/test/