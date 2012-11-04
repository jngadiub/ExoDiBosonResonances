#ifndef DIOBJECTKINFITFACTORY_H_
#define DIOBJECTKINFITFACTORY_H_
 
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "DataFormats/Common/interface/View.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "ExoDiBosonResonances/EDBRCommon/interface/JetKinFitterConst.h"

#include "TMath.h"

#include <algorithm>
#include <set>
#include <utility>



namespace cmg{
  
  template< typename T, typename U >
  class DiObjectKinFitFactory : public cmg::Factory< cmg::DiObject<T,U> >, public cmg::SettingTool<std::pair<T,U> ,cmg::DiObject<T,U> >{
  public:
    
    DiObjectKinFitFactory(const edm::ParameterSet& ps):
      inputLabel_(ps.getParameter<edm::InputTag>("inputs")),
      myKinFit_(ps.getParameter<edm::ParameterSet>("kinFit").getParameter<double>("mass"),ps.getParameter<edm::ParameterSet>("kinFit").getParameter<double>("merr")){
    }
    
    //need to override from Factory to insert "typename"
    typedef typename cmg::Factory<typename cmg::DiObject<T,U> >::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    
    ///Set kinematics to result of kinematic fit
    virtual void set(const cmg::DiObject<T,U>& pair, cmg::DiObject<T,U>* const obj ) const;
    
  private:
    const edm::InputTag inputLabel_;
    const JetKinFitterConst myKinFit_;    
  };

}

template< typename T, typename U >
typename cmg::DiObjectKinFitFactory<T,U>::event_ptr cmg::DiObjectKinFitFactory<T,U>::create(const edm::Event& iEvent, const edm::EventSetup&){
  
    typedef typename std::vector<typename cmg::DiObject<T,U> > collection;
    typedef edm::View<cmg::DiObject<T,U> > inCollection;
    
    edm::Handle<inCollection> inCands;
    iEvent.getByLabel(inputLabel_,inCands);
    
    typename cmg::DiObjectKinFitFactory<T,U>::event_ptr result(new collection);
    for(typename inCollection::const_iterator it = inCands->begin(); it != inCands->end(); ++it){
      cmg::DiObject<T, U> cmgTmp(*it);	    
      cmg::DiObjectKinFitFactory<T, U>::set(*it,&cmgTmp);	      
      result->push_back(cmgTmp);      
    }    
    return result;
}

template<typename T, typename U>
void cmg::DiObjectKinFitFactory<T, U>::set(const cmg::DiObject<T,U>& pair, cmg::DiObject<T,U>* const obj) const{

  //kinematic fit
  TLorentzVector j1corr, j2corr;
  j1corr.SetPtEtaPhiE(pair.leg1().pt(),pair.leg1().eta(),pair.leg1().phi(),pair.leg1().energy());
  j2corr.SetPtEtaPhiE(pair.leg2().pt(),pair.leg2().eta(),pair.leg2().phi(),pair.leg2().energy());
  int kinfitstatus=myKinFit_.getCorrJets(j1corr,j2corr);
  if(kinfitstatus!=0){//kinematic fit failed, set to zero for easy sorting out later ()
    //std::cout<<"Jet KinFit failed (status="<<kinfitstatus<<"), set to zero"<<std::endl;
    j1corr.SetXYZM(0,pair.leg1().py()/1000.,0,0); //keep a small component that jets remain unique
    j2corr.SetXYZM(0,pair.leg2().py()/1000.,0,0); 
  }

  obj->leg1_.setP4(reco::Candidate::PolarLorentzVector(j1corr.Pt(),j1corr.Eta(),j1corr.Phi(),j1corr.M() ) );
  obj->leg2_.setP4(reco::Candidate::PolarLorentzVector(j2corr.Pt(),j2corr.Eta(),j2corr.Phi(),j2corr.M() ) );

  TLorentzVector newComposite=j1corr + j2corr;
  obj->setP4( reco::Candidate::PolarLorentzVector(newComposite.Pt(),newComposite.Eta(),newComposite.Phi(),newComposite.M() ));
}



#endif /*DIOBJECTFACTORY_H_*/
