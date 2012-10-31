#ifndef __HiggsAna_HLLJJCommon_CMGTools_h__
#define __HiggsAna_HLLJJCommon_CMGTools_h__


#include "HiggsAna/HLLJJCommon/interface/CompoundFactories.h"
#include "HiggsAna/HLLJJCommon/interface/PFJetSmearFactory.h"
#include "CMGTools/Common/plugins/PhysicsObjectProducer.h"

typedef PhysicsObjectProducer<cmg::PFJetSmearFactory> PFJetSmearPOProducer;

typedef PhysicsObjectProducer<cmg::DiGenParticleFactory> DiGenParticlePOProducer;
typedef PhysicsObjectProducer<cmg::DiPFJetKinFitFactory> DiPFJetKinFitPOProducer;
typedef PhysicsObjectProducer<cmg::DiJetKinFitFactory> DiJetKinFitPOProducer;

typedef PhysicsObjectProducer<cmg::DiMuonDiJetFactory> DiMuonDiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronDiJetFactory> DiElectronDiJetPOProducer;
typedef PhysicsObjectProducer<cmg::DiGenParticleDiGenParticleFactory> DiGenParticleDiGenParticlePOProducer;

typedef PhysicsObjectProducer<cmg::DiMuonDiJetHiggsFactory> DiMuonDiJetHiggsPOProducer;
typedef PhysicsObjectProducer<cmg::DiElectronDiJetHiggsFactory> DiElectronDiJetHiggsPOProducer;
typedef PhysicsObjectProducer<cmg::DiGenParticleDiGenParticleHiggsFactory> DiGenParticleDiGenParticleHiggsPOProducer;


#endif
