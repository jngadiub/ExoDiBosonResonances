#include <map>
#include <string>

#include "TH1D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
// Sadly, CINT will not allow these large arrays
// to be part of the class... possibly if I change
// them to be std::vectors, eventually?

const std::string vars[77] = 
  {"nCands", "cosThetaStar", "cosTheta1", "cosTheta2", "phi", "phiStar1", "ptlep1",
   "ptlep2", "ptjet1", "ptjet2", "ptZll", "ptZjj", "yZll", "yZjj",
   "phiZll", "phiZjj", "etalep1", "etalep2", "etajet1", "etajet2", "philep1",
   "philep2", "phijet1", "phijet2", "lep", "region", "mZZ", "mZZNoKinFit",
   "ptmzz", "ptmzzNoKinFit", "mLL", "mJJ", "mJJNoKinFit", "met", "metSign",
   "nBTags", "deltaREDBR", "deltaRleplep", "deltaRjetjet", "qgProduct", "qgjet1", "qgjet2",
   "betajet1", "betajet2", "puMvajet1", "puMvajet2", "nXjets", "prunedmass", "mdrop",
   "nsubj12", "nsubj23", "tau1", "tau2", "qjet", "isolep1", "isolep2",
   "eleMVAId1", "eleMVAId2", "LD", "q1fl", "q2fl", "MCmatch", "nVtx",
   "nJets", "nPU", "HLTweight", "PUweight", "PUweight2012A", "PUweight2012B", "LumiWeight",
   "GenWeight", "weight", "weight2012A", "weight2012B", "event", "run", "ls"};

const int nBins[77] = 
  {13, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 3, 100, 100,
   100, 100, 100, 100, 100, 100, 100,
   100, 100, 100, 4, 4, 100, 43,
   7, 2, 100, 100, 100, 100, 100,
   100, 100, 100, 100, 102, 3, 102};

const double minBin[77] = 
  {0.0,-1.15, -1.15, -84., -3.7, -3.7, 34.,
   15., 0., -120., 44., 0., -2.8, -2.7,
   -3.7, -3.7, -2.8, -2.8, -2.8, -108., -3.7,
   -3.7, 0., 0., 0., 0., 100., 100.,
   0., 0., 66.5, 47., 43., 0., -2.2,
   -2.2, -1.2, 0.96, 0., -100.2, -100.2, -100.2,
   0., -1.2, -10800., -10800., 0., -115., -1080.,
   -1080., -1080., -1080., -1080., -1080., 0., 0.,
   -1.15, -1.15, -100.2, -101., -101., -1.2, 0.,
   1., 0., 0.99, 0., 0., 0., -1.15,
   0., 0., 0., 0., 2490000., 0., 6280.};

const double maxBin[77] = 
  {13.0,1.15, 1.15, 8., 3.7, 3.7, 131.,
   81., 365., 140., 154., 370., 2.8, 2.8,
   3.7, 3.7, 2.9, 2.9, 2.8, 12., 3.7,
   3.7, 1., 1., 1.1, 1.0, 1180., 1160.,
   320., 330., 113.0, 96., 137., 102., 0.,
   0., 1.2, 3.34, 4.35, -97.8, -97.8, -97.8,
   1.1, 1.2, 1000., 1000., 3., 110., 100.,
   100., 100., 100., 100., 100., 1.4, 5.4,
   1.15, 1.15, -97.8, -97., -97., 1.2, 43.,
   8., 1., 1., 16., 140., 16., 1.2,
   2.2, 0.02, 0.14, 0.02, 54510000., 3., 181720.};

/// EDBRHistoMaker is the class that analyzes the flat
/// TTree that comes out from the NTuple dumper module.
/// It doesn't make analysis; it just makes plots.
/// There are a few switches to do different plots:
/// SIGNAL - BACKGROUND,
/// MUON - ELECTRON, etc...

class EDBRHistoMaker {
public:
  EDBRHistoMaker(TTree *tree=0, 
		 bool wantElectrons=true,
		 bool wantMuons=true,
		 bool wantSideband=true, 
		 bool wantSignal=false,
		 int wantNXJets=1);
  virtual ~EDBRHistoMaker();
  
  /// This is the tree structure. This comes directly from MakeClass

  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nCands;
   Double_t        cosThetaStar[99];   //[nCands]
   Double_t        cosTheta1[99];   //[nCands]
   Double_t        cosTheta2[99];   //[nCands]
   Double_t        phi[99];   //[nCands]
   Double_t        phiStar1[99];   //[nCands]
   Double_t        ptlep1[99];   //[nCands]
   Double_t        ptlep2[99];   //[nCands]
   Double_t        ptjet1[99];   //[nCands]
   Double_t        ptjet2[99];   //[nCands]
   Double_t        ptZll[99];   //[nCands]
   Double_t        ptZjj[99];   //[nCands]
   Double_t        yZll[99];   //[nCands]
   Double_t        yZjj[99];   //[nCands]
   Double_t        phiZll[99];   //[nCands]
   Double_t        phiZjj[99];   //[nCands]
   Double_t        etalep1[99];   //[nCands]
   Double_t        etalep2[99];   //[nCands]
   Double_t        etajet1[99];   //[nCands]
   Double_t        etajet2[99];   //[nCands]
   Double_t        philep1[99];   //[nCands]
   Double_t        philep2[99];   //[nCands]
   Double_t        phijet1[99];   //[nCands]
   Double_t        phijet2[99];   //[nCands]
   Double_t        lep;
   Double_t        region[99];   //[nCands]
   Double_t        mZZ[99];   //[nCands]
   Double_t        mZZNoKinFit[99];   //[nCands]
   Double_t        ptmzz[99];   //[nCands]
   Double_t        ptmzzNoKinFit[99];   //[nCands]
   Double_t        mLL[99];   //[nCands]
   Double_t        mJJ[99];   //[nCands]
   Double_t        mJJNoKinFit[99];   //[nCands]
   Double_t        met;
   Double_t        metSign;
   Double_t        nBTags[99];   //[nCands]
   Double_t        deltaREDBR[99];   //[nCands]
   Double_t        deltaRleplep[99];   //[nCands]
   Double_t        deltaRjetjet[99];   //[nCands]
   Double_t        qgProduct[99];   //[nCands]
   Double_t        qgjet1[99];   //[nCands]
   Double_t        qgjet2[99];   //[nCands]
   Double_t        betajet1[99];   //[nCands]
   Double_t        betajet2[99];   //[nCands]
   Double_t        puMvajet1[99];   //[nCands]
   Double_t        puMvajet2[99];   //[nCands]
   Int_t           nXjets[99];   //[nCands]
   Double_t        prunedmass[99];   //[nCands]
   Double_t        mdrop[99];   //[nCands]
   Double_t        nsubj12[99];   //[nCands]
   Double_t        nsubj23[99];   //[nCands]
   Double_t        tau1[99];   //[nCands]
   Double_t        tau2[99];   //[nCands]
   Double_t        qjet[99];   //[nCands]
   Double_t        isolep1[99];   //[nCands]
   Double_t        isolep2[99];   //[nCands]
   Double_t        eleMVAId1[99];   //[nCands]
   Double_t        eleMVAId2[99];   //[nCands]
   Double_t        LD[99];   //[nCands]
   Int_t           q1fl[99];   //[nCands]
   Int_t           q2fl[99];   //[nCands]
   Double_t        MCmatch[99];   //[nCands]
   UInt_t          nVtx;
   UInt_t          nJets;
   UInt_t          nPU;
   Double_t        HLTweight;
   Double_t        PUweight;
   Double_t        PUweight2012A;
   Double_t        PUweight2012B;
   Double_t        LumiWeight;
   Double_t        GenWeight;
   Double_t        weight;
   Double_t        weight2012A;
   Double_t        weight2012B;
   UInt_t          event;
   UInt_t          run;
   UInt_t          ls;

   // List of branches
   TBranch        *b_nCands;   //!
   TBranch        *b_cosThetaStar;   //!
   TBranch        *b_cosTheta1;   //!
   TBranch        *b_cosTheta2;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_phiStar1;   //!
   TBranch        *b_ptlep1;   //!
   TBranch        *b_ptlep2;   //!
   TBranch        *b_ptjet1;   //!
   TBranch        *b_ptjet2;   //!
   TBranch        *b_ptZll;   //!
   TBranch        *b_ptZjj;   //!
   TBranch        *b_yZll;   //!
   TBranch        *b_yZjj;   //!
   TBranch        *b_phiZll;   //!
   TBranch        *b_phiZjj;   //!
   TBranch        *b_etalep1;   //!
   TBranch        *b_etalep2;   //!
   TBranch        *b_etajet1;   //!
   TBranch        *b_etajet2;   //!
   TBranch        *b_philep1;   //!
   TBranch        *b_philep2;   //!
   TBranch        *b_phijet1;   //!
   TBranch        *b_phijet2;   //!
   TBranch        *b_lep;   //!
   TBranch        *b_region;   //!
   TBranch        *b_mZZ;   //!
   TBranch        *b_mZZNoKinFit;   //!
   TBranch        *b_ptmzz;   //!
   TBranch        *b_ptmzzNoKinFit;   //!
   TBranch        *b_mLL;   //!
   TBranch        *b_mJJ;   //!
   TBranch        *b_mJJNoKinFit;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metSign;   //!
   TBranch        *b_nBTags;   //!
   TBranch        *b_deltaREDBR;   //!
   TBranch        *b_deltaRleplep;   //!
   TBranch        *b_deltaRjetjet;   //!
   TBranch        *b_qgProduct;   //!
   TBranch        *b_qgjet1;   //!
   TBranch        *b_qgjet2;   //!
   TBranch        *b_betajet1;   //!
   TBranch        *b_betajet2;   //!
   TBranch        *b_puMvajet1;   //!
   TBranch        *b_puMvajet2;   //!
   TBranch        *b_nXjets;   //!
   TBranch        *b_prunedmass;   //!
   TBranch        *b_mdrop;   //!
   TBranch        *b_nsubj12;   //!
   TBranch        *b_nsubj23;   //!
   TBranch        *b_tau1;   //!
   TBranch        *b_tau2;   //!
   TBranch        *b_qjet;   //!
   TBranch        *b_isolep1;   //!
   TBranch        *b_isolep2;   //!
   TBranch        *b_eleMVAId1;   //!
   TBranch        *b_eleMVAId2;   //!
   TBranch        *b_LD;   //!
   TBranch        *b_q1fl;   //!
   TBranch        *b_q2fl;   //!
   TBranch        *b_MCmatch;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_nPU;   //!
   TBranch        *b_HLTweight;   //!
   TBranch        *b_PUweight;   //!
   TBranch        *b_PUweight2012A;   //!
   TBranch        *b_PUweight2012B;   //!
   TBranch        *b_LumiWeight;   //!
   TBranch        *b_GenWeight;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_weight2012A;   //!
   TBranch        *b_weight2012B;   //!
   TBranch        *b_event;   //!
   TBranch        *b_run;   //!
   TBranch        *b_ls;   //!

  // Basic functions directly from MakeClass
  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Loop(std::string outFileName);
  
  // Our added functions
  void createAllHistos();
  void printAllHistos();
  void saveAllHistos(std::string outFileName);

  void setWantElectrons(bool doele=false){wantElectrons_=doele;}
  void setWantMuons(bool domu=false){wantMuons_=domu;}
  void setWantSideband(bool dosb=false){wantSideband_=dosb;}
  void setWantSignal(bool dosig=false){wantSignal_=dosig;}
  void setWantNXJets(int nxj=1){wantNXJets_=nxj;}

  bool eventPassesFlavorCut();
  bool eventInSidebandRegion();
  bool eventInSignalRegion();
  bool eventPassesRegionCut();
  bool eventPassesNXJetCut();  
  bool eventPassesCut();

  // Our added variables
  int nVars;
  bool wantElectrons_;
  bool wantMuons_;
  bool wantSideband_;
  bool wantSignal_;
  int wantNXJets_;
  double sidebandVHMassLow_;
  double sidebandVHMassHigh_;
  double signalVHMassLow_;
  double signalVHMassHigh_;

  // The histograms
  std::map<std::string,TH1D*> theHistograms;
};

void EDBRHistoMaker::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nCands", &nCands, &b_nCands);
   fChain->SetBranchAddress("cosThetaStar", cosThetaStar, &b_cosThetaStar);
   fChain->SetBranchAddress("cosTheta1", cosTheta1, &b_cosTheta1);
   fChain->SetBranchAddress("cosTheta2", cosTheta2, &b_cosTheta2);
   fChain->SetBranchAddress("phi", phi, &b_phi);
   fChain->SetBranchAddress("phiStar1", phiStar1, &b_phiStar1);
   fChain->SetBranchAddress("ptlep1", ptlep1, &b_ptlep1);
   fChain->SetBranchAddress("ptlep2", ptlep2, &b_ptlep2);
   fChain->SetBranchAddress("ptjet1", ptjet1, &b_ptjet1);
   fChain->SetBranchAddress("ptjet2", ptjet2, &b_ptjet2);
   fChain->SetBranchAddress("ptZll", ptZll, &b_ptZll);
   fChain->SetBranchAddress("ptZjj", ptZjj, &b_ptZjj);
   fChain->SetBranchAddress("yZll", yZll, &b_yZll);
   fChain->SetBranchAddress("yZjj", yZjj, &b_yZjj);
   fChain->SetBranchAddress("phiZll", phiZll, &b_phiZll);
   fChain->SetBranchAddress("phiZjj", phiZjj, &b_phiZjj);
   fChain->SetBranchAddress("etalep1", etalep1, &b_etalep1);
   fChain->SetBranchAddress("etalep2", etalep2, &b_etalep2);
   fChain->SetBranchAddress("etajet1", etajet1, &b_etajet1);
   fChain->SetBranchAddress("etajet2", etajet2, &b_etajet2);
   fChain->SetBranchAddress("philep1", philep1, &b_philep1);
   fChain->SetBranchAddress("philep2", philep2, &b_philep2);
   fChain->SetBranchAddress("phijet1", phijet1, &b_phijet1);
   fChain->SetBranchAddress("phijet2", phijet2, &b_phijet2);
   fChain->SetBranchAddress("lep", &lep, &b_lep);
   fChain->SetBranchAddress("region", region, &b_region);
   fChain->SetBranchAddress("mZZ", mZZ, &b_mZZ);
   fChain->SetBranchAddress("mZZNoKinFit", mZZNoKinFit, &b_mZZNoKinFit);
   fChain->SetBranchAddress("ptmzz", ptmzz, &b_ptmzz);
   fChain->SetBranchAddress("ptmzzNoKinFit", ptmzzNoKinFit, &b_ptmzzNoKinFit);
   fChain->SetBranchAddress("mLL", mLL, &b_mLL);
   fChain->SetBranchAddress("mJJ", mJJ, &b_mJJ);
   fChain->SetBranchAddress("mJJNoKinFit", mJJNoKinFit, &b_mJJNoKinFit);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metSign", &metSign, &b_metSign);
   fChain->SetBranchAddress("nBTags", nBTags, &b_nBTags);
   fChain->SetBranchAddress("deltaREDBR", deltaREDBR, &b_deltaREDBR);
   fChain->SetBranchAddress("deltaRleplep", deltaRleplep, &b_deltaRleplep);
   fChain->SetBranchAddress("deltaRjetjet", deltaRjetjet, &b_deltaRjetjet);
   fChain->SetBranchAddress("qgProduct", qgProduct, &b_qgProduct);
   fChain->SetBranchAddress("qgjet1", qgjet1, &b_qgjet1);
   fChain->SetBranchAddress("qgjet2", qgjet2, &b_qgjet2);
   fChain->SetBranchAddress("betajet1", betajet1, &b_betajet1);
   fChain->SetBranchAddress("betajet2", betajet2, &b_betajet2);
   fChain->SetBranchAddress("puMvajet1", puMvajet1, &b_puMvajet1);
   fChain->SetBranchAddress("puMvajet2", puMvajet2, &b_puMvajet2);
   fChain->SetBranchAddress("nXjets", nXjets, &b_nXjets);
   fChain->SetBranchAddress("prunedmass", prunedmass, &b_prunedmass);
   fChain->SetBranchAddress("mdrop", mdrop, &b_mdrop);
   fChain->SetBranchAddress("nsubj12", nsubj12, &b_nsubj12);
   fChain->SetBranchAddress("nsubj23", nsubj23, &b_nsubj23);
   fChain->SetBranchAddress("tau1", tau1, &b_tau1);
   fChain->SetBranchAddress("tau2", tau2, &b_tau2);
   fChain->SetBranchAddress("qjet", qjet, &b_qjet);
   fChain->SetBranchAddress("isolep1", isolep1, &b_isolep1);
   fChain->SetBranchAddress("isolep2", isolep2, &b_isolep2);
   fChain->SetBranchAddress("eleMVAId1", eleMVAId1, &b_eleMVAId1);
   fChain->SetBranchAddress("eleMVAId2", eleMVAId2, &b_eleMVAId2);
   fChain->SetBranchAddress("LD", LD, &b_LD);
   fChain->SetBranchAddress("q1fl", q1fl, &b_q1fl);
   fChain->SetBranchAddress("q2fl", q2fl, &b_q2fl);
   fChain->SetBranchAddress("MCmatch", MCmatch, &b_MCmatch);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("nPU", &nPU, &b_nPU);
   fChain->SetBranchAddress("HLTweight", &HLTweight, &b_HLTweight);
   fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
   fChain->SetBranchAddress("PUweight2012A", &PUweight2012A, &b_PUweight2012A);
   fChain->SetBranchAddress("PUweight2012B", &PUweight2012B, &b_PUweight2012B);
   fChain->SetBranchAddress("LumiWeight", &LumiWeight, &b_LumiWeight);
   fChain->SetBranchAddress("GenWeight", &GenWeight, &b_GenWeight);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("weight2012A", &weight2012A, &b_weight2012A);
   fChain->SetBranchAddress("weight2012B", &weight2012B, &b_weight2012B);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
}
