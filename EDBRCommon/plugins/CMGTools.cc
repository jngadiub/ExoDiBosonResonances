#include "ExoDiBosonResonances/EDBRCommon/plugins/CMGTools.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(VJetPOProducer);
DEFINE_FWK_MODULE(PFJetSmearPOProducer); //ANIELLO
DEFINE_FWK_MODULE(CmgVJetSelector);

DEFINE_FWK_MODULE(NeutrinoPOProducer);
DEFINE_FWK_MODULE(CmgNeutrinoSelector);

DEFINE_FWK_MODULE(DiGenParticlePOProducer);

DEFINE_FWK_MODULE(DiGenParticleDiGenParticlePOProducer);
DEFINE_FWK_MODULE(DiMuonDiJetPOProducer);
DEFINE_FWK_MODULE(DiElectronDiJetPOProducer);
DEFINE_FWK_MODULE(DiPFJetKinFitPOProducer);
DEFINE_FWK_MODULE(DiJetKinFitPOProducer);

DEFINE_FWK_MODULE(DiMuonSingleJetPOProducer);
DEFINE_FWK_MODULE(DiElectronSingleJetPOProducer);

DEFINE_FWK_MODULE(DiGenParticleDiGenParticleEDBRPOProducer);
DEFINE_FWK_MODULE(DiMuonDiJetEDBRPOProducer);
DEFINE_FWK_MODULE(DiElectronDiJetEDBRPOProducer);

DEFINE_FWK_MODULE(DiMuonSingleJetEDBRPOProducer);
DEFINE_FWK_MODULE(DiElectronSingleJetEDBRPOProducer);


DEFINE_FWK_MODULE(WmunuPOProducer);
DEFINE_FWK_MODULE(WmunuEDBRPOProducer);
DEFINE_FWK_MODULE(CmgWelenuSelector);

DEFINE_FWK_MODULE(WelenuPOProducer);
DEFINE_FWK_MODULE(WelenuEDBRPOProducer);
DEFINE_FWK_MODULE(CmgWmunuSelector);



DEFINE_FWK_MODULE(WelenuDiJetPOProducer);
DEFINE_FWK_MODULE(WmunuDiJetPOProducer);
DEFINE_FWK_MODULE(WelenuSingleJetPOProducer);
DEFINE_FWK_MODULE(WmunuSingleJetPOProducer);

DEFINE_FWK_MODULE(WelenuDiJetEDBRPOProducer);
DEFINE_FWK_MODULE(WmunuDiJetEDBRPOProducer);
DEFINE_FWK_MODULE(WelenuSingleJetEDBRPOProducer);
DEFINE_FWK_MODULE(WmunuSingleJetEDBRPOProducer);

//typedef PhysicsObjectProducer<cmg::DiElectronFactory> DiElectronPOProducer;
//DEFINE_FWK_MODULE(DiElectronPOProducer);
//typedef PhysicsObjectProducer<cmg::DiMuonFactory> DiMuonPOProducer;
//DEFINE_FWK_MODULE(DiMuonPOProducer);
