#include "ExoDiBosonResonances/EDBRCommon/plugins/PUWeightProducer.h" 
//#include "ExoDiBosonResonances/EDBRCommon/plugins/RhoWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/HLTWeightProducer.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/HLTWeightProducer2012.h" 
#include "ExoDiBosonResonances/EDBRCommon/plugins/BTagWeightProducer.h" 
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

#define EDBRNEUTRINO
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WelenuDiJetEDBR> > CmgWelenuDiJetEDBRSelector;
DEFINE_FWK_MODULE(CmgWelenuDiJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WmunuDiJetEDBR> > CmgWmunuDiJetEDBRSelector;
DEFINE_FWK_MODULE(CmgWmunuDiJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WelenuSingleJetEDBR> > CmgWelenuSingleJetEDBRSelector;
DEFINE_FWK_MODULE(CmgWelenuSingleJetEDBRSelector);
typedef ObjectSelector<cmg::GenericPhysicsObjectSelectorDefinition<cmg::WmunuSingleJetEDBR> > CmgWmunuSingleJetEDBRSelector;
DEFINE_FWK_MODULE(CmgWmunuSingleJetEDBRSelector);
#undef EDBRNEUTRINO

//define mergers for Resonance-like objects
typedef Merger<std::vector<cmg::DiElectronDiJetEDBR>, std::vector<cmg::DiElectronDiJetEDBR> > CmgDiElectronDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiElectronDiJetEDBRMerger );
typedef Merger<std::vector<cmg::DiMuonDiJetEDBR>, std::vector<cmg::DiMuonDiJetEDBR> > CmgDiMuonDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiMuonDiJetEDBRMerger );
typedef Merger<std::vector<cmg::DiElectronSingleJetEDBR>, std::vector<cmg::DiElectronSingleJetEDBR> > CmgDiElectronSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiElectronSingleJetEDBRMerger );
typedef Merger<std::vector<cmg::DiMuonSingleJetEDBR>, std::vector<cmg::DiMuonSingleJetEDBR> > CmgDiMuonSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgDiMuonSingleJetEDBRMerger );

#define EDBRNEUTRINO
typedef Merger<std::vector<cmg::WelenuDiJetEDBR>, std::vector<cmg::WelenuDiJetEDBR> > CmgWelenuDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgWelenuDiJetEDBRMerger );
typedef Merger<std::vector<cmg::WmunuDiJetEDBR>, std::vector<cmg::WmunuDiJetEDBR> > CmgWmunuDiJetEDBRMerger;
DEFINE_FWK_MODULE( CmgWmunuDiJetEDBRMerger );
typedef Merger<std::vector<cmg::WelenuSingleJetEDBR>, std::vector<cmg::WelenuSingleJetEDBR> > CmgWelenuSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgWelenuSingleJetEDBRMerger );
typedef Merger<std::vector<cmg::WmunuSingleJetEDBR>, std::vector<cmg::WmunuSingleJetEDBR> > CmgWmunuSingleJetEDBRMerger;
DEFINE_FWK_MODULE( CmgWmunuSingleJetEDBRMerger );
#undef EDBRNEUTRINO

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


#define EDBRNEUTRINO
typedef KineVarsAdder<cmg::WelenuDiJetEDBR> WelenuDiJetEDBRKineAdder;
typedef KineVarsAdder<cmg::WmunuDiJetEDBR>     WmunuDiJetEDBRKineAdder;
DEFINE_FWK_MODULE(WelenuDiJetEDBRKineAdder);
DEFINE_FWK_MODULE(WmunuDiJetEDBRKineAdder);
typedef KineVarsAdder<cmg::WelenuSingleJetEDBR> WelenuSingleJetEDBRKineAdder;
typedef KineVarsAdder<cmg::WmunuSingleJetEDBR>     WmunuSingleJetEDBRKineAdder;
DEFINE_FWK_MODULE(WelenuSingleJetEDBRKineAdder);
DEFINE_FWK_MODULE(WmunuSingleJetEDBRKineAdder);
#undef EDBRNEUTRINO

//////define candidate selectors
typedef BestCandidateSelector<cmg::DiElectronSingleJetEDBR, cmg::DiElectronDiJetEDBR> DiElectronNJetEDBRBestCandidateSelector;
typedef BestCandidateSelector<cmg::DiMuonSingleJetEDBR, cmg::DiMuonDiJetEDBR>     DiMuonNJetEDBRBestCandidateSelector;
DEFINE_FWK_MODULE(DiElectronNJetEDBRBestCandidateSelector);
DEFINE_FWK_MODULE(DiMuonNJetEDBRBestCandidateSelector);

#define EDBRNEUTRINO
typedef BestCandidateSelector<cmg::WelenuSingleJetEDBR, cmg::WelenuDiJetEDBR> WelenuNJetEDBRBestCandidateSelector;
typedef BestCandidateSelector<cmg::WmunuSingleJetEDBR, cmg::WmunuDiJetEDBR>     WmunuNJetEDBRBestCandidateSelector;
DEFINE_FWK_MODULE(WelenuNJetEDBRBestCandidateSelector);
DEFINE_FWK_MODULE(WmunuNJetEDBRBestCandidateSelector);
#undef EDBRNEUTRINO

/////define weight adders (weight because of PU, HLT...)
typedef WeightAdder<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRWeightAdder;
typedef WeightAdder<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRWeightAdder;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRWeightAdder);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::DiElectronDiJetEDBR> HLTWeightProducerElectron;
typedef HLTWeightProducer<cmg::DiMuonDiJetEDBR>     HLTWeightProducerMu;
DEFINE_FWK_MODULE(HLTWeightProducerElectron);
DEFINE_FWK_MODULE(HLTWeightProducerMu);
typedef HLTWeightProducer2012<cmg::DiElectronDiJetEDBR> HLTWeightProducer2012Electron;
typedef HLTWeightProducer2012<cmg::DiMuonDiJetEDBR>     HLTWeightProducer2012Mu;
DEFINE_FWK_MODULE(HLTWeightProducer2012Electron);
DEFINE_FWK_MODULE(HLTWeightProducer2012Mu);
typedef WeightAdder<cmg::DiElectronSingleJetEDBR> DiElectronVJetEDBRWeightAdder;
typedef WeightAdder<cmg::DiMuonSingleJetEDBR>     DiMuonVJetEDBRWeightAdder;
DEFINE_FWK_MODULE(DiElectronVJetEDBRWeightAdder);
DEFINE_FWK_MODULE(DiMuonVJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::DiElectronSingleJetEDBR> HLTWeightProducerEleVJet;
typedef HLTWeightProducer<cmg::DiMuonSingleJetEDBR>     HLTWeightProducerMuVJet;
DEFINE_FWK_MODULE(HLTWeightProducerEleVJet);
DEFINE_FWK_MODULE(HLTWeightProducerMuVJet);
typedef HLTWeightProducer2012<cmg::DiElectronSingleJetEDBR> HLTWeightProducer2012EleVJet;
typedef HLTWeightProducer2012<cmg::DiMuonSingleJetEDBR>     HLTWeightProducer2012MuVJet;
DEFINE_FWK_MODULE(HLTWeightProducer2012EleVJet);
DEFINE_FWK_MODULE(HLTWeightProducer2012MuVJet);

#define EDBRNEUTRINO
typedef WeightAdder<cmg::WelenuDiJetEDBR>     WelenuDiJetEDBRWeightAdder;
DEFINE_FWK_MODULE(WelenuDiJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::WelenuDiJetEDBR>     HLTWeightProducerWelenuDiJet;
DEFINE_FWK_MODULE(HLTWeightProducerWelenuDiJet);
typedef HLTWeightProducer2012<cmg::WelenuDiJetEDBR>     HLTWeightProducer2012WelenuDiJet;
DEFINE_FWK_MODULE(HLTWeightProducer2012WelenuDiJet);
typedef WeightAdder<cmg::WmunuDiJetEDBR>     WmunuDiJetEDBRWeightAdder;
DEFINE_FWK_MODULE(WmunuDiJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::WmunuDiJetEDBR>     HLTWeightProducerWmunuDiJet;
DEFINE_FWK_MODULE(HLTWeightProducerWmunuDiJet);
typedef HLTWeightProducer2012<cmg::WmunuDiJetEDBR>     HLTWeightProducer2012WmunuDiJet;
DEFINE_FWK_MODULE(HLTWeightProducer2012WmunuDiJet);
typedef WeightAdder<cmg::WelenuSingleJetEDBR>     WelenuSingleJetEDBRWeightAdder;
DEFINE_FWK_MODULE(WelenuSingleJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::WelenuSingleJetEDBR>     HLTWeightProducerWelenuSingleJet;
DEFINE_FWK_MODULE(HLTWeightProducerWelenuSingleJet);
typedef HLTWeightProducer2012<cmg::WelenuSingleJetEDBR>     HLTWeightProducer2012WelenuSingleJet;
DEFINE_FWK_MODULE(HLTWeightProducer2012WelenuSingleJet);
typedef WeightAdder<cmg::WmunuSingleJetEDBR>     WmunuSingleJetEDBRWeightAdder;
DEFINE_FWK_MODULE(WmunuSingleJetEDBRWeightAdder);
typedef HLTWeightProducer<cmg::WmunuSingleJetEDBR>     HLTWeightProducerWmunuSingleJet;
DEFINE_FWK_MODULE(HLTWeightProducerWmunuSingleJet);
typedef HLTWeightProducer2012<cmg::WmunuSingleJetEDBR>     HLTWeightProducer2012WmunuSingleJet;
DEFINE_FWK_MODULE(HLTWeightProducer2012WmunuSingleJet);


typedef BTagWeightProducer<cmg::WelenuDiJetEDBR>     BTagWeightProducerWelenuDiJet;
DEFINE_FWK_MODULE(BTagWeightProducerWelenuDiJet);  
typedef BTagWeightProducer<cmg::WmunuDiJetEDBR>     BTagWeightProducerWmunuDiJet;
DEFINE_FWK_MODULE(BTagWeightProducerWmunuDiJet);
typedef BTagWeightProducer<cmg::WelenuSingleJetEDBR>     BTagWeightProducerWelenuSingleJet;
DEFINE_FWK_MODULE(BTagWeightProducerWelenuSingleJet);
typedef BTagWeightProducer<cmg::WmunuSingleJetEDBR>     BTagWeightProducerWmunuSingleJet;
DEFINE_FWK_MODULE(BTagWeightProducerWmunuSingleJet);

#undef EDBRNEUTRINO

//define VBF taggers
typedef EDBRTagger<cmg::DiElectronDiJetEDBR> DiElectronDiJetEDBRTagger;
typedef EDBRTagger<cmg::DiMuonDiJetEDBR>     DiMuonDiJetEDBRTagger;
DEFINE_FWK_MODULE(DiElectronDiJetEDBRTagger);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRTagger);
typedef EDBRTagger<cmg::DiElectronSingleJetEDBR> DiElectronSingleJetEDBRTagger;
typedef EDBRTagger<cmg::DiMuonSingleJetEDBR>     DiMuonSingleJetEDBRTagger;
DEFINE_FWK_MODULE(DiElectronSingleJetEDBRTagger);
DEFINE_FWK_MODULE(DiMuonSingleJetEDBRTagger);

#define EDBRNEUTRINO
typedef EDBRTagger<cmg::WelenuDiJetEDBR>     WelenuDiJetEDBRTagger;
DEFINE_FWK_MODULE(WelenuDiJetEDBRTagger);
typedef EDBRTagger<cmg::WmunuDiJetEDBR>     WmunuDiJetEDBRTagger;
DEFINE_FWK_MODULE(WmunuDiJetEDBRTagger);
typedef EDBRTagger<cmg::WelenuSingleJetEDBR>     WelenuSingleJetEDBRTagger;
DEFINE_FWK_MODULE(WelenuSingleJetEDBRTagger);
typedef EDBRTagger<cmg::WmunuSingleJetEDBR>     WmunuSingleJetEDBRTagger;
DEFINE_FWK_MODULE(WmunuSingleJetEDBRTagger);
#undef EDBRNEUTRINO

DEFINE_FWK_MODULE(DummyGenProducer);
//DEFINE_FWK_MODULE(DummyConversionProducer);
