#ifndef ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h
#define ExoDiBosonResonances_EDBRCommon_KineVarsAdder_h

#include <TF1.h>
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/PatCandidates/interface/MET.h"

template<class edbrtype>
class KineVarsAdder : public edm::EDProducer {
public:
  explicit KineVarsAdder(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    nokinfitSrc_(iConfig.getParameter<edm::InputTag>("noKinFitSrc"))
    //    METSrc_(iConfig.getParameter<edm::InputTag>("METsrc"))
  {
    produces<std::vector<edbrtype> >(); // the actual filtered collection
    if( nokinfitSrc_.encode().size()){//check that the Input Tag is not empty
      isDoubleJet_=true;
    }
    else{//enpty InputTag
      isDoubleJet_=false;
    }
  }

  virtual ~KineVarsAdder() {}

  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) ;

private:
  edm::InputTag src_ ;
  edm::InputTag nokinfitSrc_ ;

  //  edm::InputTag METSrc_;
  //  StringCutObjectSelector<pat::MET> METcut_;
  bool isDoubleJet_;
};

template <class edbrtype>
void KineVarsAdder<edbrtype>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

  edm::Handle<edm::View<edbrtype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);

  edm::Handle<edm::View<edbrtype> > nokinfitcandidates;
  if(isDoubleJet_){
 
    iEvent.getByLabel(nokinfitSrc_, nokinfitcandidates);
    unsigned int ncands= edbrcandidates->size();
    unsigned int ncandsNKF= nokinfitcandidates->size();
    if(ncands!=ncandsNKF){ //these really need to be the same
      throw cms::Exception("Mismatched collection size") <<
	"Event in DoubleJet Path has mismatched collections with and without KinematicFit. #cands WITH kin-fit: "<<
	ncands<<"  #cands NO kin-fit: "<<ncandsNKF<<std::endl;

    }
  }

 
  // prepare room for output
  std::vector<edbrtype> outCands;   
 	

  for (unsigned int i=0 ; i<edbrcandidates->size() ; ++i ) {
    edm::RefToBase<edbrtype>  edbrCand = edbrcandidates->refAt(i);

    edbrtype newCand(*edbrCand);

    float nXJets=1.0;

    // also save the corresponding other candidate (need to read other LD, QG is the same)
    float mzzNKF=-999.0,ptNKF=-999.0,etaNKF=-999.0,phiNKF=-999.0;
    float mjjNKF=-999.0,ptjjNKF=-999.0,etajjNKF=-999.0,phijjNKF=-999.0;

    if(isDoubleJet_){
      nXJets=2.0; 

      edm::RefToBase<edbrtype> nokinfitCand=nokinfitcandidates->refAt(i);
      mzzNKF  = nokinfitCand->mass();
      ptNKF  = nokinfitCand->pt();
      etaNKF  = nokinfitCand->eta();
      phiNKF  = nokinfitCand->phi();
      mjjNKF  = nokinfitCand->leg2().mass();
      ptjjNKF  = nokinfitCand->leg2().pt();
      etajjNKF  = nokinfitCand->leg2().eta();
      phijjNKF  = nokinfitCand->leg2().phi();
    }

    newCand.addUserFloat("nXJets",nXJets );
    newCand.addUserFloat("nokinfitMZZ",mzzNKF );
    newCand.addUserFloat("nokinfitPT",ptNKF);
    newCand.addUserFloat("nokinfitEta",etaNKF);
    newCand.addUserFloat("nokinfitPhi",phiNKF);
    newCand.addUserFloat("nokinfitMJJ",mjjNKF);
    newCand.addUserFloat("nokinfitPTJJ",ptjjNKF);
    newCand.addUserFloat("nokinfitEtaJJ",etajjNKF);
    newCand.addUserFloat("nokinfitPhiJJ",phijjNKF);


    //LD calculation is de-activated for the moment...
    //adding the value of the LD discriminant:
    float LD=-999.0, HelPsig=-9.0, HelPbkg=9999.0;
    newCand.addUserFloat("LD", LD );
    newCand.addUserFloat("HelPsig", HelPsig );
    newCand.addUserFloat("HelPbkg", HelPbkg );


    outCands.push_back( newCand );
  }

  
  std::auto_ptr<std::vector<edbrtype> > out(new std::vector<edbrtype>(outCands));

  iEvent.put(out);


}
#endif
