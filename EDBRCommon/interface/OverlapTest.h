#ifndef OverlapTest_h
#define OverlapTest_h


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "PhysicsTools/PatUtils/interface/StringParserTools.h"


#include "PhysicsTools/PatUtils/interface/PATDiObjectProxy.h"


namespace cmg { namespace helper {
  
  // Base class for a test for overlaps
  class OverlapTest {
  public:
    OverlapTest(const std::string &name, const edm::ParameterSet &iConfig) :
      src_(iConfig.getParameter<edm::InputTag>("src")),
      name_(name),
      requireNoOverlaps_(iConfig.getParameter<bool>("requireNoOverlaps")),
      presel_(iConfig.getParameter<std::string>("preselection")),
      deltaR_(iConfig.getParameter<double>("deltaR")),
      checkRecoComponents_(iConfig.getParameter<bool>("checkRecoComponents")),
      pairCut_(iConfig.getParameter<std::string>("pairCut")) {}
    // implementation of mother methods
    /// Read input, apply preselection cut
    virtual void readInput(const edm::Event & iEvent, const edm::EventSetup &iSetup) ;
    /// Check for overlaps
    virtual bool fillOverlapsForItem(const reco::Candidate &item) const ;

    const std::string & name() const { return name_; }
    bool requireNoOverlaps() const { return requireNoOverlaps_; }
    
  protected:
    edm::InputTag src_;
    std::string   name_;
    bool          requireNoOverlaps_;
    // ---- configurables ----
    /// A generic preselection cut that can work on any Candidate, but has access also to methods of PAT specific objects
    StringCutObjectSelector<reco::Candidate,true> presel_;
    /// Delta R for the match
    double deltaR_;
    /// Check the overlapping by RECO components
    bool checkRecoComponents_;
    /// Cut on the pair of objects together
    StringCutObjectSelector<pat::DiObjectProxy,true> pairCut_;
    // ---- working variables ----
    /// The collection to check overlaps against
    edm::Handle<reco::CandidateView> candidates_;
    /// Flag saying if each element has passed the preselection or not
    std::vector<bool> isPreselected_;
  };

}}

#endif
