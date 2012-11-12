// user include files                                                                                                                                       
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "SHarper/HEEPAnalyzer/interface/HEEPCutCodes.h"

#include "TEfficiency.h"
#include "TH1F.h"

//
// class declaration
//

class HEEPElectronTurnon : public edm::EDAnalyzer {
   public:
      explicit HEEPElectronTurnon(const edm::ParameterSet&);
      ~HEEPElectronTurnon();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

  edm::InputTag eleLabel_;

  TEfficiency* h_efficiency;
  TH1F* h_passingBits;
  int electronsAnalyzed;
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
HEEPElectronTurnon::HEEPElectronTurnon(const edm::ParameterSet& iConfig) :
  eleLabel_(iConfig.getParameter<edm::InputTag>("eleLabel"))
{
  edm::Service<TFileService> fs;
  h_efficiency = fs->make<TEfficiency>("efficiency",";Electron p_{T} (GeV); Efficiency",200,0,1000); 
  h_passingBits = fs->make<TH1F>("passingBits","passingBits",14,0,14);
  h_passingBits->SetBit(TH1::kCanRebin);
  electronsAnalyzed = 0;
}


HEEPElectronTurnon::~HEEPElectronTurnon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HEEPElectronTurnon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get the electrons
  edm::Handle<edm::View<pat::Electron> > eleHandle;
  iEvent.getByLabel(eleLabel_,eleHandle);
  const edm::View<pat::Electron>& eles = *(eleHandle.product());
  

  for(size_t eleNr = 0; eleNr != eles.size(); ++eleNr) {

      const pat::Electron& ele = eles[eleNr];

      // Check that the electron is in the right region
      double et = ele.et();
      double pt = ele.pt();
      double inBarrel = ele.isEB();
      if(!inBarrel) continue;

      ++electronsAnalyzed;
      
      // Fill the efficiency
      int eleCutCode = ele.userInt("HEEPId");
      bool passed = (eleCutCode==0x0);
      h_efficiency->Fill(passed,pt);
      
      // Let's find out WHY this didn't pass

      h_passingBits->Fill("TOTAL",1);

      if((eleCutCode&heep::CutCodes::ET)==0x0)
	h_passingBits->Fill("ET",1);
      if((eleCutCode&heep::CutCodes::PT)==0x0)
	h_passingBits->Fill("PT",1);
      if((eleCutCode&heep::CutCodes::DETETA)==0x0)
	h_passingBits->Fill("DETETA",1);
      if((eleCutCode&heep::CutCodes::CRACK)==0x0)
	h_passingBits->Fill("CRACK",1);
      if((eleCutCode&heep::CutCodes::DETAIN)==0x0)
	h_passingBits->Fill("DETAIN",1);
      if((eleCutCode&heep::CutCodes::DPHIIN)==0x0)
	h_passingBits->Fill("DPHIIN",1);
      if((eleCutCode&heep::CutCodes::HADEM)==0x0)
	h_passingBits->Fill("HADEM",1);
      if((eleCutCode&heep::CutCodes::SIGMAIETAIETA)==0x0)
	h_passingBits->Fill("SIGMAIETAIETA",1);
      if((eleCutCode&heep::CutCodes::E2X5OVER5X5)==0x0)
	h_passingBits->Fill("E2X5OVER5X5",1);
      if((eleCutCode&heep::CutCodes::ISOLEMHADDEPTH1)==0x0)
	h_passingBits->Fill("ISOLEMHADDEPTH1",1);
      if((eleCutCode&heep::CutCodes::ISOLHADDEPTH2)==0x0)
	h_passingBits->Fill("ISOLHADDEPTH2",1);
      if((eleCutCode&heep::CutCodes::ISOLPTTRKS)==0x0)
	h_passingBits->Fill("ISOLPTTRKS",1);
      if((eleCutCode&heep::CutCodes::ECALDRIVEN)==0x0)
	h_passingBits->Fill("ECALDRIVEN",1);
      
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
HEEPElectronTurnon::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HEEPElectronTurnon::endJob() 
{
  h_passingBits->Scale(1.0/electronsAnalyzed);
}

// ------------ method called when starting to processes a run  ------------
void 
HEEPElectronTurnon::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HEEPElectronTurnon::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HEEPElectronTurnon::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HEEPElectronTurnon::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEEPElectronTurnon::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEEPElectronTurnon);
