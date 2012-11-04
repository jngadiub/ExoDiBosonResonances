#include "ExoDiBosonResonances/EDBRCommon/interface/JetKinFitterConst.h"
#include "Francesco/KinFitter/src/DiJetKinFitter.h"

int JetKinFitterConst::getCorrJets(TLorentzVector& j1in,TLorentzVector& j2in) const{
  int status=0;
  
  DiJetKinFitter fitalgo("kinfitter","kinfitter",M_);
  
  std::pair<TLorentzVector,TLorentzVector> result = fitalgo.fit(j1in,j2in);
  
  status = fitalgo.getStatus();

  j1in=result.first;
  j2in=result.second;

  return status;
}
