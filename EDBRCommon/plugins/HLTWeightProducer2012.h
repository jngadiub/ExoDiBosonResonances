#ifndef ExoDiBosonResonances_EDBRCommon_HLTWeightProducer2012_h
#define ExoDiBosonResonances_EDBRCommon_HLTWeightProducer2012_h
//#include <Riostream.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

template <class restype>
class HLTWeightProducer2012 : public edm::EDProducer {
public:
  explicit HLTWeightProducer2012(const edm::ParameterSet& iConfig):src_(iConfig.getParameter<edm::InputTag>("src")){

    muChan_=iConfig.getParameter<bool>("isMuChannel");
    std::vector<edm::ParameterSet> ranges =  iConfig.getParameter<std::vector<edm::ParameterSet> >("ranges");
    ranges_=ranges.size();
    
    for(std::vector<edm::ParameterSet>::iterator range = ranges.begin() ; range != ranges.end() ; range++){
      // read lumi corresponding to the run range
      lumis_.push_back(range->getParameter<double>("lumi"));
      
      // read scale factors of the global muon leg:
      std::vector<edm::ParameterSet> leg1defs = range->getParameter<std::vector<edm::ParameterSet> >("lep1");
      doubleSize1_.push_back(leg1defs.size());//store number of bins for each runrange
      doubleRangeIndex1_.push_back(doubleEffs1_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = leg1defs.begin() ; bin != leg1defs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta bounds,pt bounds, sf
	doubleEffs1_.push_back(binDescriptor[4]);
	doubleEtaRanges1_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	doublePtRanges1_.push_back( std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }

      // read scale factors of the tracker muon leg:
      std::vector<edm::ParameterSet> leg2defs = range->getParameter<std::vector<edm::ParameterSet> >("lep2");
      doubleSize2_.push_back(leg2defs.size());//store number of bins for each runrange
      doubleRangeIndex2_.push_back(doubleEffs2_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = leg2defs.begin() ; bin != leg2defs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta bounds,pt bounds, sf
	doubleEffs2_.push_back(binDescriptor[4]);
	doubleEtaRanges2_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	doublePtRanges2_.push_back( std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }

      // read scale factors of the trigger:
      std::vector<edm::ParameterSet> trigdefs = range->getParameter<std::vector<edm::ParameterSet> >("hlt");
      trigSize_.push_back(trigdefs.size());//store number of bins for each runrange
      trigRangeIndex_.push_back(trigEffs_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = trigdefs.begin() ; bin != trigdefs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta_mu1 bounds, eta_mu2 bounds, sf
	trigEffs_.push_back(binDescriptor[4]);
	trigEta1Ranges_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	trigEta2Ranges_.push_back(std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }
      

    } // END of loop over parameter sets
    produces<std::vector<restype> >();
  }

  ~HLTWeightProducer2012(){}
  
private:
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
  double getEleSF(double pt, double eta, int range);
  double getGlobalMuSF(double pt, double eta, int range);  // global leg
  double getTrkMuSF(double pt, double eta, int range);  // tracker leg
  double getHLTSF(double eta1, double eta2, int range);
  double getFromVect(double pt, double eta, int range, int type);

  edm::InputTag src_;
  bool muChan_;

  std::vector<double> lumis_;

  //global leg bin ranges and efficiencies
  std::vector<double> doubleEffs1_;
  std::vector<std::pair<double,double> > doubleEtaRanges1_;
  std::vector<std::pair<double,double> > doublePtRanges1_;
  std::vector<int>    doubleRangeIndex1_; //points to indices
  std::vector<int>    doubleSize1_;

  //tracker leg bin ranges and efficiencies
  std::vector<double> doubleEffs2_;
  std::vector<std::pair<double,double> > doubleEtaRanges2_;
  std::vector<std::pair<double,double> > doublePtRanges2_;
  std::vector<int>    doubleRangeIndex2_; //points to indices
  std::vector<int>    doubleSize2_;

  // trigger bin ranges and efficiencies
  std::vector<double> trigEffs_;
  std::vector<std::pair<double,double> > trigEta1Ranges_;
  std::vector<std::pair<double,double> > trigEta2Ranges_;
  std::vector<int>    trigRangeIndex_; //points to indices
  std::vector<int>    trigSize_;
  
  int ranges_;
    
};

template <class restype>
void HLTWeightProducer2012<restype>::produce(edm::Event& iEvent, const edm::EventSetup&) {
  
  double weight = 1.;

  // read input collection
  edm::Handle<edm::View<restype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);

  // prepare room for output
  std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());
  
  for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
    restype newCand(*edbrIt);
    if(!iEvent.isRealData()){ // only look for MC

      //guess if we are in the WW or ZZ case
      bool onlyLep1=( abs(newCand.leg1().leg2().pdgId())!=11 && abs(newCand.leg1().leg2().pdgId())!=13 && abs(newCand.leg1().leg2().pdgId())!=15);

      double pt1 = newCand.leg1().leg1().pt();
      double eta1 = newCand.leg1().leg1().eta();
      double pt2 =0.0;
      double eta2=9999.0;
      if(!onlyLep1){
	pt2 = newCand.leg1().leg2().pt();
	eta2 = newCand.leg1().leg2().eta();
      }

      //identify which is the global muon
      bool lep1IsGlbMu=false;
      bool lep2IsGlbMu=false;
      if(muChan_){
	if(onlyLep1)lep1IsGlbMu=true;
	else{
	  lep1IsGlbMu=newCand.leg1().leg1().isGlobalMuon();
	  lep2IsGlbMu=newCand.leg1().leg2().isGlobalMuon();
	}
      }

      // efficiency for double trigger
      double effAll = 0; 
      double lumiAll = 0;
    
      for(int i = 0; i < ranges_ ; i++){
	double sfLep1 = 1.0;
	double sfLep2 = 1.0;

	if(muChan_){
	  if(lep1IsGlbMu)sfLep1 =getGlobalMuSF( pt1, eta1, i );
	  else sfLep1 =getTrkMuSF( pt1, eta1, i );
	  if(!onlyLep1){//ZZ case
	    if(lep2IsGlbMu)sfLep2 = getGlobalMuSF( pt2, eta2, i );
	    else sfLep2 = getTrkMuSF( pt2, eta2, i );
	  }
	}//end if is mu channel
	else{
	  sfLep1 =getEleSF( pt1, eta1, i );
	  if(!onlyLep1){//ZZ case
	    sfLep2 =getEleSF( pt2, eta2, i );
	  }
	}
	double sfHLT = getHLTSF( eta1, eta2, i );

	double totalSF = sfLep1*sfLep2*sfHLT;
	
	effAll += lumis_[i]*totalSF;
	lumiAll += lumis_[i];
	//	std::cout<<"Cand with M="<<newCand.mass()<<" abs(pdgId)="<<abs(newCand.leg1().leg2().pdgId()) <<" range="<<i<<"  SF1="<<sfLep1<<"  SF2="<<sfLep2<<"  sfHLT="<<sfHLT<<endl;
      }//end loop on ranges

      weight = effAll / lumiAll;//total weight is the average of the time-dependent weights 
      //std::cout<<"Cand with M="<<newCand.mass()<<" "<<(onlyLep1? "WW " : "ZZ " )<<(muChan_? "MM":"EE")<<" -> added HLT weight="<<weight<<std::endl;
    }//end if is realData
   
    newCand.addUserFloat("HLTWeight",weight);
    outEDBR.push_back(newCand);
  }
  
  std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
  iEvent.put(out);
}



template <class restype>
double HLTWeightProducer2012<restype>::getFromVect(double pt, double eta, int range, int type){
  double weight=0;

  //initilize vectors byt type
  std::vector<double>* eff=0;
  std::vector<std::pair<double,double> >* etaRanges=0;
  std::vector<std::pair<double,double> >* ptRanges=0;
  std::vector<int>*    rangeIndex=0; 
  std::vector<int>*    size=0;
  
  switch(type){
  case 0: 
    eff = &doubleEffs1_;
    etaRanges = &doubleEtaRanges1_;
    ptRanges = &doublePtRanges1_;
    rangeIndex = & doubleRangeIndex1_;
    size = &doubleSize1_;
    break;
  case 1:
    eff = &doubleEffs2_;
    etaRanges = &doubleEtaRanges2_;
    ptRanges = &doublePtRanges2_;
    rangeIndex = &doubleRangeIndex2_;
    size = &doubleSize2_;
    break;
  case 2:  //never mind the name of the variables. In this case we deal with only eta1 and eta2
    eff = &trigEffs_;
    ptRanges = &trigEta1Ranges_;
    etaRanges = &trigEta2Ranges_;
    rangeIndex = &trigRangeIndex_;
    size = &trigSize_;
    break;
  } 

  for(int i = 0; i < size->at(range);i++){
    if(fabs(eta)>= etaRanges->at( rangeIndex->at(range)+i ).first && fabs(eta) < etaRanges->at( rangeIndex->at(range)+i ).second &&
       fabs(pt) >=  ptRanges->at( rangeIndex->at(range)+i ).first && fabs(pt)  <  ptRanges->at( rangeIndex->at(range)+i ).second  ){
      weight = eff->at(rangeIndex->at(range)+i);
      break;
    }
  }

  return weight;

}

template <class restype>
double HLTWeightProducer2012<restype>::getGlobalMuSF(double pt, double eta, int range){
  return getFromVect(pt,eta,range,0);
}
template <class restype>
double HLTWeightProducer2012<restype>::getTrkMuSF(double pt, double eta, int range){
  return getFromVect(pt,eta,range,1);
}
template <class restype>
double HLTWeightProducer2012<restype>::getHLTSF(double eta1, double eta2, int range){
  return getFromVect(eta1,eta2,range,2);
}

template <class restype>
double HLTWeightProducer2012<restype>::getEleSF(double pt, double eta, int range){
  return getFromVect(pt,eta,range,0);
}


#endif
