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
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/Neutrino.h"

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

  typedef cmg::DiObject<cmg::Muon,cmg::Neutrino> Wmunu;
  typedef cmg::DiObject<cmg::Electron,cmg::Neutrino> Welenu;
 
  typedef cmg::DiObject<cmg::Wmunu,cmg::DiPFJet> WmunuDiJet;
  typedef cmg::DiObject<cmg::Wmunu,cmg::VJet> WmunuSingleJet;

  typedef cmg::DiObject<cmg::Welenu,cmg::DiPFJet> WelenuDiJet;
  typedef cmg::DiObject<cmg::Welenu,cmg::VJet> WelenuSingleJet;


  // // // typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::DiJet> DiMuonDiJetEDBR;
  // // // typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::DiJet> DiElectronDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::DiPFJet> DiMuonDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::DiPFJet> DiElectronDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiGenParticle,cmg::DiGenParticle> DiGenParticleDiGenParticleEDBR;
    
  typedef cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> DiMuonSingleJetEDBR;
  typedef cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> DiElectronSingleJetEDBR;

  typedef cmg::EDBRCandidate<cmg::Muon,cmg::Neutrino> WmunuEDBR;
  typedef cmg::EDBRCandidate<cmg::Electron,cmg::Neutrino> WelenuEDBR;
  
  typedef cmg::EDBRCandidate<cmg::Wmunu,cmg::DiPFJet> WmunuDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::Wmunu,cmg::VJet> WmunuSingleJetEDBR;

  typedef cmg::EDBRCandidate<cmg::Welenu,cmg::DiPFJet> WelenuDiJetEDBR;
  typedef cmg::EDBRCandidate<cmg::Welenu,cmg::VJet> WelenuSingleJetEDBR;  	

}

#endif /*COMPOUNDTYPESHZZLL_H_*/
