#ifndef LEPTONDETISOCORRECTOR_H_
#define LEPTONDETISOCORRECTOR_H_


#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/EventBase.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CMGTools/Common/interface/SettingTool.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"

namespace cmg{
  //typedef std::vector< cmg::Lepton > PFJetCollectionAB;
  template <class LeptonType> 
    class LeptonDetIsoCorrector : public edm::EDProducer {
  public:

   explicit LeptonDetIsoCorrector(const edm::ParameterSet& ps):
     src_(ps.getParameter<edm::InputTag>("src")),
     rho_(ps.getParameter<edm::InputTag>("rho")),
     //relisocut_(ps.getParameter<double>("relisocut")),
     constantTermBarrel_(ps.getParameter<double>("constantTermBarrel")),
     linearTermBarrel_(ps.getParameter<double>("linearTermBarrel")),
     constantTermEndcap_(ps.getParameter<double>("constantTermEndcap")),
     linearTermEndcap_(ps.getParameter<double>("linearTermEndcap")),
     linearThresholdEndcap_(ps.getParameter<double>("linearThresholdEndcap")),
     etaBins_(ps.getParameter<std::vector<double> >("EAetaBins")),
     effAreaVals_(ps.getParameter<std::vector<double> >("EAvals")),
     verbose_(ps.getParameter<bool>("verbose"))
	{
	  if(etaBins_.size()!=(effAreaVals_.size()+1)){
	    throw cms::Exception("BadInput")<<"Mismatched size of vectors with eta-binning ("<<etaBins_.size()
					    <<")and effArea values ("<< effAreaVals_.size()
					    <<", it should be "<<etaBins_.size()-1 <<")"<<std::endl; 
	  }

	  //
	  produces<leptCollection>();
	}
      
     virtual ~LeptonDetIsoCorrector(){
	//
      }


  private:
     // virtual void beginJob();
     //  virtual void endJob();
      void produce( edm::Event &iEvent, const edm::EventSetup &iSetup);
      
      //data members
      const edm::InputTag src_;
      const edm::InputTag rho_;
      const double constantTermBarrel_;
      const double linearTermBarrel_;
      const double constantTermEndcap_;
      const double linearTermEndcap_;
      const double linearThresholdEndcap_;
      std::vector<double> etaBins_;//bins of eta used for EffArea
      std::vector<double> effAreaVals_;
      const bool verbose_;
      typedef std::vector<LeptonType> leptCollection;
  };
}//end namespace cmg


template <class LeptonType> 
void cmg::LeptonDetIsoCorrector<LeptonType>::produce( edm::Event &iEvent, const edm::EventSetup &iSetup){

  edm::Handle<leptCollection> input;
  iEvent.getByLabel(src_,input);

  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rho_,rhoHandle);

  double rho=(*rhoHandle);

  std::auto_ptr<leptCollection> output(new leptCollection);

  for(typename leptCollection::const_iterator it = input->begin(); it != input->end(); ++it){

    // 'it' is an iterator to a LeptonType... which is a CMG type.
    // In order to get back to the PAT quantities I need to use sourcePtr()

    const pat::Electron& ele = *(*it->sourcePtr()); //Don't ask. Just don't ask.
    double ECALIsol = ele.userIso(1);
    double HCALIsol = ele.userIso(2);
    double sumCaloEt = ECALIsol + HCALIsol;

    double sumCaloEtLimit = -1;
    double et = ele.et();

    double etaLept=fabs(ele.eta());
    const int nEtaBins=etaBins_.size();
    int binIndex=-1;
    //loop over eta bins for finding the right one
    for(int i=0;i<nEtaBins-1;i++){
      if(etaLept>=etaBins_.at(i)&&etaLept<etaBins_.at(i+1))binIndex=i;
    }
    if(binIndex<0&&verbose_){
      std::cout<<"Warning: this lepton has eta="<<etaLept<<" , outside etaBins provided for iso corrections. Not applying any correction."<<std::endl;
    }

    double effArea=0.0;
    if(binIndex>=0)effArea=effAreaVals_.at(binIndex);   
    // Ok, now we have the effective area.

    bool inBarrel = ele.isEE();
    if (inBarrel) {
      sumCaloEtLimit = 
	constantTermBarrel_ + 
	linearTermBarrel_ * et + 
	effArea * rho;
    }
    
    if (!inBarrel) { // FIXME: some electrons are neither in barrel nor endcap... strange.
      if (et < linearThresholdEndcap_)
	sumCaloEtLimit = 
	  constantTermEndcap_ + 
	  effArea * rho;
      else
	sumCaloEtLimit = 
	  constantTermEndcap_ + 
	  linearTermEndcap_ * (et - linearThresholdEndcap_) + 
	  effArea * rho;
    }

    if(sumCaloEt<sumCaloEtLimit){//hurray, lepton in output collection !
      output->push_back(*it);
    }
    

  }//end loop on input collection
  iEvent.put(output); 
}//end produce




#endif


#include "FWCore/Framework/interface/MakerMacros.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
//#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
typedef cmg::LeptonDetIsoCorrector< cmg::Electron > ElectronDetIsoCorrector;
//typedef cmg::LeptonDetIsoCorrector< cmg::Muon > MuonDetIsoCorrector;
DEFINE_FWK_MODULE(ElectronDetIsoCorrector);
//DEFINE_FWK_MODULE(MuonDetIsoCorrector);
