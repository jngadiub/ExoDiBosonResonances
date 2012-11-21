#ifndef ExoDiBosonResonances_EDBRCommon_EDBRTagger_h
#define ExoDiBosonResonances_EDBRCommon_EDBRTagger_h

#include <memory>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "CMGTools/Common/interface/Cut.h"

template <class restype> //templated according to the type of resonance
class EDBRTagger : public edm::EDProducer { 
public:
  typedef Cut< StringCutObjectSelector<restype> > CutType;
  typedef CutTree< CutType > CutTreeType;
  typedef CutTreeTester< CutTreeType, restype> CutTreeTesterType;

  explicit EDBRTagger(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    cuts_(iConfig.getParameter<edm::ParameterSet>("cuts")),
    basename_(iConfig.getParameter<std::string>("basename")),
    cutTree_("All Cuts")
  {
    produces<std::vector<restype> >();
    
    unpack( cuts_,basename_ , cutTree_);
    /// unpacks a parameterset to find all cuts, and fill them in cuts
    //CutTreePrinter<CutTreeType> printer;
    //cutTree_.visit( &printer );
  }
  
  virtual ~EDBRTagger() {}
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  
private:	
  edm::InputTag src_ ;
  edm::ParameterSet cuts_;
  std::string basename_;
  CutTreeType cutTree_;

  void unpack( const edm::ParameterSet& ps,
	       const std::string& baseName,
	       CutTreeType& cut) const;
  
};

template <class restype>
void EDBRTagger<restype>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
  // read input collection
  edm::Handle<edm::View<restype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);
    
  // prepare room for output
  std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());

  for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
    restype newCand(*edbrIt);
    CutTreeTesterType tester( & newCand );
    cutTree_.visit( &tester );
    outEDBR.push_back(newCand);


  }

  std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
  iEvent.put(out);
 
}

template<class restype>
void EDBRTagger<restype>::unpack( const edm::ParameterSet& ps,
				     const std::string& baseName,
				     EDBRTagger<restype>::CutTreeType& cutTree ) const {

  // all strings found are added as cuts.
  std::vector<std::string> cutStrings = ps.getParameterNamesForType<std::string>();

  //std::cout << "XXX " <<baseName <<" XXX" <<std::endl;

  for( unsigned i=0; i<cutStrings.size(); ++i) {
    std::string cutName = baseName;
    cutName += "_";
    cutName += cutStrings[i];
    std::string cutString = ps.getParameter< std::string >( cutStrings[i] );
    cutTree.addCut( CutType( cutName, cutString ) );
  }

  // now dealing with parameter sets included in the current parameter set
  std::vector<std::string> subPSets = ps.getParameterNamesForType<edm::ParameterSet>();
  for( unsigned i=0; i<subPSets.size(); ++i) {
    edm::ParameterSet subPSet = ps.getParameter< edm::ParameterSet >( subPSets[i] );
    std::string newBaseName = baseName;
    newBaseName += "_";
    newBaseName += subPSets[i];

    cutTree.addNode( CutTreeType( newBaseName )) ;

    unpack( subPSet, newBaseName, cutTree.lastNode() );
  }
}


#endif
