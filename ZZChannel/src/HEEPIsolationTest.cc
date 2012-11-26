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

class HEEPIsolationTest : public edm::EDAnalyzer {
   public:
      explicit HEEPIsolationTest(const edm::ParameterSet&);
      ~HEEPIsolationTest();

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
  //edm::InputTag eleRhoCorrLabel_; 
    
  TEfficiency* h_efficiency;
  TEfficiency* h_efficiency_coarse;
  TEfficiency* h_efficiency_withIsol;
  TEfficiency* h_efficiency_withIsol_coarse;
  TEfficiency* h_efficiency_leadingEle;
  TEfficiency* h_efficiency_secondEle;
  TH1F* h_passingBits;
  TH1F* h_trackIsol;
  TH1F* h_trackIsolOrig;
  TH1F* h_caloIsol;
  TH1F* h_caloIsolOrig;
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
HEEPIsolationTest::HEEPIsolationTest(const edm::ParameterSet& iConfig)
{
  eleLabel_=iConfig.getParameter<edm::InputTag>("eleLabel");

  edm::Service<TFileService> fs;
  h_efficiency                 = fs->make<TEfficiency>("efficiency",";Electron E_{T} (GeV); Efficiency",200,0,1000); 
  h_efficiency_coarse          = fs->make<TEfficiency>("efficiency_coarse",";Electron E_{T} (GeV); Efficiency",20,0,1000); 
  h_efficiency_withIsol        = fs->make<TEfficiency>("efficiency_withIsol",";Electron E_{T} (GeV); Efficiency",200,0,1000); 
  h_efficiency_withIsol_coarse = fs->make<TEfficiency>("efficiency_withIsol_coarse",";Electron E_{T} (GeV); Efficiency",20,0,1000); 
  h_efficiency_leadingEle      = fs->make<TEfficiency>("efficiency_leadingEle",";Electron E_{T} (GeV); Efficiency",20,0,1000); 
  h_efficiency_secondEle       = fs->make<TEfficiency>("efficiency_secondEle",";Electron E_{T} (GeV); Efficiency",20,0,1000); 
  h_passingBits = fs->make<TH1F>("passingBits","passingBits",16,0,16);
  h_trackIsol     = fs->make<TH1F>("trackIsol","trackIsol",250,0,50);
  h_trackIsolOrig = fs->make<TH1F>("trackIsolOrig","trackIsol",250,0,50);
  h_caloIsol      = fs->make<TH1F>("caloIsol","caloIsolOrig",100,0,100);
  h_caloIsolOrig  = fs->make<TH1F>("caloIsolOrig","caloIsolOrig",100,0,100);
  
  h_passingBits->SetBit(TH1::kCanRebin);
  h_passingBits->Sumw2();
  electronsAnalyzed = 0;
}


HEEPIsolationTest::~HEEPIsolationTest()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HEEPIsolationTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Get the electrons
  edm::Handle<edm::View<pat::Electron> > eleHandle;
  iEvent.getByLabel(eleLabel_,eleHandle);
  const edm::View<pat::Electron>& eles = *(eleHandle.product());
  
  // Get the gen electrons
  edm::Handle<edm::View<reco::Candidate> > genEleHandle;
  iEvent.getByLabel("genElectrons",genEleHandle);
  const edm::View<reco::Candidate>& genEles = *(genEleHandle.product());
 

  int numElectrons = eles.size();
  //printf("Found %i electrons\n",numElectrons);

  // Get rho 
  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(edm::InputTag("kt6PFJets","rho"),rhoHandle);
  double rho = *rhoHandle;
  
  
  // Check that electrons have matches
  int numOfWellRecoElectrons = 0;
  std::vector<size_t> wellRecoEles;
    
  for(size_t eleNr = 0; eleNr != eles.size(); ++eleNr) {
  
    const pat::Electron& ele = eles[eleNr];
    
    // Let's see if this guy has a match
    double minDeltaR = 10000;
    size_t matchedEle = 9999;
    for(size_t genEleNr = 0; genEleNr != genEles.size(); ++ genEleNr) {
      const reco::Candidate& genEle = genEles[genEleNr];
      double dR= deltaR(ele.eta(),ele.phi(),genEle.eta(),genEle.phi());
      if(dR < minDeltaR) {
	minDeltaR = dR;
	matchedEle = genEleNr;
      }   
    }
    
    if(minDeltaR < 0.05) {
      ++numOfWellRecoElectrons;
      wellRecoEles.push_back(matchedEle);
    }
  }
  
  // I want all electrons well reconstructed!
  if(numOfWellRecoElectrons < 2)
    return;
  
  // Lets's put those electrons in the proper order
  const pat::Electron& ele1 = eles[wellRecoEles[0]];
  const pat::Electron& ele2 = eles[wellRecoEles[1]];
  int ele1CutCode = ele1.userInt("HEEPId");
  int ele2CutCode = ele2.userInt("HEEPId");
  printf("ele1CutCode = %i\n",ele1CutCode);
  printf("ele2CutCode = %i\n",ele2CutCode);
  double userIsol0 = ele1.userIso(0);
  double userIsol1 = ele1.userIso(1);
  double userIsol2 = ele1.userIso(2);
  printf("First electron has et = %g, eta = %g, phi = %g, trackIsol %g, ECALIsol = %g, HCALIsol = %g\n",
	 ele1.et(), ele1.eta(),ele1.phi(),userIsol0,userIsol1,userIsol2);
  userIsol0 = ele2.userIso(0); 
  userIsol1 = ele2.userIso(1);
  userIsol2 = ele2.userIso(2);
  printf("Second electron has et = %g, eta = %g, phi = %g, trackIsol %g, ECALIsol = %g, HCALIsol = %g\n",
	 ele2.et(), ele2.eta(),ele2.phi(),userIsol0,userIsol1,userIsol2);

  
}  



// ------------ method called once each job just before starting event loop  ------------
void 
HEEPIsolationTest::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HEEPIsolationTest::endJob() 
{
  h_passingBits->Scale(1.0/electronsAnalyzed);
}

// ------------ method called when starting to processes a run  ------------
void 
HEEPIsolationTest::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HEEPIsolationTest::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HEEPIsolationTest::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HEEPIsolationTest::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEEPIsolationTest::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEEPIsolationTest);
