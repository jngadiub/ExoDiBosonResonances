#include <vector>

#include "TLorentzVector.h"
#include "TMatrixD.h"

#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"


class JetKinFitterConst{

public:
  JetKinFitterConst():M_(91.1876),Merr_(0.0){};
  JetKinFitterConst(float m, float merr ):M_(m),Merr_(merr){};//mass constraint and its error
  ~JetKinFitterConst(){};
  //void setJet4Mom(TLorentzVector j1in,TLorentzVector j2in);
  int getCorrJets(TLorentzVector& j1in,TLorentzVector& j2in) const;
  //int Refit();

private:
  const float M_,Merr_;
  //TLorentzVector j1in_, j2in_,j1out_, j2out_;
  //std::vector<TLorentzVector> corrjets_;
  //Double_t ErrEt( double Et,  double Eta) const;
  //Double_t ErrEta( double Et,  double Eta) const;
  //Double_t ErrPhi( double Et,  double Eta) const;
  // void init();
};
