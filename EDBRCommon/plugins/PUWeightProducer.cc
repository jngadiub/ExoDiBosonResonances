#include "ExoDiBosonResonances/EDBRCommon/plugins/PUWeightProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
 
PUWeightProducer::PUWeightProducer(const edm::ParameterSet& iConfig):filenameData_(iConfig.getParameter<edm::FileInPath>("filenameData").fullPath()),
								     hnameData_(iConfig.getParameter<std::string>("hnameData")),
								     filenameMC_(iConfig.getParameter<edm::FileInPath>("filenameMC").fullPath()),
								     hnameMC_(iConfig.getParameter<std::string>("hnameMC")),
								     weights_(0){
  
  weights_ = new edm::LumiReWeighting(filenameMC_,filenameData_,hnameMC_,hnameData_);
  // weights3D_ = new edm::Lumi3DReWeighting(filenameMC_,filenameData_,hnameMC_,hnameData_,"");
  // weights3D_->weight3D_init(1.0);

  produces<double>();
}

PUWeightProducer::~PUWeightProducer(){
  delete weights_;
}

void PUWeightProducer::endJob(){}
void PUWeightProducer::beginJob(){}

void PUWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup&) {

  std::auto_ptr<double> weight (new double);
  // std::auto_ptr<double> weight3D (new double);
  (*weight) = 1.;
  // (*weight3D) = 1.;

  if(!iEvent.isRealData()){ // only look for MC

    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    edm::EventBase* iEventB = dynamic_cast<edm::EventBase*>(&iEvent);
    iEventB->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    
    int npv = -1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      
      int BX = PVI->getBunchCrossing();
      
      if(BX == 0) { 
	npv = PVI->getTrueNumInteractions();
	continue;
      }

    } 
    
    (*weight) = weights_->weight( npv );
    // (*weight3D) = weights3D_->weight3D( iEvent );
 
  }

  
  iEvent.put(weight);
  // iEvent.put(weight3D,"weight3D");
 
}
