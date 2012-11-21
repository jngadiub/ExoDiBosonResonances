#ifndef ExoDiBosonResonances_EDBRCommon_PUWeightProducer_h
#define ExoDiBosonResonances_EDBRCommon_PUWeightProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
// #include "PhysicsTools/Utilities/interface/Lumi3DReWeighting.h"

class PUWeightProducer : public edm::EDProducer {
public:
  explicit PUWeightProducer(const edm::ParameterSet&);
  ~PUWeightProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  std::string   filenameData_;
  std::string   hnameData_;
  std::string   filenameMC_;
  std::string   hnameMC_;
  edm::LumiReWeighting *weights_;
};


#endif
