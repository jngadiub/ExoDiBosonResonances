#ifndef __ExoDiBosonResonances_EDBRCommon_CompoundFactories_h__
#define __ExoDiBosonResonances_EDBRCommon_CompoundFactories_h__

#include "CMGTools/Common/interface/DiObjectFactory.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/CompoundTypesHZZLL.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/EDBRCandidateFactory.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/DiObjectKinFitFactory.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/LeptonIsoCorrector.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/LeptonDetIsoCorrector.h"


namespace cmg{
  typedef DiObjectFactory< cmg::DiGenParticle::type1, cmg::DiGenParticle::type2 > DiGenParticleFactory;
  typedef DiObjectKinFitFactory< cmg::DiPFJet::type1, cmg::DiPFJet::type2 > DiPFJetKinFitFactory;
  typedef DiObjectKinFitFactory< cmg::DiJet::type1, cmg::DiJet::type2 > DiJetKinFitFactory;
  //typedef DiObjectKinFitFactory< cmg::DiPFJet::type1, cmg::DiPFJet::type2 > DiJetKinFitFactory;
  
  typedef DiObjectFactory< cmg::DiElectronDiJet::type1, cmg::DiElectronDiJet::type2 > DiElectronDiJetFactory;
  typedef DiObjectFactory< cmg::DiMuonDiJet::type1, cmg::DiMuonDiJet::type2 > DiMuonDiJetFactory;
  typedef DiObjectFactory< cmg::DiGenParticleDiGenParticle::type1, cmg::DiGenParticleDiGenParticle::type2 > DiGenParticleDiGenParticleFactory;

  typedef EDBRCandidateFactory< cmg::DiElectronDiJet::type1, cmg::DiElectronDiJet::type2 > DiElectronDiJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::DiMuonDiJet::type1, cmg::DiMuonDiJet::type2 > DiMuonDiJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::DiGenParticleDiGenParticle::type1, cmg::DiGenParticleDiGenParticle::type2 > DiGenParticleDiGenParticleEDBRFactory;


  typedef DiObjectFactory< cmg::DiElectronSingleJet::type1, cmg::DiElectronSingleJet::type2 > DiElectronSingleJetFactory;
  typedef DiObjectFactory< cmg::DiMuonSingleJet::type1, cmg::DiMuonSingleJet::type2 > DiMuonSingleJetFactory;
  typedef EDBRCandidateFactory< cmg::DiElectronSingleJet::type1, cmg::DiElectronSingleJet::type2 > DiElectronSingleJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::DiMuonSingleJet::type1, cmg::DiMuonSingleJet::type2 > DiMuonSingleJetEDBRFactory;


  typedef DiObjectFactory< cmg::Wmunu::type1, cmg::Wmunu::type2 > WmunuFactory;
  typedef DiObjectFactory< cmg::Welenu::type1, cmg::Welenu::type2 > WelenuFactory;
  typedef EDBRCandidateFactory< cmg::Wmunu::type1, cmg::Wmunu::type2  > WmunuEDBRFactory;
  typedef EDBRCandidateFactory< cmg::Welenu::type1, cmg::Welenu::type2 > WelenuEDBRFactory;

  typedef DiObjectFactory< cmg::WelenuDiJet::type1, cmg::WelenuDiJet::type2 > WelenuDiJetFactory;
  typedef DiObjectFactory< cmg::WmunuDiJet::type1, cmg::WmunuDiJet::type2 > WmunuDiJetFactory;
  typedef DiObjectFactory< cmg::WelenuSingleJet::type1, cmg::WelenuSingleJet::type2 > WelenuSingleJetFactory;
  typedef DiObjectFactory< cmg::WmunuSingleJet::type1, cmg::WmunuSingleJet::type2 > WmunuSingleJetFactory;

  typedef EDBRCandidateFactory< cmg::WelenuDiJet::type1, cmg::WelenuDiJet::type2 > WelenuDiJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::WmunuDiJet::type1, cmg::WmunuDiJet::type2 > WmunuDiJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::WelenuSingleJet::type1, cmg::WelenuSingleJet::type2 > WelenuSingleJetEDBRFactory;
  typedef EDBRCandidateFactory< cmg::WmunuSingleJet::type1, cmg::WmunuSingleJet::type2 > WmunuSingleJetEDBRFactory;
 
}


#endif
