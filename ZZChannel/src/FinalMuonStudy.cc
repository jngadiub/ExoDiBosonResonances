// system include files
#include <memory>
#include <bitset>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "ExoDiBosonResonances/EDBRMuon/interface/HPTMMuonSelector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/Utils/interface/StringToEnumValue.h"
#include "TEfficiency.h"
#include "TH1F.h"
#include "TH2F.h"

//
// class declaration
//
class FinalMuonStudy : public edm::EDAnalyzer {
   public:
      explicit FinalMuonStudy(const edm::ParameterSet&);
      ~FinalMuonStudy();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  // ----------member data ---------------------------
  edm::InputTag muons_;

  hptm::MuonSelector muonSelector_;

  TEfficiency* h_twoLegEffZPt_twoMuons;
  TEfficiency* h_twoLegEffNumVert_twoMuons;        
  TEfficiency* h_twoLegEffDeltaR_twoMuons;       
  TEfficiency* h_twoLegEffZPt_Iso;
  TEfficiency* h_twoLegEffNumVert_Iso;        
  TEfficiency* h_twoLegEffDeltaR_Iso;       
  TEfficiency* h_twoLegEffZPt;
  TEfficiency* h_twoLegEffNumVert;        
  TEfficiency* h_twoLegEffDeltaR;       
  TH2F*        h_ZPtXZMass;
  TH2F*        h_ZPtXZMass_Iso;
  TH2F*        h_ZPtXZMass_TwoMuons;
  TH1F* h_recoEta;
  TH1F* h_dR;
  TH1F* h_deltaPtOverPt_bad;
  TH1F* h_recoEta_bad;
  TH1F* h_dR_bad;
  TH1F* h_chiSquare_bad;
  TH1F* h_outer;
  TH1F* h_inner;
  TH1F* h_global;
  TH1F* h_tpfms;
  TH1F* h_picky;
  TH1F* h_numBadMuons;

};  
  
FinalMuonStudy::FinalMuonStudy(const edm::ParameterSet& iConfig)
{
  muons_= iConfig.getParameter<edm::InputTag>("muons");
  
  edm::Service<TFileService> fs;
  
  h_twoLegEffZPt             = fs->make<TEfficiency>("twoLegEffZPt",";Z p_{T} (GeV); Efficiency",50,0,1000); 
  h_twoLegEffNumVert         = fs->make<TEfficiency>("twoLegEffNumVert",";Number of vertices; Efficiency",50,-0.5,49.5); 
  h_twoLegEffDeltaR          = fs->make<TEfficiency>("twoLegEffDeltaR",";#Delta R; Efficiency",60,0.0,1.5); 
  h_twoLegEffZPt_twoMuons             = fs->make<TEfficiency>("twoLegEffZPt_twoMuons",";Z p_{T} (GeV); Efficiency",50,0,1000); 
  h_twoLegEffNumVert_twoMuons         = fs->make<TEfficiency>("twoLegEffNumVert_twoMuons",";Number of vertices; Efficiency",50,-0.5,49.5); 
  h_twoLegEffDeltaR_twoMuons          = fs->make<TEfficiency>("twoLegEffDeltaR_twoMuons",";#Delta R; Efficiency",60,0.0,1.5); 
  h_twoLegEffZPt_Iso             = fs->make<TEfficiency>("twoLegEffZPt_Iso",";Z p_{T} (GeV); Efficiency",50,0,1000); 
  h_twoLegEffNumVert_Iso         = fs->make<TEfficiency>("twoLegEffNumVert_Iso",";Number of vertices; Efficiency",50,-0.5,49.5); 
  h_twoLegEffDeltaR_Iso          = fs->make<TEfficiency>("twoLegEffDeltaR_Iso",";#Delta R; Efficiency",60,0.0,1.5); 
  h_ZPtXZMass = fs->make<TH2F>("ZPtXZMass",";Z p_{T};M_{Z} (GeV)",50,0,1000,100,0,200);
  h_ZPtXZMass_Iso = fs->make<TH2F>("ZPtXZMass_Iso",";Z p_{T};M_{Z} (GeV)",50,0,1000,100,0,200);
  h_ZPtXZMass_TwoMuons = fs->make<TH2F>("ZPtXZMass_TwoMuons",";Z p_{T};M_{Z} (GeV)",50,0,1000,100,0,200);
  h_deltaPtOverPt_bad = fs->make<TH1F>("deltaPtOverPt_bad",";#Delta p_{T}/p_{T};",200,-1,1);
  h_recoEta = fs->make<TH1F>("recoEta","recoEta",48,-2.4,2.4);
  h_recoEta_bad = fs->make<TH1F>("recoEta_bad","recoEta_bad",48,-2.4,2.4);
  h_chiSquare_bad = fs->make<TH1F>("chiSquare_bad","chiSquare_bad",100,0,10);

  h_outer = fs->make<TH1F>("outer",";#Delta p_{T}/p_{T};",200,-1,1);
  h_inner = fs->make<TH1F>("inner",";#Delta p_{T}/p_{T};",200,-1,1);
  h_global= fs->make<TH1F>("global",";#Delta p_{T}/p_{T};",200,-1,1);
  h_tpfms = fs->make<TH1F>("tmpfs",";#Delta p_{T}/p_{T};",200,-1,1);
  h_picky = fs->make<TH1F>("picky",";#Delta p_{T}/p_{T};",200,-1,1);

  h_dR  = fs->make<TH1F>("dR",";#Delta R;",150,0.0,1.5);
  h_dR_bad = fs->make<TH1F>("dR_bad",";#Delta R;",150,0.0,1.5);
  h_numBadMuons = fs->make<TH1F>("numBadMuons",";Number of bad muons;",3,-0.5,2.5);

}


FinalMuonStudy::~FinalMuonStudy()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
FinalMuonStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<edm::View<reco::Muon> > muonHandle;
  iEvent.getByLabel(muons_,muonHandle);
  const edm::View<reco::Muon>& muons = *(muonHandle.product());

  edm::Handle<edm::View<reco::Vertex> > vertexHandle;
  iEvent.getByLabel("offlinePrimaryVertices",vertexHandle);
  const edm::View<reco::Vertex>& vertices = *(vertexHandle.product());
  const reco::Vertex& vertex = vertices[0];

  // Get the gen muons
  edm::Handle<edm::View<reco::Candidate> > genMuonHandle;
  iEvent.getByLabel("genMuons",genMuonHandle);
  const edm::View<reco::Candidate>& genMuons = *(genMuonHandle.product());

  edm::Handle<edm::View<reco::Candidate> > otherMuonHandle;
  iEvent.getByLabel("otherMuons",otherMuonHandle);
  const edm::View<reco::Candidate>& otherMuons = *(otherMuonHandle.product());

  edm::Handle<edm::View<reco::Candidate> > photonHandle;
  iEvent.getByLabel("hptPhotons",photonHandle);
  const edm::View<reco::Candidate>& photons = *(photonHandle.product());

  //int numMuons = muons.size();
  //int numMatchedMuons = 0;
  //int numGoodMuons = 0;

  // Run over the muons and get the indices of RECO muons
  // which have a match to a GEN muon.
  std::vector<size_t> matchedMuons;
  std::vector<size_t> matchedGenMuons;

  for(size_t muonNr = 0; muonNr != muons.size(); ++muonNr) {
     
    const reco::Muon& recoMu = muons[muonNr];
    
    // First, let's see if this muon has a gen match
    double minDeltaR = 10000;
    size_t matchedGenMuon = 9999;
    for(size_t genMuonNr = 0; genMuonNr != genMuons.size(); ++genMuonNr) {
      const reco::Candidate& genMuon = genMuons[genMuonNr];
      double dR= deltaR(recoMu.eta(),recoMu.phi(),genMuon.eta(),genMuon.phi());
      if(dR < minDeltaR) {
	minDeltaR = dR;
	matchedGenMuon = genMuonNr;
      }   
    }

    if(minDeltaR > 0.05) continue; // Don't care about non-matched muons.

    // If we got here the muon is matched
    matchedMuons.push_back(muonNr);
    matchedGenMuons.push_back(matchedGenMuon);
  }

  if(matchedMuons.size() != 2) return; // Don't care about corner cases where we managed to match different number of muons.

  // Make the Z candidate
  const reco::Muon& m0 = muons[matchedMuons[0]];
  const reco::Muon& m1 = muons[matchedMuons[1]];
  reco::CompositeCandidate dimuon;
  dimuon.addDaughter(m0,"First muon");
  dimuon.addDaughter(m1,"Second muon");
  AddFourMomenta addP4;
  addP4.set( dimuon );

  double ZPt = dimuon.pt();
  double Zeta = dimuon.eta();
  double lmuPt = m0.pt() > m1.pt() ? m0.pt() : m1.pt();
  double lmuEta = m0.pt() > m1.pt() ? m0.eta() : m1.eta();
  double dR = deltaR(m0.eta(),m0.phi(),m1.eta(),m1.phi());
  int numVertices = vertices.size();

  h_recoEta->Fill(m0.eta());
  h_recoEta->Fill(m1.eta());
  h_dR->Fill(dR);

  bool firstMuonPassedHPT  = muonSelector_.checkMuonID(m0, vertex.position(), hptm::HIGHPT_OLD);
  bool secondMuonPassedHPT = muonSelector_.checkMuonID(m1, vertex.position(), hptm::HIGHPT_OLD);
  bool firstMuonPassedTRK  = muonSelector_.checkMuonID(m0, vertex.position(), hptm::TRACKER);
  bool secondMuonPassedTRK = muonSelector_.checkMuonID(m1, vertex.position(), hptm::TRACKER);

  bool passedHPTHPT = (firstMuonPassedHPT and secondMuonPassedHPT);
  bool passedHPTTRK = ((firstMuonPassedHPT and secondMuonPassedTRK) or (firstMuonPassedTRK and secondMuonPassedHPT));

  // Equivalent to bool=passed in the other code
  bool eventHasTwoGoodMuons = (passedHPTHPT);

  // Check the isolation
  bool m0PassedIso = false;
  bool m1PassedIso = false;
  // Artificial scope
  {
    // Detector-based isolation
    const reco::MuonIsolation& muonIso = m0.isolationR03();
    double detIsol = muonIso.sumPt/m0.pt();
    
    double correctionDetIsol = 9999;
    if(m1.innerTrack().isNonnull()) {
      correctionDetIsol = dR > 0.3 ? 0 : (-m1.innerTrack()->pt()/m0.pt());
    }
    
    if((detIsol+correctionDetIsol)<0.1)
      m0PassedIso = true;
  }
  {
    // Detector-based isolation
    const reco::MuonIsolation& muonIso = m1.isolationR03();
    double detIsol = muonIso.sumPt/m1.pt();
    
    double correctionDetIsol = 9999;
    if(m0.innerTrack().isNonnull()) {
      correctionDetIsol = dR > 0.3 ? 0 : (-m0.innerTrack()->pt()/m1.pt());
    }
    
    if((detIsol+correctionDetIsol)<0.1)
      m1PassedIso = true;
  }

  bool eventHasTwoIsolatedMuons = (m0PassedIso and m1PassedIso);

  bool eventHasTwoGoodIsolatedMuons = eventHasTwoGoodMuons and eventHasTwoIsolatedMuons;
  
  double dimuonMass = dimuon.mass();
  bool dimuonWindow = (dimuonMass > 70 and dimuonMass < 110);
  bool antiDimuonWindow = (dimuonMass < 70);
  bool eventSelected = (eventHasTwoGoodMuons and
			eventHasTwoIsolatedMuons and 
			dimuonWindow);

  
  if(eventHasTwoGoodIsolatedMuons) {
  }
  
  //if(eventSelected)
  //printf("Event Selected\n");
  h_twoLegEffZPt->Fill(eventSelected,ZPt);
  h_twoLegEffNumVert->Fill(eventSelected,numVertices);        
  h_twoLegEffDeltaR->Fill(eventSelected,dR);       
  h_twoLegEffZPt_twoMuons->Fill(eventHasTwoGoodMuons,ZPt);
  h_twoLegEffNumVert_twoMuons->Fill(eventHasTwoGoodMuons,numVertices);        
  h_twoLegEffDeltaR_twoMuons->Fill(eventHasTwoGoodMuons,dR);       
  h_twoLegEffZPt_Iso->Fill(eventHasTwoGoodIsolatedMuons,ZPt);
  h_twoLegEffNumVert_Iso->Fill(eventHasTwoGoodIsolatedMuons,numVertices);        
  h_twoLegEffDeltaR_Iso->Fill(eventHasTwoGoodIsolatedMuons,dR);       
  if(eventHasTwoGoodMuons) {
    h_ZPtXZMass_TwoMuons->Fill(dimuon.pt(),dimuon.mass());
  }
  if(eventHasTwoGoodIsolatedMuons) {
    h_ZPtXZMass_Iso->Fill(dimuon.pt(),dimuon.mass());
  }
  if(eventSelected) {
    h_ZPtXZMass->Fill(dimuon.pt(),dimuon.mass());
  }

  // Special studies for badly-reconstructed muons.
  if(eventHasTwoGoodIsolatedMuons and antiDimuonWindow) {
    
    int numberOfBadMuonsPerEvent = 0;
    bool thisEventAlready = false;
    
    for(int i=0; i!=2; ++i) {
      
      const reco::Muon& recoMu = muons[matchedMuons[i]];
      const reco::Candidate& genMu = genMuons[matchedGenMuons[i]];
      
      double matchingdR = deltaR(recoMu.eta(),recoMu.phi(),genMu.eta(),genMu.phi());
      assert(matchingdR < 0.051);
      
      double deltaPtOverPt = (recoMu.pt() - genMu.pt())/genMu.pt();
      
      if(deltaPtOverPt < (-0.2)) {
	
	printf("*******\n");
	int eventNumber = iEvent.id().event();
	printf("genMu.pt = %g for recoMu.pt = %g\n",genMu.pt(),recoMu.pt());
	printf("Gen muons:\n");
	for(size_t ii = 0; ii != genMuons.size(); ++ii) {
          const reco::Candidate& xm = genMuons[ii];
          printf("Status 3 muon:.pt = %g, eta = %g, phi = %g\n",xm.pt(),xm.eta(),xm.phi());
        }
	for(size_t ii = 0; ii != otherMuons.size(); ++ii) {
	  const reco::Candidate& xm = otherMuons[ii];
	  printf("Status 1 muon:.pt = %g, eta = %g, phi = %g\n",xm.pt(),xm.eta(),xm.phi());
	}
	for(size_t ii = 0; ii != photons.size(); ++ii) {
	  const reco::Candidate& xm = photons[ii];
	  printf("Status 1 photon:.pt = %g, eta = %g, phi = %g\n",xm.pt(),xm.eta(),xm.phi());
	}

	double m0pt = m0.pt();
	double m1pt = m1.pt();
	printf("Event number %i, m0.pt = %g, m1.pt = %g\n",eventNumber,m0pt,m1pt);
	
	numberOfBadMuonsPerEvent++;
	
	// Okay, we're in the REALLY problematic region...
	// Check the eta of the muons.
	double recoEta = recoMu.eta();
	h_recoEta_bad->Fill(recoEta);
	
	// Check if ANY of the pts gets it right!
	const reco::TrackRef& innerTrackRef = recoMu.innerTrack();
	if(innerTrackRef.isNonnull()) {
	  h_chiSquare_bad->Fill(innerTrackRef->normalizedChi2());
	  double dptpt = (innerTrackRef->pt() - genMu.pt())/genMu.pt();
	  h_inner->Fill(dptpt);
	}
	
	const reco::TrackRef& outerTrackRef = recoMu.outerTrack();
	if(outerTrackRef.isNonnull()) {
	  double dptpt = (outerTrackRef->pt() - genMu.pt())/genMu.pt();
	  h_outer->Fill(dptpt);
	}
	
	const reco::TrackRef& globalTrackRef = recoMu.globalTrack();
	if(globalTrackRef.isNonnull()) {
	  double dptpt = (globalTrackRef->pt() - genMu.pt())/genMu.pt();
	  h_global->Fill(dptpt);
	}
	
	const reco::TrackRef& tpfmsTrackRef = recoMu.tpfmsTrack();
	if(tpfmsTrackRef.isNonnull()) {
	  double dptpt = (tpfmsTrackRef->pt() - genMu.pt())/genMu.pt();
	  h_tpfms->Fill(dptpt);
	}
	
	const reco::TrackRef& pickyTrackRef = recoMu.pickyTrack();
	if(pickyTrackRef.isNonnull()) {
	  double dptpt = (pickyTrackRef->pt() - genMu.pt())/genMu.pt();
	  h_picky->Fill(dptpt);
	}

	if(!thisEventAlready) {
	  h_dR_bad->Fill(dR);
	  thisEventAlready = true;
	}

      } // closes dptpt < -0.2
      
      h_deltaPtOverPt_bad->Fill(deltaPtOverPt);
      
    } // close loop over muons in events out of dimuon windows
    
    h_numBadMuons->Fill(numberOfBadMuonsPerEvent);
    
  } // close if event is out of dimuon window
  
  
  return;
}  

// ------------ method called once each job just before starting event loop  ------------
void 
FinalMuonStudy::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FinalMuonStudy::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
FinalMuonStudy::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
FinalMuonStudy::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
FinalMuonStudy::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
FinalMuonStudy::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FinalMuonStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FinalMuonStudy);
