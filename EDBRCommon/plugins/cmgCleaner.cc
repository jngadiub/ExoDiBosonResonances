#include "ExoDiBosonResonances/EDBRCommon/plugins/cmgCleaner.h"

#include "FWCore/Framework/interface/MakerMacros.h"


#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/VJet.h"

namespace cmg {
  typedef cmg::cmgCleaner<cmg::BaseJet>  cmgBaseJetCleaner;
  typedef cmg::cmgCleaner<cmg::PFJet>  cmgPFJetCleaner;
  typedef cmg::cmgCleaner<cmg::VJet>  cmgVJetCleaner;

}
using namespace cmg;
DEFINE_FWK_MODULE(cmgBaseJetCleaner);
DEFINE_FWK_MODULE(cmgPFJetCleaner);
DEFINE_FWK_MODULE(cmgVJetCleaner);
