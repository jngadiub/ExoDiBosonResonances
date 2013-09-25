
Works in CMSSW_5_3_9 (not tested with higher releases)
Based on CMG V5_15_0
Most recent recommended branch for analysis code : edbr_post-ZZ-approval_pre-Git2



*** Installation instructions ***

# Follow these: https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsReleasesExperimental#Colin_539_V5_15_0
# BUT before compiling, update class def for CaloClusters
cvs up -r 1.15 DataFormats/CaloRecHit/src/classes_def.xml 
# (depending on the setup, either 1.14 or 1.15 actually works)
# Remove the H2TauTau package and other stuff since it is not useful and we want to keep the sandbox < 100 MB
rm -r CMGTools/RootTools 
rm -r CMGTools/ZJetsTutorial 
rm -r CMGTools/H2TauTau 
rm -r CMGTools/Utilities 
rm -r TauAnalysis/SVFitStandAlone 
rm -r TauAnalysis/CandidateTools 
#now, compile
scram b -j 4 

# Add the HEEP code and the modified HEEP isolation for boosted case.
cvs co -r V00-09-03 -d SHarper/HEEPAnalyzer UserCode/SHarper/HEEPAnalyzer
cvs co -r V00-02-03  -d TSWilliams/BstdZeeTools  UserCode/TSWilliams/BstdZee/BstdZeeTools
# Add the TOBTEC filter
cvs co -A -d KStenson/TrackingFilters UserCode/KStenson/TrackingFilters

# Install the EDBR analysis package.
# With Git you are forced to checkout the entire repository
# Follow the instructions for preparing your Git area
# -> https://twiki.cern.ch/twiki/bin/view/CMS/ExoDiBosonResonancesGitHowto
# (refer in particular to the sections "Still some configuration" and "Check-out the analysis code") 
git init
git clone git://github.com/cms-edbr/ExoDiBosonResonances

### remove analysis directories not needed for PAT production (they increase the crab sandobx)
### this is not needed if you don't want to submit PAT-production jobs to CRAB 
rm -fr AnalysisDataFormats/ EDBRCommon  EDBRElectron GeneratorStudies
# EDBRMuon needed for new tuneP


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


### This part is out-dated as it is still based on CVS. Now you should have already everything
### (CMSSW_5_3_9)
### cvs co -r edbr_post-ZZ-approval_pre-Git -d AnalysisDataFormats/ExoDiBosonResonances UserCode/ExoDiBosonResonances/AnalysisDataFormats
### cvs co -r edbr_post-ZZ-approval_pre-Git -d ExoDiBosonResonances/EDBRCommon/ UserCode/ExoDiBosonResonances/EDBRCommon/
### cvs co -r edbr_post-ZZ-approval_pre-Git -d ExoDiBosonResonances/EDBRElectron/ UserCode/ExoDiBosonResonances/EDBRElectron/
### cvs co -r edbr_post-ZZ-approval_pre-Git -d ExoDiBosonResonances/EDBRMuon/ UserCode/ExoDiBosonResonances/EDBRMuon/


### if you deleted the analysis directories for producing the PAT-tuples, bring them back
git checkout -- AnalysisDataFormats/ EDBRCommon  EDBRElectron GeneratorStudies
mkdir -p AnalysisDataFormats/; mv ExoDiBosonResonances/AnalysisDataFormats AnalysisDataFormats/ExoDiBosonResonances

rm -rf ExoDiBosonResonances/AnalysisDataFormats
cvs co -d Francesco/KinFitter/src UserCode/pandolf/KinematicFit
rm -f Francesco/KinFitter/src/T*
rm -f Francesco/KinFitter/src/LeptonNeutrinoKinFitter.*

### Notice that even though the instructions below are "commented",
### they still need to be followed!

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
