#ifndef __AnalyzerEDBR__
#define __AnalyzerEDBR__

#include <Riostream.h>
#include <string>
#include <vector>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "Alessio/RooFitUtil/src/RooFitUtils/HelicityLikelihoodDiscriminant.h"
//#include "Francesco/KinFitter/src/DiJetKinFitter.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Utilities/interface/Algorithms.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/CompoundTypesHZZLL.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "AnalysisDataFormats/ExoDiBosonResonances/interface/EDBRCandidate.h"

#include "CommonTools/CandUtils/interface/CenterOfMassBooster.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CommonTools/CandUtils/interface/cloneDecayTree.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


#include "TMatrixD.h"
#include "TVectorD.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"


using namespace std;


//template <typename T> 
class AnalyzerEDBR : public edm::EDAnalyzer{

 public:
  AnalyzerEDBR(const edm::ParameterSet &ps);


  virtual ~AnalyzerEDBR(){
  cout << "AnalyzerEDBR destructor..." << endl;
  outFile_->cd();
  outTree_->Write();
  outFile_->Close();
  }

  ///Method where the analysis is done.
 
  void beginRun(edm::Run const& iRun, edm::EventSetup const& eventSetup){
    bool aaa;
    hltConfig.init(iRun,eventSetup,"HLT",aaa);
  }

  void endRun(edm::Run const& iRun, edm::EventSetup const& eventSetup){

  }

  void analyze(edm::Event const& iEvent, edm::EventSetup const& eventSetup);
  

// /// //void analyze(edm::Event const& iEvent, edm::EventSetup const& eventSetup);



 private:
  bool debug_, isMC_,  treatVBFAsMultiple_;            // flags
  std::string cat_;
  std::string outFileName_;
  unsigned int Ngen_;
  double xsec_;

  void init();
  void initTree();
  void initDataMembers();
  void analyzeTrigger(edm::Event const& iEvent, edm::EventSetup const& eventSetup);
  // void analyzeGeneric(edbr,int ih, bool goodKinFit);
  double deltaR(reco::LeafCandidate p1,reco::LeafCandidate p2);
  double deltaR(double phi1,double eta1,double phi2,double eta2);

   //////////////////////////////////////
   /////////////////////////////////////
   //////////////////////////////////////
   /////////////////////////////////////

  template < typename T > void analyzeGeneric(T edbr,int& ih){

 
   //example for the future
   //   if(edbr->getSelection("cuts_btags_btag2")){
   //  btag[ih] = 2;
   // }	

   MCmatch[ih] = edbr->getSelection("cuts_genMatch");
   deltaREDBR[ih] = 0.0;//deltaR(edbr->phi(),edbr->eta(),genEDBR.phi(),genEDBR.eta());

   // if(debug_)cout<<"Inside AnalyzerEDBR::analyzeGeneric "<<ih<<" "<<flush;
   mzz[ih]=edbr->mass();
 
   ptmzz[ih]=edbr->pt();
  
   if(debug_)cout<<"Inside AnalyzerEDBR::analyzeGeneric mzz="<<mzz[ih]<<"  pT(noKinFit)="<<ptmzzNoKinFit[ih] <<endl;

   if(finalM_||sbM_||finalE_||sbE_){
     //     if(debug_)cout<<"AnalyzerEDBR::analyzeGeneric filling hel angles" <<endl;
     hs[ih]     = edbr->costhetastar();
     h1[ih]     = edbr->helcosthetaZl1();
     h2[ih]     = edbr->helcosthetaZl2();
     phi[ih]    = edbr->helphiZl1();
     phiS1[ih]  = edbr->phistarZl1();

     //     if(readLDFromUserFloat_) 
     LD[ih] = edbr->userFloat("LD");
       //   else 
       // LD[ih] = (*ldmapmm)[edbr];
   }
   else{//cand is coming from preselection path
     hs[ih]     = edbr->costhetastar();
     h1[ih]     = edbr->helcosthetaZl1();
     h2[ih]     = edbr->helcosthetaZl2();
     phi[ih]    = edbr->helphiZl1();
     phiS1[ih]  = edbr->phistarZl1();

     // if(readLDFromUserFloat_) 
     LD[ih] = -99.0;//edbr->userFloat("LD");
       //   else 
       // LD[ih] = (*ldmapmm)[edbr];

   }

   //   if(debug_)cout<<"AnalyzerEDBR::analyzeGeneric filling mLL" <<endl;
   mll[ih]=edbr->leg1().mass();
 
   ptZll[ih]=edbr->leg1().pt();
   ptZjj[ih]=edbr->leg2().pt();
   yZll[ih]=edbr->leg1().rapidity();
   yZjj[ih]=edbr->leg2().rapidity();
   phiZll[ih]=edbr->leg1().phi();
   phiZjj[ih]=edbr->leg2().phi();

   //   if(debug_)cout<<"AnalyzerEDBR::analyzeGeneric filling leptons" <<endl;

   bool highptLep1=true;
   if(edbr->leg1().leg2().pt()>edbr->leg1().leg1().pt())highptLep1=false;
   
   if(highptLep1){
     ptlep1[ih]=edbr->leg1().leg1().pt();
     ptlep2[ih]=edbr->leg1().leg2().pt();
     etalep1[ih]=edbr->leg1().leg1().eta();
     etalep2[ih]=edbr->leg1().leg2().eta();
     philep1[ih]=edbr->leg1().leg1().phi();
     philep2[ih]=edbr->leg1().leg2().phi();
     isolep1[ih]=edbr->leg1().leg1().relIso();
     isolep2[ih]=edbr->leg1().leg2().relIso();
   }
   else{
     ptlep1[ih]=edbr->leg1().leg2().pt();
     ptlep2[ih]=edbr->leg1().leg1().pt();
     etalep1[ih]=edbr->leg1().leg2().eta();
     etalep2[ih]=edbr->leg1().leg1().eta();
     philep1[ih]=edbr->leg1().leg2().phi();
     philep2[ih]=edbr->leg1().leg1().phi();
     isolep1[ih]=edbr->leg1().leg2().relIso();
     isolep2[ih]=edbr->leg1().leg1().relIso();
   }


   deltaRleplep[ih]=deltaR(edbr->leg1().leg1().phi(),
			   edbr->leg1().leg1().eta(),
			   edbr->leg1().leg2().phi(),
			   edbr->leg1().leg2().eta());
   
  

   //if(finalM_||sbM_||finalE_||sbE_){//with this if condition, presel will have PU weights==1

   // THESE WEIGHTS=1 FOR REAL DATA
   PU  = edbr->userFloat("PUWeights");
   PUA = edbr->userFloat("PUWeights2012A");
   PUB = edbr->userFloat("PUWeights2012B");
   HLTSF = edbr->userFloat("HLTWeight");

   // }//end if finalM_ || ....

  //  if(debug_)cout<<"AnalyzerEDBR::analyzeGeneric finishing Cand #" <<ih<<endl;
  
  }//end analyzeGeneric

  //////////////////////////////////////
  /////////////////////////////////////
  //////////////////////////////////////
  /////////////////////////////////////

  //functions specific for boosted and not-boosted topologies  
  template < typename T > void  analyzeSingleJet(T edbr,int& ih){
    //  if(debug_)cout<<"AnalyzerEDBR::analyzeSingleJet filling jet vars  " <<ih<<endl;

    nXjets[ih]=1;//edbr->nJets();
    mzzNoKinFit[ih]=edbr->mass();
    mjj[ih]=edbr->leg2().prunedMass();
    mjjNoKinFit[ih]=mjj[ih];
    ptmzzNoKinFit[ih]=edbr->pt();
    ////fill jet kine vars: first jet vars refer to jet used for building EDBR,
    ////                    second jet vars are dummy
 
    bool highptJet1=true;
    if(highptJet1){
      ptjet1[ih]=edbr->leg2().pt();
      ptjet2[ih]=-99.0;
      etajet1[ih]=edbr->leg2().eta();
      etajet2[ih]=-99.0;
      
      betajet1[ih] = edbr->leg2().beta(); 
      betajet2[ih] = -1.0;
      puMvajet1[ih] = edbr->leg2().puMva("full"); 
      puMvajet2[ih] = -9999.0;
    }

    deltaRjetjet[ih]=0.0;

    //jet sub-structure 
    prunedmass[ih]=edbr->leg2().prunedMass();
    mdrop[ih]=edbr->leg2().mdrop();
    qjet[ih]= edbr->leg2().qjet();
    nsubj12[ih]=edbr->leg2().ntau12();
    nsubj23[ih]=edbr->leg2().ntau23();
    tau1[ih]=edbr->leg2().tau1();
    tau2[ih]=edbr->leg2().tau2();


    // dummy jet vars
    btag[ih]=-1;
    q1fl[ih]=-99.0;
    q2fl[ih]=-99.0;
    qgjet1[ih]=-99.0;
    qgjet2[ih]=-99.0;
    qgProduct[ih]=-99.0;
  }//end analyzeSingleJet();


  template < typename T > void  analyzeDoubleJet(T edbr,T edbr_2,int& ih, bool & goodKinFit){


    if(edbr->leg2().mass()<90 ||edbr->leg2().mass()>92){
      cout<<"WARNING from AnalyzeEDBR::analyzeDoubleJet : KIN FIT badly converged!! M_jj="<<edbr->leg2()<<endl;
      goodKinFit = false;
    }

    nXjets[ih]=2;//edbr->nJets();
    mzzNoKinFit[ih]=edbr_2->mass();
    ptmzzNoKinFit[ih]=edbr_2->pt();
    mjj[ih]=edbr->leg2().mass();
    mjjNoKinFit[ih]=edbr_2->leg2().mass();
    if(debug_)cout<<"AnalyzerEDBR::analyzeDoubleJet filling jet vars  " <<ih<<endl;
    ////fill jet kine vars
    bool highptJet1=true;
    if(edbr->leg2().leg2().pt()>edbr->leg2().leg1().pt())highptJet1=false;
    if(highptJet1){
      ptjet1[ih]=edbr->leg2().leg1().pt();
      ptjet2[ih]=edbr->leg2().leg2().pt();
      etajet1[ih]=edbr->leg2().leg1().eta();
      etajet2[ih]=edbr->leg2().leg2().eta();
      
      betajet1[ih] = edbr->leg2().leg1().beta(); 
      betajet2[ih] = edbr->leg2().leg2().beta(); 
      puMvajet1[ih] = edbr->leg2().leg1().puMva("full"); 
      puMvajet2[ih] = edbr->leg2().leg2().puMva("full"); 
    }
    else{
      ptjet2[ih]=edbr->leg2().leg1().pt();
      ptjet1[ih]=edbr->leg2().leg2().pt();
      etajet2[ih]=edbr->leg2().leg1().eta();
      etajet1[ih]=edbr->leg2().leg2().eta();

      betajet2[ih] = edbr->leg2().leg1().beta(); 
      betajet1[ih] = edbr->leg2().leg2().beta(); 
      puMvajet2[ih] = edbr->leg2().leg1().puMva("full"); 
      puMvajet1[ih] = edbr->leg2().leg2().puMva("full");
    }

    deltaRjetjet[ih]=deltaR(edbr->leg2().leg1().phi(),
			    edbr->leg2().leg1().eta(),
			    edbr->leg2().leg2().phi(),
			    edbr->leg2().leg2().eta());

    //jet sub-structure is dummy for double jet
  //jet sub-structure 
    prunedmass[ih]=-99.0;
    mdrop[ih]=-999.0;
    qjet[ih]=-999.0;
    nsubj12[ih]=-999.0;
    nsubj23[ih]=-999.0;
    tau1[ih]=-999.0;
    tau2[ih]=-999.0;

    //#btags is dummy
    btag[ih]=-1;
    q1fl[ih]=-99.0;
    q2fl[ih]=-99.0;
    qgjet1[ih]=-99.0;
    qgjet2[ih]=-99.0;
    qgProduct[ih]=-99.0;


  
  //  if(finalM_||sbM_||finalE_||sbE_){
  //  if(readQGFromUserFloat_) qgProduct[ih] = edbr->userFloat("QG");
  //  else {  // pandolfi to be blamed for this
  //    edm::ProductID prodId = edbr->leg2().leg2().sourcePtr()->id(); //should be the same for both legs
  //    size_t index1 = edbr->leg2().leg1().sourcePtr()->key();
  //    size_t index2 = edbr->leg2().leg2().sourcePtr()->key();
  //    
  //    // QG LIKELIHOOD FROM JETS
  //    qgProduct[ih] = qgmap->get(prodId,index2) * qgmap->get(prodId,index1);
  //    if(edbr->leg2().leg1().pt()>edbr->leg2().leg2().pt()){
  //	qgjet1[ih] = qgmap->get(prodId,index1);
  //	qgjet2[ih] = qgmap->get(prodId,index2);
  //    } else {
  //	qgjet1[ih] = qgmap->get(prodId,index2);
  //	qgjet2[ih] = qgmap->get(prodId,index1);
  //   }
  //  }// end else recalculate qgProd
  //  }//end if finalM_||sbM_||finalE_||sbE_
 

  }//end analyzeDoubleJet();

  //functions specific for lepton flavors
  // void analyzeMuon(edm::RefToBase<cmg::DiMuonDiJetEDBR > edbr, int ih);
  template < typename T > void  analyzeMuon(T edbr, int ih){
    //nothing to be done
    //
    //
    if(debug_)cout<<"Inside AnalyzerEDBR::analyzeMuon"<<endl;



    //dummy for muons 
    eleMVAId1[ih] = -1.0;
    eleMVAId2[ih] = -1.0;
  }//end analyzeMuon


  template < typename T > void analyzeElectron(T edbr, int ih){


    if(debug_)cout<<"Inside AnalyzerEDBR::analyzeElectron for cand#"<<ih<<" : "<<std::flush;
    bool highptLep1=true;
    if(edbr->leg1().leg2().pt()>edbr->leg1().leg1().pt())highptLep1=false;
    
    
    if(highptLep1){
      eleMVAId1[ih] = edbr->leg1().leg1().mvaTrigV0(); 
      eleMVAId2[ih] = edbr->leg1().leg2().mvaTrigV0();
    } 
    else{
      eleMVAId1[ih] = edbr->leg1().leg2().mvaTrigV0(); 
      eleMVAId2[ih] = edbr->leg1().leg1().mvaTrigV0();
    }
    if(debug_)    std::cout<<"leg1.eleMVA="<<eleMVAId1[ih]<<"  leg2.eleMVA="<<eleMVAId2[ih]<<std::endl;
  }//end analyzeElectron




   //////////////////////////////////////
   // DATA MEMBERS


  edm::InputTag XEEColl_,XEENoKinFitColl_,XEELDMap_,XEEJColl_,XEEJLDMap_;//XEENoKinFitLDMap_;
  edm::InputTag XMMColl_,XMMNoKinFitColl_,XMMLDMap_,XMMJColl_,XMMJLDMap_;//XMMNoKinFitLDMap_;
    edm::InputTag  XQGMap_;

  const static int nMaxCand = 30;
  const static int nMaxTrig = 20;
  int nCands_;

  const static int metSignMax = 10;

  /// variables to be stored in the output tree
  TTree* outTree_;
  TFile* outFile_;

  std::vector<std::string> triggerNames_;
  int triggerRes[nMaxTrig];  

  //flags indicating whether the cand passed a certain cms.Path of the analysis
  bool preselM_, finalM_,sbM_;
  bool preselE_, finalE_,sbE_;
  bool preselM1J_, finalM1J_,sbM1J_;
  bool preselE1J_, finalE1J_,sbE1J_;
  bool anyPath_,muPath_,elePath_, singleJetPath_,doubleJetPath_;

  int nCands;
  double hs[nMaxCand], h1[nMaxCand], h2[nMaxCand], phi[nMaxCand], phiS1[nMaxCand], LD[nMaxCand]; // Helicity angles, and LD.
  double mzz[nMaxCand], mzzNoKinFit[nMaxCand], mll[nMaxCand], mjj[nMaxCand], mjjNoKinFit[nMaxCand];           // masses
  double ptmzz[nMaxCand], ptmzzNoKinFit[nMaxCand];
  double ptlep1[nMaxCand], ptlep2[nMaxCand], etalep1[nMaxCand], etalep2[nMaxCand], philep1[nMaxCand], philep2[nMaxCand];  // lepton kinematics
  double ptjet1[nMaxCand], ptjet2[nMaxCand], etajet1[nMaxCand], etajet2[nMaxCand], phijet1[nMaxCand], phijet2[nMaxCand];  // jet kinematicse
  double deltaREDBR[nMaxCand];
  double ptZll[nMaxCand], ptZjj[nMaxCand], yZll[nMaxCand], yZjj[nMaxCand], deltaRleplep[nMaxCand], deltaRjetjet[nMaxCand];
  double phiZll[nMaxCand];//={init};
  double phiZjj[nMaxCand];//={init};
  double met, metSign;            // MET and its significance
  double btag[nMaxCand], lep, reg[nMaxCand];    // b-tags, lep category, region (sig, sideband)
  double qgjet1[nMaxCand], qgjet2[nMaxCand], qgProduct[nMaxCand];    // QG likelihoods
  double betajet1[nMaxCand],betajet2[nMaxCand],puMvajet1[nMaxCand],puMvajet2[nMaxCand];//jet ID 
  double isolep1[nMaxCand], isolep2[nMaxCand], eleMVAId1[nMaxCand], eleMVAId2[nMaxCand];//lepton ID 
  double HLTSF,PU,PUA,PUB,lumiw,genw,w,wA,wB;          // weight
  double MCmatch[nMaxCand];            // mc matching flag
 
  double qjet[nMaxCand],tau1[nMaxCand],tau2[nMaxCand],nsubj12[nMaxCand],nsubj23[nMaxCand];
  double mdrop[nMaxCand],prunedmass[nMaxCand];



  unsigned int nevent,run,ls, njets, nvtx,npu;
  int q1fl[nMaxCand], q2fl[nMaxCand];
  int nXjets[nMaxCand];//by how many jets is the hadronic V made
  //int jjfl[nMaxCand];
  bool readLDFromUserFloat_,  readQGFromUserFloat_;
  HLTConfigProvider hltConfig;

};//end class AnalyzerEDBR 




#endif
