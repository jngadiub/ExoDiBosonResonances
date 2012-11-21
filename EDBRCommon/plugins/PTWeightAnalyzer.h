#ifndef ExoDiBosonResonances_EDBRCommon_PdfWeightAnalyzer_h
#define ExoDiBosonResonances_EDBRCommon_PdfWeightAnalyzer_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <TH1F.h>

class PTWeightAnalyzer : public edm::EDAnalyzer {
public:
  explicit PTWeightAnalyzer(const edm::ParameterSet&);
  ~PTWeightAnalyzer(){}
  
private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  edm::InputTag weightTag_;
  std::string   selectorPath_;
  TH1F* h_;
      
};


#endif
