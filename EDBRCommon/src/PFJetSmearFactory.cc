#include "ExoDiBosonResonances/EDBRCommon/interface/PFJetSmearFactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include <iostream>

using namespace std;


cmg::PFJetSmearFactory::event_ptr cmg::PFJetSmearFactory::create(const edm::Event& iEvent, 
								 const edm::EventSetup& iES) {
  
  cmg::PFJetSmearFactory::event_ptr result =PFJetFactory_.create(iEvent,iES);
  JetCorrectionUncertainty *jecUnc=0;  
  if(applyScale_&&jecUnc==0){//this is deprecated !
    if(!applyScaleDB_){
      jecUnc=new JetCorrectionUncertainty(scaleFile_.c_str());
    } 
    else{
      edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
      iES.get<JetCorrectionsRecord>().get("AK7PFchs",JetCorParColl); 
      JetCorrectorParameters const & JetCorPar = (*JetCorParColl)["Uncertainty"];
      jecUnc = new JetCorrectionUncertainty(JetCorPar);
    }
  }//end if applyScale and 1st event
  
  JetResolution *ptResol= 0;
  const bool  doGaussianSmear = true;  
  if(applyResolution_&&ptResol==0)ptResol=new JetResolution(resolutionFile_, doGaussianSmear);
  
  for(cmg::PFJetFactory::collection::iterator mi = result->begin();
      mi != result->end(); ++mi){
    
    
    if(applyResolution_){
    
      float ini_eta = mi->eta() ;
      float ini_pt = mi->pt();  
      float jetpt = ini_pt;
      float jetMass = mi->mass();
      if(ini_pt > 10){ // no reliable resolution numbers that low
	//std::cout << "eta/pt " << ini_eta <<" " << ini_pt << std::endl;
	TF1* fPtResol = ptResol->resolutionEtaPt(ini_eta,ini_pt);
	fPtResol->Print("v");
	float rndm = fPtResol->GetRandom();
	float etafactor = 0;
	if(fabs(ini_eta) <1.1)      etafactor = 0.04;
	else if(fabs(ini_eta) <1.7) etafactor = 0.02;
	else if(fabs(ini_eta) <2.3) etafactor = 0.09;
	else                       etafactor = 0.04;
	if(resolutionOverride_ > 0.) etafactor = resolutionOverride_;
	float jesSmearFactor = fabs(1.- (1.-rndm)*etafactor) ;
	//std::cout << rndm  << " "<<smearfactor << std::endl;
	jetpt = jesSmearFactor*ini_pt;
	jetMass = jesSmearFactor*jetMass;
	const math::PtEtaPhiMLorentzVector jetp4(jetpt, mi->eta(), mi->phi(), jetMass);//mi->mass());
	mi->setP4(jetp4);
	delete fPtResol;
      }
    }//end if applyRes

   
  if(applyScale_){   
    double jetPt = mi->pt();
    double jetEta = mi->eta();
    double jetMass = mi->mass();
    jecUnc->setJetEta(jetEta);
    jecUnc->setJetPt(jetPt);
    double unc = jecUnc->getUncertainty(true);
    float jesCorrFactor = unc*nSigmaScale_ ;
    //std::cout<<"Smearing factor = "<<smearfactor<<std::endl;
    jetPt   = (1+jesCorrFactor)*jetPt;
    jetMass = (1+jesCorrFactor)*jetMass;
    const math::PtEtaPhiMLorentzVector jetp4 (jetPt, mi->eta(), mi->phi(), jetMass);//mi->mass());
    mi->setP4(jetp4);  
  }//end if applyScale
  
 }//end loop on PFJet collection

  delete ptResol;
  delete jecUnc;//too bad we cannot declare it as data member...
  return result;
}
