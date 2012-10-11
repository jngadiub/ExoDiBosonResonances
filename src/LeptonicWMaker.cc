#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include <TH1F.h>
#include <TH2F.h>
#include <TLorentzVector.h>

class LeptonicWMaker : public edm::EDFilter {
public:
  explicit LeptonicWMaker (const edm::ParameterSet &);
  ~LeptonicWMaker();
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  virtual void beginJob();
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  edm::InputTag lepton_;
  edm::InputTag neutrino_;
  edm::InputTag jet_;
  edm::InputTag MET_;

  TH1F* h_deltaPzNeutrino;
  TH1F* h_WWmass;
};

/// Constructor
LeptonicWMaker::LeptonicWMaker(const edm::ParameterSet & cfg) :
  lepton_(cfg.getUntrackedParameter<edm::InputTag> ("lepton")),
  neutrino_(cfg.getUntrackedParameter<edm::InputTag> ("neutrino")),
  jet_(cfg.getUntrackedParameter<edm::InputTag> ("jet")),
  MET_(cfg.getUntrackedParameter<edm::InputTag> ("MET")) 
{
  edm::Service<TFileService> fs;

  h_deltaPzNeutrino = fs->make<TH1F> ("deltaPzNeutrino","deltaPzNeutrino",100,-2,2);
  h_WWmass          = fs->make<TH1F> ("WWmass","WWmass",100,500,2500);
}

/// Destructor
LeptonicWMaker::~LeptonicWMaker(){}


/// beginJob and endJob
void
LeptonicWMaker::beginJob() {
}

void
LeptonicWMaker::endJob() {
}

/// filter method
bool
LeptonicWMaker::filter(edm::Event & iEvent, const edm::EventSetup & iEventSetup) {

  using namespace reco;

  // Get the lepton
  edm::Handle<CandidateView> lepton_h;
  iEvent.getByLabel(lepton_, lepton_h);

  // Get the neutrino
  edm::Handle<CandidateView> neutrino_h;
  iEvent.getByLabel(neutrino_, neutrino_h);

  // Get the jet
  edm::Handle<CandidateView> jet_h;
  iEvent.getByLabel(jet_, jet_h);

  // Get the MET
  edm::Handle<CandidateView> MET_h;
  iEvent.getByLabel(MET_, MET_h);


  const Candidate& theLepton   = lepton_h->at(0);
  const Candidate& theNeutrino = neutrino_h->at(0);
  const Candidate& theJet      = jet_h->at(0);
  const Candidate& theMET      = MET_h->at(0);
 
  double neutrinoPz     = theMET.pt() * theLepton.pz() / theLepton.pt();
  double neutrinoEnergy = sqrt(theMET.pt() * theMET.pt() + 
			       neutrinoPz * neutrinoPz);
  
  /// First validation histogram: deltaPzNeutrino
  double deltaPzNeutrino = (neutrinoPz - theNeutrino.pz())/theNeutrino.pz();
  h_deltaPzNeutrino->Fill(deltaPzNeutrino);

  /// Second validation histogram: WW Mass
  TLorentzVector nuCand(theMET.px(),
			theMET.py(),
			neutrinoPz,
			neutrinoEnergy);
  TLorentzVector leCand(theLepton.px(),
			theLepton.py(),
			theLepton.pz(),
			theLepton.energy());
  TLorentzVector leptonicWCand(nuCand+leCand);
  TLorentzVector hadronicWCand(theJet.px(), 
			       theJet.py(),
			       theJet.pz(),
			       theJet.energy());
  TLorentzVector WWCand(leptonicWCand + hadronicWCand);
  
  h_WWmass->Fill(WWCand.M());

  return true;
}

void
LeptonicWMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plugin
DEFINE_FWK_MODULE( LeptonicWMaker );
