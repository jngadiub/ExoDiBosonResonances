//#include "ExoDiBosonResonances/EDBRCommon/plugins/PUWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/RhoWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/HLTWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightAnalyzer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/LDProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/QGLDSelector.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/WeightAdder.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/BestCandidateSelector.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/HiggsTagger.h" 
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

//typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiGenParticleDiGenParticleHiggs> > DiGenParticleDiGenParticleHiggsSelector;
//DEFINE_FWK_MODULE(DiGenParticleDiGenParticleHiggsSelector);
//typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiElectronDiJetHiggs> > CmgDiElectronDiJetHiggsSelector;
//DEFINE_FWK_MODULE(CmgDiElectronDiJetHiggsSelector);
//typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::DiMuonDiJetHiggs> > CmgDiMuonDiJetHiggsSelector;
//DEFINE_FWK_MODULE(CmgDiMuonDiJetHiggsSelector);

//typedef Merger<std::vector<cmg::DiElectronDiJetHiggs>, std::vector<cmg::DiElectronDiJetHiggs> > CmgDiElectronDiJetHiggsMerger;
//DEFINE_FWK_MODULE( CmgDiElectronDiJetHiggsMerger );
//typedef Merger<std::vector<cmg::DiMuonDiJetHiggs>, std::vector<cmg::DiMuonDiJetHiggs> > CmgDiMuonDiJetHiggsMerger;
//DEFINE_FWK_MODULE( CmgDiMuonDiJetHiggsMerger );

//DEFINE_FWK_MODULE(PUWeightProducer);
//DEFINE_FWK_MODULE(PTWeightProducer);
//DEFINE_FWK_MODULE(PTWeightAnalyzer);


//typedef LDProducer<cmg::DiElectronDiJetHiggs> DiElectronDiJetHiggsLDProducer;
//typedef LDProducer<cmg::DiMuonDiJetHiggs>     DiMuonDiJetHiggsLDProducer;
//typedef LDProducer<cmg::DiGenParticleDiGenParticleHiggs>     DiGenParticleDiGenParticleHiggsLDProducer;
//DEFINE_FWK_MODULE(DiElectronDiJetHiggsLDProducer);
//DEFINE_FWK_MODULE(DiMuonDiJetHiggsLDProducer);
//DEFINE_FWK_MODULE(DiGenParticleDiGenParticleHiggsLDProducer);

//typedef QGLDSelector<cmg::DiElectronDiJetHiggs> DiElectronDiJetHiggsQGLDSelector;
//typedef QGLDSelector<cmg::DiMuonDiJetHiggs>     DiMuonDiJetHiggsQGLDSelector;
//DEFINE_FWK_MODULE(DiElectronDiJetHiggsQGLDSelector);
//DEFINE_FWK_MODULE(DiMuonDiJetHiggsQGLDSelector);


//typedef BestCandidateSelector<cmg::DiElectronDiJetHiggs> DiElectronDiJetHiggsBestCandidateSelector;
//typedef BestCandidateSelector<cmg::DiMuonDiJetHiggs>     DiMuonDiJetHiggsBestCandidateSelector;
//DEFINE_FWK_MODULE(DiElectronDiJetHiggsBestCandidateSelector);
//DEFINE_FWK_MODULE(DiMuonDiJetHiggsBestCandidateSelector);

//typedef WeightAdder<cmg::DiElectronDiJetHiggs> DiElectronDiJetHiggsWeightAdder;
//typedef WeightAdder<cmg::DiMuonDiJetHiggs>     DiMuonDiJetHiggsWeightAdder;
//DEFINE_FWK_MODULE(DiElectronDiJetHiggsWeightAdder);
//DEFINE_FWK_MODULE(DiMuonDiJetHiggsWeightAdder);

//typedef HLTWeightProducer<cmg::DiElectronDiJetHiggs> HLTWeightProducerElectron;
//typedef HLTWeightProducer<cmg::DiMuonDiJetHiggs>     HLTWeightProducerMu;
//DEFINE_FWK_MODULE(HLTWeightProducerElectron);
//DEFINE_FWK_MODULE(HLTWeightProducerMu);


//typedef HiggsTagger<cmg::DiElectronDiJetHiggs> DiElectronDiJetHiggsTagger;
//typedef HiggsTagger<cmg::DiMuonDiJetHiggs>     DiMuonDiJetHiggsTagger;
//DEFINE_FWK_MODULE(DiElectronDiJetHiggsTagger);
//DEFINE_FWK_MODULE(DiMuonDiJetHiggsTagger);


DEFINE_FWK_MODULE(DummyGenProducer);
//DEFINE_FWK_MODULE(DummyConversionProducer);
