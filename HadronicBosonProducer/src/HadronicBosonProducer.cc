// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
//
// class declaration
//

class HadronicBosonProducer : public edm::EDProducer {
   public:
      explicit HadronicBosonProducer(const edm::ParameterSet&);
      ~HadronicBosonProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
  edm::InputTag jets_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
HadronicBosonProducer::HadronicBosonProducer(const edm::ParameterSet& iConfig):
  jets_(iConfig.getParameter<edm::InputTag>("jets"))
{
  // We produce a new collection of candidates with references to their daughters.
  produces<std::vector<pat::CompositeCandidate> >();
}


HadronicBosonProducer::~HadronicBosonProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HadronicBosonProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   std::auto_ptr<std::vector<pat::CompositeCandidate> > hadronicBosonCandidates( new std::vector<pat::CompositeCandidate>());

   // First, we get all the jets in the event.
   // There is an underlying assumption here that these jets have already been treated
   // (pt and eta cuts, jet ID, grooming);
   Handle<edm::View<reco::Candidate> > jets;
   iEvent.getByLabel(jets_,jets);

   // We could set some preselection cuts here...
   // But for the time being, we do not do that.

   // Loop over the jets, and make a hadronic boson candidate for each jet
   size_t numJets = jets->size();

   for(size_t i = 0; i!= numJets; ++i) {
     const reco::Candidate& j1 = jets->at(i);
     pat::CompositeCandidate HBSingleJet;
     HBSingleJet.addDaughter(j1);
     AddFourMomenta addFourMomenta;
     addFourMomenta.set( HBSingleJet );
     
     // Here the mass drop is the mass drop in between both subjets of the leading jet
     double massdrop = 1.0;
     if(j1.numberOfDaughters() > 1)
       massdrop = std::max(j1.daughter(0)->mass(),j1.daughter(1)->mass())/j1.mass();
     else
       massdrop = 1.0;
     HBSingleJet.addUserFloat( "massdrop", massdrop);
     HBSingleJet.addUserInt( "nJets", 1);
     hadronicBosonCandidates->push_back(HBSingleJet);
   }

   // Now loop over the jets, and make a hadronic boson candidate for each pair of jets
   for(size_t i = 0; i!= numJets; ++i) {
     for(size_t j = i; j!= numJets; ++j) {
       
       if(i==j) continue; // We want different jets
       const reco::Candidate& j1 = jets->at(i);
       const reco::Candidate& j2 = jets->at(j);
       pat::CompositeCandidate HBDoubleJet;
       HBDoubleJet.addDaughter(j1);
       HBDoubleJet.addDaughter(j2);
       AddFourMomenta addFourMomenta;
       addFourMomenta.set( HBDoubleJet );
       
       // Here the mass drop is the mass drop in between jets
       double massdrop = std::max(j1.mass(),j2.mass())/HBDoubleJet.mass();
       HBDoubleJet.addUserFloat( "massdrop", massdrop);
       HBDoubleJet.addUserInt( "nJets", 2);
       hadronicBosonCandidates->push_back(HBDoubleJet);
     }
   }

   // Put the hadronic bosons in the event.
   iEvent.put(hadronicBosonCandidates);

 
}

// ------------ method called once each job just before starting event loop  ------------
void 
HadronicBosonProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HadronicBosonProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
HadronicBosonProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HadronicBosonProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HadronicBosonProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HadronicBosonProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HadronicBosonProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HadronicBosonProducer);
