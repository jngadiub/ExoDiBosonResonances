#include "ExoDiBosonResonances/EDBRCommon/plugins/PUWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/RhoWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/HLTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightAnalyzer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/LDProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/KineVarsAdder.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/WeightAdder.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/BestCandidateSelector.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/EDBRTagger.h" 
//#include "CMGTools/Common/plugins/HistogramAnalyzer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/interface/JetCountingHistograms.h" 

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/Merger.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/CompoundTypesHZZLL.h"

#include "ExoDiBosonResonances/EDBRCommon/plugins/DummyGenProducer.h"
//#include "ExoDiBosonResonances/EDBRCommon/plugins/DummyConversionProducer.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiPFJet> > DiJetSelector;
DEFINE_FWK_MODULE(DiJetSelector);

//define selectors for Resonance-like objects
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiGenParticleDiGenParticleEDBR> > DiGenParticleDiGenParticleEDBRSelector;
DEFINE_FWK_MODULE(DiGenParticleDiGenParticleEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiElectronDiJetEDBR> > CmgDiElectronDiJetEDBRSelector;
DEFINE_FWK_MODULE(CmgDiElectronDiJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuonDiJetEDBR> > CmgDiMuonDiJetEDBRSelector;
DEFINE_FWK_MODULE(CmgDiMuonDiJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiElectronSingleJetEDBR> > CmgDiElectronSingleJetEDBRSelector;
DEFINE_FWK_MODULE(CmgDiElectronSingleJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuonSingleJetEDBR> > CmgDiMuonSingleJetEDBRSelector;
DEFINE_FWK_MODULE(CmgDiMuonSingleJetEDBRSelector);


//define mergers for Resonance-like objects
typedef Merger<std::vector<cmg::DiElectronDiJetEDBR>, std::vector<cmg::DiElectronDiJetEDBR> > CmgDiElectronDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiElectronDiJetEDBRMerger );
typedef Merger<std::vector<cmg::DiMuonDiJetEDBR>, std::vector<cmg::DiMuonDiJetEDBR> > CmgDiMuonDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiMuonDiJetEDBRMerger );
typedef Merger<std::vector<cmg::DiElectronSingleJetEDBR>, std::vector<cmg::DiElectronSingleJetEDBR> > CmgDiElectronSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiElectronSingleJetEDBRMerger );
typedef Merger<std::vector<cmg::DiMuonSingleJetEDBR>, std::vector<cmg::DiMuonSingleJetEDBR> > CmgDiMuonSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiMuonSingleJetEDBRMerger );

DEFINE_FWK_MODULE(PUWeightProducer);
DEFINE_FWK_MODULE(PTWeightProducer);
DEFINE_FWK_MODULE(PTWeightAnalyzer);

//////for the time being, we do not calculate a kinematic discriminant out of the hel angles...
//typedef LDProducer<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRLDProducer;
//typedef LDProducer<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRLDProducer;
//typedef LDProducer<cmg::DiGenParticleDiGenParticleEDBR>     DiGenParticleDiGenParticleEDBRLDProducer;
//DEFINE_FWK_MODULE(DiElectronDiJetEDBRLDProducer);
//DEFINE_FWK_MODULE(DiMuonDiJetEDBRLDProducer);
//DEFINE_FWK_MODULE(DiGenParticleDiGenParticleEDBRLDProducer);

//////embed extra-kine vars in EDBR candidate as userFloats
typedef KineVarsAdder<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRKineAdder;
typedef KineVarsAdder<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRKineAdder;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRKineAdder);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRKineAdder);
typedef KineVarsAdder<cmg::DiElectronSingleJetEDBR> DiElectronSingleJetEDBRKineAdder;
typedef KineVarsAdder<cmg::DiMuonSingleJetEDBR>     DiMuonSingleJetEDBRKineAdder;
DEFINE_FWK_MODULE(DiElectronSingleJetEDBRKineAdder);
DEFINE_FWK_MODULE(DiMuonSingleJetEDBRKineAdder);

//////define candidate selectors
typedef BestCandidateSelector<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRBestCandidateSelector;
typedef BestCandidateSelector<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRBestCandidateSelector;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRBestCandidateSelector);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRBestCandidateSelector);

/////define weight adders (weight because of PU, HLT...)
typedef WeightAdder<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRWeightAdder;
typedef WeightAdder<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRWeightAdder;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRWeightAdder);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::DiElectronDiJetEDBR> HLTWeightProducerElectron;
typedef HLTWeightProducer<cmg::DiMuonDiJetEDBR>     HLTWeightProducerMu;
DEFINE_FWK_MODULE(HLTWeightProducerElectron);
DEFINE_FWK_MODULE(HLTWeightProducerMu);
typedef WeightAdder<cmg::DiElectronSingleJetEDBR> DiElectronVJetEDBRWeightAdder;
typedef WeightAdder<cmg::DiMuonSingleJetEDBR>     DiMuonVJetEDBRWeightAdder;
DEFINE_FWK_MODULE(DiElectronVJetEDBRWeightAdder);
DEFINE_FWK_MODULE(DiMuonVJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::DiElectronSingleJetEDBR> HLTWeightProducerEleVJet;
typedef HLTWeightProducer<cmg::DiMuonSingleJetEDBR>     HLTWeightProducerMuVJet;
DEFINE_FWK_MODULE(HLTWeightProducerEleVJet);
DEFINE_FWK_MODULE(HLTWeightProducerMuVJet);

//define VBF taggers
typedef EDBRTagger<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRTagger;
typedef EDBRTagger<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRTagger;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRTagger);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRTagger);
typedef EDBRTagger<cmg::DiElectronSingleJetEDBR> DiElectronVJetEDBRTagger;
typedef EDBRTagger<cmg::DiMuonSingleJetEDBR>     DiMuonVJetEDBRTagger;
DEFINE_FWK_MODULE(DiElectronVJetEDBRTagger);
DEFINE_FWK_MODULE(DiMuonVJetEDBRTagger);


DEFINE_FWK_MODULE(DummyGenProducer);
//DEFINE_FWK_MODULE(DummyConversionProducer);
