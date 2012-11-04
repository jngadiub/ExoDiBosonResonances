#ifndef cmgCleaner_h
#define cmgCleaner_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "ExoDiBosonResonances/EDBRCommon/interface/OverlapTest.h"

namespace cmg {
  
  template<class objectType>
  class cmgCleaner : public edm::EDProducer {
  public:
    explicit cmgCleaner(const edm::ParameterSet & iConfig);
    virtual ~cmgCleaner() {}
    
    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
  private:
    typedef StringCutObjectSelector<objectType> Selector;
    
    edm::InputTag src_;
    bool doPreselection_, doFinalCut_;
    Selector preselectionCut_;
    Selector finalCut_;
    
    typedef cmg::helper::OverlapTest OverlapTest;
    // make a list of overlap tests (ptr_vector instead of std::vector because they're polymorphic)
    std::vector<OverlapTest> overlapTests_;
  };
  
} // namespace


template <class objType>
cmg::cmgCleaner<objType>::cmgCleaner(const edm::ParameterSet & iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    preselectionCut_(iConfig.getParameter<std::string>("preselection")),
    finalCut_(iConfig.getParameter<std::string>("finalCut"))
{
    // pick parameter set for overlaps
    edm::ParameterSet overlapPSet = iConfig.getParameter<edm::ParameterSet>("checkOverlaps");
    // get all the names of the tests (all nested PSets in this PSet)
    std::vector<std::string> overlapNames = overlapPSet.getParameterNamesForType<edm::ParameterSet>();
    // loop on them
    for (std::vector<std::string>::const_iterator itn = overlapNames.begin(); itn != overlapNames.end(); ++itn) {
      // retrieve configuration
      edm::ParameterSet cfg = overlapPSet.getParameter<edm::ParameterSet>(*itn);
      // skip empty parameter sets
      if (cfg.empty()) continue;
      // create the appropriate OverlapTest
      overlapTests_.push_back(cmg::helper::OverlapTest(*itn, cfg));
    }


    produces<std::vector<objType> >();
}




template <class objType>
void
cmg::cmgCleaner<objType>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  // Read the input. We use edm::View<> in case the input happes to be something different than a std::vector<>
  edm::Handle<edm::View<objType> > candidates;
  iEvent.getByLabel(src_, candidates);

  // Prepare a collection for the output
  std::auto_ptr< std::vector<objType> > output(new std::vector<objType>());

  // initialize the overlap tests
  for (std::vector<OverlapTest>::iterator itov = overlapTests_.begin(), edov = overlapTests_.end(); itov != edov; ++itov) {
    itov->readInput(iEvent,iSetup);
  }

  for (typename edm::View<objType>::const_iterator it = candidates->begin(), ed = candidates->end(); it != ed; ++it) {
      // Apply a preselection to the inputs and copy them in the output
      if (!preselectionCut_(*it)) continue;

      // Add it to the list and take a reference to it, so it can be modified (e.g. to set the overlaps)
      // If at some point I'll decide to drop this item, I'll use pop_back to remove it
      output->push_back(*it);
      objType &obj = output->back();

      // Look for overlaps
      bool badForOverlap = false;
      for (std::vector<OverlapTest>::iterator itov = overlapTests_.begin(), edov = overlapTests_.end(); itov != edov; ++itov) {
        bool hasOverlap = itov->fillOverlapsForItem(obj);
        if (hasOverlap && itov->requireNoOverlaps()) {
            badForOverlap = true; // mark for discarding
            break; // no point in checking the others, as this item will be discarded
        }
	obj.addSelection("cuts_"+itov->name(),hasOverlap);
      }
      if (badForOverlap) { output->pop_back(); continue; }

      // Apply one final selection cut
      if (!finalCut_(obj)) output->pop_back();
  }

  iEvent.put(output);
}


#endif
