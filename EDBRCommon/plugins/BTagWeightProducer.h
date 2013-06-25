#ifndef ExoDiBosonResonances_EDBRCommon_BTagWeightProducer_h
#define ExoDiBosonResonances_EDBRCommon_BTagWeightProducer_h
//#include <Riostream.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

template <class restype>
class BTagWeightProducer : public edm::EDProducer {
	public:
		explicit BTagWeightProducer(const edm::ParameterSet& iConfig):
		src_(iConfig.getParameter<edm::InputTag>("src"))
		{

			produces<std::vector<restype> >();
		}

		~BTagWeightProducer(){}

	private:
		virtual void produce(edm::Event&, const edm::EventSetup&);
		edm::InputTag src_;
};

template <class restype>
void BTagWeightProducer<restype>::produce(edm::Event& iEvent, const edm::EventSetup&) {

	edm::Handle<edm::View<restype> > edbrcandidates;
	iEvent.getByLabel(src_, edbrcandidates);

	// prepare room for output
	std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());

	for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
		restype newCand(*edbrIt);
		double weight=1.;
		if(!iEvent.isRealData()){ // only look for MC

		}//end if is realData

		newCand.addUserFloat("BTagWeight",weight);
		outEDBR.push_back(newCand);
	}

	std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
	iEvent.put(out);
}


#endif
