#include "ExoDiBosonResonances/EDBRCommon/plugins/PTWeightAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

PTWeightAnalyzer::PTWeightAnalyzer(const edm::ParameterSet& iConfig):weightTag_(iConfig.getParameter<edm::InputTag>("src")),
								     selectorPath_(iConfig.getParameter<std::string>("selectorPath")),
								     h_(0){
  edm::Service<TFileService> fs;
  h_ = fs->make<TH1F>( "events"  , "events", 4,  -.5, 3.5 );
  h_->GetXaxis()->SetBinLabel(1,"Total");
  h_->GetXaxis()->SetBinLabel(2,"weighted");
  h_->GetXaxis()->SetBinLabel(3,"Selected");
  h_->GetXaxis()->SetBinLabel(4,"Weighted");
}

void PTWeightAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup&){
  edm::Handle<double> weightHandle;
  iEvent.getByLabel(weightTag_, weightHandle);

  edm::Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel(edm::InputTag("TriggerResults"), triggerResults);
  
  bool selectedEvent = false;
  const edm::TriggerNames & trigNames = iEvent.triggerNames(*triggerResults);
  unsigned int pathIndex = trigNames.triggerIndex(selectorPath_);
  bool pathFound = (pathIndex<trigNames.size()); // pathIndex >= 0, since pathIndex is unsigned
  if (pathFound) {
    if (triggerResults->accept(pathIndex)) selectedEvent = true;
  }

  h_->Fill(0.,1.);
  h_->Fill(1.,*weightHandle);
  if(selectedEvent){
    h_->Fill(2.,1.);
    h_->Fill(3.,*weightHandle);  
  }
  
}
