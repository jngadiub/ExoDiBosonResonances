#ifndef ExoDiBosonResonances_EDBRCommon_HLTWeightProducer_h
#define ExoDiBosonResonances_EDBRCommon_HLTWeightProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

template <class restype>
class HLTWeightProducer : public edm::EDProducer {
public:
  explicit HLTWeightProducer(const edm::ParameterSet& iConfig):src_(iConfig.getParameter<edm::InputTag>("src")){

    std::vector<edm::ParameterSet> ranges =  iConfig.getParameter<std::vector<edm::ParameterSet> >("ranges");
    ranges_=ranges.size();
    
    for(std::vector<edm::ParameterSet>::iterator range = ranges.begin() ; range != ranges.end() ; range++){
      // read lumi corresponding to the run range
      lumis_.push_back(range->getParameter<double>("lumi"));
      
      // read first leg double efficiencies corresponding to the run range
      std::vector<edm::ParameterSet> leg1defs = range->getParameter<std::vector<edm::ParameterSet> >("double1");
      doubleSize1_.push_back(leg1defs.size());//store number of bins for each runrange
      doubleRangeIndex1_.push_back(doubleEffs1_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = leg1defs.begin() ; bin != leg1defs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta,pt bounds, eff
	doubleEffs1_.push_back(binDescriptor[4]);
	doubleEtaRanges1_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	doublePtRanges1_.push_back( std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }
      // read first leg double efficiencies corresponding to the run range
      std::vector<edm::ParameterSet> leg2defs = range->getParameter<std::vector<edm::ParameterSet> >("double2");
      doubleSize2_.push_back(leg2defs.size());//store number of bins for each runrange
      doubleRangeIndex2_.push_back(doubleEffs2_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = leg2defs.begin() ; bin != leg2defs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta,pt bounds, eff
	doubleEffs2_.push_back(binDescriptor[4]);
	doubleEtaRanges2_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	doublePtRanges2_.push_back( std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }
      // read first leg double efficiencies corresponding to the run range
      std::vector<edm::ParameterSet> singledefs = range->getParameter<std::vector<edm::ParameterSet> >("single");
      singleSize_.push_back(singledefs.size());//store number of bins for each runrange
      singleRangeIndex_.push_back(singleEffs_.size());//store the index for the flattened vector
      for(std::vector<edm::ParameterSet>::const_iterator bin = singledefs.begin() ; bin != singledefs.end() ;bin++ ){// fill all bins into the flat vector
	std::vector<double> binDescriptor = bin->getParameter<std::vector<double> >("bin");
	assert(binDescriptor.size()==5);//alswas need eta,pt bounds, eff
	singleEffs_.push_back(binDescriptor[4]);
	singleEtaRanges_.push_back(std::make_pair(binDescriptor[0],binDescriptor[1]));
	singlePtRanges_.push_back( std::make_pair(binDescriptor[2],binDescriptor[3]));	
      }
      
      

    } // END of loop over parameter sets
    produces<std::vector<restype> >();
  }

  ~HLTWeightProducer(){}
  
private:
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
  double getDoubleHLTSF1(double pt, double eta, int range);  // leading leg
  double getDoubleHLTSF2(double pt, double eta, int range);  // subleading leg
  double getSingleHLTSF(double pt, double eta, int range);
  double getFromVect(double pt, double eta, int range, int type);

  edm::InputTag src_;

  std::vector<double> lumis_;

  //leading leg bin ranges and efficiencies
  std::vector<double> doubleEffs1_;
  std::vector<std::pair<double,double> > doubleEtaRanges1_;
  std::vector<std::pair<double,double> > doublePtRanges1_;
  std::vector<int>    doubleRangeIndex1_; //points to indices
  std::vector<int>    doubleSize1_;

  //second leg bin ranges and efficiencies
  std::vector<double> doubleEffs2_;
  std::vector<std::pair<double,double> > doubleEtaRanges2_;
  std::vector<std::pair<double,double> > doublePtRanges2_;
  std::vector<int>    doubleRangeIndex2_; //points to indices
  std::vector<int>    doubleSize2_;

  //single trigger bin ranges and efficiencies
  std::vector<double> singleEffs_;
  std::vector<std::pair<double,double> > singleEtaRanges_;
  std::vector<std::pair<double,double> > singlePtRanges_;
  std::vector<int>    singleRangeIndex_; //points to indices
  std::vector<int>    singleSize_;
  
  int ranges_;
    
};

template <class restype>
void HLTWeightProducer<restype>::produce(edm::Event& iEvent, const edm::EventSetup&) {
  
  double weight = 1.;

  // read input collection
  edm::Handle<edm::View<restype> > edbrcandidates;
  iEvent.getByLabel(src_, edbrcandidates);

  // prepare room for output
  std::vector<restype> outEDBR;   outEDBR.reserve(edbrcandidates->size());
  
  for ( typename edm::View<restype>::const_iterator edbrIt = edbrcandidates->begin() ; edbrIt != edbrcandidates->end() ; ++edbrIt ) {
    restype newCand(*edbrIt);
    if(!iEvent.isRealData()){ // only look for MC
      double pt1 = newCand.leg1().leg1().pt();
      double pt2 = newCand.leg1().leg2().pt();
      double eta1 = newCand.leg1().leg1().eta();
      double eta2 = newCand.leg1().leg2().eta();
      
      // efficiency for double trigger
      double effAll = 0; 
      double lumiAll = 0;
    
      for(int i = 0; i < ranges_ ; i++){
	double effDouble1 = getDoubleHLTSF1( pt1, eta1, i );
	double effDouble2 = getDoubleHLTSF2( pt2, eta2, i );
	double effSingle1 = getSingleHLTSF( pt1, eta1, i );
	double effSingle2 = getSingleHLTSF( pt2, eta2, i );
	
	double totalEff = effDouble1 * effDouble2 + effSingle2 * (1. - effDouble1 ) + effSingle1 * (1. - effDouble2 ); 
	
	effAll += lumis_[i]*totalEff;
	lumiAll += lumis_[i];
      }
      weight = effAll / lumiAll;
    }
    newCand.addUserFloat("HLTWeight",weight);
    outEDBR.push_back(newCand);
  }
  
  std::auto_ptr<std::vector<restype> > out(new std::vector<restype>(outEDBR));
  iEvent.put(out);
}



template <class restype>
double HLTWeightProducer<restype>::getFromVect(double pt, double eta, int range, int type){
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
  case 2:
    eff = &singleEffs_;
    etaRanges = &singleEtaRanges_;
    ptRanges = &singlePtRanges_;
    rangeIndex = &singleRangeIndex_;
    size = &singleSize_;
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
double HLTWeightProducer<restype>::getDoubleHLTSF1(double pt, double eta, int range){
  return getFromVect(pt,eta,range,0);
}
template <class restype>
double HLTWeightProducer<restype>::getDoubleHLTSF2(double pt, double eta, int range){
  return getFromVect(pt,eta,range,1);
}
template <class restype>
double HLTWeightProducer<restype>::getSingleHLTSF(double pt, double eta, int range){
  return getFromVect(pt,eta,range,2);
}

#endif
