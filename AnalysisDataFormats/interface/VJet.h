#ifndef _AnalysisDataFormats_ExoDiBosonResonances_VJet_H_
#define _AnalysisDataFormats_ExoDiBosonResonances_VJet_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"
#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"

#include <vector>

namespace cmg {

  //forward def needed
  class VJet;

  /** Class representing V-tagged jets in the CMG framework.
      
  In addition to the attributes from the BaseJet and PFJet mother classes, 
  VJets contains...

  */


  class VJet : public PFJet {
  public:

    ///number of subjets in pruned jet
    //   static const unsigned NSUBJETS;
    //empty constructor
    VJet() :  qjet_(-1.0), tau1_(0.0), tau2_(-1.0), tau3_(99.0),mdrop_(-99.0), prunedMass_(-111.0){};

    //constructor
      VJet(const value& j): PFJet(j), qjet_(-1.0), tau1_(0.0), tau2_(-1.0), tau3_(99.0),mdrop_(-99.0), prunedMass_(-111.0){};

    virtual ~VJet(){}

    float qjet() const {return qjet_;}
    float tau1() const {return tau1_;}
    float tau2() const {return tau2_;}
    float tau3() const {return tau3_;}
    float ntau21() const {return tau2_/tau1_;}
    float ntau32() const {return tau3_/tau2_;}
    float ntau31() const {return tau3_/tau1_;}
    float mdrop() const {return mdrop_;}
    float prunedMass() const {return prunedMass_;}

    //return pointer to matched pruned jet
    pat::JetPtr const* prunedJetPtr() const{
      return &prunedJetPtr_;
    }

    void setPrunedJetPtr(const pat::JetPtr& ptr){
      prunedJetPtr_=ptr;
    }

    friend class VJetFactory;

  private:

    float qjet_; //qjet volatility
    float tau1_, tau2_,tau3_;
    float mdrop_, prunedMass_;
    pat::JetPtr prunedJetPtr_;

  };
}


#endif
