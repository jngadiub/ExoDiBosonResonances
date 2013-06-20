#include "ExoDiBosonResonances/EDBRCommon/interface/PFJetSmearFactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "TRandom3.h"

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
  
  for(cmg::PFJetFactory::collection::iterator mi = result->begin();
      mi != result->end(); ++mi){
    
    if(applyScale_){   
      double jetPt = mi->pt();
      double jetEta = mi->eta();
      double jetMass = mi->mass();
      jecUnc->setJetEta(jetEta);
      jecUnc->setJetPt(jetPt);
      double unc = jecUnc->getUncertainty(true);
      float jesCorrFactor = unc*nSigmaScale_ ;
      jetPt   = (1+jesCorrFactor)*jetPt;
      jetMass = (1+jesCorrFactor)*jetMass;
      const math::PtEtaPhiMLorentzVector jetp4 (jetPt, mi->eta(), mi->phi(), jetMass);
      mi->setP4(jetp4);  
    }//end if applyScale
    
    if(applyResolution_){
      float jesSmearFactor = JetResolutionPFJet(iEvent, mi);  
      double jetPt = mi->pt();
      double jetMass = mi-> mass();
      jetPt   = jesSmearFactor*jetPt;
      jetMass = jesSmearFactor*jetMass;
      const math::PtEtaPhiMLorentzVector jetp4 (jetPt, mi->eta(), mi->phi(), jetMass);
      mi->setP4(jetp4);  
    }//end if applyResolution
    
  }//end loop on PFJet collection
  
  delete jecUnc;//too bad we cannot declare it as data member...
  return result;
}


float cmg::PFJetSmearFactory::JetResolutionPFJet(const edm::Event& iEvent, cmg::PFJetFactory::collection::iterator output){
  bool genjet_match = false;
  float genpt = -10;
  float jetpt = output->pt();
  double factor = 1;
  if(fabs(output->eta())<0.5)      factor = 1.052;
  else if(fabs(output->eta())<1.1) factor = 1.057;
  else if(fabs(output->eta())<1.7) factor = 1.096;
  else if(fabs(output->eta())<2.3) factor = 1.134;
  else                             factor = 1.288;

  edm::Handle<reco::GenJetCollection> GenjetCands;
  iEvent.getByLabel("ca8GenJetsNoNu",GenjetCands);

  double dR = 999;
  for(reco::GenJetCollection::const_iterator genmi = GenjetCands->begin(); genmi != GenjetCands->end(); ++genmi){
    if(deltaR(*output,*genmi) < dR  && deltaR(*output,*genmi) < 0.3){
      dR = deltaR(*output,*genmi);
      genjet_match = true;
      genpt = genmi->pt();
    }
  }

  if(genjet_match){
    jetpt = max(0.,genpt+factor*(jetpt-genpt));
  }
  else{
    double sigmaMC;
    if(fabs(output->eta()) < 0.5 )   sigmaMC = 0.05206; 
    else if(fabs(output->eta())<1.0) sigmaMC = 0.05403;
    else if(fabs(output->eta())<1.5) sigmaMC = 0.06231;
    else if(fabs(output->eta())<2.0) sigmaMC = 0.06452;
    else                             sigmaMC = 0.07672;
    TRandom3 r;
    r.SetSeed(0);
    double resol = sigmaMC;
    double sigma = resol * sqrt(factor*factor-1);
    double smear = r.Gaus(1,sigma);
    jetpt = jetpt*smear;
  }
  
  float smearing_factor = jetpt/output->pt();
  return smearing_factor;
}

