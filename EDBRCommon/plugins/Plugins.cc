#include "ExoDiBosonResonances/EDBRCommon/plugins/PUWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/RhoWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/HLTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightAnalyzer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/LDProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/QGLDSelector.h" 
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

//define mergers for Resonance-like objects
typedef Merger<std::vector<cmg::DiElectronDiJetEDBR>, std::vector<cmg::DiElectronDiJetEDBR> > CmgDiElectronDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiElectronDiJetEDBRMerger );
typedef Merger<std::vector<cmg::DiMuonDiJetEDBR>, std::vector<cmg::DiMuonDiJetEDBR> > CmgDiMuonDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiMuonDiJetEDBRMerger );

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

//typedef QGLDSelector<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRQGLDSelector;
//typedef QGLDSelector<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRQGLDSelector;
//DEFINE_FWK_MODULE(DiElectronDiJetEDBRQGLDSelector);
//DEFINE_FWK_MODULE(DiMuonDiJetEDBRQGLDSelector);

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


//define VBF taggers
typedef EDBRTagger<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRTagger;
typedef EDBRTagger<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRTagger;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRTagger);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRTagger);


DEFINE_FWK_MODULE(DummyGenProducer);
//DEFINE_FWK_MODULE(DummyConversionProducer);
