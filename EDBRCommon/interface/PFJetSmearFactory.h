#ifndef _CMGTools_Common_PFJetSmearFactory_h_
#define _CMGTools_Common_PFJetSmearFactory_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"

#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include <iostream>
#include <memory>

namespace cmg{
  
  class PFJetSmearFactory : public Factory<cmg::PFJet>{
    
  public:
    PFJetSmearFactory(const edm::ParameterSet& ps):
      PFJetFactory_(ps.getParameter<edm::ParameterSet>("PFJetFactory")),
      applyResolution_(ps.getParameter<bool>("applyResolution")),
      applyScale_(ps.getParameter<bool>("applyScale")),
      applyScaleDB_(ps.getParameter<bool>("applyScaleFromDB")),
      scaleFile_(ps.getParameter<edm::FileInPath>("scaleFile").fullPath()),
      nSigmaScale_(ps.getParameter<double>("nSigmaScale"))
	{
	  if(applyScale_)      std::cout<<"For the Standard Jet you are applying the JES with sigma equal to: "<< nSigmaScale_ << std::endl;
	  if(applyResolution_) std::cout<<"For the Standard Jet you are applying the JER smearing"<< std::endl;
	  
	}
      float JetResolutionPFJet(const edm::Event& iEvent, cmg::PFJetFactory::collection::iterator output);
      
      
      virtual event_ptr create(const edm::Event&, const edm::EventSetup&) ;
      
  private:
      const edm::InputTag jetLabel_;
      PFJetFactory PFJetFactory_;
      
      const bool applyResolution_;
      const bool applyScale_, applyScaleDB_;
      const std::string scaleFile_;
      const double nSigmaScale_;
      
      //  JetCorrectionUncertainty *jecUnc_;  
      
  };
  
}


#endif 
