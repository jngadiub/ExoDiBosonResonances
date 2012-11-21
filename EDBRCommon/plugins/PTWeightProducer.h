#ifndef ExoDiBosonResonances_EDBRCommon_PTWeightProducer_h
#define ExoDiBosonResonances_EDBRCommon_PTWeightProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TGraph.h>

class PTWeightProducer : public edm::EDProducer {
public:
  explicit PTWeightProducer(const edm::ParameterSet&);
  ~PTWeightProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag genTag_;
  int           pdgId_;
  std::string   filename_;
  TGraph* corrfactors_;
    
};


#endif
