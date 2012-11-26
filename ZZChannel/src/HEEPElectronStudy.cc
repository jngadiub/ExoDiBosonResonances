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

class HEEPElectronStudy : public edm::EDAnalyzer {
   public:
      explicit HEEPElectronStudy(const edm::ParameterSet&);
      ~HEEPElectronStudy();

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
HEEPElectronStudy::HEEPElectronStudy(const edm::ParameterSet& iConfig)
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


HEEPElectronStudy::~HEEPElectronStudy()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HEEPElectronStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
  bool passed1 = (ele1CutCode==0x0);
  bool passed2 = (ele2CutCode==0x0);
  
  if(ele1.pt() > ele2.pt()) {
    h_efficiency_leadingEle->Fill(passed1,ele1.et());
    h_efficiency_secondEle->Fill(passed2,ele2.et());
  }
  else {
   h_efficiency_leadingEle->Fill(passed2,ele2.et());
   h_efficiency_secondEle->Fill(passed1,ele1.et());
  }
 
  // Check all electrons
  for(size_t eleNr = 0; eleNr != wellRecoEles.size(); ++eleNr) {
    
    size_t matchedElectron = wellRecoEles[eleNr];
    const pat::Electron& ele = eles[matchedElectron];
    
    // Check that the electron is in the right region
    // (for the time being, barrel only)
    double et = ele.et();
    //double pt = ele.closestCtfTrackRef()->pt();
    //double eta = ele.eta();
    //printf("Electron has pt = %g, eta = %g\n",pt, eta);
    double inBarrel = ele.isEB();
    if(!inBarrel) continue;
    
    ++electronsAnalyzed;
    
    // Fill the efficiency. Notice that the HEEPId bit
    // is defined with NO isolation cuts.
    int eleCutCode = ele.userInt("HEEPId");
    bool passed = (eleCutCode==0x0);
    h_efficiency->Fill(passed,et);
    h_efficiency_coarse->Fill(passed,et);
    
    // Let's find out WHY this didn't pass
    
    h_passingBits->Fill("TOTAL",1);
    
    if((eleCutCode&heep::CutCodes::ET)==0x0)
      h_passingBits->Fill("ET",1);
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
    
    // This one is not used right now...
    //if((eleCutCode&heep::CutCodes::ISOLHADDEPTH2)==0x0)
    //h_passingBits->Fill("ISOLHADDEPTH2",1);
    
    /// *** This is what we would be doing if we were using the regular isolation 
    //double sumCaloEt = ele.dr03EcalRecHitSumEt()+ele.dr03HcalDepth1TowerSumEt();
    /// ***But let's use the modified isolation
    
    double sumCaloEt = ele.userIsolation(pat::User2Iso)+ele.userIsolation(pat::User3Iso);
    bool isolEmHadD1 = false;
    //printf("sumCaloEt new = %g\n",sumCaloEt);
    if(sumCaloEt < (2.0 + 0.03*et + 0.28*rho)) {
      h_passingBits->Fill("ISOLEMHADDEPTH1",1);
      isolEmHadD1 = true;
    }
    h_caloIsol->Fill(sumCaloEt);
    //
    sumCaloEt = ele.dr03EcalRecHitSumEt()+ele.dr03HcalDepth1TowerSumEt();
    //printf("sumCaloEt old = %g\n",sumCaloEt);
    if(sumCaloEt < (2.0 + 0.03*et + 0.28*rho)) 
      h_passingBits->Fill("ISOLEMHADDEPTH1_ORIG",1);
    h_caloIsolOrig->Fill(sumCaloEt);
      
   // double sumTrackPt = ele.dr03TkSumPt()
    double sumTrackPt = ele.userIsolation(pat::User1Iso);
    bool isolPtTracks = false;
    //printf("sumTrackPt new = %g\n",sumTrackPt);
    if(sumTrackPt < 5.0) {
      h_passingBits->Fill("ISOLPTTRKS",1);
      isolPtTracks = true;
    }
    h_trackIsol->Fill(sumTrackPt);
    sumTrackPt = ele.dr03TkSumPt();
    //printf("sumTrackPt old = %g\n",sumTrackPt);
    if(sumTrackPt < 5.0)
      h_passingBits->Fill("ISOLPTTRKS_ORIG",1);
    h_trackIsolOrig->Fill(sumTrackPt);
    
    //printf("***********************\n");
    
    if((eleCutCode&heep::CutCodes::NRMISSHITS)==0x0)
      h_passingBits->Fill("NRMISSHITS",1);
    if((eleCutCode&heep::CutCodes::DXY)==0x0)
      h_passingBits->Fill("DXY",1);
    if((eleCutCode&heep::CutCodes::ECALDRIVEN)==0x0)
      h_passingBits->Fill("ECALDRIVEN",1);
    
    h_efficiency_withIsol->Fill(passed&&isolEmHadD1&&isolPtTracks,et);
    h_efficiency_withIsol_coarse->Fill(passed&&isolEmHadD1&&isolPtTracks,et);
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
HEEPElectronStudy::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HEEPElectronStudy::endJob() 
{
  h_passingBits->Scale(1.0/electronsAnalyzed);
}

// ------------ method called when starting to processes a run  ------------
void 
HEEPElectronStudy::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HEEPElectronStudy::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HEEPElectronStudy::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HEEPElectronStudy::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEEPElectronStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEEPElectronStudy);
