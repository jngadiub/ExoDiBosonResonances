#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <TFile.h>

PTWeightProducer::PTWeightProducer(const edm::ParameterSet& iConfig):genTag_(iConfig.getParameter<edm::InputTag>("src")),
								     pdgId_(iConfig.getParameter<int>("pdgId")),
								     filename_(iConfig.getParameter<edm::FileInPath>("filename").fullPath()),
								     corrfactors_(0){
  TFile* infile = TFile::Open(filename_.c_str());
  if(infile == 0 )
    throw cms::Exception("PTReweight") << " Could not Open weight file " << filename_ << std::endl;

  corrfactors_ = dynamic_cast<TGraph*>((dynamic_cast<TGraph*>(infile->Get("Graph")))->Clone("CorrFac"));
  infile->Close();

  if(corrfactors_ == 0 )
    throw cms::Exception("PTReweight") << " Found no correction factors in file " << filename_ << std::endl;

  produces<double>();
}

PTWeightProducer::~PTWeightProducer(){
  delete corrfactors_;
}

void PTWeightProducer::endJob(){}
void PTWeightProducer::beginJob(){}

void PTWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup&) {

  std::auto_ptr<double> weight (new double);
  (*weight) = 1.;

  if(!iEvent.isRealData()){ // only look for MC
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel(genTag_, genParticles);
    unsigned int gensize = genParticles->size();
    for (unsigned int i = 0; i<gensize; ++i) {
      if ((*genParticles)[i].pdgId() == pdgId_){ // found the Higgs
	float pt = (*genParticles)[i].pt();
	if(pt >=0 && pt <=500)
	  (*weight) = corrfactors_->Eval(pt,0,"S");
	break;
      }
    }
  }

  iEvent.put(weight);
}
