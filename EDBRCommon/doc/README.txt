
Works in CMSSW_5_3_3_patch3 (not tested with higher releases)
Based on CMG V5_10_0
Most recent recommended tag for analysis code : edbr_zz_20121222 (but the HEAD might be better)


*** Installation instructions ***

Follow these: https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsReleasesExperimental#Mike_Cris_533_V5_10_0
scram b -j 4

cvs up -r 1.7 CMGTools/Common/python/PAT/jetSubstructure_cff.py
cvs up -r 1.10 CMGTools/Common/python/Tools/getGlobalTag.py
cvs up -r 1.53 PhysicsTools/PatAlgos/python/tools/tauTools.py
cvs up -r 1.12 PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cff.py
cvs up -r 1.15 PhysicsTools/PatAlgos/python/recoLayer0/tauDiscriminators_cff.py
chmod +x CMGTools/Production/scripts/*.py
chmod +x CMGTools/Common/scripts/*.py
cd EGamma/EGammaAnalysisTools/data
cat download.url | xargs wget
cd -

# Add the HEEP code and the modified HEEP isolation for boosted case.
cvs co -r V00-09-03 -d SHarper/HEEPAnalyzer UserCode/SHarper/HEEPAnalyzer
cvs co -r V00-02-01  -d TSWilliams/BstdZeeTools  UserCode/TSWilliams/BstdZee/BstdZeeTools

cvs co -r edbr_zz_20130115 -d ExoDiBosonResonances/PATtupleProduction/ UserCode/ExoDiBosonResonances/PATtupleProduction/

### end part needed just for PAT-production, compile with: scram b -j7
###########################################
### if you want to run the analysis, you need also the following

cvs co -r edbr_zz_20130115 -d AnalysisDataFormats/ExoDiBosonResonances UserCode/ExoDiBosonResonances/AnalysisDataFormats
cvs co -r edbr_zz_20130115 -d ExoDiBosonResonances/EDBRCommon/ UserCode/ExoDiBosonResonances/EDBRCommon/
cvs co -r edbr_zz_20130115 -d ExoDiBosonResonances/EDBRElectron/ UserCode/ExoDiBosonResonances/EDBRElectron/
cvs co -r edbr_zz_20130115 -d ExoDiBosonResonances/EDBRMuon/ UserCode/ExoDiBosonResonances/EDBRMuon/

rm -rf ExoDiBosonResonances/AnalysisDataFormats
cvs co -d Francesco/KinFitter/src UserCode/pandolf/KinematicFit
rm -f Francesco/KinFitter/src/T*
rm -f Francesco/KinFitter/src/LeptonNeutrinoKinFitter.*

#remove spurious semi-colons in Francesco/KinFitter/src/GlobalFitter.cc

#In AnalysisDataFormats/CMGTools/interface/DiObject.h add
#template <typename T, typename U> class DiObjectKinFitFactory; near the top and
#friend class cmg::DiObjectKinFitFactory<T,U>; near the bottom.

#In AnalysisDataFormats/CMGTools/interface/PFJet.h add this line 
#friend class VJetFactory;
#just after the line   :  friend class PFJetFactory;

scram b -j 7

*** Running instructions ***

Step 1: make PAT-tuples
Step 2: make cmgTuple from the PAT-tuples, 
        see ExoDiBosonResonances/EDBRCommon/doc/HOWTO_run_cmgTuple_step.txt
Step 3: make TTree from cmgTuple
        see ExoDiBosonResonances/EDBRCommon/doc/HOWTO_run_TreeMaking_step.txt