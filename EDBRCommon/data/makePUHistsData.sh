# Shamefully stolen from Cory Fantasia
# The JSON files for 2012 are in:
# /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV
BaseDir=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV
# Here we should put the JSON file of our analyzed data.
# For the time being, since we have no analyzed data, I'm putting the
# admixture of all JSON files for different reconstructions

# Golden JSON
#AnalysisJSON=${BaseDir}/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt
AnalysisJSON=${BaseDir}/Reprocessing/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt
echo "The final analysis JSON is: $AnalysisJSON"

# Pileup JSON
LumiJSON=${BaseDir}/PileUp/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr.txt

#JSON file used to filter events (from DCSOnly or Prompt subdir)

pileupCalc.py \
    -i ${AnalysisJSON} \
    --inputLumiJSON ${LumiJSON} \
    --calcMode true \
    --minBiasXsec 69400 \
    --maxPileupBin 60 \
    --numPileupBins 60  \
    PUDist_Run2012Full_Truth_69p4mb.root

pileupCalc.py \
    -i ${AnalysisJSON} \
    --inputLumiJSON ${LumiJSON} \
    --calcMode true \
    --minBiasXsec 73564 \
    --maxPileupBin 60 \
    --numPileupBins 60  \
    PUDist_Run2012Full_Truth_73p6mb.root

pileupCalc.py \
    -i ${AnalysisJSON} \
    --inputLumiJSON ${LumiJSON} \
    --calcMode true \
    --minBiasXsec 65236 \
    --maxPileupBin 60 \
    --numPileupBins 60  \
    PUDist_Run2012Full_Truth_65p2mb.root

echo "The pileup histograms for data are in PUDist_Run2012Full_Truth_69p4mb.root, PUDist_Run2012Full_Truth_73p6mb.root and PUDist_Run2012Full_Truth_65p2mb.root"
