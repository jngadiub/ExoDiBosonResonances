#ifndef ExoDiBosonResonances_EDBRCommon_BestCandidateSelector_h
#define ExoDiBosonResonances_EDBRCommon_BestCandidateSelector_h

#include <TF1.h>

template<class restype>
class BestCandidateSelector : public edm::EDProducer {
public:
  explicit BestCandidateSelector(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    secondarySrc_(iConfig.getParameter<edm::InputTag>("secondary")),
    doBtag_(iConfig.getParameter<bool>("btagpriority")),
    priorityList_(iConfig.getParameter<std::vector<std::string> >("btagSelectionList"))
  {
    produces<std::vector<restype> >("primary"); // the actual filtered collection
    produces<std::vector<restype> >("secondary");// the corresponding fitted/non-fitted collection
  }

  virtual ~BestCandidateSelector() {}

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;

private:
  edm::InputTag src_ ;
  edm::InputTag secondarySrc_ ;

  bool doBtag_;
  std::vector<std::string> priorityList_;
};

template <class restype>
void BestCandidateSelector<restype>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<edm::View<restype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);

  edm::Handle<edm::View<restype> > secondarycandidates;
  iEvent.getByLabel(secondarySrc_, secondarycandidates);
  
  // prepare room for output
  std::vector<restype> outCands;   
  std::vector<restype> outSecondary;
  
  unsigned int bestindex=9999;
  float bestDiff =99999999;
  const float nomZmass=91.1876;
  bool found = false;

  std::vector<int>   bestByCategory; bestByCategory.reserve(priorityList_.size()) ;  bestByCategory.assign(priorityList_.size(),9999);
  std::vector<float> diffByCategory; diffByCategory.reserve(priorityList_.size()) ;  diffByCategory.assign(priorityList_.size(),99999999);
  
  
  for (unsigned int i=0 ; i<edbrcandidates->size() ; ++i ) {
    edm::RefToBase<restype>  edbrCand = edbrcandidates->refAt(i);
    //compute mass differences
    float leptonicMass = edbrCand->leg1().mass();
    float quarkMass = (edbrCand->leg2().leg1().sourcePtr()->get()->p4() + edbrCand->leg2().leg2().sourcePtr()->get()->p4()).M();
    float diff = (leptonicMass - nomZmass)*(leptonicMass - nomZmass) + (quarkMass - nomZmass)*(quarkMass - nomZmass);
    
    if(diff < bestDiff){//new best candidate found
      found = true;
      bestDiff = diff;
      bestindex = i;
    }      
    if(doBtag_){// look for best candidates separately by category
      for(unsigned int cat = 0 ; cat <  priorityList_.size() ; cat++){
	if(edbrCand->getSelection(priorityList_[cat])){
	  if(diff < diffByCategory[cat]){
	    diffByCategory[cat]= diff;
	    bestByCategory[cat]= i;
	  }
	} 
      }
    }
  }
  
  if(doBtag_){//get the best candidate of highest priority
    for(unsigned int cat = 0 ; cat <  priorityList_.size() ; cat++){
      if(bestByCategory[cat] < 1000){//we have a candidate in this category
	bestindex = bestByCategory[cat];
	break; //priority list is ordered => take the first one;
      } 
    }
  }

  if(found){// cuts passed! => save candidate
    restype newCand(*(edbrcandidates->refAt(bestindex)));
    outCands.push_back( newCand );
    // also save the corresponding other candidate (need to read other LD, QG is the same)
    restype newSecond(*(secondarycandidates->refAt(bestindex)));
    outSecondary.push_back( newSecond );
  }
  
  std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outCands));
  std::auto_ptr<std::vector<restype> > secondaryout(new std::vector<restype>(outSecondary));
  iEvent.put(out,"primary");
  iEvent.put(secondaryout,"secondary");

}
#endif
