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
  AnalyzerEDBR(const edm::ParameterSet &ps){

    debug_    = ps.getParameter<bool>("debug");
    if(debug_)    cout << "AnalyzerEDBR constructor 555..." << endl;
    cout<<"pippo1"<<endl;
    isMC_       = ps.getParameter<bool>("isMC");
    treatVBFAsMultiple_    = ps.getParameter<bool>("treatVBFAsMultiple");
    Ngen_     = ps.getParameter<unsigned int>("Ngen");
    xsec_     = ps.getParameter<double>("xsec"); // in fb
    cout<<"pippo2"<<endl;
    cat_             = ps.getParameter<std::string>("EventCategory");
    XEEColl_         = ps.getParameter<edm::InputTag>("EDBREECollection");
    XEENoKinFitColl_ = ps.getParameter<edm::InputTag>("EDBREENoKinFitCollection");
    XEELDMap_        = ps.getParameter<edm::InputTag>("EDBREELDValueMap");
    XMMColl_         = ps.getParameter<edm::InputTag>("EDBRMMCollection");
    XMMNoKinFitColl_ = ps.getParameter<edm::InputTag>("EDBRMMNoKinFitCollection");
    XMMLDMap_        = ps.getParameter<edm::InputTag>("EDBRMMLDValueMap");
    XQGMap_          = ps.getParameter<edm::InputTag>("EDBRQGValueMap");
    //XEENoKinFitLDMap_=ps.getParameter<edm::InputTag>("EDBREENoKinFitLDValueMap");
    //XMMNoKinFitLDMap_=ps.getParameter<edm::InputTag>("EDBRMMNoKinFitLDValueMap");
    
    cout<<"pippo3"<<endl;
    if(XEELDMap_.label()=="" ||XMMLDMap_.label()=="" ) readLDFromUserFloat_=true;
    else readLDFromUserFloat_=false;
    
    if(XQGMap_.label()=="") readQGFromUserFloat_=true;
    else readQGFromUserFloat_=false;
    

    outFileName_ = ps.getParameter<string>("outFileName");
    outFile_ = new TFile(outFileName_.c_str(),"recreate");
    if(debug_)cout<<"Storing output TTree in "<<outFileName_.c_str()<<endl;
    
    triggerNames_ = ps.getParameter< vector<string> >("triggerNames");
    
    if(cat_!=""){
      if( cat_!="mmjj" &&cat_!="eejj" && cat_!="mmj" && cat_!="eej"){
      throw cms::Exception("AnalyzerEDBR: Wrong event category passed as input. Possibilities are: mmjj, eejj, mmj, eej");
    }
    }

    if(debug_)cout<<"Initializing"<<endl;

    init();
    if(debug_)cout<<"Initialization is over"<<endl;

  }//end constructor


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


 void analyze(edm::Event const& iEvent, edm::EventSetup const& eventSetup){

  if(debug_) cout<<"\n\nAnalyzing event"<<endl;

  initDataMembers();

  if(isMC_){
    // Ngen=Ngen_;
    lumiw = xsec_/Ngen_;
    edm::Handle<GenEventInfoProduct> hGenEvtInfo;
    // cout<<"Am I crashing ?  "<<flush;
    if(iEvent.getByLabel("generator",hGenEvtInfo)){
      genw=hGenEvtInfo->weights()[0];
      // cout<<"  No ! "<<genw<<endl;
    }
  }
  if(debug_)
    cout<<"lumi weight="<<lumiw<<endl;

 
  nevent = iEvent.eventAuxiliary().event();
  run    = iEvent.eventAuxiliary().run();
  ls     = iEvent.eventAuxiliary().luminosityBlock();
  if(debug_) cout<<endl<<" Run "<<run<<"  Event "<<nevent<<endl;

  // GET VERTICES
  edm::Handle<reco::VertexCollection> vertexCollection;
  if(iEvent.getByLabel("offlinePrimaryVertices",vertexCollection))  nvtx=vertexCollection->size();
  else{
    if(debug_)cout<<"WARNING: VertexCollection called \'offlinePrimaryVertices\' NOT FOUND !"<<endl;
    nvtx=0;
  }

  edm::Handle<std::vector<cmg::PFJet> > allJets;
  iEvent.getByLabel("jetIDJet", allJets);
  njets = allJets->size();

  // GET MISSING ET
  edm::Handle<edm::View<pat::MET> > metHandle;
  iEvent.getByLabel("patMETs", metHandle);
  met     = metHandle->at(0).pt();
  metSign = metHandle->at(0).significance(); 


  analyzeTrigger(iEvent, eventSetup);

  bool eleEvent   = elePath_; //&&(cat_=="eejj" || cat_=="eej")
  bool muEvent    = muPath_;
  bool goodKinFit = true;



  edm::Handle<std::vector<reco::GenParticle> > genParticles;
  iEvent.getByLabel(std::string("genParticles"), genParticles);
  reco::GenParticle genHiggs;

  for(std::vector<reco::GenParticle>::const_iterator genParticle=genParticles->begin(); genParticle!=genParticles->end(); ++genParticle){
    if(genParticle->pdgId()==25||genParticle->pdgId()==39) genHiggs=(*genParticle);
      // if(debug_) cout<<"particle "<<genParticle->pdgId()<<" status "<<genParticle->status()<<endl;
  }

  // edm::Handle<edm::ValueMap<float> > qgmap;
  // if(anyPath_){
  //  if(!readQGFromUserFloat_)iEvent.getByLabel(HiggsQGMap_, qgmap);
  // }



  if(muPath_){
    edm::Handle<edm::View< cmg::DiMuonDiJetEDBR > > finalEDBRcand;
    edm::Handle<edm::View< cmg::DiMuonDiJetEDBR > > finalEDBRcand_2;
    iEvent.getByLabel(XMMColl_        , finalEDBRcand  );  // With kinfit
    iEvent.getByLabel(XMMNoKinFitColl_, finalEDBRcand_2);  // Without kinfit

    int nCandidates=finalEDBRcand->size();
    if (nCandidates > nMaxCand) nCandidates = nMaxCand;
    if(debug_)cout<<"read from MUON event, there are "<<nCandidates<<" H cands"<<endl;
    //  if(nCandidates>0){
    //  if(muPath_)    muEvent = true;
    //  if(elePath_)   eleEvent = true;
    // }
    int ih = 0;
    for(int iih=0;iih<nCandidates;iih++){
       
      lep=1; 
      edm::RefToBase<cmg::DiMuonDiJetEDBR> edbrM =finalEDBRcand->refAt(iih);
      edm::RefToBase<cmg::DiMuonDiJetEDBR> edbrM_2 =finalEDBRcand_2->refAt(iih);
      analyzeGeneric(edbrM, edbrM_2,ih,goodKinFit);
      ih++;
      nCands = ih;
    
    }//end loop on candidates

  }//end if mmjj


  if(cat_=="eejj"){
    edm::Handle<edm::View< cmg::DiElectronDiJetEDBR > > finalEDBRcand;
    edm::Handle<edm::View< cmg::DiElectronDiJetEDBR > > finalEDBRcand_2;
    iEvent.getByLabel(XEEColl_        , finalEDBRcand  );  // With kinfit
    iEvent.getByLabel(XEENoKinFitColl_, finalEDBRcand_2);  // Without kinfit
 
    int nCandidates=finalEDBRcand->size();
    if (nCandidates > nMaxCand) nCandidates = nMaxCand;
    if(debug_)cout<<"read from ELE event, there are "<<nCandidates<<" H cands"<<endl;
    //  if(nCandidates>0){
    //  if(muPath_)    muEvent = true;
    //  if(elePath_)   eleEvent = true;
    // }
    int ih = 0;
    for(int iih=0;iih<nCandidates;iih++){
       
      lep=0; 
      edm::RefToBase<cmg::DiElectronDiJetEDBR> edbrE =finalEDBRcand->refAt(iih);
      edm::RefToBase<cmg::DiElectronDiJetEDBR> edbrE_2 =finalEDBRcand_2->refAt(iih);
      analyzeGeneric(edbrE, edbrE_2,ih,goodKinFit);
      ih++;
      nCands = ih;
    
    }//end loop on candidates

  }//end if eejj
  
  bool passCuts=true;
  bool storeEvt=goodKinFit && (muPath_ || elePath_) && lep<2 && passCuts;
  if(storeEvt){
    outTree_->Fill(); 
  }

  /// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

 
}//
//end AnalyzerEDBR::analyze
  

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



   //////////////////////////////////////
   /////////////////////////////////////
   //////////////////////////////////////
   /////////////////////////////////////

  template < typename T > void analyzeGeneric(T edbr,T edbr_2,int& ih, bool & goodKinFit){

  if(edbr->leg2().mass()<90 ||edbr->leg2().mass()>92){
     cout<<"WARNING from AnalyzeEDBR::analyzeGeneric : KIN FIT badly converged!! M_jj="<<edbr->leg2()<<endl;
     goodKinFit = false;
   }
   //example for the future
   //   if(edbr->getSelection("cuts_btags_btag2")){
   //  btag[ih] = 2;
   // }	

   MCmatch[ih] = edbr->getSelection("cuts_genMatch");
   // deltaREDBR[ih] = deltaR(edbr->phi(),edbr->eta(),genEDBR.phi(),genEDBR.eta());


   mzz[ih]=edbr->mass();
   mzzNoKinFit[ih]=edbr_2->mass();
   mll[ih]=edbr->leg1().mass();
   mjj[ih]=edbr->leg2().mass();
   mjjNoKinFit[ih]=edbr_2->leg2().mass();

   //fill lepton kine vars (only the lept flav independent ones)
   bool highptLep1=true;
   if(edbr->leg1().leg2().pt()>edbr->leg1().leg1().pt())highptLep1=false;
   if(highptLep1){
     ptlep1[ih]=edbr->leg1().leg1().pt();
     ptlep2[ih]=edbr->leg1().leg2().pt();
   }
   else{
     ptlep2[ih]=edbr->leg1().leg1().pt();
     ptlep1[ih]=edbr->leg1().leg2().pt();
   }

   //fill jet kine vars
   bool highptJet1=true;
   if(edbr->leg2().leg2().pt()>edbr->leg2().leg1().pt())highptJet1=false;
   if(highptJet1){
     ptjet1[ih]=edbr->leg2().leg1().pt();
     ptjet2[ih]=edbr->leg2().leg2().pt();
   }
   else{
     ptjet2[ih]=edbr->leg2().leg1().pt();
     ptjet1[ih]=edbr->leg2().leg2().pt();
   }


  }//end analyzeGeneric

  //functions specific for lepton flavors
  void analyzeMuon(cmg::DiMuonDiJetEDBR *edbr, bool highptLeg1);
  void analyzeElectron(cmg::DiElectronDiJetEDBR *edbr, bool highptLeg1);




   //////////////////////////////////////
   // DATA MEMBERS


  edm::InputTag XEEColl_,XEENoKinFitColl_,XEELDMap_;//XEENoKinFitLDMap_;
  edm::InputTag XMMColl_,XMMNoKinFitColl_,XMMLDMap_;//XMMNoKinFitLDMap_;
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
  bool anyPath_,muPath_,elePath_;

  int nCands;
  double hs[nMaxCand], h1[nMaxCand], h2[nMaxCand], phi[nMaxCand], phiS1[nMaxCand], LD[nMaxCand]; // Helicity angles, and LD.
  double mzz[nMaxCand], mzzNoKinFit[nMaxCand], mll[nMaxCand], mjj[nMaxCand], mjjNoKinFit[nMaxCand];           // masses
  double ptmzz[nMaxCand], ptmzzNoKinFit[nMaxCand];
  double ptlep1[nMaxCand], ptlep2[nMaxCand], etalep1[nMaxCand], etalep2[nMaxCand];  // lepton kinematics
  double ptjet1[nMaxCand], ptjet2[nMaxCand], etajet1[nMaxCand], etajet2[nMaxCand];  // jet kinematicse
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
 



  unsigned int nevent,run,ls, njets, nvtx,npu;
  int q1fl[nMaxCand], q2fl[nMaxCand];
  //int jjfl[nMaxCand];
  bool readLDFromUserFloat_,  readQGFromUserFloat_;
  HLTConfigProvider hltConfig;

};//end class AnalyzerEDBR 





void AnalyzerEDBR::init(){

  initTree();



}//end init()


void AnalyzerEDBR::initTree(){

  if(debug_)cout<<"creating the output TTree"<<endl;
  outTree_ = new TTree("SelectedCandidates","angles etc.");
  outTree_->Branch("nCands"          ,&nCands        ,"nCands/I"               );
  outTree_->Branch("cosThetaStar"    ,&hs            ,"cosThetaStar[nCands]/D" );
  outTree_->Branch("cosTheta1"       ,&h1            ,"cosTheta1[nCands]/D"    );
  outTree_->Branch("cosTheta2"       ,&h2            ,"cosTheta2[nCands]/D"    );
  outTree_->Branch("phi"             ,&phi           ,"phi[nCands]/D"          );
  outTree_->Branch("phiStar1"        ,&phiS1         ,"phiStar1[nCands]/D"     );
  outTree_->Branch("ptlep1"          ,&ptlep1        ,"ptlep1[nCands]/D"       );
  outTree_->Branch("ptlep2"          ,&ptlep2        ,"ptlep2[nCands]/D"       );
  outTree_->Branch("ptjet1"          ,&ptjet1        ,"ptjet1[nCands]/D"       );
  outTree_->Branch("ptjet2"          ,&ptjet2        ,"ptjet2[nCands]/D"       );
  outTree_->Branch("ptZll"           ,&ptZll         ,"ptZll[nCands]/D"        );
  outTree_->Branch("ptZjj"           ,&ptZjj         ,"ptZjj[nCands]/D"        );
  outTree_->Branch("yZll"            ,&yZll          ,"yZll[nCands]/D"         );
  outTree_->Branch("yZjj"            ,&yZjj          ,"yZjj[nCands]/D"         );
  outTree_->Branch("phiZll"          ,&phiZll        ,"phiZll[nCands]/D"       );
  outTree_->Branch("phiZjj"          ,&phiZjj        ,"phiZjj[nCands]/D"       );
  outTree_->Branch("etalep1"         ,&etalep1       ,"etalep1[nCands]/D"      );
  outTree_->Branch("etalep2"         ,&etalep2       ,"etalep2[nCands]/D"      );
  outTree_->Branch("etajet1"         ,&etajet1       ,"etajet1[nCands]/D"      );
  outTree_->Branch("etajet2"         ,&etajet2       ,"etajet2[nCands]/D"      );
  outTree_->Branch("lep"             ,&lep           ,"lep/D"                  );
  outTree_->Branch("region"          ,&reg           ,"region[nCands]/D"       );
  outTree_->Branch("mZZ"             ,&mzz           ,"mZZ[nCands]/D"          );
  outTree_->Branch("mZZNoKinFit"     ,&mzzNoKinFit   ,"mZZNoKinFit[nCands]/D"  );
  outTree_->Branch("ptmzz"           ,&ptmzz         ,"ptmzz[nCands]/D"        );
  outTree_->Branch("ptmzzNoKinFit"   ,&ptmzzNoKinFit ,"ptmzzNoKinFit[nCands]/D");
  outTree_->Branch("mLL"             ,&mll           ,"mLL[nCands]/D"          );
  outTree_->Branch("mJJ"             ,&mjj           ,"mJJ[nCands]/D"          );
  outTree_->Branch("mJJNoKinFit"     ,&mjjNoKinFit   ,"mJJNoKinFit[nCands]/D"  );
  outTree_->Branch("met"             ,&met           ,"met/D"                  );
  outTree_->Branch("metSign"         ,&metSign       ,"metSign/D"              );
  outTree_->Branch("nBTags"          ,&btag          ,"nBTags[nCands]/D"       );
  outTree_->Branch("deltaREDBR"     ,&deltaREDBR   ,"deltaREDBR[nCands]/D"  );
  outTree_->Branch("deltaRleplep"    ,&deltaRleplep  ,"deltaRleplep[nCands]/D" );
  outTree_->Branch("deltaRjetjet"    ,&deltaRjetjet  ,"deltaRjetjet[nCands]/D" );
  outTree_->Branch("qgProduct"       ,&qgProduct     ,"qgProduct[nCands]/D"    );
  outTree_->Branch("qgjet1"          ,&qgjet1        ,"qgjet1[nCands]/D"       );
  outTree_->Branch("qgjet2"          ,&qgjet2        ,"qgjet2[nCands]/D"       );
  outTree_->Branch("betajet1"        ,&betajet1      ,"betajet1[nCands]/D"     ); 
  outTree_->Branch("betajet2"        ,&betajet2      ,"betajet2[nCands]/D"     ); 
  outTree_->Branch("puMvajet1"       ,&puMvajet1     ,"puMvajet1[nCands]/D"    ); 
  outTree_->Branch("puMvajet2"       ,&puMvajet2     ,"puMvajet2[nCands]/D"    ); 
  outTree_->Branch("isolep1"         ,&isolep1       ,"isolep1[nCands]/D"      ); 
  outTree_->Branch("isolep2"         ,&isolep2       ,"isolep2[nCands]/D"      ); 
  outTree_->Branch("eleMVAId1"       ,&eleMVAId1     ,"eleMVAId1[nCands]/D"    );
  outTree_->Branch("eleMVAId2"       ,&eleMVAId2     ,"eleMVAId2[nCands]/D"    );
  outTree_->Branch("LD"              ,&LD            ,"LD[nCands]/D"           );
  //  outTree_->Branch("aplanarity"      ,&aplanarity    ,"aplanarity[nCands]/D"   );
  // outTree_->Branch("sphericity"      ,&sphericity    ,"sphericity[nCands]/D"   );
  // outTree_->Branch("centrality"      ,&centrality    ,"centrality[nCands]/D"   );
  outTree_->Branch("q1fl"            ,&q1fl          ,"q1fl[nCands]/I"         );
  outTree_->Branch("q2fl"            ,&q2fl          ,"q2fl[nCands]/I"         );
  //outTree_->Branch("jjfl"            ,&jjfl          ,"jjfl[nCands]/I"         );
  outTree_->Branch("MCmatch"         ,&MCmatch       ,"MCmatch[nCands]/D"      );
  outTree_->Branch("nVtx"            ,&nvtx          ,"nVtx/i"                 );
  outTree_->Branch("nJets"           ,&njets         ,"nJets/i"                );
  outTree_->Branch("nPU"             ,&npu           ,"nPU/i"                  );
  outTree_->Branch("HLTweight"       ,&HLTSF         ,"HLTweight/D"            ); 
  outTree_->Branch("PUweight"        ,&PU            ,"PUweight/D"             ); 
  outTree_->Branch("PUweight2012A"   ,&PUA           ,"PUweight2012A/D"        ); 
  outTree_->Branch("PUweight2012B"   ,&PUB           ,"PUweight2012B/D"        );
  outTree_->Branch("LumiWeight"      ,&lumiw          ,"LumiWeight/D"           );  // For correct lumi scaling
  outTree_->Branch("GenWeight"       ,&genw          ,"GenWeight/D"            );  // Gen level MC weights
  outTree_->Branch("weight"          ,&w             ,"weight/D"               );  // Product of PU and lumi weights
  outTree_->Branch("weight2012A"     ,&wA            ,"weight2012A/D"          );
  outTree_->Branch("weight2012B"     ,&wB            ,"weight2012B/D"          );
  outTree_->Branch("event"           ,&nevent        ,"event/i"                );
  outTree_->Branch("run"             ,&run           ,"run/i"                  );
  outTree_->Branch("ls"              ,&ls            ,"ls/i"                   );

  if(triggerNames_.size()>0){
  if(debug_)cout<<"Adding branches with trigger names"<<endl;
  //flags for telling if the event passed a certain trig path
  char triggerNamePiuI[200];
  for(unsigned int iTrig=0;iTrig<triggerNames_.size();iTrig++) {
    sprintf(triggerNamePiuI,"%s/I",(triggerNames_.at(iTrig)).c_str());
    outTree_->Branch((triggerNames_.at(iTrig)).c_str(),        &triggerRes[iTrig],       triggerNamePiuI);
  }

  }

}//end initTree()

void AnalyzerEDBR::initDataMembers(){

 lep   = -99;
  lumiw  =   1;
  PU    =   1;
  PUA   =   1;
  PUB   =   11;
  genw  =   1.0;
  w     =   0.0;
  wA    =   1;
  wB    =   1;
  HLTSF =   1;

}//end AnalyzeEDBR::initDataMembers()

void AnalyzerEDBR::analyzeTrigger(edm::Event const& iEvent, edm::EventSetup const& eventSetup){

  edm::Handle<edm::TriggerResults> HLTR;
  iEvent.getByLabel(edm::InputTag("TriggerResults","","HLT"), HLTR);

  for(unsigned int iTrig=0;iTrig<triggerNames_.size();iTrig++) {
    int triggerIndex = (int)hltConfig.triggerIndex((triggerNames_.at(iTrig)).c_str()) ;
    triggerRes[iTrig] = 0;
    // triggerIndex must be less than the size of HLTR or you get a CMSException: _M_range_check
    if (triggerIndex < (int)HLTR->size()) triggerRes[iTrig] = (int)HLTR->accept(triggerIndex);
  }

  //check which analysis paths the event passed.
  //this matters as we can ask for some specific things according to the 
  //path (for example electron specific quantitites for cands passing the eejj path)

  preselM_=iEvent.triggerResultsByName("CMG").accept("preselMuPath");
  finalM_=false;// iEvent.triggerResultsByName("CMG").accept("cmgXZZMM");
  sbM_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZMMSideband")!=iEvent.triggerResultsByName("CMG").size() &&     iEvent.triggerResultsByName("CMG").accept("cmgXZZMMSideband");
  preselE_=iEvent.triggerResultsByName("CMG").accept("preselElePath");
  finalE_=false;// iEvent.triggerResultsByName("CMG").accept("cmgXZZEE");
  sbE_=false;//iEvent.triggerResultsByName("CMG").triggerIndex("cmgXZZEESideband")!=iEvent.triggerResultsByName("CMG").size() &&  iEvent.triggerResultsByName("CMG").accept("cmgXZZEESideband");

  muPath_=false;
  elePath_=false;
  anyPath_=false;
  if(preselM_ || finalM_ || sbM_)muPath_=true;
  if(preselE_ || finalE_ || sbE_)elePath_=true;
  if(muPath_ || elePath_) anyPath_=true;

}//end  AnalyzeEDBR::analyzeTrigger()

void AnalyzerEDBR::analyzeMuon(cmg::DiMuonDiJetEDBR *edbr){
  //nothing to be done
  //
  //

  //dummy for muons 
  eleMVAId1[ih] = -1.0;
  eleMVAId2[ih] = -1.0;
}

void AnalyzerEDBR::analyzeElectron(cmg::DiElectronDiJetEDBR *edbr){
  bool highptLep1=true;
  if(edbr->leg1().leg2().pt()>edbr->leg1().leg1().pt())highptLep1=false;
  

  if(highptLeg1){
    eleMVAId1[ih] = edbr->leg1().leg1().mvaTrigV0(); 
    eleMVAId2[ih] = edbr->leg1().leg2().mvaTrigV0();
  } 
  else{
    eleMVAId1[ih] = edbr->leg1().leg2().mvaTrigV0(); 
    eleMVAId2[ih] = edbr->leg1().leg1().mvaTrigV0();
  }
}

#endif
