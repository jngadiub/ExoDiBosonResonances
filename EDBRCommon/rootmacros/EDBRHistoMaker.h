#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"


/// The large arrays that were here are now GONE.
/// Instead, we have this helper that holds the
/// information of all our histograms.

class HistoFactory{
 public:
  std::vector<std::string> vars;
  std::vector<int> nBins;
  std::vector<double> minBin;
  std::vector<double> maxBin;
  void setHisto(std::string s, int n, double min, double max) {
    vars.push_back(s);
    nBins.push_back(n);
    minBin.push_back(min);
    maxBin.push_back(max);
  }
};

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
		 bool wantFullRange=false,
		 int  wantNXJets=1,
		 bool isZZchannel=1);
  virtual ~EDBRHistoMaker();

  /// This is the tree structure. This comes directly from MakeClass
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Int_t           nCands;
  UInt_t          event;
  UInt_t          run;
  UInt_t          ls;
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
  Double_t        lep[99];       //[nCands]
  Double_t        region[99];   //[nCands]
  Int_t           nXjets[99];   //[nCands]
  Double_t        vTagPurity[99];       //[nCands]
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
  Double_t        prunedmass[99];   //[nCands]
  Double_t        mdrop[99];   //[nCands]
  Double_t        nsubj21[99];   //[nCands]
  Double_t        nsubj32[99];   //[nCands]
  Double_t        tau1[99];   //[nCands]
  Double_t        tau2[99];   //[nCands]
  Double_t        qjet[99];   //[nCands]
  Double_t        isolep1[99];   //[nCands]
  Double_t        isolep2[99];   //[nCands]
  Double_t        isomu1mod[99];   //[nCands]
  Double_t        isomu2mod[99];   //[nCands]
  Double_t        isoele1trk[99];   //[nCands]
  Double_t        isoele2trk[99];   //[nCands]
  Double_t        isoele1calo[99];   //[nCands]
  Double_t        isoele2calo[99];   //[nCands]
  Double_t        eleMVAId1[99];   //[nCands]
  Double_t        eleMVAId2[99];   //[nCands]
  Double_t        LD[99];   //[nCands]
  Int_t           q1fl[99];   //[nCands]
  Int_t           q2fl[99];   //[nCands]
  Double_t        MCmatch[99];   //[nCands]
  UInt_t          nVtx;
  UInt_t          nJets;
  UInt_t          nAK5jets;
  UInt_t          nPU;
  Double_t        HLTweight;
  Double_t        BTagWeight;
  Double_t        PUweight;
  Double_t        PUweight2012A;
  Double_t        PUweight2012B;
  Double_t        LumiWeight;
  Double_t        GenWeight;
  Double_t        weight;
  Double_t        weight2012A;
  Double_t        weight2012B;
  Int_t           VBFTag[99];   //[nCands]
  Double_t        VBFmJJ[99];   //[nCands]
  Double_t        VBFdeltaEta[99];   //[nCands]
  Double_t        VBFptjet1[99];   //[nCands]
  Double_t        VBFptjet2[99];   //[nCands]
  Double_t        VBFetajet1[99];   //[nCands]
  Double_t        VBFetajet2[99];   //[nCands]
  Double_t        VBFphijet1[99];   //[nCands]
  Double_t        VBFphijet2[99];   //[nCands]
  Double_t        massGenX;
  Double_t        ptGenX;
  Double_t        yGenX;
  Double_t        phiGenX;
  Int_t           pdgIdGenX;
  Double_t        massGenVll;
  Double_t        ptGenVll;
  Double_t        yGenVll;
  Double_t        phiGenVll;
  Double_t        massGenVqq;
  Double_t        ptGenVqq;
  Double_t        yGenVqq;
  Double_t        phiGenVqq;
  Int_t           nLooseMu;
  Int_t           nLooseEle;
  Double_t        mt[99];      //[nCands]
  Double_t        nbtagsL[99];  //[nCands]
  Double_t        nbtagsM[99];  //[nCands]
  Double_t        nbtagsT[99];  //[nCands]
  Double_t        nbtagscleanL[99];  //[nCands]
  Double_t        nbtagscleanM[99];  //[nCands]
  Double_t        nbtagscleanT[99];  //[nCands]
  Double_t        eleid_sigmaIetaIeta[99];  //[nCands] 
  Double_t        eleid_deltaPhiSuperClusterTrackAtVtx[99];  //[nCands] 
  Double_t        eleid_deltaEtaSuperClusterTrackAtVtx[99];  //[nCands] 
  Double_t        eleid_hadronicOverEm[99];  //[nCands] 
  Double_t        eleid_numberOfHits[99];  //[nCands] 
  Double_t        eleid_dxy[99];  //[nCands] 
  Double_t        eleid_dz[99];  //[nCands] 
  Double_t        eleid_ecalDriven[99];  //[nCands]
  Double_t        eleid_convDist[99];  //[nCands]
  Double_t        eleid_convDcot[99];  //[nCands]
  Double_t        eleid_isConv[99];  //[nCands]
  Double_t        eleid_passConversionVeto[99];  //[nCands]
  Double_t        eleid_numberOfLostHits[99];  //[nCands]  
  Double_t        eleid_e1x5[99];  //[nCands] 
  Double_t        eleid_e2x5Max[99];  //[nCands] 
  Double_t        eleid_e5x5[99];  //[nCands] 
  Double_t        eleid_e1x5Over5x5[99];  //[nCands] 
  Double_t        eleid_e2x5MaxOver5x5[99];  //[nCands]
  Double_t        mZZ_type0[99]; //[nCands]
  Double_t        mZZ_type1[99]; //[nCands]
  Double_t        mZZ_type2[99]; //[nCands]
  Double_t        mZZ_type3[99]; //[nCands]
  Double_t        mZZ_type4[99]; //[nCands]
  Double_t		mZZ_type0_ptUncorrected[99]; //[nCands]
  Double_t        mZZ_type1_ptUncorrected[99]; //[nCands]
  Double_t        mZZ_type2_ptUncorrected[99]; //[nCands]
  Double_t        mZZ_type3_ptUncorrected[99]; //[nCands]
  Double_t        mZZ_type4_ptUncorrected[99]; //[nCands]
  Double_t		pz_type0[99]; //[nCands]
  Double_t        pz_type1[99]; //[nCands]
  Double_t        pz_type2[99]; //[nCands]
  Double_t        pz_type3[99]; //[nCands]
  Double_t        pz_type4[99]; //[nCands]
  Double_t        pt_neutrino[99]; //[nCands]
  Double_t        pt_neutrino_corrected[99]; //[nCands]
  Double_t        TOBTECjetsId[99];//[nCands]

  // List of branches
  TBranch        *b_nCands;   //!
  TBranch        *b_event;   //!
  TBranch        *b_run;   //!
  TBranch        *b_ls;   //!
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
  TBranch        *b_nXjets;   //!
  TBranch        *b_vTagPurity;   //!
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
  TBranch        *b_prunedmass;   //!
  TBranch        *b_mdrop;   //!
  TBranch        *b_nsubj21;   //!
  TBranch        *b_nsubj32;   //!
  TBranch        *b_tau1;   //!
  TBranch        *b_tau2;   //!
  TBranch        *b_qjet;   //!
  TBranch        *b_isolep1;   //!
  TBranch        *b_isolep2;   //!
  TBranch        *b_isomu1mod;   //!
  TBranch        *b_isomu2mod;   //!
  TBranch        *b_isoele1trk;   //!
  TBranch        *b_isoele2trk;   //!
  TBranch        *b_isoele1calo;   //!
  TBranch        *b_isoele2calo;   //!
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
  TBranch        *b_VBFTag;   //!
  TBranch        *b_VBFmJJ;   //!
  TBranch        *b_VBFdeltaEta;   //!
  TBranch        *b_VBFptjet1;   //!
  TBranch        *b_VBFptjet2;   //!
  TBranch        *b_VBFetajet1;   //!
  TBranch        *b_VBFetajet2;   //!
  TBranch        *b_VBFphijet1;   //!
  TBranch        *b_VBFphijet2;   //!
  TBranch        *b_massGenX;   //!
  TBranch        *b_ptGenX;   //!
  TBranch        *b_yGenX;   //!
  TBranch        *b_phiGenX;   //!
  TBranch        *b_pdgIdGenX;   //!
  TBranch        *b_massGenVll;   //!
  TBranch        *b_ptGenVll;   //!
  TBranch        *b_yGenVll;   //!
  TBranch        *b_phiGenVll;   //!
  TBranch        *b_massGenVqq;   //!
  TBranch        *b_ptGenVqq;   //!
  TBranch        *b_yGenVqq;   //!
  TBranch        *b_phiGenVqq;   //!
  TBranch        *b_nLooseMu;   //!
  TBranch        *b_nLooseEle;   //!
  TBranch        *b_mt;   //!

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
  void setUnitaryWeights(bool setuniw=false){setUnitaryWeights_=setuniw;}

  bool eventPassesFlavorCut(int i);
  bool eventPassesLeptonicZPtCut(int i, double ptZll_threshold);
  bool eventPassesLep1PtCut(int i, double ptlep1_threshold);
  bool eventInSidebandRegion(int i);
  bool eventInSignalRegion(int i);
  bool eventPassesRegionCut(int i);
  bool eventPassesNXJetCut(int i);  
  bool eventPassesCut(int i, double ptZll_threshold, double ptlep1_threshold );
  bool eventPassesVBFCut(int i);

  double deltaPhi(const double& phi1, const double& phi2)
  { 
    double deltaphi = fabs(phi1 - phi2);
    if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
    return deltaphi;
  }

  //  ------------------------------------------------------------

  double deltaEta(const double& eta1, const double& eta2)
  { 
    double deltaeta = fabs(eta1 - eta2);
    return deltaeta;
  }

  //  ------------------------------------------------------------

  double deltaR(const double& eta1, const double& phi1,
		const double& eta2, const double& phi2)
  { 
    double deltaphi = deltaPhi(phi1, phi2);
    double deltaeta = deltaEta(eta1, eta2);
    double deltar = sqrt(deltaphi*deltaphi + deltaeta*deltaeta);
    return deltar;
  }

  int check ( double pt, vector<double> * ptZ  )
  {
    int goodw=1;
    for(unsigned int i =0; i< ptZ->size(); i++)
      {   
	//printf("Comparing %g and %g\n",pt,ptZ->at(i));
	if(pt==ptZ->at(i)) { goodw=0; break;}
	//else {printf("I think they're different\n");}
      }   

    return goodw;
  }

  // Our added variables
  int nVars;
  bool wantElectrons_;
  bool wantMuons_;
  bool wantSideband_;
  bool wantSignal_;
  bool wantFullRange_;
  bool setUnitaryWeights_;
  bool debug_;
  int wantNXJets_;
  double sidebandVHMassLow_;
  double sidebandVHMassHigh_;
  double signalVHMassLow_;
  double signalVHMassHigh_;
  bool isZZchannel_;

  // The histograms
  HistoFactory hs;
  std::map<std::string,TH1D*> theHistograms;
  TH2D *hmjmzz; 
  TH1D *hmzzNEW;
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
  fChain->SetBranchAddress("event", &event, &b_event);
  fChain->SetBranchAddress("run", &run, &b_run);
  fChain->SetBranchAddress("ls", &ls, &b_ls);
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
  fChain->SetBranchAddress("lep", lep, &b_lep);
  fChain->SetBranchAddress("region", region, &b_region);
  fChain->SetBranchAddress("nXjets", nXjets, &b_nXjets);
  fChain->SetBranchAddress("vTagPurity", vTagPurity, &b_vTagPurity);
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
  fChain->SetBranchAddress("prunedmass", prunedmass, &b_prunedmass);
  fChain->SetBranchAddress("mdrop", mdrop, &b_mdrop);
  fChain->SetBranchAddress("nsubj21", nsubj21, &b_nsubj21);
  fChain->SetBranchAddress("nsubj32", nsubj32, &b_nsubj32);
  fChain->SetBranchAddress("tau1", tau1, &b_tau1);
  fChain->SetBranchAddress("tau2", tau2, &b_tau2);
  fChain->SetBranchAddress("qjet", qjet, &b_qjet);
  fChain->SetBranchAddress("isolep1", isolep1, &b_isolep1);
  fChain->SetBranchAddress("isolep2", isolep2, &b_isolep2);
  fChain->SetBranchAddress("isomu1mod", isomu1mod, &b_isomu1mod);
  fChain->SetBranchAddress("isomu2mod", isomu2mod, &b_isomu2mod);
  fChain->SetBranchAddress("isoele1trk", isoele1trk, &b_isoele1trk);
  fChain->SetBranchAddress("isoele2trk", isoele2trk, &b_isoele2trk);
  fChain->SetBranchAddress("isoele1calo", isoele1calo, &b_isoele1calo);
  fChain->SetBranchAddress("isoele2calo", isoele2calo, &b_isoele2calo);
  fChain->SetBranchAddress("eleMVAId1", eleMVAId1, &b_eleMVAId1);
  fChain->SetBranchAddress("eleMVAId2", eleMVAId2, &b_eleMVAId2);
  fChain->SetBranchAddress("LD", LD, &b_LD);
  fChain->SetBranchAddress("q1fl", q1fl, &b_q1fl);
  fChain->SetBranchAddress("q2fl", q2fl, &b_q2fl);
  fChain->SetBranchAddress("MCmatch", MCmatch, &b_MCmatch);
  fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
  fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
  fChain->SetBranchAddress("nAK5jets", &nAK5jets);
  fChain->SetBranchAddress("nPU", &nPU, &b_nPU);
  fChain->SetBranchAddress("HLTweight", &HLTweight, &b_HLTweight);
  fChain->SetBranchAddress("BTagWeight", &BTagWeight);
  fChain->SetBranchAddress("PUweight", &PUweight, &b_PUweight);
  fChain->SetBranchAddress("PUweight2012A", &PUweight2012A, &b_PUweight2012A);
  fChain->SetBranchAddress("PUweight2012B", &PUweight2012B, &b_PUweight2012B);
  fChain->SetBranchAddress("LumiWeight", &LumiWeight, &b_LumiWeight);
  fChain->SetBranchAddress("GenWeight", &GenWeight, &b_GenWeight);
  fChain->SetBranchAddress("weight", &weight, &b_weight);
  fChain->SetBranchAddress("weight2012A", &weight2012A, &b_weight2012A);
  fChain->SetBranchAddress("weight2012B", &weight2012B, &b_weight2012B);
  fChain->SetBranchAddress("VBFTag", VBFTag, &b_VBFTag);
  fChain->SetBranchAddress("VBFmJJ", VBFmJJ, &b_VBFmJJ);
  fChain->SetBranchAddress("VBFdeltaEta", VBFdeltaEta, &b_VBFdeltaEta);
  fChain->SetBranchAddress("VBFptjet1", VBFptjet1, &b_VBFptjet1);
  fChain->SetBranchAddress("VBFptjet2", VBFptjet2, &b_VBFptjet2);
  fChain->SetBranchAddress("VBFetajet1", VBFetajet1, &b_VBFetajet1);
  fChain->SetBranchAddress("VBFetajet2", VBFetajet2, &b_VBFetajet2);
  fChain->SetBranchAddress("VBFphijet1", VBFphijet1, &b_VBFphijet1);
  fChain->SetBranchAddress("VBFphijet2", VBFphijet2, &b_VBFphijet2);
  fChain->SetBranchAddress("massGenX", &massGenX, &b_massGenX);
  fChain->SetBranchAddress("ptGenX", &ptGenX, &b_ptGenX);
  fChain->SetBranchAddress("yGenX", &yGenX, &b_yGenX);
  fChain->SetBranchAddress("phiGenX", &phiGenX, &b_phiGenX);
  fChain->SetBranchAddress("pdgIdGenX", &pdgIdGenX, &b_pdgIdGenX);
  fChain->SetBranchAddress("massGenVll", &massGenVll, &b_massGenVll);
  fChain->SetBranchAddress("ptGenVll", &ptGenVll, &b_ptGenVll);
  fChain->SetBranchAddress("yGenVll", &yGenVll, &b_yGenVll);
  fChain->SetBranchAddress("phiGenVll", &phiGenVll, &b_phiGenVll);
  fChain->SetBranchAddress("massGenVqq", &massGenVqq, &b_massGenVqq);
  fChain->SetBranchAddress("ptGenVqq", &ptGenVqq, &b_ptGenVqq);
  fChain->SetBranchAddress("yGenVqq", &yGenVqq, &b_yGenVqq);
  fChain->SetBranchAddress("phiGenVqq", &phiGenVqq, &b_phiGenVqq);
  fChain->SetBranchAddress("nLooseMu", &nLooseMu, &b_nLooseMu);
  fChain->SetBranchAddress("nLooseEle", &nLooseEle, &b_nLooseEle);
  fChain->SetBranchAddress("mt", mt, &b_mt);
  fChain->SetBranchAddress("nbtagsL",nbtagsL);
  fChain->SetBranchAddress("nbtagsM",nbtagsM);
  fChain->SetBranchAddress("nbtagsT",nbtagsT);
  fChain->SetBranchAddress("nbtagscleanL",nbtagscleanL);
  fChain->SetBranchAddress("nbtagscleanM",nbtagscleanM);
  fChain->SetBranchAddress("nbtagscleanT",nbtagscleanT);

  fChain->SetBranchAddress("eleid_sigmaIetaIeta",eleid_sigmaIetaIeta);
  fChain->SetBranchAddress("eleid_deltaPhiSuperClusterTrackAtVtx",eleid_deltaPhiSuperClusterTrackAtVtx);
  fChain->SetBranchAddress("eleid_deltaEtaSuperClusterTrackAtVtx",eleid_deltaEtaSuperClusterTrackAtVtx);
  fChain->SetBranchAddress("eleid_hadronicOverEm",eleid_hadronicOverEm);
  fChain->SetBranchAddress("eleid_dxy",eleid_dxy);
  fChain->SetBranchAddress("eleid_dz",eleid_dz);
  fChain->SetBranchAddress("eleid_numberOfHits",eleid_numberOfHits);
  fChain->SetBranchAddress("eleid_ecalDriven",eleid_ecalDriven);
  fChain->SetBranchAddress("eleid_convDist",eleid_convDist);
  fChain->SetBranchAddress("eleid_convDcot",eleid_convDcot);
  fChain->SetBranchAddress("eleid_isConv",eleid_isConv);
  fChain->SetBranchAddress("eleid_passConversionVeto",eleid_passConversionVeto);
  fChain->SetBranchAddress("eleid_numberOfLostHits",eleid_numberOfLostHits);
  fChain->SetBranchAddress("eleid_e1x5",eleid_e1x5);
  fChain->SetBranchAddress("eleid_e2x5Max",eleid_e2x5Max);
  fChain->SetBranchAddress("eleid_e5x5",eleid_e5x5);
  fChain->SetBranchAddress("eleid_e1x5Over5x5",eleid_e1x5Over5x5);
  fChain->SetBranchAddress("eleid_e2x5MaxOver5x5",eleid_e2x5MaxOver5x5);

  fChain->SetBranchAddress("mZZ_type0",mZZ_type0);
  fChain->SetBranchAddress("mZZ_type1",mZZ_type1);
  fChain->SetBranchAddress("mZZ_type2",mZZ_type2);
  fChain->SetBranchAddress("mZZ_type3",mZZ_type3);
  fChain->SetBranchAddress("mZZ_type4",mZZ_type4);
  fChain->SetBranchAddress("mZZ_type0_ptUncorrected",mZZ_type0_ptUncorrected);
  fChain->SetBranchAddress("mZZ_type1_ptUncorrected",mZZ_type1_ptUncorrected);
  fChain->SetBranchAddress("mZZ_type2_ptUncorrected",mZZ_type2_ptUncorrected);
  fChain->SetBranchAddress("mZZ_type3_ptUncorrected",mZZ_type3_ptUncorrected);
  fChain->SetBranchAddress("mZZ_type4_ptUncorrected",mZZ_type4_ptUncorrected);
  fChain->SetBranchAddress("pz_type0",pz_type0);
  fChain->SetBranchAddress("pz_type1",pz_type1);
  fChain->SetBranchAddress("pz_type2",pz_type2);
  fChain->SetBranchAddress("pz_type3",pz_type3);
  fChain->SetBranchAddress("pz_type4",pz_type4);
  fChain->SetBranchAddress("pt_neutrino",pt_neutrino);
  fChain->SetBranchAddress("pt_neutrino_corrected",pt_neutrino_corrected);
  fChain->SetBranchAddress("TOBTECjetsId",TOBTECjetsId);
}

EDBRHistoMaker::EDBRHistoMaker(TTree* tree, 
			       bool wantElectrons,
			       bool wantMuons,
			       bool wantSideband,
			       bool wantSignal,
			       bool wantFullRange,
			       int  wantNXJets,
			       bool isZZchannel){
  fChain = 0;

  /*
  // Definition of regions
  sidebandVHMassLow_  =  0.0;  // GeV
  sidebandVHMassHigh_ =  70.0; // GeV
  signalVHMassLow_    =  70.0; // GeV
  signalVHMassHigh_   = 105.0; // GeV
  */

  // Which category do we want to analyze?
  wantElectrons_ = wantElectrons;
  wantMuons_ = wantMuons;
  wantSideband_ = wantSideband;
  wantSignal_ = wantSignal;
  wantFullRange_ = wantFullRange;
  wantNXJets_ = wantNXJets;
  isZZchannel_ =isZZchannel;

  debug_ = true;
  Init(tree);
  createAllHistos();
  printAllHistos();
}

EDBRHistoMaker::~EDBRHistoMaker() {
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}                  

Int_t EDBRHistoMaker::GetEntry(Long64_t entry) {
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t EDBRHistoMaker::LoadTree(Long64_t entry) {
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
  }
  return centry;
}

//-------------------------
// Infrastructure functions
//-------------------------

void EDBRHistoMaker::createAllHistos() {

  /// This part substitutes the big arrays that used to be 
  /// in the beginning of this file.
  /// Much simpler to create histos now: just add them to
  /// hs with hs.setHisto(name,nbins,min,max);
  hs.setHisto("nCands",30,-0.5,29.5);
  hs.setHisto("cosThetaStar",100,-1.15,1.15);
  hs.setHisto("cosTheta1",100,-1.15,1.15);
  hs.setHisto("cosTheta2",100,-1.15,1.15);
  hs.setHisto("phi",100,-3.7,3.7);
  hs.setHisto("phiStar1",100,-3.7,3.7);
  hs.setHisto("ptlep1",120,0,1200);
  hs.setHisto("ptlep2",60,0,600);
  hs.setHisto("ptjet1",50,0,500);
  hs.setHisto("ptjet2",50,0,500);
  hs.setHisto("ptZll",76,80,1600); // 20 GeV bins
  hs.setHisto("ptZjj",76,80,1600); // 20 GeV bins
  hs.setHisto("yZll",28,-2.8,2.8);
  hs.setHisto("yZjj",28,-2.8,2.8);
  hs.setHisto("phiZll",100,-3.7,3.7);
  hs.setHisto("phiZjj",100,-3.7,3.7);
  hs.setHisto("etalep1",25,-251,2.5);
  hs.setHisto("etalep2",25,-2.5,2.5);
  hs.setHisto("etajet1",25,-2.5,2.5);
  hs.setHisto("etajet2",25,-2.5,2.5);
  hs.setHisto("philep1",100,-3.7,3.7);
  hs.setHisto("philep2",100,-3.7,3.7);
  hs.setHisto("phijet1",100,-3.7,3.7);
  hs.setHisto("phijet2",100,-3.7,3.7);
  hs.setHisto("lep",100,0,1);
  hs.setHisto("region",100,0,1);
  hs.setHisto("mZZFixedBinning",56,200,3000); // 50 GeV bins...
  //but to have the signal spread around 4 bins maybe we want 25 GeV bins?
  hs.setHisto("mZZNoKinFit",81,175,2200);

  //  const int nBinsMZZ=30;
  //const double binsMZZ[nBinsMZZ]={500,525,550,575,600,625,650,675,700,725,
  //			  750,775,800,825, 850,875,900,925,950,975,
  //			  1000,1025,1050,1075,1100,1125,1150,1175,1200,1225};
  
  const int nBinsMZZ=36;
  const double binsMZZ[nBinsMZZ]={500,550,600,650,700,750,800,850,900,950,
				  1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,
				  1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,
				  2500,2600,2700,2800,2900,3000};

  //  const int nBinsMZZ=19;
  //const double binsMZZ[nBinsMZZ]={500,550,600,650,700,750,800,850,900,950,
  //			  1000,1050,1100,1150,1200,1250,1300,1350,1400};

  hmzzNEW=new TH1D("h_mZZ","M_{ZZ} (variable bin size); M_{ZZ} [GeV]; #evetns",nBinsMZZ-1,binsMZZ);



  hs.setHisto("ptmzz",35,0,350);
  hs.setHisto("ptmzzNoKinFit",35,0,350);
  hs.setHisto("mLL",40,70,110);
  hs.setHisto("mJJ",20,40,140);
  hs.setHisto("prunedmass",28,0,140);   // 5 GeV bins
  hs.setHisto("mJJNoKinFit",24,40,160); // 5 GeV bins
  hs.setHisto("met",35,0,600);//35      0       600
  hs.setHisto("metSign",20,0,10);
  //hs.setHisto("nBTags",100,-2.2,0);
  hs.setHisto("deltaREDBR",100,0,4);
  hs.setHisto("deltaRleplep",100,0,4);
  hs.setHisto("deltaRjetjet",100,0,4);
  hs.setHisto("betajet1",100,0,1.1);
  hs.setHisto("nXjets",6,-0.5,5.5);
  hs.setHisto("mdrop",35,0.1,1.15);
  hs.setHisto("nsubj21",35,0.2,1.1);
  //hs.setHisto("nsubj32",100,-1080,100);
  //hs.setHisto("tau1",100,-1080,100);
  //hs.setHisto("tau2",100,-1080,100);
  hs.setHisto("qjet",35,0,1);//35     0        1
  hs.setHisto("isomu1mod",100,0,0.2);
  hs.setHisto("isomu2mod",100,0,0.2);
  hs.setHisto("isoele1calo",100,0,0.5);
  hs.setHisto("isoele2calo",100,0,0.5);
  hs.setHisto("isoele1trk",100,0,10);
  hs.setHisto("isoele2trk",100,0,10);
  //hs.setHisto("LD",100,-101,-97);
  //hs.setHisto("q1fl",4,-101,-97);
  //hs.setHisto("q2fl",4,-101,-97);
  hs.setHisto("MCmatch",100,-1.2,1.2);
  hs.setHisto("nVtx",40,-0.5,39.5);
  hs.setHisto("nJets",10,0.5,10.5);
  //hs.setHisto("nPU",2,0,1);
  hs.setHisto("HLTweight",40,0,4);
  hs.setHisto("BTagWeight",40,0,4);
  hs.setHisto("PUweight",100,0,10);
  //hs.setHisto("PUweight2012A",100,0,10);
  //hs.setHisto("PUweight2012B",100,0,10);
  hs.setHisto("LumiWeight",100,0,0.1);
  hs.setHisto("GenWeight",100,0,10);
  //hs.setHisto("weight",100,0,10);
  //hs.setHisto("weight2012A",100,0,10);
  //hs.setHisto("weight2012B",100,0,10);
  hs.setHisto("event",100,0,1e+09);
  hs.setHisto("run",100,190000,210000);
  hs.setHisto("ls",100,0,10000);
  hs.setHisto("nVL",10,0,10);
  hs.setHisto("VBFTag",2,0,2);
  hs.setHisto("VBFmJJ",100,0,1000);
  hs.setHisto("VBFdeltaEta",100,0,10);
  hs.setHisto("nLooseEle",10,0,10);
  hs.setHisto("nLooseMu",10,0,10);
  hs.setHisto("mt",80,0,130);
  hs.setHisto("nbtagsL",5,0,5);
  hs.setHisto("nbtagsM",5,0,5);
  hs.setHisto("nbtagsT",5,0,5);
  hs.setHisto("nbtagscleanL",5,0,5);
  hs.setHisto("nbtagscleanM",5,0,5);
  hs.setHisto("nbtagscleanT",5,0,5);
  hs.setHisto("deltaR_LJ",40,0,10);
  hs.setHisto("deltaPhi_JMET",40,0,4);
  hs.setHisto("deltaPhi_JWL",40,0,4);
  hs.setHisto("deltaPhi_LJ",40,0,4);
  hs.setHisto("nAK5jets",10,0,10);
  hs.setHisto("deltaPhi_LMET",40,0,4);
  hs.setHisto("ptLoverJ",50,0,1.5);

  hs.setHisto("eleid_sigmaIetaIeta",50,0,0.04);
  hs.setHisto("eleid_deltaPhiSuperClusterTrackAtVtx",50,-0.07,0.07);
  hs.setHisto("eleid_deltaEtaSuperClusterTrackAtVtx",50,-0.008,0.008);
  hs.setHisto("eleid_hadronicOverEm",50,0,0.06);
  hs.setHisto("eleid_numberOfHits",5,0,3);
  hs.setHisto("eleid_dxy",50,-0.1,0.1);
  hs.setHisto("eleid_dz",50,-0.4,0.4);
  hs.setHisto("eleid_ecalDriven",3,0,3);
  hs.setHisto("eleid_convDist",50,-1,1);
  hs.setHisto("eleid_convDcot",50,-1,1);
  hs.setHisto("eleid_isConv",3,0,3);
  hs.setHisto("eleid_passConversionVeto",3,0,3);
  hs.setHisto("eleid_passConversionVeto_old",3,0,3);
  hs.setHisto("eleid_numberOfLostHits",3,0,3);  
  hs.setHisto("eleid_e1x5",100,0,2000); 
  hs.setHisto("eleid_e2x5Max",100,0,2000); 
  hs.setHisto("eleid_e5x5",100,0,2000); 
  hs.setHisto("eleid_e1x5Over5x5",50,0.2,1.4); 
  hs.setHisto("eleid_e2x5MaxOver5x5",50,0.2,1.4);

  hs.setHisto("mZZ_type0",50,200,3000);
  hs.setHisto("mZZ_type1",50,200,3000);
  hs.setHisto("mZZ_type2",50,200,3000);
  hs.setHisto("mZZ_type3",50,200,3000);
  hs.setHisto("mZZ_type4",50,200,3000);
  hs.setHisto("mZZ_type0_ptUncorrected",50,200,3000);
  hs.setHisto("mZZ_type1_ptUncorrected",50,200,3000);
  hs.setHisto("mZZ_type2_ptUncorrected",50,200,3000);
  hs.setHisto("mZZ_type3_ptUncorrected",50,200,3000);
  hs.setHisto("mZZ_type4_ptUncorrected",50,200,3000);
  hs.setHisto("pz_type0",60,-500,500);
  hs.setHisto("pz_type1",60,-500,500);
  hs.setHisto("pz_type2",60,-500,500);
  hs.setHisto("pz_type3",60,-500,500);
  hs.setHisto("pz_type4",60,-500,500);
  hs.setHisto("pt_neutrino",35,0,500);
  hs.setHisto("pt_neutrino_corrected",35,0,500);
  hs.setHisto("TOBTECjetsId",2,0,2);

  if(isZZchannel_==0)//WW channel, use special binning in sync with FNAL
    {
      hs.setHisto("ptlep1",50,0,600);
      hs.setHisto("etalep1",25,-2.1,2.1);	    
      hs.setHisto("philep1",30,-3.14,3.14);
      hs.setHisto("ptZll",40,200,600);
      hs.setHisto("mt",50,0,150);
      hs.setHisto("met",35,0,500);
      hs.setHisto("philep2",30,-3.14,3.14);
      hs.setHisto("nVtx",40,0,40);
      hs.setHisto("ptZjj",40,200,600);
      hs.setHisto("ptjet1",40,200,600);
      hs.setHisto("etajet1",30,-2.5,2.5);
      hs.setHisto("phijet1",40,-3.14,3.14);
      hs.setHisto("mJJ",28,0,140);
      hs.setHisto("prunedmass",28,0,140);
      hs.setHisto("mJJNoKinFit",28,0,140);
      hs.setHisto("mdrop",35,0.1,1.15);
      hs.setHisto("nsubj21",35,0.1,1.);
      hs.setHisto("qjet",35,0,1);
    }



  char buffer[256];
  char buffer2[256];

  nVars = hs.vars.size();

  for(int i = 0; i!= nVars; ++i) {
    sprintf(buffer,"h_%s",hs.vars[i].c_str());
    sprintf(buffer2,"%s;%s;Number of events;",hs.vars[i].c_str(),hs.vars[i].c_str());
    TH1D* histogram = new TH1D(buffer,
			       buffer2,
			       hs.nBins[i],
			       hs.minBin[i],
			       hs.maxBin[i]);
    histogram->SetDirectory(0);
    histogram->Sumw2();
    theHistograms[hs.vars[i]] = histogram;
  }

  hmjmzz=new TH2D("h_mj_vs_mzz","Correlation plot M_{J} vs M_{ZZ}; M_{ZZ} [GeV]; M_{J} [GeV]",240,200,2600,20,35.0,135.0);


}

void EDBRHistoMaker::printAllHistos() {
  printf("We have %i histograms \n",int(theHistograms.size()));
  typedef std::map<std::string, TH1D*>::iterator it_type;
  for(it_type iterator = theHistograms.begin(); iterator != theHistograms.end(); iterator++) {
    //iterator->second->Print();
    // Repeat if you also want to iterate through the second map.
  }
}

void EDBRHistoMaker::saveAllHistos(std::string outFileName) {

  TFile* outFile = TFile::Open(outFileName.c_str(),"RECREATE");

  for(int i = 0; i!=nVars; ++i) {
    std::string name = hs.vars[i];
    const TH1D* thisHisto = this->theHistograms[name];
    thisHisto->Write();
  }
  hmjmzz->Write();
  hmzzNEW->Write();
  outFile->Close();
}

//------------------
// Physics functions
//------------------

bool EDBRHistoMaker::eventPassesFlavorCut(int i){
  bool passesFlavour = ((lep[i] == 0 and wantElectrons_) or
			(lep[i] == 1 and wantMuons_));

  return passesFlavour;
}


bool EDBRHistoMaker::eventPassesLep1PtCut(int i, double ptlep1_threshold) {

  bool pass = false;

  pass = ( ptlep1 [i] > ptlep1_threshold);

  return pass;

}

bool EDBRHistoMaker::eventPassesLeptonicZPtCut(int i, double ptZll_threshold){

  bool passesLeptonicZPt = false;

  passesLeptonicZPt = (ptZll[i] > ptZll_threshold);

  return passesLeptonicZPt;
}

// These functions have to aware if we're in the one jet / 
// two jet case.
bool EDBRHistoMaker::eventInSidebandRegion(int i){

  bool isInSideband = false;

  isInSideband = (region[i] == 0);

  return isInSideband;
}

bool EDBRHistoMaker::eventInSignalRegion(int i){

  bool isInSignal = false;

  isInSignal = (region[i] == 1);

  return isInSignal;
}

bool EDBRHistoMaker::eventPassesRegionCut(int i){
  bool isInSideband = eventInSidebandRegion(i);
  bool isInSignal   = eventInSignalRegion(i);
  bool passesRegion = ((isInSideband and wantSideband_) or
		       (isInSignal and wantSignal_)) ;
  if(wantFullRange_) passesRegion=1;
  return passesRegion;
}

bool  EDBRHistoMaker::eventPassesNXJetCut(int i){
  if(wantNXJets_==-1)
    return true;
  bool passesNXJ = (nXjets[i] == wantNXJets_);
  return passesNXJ;
}

bool EDBRHistoMaker::eventPassesVBFCut(int i){

  bool vbfFlag = false;

  vbfFlag = (VBFTag[i] == 0);

  return vbfFlag;
}



bool EDBRHistoMaker::eventPassesCut(int i, double ptZll_threshold, double ptlep1_threshold ) {

  bool passesFlavour = eventPassesFlavorCut(i);
  bool passesRegion  = eventPassesRegionCut(i);
  bool passesNXJet   = eventPassesNXJetCut(i);
  bool passesLeptonicZPt = eventPassesLeptonicZPtCut(i, ptZll_threshold);
  bool passesLep1Pt  = eventPassesLep1PtCut(i, ptlep1_threshold);
  bool passesVBF     = eventPassesVBFCut(i);
  bool passesVTag = (nXjets[i]!=1) || (vTagPurity[i]==0);//==1 -> HighPurity
  bool passesMZZ=mZZ[i]>=500.0&&mZZ[i]<1400.0;


  
  /* printf("passesFlavour: %i\n",passesFlavour); */
/*   printf("passesRegion: %i\n",passesRegion); */
/*   printf("passesNXJet: %i\n",passesNXJet); */
/*   printf("passesLeptonicZPt: %i\n",passesLeptonicZPt); */
/*   printf("passesVTag: %i\n",passesVTag); */
  
  bool result = 
    passesFlavour &&
    passesRegion &&
    passesNXJet && 
    passesLep1Pt &&
    passesLeptonicZPt;// && passesVTag;//&&passesMZZ;



  return result;//&&passesVTag&&passesMZZ;
}

///----------------------------------------------------------------
/// This is the important function, the loop over all events.
/// Here we fill the histograms according to cuts, weights,
/// and can also filter out events on an individual basis.
///----------------------------------------------------------------
void EDBRHistoMaker::Loop(std::string outFileName){

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if(jentry==0){
      //      printf("Entry number %i...\n",(int)jentry);
      float genLumi=1.0/LumiWeight;
      if(genLumi==1.0)genLumi=-1.0;
      if(genLumi!=-1.0) std::cout<<"Lumi of this sample: "<<genLumi <<"  /pb"<<std::endl;
      else std::cout<<"Lumi of this sample: xxx  /pb (dummy for data)"<<std::endl;

    }
    // We calculate a weight here.
    double actualWeight = weight;
    //double actualWeight = HLTweight*PUweight*LumiWeight*GenWeight;
    //double actualWeight = PUweight*LumiWeight*GenWeight;
    if(setUnitaryWeights_) {
      if(jentry==0)printf("Unitary weights set!\n");
      actualWeight=1.0;
    }
    //printf("jentry is %i\n",(int)jentry);
    // We get the histogram from the map by string and fill it.
    // We could wrap all the fills in the this->eventPassesCut()
    // to fill histograms only for the events which pass a given
    // cut (Sideband / SignalRegion, Muon / Electron, 
    // Single / Double jet ...) 

    vector<double> ptZ;
    //printf("nCands == %i\n",nCands);
    for (int iptz=0;iptz<nCands;iptz++)
      {   
	//printf("Okay, going to call check()\n");
	if(check(ptZll[iptz],&ptZ)==1){
	  ptZ.push_back(ptZll[iptz]);
	}   
      }
    int wnum = ptZ.size();


    bool filled = 0;
    for(int ivec=0;ivec<nCands;ivec++){
      //  if(mZZ[ivec]>2400.0)cout<<"Found one event with MZZ="<<mZZ[ivec]<<endl;

      // Remember: bool eventPassesCut(int i, double ptZll_threshold, double ptlep1_threshold );
      if(eventPassesCut(ivec, 80, 20)){
	//if(mZZ[ivec]>2400.0)cout<<"Found (2) one event with MZZ="<<mZZ[ivec]<<endl;
	//calculate "deltaPhi_JMET","deltaPhi_JWL","deltaR_LJ"
	double deltaR_LJ = deltaR(etalep1[ivec],philep1[ivec],etajet1[ivec],phijet1[ivec]);
	double deltaPhi_LJ   = deltaPhi(phijet1[ivec],philep1[ivec]);
	double deltaPhi_JMET = deltaPhi(phijet1[ivec],philep2[ivec]);
	double deltaPhi_JWL  = deltaPhi(phijet1[ivec],phiZll[ivec]); 
	double deltaPhi_LMET = deltaPhi(philep1[ivec],philep2[ivec]);
	double ptLoverJ      = ptlep1[ivec]/ptjet1[ivec];

	double eleid_passConversionVeto_old=0;
	if(fabs(eleid_convDist[ivec])>0.02 || fabs(eleid_convDcot[ivec])>0.02)eleid_passConversionVeto_old=1;

	if(isZZchannel_==0)//WW channel, veto second loose lepton
	  {
	    if( nLooseEle+nLooseMu==1 );//loose lepton veto selection
	    else continue;	

	    //if(eventPassesCut(ivec, 200, 20));//cut on WL pt
	    //else continue;

	    //cut from fermilab
	    if(deltaR_LJ>1.57 && deltaPhi_JMET>2. && deltaPhi_JWL>2.);
	    else continue;
					
	    //mt cut 30
	    //if(mt[ivec]>30);
	    //else continue;

	    //cut on mjj
	    //if(mJJNoKinFit[ivec]<50)continue;	

	    //if(ptZjj[ivec]>225;
	    //else continue;

	    //conversion veto
	    //if(eleid_passConversionVeto[ivec]==1);
	    //else continue;
	    //if(eleid_passConversionVeto_old==1);
	    //else continue;
	    //if(eleid_numberOfLostHits[ivec]==0);
	    //else continue;

	    //eta cut on electron: using only barrel
	    //if(fabs(etalep1[ivec])<1.442);
	    //else continue;

	    if(wantMuons_||(wantElectrons_&&met>80));
	    else continue;

	    //b veto cut
	    if(nbtagsM[ivec]==0) ;
	    else continue;

	    //b cut - ttbar control region - to sync we use clean M
	    //if(nbtagscleanM[ivec]>=1) ;
	    //else continue;

	    //nsubjettiness HP
	    //if(nsubj21[ivec]<0.5) ;
	    //else continue;

	    //nsubjettiness LP
	    //if(nsubj21[ivec]>0.5&&nsubj21[ivec]<0.75);
	    //else continue;

	    //-- END of ALL CUTS --

	    //Printout for debugging	
										       		       
	    if(mZZ_type2_ptUncorrected[ivec]>1800)
	      {
		//RunNumber:LumiSection:EvtNumber
		cout << run << ":" << ls << ":" << event << endl;
					    
		cout << event             << " * " << run           << " * " <<mZZ_type2_ptUncorrected[ivec] << " * " 
		     << ptlep1[ivec]      << " * " << etalep1[ivec] << " * " << philep1[ivec]   << " * "   
		     << met               << " * " << philep2[ivec] << " * " << mt[ivec]        << " * " 
		     << ptZll[ivec]                                                         << " * "
		     << ptjet1[ivec]      << " * " << etajet1[ivec] << " * " << phijet1[ivec]   << " * " 
		     << mJJNoKinFit[ivec] << " * " << nsubj21[ivec] << endl << endl; 					    
	      }
					  
					
				       
					
	  }

	/// Here go the histograms that must be filled only once per event.
	if(filled==0)
	  {   
	    (theHistograms["nVL"])->Fill(wnum,actualWeight);
	    (theHistograms["nCands"])->Fill(nCands,actualWeight);
					
	    (theHistograms["HLTweight"])->Fill(PUweight);
	    (theHistograms["BTagWeight"])->Fill(BTagWeight);
	    (theHistograms["PUweight"])->Fill(PUweight);
	    (theHistograms["LumiWeight"])->Fill(LumiWeight);
	    (theHistograms["GenWeight"])->Fill(GenWeight);

	    (theHistograms["nLooseEle"])->Fill(nLooseEle,actualWeight);
	    (theHistograms["nLooseMu"])->Fill(nLooseMu,actualWeight);

	    (theHistograms["nVtx"])->Fill(nVtx,actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nJets"])->Fill(nJets,actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nAK5jets"])->Fill(nAK5jets,actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["met"])->Fill(met,actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["metSign"])->Fill(metSign,actualWeight);//printf("line number %i\n",__LINE__);

	    (theHistograms["nbtagsL"])->Fill(nbtagsL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nbtagsM"])->Fill(nbtagsM[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nbtagsT"])->Fill(nbtagsT[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nbtagscleanL"])->Fill(nbtagscleanL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nbtagscleanM"])->Fill(nbtagscleanM[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	    (theHistograms["nbtagscleanT"])->Fill(nbtagscleanT[ivec],actualWeight);//printf("line number %i\n",__LINE__);

	    filled =1; 
	  }

	(theHistograms["deltaR_LJ"])->Fill(deltaR_LJ,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaPhi_JMET"])->Fill(deltaPhi_JMET,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaPhi_LMET"])->Fill(deltaPhi_LMET,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaPhi_JWL"])->Fill(deltaPhi_JWL,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaPhi_LJ"])->Fill(deltaPhi_LJ,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptLoverJ"])->Fill(ptLoverJ,actualWeight);//printf("line number %i\n",__LINE__);

	(theHistograms["ptlep1"])->Fill(ptlep1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptlep2"])->Fill(ptlep2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptjet1"])->Fill(ptjet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptjet2"])->Fill(ptjet2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptZll"])->Fill(ptZll[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["ptZjj"])->Fill(ptZjj[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["yZll"])->Fill(yZll[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["yZjj"])->Fill(yZjj[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mLL"])->Fill(mLL[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mJJ"])->Fill(mJJ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZFixedBinning"])->Fill(mZZ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["prunedmass"])->Fill(prunedmass[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mdrop"])->Fill(mdrop[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["qjet"])->Fill(qjet[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mJJNoKinFit"])->Fill(mJJNoKinFit[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["nsubj21"])->Fill(nsubj21[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["nXjets"])->Fill(nXjets[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["betajet1"])->Fill(betajet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isomu1mod"])->Fill(isomu1mod[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isomu2mod"])->Fill(isomu2mod[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isoele1trk"])->Fill(isoele1trk[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isoele2trk"])->Fill(isoele2trk[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isoele1calo"])->Fill(isoele1calo[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["isoele2calo"])->Fill(isoele2calo[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["etalep1"])->Fill(etalep1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["etalep2"])->Fill(etalep2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["etajet1"])->Fill(etajet1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["etajet2"])->Fill(etajet2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaREDBR"])->Fill(deltaREDBR[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaRleplep"])->Fill(deltaRleplep[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["deltaRjetjet"])->Fill(deltaRjetjet[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["VBFTag"])->Fill(VBFTag[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["VBFmJJ"])->Fill(VBFmJJ[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["VBFdeltaEta"])->Fill(VBFdeltaEta[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mt"])->Fill(mt[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["lep"])->Fill(lep[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_sigmaIetaIeta"])->Fill(eleid_sigmaIetaIeta[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_deltaPhiSuperClusterTrackAtVtx"])->Fill(eleid_deltaPhiSuperClusterTrackAtVtx[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_deltaEtaSuperClusterTrackAtVtx"])->Fill(eleid_deltaEtaSuperClusterTrackAtVtx[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_hadronicOverEm"])->Fill(eleid_hadronicOverEm[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_numberOfHits"])->Fill(eleid_numberOfHits[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_dxy"])->Fill(eleid_dxy[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_dz"])->Fill(eleid_dz[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_ecalDriven"])->Fill(eleid_ecalDriven[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_convDist"])->Fill(eleid_convDist[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_convDcot"])->Fill(eleid_convDcot[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_isConv"])->Fill(eleid_isConv[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_passConversionVeto"])->Fill(eleid_passConversionVeto[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_passConversionVeto_old"])->Fill(eleid_passConversionVeto_old,actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_numberOfLostHits"])->Fill(eleid_numberOfLostHits[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_e1x5"])->Fill(eleid_e1x5[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_e2x5Max"])->Fill(eleid_e2x5Max[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_e5x5"])->Fill(eleid_e5x5[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_e1x5Over5x5"])->Fill(eleid_e1x5Over5x5[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["eleid_e2x5MaxOver5x5"])->Fill(eleid_e2x5MaxOver5x5[ivec],actualWeight);//printf("line number %i\n",__LINE__);

	(theHistograms["mZZ_type0"])->Fill(mZZ_type0[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type1"])->Fill(mZZ_type1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type2"])->Fill(mZZ_type2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type3"])->Fill(mZZ_type3[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type4"])->Fill(mZZ_type4[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type0_ptUncorrected"])->Fill(mZZ_type0_ptUncorrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type1_ptUncorrected"])->Fill(mZZ_type1_ptUncorrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type2_ptUncorrected"])->Fill(mZZ_type2_ptUncorrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type3_ptUncorrected"])->Fill(mZZ_type3_ptUncorrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["mZZ_type4_ptUncorrected"])->Fill(mZZ_type4_ptUncorrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pz_type0"])->Fill(pz_type0[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pz_type1"])->Fill(pz_type1[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pz_type2"])->Fill(pz_type2[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pz_type3"])->Fill(pz_type3[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pz_type4"])->Fill(pz_type4[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pt_neutrino"])->Fill(pt_neutrino[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["pt_neutrino_corrected"])->Fill(pt_neutrino_corrected[ivec],actualWeight);//printf("line number %i\n",__LINE__);
	(theHistograms["TOBTECjetsId"])->Fill(TOBTECjetsId[ivec],actualWeight);//printf("line number %i\n",__LINE__);
				
	/// Also, once per event we check if the event is interesting...
	/// What's interesting?
	/// * nCands > 10
	/// * mZZ in the [1900,2100] range
	if(false) {
	  if(nCands>9)
	    printf("High nCands events: nCands = %i, Run = %i, Lumi = %i, Event = %i\n",nCands,run,ls,event);
	  if(mZZ[ivec] > 1900 and mZZ[ivec] < 2100)
	    printf("Event around 2TeV: mZZ = %g, Run = %i, Lumi = %i, Event = %i\n",mZZ[ivec],run,ls,event);
	}

	// (theHistograms[""])->Fill([ivec],actualWeight);
	hmjmzz->Fill(mZZ[ivec],mJJ[ivec],actualWeight);
	hmzzNEW->Fill(mZZ[ivec],actualWeight);

      }//end if eventPassesCut

    }//end loop over nCands
  }//end loop over entries

  if(isZZchannel_==0)//WW channel, change the names, which will be the plot lable
    {
      (theHistograms["ptlep1"])->SetName("h_ptlepton");
      //(theHistograms["ptlep2"])->SetName("h_ptneutrino");
      (theHistograms["ptZll"])->SetName("h_ptWL");
      (theHistograms["ptZjj"])->SetName("h_ptWjj");
      (theHistograms["yZll"])->SetName("h_yWL");
      (theHistograms["yZjj"])->SetName("h_yWjj");
      (theHistograms["mLL"])->SetName("h_mWL");
      (theHistograms["mZZ"])->SetName("h_mWW");
      (theHistograms["etalep1"])->SetName("h_etalep");
      (theHistograms["etalep2"])->SetName("h_etaneu");
    }

  std::cout<<"From makeHisto: the histo with #vtx has "<<(theHistograms["nVtx"])->GetEntries()<<" entries"<<std::endl;
  this->saveAllHistos(outFileName);
}
