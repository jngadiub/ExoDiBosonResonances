#ifndef _ExoDiBosonResonances_EDBRCommon_VJetFactory_h_
#define _ExoDiBosonResonances_EDBRCommon_VJetFactory_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/ExoDiBosonResonances/interface/VJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"

#include <iostream>
#include <memory>

namespace cmg{

class VJetFactory : public Factory<cmg::VJet>{
  public:
    VJetFactory(const edm::ParameterSet& ps);
    
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    void set(const edm::Event& iEvent, const edm::EventSetup& iSetup,
			  const pat::JetPtr& input, cmg::VJet *output);

  private:
    const edm::InputTag jetLabel_;
    const edm::InputTag prunedJetLabel_;
    //    const edm::InputTag subJetLabel_;
    const BaseJetFactory baseJetFactory_;
    const PFJetFactory pfJetFactory_;
    edm::InputTag puVariablesTag_;
    std::vector< edm::InputTag > puMvasTag_, puIdsTag_,puNamesTag_;
    const bool useConstituents_;
    const bool verbose_;

    //------------------------------ANIELLO------------------------------//
    const bool applyResolution_;
    const std::string resolutionFile_;
    const double resolutionOverride_;
    
    const bool applyScale_, applyScaleDB_;
    const std::string scaleFile_;
    const double nSigmaScale_;
    //------------------------------ANIELLO------------------------------//
  };

}


#endif
