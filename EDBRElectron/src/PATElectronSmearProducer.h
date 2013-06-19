#ifndef ExoDiBosonResonances_EDBRCommon_PATElectronSmearProducer_h
#define ExoDiBosonResonances_EDBRCommon_PATElectronSmearProducer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "boost/scoped_ptr.hpp"
namespace CLHEP {
  class RandGaussQ;
}

class PATElectronSmearProducer : public edm::EDProducer {
public:
  explicit PATElectronSmearProducer(const edm::ParameterSet&);
  ~PATElectronSmearProducer();
  
private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag inputElectrons_;
  double        momentumScale_;
  double        highPtRegion_;
  double        extraMomentumScale_;
  double        electronMass;
  double        factor;
  bool          isScale_;
  bool          isPositive_;
  boost::scoped_ptr<CLHEP::RandGaussQ> gaussDistribution_;
};


#endif
