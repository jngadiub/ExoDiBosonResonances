#include "ExoDiBosonResonances/EDBRCommon/plugins/CMGTools.h"
#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(VJetPOProducer);
//DEFINE_FWK_MODULE(PFJetSmearPOProducer);
DEFINE_FWK_MODULE(DiGenParticlePOProducer);

DEFINE_FWK_MODULE(DiGenParticleDiGenParticlePOProducer);
DEFINE_FWK_MODULE(DiMuonDiJetPOProducer);
DEFINE_FWK_MODULE(DiElectronDiJetPOProducer);
DEFINE_FWK_MODULE(DiPFJetKinFitPOProducer);
DEFINE_FWK_MODULE(DiJetKinFitPOProducer);

DEFINE_FWK_MODULE(DiGenParticleDiGenParticleEDBRPOProducer);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRPOProducer);
DEFINE_FWK_MODULE(DiElectronDiJetEDBRPOProducer);

//typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
//DEFINE_FWK_MODULE(DiElectronPOProducer);
//typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
//DEFINE_FWK_MODULE(DiMuonPOProducer);
