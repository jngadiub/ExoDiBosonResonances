#ifndef ExoDiBosonResonances_EDBRCommon_WeightAdder_h
#define ExoDiBosonResonances_EDBRCommon_WeightAdder_h

#include <memory>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


template <class restype>
class WeightAdder : public edm::EDProducer { 
public:
  explicit WeightAdder(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    weight_(iConfig.getParameter<edm::InputTag>("weight"))
  {
    produces<std::vector<restype> >();
  }
  
  virtual ~WeightAdder() {}
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  
private:	
  edm::InputTag src_ ;
  edm::InputTag weight_ ;
};

template <class restype>
void WeightAdder<restype>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // read input collection
  edm::Handle<edm::View<restype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);
  
  edm::Handle<double > weightHandle;
  iEvent.getByLabel(weight_, weightHandle);
  
  // prepare room for output
  std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());

  for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
    restype newCand(*edbrIt);
    newCand.addUserFloat(weight_.label(),*weightHandle);
    outEDBR.push_back(newCand);
  }

  std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
  iEvent.put(out);
 
}

#endif
