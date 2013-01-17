# Shamefully stolen from Cory Fantasia
# The JSON files for 2012 are in:
# /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV
BaseDir=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV
# Here we should put the JSON file of our analyzed data.
# For the time being, since we have no analyzed data, I'm putting the
# admixture of all JSON files for different reconstructions

AB13JulJSON=${BaseDir}/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt
ArecoverJSON=${BaseDir}/Reprocessing/Cert_190782-190949_8TeV_06Aug2012ReReco_Collisions12_JSON.txt
C24AugJSON=${BaseDir}/Reprocessing/Cert_198022-198523_8TeV_24Aug2012ReReco_Collisions12_JSON.txt
CDPromptJSON=${BaseDir}/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt

# Some printouts
echo "Mixing the following JSON files:"
echo "A rereco: $AB13JulJSON"
echo "A recover rereco: $ArecoverJSON"
echo "B rereco: $AB13JulJSON"
echo "C rereco: $C24AugJSON"
echo "C prompt: $CDPromptJSON"
echo "D prompt: $CDPromptJSON"

mergeJSON.py ${AB13JulJSON} ${ArecoverJSON} --output=fullABJSON.txt
mergeJSON.py fullABJSON.txt ${C24AugJSON} --output=fullReRecoJSON.txt
filterJSON.py --min 198523 --max 999999 ${CDPromptJSON} --output promptJSON.txt
mergeJSON.py fullReRecoJSON.txt promptJSON.txt --output=goldenAnalysisJSON.txt

# Golden JSON
#AnalysisJSON=${BaseDir}/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt
AnalysisJSON=goldenAnalysisJSON.txt
echo "The final analysis JSON is: $AnalysisJSON"

# Pileup JSON
LumiJSON=${BaseDir}/PileUp/pileup_JSON_DCSONLY_190389-208686_corr.txt

#JSON file used to filter events (from DCSOnly or Prompt subdir)

pileupCalc.py \
    -i ${AnalysisJSON} \
    --inputLumiJSON ${LumiJSON} \
    --calcMode true \
    --minBiasXsec 69300 \
    --maxPileupBin 60 \
    --numPileupBins 60  \
    PUDist_Run2012Full_Truth_69p3mb.root

pileupCalc.py \
    -i ${AnalysisJSON} \
    --inputLumiJSON ${LumiJSON} \
    --calcMode true \
    --minBiasXsec 73500 \
    --maxPileupBin 60 \
    --numPileupBins 60  \
    PUDist_Run2012Full_Truth_73p5mb.root

echo "The pileup histograms for data are in PUDist_Run2012Full_Truth_69p3mb.root and PUDist_Run2012Full_Truth_73p5mb.root"
