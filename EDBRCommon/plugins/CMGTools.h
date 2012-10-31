#ifndef __ExoDiBosonResonances_EDBRCommon_CMGTools_h__
#define __ExoDiBosonResonances_EDBRCommon_CMGTools_h__

#include "ExoDiBosonResonances/EDBRCommon/interface/CompoundFactories.h"
//#include "ExoDiBosonResonances/EDBRCommon/interface/PFJetSmearFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

//typedef PhysicsObjectProducer<cmg::PFJetSmearFactory> PFJetSmearPOProducer;

typedef PhysicsObjectProducer<cmg::DiGenParticleFactory> DiGenParticlePOProducer;
//typedef PhysicsObjectProducer<cmg::DiPFJetKinFitFactory> DiPFJetKinFitPOProducer;
//typedef PhysicsObjectProducer<cmg::DiJetKinFitFactory> DiJetKinFitPOProducer;

typedef PhysicsObjectProducer<cmg::DiMuonDiJetFactory> DiMuonDiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronDiJetFactory> DiElectronDiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiGenParticleDiGenParticleFactory> DiGenParticleDiGenParticlePOProducer;

//typedef PhysicsObjectProducer<cmg::DiMuonDiJetHiggsFactory> DiMuonDiJetHiggsPOProducer;
//typedef PhysicsObjectProducer<cmg::DiElectronDiJetHiggsFactory> DiElectronDiJetHiggsPOProducer;
//typedef PhysicsObjectProducer<cmg::DiGenParticleDiGenParticleHiggsFactory> DiGenParticleDiGenParticleHiggsPOProducer;


#endif
