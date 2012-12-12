#ifndef EDBRCANDIDATEFACTORY_H_
#define EDBRCANDIDATEFACTORY_H_

#include "AnalysisDataFormats/ExoDiBosonResonances/interface/EDBRCandidate.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/VJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "CommonTools/CandUtils/interface/CenterOfMassBooster.h"
#include "CommonTools/CandUtils/interface/cloneDecayTree.h"
#include <cmath>


namespace cmg{

  template< typename T, typename U > class EDBRCandidateFactory : public cmg::Factory< cmg::EDBRCandidate<T,U> >,  public cmg::SettingTool<cmg::DiObject<T,U> ,cmg::EDBRCandidate<T,U> > {

  public:

    EDBRCandidateFactory(const edm::ParameterSet& ps):
      inLabel_(ps.getParameter<edm::InputTag>("inputs")),
      vbftag_(ps.getParameter<edm::InputTag>("vbftag")),
      vbfoverlapveto_(ps.getParameter<std::string>("overlapcut")){
    }

    //need to override from Factory to insert "typename"
    typedef typename cmg::Factory<typename cmg::EDBRCandidate<T,U> >::event_ptr event_ptr;
    typedef cmg::Factory<cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> >::event_ptr EEJ_event_ptr;
    typedef cmg::Factory<cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> >::event_ptr MMJ_event_ptr;
    virtual event_ptr create(const edm::Event& iEvent, const edm::EventSetup& iSetup);


    // virtual event_ptr2 create(const edm::Event& iEvent, const edm::EventSetup& iSetup);

    /*
     typedef typename cmg::Factory<typename cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> >::event_ptr event_ptr2;
    //    template<> 
     typename cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::event_ptr create(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
      std::cout<<"Blah"<<std::endl;      
      typename cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::event_ptr result(new collection);
 
      return result;
    }//end create (2)
    */

    ///Set angular variables etc
    virtual void set(const cmg::DiObject<T,U>& pair, cmg::EDBRCandidate<T,U>* const obj, bool isMergedJet=false) const;
    // virtual void set(const cmg::DiObject<cmg::DiElectron,cmg::VJet>& in, cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet>* const obj) const;  
  private:       
    const edm::InputTag inLabel_;
    const edm::InputTag vbftag_;
    const StringCutObjectSelector<cmg::EDBRCandidate<T,U> > vbfoverlapveto_;
  };




  
template< typename T, typename U > typename cmg::EDBRCandidateFactory<T,U>::event_ptr cmg::EDBRCandidateFactory<T,U>::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  
  typedef  std::vector< cmg::EDBRCandidate<T,U> > collection;
  typedef  edm::View< cmg::DiObject<T,U> > diOcollection;
  
  edm::Handle<diOcollection> diCands;
  iEvent.getByLabel(inLabel_,diCands);
    
  edm::Handle<std::vector<cmg::DiPFJet> > vbfPairs;
  iEvent.getByLabel(vbftag_,vbfPairs);

  
  typename cmg::EDBRCandidateFactory<T,U>::event_ptr result(new collection);
  for(typename diOcollection::const_iterator it = diCands->begin(); it != diCands->end(); ++it){
    // first create the  "pure" EDBR candidate without vbf tags
    cmg::EDBRCandidate<T, U> cmgTmp(*it);
    cmg::EDBRCandidateFactory<T, U>::set(cmgTmp,&cmgTmp);
    result->push_back(cmgTmp);
    // loop over vbf tagging pairs, but only add non-overlapping candidates
    if( vbfPairs.isValid()) {
      for(unsigned int i = 0 ; i < vbfPairs.product()->size() ; i++){
	cmg::EDBRCandidate<T, U> cmgTmpVBF(cmgTmp);
	edm::Ptr<cmg::DiPFJet > tmpptr(vbfPairs,i);
	cmgTmpVBF.vbfptr_=tmpptr;
	if(vbfoverlapveto_(cmgTmpVBF))
	  result->push_back(cmgTmpVBF);
	  }
    }
  }
  return result;
}


template<>  cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::EEJ_event_ptr cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  // std::cout<<"USING SINGLE JET cmg::EDBRCandidateFactory::create (EEJ)"<<std::endl;

  typedef  std::vector< cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> > collection;
  typedef  edm::View< cmg::DiObject<cmg::DiElectron,cmg::VJet> > diOcollection;
  
  edm::Handle<diOcollection> diCands;
  iEvent.getByLabel(inLabel_,diCands);
    
  edm::Handle<std::vector<cmg::DiPFJet> > vbfPairs;
  iEvent.getByLabel(vbftag_,vbfPairs);

  
  typename cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::EEJ_event_ptr result(new collection);
  for(typename diOcollection::const_iterator it = diCands->begin(); it != diCands->end(); ++it){
    // first create the  "pure" EDBR candidate without vbf tags
    cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> cmgTmp(*it);
    cmg::EDBRCandidateFactory<cmg::DiElectron,cmg::VJet>::set(cmgTmp,&cmgTmp,true);
    result->push_back(cmgTmp);
    // loop over vbf tagging pairs, but only add non-overlapping candidates
    if( vbfPairs.isValid()) {
      for(unsigned int i = 0 ; i < vbfPairs.product()->size() ; i++){
	cmg::EDBRCandidate<cmg::DiElectron,cmg::VJet> cmgTmpVBF(cmgTmp);
	edm::Ptr<cmg::DiPFJet > tmpptr(vbfPairs,i);
	cmgTmpVBF.vbfptr_=tmpptr;
	if(vbfoverlapveto_(cmgTmpVBF))
	  result->push_back(cmgTmpVBF);
	  }
    }
  }
  return result;
}


template<>  cmg::EDBRCandidateFactory<cmg::DiMuon,cmg::VJet>::MMJ_event_ptr cmg::EDBRCandidateFactory<cmg::DiMuon,cmg::VJet>::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  // std::cout<<"USING SINGLE JET cmg::EDBRCandidateFactory::create (MMJ)"<<std::endl;

  typedef  std::vector< cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> > collection;
  typedef  edm::View< cmg::DiObject<cmg::DiMuon,cmg::VJet> > diOcollection;
  
  edm::Handle<diOcollection> diCands;
  iEvent.getByLabel(inLabel_,diCands);
    
  edm::Handle<std::vector<cmg::DiPFJet> > vbfPairs;
  iEvent.getByLabel(vbftag_,vbfPairs);

  
  typename cmg::EDBRCandidateFactory<cmg::DiMuon,cmg::VJet>::MMJ_event_ptr result(new collection);
  for(typename diOcollection::const_iterator it = diCands->begin(); it != diCands->end(); ++it){
    // first create the  "pure" EDBR candidate without vbf tags
    cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> cmgTmp(*it);
    cmg::EDBRCandidateFactory<cmg::DiMuon,cmg::VJet>::set(cmgTmp,&cmgTmp,true);
    result->push_back(cmgTmp);
    // loop over vbf tagging pairs, but only add non-overlapping candidates
    if( vbfPairs.isValid()) {
      for(unsigned int i = 0 ; i < vbfPairs.product()->size() ; i++){
	cmg::EDBRCandidate<cmg::DiMuon,cmg::VJet> cmgTmpVBF(cmgTmp);
	edm::Ptr<cmg::DiPFJet > tmpptr(vbfPairs,i);
	cmgTmpVBF.vbfptr_=tmpptr;
	if(vbfoverlapveto_(cmgTmpVBF))
	  result->push_back(cmgTmpVBF);
	  }
    }
  }
  return result;
}


template<typename T, typename U> void cmg::EDBRCandidateFactory<T, U>::set(const cmg::DiObject<T,U>& in, cmg::EDBRCandidate<T,U>* const obj, bool isMergedJet) const{
  double costhetastar=-77.0;
  double helphi=-77.0;
  double helphiZll=-77.0;
  double helphiZjj=-77.0;
  double helcosthetaZll=-77.0;
  double helcosthetaZjj=-77.0;
  double phistarZll=+33.0, phistarZjj=-33.0;
  
  // create boosters to various objects
  CenterOfMassBooster boostX( in );
  CenterOfMassBooster boostZll( in.leg1() );
  CenterOfMassBooster boostZjj( in.leg2() );
  
  //clone Zs so that they are not const any more, then boots the EDBR frame
  std::auto_ptr<reco::Candidate> ZllboostedX =cloneDecayTree( in.leg1() );
  std::auto_ptr<reco::Candidate> ZjjboostedX =cloneDecayTree( in.leg2() );
  
  boostX.set(*ZllboostedX);
  boostX.set(*ZjjboostedX);

  phistarZll=ZllboostedX->p4().phi();
  phistarZjj=ZjjboostedX->p4().phi();
 

  // bool Zllis1=false;
  // int sign_Zll=-1;
  if(!(phistarZll>=0.0 && phistarZll<M_PI)){
    if(phistarZjj<0.0&&phistarZjj>-M_PI){//uh-oh,this should not be...
      std::cout<<"ERROR in phi assignment !!!!!!"<<std::endl;
    }
  }
  //  else{
  //Zllis1=true; sign_Zll=+1;
  // }


  costhetastar=cos(ZllboostedX->p4().theta()); //Z are distinguishable, Zll is always what Z1 is in the 4l case
  
  math::XYZVector v_pbeamLAB( 0.0, 0.0, 1.0 );
  //cross prod beam x Zll
  math::XYZVector v_1 = (v_pbeamLAB.Cross(  (ZllboostedX->momentum()).unit()) ).unit();//versor normal to z-z' plane

  math::XYZVector v_2(0.,-33333.,999.);//dummy init
  int negLeptInd=-1;
  if(ZllboostedX->daughter(0)->charge()<0.0) negLeptInd=0;
  else  negLeptInd=+1;
  const reco::Candidate *NegLeptboostedX = ZllboostedX->daughter(negLeptInd); // XXX check for double boost

  if( abs(NegLeptboostedX->pdgId())!=11 && abs(NegLeptboostedX->pdgId())!=13  )
    std::cout<<"WARNING: 1st Lepton is neither an electron nor a muon !!!  PdgId="<<NegLeptboostedX->pdgId()<<std::endl;
  //   cout<<"L1 4-mom before boost to X (0): "<<Zll->daughter(negLeptInd)->p4().x()<<", "<<Zll->daughter(negLeptInd)->p4().y()<<", " <<Zll->daughter(negLeptInd)->p4().z()<<", "<<Zll->daughter(negLeptInd)->p4().P()<<endl;
  //   cout<<"L1 4-mom after boost to X (1): "<<NegLeptboostedX->p4().x()<<", "<<NegLeptboostedX->p4().y()<<", " <<NegLeptboostedX->p4().z()<<", "<<NegLeptboostedX->p4().P()<<endl;
  
  //v_2 = cross prod l1 x l2 = versor normal to Zll decay plane
  // careful to the order: L1, the z-axis and Z->ll make a right-handed (non-orthogonal) frame (xyz); at the end we want the angle btw x and y
  
  v_2=(ZllboostedX->momentum().Cross(NegLeptboostedX->momentum().unit())).unit();
  helphiZll=fabs( acos(v_1.Dot(v_2)) );//two-fold ambiguity when doing the acos

  //resolve sign ambiguities: clockwise rotation around ZllboostedX flight direction
  if(v_pbeamLAB.Dot(v_2)>0.0)helphiZll=-1.0*helphiZll;
  else helphiZll=+1.0*helphiZll;


  //std::cout << "cloning" << std::endl;
  //cosThetaZll
  std::auto_ptr<reco::Candidate> XboostedZll =cloneDecayTree( in );
  std::auto_ptr<reco::Candidate> ZllboostedZll =cloneDecayTree( in.leg1() );
  boostZll.set(*ZllboostedZll);
  boostZll.set(*XboostedZll);
  const reco::Candidate& lepton_neg=(*ZllboostedZll->daughter(negLeptInd));
  helcosthetaZll = (-1.0*(lepton_neg.p4().x()* XboostedZll->p4().x()+
			  lepton_neg.p4().y()* XboostedZll->p4().y()+
			  lepton_neg.p4().z()* XboostedZll->p4().z())/
		    (lepton_neg.p4().P()* XboostedZll->p4().P())  );

 

 
  if(!isMergedJet){
  //the sign is undefined (impossible to distinguish q from qbar)
    math::XYZVector v_3(0.,-22222.,888.);
  //possible convention: take the jet with highest pt in LAB
  //is it really a good idea? are we losing resolution ? possible pT mismatch GEN vs RECO
  //other idea: just pick the jet with pos azim angle in the Zjj rest frame
  std::auto_ptr<reco::Candidate> ZjjboostedZjj =cloneDecayTree( in.leg2() );
  boostZjj.set(*ZjjboostedZjj);
  int posphiZjjdau=-1;
  for(size_t dau=0;dau<2;dau++){     //ZjjboostedZjj->numberOfDaughters();dau++){ //careful! there 4 daughters!
    if(ZjjboostedZjj->daughter(dau)->phi()>=0)posphiZjjdau=dau; // changed >0 to >=0 to avoid problems with non-converging kinematic fit
  }
 
  const reco::Candidate *JetboostedX = ZjjboostedX->daughter(posphiZjjdau);
 
  //v_3 = similar to v_2, BUT
  //now, if we want a right-handed set of unit-vectors, keeping the same direction of the z-axis
  //we must swap the direction of one of the other two vectors of the Z bosons.
  //Keeping the same direction of the z-axis
  //means measuring phiZll and phiZjj w.r.t. to the same v_1 vector (i.e. w.r.t. the same z'-Zll plane)
  v_3=   ((-1.0*ZjjboostedX->momentum()).Cross(JetboostedX->momentum().unit())).unit() ;

  //in other terms: we can define v_3 as above and then do the crss prod with v_1
  //or define v_3 in a way consistent with v_2 and then do the cross product with a newly defined
  //unit vector v_4 =  (v_pbeamLAB.Cross(  (ZjjboostedX->momentum()).unit()) ).unit();//versor normal to z-Zjj plane

  //phi1 and phi2

  helphiZjj=fabs( acos(v_1.Dot(v_3)) );//two-fold ambiguity when doing the acos
  //phi
  helphi    =fabs( acos(v_2.Dot(v_3)) );//two-fold ambiguity when doing the acos + pi ambiguity from sign of v_3

  //resolve sign ambiguities
    if(v_pbeamLAB.Dot(v_3)>0.0)helphiZjj=+1.0*helphiZjj;
    else helphiZjj=-1.0*helphiZjj;
    if(NegLeptboostedX->momentum().Dot(v_3)>0.0)helphi= +1.0 * helphi;
    else helphi= -1.0 * helphi;

  std::auto_ptr<reco::Candidate> XboostedZjj =cloneDecayTree( in );
  boostZjj.set(*XboostedZjj);

  //since we cannot distinguish btw quark/anti-q, there is an ambiguity also in cos(theta_jj)
  //notice that we take the abs value of cos(theta_jj)
  const reco::Candidate& daughter_posphi=(*ZjjboostedZjj->daughter(posphiZjjdau));
  helcosthetaZjj = fabs((daughter_posphi.p4().x()* XboostedZjj->p4().x()+
			 daughter_posphi.p4().y()* XboostedZjj->p4().y()+
			 daughter_posphi.p4().z()* XboostedZjj->p4().z())/
			(daughter_posphi.p4().P()* XboostedZjj->p4().P())  );

  //sanity check
  if(fabs(helphi+helphiZll+helphiZjj) > 0.001){
    if( (helphi+helphiZll+helphiZjj - 2*M_PI > 0.001) && (helphi+helphiZll-helphiZjj + 2*M_PI > 0.001) ){
      std::cout <<">>>>> WARNING !!!! Error when calculating Helicity angles ! Sum of HelPhi different from zero ! -> HelPhi = "<<helphi<<" HelPhiZLL = "<<helphiZll<<" HelPhiZjj = "<<helphiZjj<<"  SUM = "<< helphi+helphiZll+helphiZjj <<"  Phi+PhiLL-PhiJJ = "<<helphi+helphiZll-helphiZjj <<std::endl;

    }
  }

  }  //end if it is NOT Merged Jet case


    obj->costhetastar_ = costhetastar;
    obj->helphi_ = helphi;
    obj->helphiZll_ = helphiZll;
    obj->helphiZjj_ = helphiZjj;
    obj->helcosthetaZll_ = helcosthetaZll;
    obj->helcosthetaZjj_ = helcosthetaZjj;
    obj->phistarZll_ = phistarZll;
    obj->phistarZjj_ = phistarZjj; 
}


}//end namespace cmg

#endif
