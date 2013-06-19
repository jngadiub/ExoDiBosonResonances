#include "ExoDiBosonResonances/EDBRElectron/src/PATElectronSmearProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "CLHEP/Random/RandGaussQ.h"

PATElectronSmearProducer::PATElectronSmearProducer(const edm::ParameterSet& iConfig):
  inputElectrons_(iConfig.getParameter<edm::InputTag>("src")),
  momentumScale_(iConfig.getParameter<double>("momentumScale")),
  highPtRegion_(iConfig.getParameter<double>("highPtRegion")),
  extraMomentumScale_(iConfig.getParameter<double>("extraMomentumScale")),
  electronMass(0.000510998910),//GeV
  factor(1.0),
  isScale_(iConfig.getParameter<bool>("isScale")),
  isPositive_(iConfig.getParameter<bool>("isPositive"))
{
  produces<std::vector<pat::Electron> >();
}

PATElectronSmearProducer::~PATElectronSmearProducer(){
}

void PATElectronSmearProducer::endJob(){}

void PATElectronSmearProducer::beginJob(){
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) {
    throw cms::Exception("Configuration")
      << "PATElectronSmearProducer requires the RandomNumberGeneratorService\n"
      "which is not present in the configuration file.  You must add the service\n"
      "in the configuration file or remove the modules that require it.";
  }
  
  // Get the engine.
  // Note this can be done in any method of the module
  CLHEP::HepRandomEngine& engine = rng->getEngine();

  // engine MUST be a reference here, if a pointer is used the
  // distribution will destroy the engine in its destructor, a major
  // problem because the service owns the engine and will destroy it 
  if(momentumScale_ < 0)
    throw cms::Exception("Configuration")
      << "Use isPositive = cms.bool(False) to make a negative scale.";
  
  // Setting up the gauss distribution
  if(momentumScale_ > 1E-6)
    gaussDistribution_.reset(new CLHEP::RandGaussQ(engine,1.0,momentumScale_));
}

void PATElectronSmearProducer::produce(edm::Event& iEvent, const edm::EventSetup&) {

  //load electrons
  edm::Handle<std::vector<pat::Electron> > eleHandle;
  iEvent.getByLabel(inputElectrons_,eleHandle);
  const std::vector<pat::Electron>& electrons = *(eleHandle.product());

  //smeared electrons
  std::auto_ptr<std::vector<pat::Electron> > smearedElectrons (new std::vector<pat::Electron>);

  for(size_t ii=0; ii!=electrons.size(); ++ii) {

    pat::Electron* clonedEle = (pat::Electron*)electrons[ii].clone();

    /// If isScale_, we're shifting the average momentum
    /// else, we're smearing it gaussianly.
    if(isScale_) {
      /// If it's a scale, maybe we should consider the high pT region.
      if(clonedEle->pt() > highPtRegion_) {
	double extraError = ((clonedEle->pt())/1000.0)*extraMomentumScale_;
	double totalError = sqrt(extraError*extraError + momentumScale_*momentumScale_);
	if(isPositive_)
	  factor = 1.0+totalError;
	else
	  factor = 1.0-totalError;
      }      
      else {
	if(isPositive_)
	  factor = 1.0+momentumScale_;
	else
	  factor = 1.0-momentumScale_;
      } // Closes highPtRegion_
    } // Closes isScale_
    else
      factor = gaussDistribution_->fire();
    
    /// First, scale the momentum 
    double pX = factor*(clonedEle->px());
    double pY = factor*(clonedEle->py());
    double pZ = clonedEle->pz(); /// Yes, pZ is not smeared.
    double energy = sqrt((pX*pX)+(pY*pY)+(pZ*pZ)+(electronMass*electronMass));
    
    math::XYZTLorentzVector newP4(pX,pY,pZ,energy);
      
    clonedEle->setP4(newP4); /// This sets the new LorentzVector

    smearedElectrons->push_back(*(clonedEle));
  }
  
  iEvent.put(smearedElectrons);
}
//define this as a plug-in
DEFINE_FWK_MODULE(PATElectronSmearProducer);
