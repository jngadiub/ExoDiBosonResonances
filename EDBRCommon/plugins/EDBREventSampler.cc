#include "ExoDiBosonResonances/EDBRCommon/plugins/EDBREventSampler.h"


EDBREventSampler::EDBREventSampler(const edm::ParameterSet & iConfig){

  seed_ = iConfig.getParameter< int >("RandomGenSeed");
  sampling_= iConfig.getParameter< double >("SamplingFactor");

  myrand_=new TRandom3(seed_);
}

bool  EDBREventSampler::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){

  double newRndmNum= myrand_->Rndm();
  if(newRndmNum>sampling_)return false;
  
  return true;

}



#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EDBREventSampler);
