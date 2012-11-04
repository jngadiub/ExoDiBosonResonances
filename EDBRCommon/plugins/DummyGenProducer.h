#ifndef ExoDiBosonResonances_EDBRCommon_DummyGenProducer_h
#define ExoDiBosonResonances_EDBRCommon_DummyGenProducer_h

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class DummyGenProducer : public edm::EDProducer {
public:
  explicit DummyGenProducer(const edm::ParameterSet& iConfig){
    produces<std::vector<reco::GenParticle> >();
  }

  virtual ~DummyGenProducer() {}

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup){
    std::auto_ptr<std::vector<reco::GenParticle> > out(new std::vector<reco::GenParticle>());
    iEvent.put(out);
  }
};
#endif
