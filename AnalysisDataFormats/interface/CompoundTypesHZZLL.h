#ifndef COMPOUNDTYPESHZZLL_H_
#define COMPOUNDTYPESHZZLL_H_

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"

#include "AnalysisDataFormats/HiggsAna/interface/HiggsCandidate.h"


namespace cmg{

  typedef cmg::DiObject<cmg::GenParticle,cmg::GenParticle> DiGenParticle;
    
  //  typedef cmg::DiObject<cmg::DiMuon,cmg::DiJet> DiMuonDiJet;
  // typedef cmg::DiObject<cmg::DiElectron,cmg::DiJet> DiElectronDiJet;
  typedef cmg::DiObject<cmg::DiMuon,cmg::DiPFJet> DiMuonDiJet;
  typedef cmg::DiObject<cmg::DiElectron,cmg::DiPFJet> DiElectronDiJet;
  typedef cmg::DiObject<cmg::DiMuon,cmg::DiPFJet> DiMuonDiPFJet;
  typedef cmg::DiObject<cmg::DiElectron,cmg::DiPFJet> DiElectronDiPFJet;
  typedef cmg::DiObject<cmg::DiGenParticle,cmg::DiGenParticle> DiGenParticleDiGenParticle;

  //  typedef cmg::HiggsCandidate<cmg::DiMuon,cmg::DiJet> DiMuonDiJetHiggs;
  // typedef cmg::HiggsCandidate<cmg::DiElectron,cmg::DiJet> DiElectronDiJetHiggs;
  typedef cmg::HiggsCandidate<cmg::DiMuon,cmg::DiPFJet> DiMuonDiJetHiggs;
  typedef cmg::HiggsCandidate<cmg::DiElectron,cmg::DiPFJet> DiElectronDiJetHiggs;
  typedef cmg::HiggsCandidate<cmg::DiGenParticle,cmg::DiGenParticle> DiGenParticleDiGenParticleHiggs;
    


}

#endif /*COMPOUNDTYPESHZZLL_H_*/
