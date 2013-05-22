#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"
#include "DataFormats/Math/interface/LorentzVector.h"

/// Class declaration
class HPTMNewTunePProducer : public edm::EDProducer {

private:
  edm::InputTag muLabel_; 

public:
  explicit HPTMNewTunePProducer(const edm::ParameterSet& iPara);
  ~HPTMNewTunePProducer(){}
private:
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
  double muonMass;
};

/// Class Definition
HPTMNewTunePProducer::HPTMNewTunePProducer(const edm::ParameterSet& iPara)  
{
  muonMass = 0.1056583715; //GeV 
  muLabel_=iPara.getParameter<edm::InputTag>("muLabel");
  produces < reco::MuonCollection >();
}

void HPTMNewTunePProducer::produce(edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 
  //load muons
  edm::Handle<edm::View<reco::Muon> > muHandle;
  iEvent.getByLabel(muLabel_,muHandle);
  const edm::View<reco::Muon>& muons = *(muHandle.product());

  //prepare output collection
  std::auto_ptr<reco::MuonCollection> outMuons(new reco::MuonCollection());

  //clone muons, set pt to TuneP pt
  for(size_t muonNr=0;muonNr<muons.size();muonNr++){

    reco::Muon * recoMu = muons[muonNr].clone();

    if(recoMu->isTrackerMuon()) {
    
    // call to get the optimal muon track
      reco::TrackRef cktTrack = (muon::tevOptimized(*recoMu, 200, 17., 40., 0.25)).first; 
      double oPx = cktTrack->px();
      double oPy = cktTrack->py();
      double oPz = cktTrack->pz();
      double oE = sqrt((oPx*oPx) + (oPy*oPy) + (oPz*oPz) + (muonMass*muonMass));
      math::XYZTLorentzVector oP4(oPx,oPy,oPz,oE);
      
      recoMu->setP4(oP4); /// This sets the new LorentzVector
      recoMu->setTrack(cktTrack); /// This sets the chosen track
    }
    
    outMuons->push_back(*(recoMu));
    delete recoMu;
  }

  //store the output
  iEvent.put(outMuons);
}



//define this as a plug-in
DEFINE_FWK_MODULE(HPTMNewTunePProducer);
