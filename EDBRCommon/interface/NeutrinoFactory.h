#ifndef _ExoDiBosonResonances_EDBRCommon_NeutrinoFactory_h_
#define _ExoDiBosonResonances_EDBRCommon_NeutrinoFactory_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/ExoDiBosonResonances/interface/Neutrino.h"
#include "CMGTools/Common/interface/Factory.h"

#include "CMGTools/Common/interface/MuonFactory.h"
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"

#include "TLorentzVector.h"

#include <iostream>
#include <memory>

namespace cmg{

class NeutrinoFactory : public Factory<cmg::Neutrino>{
  public:
    NeutrinoFactory(const edm::ParameterSet& ps);
    virtual ~NeutrinoFactory(){;
      //empty
    }
    
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    void setEleNeutrino(const edm::Event&, const edm::EventSetup&, std::vector<TLorentzVector> &, std::vector<int> &, std::vector<TLorentzVector> &);
    void setMuNeutrino(const edm::Event&, const edm::EventSetup&, std::vector<TLorentzVector> &,  std::vector<int> &, std::vector<TLorentzVector> &);
	
  private:

    TLorentzVector neutrinoP4(TLorentzVector* met, TLorentzVector* lep, int lepType);//0 ele, 1 mu

    bool isEleNeutrino_;
    edm::InputTag leptonLabel_;   
    edm::InputTag metLabel_;
    double  MW_;    
  };

}


#endif
