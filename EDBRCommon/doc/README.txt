
Works in CMSSW_5_3_3_patch3 (not tested with higher releases)
Based on CMG V5_10_0
Most recent recommended tag for analysis code : edbr_zz2l2j_20121127A (but the HEAD might be better)


*** Installation instructions ***

Follow these: https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsReleasesExperimental#Mike_Cris_533_V5_10_0
scram b -j 4

cvs co -d AnalysisDataFormats/ExoDiBosonResonances UserCode/ExoDiBosonResonances/AnalysisDataFormats
cvs co -d ExoDiBosonResonances/EDBRCommon UserCode/ExoDiBosonResonances/EDBRCommon
cvs co -d ExoDiBosonResonances/EDBRElectron UserCode/ExoDiBosonResonances/EDBRElectron
rm -rf ExoDiBosonResonances/AnalysisDataFormats
cvs co -d Francesco/KinFitter/src UserCode/pandolf/KinematicFit
rm -f Francesco/KinFitter/src/T*
rm -f Francesco/KinFitter/src/LeptonNeutrinoKinFitter.*

#remove spurious semi-colons in Francesco/KinFitter/src/GlobalFitter.cc

#In AnalysisDataFormats/CMGTools/interface/DiObject.h add
#template <typename T, typename U> class DiObjectKinFitFactory; near the top and
#friend class cmg::DiObjectKinFitFactory<T,U>; near the bottom.

cd Francesco; scram b; cd -
cd AnalysisDataFormats; scram b; cd -

cd ExoDiBosonResonances
scram b -j 4

*** Running instructions ***

See ExoDiBosonResonances/EDBRCommon/doc/HOWTO_run_cmgTuple_step.txt