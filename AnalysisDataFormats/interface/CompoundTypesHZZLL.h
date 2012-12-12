#ifndef COMPOUNDTYPESHZZLL_H_
#define COMPOUNDTYPESHZZLL_H_

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"

#include "AnalysisDataFormats/ExoDiBosonResonances/interface/EDBRCandidate.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/VJet.h"


namespace cmg{

  typedef cmg::DiObject<cmg::GenParticle,cmg::GenParticle> DiGenParticle;
    
  //  typedef cmg::DiObject<cmg::DiMuon,cmg::DiJet> DiMuonDiJet;
  // typedef cmg::DiObject<cmg::DiElectron,cmg::DiJet> DiElectronDiJet;
  typedef cmg::DiObject<cmg::DiMuon,cmg::DiPFJet> DiMuonDiJet;
  typedef cmg::DiObject<cmg::DiElectron,cmg::DiPFJet> DiElectronDiJet;

  typedef cmg::DiObject<cmg::DiMuon,cmg::DiPFJet> DiMuonDiPFJet;
  typedef cmg::DiObject<cmg::DiElectron,cmg::DiPFJet> DiElectronDiPFJet;
  typedef cmg::DiObject<cmg::DiGenParticle,cmg::DiGenParticle> DiGenParticleDiGenParticle;

  typedef cmg::DiObject<cmg::DiMuon,cmg::VJet> DiMuonSingleJet;
  typedef cmg::DiObject<cmg::DiElectron,cmg::VJet> DiElectronSingleJet;
 

  // // // typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::DiJet> DiMuonDiJetEDBR;
  // // // typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::DiJet> DiElectronDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::DiPFJet> DiMuonDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::DiPFJet> DiElectronDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiGenParticle,cmg::DiGenParticle> DiGenParticleDiGenParticleEDBR;
    
  typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> DiMuonSingleJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> DiElectronSingleJetEDBR;

}

#endif /*COMPOUNDTYPESHZZLL_H_*/
