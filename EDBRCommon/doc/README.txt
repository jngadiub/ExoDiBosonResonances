
Works in CMSSW_5_3_9 (not tested with higher releases)
Based on CMG V5_15_0
Most recent recommended tag for analysis code : edbr_vv_20130527b



*** Installation instructions ***

# Follow these: https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsReleasesExperimental#Colin_539_V5_15_0
# BUT before compiling, update class def for CaloClusters
cvs up -r 1.15 DataFormats/CaloRecHit/src/classes_def.xml
# Remove the H2TauTau pachage since it is not useful and we want to keep the sandbox < 100 MB
rm -rf CMGTools/H2TauTau
#now, compile
scram b -j 4 

# Add the HEEP code and the modified HEEP isolation for boosted case.
cvs co -r V00-09-03 -d SHarper/HEEPAnalyzer UserCode/SHarper/HEEPAnalyzer
cvs co -r V00-02-03  -d TSWilliams/BstdZeeTools  UserCode/TSWilliams/BstdZee/BstdZeeTools
# Add the new TuneP muons
cvs co -A -d ExoDiBosonResonances/EDBRMuon/ UserCode/ExoDiBosonResonances/EDBRMuon/
# The PATtuplization code itself
cvs co -A -d ExoDiBosonResonances/PATtupleProduction/ UserCode/ExoDiBosonResonances/PATtupleProduction/
# Add the TOBTEC filter
cvs co -A -d KStenson/TrackingFilters UserCode/KStenson/TrackingFilters
# and add the missing BuildFile, as well as move the plugins directory to the src one
mv ExoDiBosonResonances/PATtupleProduction/data/Buildfile_KStenson KStenson/TrackingFilters/BuildFile.xml
rm -rf KStenson/TrackingFilters/src
mv KStenson/TrackingFilters/plugins KStenson/TrackingFilters/src 

######## updated recipe for new MET corrections 
# On second hand, better not to do it, makes the sandbox too big
#addpkg PhysicsTools/PatAlgos V08-09-57 
#addpkg DataFormats/StdDictionaries V00-02-15 
#addpkg DataFormats/METReco V03-03-11-01
#addpkg JetMETCorrections/Type1MET V04-06-09-02
#cvs co -r V00-03-23 CommonTools/RecoAlgos
#rm -fr FWCore/GuiBrowser

#compile again
scram b -j 4 

### end part needed just for PAT-production, compile with: scram b -j7
###########################################
### if you want to run the analysis, you need also the following

cvs co -r edbr_vv_20130524 -d AnalysisDataFormats/ExoDiBosonResonances UserCode/ExoDiBosonResonances/AnalysisDataFormats
cvs co -r edbr_vv_20130524 -d ExoDiBosonResonances/EDBRCommon/ UserCode/ExoDiBosonResonances/EDBRCommon/
cvs co -r edbr_vv_20130524 -d ExoDiBosonResonances/EDBRElectron/ UserCode/ExoDiBosonResonances/EDBRElectron/
cvs co -r edbr_vv_20130524 -d ExoDiBosonResonances/EDBRMuon/ UserCode/ExoDiBosonResonances/EDBRMuon/

rm -rf ExoDiBosonResonances/AnalysisDataFormats
cvs co -d Francesco/KinFitter/src UserCode/pandolf/KinematicFit
rm -f Francesco/KinFitter/src/T*
rm -f Francesco/KinFitter/src/LeptonNeutrinoKinFitter.*

#remove spurious semi-colons in Francesco/KinFitter/src/GlobalFitter.cc
# at the end of these two functions
# void MissingEnergy1::SetSumEt(float SumEt){
# void MissingEnergy1::SetNeutrino(TLorentzVector Neutrino){

#In AnalysisDataFormats/CMGTools/interface/DiObject.h add
#template <typename T, typename U> class DiObjectKinFitFactory; near the top and
#friend class cmg::DiObjectKinFitFactory<T,U>; near the bottom.

#In AnalysisDataFormats/CMGTools/interface/PFJet.h add this line 
#friend class VJetFactory;
#just after the line   :  friend class PFJetFactory;

# waiting for a fixed CVS version of the CMGTools PFJetFactory, replace
# the file with our temporary version
cp ExoDiBosonResonances/EDBRCommon/test/Temporary.PFJetFactory.cc CMGTools/Common/src/PFJetFactory.cc

scram b -j 7





*** Running instructions ***

Step 1: make PAT-tuples
Step 2: make cmgTuple from the PAT-tuples, 
        see ExoDiBosonResonances/EDBRCommon/doc/HOWTO_run_cmgTuple_step.txt
Step 3: make TTree from cmgTuple
        see ExoDiBosonResonances/EDBRCommon/doc/HOWTO_run_TreeMaking_step.txt