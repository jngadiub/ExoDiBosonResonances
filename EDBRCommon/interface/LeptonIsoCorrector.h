#ifndef LEPTONISOCORRECTOR_H_
#define LEPTONISOCORRECTOR_H_


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
    class LeptonIsoCorrector : public edm::EDProducer {
  public:

   explicit LeptonIsoCorrector(const edm::ParameterSet& ps):
     src_(ps.getParameter<edm::InputTag>("src")),
     rho_(ps.getParameter<edm::InputTag>("rho")),
     relisocut_(ps.getParameter<double>("relisocut")),
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
      
     virtual ~LeptonIsoCorrector(){
	//
      }


  private:
     // virtual void beginJob();
     //  virtual void endJob();
      void produce( edm::Event &iEvent, const edm::EventSetup &iSetup);
      
      //data members
      const edm::InputTag src_;
      const edm::InputTag rho_;
      const double relisocut_;
      std::vector<double> etaBins_;//bins of eta used for EffArea
      std::vector<double> effAreaVals_;
      const bool verbose_;
      typedef std::vector<LeptonType> leptCollection;
  };
}//end namespace cmg


template <class LeptonType> 
void cmg::LeptonIsoCorrector<LeptonType>::produce( edm::Event &iEvent, const edm::EventSetup &iSetup){

  edm::Handle<leptCollection> input;
  iEvent.getByLabel(src_,input);

  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rho_,rhoHandle);

  double rho=(*rhoHandle);

  std::auto_ptr<leptCollection> output(new leptCollection);

  for(typename leptCollection::const_iterator it = input->begin(); it != input->end(); ++it){
    double chIso=it->chargedHadronIso();//only charged hadrons
    // double chAllIso=it->chargedAllIso();//ch hadrons + ele + mu
    double nhIso=it->neutralHadronIso();
    double phIso=it->photonIso();

    double uncorr_iso=chIso+nhIso+phIso;
    double uncorr_reliso=uncorr_iso/it->pt();

    //for instructions on how to correct, refer to 
    //https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection
    double etaLept=fabs(it->eta());
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

    double corr_nhIso=(nhIso+phIso)-effArea*rho;
    if(corr_nhIso<0.0)corr_nhIso=0.0;
    double corr_iso=chIso+corr_nhIso;
    double corr_reliso=corr_iso/it->pt();

    if(verbose_) std::cout<<"Corrected the isolation for lepton with pt="<<it->pt()<<" and eta="<<it->eta()<<" rho="<<rho <<" EffArea="<< effArea<<". RelIso uncorr.: "<<uncorr_reliso<<" ---> RelIso corr.: "<<corr_reliso<<std::endl;

    if(corr_reliso<relisocut_){//hurray, lepton in output collection !
      output->push_back(*it);
    }
    

  }//end loop on input collection
  iEvent.put(output); 
}//end produce




#endif


#include "FWCore/Framework/interface/MakerMacros.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
typedef cmg::LeptonIsoCorrector< cmg::Electron > ElectronIsoCorrector;
typedef cmg::LeptonIsoCorrector< cmg::Muon > MuonIsoCorrector;
DEFINE_FWK_MODULE(ElectronIsoCorrector);
DEFINE_FWK_MODULE(MuonIsoCorrector);
