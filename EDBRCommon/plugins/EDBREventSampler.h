#ifndef __EDBREventSampler__
#define __EDBREventSampler__

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "TRandom3.h"


class EDBREventSampler : public edm::EDFilter {

 public:

  explicit EDBREventSampler(const edm::ParameterSet & iConfig);
  virtual ~EDBREventSampler() { delete myrand_; }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

 private:

  double seed_;
  TRandom3 *myrand_;

  double sampling_;



};


#endif
