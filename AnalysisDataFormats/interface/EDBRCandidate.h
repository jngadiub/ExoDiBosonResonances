#ifndef EDBRCANDIDATE_H_
#define EDBRCANDIDATE_H_

#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"


namespace cmg {

  template <typename T, typename U> class EDBRCandidateFactory;

  template< typename TL1, typename TL2 > class EDBRCandidate : public DiObject<TL1,TL2>{
  public:
    EDBRCandidate(): 
      DiObject<TL1,TL2>(),
      nj_(0),
      costhetastar_(UnSet(Double_t)),
      helphi_(UnSet(Double_t)),
      helphiZll_(UnSet(Double_t)),
      helphiZjj_(UnSet(Double_t)),
      helcosthetaZll_(UnSet(Double_t)),
      helcosthetaZjj_(UnSet(Double_t)),
      phistarZll_(UnSet(Double_t)),
      phistarZjj_(UnSet(Double_t)),
      vbfptr_(),
      userFloatLabels_(),
      userFloats_(){      
    }
    EDBRCandidate(const EDBRCandidate<TL1,TL2>& other):
      DiObject<TL1,TL2>(other),
      nj_(0),
      costhetastar_(other.costhetastar_),
      helphi_(other.helphi_),
      helphiZll_(other.helphiZll_),
      helphiZjj_(other.helphiZjj_),
      helcosthetaZll_(other.helcosthetaZll_),
      helcosthetaZjj_(other.helcosthetaZjj_),
      phistarZll_(other.phistarZll_),
      phistarZjj_(other.phistarZjj_),
      vbfptr_(other.vbfptr_),
      userFloatLabels_(other.userFloatLabels_),
      userFloats_(other.userFloats_){
    }
    EDBRCandidate(const DiObject<TL1,TL2>& other):
      DiObject<TL1,TL2>(other),
      nj_(0),
      costhetastar_(UnSet(Double_t)),
      helphi_(UnSet(Double_t)),
      helphiZll_(UnSet(Double_t)),
      helphiZjj_(UnSet(Double_t)),
      helcosthetaZll_(UnSet(Double_t)),
      helcosthetaZjj_(UnSet(Double_t)),
      phistarZll_(UnSet(Double_t)),
      phistarZjj_(UnSet(Double_t)),
      vbfptr_(),
      userFloatLabels_(),
      userFloats_(){
   }

    virtual ~EDBRCandidate(){}

    int nJets() const {return nj_;} //by how many jets is composed the hadronic V

    Double_t costhetastar() const{ return costhetastar_;}
    Double_t helphi() const{ return helphi_;}
    Double_t helphiZl1() const{ return helphiZll_;}
    Double_t helphiZl2() const{ return helphiZjj_;}
    Double_t helcosthetaZl1() const{ return helcosthetaZll_;}
    Double_t helcosthetaZl2() const{ return helcosthetaZjj_;}
    Double_t phistarZl1() const{ return phistarZll_;}
    Double_t phistarZl2() const{ return phistarZjj_;}

    edm::Ptr<cmg::DiPFJet> vbfptr() const{ return vbfptr_;}

    float userFloat( const std::string & key ) const;
    float userFloat( const char* key ) const {return userFloat(std::string(key));};
    void addUserFloat( const  std::string & label, float data );
    const std::vector<std::string> & userFloatNames() const  { return userFloatLabels_; }
    bool hasUserFloat( const char * key ) const { return hasUserFloat(std::string(key)); }
    bool hasUserFloat( const std::string & key ) const {
      return std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key) != userFloatLabels_.end();
    }
        
  private:

    int nj_;
    // rest frame angles
    Double_t costhetastar_;
    Double_t helphi_;
    Double_t helphiZll_;
    Double_t helphiZjj_;
    Double_t helcosthetaZll_;
    Double_t helcosthetaZjj_;
    Double_t phistarZll_;
    Double_t phistarZjj_;

    // pointer to dijet pair for vbf tagging
    edm::Ptr<cmg::DiPFJet > vbfptr_;

    //userfloat for everything else
    std::vector<std::string>      userFloatLabels_;
    std::vector<float>            userFloats_;

    friend class cmg::EDBRCandidateFactory<TL1,TL2>;
    
  };

  template< typename TL1, typename TL2 >
  float EDBRCandidate<TL1,TL2>::userFloat( const std::string &key ) const
  {
    std::vector<std::string>::const_iterator it = std::find(userFloatLabels_.begin(), userFloatLabels_.end(), key);
    if (it != userFloatLabels_.end()) {
    return userFloats_[it - userFloatLabels_.begin()];
    }
    return 0.0;
  }
  
  template< typename TL1, typename TL2 >
  void EDBRCandidate<TL1,TL2>::addUserFloat( const std::string & label, float data )
  {
    userFloatLabels_.push_back(label);
    userFloats_.push_back( data );
  }
  
}

#endif
