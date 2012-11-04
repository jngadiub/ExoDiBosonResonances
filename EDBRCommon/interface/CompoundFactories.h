#ifndef __ExoDiBosonResonances_EDBRCommon_CompoundFactories_h__
#define __ExoDiBosonResonances_EDBRCommon_CompoundFactories_h__

#include "CMGTools/Common/interface/DiObjectFactory.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/CompoundTypesHZZLL.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
//#include "ExoDiBosonResonances/EDBRCommon/interface/HiggsCandidateFactory.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/DiObjectKinFitFactory.h"
#include "ExoDiBosonResonances/EDBRCommon/interface/LeptonIsoCorrector.h"


namespace cmg{
  typedef DiObjectFactory< cmg::DiGenParticle::type1, cmg::DiGenParticle::type2 > DiGenParticleFactory;
  typedef DiObjectKinFitFactory< cmg::DiPFJet::type1, cmg::DiPFJet::type2 > DiPFJetKinFitFactory;
  //typedef DiObjectKinFitFactory< cmg::DiJet::type1, cmg::DiJet::type2 > DiJetKinFitFactory;
  //typedef DiObjectKinFitFactory< cmg::DiPFJet::type1, cmg::DiPFJet::type2 > DiJetKinFitFactory;
  
  typedef DiObjectFactory< cmg::DiElectronDiJet::type1, cmg::DiElectronDiJet::type2 > DiElectronDiJetFactory;
  typedef DiObjectFactory< cmg::DiMuonDiJet::type1, cmg::DiMuonDiJet::type2 > DiMuonDiJetFactory;
  typedef DiObjectFactory< cmg::DiGenParticleDiGenParticle::type1, cmg::DiGenParticleDiGenParticle::type2 > DiGenParticleDiGenParticleFactory;

  //typedef HiggsCandidateFactory< cmg::DiElectronDiJet::type1, cmg::DiElectronDiJet::type2 > DiElectronDiJetHiggsFactory;
  //typedef HiggsCandidateFactory< cmg::DiMuonDiJet::type1, cmg::DiMuonDiJet::type2 > DiMuonDiJetHiggsFactory;
  //typedef HiggsCandidateFactory< cmg::DiGenParticleDiGenParticle::type1, cmg::DiGenParticleDiGenParticle::type2 > DiGenParticleDiGenParticleHiggsFactory;
 
 
}


#endif
